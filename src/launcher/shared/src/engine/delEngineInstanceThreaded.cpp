/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dragengine/dragengine_configuration.h>

#ifndef OS_ANDROID

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#endif

#include "delEngineInstanceThreaded.h"
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



// Class delEngineInstanceThreaded::Factory
/////////////////////////////////////////////

delEngineInstanceThreaded::Factory::Factory(){}
delEngineInstanceThreaded::Factory::~Factory(){}

delEngineInstance *delEngineInstanceThreaded::Factory::CreateEngineInstance(
delLauncher &launcher, const char *logfile ){
	return new delEngineInstanceThreaded( launcher, logfile );
}



// Class delEngineInstanceThreaded
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineInstanceThreaded::delEngineInstanceThreaded( delLauncher &launcher, const char *logfile ) :
delEngineInstance( launcher, logfile ),
pGameCollectChangedParams( nullptr ),

#ifdef OS_W32
pPipeIn( INVALID_HANDLE_VALUE ),
pPipeOut( INVALID_HANDLE_VALUE ),
pProcessHandle( INVALID_HANDLE_VALUE ),
pThreadHandle( INVALID_HANDLE_VALUE ),
pProcessID( 0 ),
pThreadID( 0 ),
pExecutableName( pDefaultExecutableName )
#else

pPipeIn( 0 ),
pPipeOut( 0 ),
pProcessID( 0 )
#endif
{
}

delEngineInstanceThreaded::~delEngineInstanceThreaded(){
	StopEngine();
}



// Management
///////////////

#ifdef OS_W32
decString delEngineInstanceThreaded::pDefaultExecutableName( "delauncher-engine" );

void delEngineInstanceThreaded::SetExecutableName( const char *executableName ){
	pExecutableName = executableName;
}

void delEngineInstanceThreaded::SetDefaultExecutableName( const char *executableName ){
	pDefaultExecutableName = executableName;
}
#endif



bool delEngineInstanceThreaded::IsEngineRunning() const{
#ifdef OS_W32
	return pProcessHandle != INVALID_HANDLE_VALUE;
#else
	return pProcessID > 0;
#endif
}

