/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#else
#include <sys/wait.h>
#include <sys/select.h>
#endif

#include "delEngineInstance.h"
#include "delEngineProcess.h"
#include "modules/delEngineModule.h"
#include "modules/parameter/delEMParameter.h"
#include "../delLauncher.h"
#include "../game/profile/delGPModule.h"
#include "../game/profile/delGPModuleList.h"
#include "../game/profile/delGPMParameter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>



// Class delEngineInstance
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineInstance::delEngineInstance( delLauncher &launcher, const char *logfile ) :
pLauncher( launcher ),
pLogFile( logfile ),
pStopping( false ),
pGameCollectChangedParams( NULL ),

#ifdef OS_W32
pPipeIn( INVALID_HANDLE_VALUE ),
pPipeOut( INVALID_HANDLE_VALUE ),
pProcessHandle( INVALID_HANDLE_VALUE ),
pThreadHandle( INVALID_HANDLE_VALUE ),
pProcessID( 0 ),
pThreadID( 0 ),
pExecutableName( "delauncher-engine" )
#else

pPipeIn( 0 ),
pPipeOut( 0 ),
pProcessID( 0 )
#endif
{
}

delEngineInstance::~delEngineInstance(){
	StopEngine();
}



// Management
///////////////
#ifdef OS_W32
void delEngineInstance::SetExecutableName( const char *executableName ){
	pExecutableName = executableName;
}
#endif



bool delEngineInstance::IsEngineRunning() const{
#ifdef OS_W32
	return pProcessHandle != INVALID_HANDLE_VALUE;
#else
	return pProcessID > 0;
#endif
}

