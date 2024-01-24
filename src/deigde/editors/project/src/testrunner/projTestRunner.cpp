/* 
 * Drag[en]gine IGDE Project Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#include <dragengine/app/deOSWindows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/select.h>
#endif

#include "projTestRunner.h"
#include "profile/projTRProfile.h"
#include "profile/projTRPParameter.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../gui/projWindowMain.h"
#include "../loadsave/projLauncherEngineConfig.h"

#include "../../testrun/projTestRunConstants.h"

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Definitions
////////////////

#define LOGSOURCE "Test-Runner"



// Class projTestRunner
/////////////////////////

// Constructor, destructor
////////////////////////////

projTestRunner::projTestRunner( projWindowMain &windowMain ) :
pWindowMain( windowMain ),
pProfile( NULL ),
pLauncherProfile( NULL ),

#ifdef OS_W32
pPipeIn( INVALID_HANDLE_VALUE ),
pPipeOut( INVALID_HANDLE_VALUE ),
pProcessHandle( INVALID_HANDLE_VALUE ),
pThreadHandle( INVALID_HANDLE_VALUE ),
pProcessID( 0 ),
pThreadID( 0 )

#else
pPipeIn( 0 ),
pPipeOut( 0 ),
pProcessID( 0 )
#endif
{
}

projTestRunner::~projTestRunner(){
	Kill();
}



// Management
///////////////

void projTestRunner::LoadEngineConfiguration(){
	// clear old configuration
	pLauncherProfiles.RemoveAll();
	
	// find path of engine config file
	decPath pathConfigUser;
	const char *value;
	
	#ifdef OS_W32
	pathConfigUser.SetFromNative( deOSWindows::ParseNativePath( "@RoamingAppData" ) );
	pathConfigUser.AddComponent( "DELaunchers" );
	pathConfigUser.AddComponent( "Config" );
	
	#elif defined( OS_BEOS )
	pathConfigUser.SetFromNative( "/boot/home/config/settings/delauncher" );
	
	#else
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	value = getenv( "HOME" );
	if( value ){
		pathConfigUser.SetFromNative( value );
		
	}else{
		value = getenv( "USER" );
		
		if( value ){
			pathConfigUser.SetFromNative( "/home" );
			pathConfigUser.AddComponent( value );
			
		}else{
			value = getenv( "LOGUSER" );
			
			if( value ){
				pathConfigUser.SetFromNative( "/home" );
				pathConfigUser.AddComponent( value );
			}
		}
	}
	
	if( pathConfigUser.GetComponentCount() > 0 ){
		pathConfigUser.AddComponent( ".config" );
		pathConfigUser.AddComponent( "delauncher" );
	}
	#endif
	
	value = getenv( "DELAUNCHER_USER_CONFIG" );
	if( value ){
		pathConfigUser.SetFromNative( value );
	}
	#ifdef OS_W32
	pathConfigUser.SetFromNative( deOSWindows::ParseNativePath( pathConfigUser.GetPathNative() ) );
	#endif
	
	// engine file
	pathConfigUser.AddComponent( "launcher.xml" );
	decBaseFileReaderReference reader;
	
	try{
		reader.TakeOver( new decDiskFileReader( pathConfigUser.GetPathNative() ) );
		projLauncherEngineConfig( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
		pWindowMain.GetLogger()->LogInfo( LOGSOURCE,
			"Failed loading launcher engine configuration. Using defaults" );
		pLauncherProfiles.RemoveAll();
		
		// TODO add default profile
	}
}

void projTestRunner::SetDefaultLauncherProfileName( const char *name ){
	pDefaultLauncherProfileName = name;
}

bool projTestRunner::IsRunning(){
	// windows
	#ifdef OS_W32
	if( pProcessHandle == INVALID_HANDLE_VALUE ){
		return false; // not running
	}
	
	DWORD exitCode;
	if( GetExitCodeProcess( pProcessHandle, &exitCode ) ){
		if( exitCode == STILL_ACTIVE ){
			return true; // still running
			
		}else{
			pWindowMain.GetLogger()->LogInfoFormat( LOGSOURCE,
				"Test-Runner stopped running with exit code %d", ( int )exitCode );

			pProcessHandle = INVALID_HANDLE_VALUE;
			return false; // process stopped
		}
		
	}else{
		// query failed. consider process still running for the time being
		return true;
	}
	
	// unix
	#else
	if( pProcessID == 0 ){
		return false; // not running
	}
	
	const pid_t result = waitpid( pProcessID, NULL, WNOHANG ); 
	
	if( result == 0 ){
		return true; // process still running
		
	}else if( result == pProcessID ){
		pProcessID = 0;
		return false; // process stopped
		
	}else{
		// something went wrong querying the status (result == -1).
		// consider process still running for the time being
		return true;
	}
	#endif
}

void projTestRunner::Start( projProfile *profile, projTRProfile *launcherProfile ){
	if( ! profile ){
		DETHROW( deeInvalidParam );
	}
	
	if( IsRunning() ){
		return;
	}
	
	pProfile = profile;
	pLauncherProfile = launcherProfile;
	
	pWindowMain.GetLogger()->LogInfoFormat( LOGSOURCE,
		"Launching Test-Runner using profile '%s'...", profile->GetName().GetString() );
	
	#ifdef OS_W32
	if( pProcessHandle == INVALID_HANDLE_VALUE ){
		HANDLE pipesInRead = INVALID_HANDLE_VALUE;
		HANDLE pipesInWrite = INVALID_HANDLE_VALUE;
		HANDLE pipesOutRead = INVALID_HANDLE_VALUE;
		HANDLE pipesOutWrite = INVALID_HANDLE_VALUE;
		SECURITY_ATTRIBUTES secattr;
		
		decPath pathTestRunner( decPath::CreatePathNative( pWindowMain.GetEditorModule().GetEditorPathLib() ) );
		pathTestRunner.AddComponent( "testrunner.exe" );
		const decString strPathTestRunner( pathTestRunner.GetPathNative() );
		
		wchar_t widePath[ MAX_PATH ];
		deOSWindows::Utf8ToWide( strPathTestRunner, widePath, MAX_PATH );
		
		PROCESS_INFORMATION procInfo;
		STARTUPINFO startInfo;
		DWORD bytesWritten = 0;
		
		ZeroMemory( &procInfo, sizeof( procInfo ) );
		procInfo.hProcess = INVALID_HANDLE_VALUE;
		
		try{
			// create the pipes. the child process inherits the in-pipe read end
			// and the out-pipe write end. not inherited are the in-pipe write end
			// and the out-pipe read end.
			memset( &secattr, '\0', sizeof( secattr ) );
			secattr.nLength = sizeof( secattr );
			secattr.bInheritHandle = TRUE;
			secattr.lpSecurityDescriptor = NULL;
			
			if( ! CreatePipe( &pipesInRead, &pipesInWrite, &secattr, 0 ) ){
				DETHROW( deeInvalidAction );
			}
			if( ! CreatePipe( &pipesOutRead, &pipesOutWrite, &secattr, 0 ) ){
				DETHROW( deeInvalidAction );
			}
			
			if( ! SetHandleInformation( pipesInWrite, HANDLE_FLAG_INHERIT, 0 ) ){
				DETHROW( deeInvalidAction );
			}
			if( ! SetHandleInformation( pipesOutRead, HANDLE_FLAG_INHERIT, 0 ) ){
				DETHROW( deeInvalidAction );
			}
			
			// windows does not provide a simple way to make the pipe ends available to
			// the child process like fork does under posix. a typical hack consists of
			// replacing the stdin and stdout pipe of the child process before creating
			// it. to be more flexible though and to not cause too much troubles with
			// messing with the streams only the input stream is replaced and the other
			// pipes send using a write over the input pipe
			ZeroMemory( &startInfo, sizeof( startInfo ) );
			startInfo.cb = sizeof( startInfo );
			startInfo.hStdError = GetStdHandle( STD_ERROR_HANDLE );
			startInfo.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
			startInfo.hStdInput = pipesInRead;
			startInfo.dwFlags |= STARTF_USESTDHANDLES;
			
			if( ! CreateProcess( NULL, widePath, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInfo ) ){
				DETHROW( deeInvalidAction );
			}
			
			if( ! WriteFile( pipesInWrite, &pipesOutWrite, sizeof( pipesOutWrite ), &bytesWritten, NULL ) ){
				DETHROW( deeInvalidAction );
			}
			if( bytesWritten < sizeof( pipesOutWrite ) ){
				DETHROW( deeInvalidAction );
			}
			
			// close unused ends
			CloseHandle( pipesInRead );
			pipesInRead = INVALID_HANDLE_VALUE;
			CloseHandle( pipesOutWrite );
			pipesOutWrite = INVALID_HANDLE_VALUE;
			
		}catch( const deException &e ){
			if( procInfo.hProcess != INVALID_HANDLE_VALUE ){
				TerminateProcess( procInfo.hProcess, 0 );
				WaitForSingleObject( procInfo.hProcess, 5000 );
				CloseHandle( procInfo.hProcess );
				CloseHandle( procInfo.hThread );
			}
			if( pipesInRead != INVALID_HANDLE_VALUE ){
				CloseHandle( pipesInRead );
			}
			if( pipesInWrite != INVALID_HANDLE_VALUE ){
				CloseHandle( pipesInWrite );
			}
			if( pipesOutRead != INVALID_HANDLE_VALUE ){
				CloseHandle( pipesOutRead );
			}
			if( pipesOutWrite != INVALID_HANDLE_VALUE ){
				CloseHandle( pipesOutWrite );
			}
			
			pWindowMain.DisplayException( e );
			return;
		}
		
		pProcessHandle = procInfo.hProcess;
		pProcessID = procInfo.dwProcessId;
		pThreadHandle = procInfo.hThread;
		pThreadID = procInfo.dwThreadId;
		pPipeIn = pipesInWrite;
		pPipeOut = pipesOutRead;
	}
	
	#else
	// forking duplicates the memory and file handles. this has the unholy result that
	// pending file writes are also duplicated resulting in garbage in the files. to
	// avoid this problem either a separate process can be used like the one in the
	// windows case or a flush can be forced of all open files. we use the second
	// solution here
	fflush( NULL );
	
	if( ! pProcessID ){
		int pipesIn[ 2 ] = { 0, 0 };
		int pipesOut[ 2 ] = { 0, 0 };
		
		if( pipe( pipesIn ) || pipe( pipesOut ) ){
			if( pipesIn[ 0 ] ){
				close( pipesIn[ 0 ] );
			}
			if( pipesIn[ 1 ] ){
				close( pipesIn[ 1 ] );
			}
			if( pipesOut[ 0 ] ){
				close( pipesOut[ 0 ] );
			}
			if( pipesOut[ 1 ] ){
				close( pipesOut[ 1 ] );
			}
			// TODO LOG PROBLEM
			return;
		}
		
		pProcessID = fork();
		
		if( pProcessID == 0 ){
			close( pipesIn[ 1 ] );
			close( pipesOut[ 0 ] );
			
			char arg1[ 10 ];
			char arg2[ 10 ];
			
			snprintf( arg1, 10, "%d", pipesIn[ 0 ] );
			snprintf( arg2, 10, "%d", pipesOut[ 1 ] );
			/*
			if( write( STDIN_FILENO, &pipesIn[ 0 ], sizeof( pipesIn[ 0 ] ) ) != sizeof( pipesIn[ 0 ] ) ){
				printf( "FUCK1\n" );
				exit( 0 );
			}
			if( write( STDIN_FILENO, &pipesOut[ 1 ], sizeof( pipesOut[ 1 ] ) ) != sizeof( pipesOut[ 1 ] ) ){
				printf( "FUCK2\n" );
				exit( 0 );
			}
			*/
			
			decPath pathTestRunner( decPath::CreatePathNative( pWindowMain.GetEditorModule().GetEditorPathLib() ) );
			pathTestRunner.AddComponent( "testrunner" );
			const decString strPathTestRunner( pathTestRunner.GetPathNative() );
			execlp( strPathTestRunner.GetString(), "testrunner", arg1, arg2, NULL );
			// we should never get here or something went wrong
			exit( 0 );
		}
		
		close( pipesIn[ 0 ] );
		close( pipesOut[ 1 ] );
		
		pPipeIn = pipesIn[ 1 ];
		pPipeOut = pipesOut[ 0 ];
	}
	#endif
	
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Test-Runner launched" );

	// init log file for reading. has to be done before sending the launc parameters since
	// the log file path is determined in pInitLogFile
	pInitLogFile();
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Log-file prepared" );
	
	// send launch parameters
	pSendLaunchParameters();
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Run parameters send to Test-Runner" );
}