bool delEngineInstanceThreaded::StartEngine(){
	decPath logFile;
	logFile.SetFromNative( GetLauncher().GetPathLogs() );
	logFile.AddUnixPath( GetLogFile() );
	
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
			
			uint8_t flags = 0;
			if( GetUseConsole() ){
				flags |= 0x1;
			}
			
			if( ! WriteFile( pipesInWrite, &flags, 1, &bytesWritten, NULL ) ){
				DETHROW_INFO( deeInvalidAction, "send flags failed" );
			}
			if( bytesWritten < 1 ){
				DETHROW_INFO( deeInvalidAction, "send flags too short" );
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
			GetLauncher().GetLogger()->LogException( GetLauncher().GetLogSource(), e );
			GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "GetLastError %lx", GetLastError() );
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
		
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
			"Game engine running in process %i", ( int )pProcessID );
		
		// connect logger
		/*
		pReadLog = new delEngineInstanceThreadedReadLog( this, pLogger );
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
			process.SetUseConsole( GetUseConsole() );
			
			process.Run();
			exit( 0 );
			
		}else{
			close( pipesIn[ 0 ] );
			close( pipesOut[ 1 ] );
			
			pPipeIn = pipesIn[ 1 ];
			pPipeOut = pipesOut[ 0 ];
			
			GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
				"Game engine running in process %i", pProcessID );
		}
		
		return true;
	}
#endif
	
	return false;
}

void delEngineInstanceThreaded::StopEngine(){
#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

void delEngineInstanceThreaded::KillEngine(){
#ifdef OS_W32
	if( pProcessHandle != INVALID_HANDLE_VALUE ){
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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



void delEngineInstanceThreaded::WriteUCharToPipe( int value ){
	if( value < 0 || value > 0xff ){
		DETHROW( deeInvalidParam );
	}
	const uint8_t value2 = ( uint8_t )value;
	WriteToPipe( &value2, sizeof( uint8_t ) );
}

void delEngineInstanceThreaded::WriteUShortToPipe( int value ){
	if( value < 0 || value > 0xffff ){
		DETHROW( deeInvalidParam );
	}
	const uint16_t value2 = ( uint16_t )value;
	WriteToPipe( &value2, sizeof( uint16_t ) );
}

void delEngineInstanceThreaded::WriteFloatToPipe( float value ){
	WriteToPipe( &value, sizeof( float ) );
}

void delEngineInstanceThreaded::WriteString16ToPipe( const char *string ){
	const int length = ( int )strlen( string );
	WriteUShortToPipe( length );
	if( length > 0 ){
		WriteToPipe( string, length );
	}
}

void delEngineInstanceThreaded::WriteToPipe( const void *data, int length ){
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



int delEngineInstanceThreaded::ReadUCharFromPipe(){
	uint8_t value;
	ReadFromPipe( &value, sizeof( uint8_t ) );
	return value;
}

int delEngineInstanceThreaded::ReadUShortFromPipe(){
	uint16_t value;
	ReadFromPipe( &value, sizeof( uint16_t ) );
	return value;
}

int delEngineInstanceThreaded::ReadIntFromPipe(){
	int32_t intval;
	ReadFromPipe( &intval, sizeof( int32_t ) );
	return intval;
}

float delEngineInstanceThreaded::ReadFloatFromPipe(){
	float value;
	ReadFromPipe( &value, sizeof( float ) );
	return value;
}

void delEngineInstanceThreaded::ReadString16FromPipe( decString &string ){
	const int length = ReadUShortFromPipe();
	string.Set( ' ', length );
	if( length > 0 ){
		ReadFromPipe( ( char* )string.GetString(), length );
	}
}

void delEngineInstanceThreaded::ReadFromPipe( void *data, int length ){
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

bool delEngineInstanceThreaded::CheckCanReadPipe(){
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

void delEngineInstanceThreaded::DrainReadPipe(){
	while( CheckCanReadPipe() ){
		ReadUCharFromPipe();
	}
}



bool delEngineInstanceThreaded::StopProcess(){
#ifdef OS_W32
	if( pPipeIn != INVALID_HANDLE_VALUE ){
#else
	if( pPipeIn ){
#endif
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
			"Sending eccStopProcess to process %i", ( int )pProcessID );
		
		try{
			WriteUCharToPipe( delEngineProcess::eccStopProcess );
			
		}catch( const deException &e ){
			GetLauncher().GetLogger()->LogError( GetLauncher().GetLogSource(),
				"EngineInstance.StopProcess sending eccStopProcess failed with exception:" );
			GetLauncher().GetLogger()->LogException( GetLauncher().GetLogSource(), e );
			
			KillEngine();
			return true;
		}
		
#ifdef OS_W32
		WaitForSingleObject( pProcessHandle, 5000 );
#else
		waitpid( pProcessID, NULL, 0 );
#endif
		
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
			"Process %i terminated", ( int )pProcessID );
		return true;
	}
	
	return false;
}

void delEngineInstanceThreaded::GetProperty( int property, decString &value ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccGetProperty(property=%i) to process %i", property, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetProperty );
	WriteUCharToPipe( property );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	ReadString16FromPipe( value );
}

void delEngineInstanceThreaded::LoadModules(){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccLoadModules to process %i", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccLoadModules );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::GetInternalModules(delEngineModuleList &list){
	GetLauncher().GetLogger()->LogInfoFormat(GetLauncher().GetLogSource(),
		"Sending eccGetInternalModules to process %d", (int)pProcessID);
	
	WriteUCharToPipe(delEngineProcess::eccGetInternalModules);
	
	if(ReadUCharFromPipe() != delEngineProcess::ercSuccess){
		DETHROW(deeInvalidAction);
	}
	
	const int count = ReadUShortFromPipe();
	decString string;
	int i;
	
	for(i=0; i<count; i++){
		const delEngineModule::Ref emod(delEngineModule::Ref::NewWith());
		emod->SetType((deModuleSystem::eModuleTypes)ReadUCharFromPipe());
		ReadString16FromPipe(string);
		emod->SetName(string);
		ReadString16FromPipe(string);
		emod->SetDescription(decUnicodeString::NewFromUTF8(string));
		ReadString16FromPipe(string);
		emod->SetAuthor(decUnicodeString::NewFromUTF8(string));
		ReadString16FromPipe(string);
		emod->SetVersion(string);
		ReadString16FromPipe(string);
		emod->SetDirectoryName(string);
		ReadString16FromPipe(string);
		emod->SetPattern(string);
		emod->SetPriority(ReadUShortFromPipe());
		emod->SetIsFallback(ReadUCharFromPipe() != 0);
		list.Add(emod);
	}
}

int delEngineInstanceThreaded::GetModuleStatus( const char *moduleName, const char *moduleVersion ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

void delEngineInstanceThreaded::GetModuleParams( delEngineModule &module ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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
			
			ReadString16FromPipe( string );
			info.SetDefaultValue( string );
			
			const int entryCount = ReadUShortFromPipe();
			for( j=0; j<entryCount; j++ ){
				ReadString16FromPipe( entry.value );
				ReadString16FromPipe( entry.displayName );
				ReadString16FromPipe( entry.description );
				info.AddSelectionEntry( entry );
			}
			
			ReadString16FromPipe( string );
			
			parameters.Add( delEMParameter::Ref::NewWith(i, info, string) );
			
		}catch( const deException &e ){
			GetLauncher().GetLogger()->LogError( GetLauncher().GetLogSource(),
				"EngineInstance.GetModuleParamList parameter creation failed:" );
			GetLauncher().GetLogger()->LogException( GetLauncher().GetLogSource(), e );
			throw;
		}
	}
}

void delEngineInstanceThreaded::SetModuleParameter( const char *moduleName, const char *moduleVersion,
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
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

void delEngineInstanceThreaded::ActivateModule( const char *moduleName, const char *moduleVersion ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccActivateModule(module='%s':%s) to process %i",
		moduleName, moduleVersion, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccActivateModule );
	WriteString16ToPipe( moduleName );
	WriteString16ToPipe( moduleVersion );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::EnableModule( const char *moduleName, const char *moduleVersion, bool enable ){
	if( ! moduleName ){
		DETHROW_INFO( deeNullPointer, "moduleName" );
	}
	if( ! moduleVersion ){
		DETHROW_INFO( deeNullPointer, "moduleVersion" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

void delEngineInstanceThreaded::SetDataDirectory( const char *directory ){
	if( ! directory ){
		DETHROW_INFO( deeNullPointer, "directory" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetDataDir(directory='%s') to process %i", directory, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetDataDir );
	WriteString16ToPipe( directory );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ) DETHROW( deeInvalidAction );
}

void delEngineInstanceThreaded::SetCacheAppID( const char *cacheAppID ){
	if( ! cacheAppID ){
		DETHROW_INFO( deeNullPointer, "cacheAppID" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetCacheAppID(id='%s') to process %i", cacheAppID, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetCacheAppID );
	WriteString16ToPipe( cacheAppID );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::SetPathOverlay( const char* path ){
	if( ! path ){
		DETHROW_INFO( deeNullPointer, "path" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetPathOverlay(path='%s') to process %i", path, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetPathOverlay );
	WriteString16ToPipe( path );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::SetPathCapture( const char* path ){
	if( ! path ){
		DETHROW_INFO( deeNullPointer, "path" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetPathCapture(path='%s') to process %i", path, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetPathCapture );
	WriteString16ToPipe( path );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::SetPathConfig( const char* path ){
	if( ! path ){
		DETHROW_INFO( deeNullPointer, "path" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetPathConfig(path='%s') to process %i", path, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetPathConfig );
	WriteString16ToPipe( path );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ){
	VFSAddDiskDir( vfsRoot, nativeDirectory, readOnly, decStringSet() );
}

void delEngineInstanceThreaded::VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory,
bool readOnly, const decStringSet &hiddenPath ){
	DEASSERT_NOTNULL( vfsRoot )
	DEASSERT_NOTNULL( nativeDirectory )
	
	const int hiddenPathCount = hiddenPath.GetCount();
	DEASSERT_TRUE( hiddenPathCount <= 0xffff )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccVFSAddDiskDir(vfsRoot='%s',nativeDirectory='%s',readOnly=%c,hiddenPath=%d) to process %i",
		vfsRoot, nativeDirectory, readOnly?'y':'n', hiddenPathCount, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddDiskDir );
	WriteString16ToPipe( vfsRoot );
	WriteString16ToPipe( nativeDirectory );
	WriteUCharToPipe( readOnly ? 1 : 0 );
	
	WriteUShortToPipe( ( unsigned short )hiddenPathCount );
	int i;
	for( i=0; i<hiddenPathCount; i++ ){
		WriteString16ToPipe( hiddenPath.GetAt( i ) );
	}
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::VFSAddScriptSharedDataDir(){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccVFSAddScriptSharedDataDir to process %i", ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddScriptSharedDataDir );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::VFSAddDelgaFile( const char *delgaFile, const char *archivePath ){
	VFSAddDelgaFile( delgaFile, archivePath, decStringSet() );
}

void delEngineInstanceThreaded::VFSAddDelgaFile( const char *delgaFile,
const char *archivePath, const decStringSet &hiddenPath ){
	DEASSERT_NOTNULL( delgaFile )
	
	const int hiddenPathCount = hiddenPath.GetCount();
	DEASSERT_TRUE( hiddenPathCount <= 0xffff )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccVFSAddDelga(delgaFile='%s', archivePath=%s, hiddenPath=%d) to process %i",
		delgaFile, archivePath, hiddenPathCount, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccVFSAddDelgaFile );
	WriteString16ToPipe( delgaFile );
	WriteString16ToPipe( archivePath );
	
	WriteUShortToPipe( ( unsigned short )hiddenPathCount );
	int i;
	for( i=0; i<hiddenPathCount; i++ ){
		WriteString16ToPipe( hiddenPath.GetAt( i ) );
	}
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::ModulesAddVFSContainers( const char *stage ){
	DEASSERT_NOTNULL( stage )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccModulesAddVFSContainers(stage='%s') to process %i", stage, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccModulesAddVFSContainers );
	WriteString16ToPipe( stage );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::SetCmdLineArgs(const char *arguments)
{
    if( ! arguments ){
		DETHROW_INFO( deeNullPointer, "arguments" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccSetCmdLineArgs(arguments='%s') to process %i", arguments, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccSetCmdLineArgs );
	WriteString16ToPipe( arguments );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
}

void delEngineInstanceThreaded::CreateRenderWindow( int width, int height, bool fullScreen,
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
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Sending eccCreateRenderWindow("
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

void delEngineInstanceThreaded::StartGame( const char *scriptDirectory, const char *gameObject,
delGPModuleList *collectChangedParams ){
	StartGame( scriptDirectory, "", gameObject, collectChangedParams );
}

void delEngineInstanceThreaded::StartGame( const char *scriptDirectory, const char *scriptVersion,
const char *gameObject, delGPModuleList *collectChangedParams ){
	if( ! scriptDirectory || ! scriptVersion ){
		DETHROW_INFO( deeNullPointer, "scriptDirectory" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Sending eccStartGame("
		"scriptDirectory='%s' scriptVersion='%s' gameObject='%s') to process %i",
		scriptDirectory, scriptVersion, gameObject, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccStartGame );
	WriteString16ToPipe( scriptDirectory );
	WriteString16ToPipe( scriptVersion );
	WriteString16ToPipe( gameObject );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	pGameCollectChangedParams = collectChangedParams;
}

void delEngineInstanceThreaded::StopGame(){
	GetLauncher().GetLogger()->LogInfoFormat(GetLauncher().GetLogSource(),
		"Sending eccStopGame to process %d", (int)pProcessID);
	
	WriteUCharToPipe( delEngineProcess::eccStopGame );
	
	try{
		if(ReadUCharFromPipe() != delEngineProcess::ercSuccess){
			DETHROW(deeInvalidAction);
		}
		
	}catch(const deException &){
		GetLauncher().GetLogger()->LogInfoFormat(GetLauncher().GetLogSource(),
			"Sending eccStopGame to process %d failed. Process no more running?", (int)pProcessID);
	}
}

int delEngineInstanceThreaded::IsGameRunning(){
	if( ! IsEngineRunning() ){
		return 0;
	}
	
	// check if the process is still running. could be it crashed for some reason
#ifdef OS_W32
	DWORD exitCode = 0;
	if( ! GetExitCodeProcess( pProcessHandle, &exitCode ) ){
		// child stopped for some reason. kill it to make sure all is cleaned up one way or the other
		KillEngine();
		return 0;
	}
	
	if( exitCode != STILL_ACTIVE ){
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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
		KillEngine();
		return 0;
	}
	
	// this is a little hack right now. we poke at the pipe to see if a result came back
	// without a command being send. if so this has to be the game exiting.
	/*
	if( CheckCanReadPipe() ){
		//const int result = ReadUCharFromPipe();
		//GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Received game stop (result=%d) from process %d", result, ( int )pProcessID );
		GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Game process %d exited", ( int )pProcessID );
		return 0;
	}
	*/