bool delEngineInstance::StartEngine(){
	decPath logFile;
	logFile.SetFromNative( pLauncher.GetPathLogs() );
	logFile.AddUnixPath( pLogFile );
	
#ifdef OS_W32
	if( pProcessHandle == INVALID_HANDLE_VALUE ){
		HANDLE pipesInRead = INVALID_HANDLE_VALUE;
		HANDLE pipesInWrite = INVALID_HANDLE_VALUE;
		HANDLE pipesOutRead = INVALID_HANDLE_VALUE;
		HANDLE pipesOutWrite = INVALID_HANDLE_VALUE;
		SECURITY_ATTRIBUTES secattr;
		PROCESS_INFORMATION procInfo;
		unsigned short logFileLength;
		STARTUPINFO startInfo;
		DWORD bytesWritten = 0;
		DWORD bytesRead = 0;
		char sync;
		
		ZeroMemory( &procInfo, sizeof( procInfo ) );
		procInfo.hProcess = INVALID_HANDLE_VALUE;
		
		TCHAR cmdline[ 256 ];
		ZeroMemory( &cmdline, sizeof( cmdline ) );
		deOSWindows::Utf8ToWide( pExecutableName, &cmdline[ 0 ], sizeof( cmdline ) - 1 );
		
		try{
			// create the pipes. the child process inherits the in-pipe read end
			// and the out-pipe write end. not inherited are the in-pipe write end
			// and the out-pipe read end.
			memset( &secattr, '\0', sizeof( secattr ) );
			secattr.nLength = sizeof( secattr );
			secattr.bInheritHandle = TRUE;
			secattr.lpSecurityDescriptor = NULL;
			
			if( ! CreatePipe( &pipesInRead, &pipesInWrite, &secattr, 0 ) ){
				DETHROW_INFO( deeInvalidAction, "create in-read pipe failed" );
			}
			if( ! CreatePipe( &pipesOutRead, &pipesOutWrite, &secattr, 0 ) ){
				DETHROW_INFO( deeInvalidAction, "create out-write pipe failed" );
			}
//			printf( "pipes in(%p,%p) out(%p,%p)...\n", pipesInRead, pipesInWrite, pipesOutRead, pipesOutWrite );
			
			if( ! SetHandleInformation( pipesInWrite, HANDLE_FLAG_INHERIT, 0 ) ){
				DETHROW_INFO( deeInvalidAction, "set handle information in-write failed" );
			}
			if( ! SetHandleInformation( pipesOutRead, HANDLE_FLAG_INHERIT, 0 ) ){
				DETHROW_INFO( deeInvalidAction, "set handle information out-read failed" );
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
			
			if( ! CreateProcess( NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInfo ) ){
				DETHROW_INFO( deeInvalidAction, "create process failed" );
			}
			
//			printf( "send out pipe...\n" );
			if( ! WriteFile( pipesInWrite, &pipesOutWrite, sizeof( pipesOutWrite ), &bytesWritten, NULL ) ){
				DETHROW_INFO( deeInvalidAction, "send out pipe failed" );
			}
			if( bytesWritten < sizeof( pipesOutWrite ) ){
				DETHROW_INFO( deeInvalidAction, "send out pipe too short" );
			}
			
			// send name of log file relative to the log directory
			logFileLength = ( unsigned short )strlen( logFile.GetPathNative() );
			
//			printf( "send log file length...\n" );
			if( ! WriteFile( pipesInWrite, &logFileLength, sizeof( logFileLength ), &bytesWritten, NULL ) ){
				DETHROW_INFO( deeInvalidAction, "send log file length failed" );
			}
			if( bytesWritten < sizeof( logFileLength ) ){
				DETHROW_INFO( deeInvalidAction, "send log file length too short" );
			}
			
//			printf( "send log file name...\n" );
			if( ! WriteFile( pipesInWrite, logFile.GetPathNative(), ( int )logFileLength, &bytesWritten, NULL ) ){
				DETHROW_INFO( deeInvalidAction, "send log file name failed" );
			}
			if( bytesWritten < ( DWORD )logFileLength ){
				DETHROW_INFO( deeInvalidAction, "send log file name too short" );
			}
			
			// read sync
//			printf( "reading sync\n" );
			if( ! ReadFile( pipesOutRead, &sync, 1, &bytesRead, NULL ) ){
				DETHROW_INFO( deeInvalidParam, "read sync failed" );
			}
			if( bytesRead < 1 ){
				DETHROW_INFO( deeInvalidParam, "read sync too short" );
			}
			if( sync != 1 ){
				DETHROW_INFO( deeInvalidParam, "sync value invalid" );
			}
			
//			printf( "closing loose ends...\n" );
			CloseHandle( pipesInRead );
			pipesInRead = INVALID_HANDLE_VALUE;
			CloseHandle( pipesOutWrite );
			pipesOutWrite = INVALID_HANDLE_VALUE;
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
			pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(), "GetLastError %lx", GetLastError() );
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
			return false;
		}
		
		pProcessHandle = procInfo.hProcess;
		pProcessID = procInfo.dwProcessId;
		pThreadHandle = procInfo.hThread;
		pThreadID = procInfo.dwThreadId;
		pPipeIn = pipesInWrite;
		pPipeOut = pipesOutRead;
		
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Game engine running in process %i", ( int )pProcessID );
		
		// connect logger
		/*
		pReadLog = new delEngineInstanceReadLog( this, pLogger );
		if( ! pReadLog ) DETHROW( deeOutOfMemory );
		
		pReadLog->Start();
		*/
		
		return true;
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
			return false;
		}
		
		pProcessID = fork();
		
		if( pProcessID == 0 ){
			close( pipesIn[ 1 ] );
			close( pipesOut[ 0 ] );
			
			delEngineProcess process( pipesIn[ 0 ], pipesOut[ 1 ], logFile.GetPathNative() );
			
			process.Run();
			exit( 0 );
			
		}else{
			close( pipesIn[ 0 ] );
			close( pipesOut[ 1 ] );
			
			pPipeIn = pipesIn[ 1 ];
			pPipeOut = pipesOut[ 0 ];
			
			pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
				"Game engine running in process %i", pProcessID );
		}
		
		return true;
	}