void projTestRunner::Stop(){
	if( ! IsRunning() ){
		return;
	}
	
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Stopping Test-Runner..." );

	#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		WaitForSingleObject( pProcessHandle, 5000 );
		
		if( pThreadHandle ){
			CloseHandle( pThreadHandle );
			pThreadHandle = INVALID_HANDLE_VALUE;
			pThreadID = 0;
		}
		
		if( pProcessHandle != INVALID_HANDLE_VALUE ){
			CloseHandle( pProcessHandle );
			pProcessHandle = INVALID_HANDLE_VALUE;
			pProcessID = 0;
		}
	}
	
	if( pPipeIn != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeIn );
		pPipeIn = INVALID_HANDLE_VALUE;
	}
	
	if( pPipeOut != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeOut );
		pPipeOut = INVALID_HANDLE_VALUE;
	}
	
	#else
	if( pProcessID ){
		waitpid( pProcessID, NULL, 0 );
		pProcessID = 0;
	}
	
	if( pPipeIn ){
		close( pPipeIn );
		pPipeIn = 0;
	}
	
	if( pPipeOut ){
		close( pPipeOut );
		pPipeOut = 0;
	}
	#endif
	
	pProfile = NULL;
	pLauncherProfile = NULL;

	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Test-Runner stopped" );
}

