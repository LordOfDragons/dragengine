/* 
 * Drag[en]gine GUI Launcher
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

#if ! defined( OS_W32 ) || defined( OS_W32_HELPER )

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#include "deglEngineProcess.h"
#include "deglEngineProcessRunGame.h"
#include "../logger/deglLoggerWritePipe.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_UNIX
#	include <dragengine/app/deOSUnix.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_W32
#	include <dragengine/app/deOSWindows.h>
#else
#	error OS not supported!
#endif
#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveReference.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/systems/modules/ai/deBaseAIModule.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>
#include <dragengine/systems/modules/audio/deBaseAudioModule.h>
#include <dragengine/systems/modules/crashrecovery/deBaseCrashRecoveryModule.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/network/deBaseNetworkModule.h>
#include <dragengine/systems/modules/physics/deBasePhysicsModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>
#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerModule.h>



// Definitions
////////////////

#define LOGSOURCE "Engine Process"



#ifdef OS_W32
// entry point for windows32 helper binary
int main( int argc, char **args ){
	HANDLE pipeIn = INVALID_HANDLE_VALUE;
	HANDLE pipeOut = INVALID_HANDLE_VALUE;
	uint16_t logfileLength;
	decString logfile;
	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;
	char sync = 1;
	
//	FILE *w32ConH4xFile = freopen( "OOPS.log", "w", stdout );
//	if( ! w32ConH4xFile ) return -1;
//	setvbuf( w32ConH4xFile, NULL, _IONBF, 0 );
	
//	printf( "GO!\n" );
//	fflush( stdout );
	pipeIn = GetStdHandle( STD_INPUT_HANDLE );
//	printf( "using in pipe %p\n", pipeIn );
//	fflush( stdout );
	
	if( pipeIn == INVALID_HANDLE_VALUE ) return -1;
	
	try{
//		printf( "reading write pipe\n" );
//		fflush( stdout );
		// read out-pipe handle
		if( ! ReadFile( pipeIn, &pipeOut, sizeof( pipeOut ), &bytesRead, NULL ) ) DETHROW( deeInvalidParam );
		if( bytesRead < sizeof( pipeOut ) ) DETHROW( deeInvalidParam );
		
		// read log filename
//		printf( "reading log file length\n" );
//		fflush( stdout );
		if( ! ReadFile( pipeIn, &logfileLength, sizeof( logfileLength ), &bytesRead, NULL ) ) DETHROW( deeInvalidParam );
		if( bytesRead < sizeof( logfileLength ) ) DETHROW( deeInvalidParam );
		
//		printf( "reading log file name\n");
//		fflush( stdout );
		logfile.Set( ' ', logfileLength );
		if( ! ReadFile( pipeIn, ( char* )logfile.GetString(), ( int )logfileLength, &bytesRead, NULL ) ) DETHROW( deeInvalidParam );
		if( bytesRead < ( DWORD )logfileLength ) DETHROW( deeInvalidParam );
		
		// send sync
//		printf( "sending sync\n" );
//		fflush( stdout );
		if( ! WriteFile( pipeOut, &sync, 1, &bytesWritten, NULL ) ) DETHROW( deeInvalidAction );
		if( bytesWritten < 1 ) DETHROW( deeInvalidAction );
		
		// start process
//		printf( "start process\n");
//		fflush( stdout );
		deglEngineProcess process( pipeIn, pipeOut, logfile.GetString() );
		
		process.Run();
		
	}catch( const deException &e ){
//		e.PrintError();
//		printf( "GetLastError %x\n", ( unsigned int )GetLastError() );
		if( pipeIn != INVALID_HANDLE_VALUE ){
			CloseHandle( pipeIn );
		}
		if( pipeOut != INVALID_HANDLE_VALUE ){
			CloseHandle( pipeOut );
		}
		return -1;
	}
	
	CloseHandle( pipeIn );
	CloseHandle( pipeOut );
	return 0;
}
#endif



// Class deglEngineProcess
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

#ifdef OS_W32
deglEngineProcess::deglEngineProcess( HANDLE pipeIn, HANDLE pipeOut, const char *logfile ){
#else
deglEngineProcess::deglEngineProcess( int pipeIn, int pipeOut, const char *logfile ){
#endif
	if( ! logfile ) DETHROW( deeInvalidParam );
	
	pPipeIn = pipeIn;
	pPipeOut = pipeOut;
	pEngine = NULL;
	pEngineRunning = false;
	pStopProcess = true;
	pRunGame = NULL;
	pLogger = NULL;
	
	pCreateLogger( logfile );
}

deglEngineProcess::~deglEngineProcess(){
	StopEngine();
	if( pLogger ) pLogger->FreeReference();
}



// Management
///////////////

void deglEngineProcess::Run(){
	try{
		StartEngine();
		
		pStopProcess = false;
		ReadCommandsFromInPipe();
		
		StopEngine();
		
	}catch( const deException &e ){
		pLogger->LogException( LOGSOURCE, e );
		StopEngine();
	}
}

void deglEngineProcess::StartEngine(){
	if( pEngineRunning ){
		return;
	}
	
	deOS *os = NULL;
	bool useConsole = false;
	
	try{
		// create os
		if( useConsole ){
			#ifdef OS_UNIX
			os = new deOSConsole();
			#elif defined OS_W32
			os = new deOSWindows();
			#endif
			
		}else{
			#ifdef OS_BEOS
			os = new deOSBeOS();
			#elif defined OS_UNIX
				#ifdef HAS_LIB_X11
				os = new deOSUnix();
				#else
				os = new deOSConsole();
				#endif
			#elif defined OS_W32
			os = new deOSWindows();
			os->CastToOSWindows()->SetInstApp( GetModuleHandle( NULL ) );
			#endif
		}
		
		// create game engine
		pEngine = new deEngine( os );
		os = NULL;
		
		pEngine->SetLogger( pLogger );
		
		pEngineRunning = true;
		
	}catch( const deException &e ){
		pLogger->LogError( LOGSOURCE, "EngineProcess.StartEngine failed with exception:" );
		pLogger->LogException( LOGSOURCE, e );
		if( pEngine ){
			delete pEngine;
			pEngine = NULL;
		}
		if( os ){
			delete os;
		}
		throw;
	}
}

void deglEngineProcess::StopEngine(){
	if( pRunGame ){
		pRunGame->Stop();
		
		delete pRunGame;
		pRunGame = NULL;
	}
	
	if( pEngine ){
		delete pEngine;
		pEngine = NULL;
	}
	
	pEngineRunning = false;
}

void deglEngineProcess::ReadCommandsFromInPipe(){
	while( ! pStopProcess ){
		const eCommandCodes command = ( eCommandCodes )ReadUCharFromPipe();
		
		if( pRunGame ){ // this should never happen
			WriteUCharToPipe( ercFailed );
			continue;
		}
		
		switch( command ){
		case eccStopProcess:
			pLogger->LogInfo( LOGSOURCE, "Received eccStopProcess" );
			pStopProcess = true;
			break;
			
		case eccGetProperty:
			pLogger->LogInfo( LOGSOURCE, "Received eccGetProperty" );
			CommandGetProperty();
			break;
			
		case eccLoadModules:
			pLogger->LogInfo( LOGSOURCE, "Received eccLoadModules" );
			CommandLoadModules();
			break;
			
		case eccGetModuleStatus:
			pLogger->LogInfo( LOGSOURCE, "Received eccGetModuleStatus" );
			CommandGetModuleStatus();
			break;
			
		case eccGetModuleParamList:
			pLogger->LogInfo( LOGSOURCE, "Received eccGetModuleParamList" );
			CommandGetModuleParamList();
			break;
			
		case eccSetModuleParameter:
			pLogger->LogInfo( LOGSOURCE, "Received eccSetModuleParameter" );
			CommandSetModuleParameter();
			break;
			
		case eccActivateModule:
			pLogger->LogInfo( LOGSOURCE, "Received eccActivateModule" );
			CommandActivateModule();
			break;
			
		case eccEnableModule:
			pLogger->LogInfo( LOGSOURCE, "Received eccEnableModule" );
			CommandEnableModule();
			break;
			
		case eccSetDataDir:
			pLogger->LogInfo( LOGSOURCE, "Received eccSetDataDir" );
			CommandSetDataDir();
			break;
			
		case eccSetCacheAppID:
			pLogger->LogInfo( LOGSOURCE, "Received eccSetCacheAppID" );
			CommandSetCacheAppID();
			break;
			
		case eccVFSAddDiskDir:
			pLogger->LogInfo( LOGSOURCE, "Received eccVFSAddDiskDir" );
			CommandVFSAddDiskDir();
			break;
			
		case eccVFSAddScriptSharedDataDir:
			pLogger->LogInfo( LOGSOURCE, "Received eccVFSAddScriptSharedDataDir" );
			CommandVFSAddScriptSharedDataDir();
			break;
			
		case eccVFSAddDelgaFile:
			pLogger->LogInfo( LOGSOURCE, "Received eccVFSAddDelgaFile" );
			CommandVFSAddDelgaFile();
			break;
			
		case eccSetCmdLineArgs:
			pLogger->LogInfo( LOGSOURCE, "Received eccSetCmdLineArgs" );
			CommandSetCmdLineArgs();
			break;
			
		case eccCreateRenderWindow:
			pLogger->LogInfo( LOGSOURCE, "Received eccCreateRenderWindow" );
			CommandCreateRenderWindow();
			break;
			
		case eccStartGame:
			pLogger->LogInfo( LOGSOURCE, "Received eccStartGame" );
			CommandStartGame();
			break;
			
		case eccStopGame:
			pLogger->LogError( LOGSOURCE, "Received eccStopGame. Already stopped" );
			WriteUCharToPipe( ercFailed );
			break;
			
		case eccGetDisplayCurrentResolution:
			pLogger->LogInfo( LOGSOURCE, "Received eccGetDisplayCurrentResolution" );
			CommandGetDisplayCurrentResolution();
			break;
			
		case eccGetDisplayResolutions:
			pLogger->LogInfo( LOGSOURCE, "Received eccGetDisplayResolutions" );
			CommandGetDisplayResolutions();
			break;
			
		case eccReadDelgaGameDefs:
			pLogger->LogInfo( LOGSOURCE, "Received eccReadDelgaGameDefs" );
			CommandDelgaReadGameDefs();
			break;
			
		case eccReadDelgaPatchDefs:
			pLogger->LogInfo( LOGSOURCE, "Received eccReadDelgaPatchDefs" );
			CommandDelgaReadPatchDefs();
			break;
			
		case eccReadDelgaFiles:
			pLogger->LogInfo( LOGSOURCE, "Received eccReadDelgaFiles" );
			CommandDelgaReadFiles();
			break;
			
		default:
			pLogger->LogErrorFormat( LOGSOURCE,
				"ReadCommandsFromInPipe failed with exception (command=%i):", command );
			pLogger->LogInfoFormat( LOGSOURCE, "Received unknown command %i", command );
			WriteUCharToPipe( ercFailed );
		}
	}
}



void deglEngineProcess::WriteUCharToPipe( int value ){
	if( value < 0 || value > 0xff ){
		DETHROW( deeInvalidParam );
	}
	const uint8_t uchar = ( uint8_t )value;
	WriteToPipe( &uchar, sizeof( uint8_t ) );
}

void deglEngineProcess::WriteUShortToPipe( int value ){
	if( value < 0 || value > 0xffff ){
		DETHROW( deeInvalidParam );
	}
	const uint16_t ushort = ( uint16_t )value;
	WriteToPipe( &ushort, sizeof( uint16_t ) );
}

void deglEngineProcess::WriteIntToPipe( int value ){
	const int32_t intval = ( int32_t )value;
	WriteToPipe( &intval, sizeof( int32_t ) );
}

void deglEngineProcess::WriteFloatToPipe( float value ){
	WriteToPipe( &value, sizeof( float ) );
}

void deglEngineProcess::WriteString16ToPipe( const char *string ){
	const int length = strlen( string );
	WriteUShortToPipe( length );
	if( length > 0 ){
		WriteToPipe( string, length );
	}
}

void deglEngineProcess::WriteToPipe( const void *data, int length ){
	#ifdef OS_W32
	DWORD bytesWritten = 0;
	if( pPipeOut == INVALID_HANDLE_VALUE
	|| ! WriteFile( pPipeOut, data, length, &bytesWritten, NULL )
	|| ( int )bytesWritten != length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( ! pPipeOut || write( pPipeOut, data, length ) != length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}



int deglEngineProcess::ReadUCharFromPipe(){
	uint8_t uchar;
	ReadFromPipe( &uchar, sizeof( uint8_t ) );
	return uchar;
}

int deglEngineProcess::ReadUShortFromPipe(){
	uint16_t ushort;
	ReadFromPipe( &ushort, sizeof( uint16_t ) );
	return ushort;
}

int deglEngineProcess::ReadFloatFromPipe(){
	float value;
	ReadFromPipe( &value, sizeof( float ) );
	return value;
}

void deglEngineProcess::ReadString16FromPipe( decString &string ){
	const int length = ReadUShortFromPipe();
	string.Set( ' ', length );
	if( length > 0 ){
		ReadFromPipe( ( char* )string.GetString(), length );
	}
}

void deglEngineProcess::ReadFromPipe( void *data, int length ){
	#ifdef OS_W32
	DWORD bytesRead = 0;
	if( pPipeIn == INVALID_HANDLE_VALUE
	|| ! ReadFile( pPipeIn, data, length, &bytesRead, NULL )
	|| ( int )bytesRead != length ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	if( ! pPipeIn || read( pPipeIn, data, length ) != length ){
		DETHROW( deeInvalidAction );
	}
	#endif
}


void deglEngineProcess::CommandGetProperty(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	unsigned char property = -1;
	
	try{
		property = ReadUCharFromPipe();
		
		switch( property ){
		case epPathEngineConfig:
			WriteUCharToPipe( ercSuccess );
			WriteString16ToPipe( pEngine->GetOS()->GetPathUserConfig() );
			return;
			
		case epPathEngineShare:
			WriteUCharToPipe( ercSuccess );
			WriteString16ToPipe( pEngine->GetOS()->GetPathShare() );
			return;
			
		case epPathEngineLib:
			WriteUCharToPipe( ercSuccess );
			WriteString16ToPipe( pEngine->GetOS()->GetPathEngine() );
			return;
			
		case epPathEngineCache:
			WriteUCharToPipe( ercSuccess );
			WriteString16ToPipe( pEngine->GetOS()->GetPathUserCache() );
			return;
			
		default:
			break;
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetProperty "
			"failed with exception (property=%i):", property );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandLoadModules(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	try{
		pEngine->LoadModules();
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogError( LOGSOURCE, "EngineProcess.CommandLoadModules failed with exception:" );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandGetModuleStatus(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deLoadableModule *module = NULL;
	decString moduleVersion;
	decString moduleName;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		
		if( ! module ){
			pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetModuleStatus "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			
		}else{
			WriteUCharToPipe( ercSuccess );
			WriteUShortToPipe( module->GetErrorCode() );
			return;
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetModuleStatus "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandGetModuleParamList(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString moduleVersion;
	decString moduleName;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		
		const deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
		if( ! module ){
			pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetModuleParamList "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			WriteUCharToPipe( ercFailed );
			return;
		}
		
		const deBaseModule * const baseModule = module->GetModule();
		if( ! baseModule ){
			WriteUCharToPipe( ercFailed );
			return;
		}
		
		const int count = baseModule->GetParameterCount();
		int i, j;
		
		WriteUCharToPipe( ercSuccess );
		WriteUShortToPipe( count );
		for( i=0; i<count; i++ ){
			deModuleParameter parameter;
			baseModule->GetParameterInfo( i, parameter );
			
			WriteString16ToPipe( parameter.GetName() );
			WriteString16ToPipe( parameter.GetDescription() );
			WriteUCharToPipe( parameter.GetType() );
			WriteFloatToPipe( parameter.GetMinimumValue() );
			WriteFloatToPipe( parameter.GetMaximumValue() );
			WriteFloatToPipe( parameter.GetValueStepSize() );
			WriteUCharToPipe( parameter.GetCategory() );
			WriteString16ToPipe( parameter.GetDisplayName() );
			
			const int entryCount = parameter.GetSelectionEntryCount();
			WriteUShortToPipe( entryCount );
			for( j=0; j<entryCount; j++ ){
				const deModuleParameter::SelectionEntry &entry = parameter.GetSelectionEntryAt( j );
				WriteString16ToPipe( entry.value );
				WriteString16ToPipe( entry.displayName );
				WriteString16ToPipe( entry.description );
			}
			
			WriteString16ToPipe( baseModule->GetParameterValue( parameter.GetName() ) );
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetModuleParamList "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		WriteUCharToPipe( ercFailed );
	}
}

void deglEngineProcess::CommandSetModuleParameter(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deLoadableModule *module = NULL;
	decString moduleVersion;
	decString moduleName;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		
		if( moduleVersion.IsEmpty() ){
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
			
		}else{
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		}
		
		if( ! module ){
			pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandSetModuleParameter "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			
		}else{
			deBaseModule * const baseModule = module->GetModule();
			
			if( baseModule ){
				decString parameter, value;
				ReadString16FromPipe( parameter );
				ReadString16FromPipe( value );
				
				baseModule->SetParameterValue( parameter, value );
				
				WriteUCharToPipe( ercSuccess );
				
				return;
			}
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandSetModuleParameter "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandActivateModule(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deLoadableModule *module = NULL;
	decString moduleName;
	decString moduleVersion;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		
		if( moduleVersion.IsEmpty() ){
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
			
		}else{
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		}
		
		if( ! module ){
			pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandActivateModule "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			WriteUCharToPipe( ercFailed );
			return;
		}
		
		switch( module->GetType() ){
		case deModuleSystem::emtGraphic:
			pEngine->GetGraphicSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtAudio:
			pEngine->GetAudioSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtInput:
			pEngine->GetInputSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtNetwork:
			pEngine->GetNetworkSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtPhysics:
			pEngine->GetPhysicsSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtAnimator:
			pEngine->GetAnimatorSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtAI:
			pEngine->GetAISystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtCrashRecovery:
			pEngine->GetCrashRecoverySystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtSynthesizer:
			pEngine->GetSynthesizerSystem()->SetActiveModule( module );
			break;
			
		case deModuleSystem::emtScript:
			pEngine->GetScriptingSystem()->SetActiveModule( module );
			break;
			
		default:
			WriteUCharToPipe( ercFailed );
			return;
		}
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandActivateModule "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandEnableModule(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deLoadableModule *module = NULL;
	decString moduleName;
	decString moduleVersion;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		const bool enable = ( ReadUCharFromPipe() != 0 );
		
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		
		if( ! module ){
			pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandEnableModule "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			
		}else{
			module->SetEnabled( enable );
			
			WriteUCharToPipe( ercSuccess );
			return;
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandActivateModule "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}

	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandSetDataDir(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString directory;
	
	try{
		ReadString16FromPipe( directory );
		pEngine->SetDataDir( directory );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandSetDataDir "
			"failed with exception (dir=%s):", directory.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandSetCacheAppID(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString cacheAppID;
	
	try{
		ReadString16FromPipe( cacheAppID );
		pEngine->SetCacheAppID( cacheAppID );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandSetCacheAppID "
			"failed with exception (dir=%s):", cacheAppID.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandVFSAddDiskDir(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deVFSDiskDirectory *container = NULL;
	decPath pathRoot, pathDisk;
	bool readOnly = false;
	decString root, disk;
	
	try{
		ReadString16FromPipe( root );
		ReadString16FromPipe( disk );
		readOnly = ( ReadUCharFromPipe() != 0 );
		
		pathRoot.SetFromUnix( root.GetString() );
		pathDisk.SetFromNative( disk.GetString() );
		
		container = new deVFSDiskDirectory( pathRoot, pathDisk );
		container->SetReadOnly( readOnly );
		vfs.AddContainer( container );
		container->FreeReference();
		container = NULL;
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		if( container ){
			container->FreeReference();
		}
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandVFSAddDiskDir "
			"failed with exception (root=%s,disk=%s,ro=%c):", pathRoot.GetPathNative().GetString(), pathDisk.GetPathUnix().GetString(), readOnly ? 't' : 'n' );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandVFSAddScriptSharedDataDir(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	
	try{
		pEngine->GetScriptingSystem()->AddVFSSharedDataDir( vfs );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandVFSAddScriptSharedDataDir "
			"failed with exception:" );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandVFSAddDelgaFile(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	deVFSDiskDirectory *delgaVfsDelgaDir = NULL;
	deArchiveContainer *delgaContainer = NULL;
	deVirtualFileSystem *delgaVfs = NULL;
	deArchive *delgaArchive = NULL;
	decString archivePath;
	decString delgaFile;
	
	try{
		ReadString16FromPipe( delgaFile );
		ReadString16FromPipe( archivePath );
		
		// add delga file archive container
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		
		delgaVfs = new deVirtualFileSystem;
		
		delgaVfsDelgaDir = new deVFSDiskDirectory( pathDelgaDir );
		delgaVfs->AddContainer( delgaVfsDelgaDir );
		delgaVfsDelgaDir->FreeReference();
		delgaVfsDelgaDir = NULL;
		
		delgaArchive = amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" );
		delgaVfs->FreeReference();
		delgaVfs = NULL;
		
		delgaContainer = amgr.CreateContainer( decPath::CreatePathUnix( "/" ),
			delgaArchive, decPath::CreatePathUnix( archivePath ) );
		delgaArchive->FreeReference();
		delgaArchive = NULL;
		
		vfs.AddContainer( delgaContainer );
		delgaContainer->FreeReference();
		delgaContainer = NULL;
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		if( delgaContainer ){
			delgaContainer->FreeReference();
		}
		if( delgaArchive ){
			delgaArchive->FreeReference();
		}
		if( delgaVfsDelgaDir ){
			delgaVfsDelgaDir->FreeReference();
		}
		if( delgaVfs ){
			delgaVfs->FreeReference();
		}
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandVFSAddDelgaFile "
			"failed with exception (delgaFile=%s, archivePath=%s):",
			delgaFile.GetString(), archivePath.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandSetCmdLineArgs(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString cmdLineArgs;
	
	try{
		ReadString16FromPipe( cmdLineArgs );
		
		pEngine->GetArguments()->AddArgsSplit( cmdLineArgs.GetString() );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandSetCmdLineArgs "
			"failed with exception (args=%s):", cmdLineArgs.GetString() );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandCreateRenderWindow(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deGraphicSystem &grasys = *pEngine->GetGraphicSystem();
	decString windowTitle;
	decString iconPath;
	bool fullScreen = false;
	int height = 0;
	int width = 0;
	
	try{
		width = ReadUShortFromPipe();
		height = ReadUShortFromPipe();
		fullScreen = ( ReadUCharFromPipe() != 0 );
		ReadString16FromPipe( windowTitle );
		ReadString16FromPipe( iconPath );
		
		deImageReference icon;
		if( ! iconPath.IsEmpty() ){
			try{
				icon.TakeOver( pEngine->GetImageManager()->LoadImage( iconPath, "/" ) );
				
			}catch( const deException &e ){
				icon = NULL;
				pLogger->LogException( LOGSOURCE, e );
			}
		}
		
		grasys.CreateAndSetRenderWindow( width, height, fullScreen, windowTitle.GetString(), icon );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandCreateRenderWindow "
			" failed with exception (width=%i,height=%i,title=%s,fullScreen=%c):",
			width, height, windowTitle.GetString(), fullScreen );
		pLogger->LogException( LOGSOURCE, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void deglEngineProcess::CommandStartGame(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	// prepare for running game. this part sends back the result through the pipe
	decString scriptDirectory;
	decString gameObject;
	
	try{
		ReadString16FromPipe( scriptDirectory );
		ReadString16FromPipe( gameObject );
		
		// create a thread processing the in pipe while we are spinning the game loop
		pRunGame = new deglEngineProcessRunGame( this );
		if( ! pRunGame ){
			DETHROW( deeOutOfMemory );
		}
		
		// now we are ready to start the thread. send success code before thread takes over
		pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame "
			"launching in pipe processing thread" );
		WriteUCharToPipe( ercSuccess );
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandStartGame "
			"failed with exception (scriptDir=%s, gameObject=%s):",
			scriptDirectory.GetString(), gameObject.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		
		if( pRunGame ){
			pRunGame->Stop();
			delete pRunGame;
			pRunGame = NULL;
		}
		
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	// store single type module parameters to compare after engine exits if user changed them
	struct sModuleParamState{
		const deBaseModule *module;
		decStringDictionary parameters;
	};
	sModuleParamState moduleState[ 10 ] = {
		{ pEngine->GetAISystem()->GetActiveModule() },
		{ pEngine->GetAnimatorSystem()->GetActiveModule() },
		{ pEngine->GetAudioSystem()->GetActiveModule() },
		{ pEngine->GetCrashRecoverySystem()->GetActiveModule() },
		{ pEngine->GetGraphicSystem()->GetActiveModule() },
		{ pEngine->GetInputSystem()->GetActiveModule() },
		{ pEngine->GetNetworkSystem()->GetActiveModule() },
		{ pEngine->GetPhysicsSystem()->GetActiveModule() },
		{ pEngine->GetScriptingSystem()->GetActiveModule() },
		{ pEngine->GetSynthesizerSystem()->GetActiveModule() } };
	deModuleParameter moduleParameter;
	int i, j;
	
	for( i=0; i<10; i++ ){
		if( moduleState[ i ].module ){
			const int count = moduleState[ i ].module->GetParameterCount();
			for( j=0; j<count; j++ ){
				moduleState[ i ].module->GetParameterInfo( j, moduleParameter );
				moduleState[ i ].parameters.SetAt( moduleParameter.GetName(),
					moduleState[ i ].module->GetParameterValue( moduleParameter.GetName() ) );
			}
		}
	}
	
	// run game. this part does not send anything back to the pipe unless a command is received
	try{
		pRunGame->Start();
		
		pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame launching game" );
		pEngine->Run( scriptDirectory, gameObject );
		
		// when we get here this can be either because the player quit the game or
		// because we received a quit request from the launcher. either way stop the
		// thread but do not interact with the pipe. we use exit code for this.
		pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame game excited,"
			" stopping in-pipe processing thread" );
		pRunGame->Stop();
		delete pRunGame;
		pRunGame = NULL;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandStartGame "
			"failed with exception (scriptDir=%s, gameObject=%s):",
			scriptDirectory.GetString(), gameObject.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		if( pRunGame ){
			pRunGame->Stop();
			delete pRunGame;
			pRunGame = NULL;
		}
	}
	
	// compare module parameters against stored ones and send back those which changed
	pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame sending module parameter changes" );
	WriteUCharToPipe( ercGameExited );
	
	for( i=0; i<10; i++ ){
		if( moduleState[ i ].module ){
			const decStringList keys( moduleState[ i ].parameters.GetKeys() );
			const int count = keys.GetCount();
			for( j=0; j<count; j++ ){
				const decString &name = keys.GetAt( j );
				const decString value( moduleState[ i ].module->GetParameterValue( name ) );
				if( value != moduleState[ i ].parameters.GetAt( name ) ){
					WriteString16ToPipe( moduleState[ i ].module->GetLoadableModule().GetName() );
					WriteString16ToPipe( name );
					WriteString16ToPipe( value );
				}
			}
		}
	}
	
	WriteString16ToPipe( "" ); // end of list marker
	
	// wait for response
	pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame waiting for acknowledge" );
	ReadUCharFromPipe();
	
	// shut down process
	pLogger->LogInfo( LOGSOURCE, "EngineProcess.CommandStartGame acknowledge received, exiting process" );
	pStopProcess = true;
}

void deglEngineProcess::CommandGetDisplayCurrentResolution(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	int display = 0;
	
	try{
		display = ReadUCharFromPipe();
		
		const decPoint resolution( pEngine->GetOS()->GetDisplayCurrentResolution( display ) );
		
		WriteUCharToPipe( ercSuccess );
		WriteToPipe( &resolution, sizeof( decPoint ) );
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetDisplayCurrentResolution "
			"failed with exception (display=%d):", display );
		pLogger->LogException( LOGSOURCE, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void deglEngineProcess::CommandGetDisplayResolutions(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decPoint *resolutions = NULL;
	int display = 0;
	int count = 0;
	
	try{
		display = ReadUCharFromPipe();
		count = ReadUCharFromPipe();
		
		const int resolutionCount = decMath::min(
			pEngine->GetOS()->GetDisplayResolutionCount( display ), 255 );
		
		if( count == 0 ){
			WriteUCharToPipe( ercSuccess );
			WriteUCharToPipe( resolutionCount );
			return;
		}
		
		resolutions = new decPoint[ resolutionCount ];
		int i;
		for( i=0; i<resolutionCount; i++ ){
			resolutions[ i ] = pEngine->GetOS()->GetDisplayResolution( display, i );
		}
		
		WriteUCharToPipe( ercSuccess );
		WriteUCharToPipe( resolutionCount );
		WriteToPipe( resolutions, sizeof( decPoint ) * resolutionCount );
		
		delete [] resolutions;
		
	}catch( const deException &e ){
		if( resolutions ){
			delete [] resolutions;
		}
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandGetDisplayResolutions "
			" failed with exception (display=%d, count=%d):", display, count );
		pLogger->LogException( LOGSOURCE, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void deglEngineProcess::CommandDelgaReadGameDefs(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVFSContainerReference delgaContainer;
	deVirtualFileSystemReference delgaVfs;
	decBaseFileReaderReference reader;
	deArchiveReference delgaArchive;
	decString gameDef;
	decString delgaFilename;
	decStringList gameDefs;
	
	try{
		ReadString16FromPipe( delgaFilename );
		
		// open delga file
		deArchiveManager &amgr = *pEngine->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( new deVFSDiskDirectory( pathDelgaDir ) );
		
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) );
		
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		container.TakeOver( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) );
		vfs->AddContainer( container );
		
		deCollectFileSearchVisitor collect( "*.degame", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		const int fileCount = files.GetCount();
		int i;
		for( i=0; i<fileCount; i++ ){
			const decPath &path = files.GetAt( i );
			
			reader.TakeOver( vfs->OpenFileForReading( path ) );
			const int size = reader->GetLength();
			gameDef.Set( ' ', size );
			reader->Read( ( void* )gameDef.GetString(), size );
			
			gameDefs.Add( gameDef );
		}
		
		// send back result
		const int gameDefCount = gameDefs.GetCount();
		WriteUCharToPipe( ercSuccess );
		WriteUShortToPipe( gameDefCount );
		for( i=0; i<gameDefCount; i++ ){
			WriteString16ToPipe( gameDefs.GetAt( i ) );
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandDelgaReadGameDefs "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void deglEngineProcess::CommandDelgaReadPatchDefs(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVFSContainerReference delgaContainer;
	deVirtualFileSystemReference delgaVfs;
	decBaseFileReaderReference reader;
	deArchiveReference delgaArchive;
	decString patchDef;
	decString delgaFilename;
	decStringList patchDefs;
	
	try{
		ReadString16FromPipe( delgaFilename );
		
		// open delga file
		deArchiveManager &amgr = *pEngine->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( new deVFSDiskDirectory( pathDelgaDir ) );
		
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) );
		
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		container.TakeOver( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) );
		vfs->AddContainer( container );
		
		deCollectFileSearchVisitor collect( "*.depatch", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		const int fileCount = files.GetCount();
		int i;
		for( i=0; i<fileCount; i++ ){
			const decPath &path = files.GetAt( i );
			
			reader.TakeOver( vfs->OpenFileForReading( path ) );
			const int size = reader->GetLength();
			patchDef.Set( ' ', size );
			reader->Read( ( void* )patchDef.GetString(), size );
			
			patchDefs.Add( patchDef );
		}
		
		// send back result
		const int patchDefCount = patchDefs.GetCount();
		WriteUCharToPipe( ercSuccess );
		WriteUShortToPipe( patchDefCount );
		for( i=0; i<patchDefCount; i++ ){
			WriteString16ToPipe( patchDefs.GetAt( i ) );
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandDelgaReadPatchDefs "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void deglEngineProcess::CommandDelgaReadFiles(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVFSContainerReference delgaContainer;
	deVirtualFileSystemReference delgaVfs;
	decBaseFileReaderReference reader;
	deArchiveReference delgaArchive;
	decObjectOrderedSet filesContent;
	decStringList filenames;
	decString delgaFilename;
	
	try{
		ReadString16FromPipe( delgaFilename );
		
		const int fileCount = ReadUCharFromPipe();
		int i;
		for( i=0; i<fileCount; i++ ){
			decString filename;
			ReadString16FromPipe( filename );
			filenames.Add( filename );
		}
		
		// open delga file
		deArchiveManager &amgr = *pEngine->GetArchiveManager();
		const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		delgaVfs.TakeOver( new deVirtualFileSystem );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( new deVFSDiskDirectory( pathDelgaDir ) );
		
		delgaArchive.TakeOver( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) );
		
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		container.TakeOver( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) );
		vfs->AddContainer( container );
		
		// read files
		for( i=0; i<fileCount; i++ ){
			const decString &filename = filenames.GetAt( i );
			reader.TakeOver( vfs->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
			const int size = reader->GetLength();
			decMemoryFileReference content;
			content.TakeOver( new decMemoryFile( filename ) );
			content->Resize( size );
			reader->Read( content->GetPointer(), size );
			filesContent.Add( ( decMemoryFile* )content );
		}
		
		// send back result
		WriteUCharToPipe( ercSuccess );
		WriteUCharToPipe( fileCount );
		for( i=0; i<fileCount; i++ ){
			const decMemoryFile &content = *( ( decMemoryFile* )filesContent.GetAt( i ) );
			WriteIntToPipe( content.GetLength() );
			WriteToPipe( content.GetPointer(), content.GetLength() );
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( LOGSOURCE, "EngineProcess.CommandDelgaFiles "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( LOGSOURCE, e );
		
		WriteUCharToPipe( ercFailed );
	}
}



// Private Functions
//////////////////////

void deglEngineProcess::pCreateLogger( const char *logfile ){
	if( strlen( logfile ) == 0 ){
		pLogger = new deLoggerConsoleColor;
		if( ! pLogger ) DETHROW( deeOutOfMemory );
		
	}else{
		decBaseFileWriter *fileWriter = NULL;
		decPath diskPath;
		decPath filePath;
		
		diskPath.SetFromNative( logfile );
		filePath.AddComponent( diskPath.GetLastComponent() );
		diskPath.RemoveLastComponent();
		
		deVFSDiskDirectory *diskDir = NULL;
		
		try{
			diskDir = new deVFSDiskDirectory( diskPath );
			/*
			if( diskDir->ExistsFile( filePath ) ){
				//fileWriter = new decDiskFileWriter( logfile, true );
				
			}else{
				fileWriter = diskDir->OpenFileForWriting( filePath );
			}
			*/
			fileWriter = diskDir->OpenFileForWriting( filePath );
			
			pLogger = new deLoggerFile( fileWriter );
			fileWriter->FreeReference();
			
			diskDir->FreeReference();
			
		}catch( const deException & ){
			if( fileWriter ){
				fileWriter->FreeReference();
			}
			if( diskDir ){
				diskDir->FreeReference();
			}
			throw;
		}
	}
}

#endif // ! OS_W32 || OS_W32_HELPER