#endif
	
	return false;
}

void delEngineInstance::StopEngine(){
#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Stopping game engine in process %i", ( int )pProcessID );
		if( ! StopProcess() ){
			TerminateProcess( pProcessHandle, 0 );
			WaitForSingleObject( pProcessHandle, 5000 );
		}
		
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
	
#else
	if( pProcessID ){
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Stopping game engine in process %i", pProcessID );
		if( ! StopProcess() ){
			kill( pProcessID, SIGKILL );
			waitpid( pProcessID, NULL, 0 );
		}
		
		pProcessID = 0;
	}
#endif
	
#ifdef OS_W32
	if( pPipeIn != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeIn );
		pPipeIn = INVALID_HANDLE_VALUE;
	}
	
	if( pPipeOut != INVALID_HANDLE_VALUE ){
		CloseHandle( pPipeOut );
		pPipeOut = INVALID_HANDLE_VALUE;
	}
	
#else
	if( pPipeIn ){
		// check if the game wrote out something for us
		close( pPipeIn );
		pPipeIn = 0;
	}
	
	if( pPipeOut ){
		close( pPipeOut );
		pPipeOut = 0;
	}
#endif
}

void delEngineInstance::KillProcess(){
#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Killing game engine process %i", ( int )pProcessID );
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
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Killing game engine process %i", pProcessID );
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
}



void delEngineInstance::WriteUCharToPipe( int value ){
	if( value < 0 || value > 0xff ){
		DETHROW( deeInvalidParam );
	}
	const uint8_t uchar = ( uint8_t )value;
	WriteToPipe( &uchar, sizeof( uint8_t ) );
}

void delEngineInstance::WriteUShortToPipe( int value ){
	if( value < 0 || value > 0xffff ){
		DETHROW( deeInvalidParam );
	}
	const uint16_t ushort = ( uint16_t )value;
	WriteToPipe( &ushort, sizeof( uint16_t ) );
}

void delEngineInstance::WriteFloatToPipe( float value ){
	WriteToPipe( &value, sizeof( float ) );
}

void delEngineInstance::WriteString16ToPipe( const char *string ){
	const int length = strlen( string );
	WriteUShortToPipe( length );
	if( length > 0 ){
		WriteToPipe( string, length );
	}
}