void projTestRunner::Kill(){
	if( ! IsRunning() ){
		return;
	}
	
	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Killing Test-Runner..." );

	#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		TerminateProcess( pProcessHandle, 0 );
		WaitForSingleObject( pProcessHandle, 5000 );
		
		if( pThreadHandle ){
			CloseHandle( pThreadHandle );
			pThreadHandle = INVALID_HANDLE_VALUE;
			pThreadID = 0;
		}
		
		if( pProcessHandle != INVALID_HANDLE_VALUE ){
			CloseHandle( pProcessHandle );
			pProcessHandle = INVALID_HANDLE_VALUE;
			pProcessID = 0;
		}
	}
	
	if( pPipeIn != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeIn );
		pPipeIn = INVALID_HANDLE_VALUE;
	}
	
	if( pPipeOut != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeOut );
		pPipeOut = INVALID_HANDLE_VALUE;
	}
	
	#else
	if( pProcessID ){
		kill( pProcessID, SIGKILL );
		waitpid( pProcessID, NULL, 0 );
		pProcessID = 0;
	}
	
	if( pPipeIn ){
		close( pPipeIn );
		pPipeIn = 0;
	}
	
	if( pPipeOut ){
		close( pPipeOut );
		pPipeOut = 0;
	}
	#endif
	
	pProfile = NULL;
	pLauncherProfile = NULL;

	pWindowMain.GetLogger()->LogInfo( LOGSOURCE, "Test-Runner killed" );
}