#endif
	
	// check if the game exited normally writing final information to us
	if( pGameCollectChangedParams ){
		if( CheckCanReadPipe() && ReadUCharFromPipe() == delEngineProcess::ercGameExited ){
			// read changed module parameters
			GetLauncher().GetLogger()->LogInfo( GetLauncher().GetLogSource(),
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
				
				module->GetParameters().Add( delGPMParameter::Ref::NewWith(paramName, paramValue) );
			}
			
			// write any byte to pipe to signal process can exit now
			GetLauncher().GetLogger()->LogInfo( GetLauncher().GetLogSource(), "sending acknowledge" );
			WriteUCharToPipe( 1 );
			GetLauncher().GetLogger()->LogInfo( GetLauncher().GetLogSource(), "running game finished" );
			
			pGameCollectChangedParams = nullptr;
			StopEngine();
			return 0; // exit
		}
	}
	
	return 1;
}

decPoint delEngineInstanceThreaded::GetDisplayCurrentResolution( int display ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

int delEngineInstanceThreaded::GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount ){
	if( resolutionCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "resolutionCount < 0" );
	}
	if( resolutionCount > 0 && ! resolutions ){
		DETHROW_INFO( deeInvalidParam, "resolutionCount > 0 with resolutions = nullptr" );
	}
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Sending eccGetDisplayResolutions(display=%d, resolutions=%d, resolutionCount=%d) to process %i",
		display, resolutions ? 1 : 0, resolutionCount, ( int )pProcessID );
	
	WriteUCharToPipe( delEngineProcess::eccGetDisplayResolutions );
	WriteUCharToPipe( display );
	WriteUCharToPipe( resolutionCount );
	
	if( ReadUCharFromPipe() != delEngineProcess::ercSuccess ){
		DETHROW( deeInvalidAction );
	}
	
	const int sendResolutionCount = ReadUCharFromPipe();
	if( resolutionCount > 0 ){
		if( sendResolutionCount > resolutionCount ){
			DETHROW( deeInvalidAction );
		}
		if( sendResolutionCount > 0 ){
			ReadFromPipe( resolutions, sizeof( decPoint ) * sendResolutionCount );
		}
	}
	
	return sendResolutionCount;
}

int delEngineInstanceThreaded::GetDisplayCurrentScaleFactor(int display){
	GetLauncher().GetLogger()->LogInfoFormat(GetLauncher().GetLogSource(),
		"Sending eccGetDisplayCurrentScaleFactor(display=%d) to process %d",
		display, (int)pProcessID );
	
	WriteUCharToPipe(delEngineProcess::eccGetDisplayCurrentScaleFactor);
	WriteUCharToPipe(display);
	
	if(ReadUCharFromPipe() != delEngineProcess::ercSuccess){
		DETHROW(deeInvalidAction);
	}
	
	return ReadUShortFromPipe();
}

void delEngineInstanceThreaded::ReadDelgaGameDefs( const char *delgaFile, decStringList &list ){
	// send command
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

void delEngineInstanceThreaded::ReadDelgaPatchDefs( const char *delgaFile, decStringList &list ){
	// send command
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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


void delEngineInstanceThreaded::ReadDelgaFiles( const char *delgaFile,
const decStringList &filenames, decObjectOrderedSet &filesContent ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
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

#endif // OS_ANDROID