void delEngineInstance::WriteToPipe( const void *data, int length ){
	#ifdef OS_W32
	DWORD bytesWritten = 0;
	
	if( pPipeIn == INVALID_HANDLE_VALUE
	|| ! WriteFile( pPipeIn, data, length, &bytesWritten, NULL )
	|| ( int )bytesWritten != length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( ! pPipeOut || write( pPipeIn, data, length ) < length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}



int delEngineInstance::ReadUCharFromPipe(){
	uint8_t uchar;
	ReadFromPipe( &uchar, sizeof( uint8_t ) );
	return uchar;
}

int delEngineInstance::ReadUShortFromPipe(){
	uint16_t ushort;
	ReadFromPipe( &ushort, sizeof( uint16_t ) );
	return ushort;
}

int delEngineInstance::ReadIntFromPipe(){
	int32_t intval;
	ReadFromPipe( &intval, sizeof( int32_t ) );
	return intval;
}

float delEngineInstance::ReadFloatFromPipe(){
	float value;
	ReadFromPipe( &value, sizeof( float ) );
	return value;
}

void delEngineInstance::ReadString16FromPipe( decString &string ){
	const int length = ReadUShortFromPipe();
	string.Set( ' ', length );
	if( length > 0 ){
		ReadFromPipe( ( char* )string.GetString(), length );
	}
}

void delEngineInstance::ReadFromPipe( void *data, int length ){
	#ifdef OS_W32
	DWORD bytesRead = 0;
	
	if( pPipeOut == INVALID_HANDLE_VALUE
	|| ! ReadFile( pPipeOut, data, length, &bytesRead, NULL )
	|| ( int )bytesRead != length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( ! pPipeOut || read( pPipeOut, data, length ) != length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}

bool delEngineInstance::CheckCanReadPipe(){
	#ifdef OS_W32
	if( pPipeOut == INVALID_HANDLE_VALUE ){
		return false;
	}
	
	DWORD readableBytes = 0;
	return PeekNamedPipe( pPipeOut, NULL, 0, NULL, &readableBytes, NULL ) && readableBytes > 0;
	
	#else
	if( ! pPipeOut ){
		return false;
	}
	
	struct timeval tv;
	fd_set rfds;
	
	FD_ZERO( &rfds );
	FD_SET( pPipeOut, &rfds );
	
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	return select( pPipeOut + 1, &rfds, NULL, NULL, &tv ) == 1;
	#endif
}

void delEngineInstance::DrainReadPipe(){
	while( CheckCanReadPipe() ){
		ReadUCharFromPipe();
	}
}



bool delEngineInstance::StopProcess(){
#ifdef OS_W32
	if( pPipeIn != INVALID_HANDLE_VALUE ){
#else
	if( pPipeIn ){
#endif
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Sending eccStopProcess to process %i", ( int )pProcessID );
		
		try{
			WriteUCharToPipe( delEngineProcess::eccStopProcess );
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogError( pLauncher.GetLogSource(),
				"EngineInstance.StopProcess sending eccStopProcess failed with exception:" );
			pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		}
		
#ifdef OS_W32
		WaitForSingleObject( pProcessHandle, 5000 );
#else
		waitpid( pProcessID, NULL, 0 );
#endif
		
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Process %i terminated", ( int )pProcessID );
		return true;
	}
	
	return false;
}

void delEngineInstance::GetProperty( int property, decString &value ){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccGetProperty(property=%i) to process %i", property, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetProperty );
	WriteUCharToPipe( property );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	ReadString16FromPipe( value );
}

void delEngineInstance::LoadModules(){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccLoadModules to process %i", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccLoadModules );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

int delEngineInstance::GetModuleStatus( const char *moduleName, const char *moduleVersion ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccGetModuleStatus(module='%s':%s) to process %i",
		moduleName, moduleVersion, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetModuleStatus );
	WriteString16ToPipe( moduleName );
	WriteString16ToPipe( moduleVersion );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	return ReadUShortFromPipe();
}

void delEngineInstance::GetModuleParamList( delEngineModule &module ){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccGetModuleParamList(moduleName='%s') to process %i",
		module.GetName().GetString(), ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetModuleParamList );
	WriteString16ToPipe( module.GetName() );
	WriteString16ToPipe( module.GetVersion() );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ) DETHROW( deeInvalidAction );
	
	delEMParameterList &parameters = module.GetParameters();
	deModuleParameter::SelectionEntry entry;
	const int count = ReadUShortFromPipe();
	decString string;
	int i, j;
	
	for( i=0; i<count; i++ ){
		try{
			deModuleParameter info;
			
			ReadString16FromPipe( string );
			info.SetName( string );
			
			ReadString16FromPipe( string );
			info.SetDescription( string );
			
			info.SetType( ( deModuleParameter::eParameterTypes )ReadUCharFromPipe() );
			info.SetMinimumValue( ReadFloatFromPipe() );
			info.SetMaximumValue( ReadFloatFromPipe() );
			info.SetValueStepSize( ReadFloatFromPipe() );
			info.SetCategory( ( deModuleParameter::eCategory )ReadUCharFromPipe() );
			
			ReadString16FromPipe( string );
			info.SetDisplayName( string );
			
			const int entryCount = ReadUShortFromPipe();
			for( j=0; j<entryCount; j++ ){
				ReadString16FromPipe( entry.value );
				ReadString16FromPipe( entry.displayName );
				ReadString16FromPipe( entry.description );
				info.AddSelectionEntry( entry );
			}
			
			ReadString16FromPipe( string );
			
			parameters.Add( delEMParameter::Ref::New( new delEMParameter( i, info, string ) ) );
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogError( pLauncher.GetLogSource(),
				"EngineInstance.GetModuleParamList parameter creation failed:" );
			pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
			throw;
		}
	}
}

void delEngineInstance::SetModuleParameter( const char *moduleName, const char *moduleVersion,
const char *parameter, const char *value ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	if( ! parameter ){
		DETHROW_INFO( deeNullPointer, "parameter" );
	}
	if( ! value ){
		DETHROW_INFO( deeNullPointer, "value" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccSetModuleParameter(module='%s':%s,parameter=%s,value='%s') to process %d",
		moduleName, moduleVersion, parameter, value, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetModuleParameter );
	WriteString16ToPipe( moduleName );
	WriteString16ToPipe( moduleVersion );
	WriteString16ToPipe( parameter );
	WriteString16ToPipe( value );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::ActivateModule( const char *moduleName, const char *moduleVersion ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccActivateModule(module='%s':%s) to process %i",
		moduleName, moduleVersion, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccActivateModule );
	WriteString16ToPipe( moduleName );
	WriteString16ToPipe( moduleVersion );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::EnableModule( const char *moduleName, const char *moduleVersion, bool enable ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccEnableModule(module='%s':%s %s) to process %i",
		moduleName, moduleVersion, enable ? "enable" : "disable", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccEnableModule );
	WriteString16ToPipe( moduleName );
	WriteString16ToPipe( moduleVersion );
	WriteUCharToPipe( enable ? 1 : 0 );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::SetDataDirectory( const char *directory ){
	if( ! directory ){
		DETHROW_INFO( deeNullPointer, "directory" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccSetDataDir(directory='%s') to process %i", directory, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetDataDir );
	WriteString16ToPipe( directory );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ) DETHROW( deeInvalidAction );
}

void delEngineInstance::SetCacheAppID( const char *cacheAppID ){
	if( ! cacheAppID ){
		DETHROW_INFO( deeNullPointer, "cacheAppID" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccSetCacheAppID(id='%s') to process %i", cacheAppID, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetCacheAppID );
	WriteString16ToPipe( cacheAppID );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ){
	if( ! vfsRoot ){
		DETHROW_INFO( deeNullPointer, "vfsRoot" );
	}
	if( ! nativeDirectory ){
		DETHROW_INFO( deeNullPointer, "nativeDirectory" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccVFSAddDiskDir(vfsRoot='%s',nativeDirectory='%s',readOnly=%c) to process %i",
		vfsRoot, nativeDirectory, readOnly?'y':'n', ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddDiskDir );
	WriteString16ToPipe( vfsRoot );
	WriteString16ToPipe( nativeDirectory );
	WriteUCharToPipe( readOnly ? 1 : 0 );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::VFSAddScriptSharedDataDir(){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccVFSAddScriptSharedDataDir to process %i", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddScriptSharedDataDir );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::VFSAddDelgaFile( const char *delgaFile, const char *archivePath ){
	if( ! delgaFile ){
		DETHROW_INFO( deeNullPointer, "delgaFile" );
	}
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccVFSAddDelga(delgaFile='%s', archivePath=%s) to process %i",
		delgaFile, archivePath, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddDelgaFile );
	WriteString16ToPipe( delgaFile );
	WriteString16ToPipe( archivePath );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::SetCmdLineArgs( const char *arguments ){
	if( ! arguments ){
		DETHROW_INFO( deeNullPointer, "arguments" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccSetCmdLineArgs(arguments='%s') to process %i", arguments, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetCmdLineArgs );
	WriteString16ToPipe( arguments );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::CreateRenderWindow( int width, int height, bool fullScreen,
const char *windowTitle, const char *iconPath ){
	if( width < 1 ){
		DETHROW_INFO( deeInvalidAction, "width < 1" );
	}
	if( height < 1 ){
		DETHROW_INFO( deeInvalidAction, "height < 1" );
	}
	if( ! windowTitle ){
		DETHROW_INFO( deeNullPointer, "windowTitle" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(), "Sending eccCreateRenderWindow("
		"width=%d,height=%d,fullScreen=%c,windowTitle='%s',iconPath='%s') to process %d",
		width, height, fullScreen?'y':'n', windowTitle, iconPath, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccCreateRenderWindow );
	WriteUShortToPipe( width );
	WriteUShortToPipe( height );
	WriteUCharToPipe( fullScreen ? 1 : 0 );
	WriteString16ToPipe( windowTitle );
	WriteString16ToPipe( iconPath );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstance::StartGame( const char *scriptDirectory, const char *gameObject,
delGPModuleList *collectChangedParams ){
	if( ! scriptDirectory ){
		DETHROW_INFO( deeNullPointer, "scriptDirectory" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(), "Sending eccStartGame("
		"scriptDirectory='%s' gameObject='%s') to process %i", scriptDirectory,
		gameObject, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccStartGame );
	WriteString16ToPipe( scriptDirectory );
	WriteString16ToPipe( gameObject );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	pGameCollectChangedParams = collectChangedParams;
}

void delEngineInstance::StopGame(){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccStopGame to process %i", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccStopGame );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

int delEngineInstance::IsGameRunning(){
	if( ! IsEngineRunning() ){
		return 0;
	}
	
	// check if the process is still running. could be it crashed for some reason
#ifdef OS_W32
	DWORD exitCode = 0;
	if( ! GetExitCodeProcess( pProcessHandle, &exitCode ) ){
		// child stopped for some reason. kill it to make sure all is cleaned up one way or the other
		KillProcess();
		return 0;
	}
	
	if( exitCode != STILL_ACTIVE ){
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
			"Received game stop (result=%ld) from process %d", exitCode, ( int )pProcessID );
		
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
		
		return 0;
	}
	
#else
	int status;
	if( waitpid( pProcessID, &status, WNOHANG | WUNTRACED | WCONTINUED ) ){
		// child stopped for some reason. kill it to make sure all is cleaned up one way or the other
		KillProcess();
		return 0;
	}
	
	// this is a little hack right now. we poke at the pipe to see if a result came back
	// without a command being send. if so this has to be the game exiting.
	/*
	if( CheckCanReadPipe() ){
		//const int result = ReadUCharFromPipe();
		//pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(), "Received game stop (result=%d) from process %d", result, ( int )pProcessID );
		pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(), "Game process %d exited", ( int )pProcessID );
		return 0;
	}
	*/
#endif
	
	// check if the game exited normally writing final information to us
	if( pGameCollectChangedParams ){
		if( CheckCanReadPipe() && ReadUCharFromPipe() == delEngineProcess::ercGameExited ){
			// read changed module parameters
			pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(),
				"game exited. reading module parameter changes" );
			pGameCollectChangedParams->RemoveAll();
			
			decString moduleName, paramName, paramValue;
			delGPModule::Ref module;
			
			while( true ){
				ReadString16FromPipe( moduleName );
				if( moduleName.IsEmpty() ){
					break; // end of list marker
				}
				
				module = pGameCollectChangedParams->GetNamed( moduleName );
				if( ! module ){
					module.TakeOver( new delGPModule( moduleName ) );
					pGameCollectChangedParams->Add( module );
				}
				
				ReadString16FromPipe( paramName );
				ReadString16FromPipe( paramValue );
				
				module->GetParameters().Add( delGPMParameter::Ref::New(
					new delGPMParameter( paramName, paramValue ) ) );
			}
			
			// write any byte to pipe to signal process can exit now
			pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(), "sending acknowledge" );
			WriteUCharToPipe( 1 );
			pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(), "running game finished" );
			
			pGameCollectChangedParams = NULL;
			return 0; // exit
		}
	}
	
	return 1;
}

decPoint delEngineInstance::GetDisplayCurrentResolution( int display ){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccGetDisplayCurrentResolution(display=%d)"
		" to process %i", display, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetDisplayCurrentResolution );
	WriteUCharToPipe( display );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	decPoint resolution;
	ReadFromPipe( &resolution, sizeof( decPoint ) );
	return resolution;
}

int delEngineInstance::GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount ){
	if( resolutionCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "resolutionCount < 0" );
	}
	if( resolutionCount > 0 && ! resolutions ){
		DETHROW_INFO( deeInvalidParam, "resolutionCount > 0 with resolutions = nullptr" );
	}
	
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccGetDisplayResolutions(display=%d, resolutions=%d, resolutionCount=%d) to process %i",
		display, resolutions ? 1 : 0, resolutionCount, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetDisplayResolutions );
	WriteUCharToPipe( display );
	WriteUCharToPipe( resolutionCount );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	const int sendResolutionCount = ReadUCharFromPipe();
	if( sendResolutionCount > resolutionCount ){
		DETHROW( deeInvalidAction );
	}
	
	if( sendResolutionCount > 0 ){
		ReadFromPipe( resolutions, sizeof( decPoint ) * sendResolutionCount );
	}
	
	return sendResolutionCount;
}

void delEngineInstance::ReadDelgaGameDefs( const char *delgaFile, decStringList &list ){
	// send command
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccReadDelgaGameDefs (delgaFile=%s) to process %i", delgaFile, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccReadDelgaGameDefs );
	WriteString16ToPipe( delgaFile );
	
	// wait for command to finish
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	// read list of files and content. no processing done yet to not fail
	const int count = ReadUShortFromPipe();
	decString fileContent;
	int i;
	
	for( i=0; i<count; i++ ){
		ReadString16FromPipe( fileContent );
		list.Add( fileContent );
	}
}

void delEngineInstance::ReadDelgaPatchDefs( const char *delgaFile, decStringList &list ){
	// send command
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccReadDelgaPatchDefs (delgaFile=%s) to process %i", delgaFile, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccReadDelgaPatchDefs );
	WriteString16ToPipe( delgaFile );
	
	// wait for command to finish
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	// read list of files and content. no processing done yet to not fail
	const int count = ReadUShortFromPipe();
	decString fileContent;
	int i;
	
	for( i=0; i<count; i++ ){
		ReadString16FromPipe( fileContent );
		list.Add( fileContent );
	}
}


void delEngineInstance::ReadDelgaFiles( const char *delgaFile,
const decStringList &filenames, decObjectOrderedSet &filesContent ){
	pLauncher.GetLogger()->LogInfoFormat( pLauncher.GetLogSource(),
		"Sending eccReadDelgaFiles (delgaFile=%s) to process %i", delgaFile, ( int )pProcessID );
	
	const int fileCount = filenames.GetCount();
	filesContent.RemoveAll();
	if( fileCount == 0 ){
		return;
	}
	if( fileCount > 255 ){
		DETHROW( deeInvalidParam );
	}
	
	WriteUCharToPipe( delEngineProcess::eccReadDelgaFiles );
	WriteString16ToPipe( delgaFile );
	
	WriteUCharToPipe( fileCount );
	int i;
	for( i=0; i<fileCount; i++ ){
		WriteString16ToPipe( filenames.GetAt( i ) );
	}
	
	// wait for command to finish
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	// read file content
	const int count = ReadUCharFromPipe();
	decMemoryFile::Ref content;
	
	for( i=0; i<count; i++ ){
		const int size = ReadIntFromPipe();
		content.TakeOver( new decMemoryFile( filenames.GetAt( i ) ) );
		content->Resize( size );
		ReadFromPipe( content->GetPointer(), size );
		filesContent.Add( content );
	}
}