void projTestRunner::WriteUCharToPipe( int value ){
	if( value < 0 || value > 0xff ){
		DETHROW( deeInvalidParam );
	}
	const uint8_t uchar = ( uint8_t )value;
	WriteToPipe( &uchar, sizeof( uint8_t ) );
}

void projTestRunner::WriteUShortToPipe( int value ){
	if( value < 0 || value > 0xffff ){
		DETHROW( deeInvalidParam );
	}
	const uint16_t vushort = ( uint16_t )value;
	WriteToPipe( &vushort, sizeof( uint16_t ) );
}

void projTestRunner::WriteFloatToPipe( float value ){
	WriteToPipe( &value, sizeof( float ) );
}

void projTestRunner::WriteString16ToPipe( const char *string ){
	const int length = ( int )strlen( string );
	WriteUShortToPipe( length );
	WriteToPipe( string, length );
}

void projTestRunner::WriteToPipe( const void *data, int length ){
	if( length == 0 ){
		return;
	}
	
	#ifdef OS_W32
	DWORD bytesWritten = 0;
	
	if( ! WriteFile( pPipeIn, data, length, &bytesWritten, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	if( ( int )bytesWritten < length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( write( pPipeIn, data, length ) < length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}



int projTestRunner::ReadUCharFromPipe(){
	uint8_t uchar;
	ReadFromPipe( &uchar, sizeof( uint8_t ) );
	return uchar;
}

int projTestRunner::ReadUShortFromPipe(){
	uint16_t vushort;
	ReadFromPipe( &vushort, sizeof( uint16_t ) );
	return vushort;
}

float projTestRunner::ReadFloatFromPipe(){
	float value;
	ReadFromPipe( &value, sizeof( float ) );
	return value;
}

void projTestRunner::ReadString16FromPipe( decString &string ){
	const int length = ReadUShortFromPipe();
	string.Set( ' ', length );
	ReadFromPipe( ( char* )string.GetString(), length );
}

void projTestRunner::ReadFromPipe( void *data, int length ){
	if( length == 0 ){
		return;
	}
	
	#ifdef OS_W32
	DWORD bytesRead = 0;
	
	if( ! ReadFile( pPipeOut, data, length, &bytesRead, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	if( ( int )bytesRead < length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( read( pPipeOut, data, length ) != length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}



void projTestRunner::SendQuit(){
	#ifdef OS_W32
	if( pPipeIn == INVALID_HANDLE_VALUE ){
		DETHROW( deeInvalidParam );
	}
	#else
	if( ! pPipeIn ){
		DETHROW( deeInvalidParam );
	}
	#endif
	
	WriteUCharToPipe( projTestRunConstants::ecQuit );
}

decString projTestRunner::ReadNextLogData(){
	if( ! pLogFileReader ){
		return decString();
	}
	
	const int position = pLogFileReader->GetPosition();
	pLogFileReader->SetPositionEnd( 0 );
	const int end = pLogFileReader->GetPosition();
	
	decString content;
	
	if( end > position ){
		content.Set( ' ', end - position );
		pLogFileReader->SetPosition( position );
		pLogFileReader->Read( ( char* )content.GetString(), end - position );
	}
	
	if( ! IsRunning() ){
		pLogFileReader = NULL;  // close log file
	}
	
	return content;
}

decString projTestRunner::GetLastLogContent(){
	const igdeGameProject &project = *pWindowMain.GetGameProject();
	decPath path;
	path.SetFromNative( project.GetDirectoryPath() );
	path.AddUnixPath( "testRun.log" );
	
	decBaseFileReaderReference reader;
	try{
		reader.TakeOver( new decDiskFileReader( path.GetPathNative() ) );
		
		const int length = reader->GetLength();
		if( length == 0 ){
			return "";
		}
		
		decString content;
		content.Set( ' ', length );
		reader->Read( ( char* )content.GetString(), length );
		return content;
		
	}catch( const deException & ){
		// ignore errors. thread this as file not found
		return "";
	}
}




// Private Functions
//////////////////////

void projTestRunner::pInitLogFile(){
	// make sure no reader is open
	pLogFileReader = NULL;
	
	// store path to log file to use
	const igdeGameProject &project = *pWindowMain.GetGameProject();
	
	decPath path;
	path.SetFromNative( project.GetDirectoryPath() );
	path.AddUnixPath( "testRun.log" );
	
	pPathLogFile = path.GetPathNative();
	
	// create log file or truncate it to 0 length if present
	decDiskFileWriter::Ref::New( new decDiskFileWriter( pPathLogFile, false ) );
	
	// open file for reading
	pLogFileReader.TakeOver( new decDiskFileReader( pPathLogFile ) );
}

void projTestRunner::pSendLaunchParameters(){
	// get project profile to use
	const igdeGameProject &project = *pWindowMain.GetGameProject();
	decPath path;
	int i, count;
	
	// merge them together
	int windowSizeX = 0;
	int windowSizeY = 0;
	bool fullScreen = true;
	
	if( pLauncherProfile ){
		windowSizeX = pLauncherProfile->GetWidth();
		windowSizeY = pLauncherProfile->GetHeight();
		fullScreen = pLauncherProfile->GetFullScreen();
	}
	
	if( pProfile->GetWindowSize().x != 0 ){
		windowSizeX = pProfile->GetWindowSize().x;
	}
	if( pProfile->GetWindowSize().y != 0 ){
		windowSizeY = pProfile->GetWindowSize().y;
	}
	if( pProfile->GetWindowSize() != decPoint() ){
		fullScreen = false;
	}
	
	decString runArguments;
	projTRPParameterList parameters;
	
	if( pLauncherProfile ){
		const projTRPParameterList &lpparams = pLauncherProfile->GetParameters();
		count = lpparams.GetCount();
		for( i=0; i<count; i++ ){
			parameters.Set( *lpparams.GetAt( i ) );
		}
		
		if( pLauncherProfile->GetReplaceRunArguments() ){
			runArguments = pLauncherProfile->GetRunArguments();
			
		}else if( ! pLauncherProfile->GetRunArguments().IsEmpty() ){
			if( ! runArguments.IsEmpty() ){
				runArguments += " ";
			}
			runArguments += pLauncherProfile->GetRunArguments();
		}
		
	}else{
		// TODO get this from engine controller somehow
	}
	
	if( ! pProfile->GetRunArguments().IsEmpty() ){
		if( ! runArguments.IsEmpty() ){
			runArguments += " ";
		}
		runArguments += pProfile->GetRunArguments();
	}
	
	// send parameters over the pipe
	WriteString16ToPipe( pPathLogFile );
	
	path.SetFromNative( project.GetDirectoryPath() );
	path.AddUnixPath( project.GetPathData() );
	WriteString16ToPipe( path.GetPathNative() );
	
	path.SetFromNative( project.GetDirectoryPath() );
	path.AddUnixPath( project.GetPathCache() );
	path.AddComponent( "testrun" );
	path.AddComponent( "overlay" );
	WriteString16ToPipe( path.GetPathNative() );
	
	path.RemoveLastComponent();
	path.AddComponent( "config" );
	WriteString16ToPipe( path.GetPathNative() );
	
	path.RemoveLastComponent();
	path.AddComponent( "capture" );
	WriteString16ToPipe( path.GetPathNative() );
	
	WriteString16ToPipe( pProfile->GetScriptDirectory() );
	WriteString16ToPipe( project.GetScriptModuleVersion() );
	WriteString16ToPipe( pProfile->GetGameObject() );
	WriteString16ToPipe( pProfile->GetPathConfig() );
	WriteString16ToPipe( pProfile->GetPathCapture() );
	
	WriteString16ToPipe( pProfile->GetIdentifier().ToHexString( false ) );
	WriteUShortToPipe( windowSizeX );
	WriteUShortToPipe( windowSizeY );
	WriteUCharToPipe( fullScreen ? 1 : 0 );
	WriteString16ToPipe( decString( "Test Run: " ) + project.GetName() );
	
	count = parameters.GetCount();
	WriteUShortToPipe( count );
	for( i=0; i<count; i++ ){
		const projTRPParameter &parameter = *parameters.GetAt( i );
		WriteString16ToPipe( parameter.GetModule() );
		WriteString16ToPipe( parameter.GetName() );
		WriteString16ToPipe( parameter.GetValue() );
	}
	
	WriteString16ToPipe( runArguments );
	
	const decStringSet &excludePatterns = pProfile->GetExcludePatterns();
	count = excludePatterns.GetCount();
	WriteUShortToPipe( count );
	for( i=0; i<count; i++ ){
		WriteString16ToPipe( excludePatterns.GetAt( i ) );
	}
	
	const decStringSet &requiredExtensions = pProfile->GetRequiredExtensions();
	count = requiredExtensions.GetCount();
	WriteUShortToPipe( count );
	for( i=0; i<count; i++ ){
		WriteString16ToPipe( requiredExtensions.GetAt( i ) );
	}
	
	WriteString16ToPipe( project.GetScriptModule() );
	WriteString16ToPipe( project.GetScriptModuleVersion() );
	
	if( pLauncherProfile ){
		WriteString16ToPipe( pLauncherProfile->GetModuleGraphic() );
		WriteString16ToPipe( pLauncherProfile->GetModuleInput() );
		WriteString16ToPipe( pLauncherProfile->GetModulePhysics() );
		WriteString16ToPipe( pLauncherProfile->GetModuleAnimator() );
		WriteString16ToPipe( pLauncherProfile->GetModuleAI() );
		WriteString16ToPipe( pLauncherProfile->GetModuleCrashRecovery() );
		WriteString16ToPipe( pLauncherProfile->GetModuleAudio() );
		WriteString16ToPipe( pLauncherProfile->GetModuleSynthesizer() );
		WriteString16ToPipe( pLauncherProfile->GetModuleNetwork() );
		WriteString16ToPipe( pLauncherProfile->GetModuleVR() );
		
	}else{
		WriteString16ToPipe( "" ); // default graphic
		WriteString16ToPipe( "" ); // default input
		WriteString16ToPipe( "" ); // default physics
		WriteString16ToPipe( "" ); // default animator
		WriteString16ToPipe( "" ); // default ai
		WriteString16ToPipe( "" ); // default crash recovery
		WriteString16ToPipe( "" ); // default audio
		WriteString16ToPipe( "" ); // default synthesizer
		WriteString16ToPipe( "" ); // default network
		WriteString16ToPipe( "" ); // default vr
		
		/*
		const deEngine &engine = *pWindowMain.GetEngine();
		WriteString16ToPipe( engine.GetGraphicSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetInputSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetVRSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetPhysicsSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetAnimatorSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetAISystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetCrashRecoverySystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetAudioSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetSynthesizerSystem()->GetActiveLoadableModule()->GetName() );
		WriteString16ToPipe( engine.GetNetworkSystem()->GetActiveLoadableModule()->GetName() );
		*/
	}
	
	// result code
// 	const int resultCode = ReadUCharFromPipe();
// 	if( resultCode != projTestRunConstants::ercSuccess ){
// 		DETHROW( deeInvalidParam );
// 	}
}
