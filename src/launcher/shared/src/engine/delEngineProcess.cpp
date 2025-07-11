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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#ifdef OS_UNIX
#include <unistd.h>
#endif

#include "delEngineProcess.h"
#include "delEngineProcessRunGame.h"
#include "../logger/delLoggerWritePipe.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_WEBWASM
#	include <dragengine/app/deOSWebWasm.h>
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
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/resources/archive/deArchive.h>
#include <dragengine/resources/archive/deArchiveContainer.h>
#include <dragengine/resources/archive/deArchiveManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
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
#include <dragengine/systems/deVRSystem.h>
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
#include <dragengine/systems/modules/vr/deBaseVRModule.h>



// Class delEngineProcess
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

#ifdef OS_W32
delEngineProcess::delEngineProcess( HANDLE pipeIn, HANDLE pipeOut, const char *logfile, const char *logSource ) :
#else
delEngineProcess::delEngineProcess( int pipeIn, int pipeOut, const char *logfile, const char *logSource ) :
#endif
pPipeIn( pipeIn ),
pPipeOut( pipeOut ),

pEngine( nullptr ),
pEngineRunning( false ),
pStopProcess( true ),
pStopGame( false ),
pRunGame( nullptr ),

pLogSource( logSource ),
pUseConsole( false )
{
	pCreateLogger( logfile );
}

delEngineProcess::~delEngineProcess(){
	StopEngine();
}



// Management
///////////////

void delEngineProcess::SetUseConsole( bool useConsole ){
	pUseConsole = useConsole;
}

void delEngineProcess::SetStopGame(){
	pStopGame = true;
}

void delEngineProcess::Run(){
	try{
		StartEngine();
		
		pStopProcess = false;
		pStopGame = false;
		ReadCommandsFromInPipe();
		
		StopEngine();
		
	}catch( const deException &e ){
		pLogger->LogException( pLogSource, e );
		StopEngine();
	}
}

void delEngineProcess::StartEngine(){
	if( pEngineRunning ){
		return;
	}
	
	deOS *os = nullptr;
	
	try{
		// create os
		if( pUseConsole ){
			#if defined OS_ANDROID || defined OS_WEBWASM
				DETHROW_INFO(deeInvalidAction, "not supported");
			#elif defined OS_UNIX
				pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS Console (console requested)" );
				os = new deOSConsole();
			#elif defined OS_W32
				pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS Windows (console requested)" );
				os = new deOSWindows();
			#endif
			
		}else{
			#ifdef OS_BEOS
				pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS BeOS" );
				os = new deOSBeOS();
			#elif defined OS_ANDROID
				DETHROW_INFO(deeInvalidAction, "not supported");
			#elif defined OS_WEBWASM
				DETHROW_INFO(deeInvalidAction, "not supported");
			#elif defined OS_UNIX
				#ifdef HAS_LIB_X11
				pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS Unix" );
				os = new deOSUnix();
				#else
				pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS Console" );
				os = new deOSConsole();
				#endif
			#elif defined OS_W32
			pLogger->LogInfo( pLogSource, "EngineProcess.StartEngine: Create OS Windows" );
			os = new deOSWindows();
			os->CastToOSWindows()->SetInstApp( GetModuleHandle( NULL ) );
			#endif
		}
		
		// create game engine
		pEngine = new deEngine( os );
		os = nullptr;
		
		pEngine->SetLogger( pLogger );
		
		pEngineRunning = true;
		
	}catch( const deException &e ){
		pLogger->LogError( pLogSource, "EngineProcess.StartEngine failed with exception:" );
		pLogger->LogException( pLogSource, e );
		if( pEngine ){
			delete pEngine;
			pEngine = nullptr;
		}
		if( os ){
			delete os;
		}
		throw;
	}
}

void delEngineProcess::StopEngine(){
	if( pRunGame ){
		pRunGame->Stop();
		
		delete pRunGame;
		pRunGame = nullptr;
	}
	
	if( pEngine ){
		delete pEngine;
		pEngine = nullptr;
	}
	
	pEngineRunning = false;
}

void delEngineProcess::ReadCommandsFromInPipe(){
	while( ! pStopProcess ){
		const eCommandCodes command = ( eCommandCodes )ReadUCharFromPipe();
		
		if( pRunGame ){ // this should never happen
			WriteUCharToPipe( ercFailed );
			continue;
		}
		
		switch( command ){
		case eccStopProcess:
			pLogger->LogInfo( pLogSource, "Received eccStopProcess" );
			pStopProcess = true;
			break;
			
		case eccGetProperty:
			pLogger->LogInfo( pLogSource, "Received eccGetProperty" );
			CommandGetProperty();
			break;
			
		case eccLoadModules:
			pLogger->LogInfo( pLogSource, "Received eccLoadModules" );
			CommandLoadModules();
			break;
			
		case eccGetModuleStatus:
			pLogger->LogInfo( pLogSource, "Received eccGetModuleStatus" );
			CommandGetModuleStatus();
			break;
			
		case eccGetInternalModules:
			pLogger->LogInfo( pLogSource, "Received eccGetInternalModules" );
			CommandGetInternalModules();
			break;
			
		case eccGetModuleParamList:
			pLogger->LogInfo( pLogSource, "Received eccGetModuleParamList" );
			CommandGetModuleParamList();
			break;
			
		case eccSetModuleParameter:
			pLogger->LogInfo( pLogSource, "Received eccSetModuleParameter" );
			CommandSetModuleParameter();
			break;
			
		case eccActivateModule:
			pLogger->LogInfo( pLogSource, "Received eccActivateModule" );
			CommandActivateModule();
			break;
			
		case eccEnableModule:
			pLogger->LogInfo( pLogSource, "Received eccEnableModule" );
			CommandEnableModule();
			break;
			
		case eccSetDataDir:
			pLogger->LogInfo( pLogSource, "Received eccSetDataDir" );
			CommandSetDataDir();
			break;
			
		case eccSetCacheAppID:
			pLogger->LogInfo( pLogSource, "Received eccSetCacheAppID" );
			CommandSetCacheAppID();
			break;
			
		case eccVFSAddDiskDir:
			pLogger->LogInfo( pLogSource, "Received eccVFSAddDiskDir" );
			CommandVFSAddDiskDir();
			break;
			
		case eccVFSAddScriptSharedDataDir:
			pLogger->LogInfo( pLogSource, "Received eccVFSAddScriptSharedDataDir" );
			CommandVFSAddScriptSharedDataDir();
			break;
			
		case eccVFSAddDelgaFile:
			pLogger->LogInfo( pLogSource, "Received eccVFSAddDelgaFile" );
			CommandVFSAddDelgaFile();
			break;
			
		case eccModulesAddVFSContainers:
			pLogger->LogInfo( pLogSource, "Received eccModulesAddVFSContainers" );
			CommandModulesAddVFSContainers();
			break;
			
		case eccSetCmdLineArgs:
			pLogger->LogInfo( pLogSource, "Received eccSetCmdLineArgs" );
			CommandSetCmdLineArgs();
			break;
			
		case eccCreateRenderWindow:
			pLogger->LogInfo( pLogSource, "Received eccCreateRenderWindow" );
			CommandCreateRenderWindow();
			break;
			
		case eccStartGame:
			pLogger->LogInfo( pLogSource, "Received eccStartGame" );
			CommandStartGame();
			break;
			
		case eccStopGame:
			pLogger->LogError( pLogSource, "Received eccStopGame. Already stopped" );
			WriteUCharToPipe( ercFailed );
			break;
			
		case eccGetDisplayCurrentResolution:
			pLogger->LogInfo( pLogSource, "Received eccGetDisplayCurrentResolution" );
			CommandGetDisplayCurrentResolution();
			break;
			
		case eccGetDisplayResolutions:
			pLogger->LogInfo( pLogSource, "Received eccGetDisplayResolutions" );
			CommandGetDisplayResolutions();
			break;
			
		case eccGetDisplayCurrentScaleFactor:
			pLogger->LogInfo(pLogSource, "Received eccGetDisplayCurrentScaleFactor");
			CommandGetDisplayCurrentScaleFactor();
			break;
			
		case eccReadDelgaGameDefs:
			pLogger->LogInfo( pLogSource, "Received eccReadDelgaGameDefs" );
			CommandDelgaReadGameDefs();
			break;
			
		case eccReadDelgaPatchDefs:
			pLogger->LogInfo( pLogSource, "Received eccReadDelgaPatchDefs" );
			CommandDelgaReadPatchDefs();
			break;
			
		case eccReadDelgaFiles:
			pLogger->LogInfo( pLogSource, "Received eccReadDelgaFiles" );
			CommandDelgaReadFiles();
			break;
			
		case eccSetPathOverlay:
			pLogger->LogInfo( pLogSource, "Received eccSetPathOverlay" );
			CommandSetPathOverlay();
			break;
			
		case eccSetPathCapture:
			pLogger->LogInfo( pLogSource, "Received eccSetPathCapture" );
			CommandSetPathCapture();
			break;
			
		case eccSetPathConfig:
			pLogger->LogInfo( pLogSource, "Received eccSetPathConfig" );
			CommandSetPathConfig();
			break;
			
		default:
			pLogger->LogErrorFormat( pLogSource,
				"ReadCommandsFromInPipe failed with exception (command=%d):", command );
			pLogger->LogInfoFormat( pLogSource, "Received unknown command %d", command );
			WriteUCharToPipe( ercFailed );
		}
	}
}



void delEngineProcess::WriteUCharToPipe( int value ){
	if( value < 0 || value > 0xff ){
		DETHROW( deeInvalidParam );
	}
	const uint8_t wpuchar = ( uint8_t )value;
	WriteToPipe( &wpuchar, sizeof( uint8_t ) );
}

void delEngineProcess::WriteUShortToPipe( int value ){
	if( value < 0 || value > 0xffff ){
		DETHROW( deeInvalidParam );
	}
	const uint16_t wpushort = ( uint16_t )value;
	WriteToPipe( &wpushort, sizeof( uint16_t ) );
}

void delEngineProcess::WriteIntToPipe( int value ){
	const int32_t intval = ( int32_t )value;
	WriteToPipe( &intval, sizeof( int32_t ) );
}

void delEngineProcess::WriteFloatToPipe( float value ){
	WriteToPipe( &value, sizeof( float ) );
}

void delEngineProcess::WriteString16ToPipe( const char *string ){
	const int length = ( int )strlen( string );
	WriteUShortToPipe( length );
	if( length > 0 ){
		WriteToPipe( string, length );
	}
}

void delEngineProcess::WriteToPipe( const void *data, int length ){
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



int delEngineProcess::ReadUCharFromPipe(){
	uint8_t value;
	ReadFromPipe( &value, sizeof( uint8_t ) );
	return value;
}

int delEngineProcess::ReadUShortFromPipe(){
	uint16_t value;
	ReadFromPipe( &value, sizeof( uint16_t ) );
	return value;
}

float delEngineProcess::ReadFloatFromPipe(){
	float value;
	ReadFromPipe( &value, sizeof( float ) );
	return value;
}

void delEngineProcess::ReadString16FromPipe( decString &string ){
	const int length = ReadUShortFromPipe();
	string.Set( ' ', length );
	if( length > 0 ){
		ReadFromPipe( ( char* )string.GetString(), length );
	}
}

void delEngineProcess::ReadFromPipe( void *data, int length ){
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


void delEngineProcess::CommandGetProperty(){
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetProperty "
			"failed with exception (property=%d):", property );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandLoadModules(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	try{
		pEngine->LoadModules();
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogError( pLogSource, "EngineProcess.CommandLoadModules failed with exception:" );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandGetModuleStatus(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString moduleVersion;
	decString moduleName;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		const deLoadableModule * const module = pEngine->GetModuleSystem()->
			GetModuleNamed( moduleName, moduleVersion );
		
		if( ! module ){
			pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetModuleStatus "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			
		}else{
			WriteUCharToPipe( ercSuccess );
			WriteUShortToPipe( module->GetErrorCode() );
			return;
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetModuleStatus "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandGetInternalModules(){
	if(!pEngine){
		WriteUCharToPipe(ercFailed);
		return;
	}
	
	const deModuleSystem &modsys = *pEngine->GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i, foundCount = 0;
	
	for(i=0; i<count; i++){
		deLoadableModule &mod = *modsys.GetModuleAt(i);
		if(mod.IsInternalModule()){
			foundCount++;
		}
	}
	
	WriteUCharToPipe(ercSuccess);
	WriteUShortToPipe(foundCount);
	
	for(i=0; i<count; i++){
		const deLoadableModule &mod = *modsys.GetModuleAt(i);
		if(!mod.IsInternalModule()){
			continue;
		}
		
		WriteUCharToPipe((int)mod.GetType());
		WriteString16ToPipe(mod.GetName());
		WriteString16ToPipe(mod.GetDescription());
		WriteString16ToPipe(mod.GetAuthor());
		WriteString16ToPipe(mod.GetVersion());
		WriteString16ToPipe(mod.GetDirectoryName());
		WriteString16ToPipe(mod.GetPatternList().Join(","));
		WriteUShortToPipe(mod.GetPriority());
		WriteUCharToPipe(mod.GetIsFallback() ? 1 : 0);
	}
}

void delEngineProcess::CommandGetModuleParamList(){
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
			pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetModuleParamList "
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
			WriteString16ToPipe( parameter.GetDefaultValue() );
			
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetModuleParamList "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( pLogSource, e );
		WriteUCharToPipe( ercFailed );
	}
}

void delEngineProcess::CommandSetModuleParameter(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString moduleVersion;
	decString moduleName;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		
		deLoadableModule *module = nullptr;
		if( moduleVersion.IsEmpty() ){
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
			
		}else{
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		}
		
		if( ! module ){
			pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetModuleParameter "
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetModuleParameter "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandActivateModule(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString moduleName;
	decString moduleVersion;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		
		deLoadableModule *module = nullptr;
		if( moduleVersion.IsEmpty() ){
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
			
		}else{
			module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		}
		
		if( ! module ){
			pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandActivateModule "
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
			
		case deModuleSystem::emtVR:
			pEngine->GetVRSystem()->SetActiveModule( module );
			break;
			
		default:
			WriteUCharToPipe( ercFailed );
			return;
		}
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandActivateModule "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandEnableModule(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString moduleName;
	decString moduleVersion;
	
	try{
		ReadString16FromPipe( moduleName );
		ReadString16FromPipe( moduleVersion );
		const bool enable = ( ReadUCharFromPipe() != 0 );
		
		deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
		
		if( ! module ){
			pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandEnableModule "
				"module '%s':%s not found", moduleName.GetString(), moduleVersion.GetString() );
			
		}else{
			module->SetEnabled( enable );
			
			WriteUCharToPipe( ercSuccess );
			return;
		}
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandActivateModule "
			"failed with exception (module='%s':%s):", moduleName.GetString(),
			moduleVersion.GetString() );
		pLogger->LogException( pLogSource, e );
	}

	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetDataDir(){
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetDataDir "
			"failed with exception (dir=%s):", directory.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetCacheAppID(){
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetCacheAppID "
			"failed with exception (dir=%s):", cacheAppID.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetPathOverlay(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString path;
	
	try{
		ReadString16FromPipe( path );
		pEngine->SetPathOverlay( path );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetPathOverlay "
			"failed with exception (dir=%s):", path.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetPathCapture(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString path;
	
	try{
		ReadString16FromPipe( path );
		pEngine->SetPathCapture( path );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetPathCapture "
			"failed with exception (dir=%s):", path.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetPathConfig(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString path;
	
	try{
		ReadString16FromPipe( path );
		pEngine->SetPathConfig( path );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetPathConfig "
			"failed with exception (dir=%s):", path.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandVFSAddDiskDir(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decPath pathRoot, pathDisk;
	bool readOnly = false;
	decString root, disk;
	
	try{
		ReadString16FromPipe( root );
		ReadString16FromPipe( disk );
		readOnly = ReadUCharFromPipe() != 0;
		
		pathRoot.SetFromUnix( root.GetString() );
		pathDisk.SetFromNative( disk.GetString() );
		
		const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New(
			new deVFSDiskDirectory( pathRoot, pathDisk, readOnly ) ) );
		
		const int hiddenPathCount = ReadUShortFromPipe();
		decString hiddenPath;
		int i;
		
		for( i=0; i<hiddenPathCount; i++ ){
			ReadString16FromPipe( hiddenPath );
			container->AddHiddenPath( decPath::CreatePathUnix( hiddenPath ) );
		}
		
		pEngine->GetVirtualFileSystem()->AddContainer( container );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandVFSAddDiskDir "
			"failed with exception (root=%s,disk=%s,ro=%c):", pathRoot.GetPathNative().GetString(),
			pathDisk.GetPathUnix().GetString(), readOnly ? 't' : 'n' );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandVFSAddScriptSharedDataDir(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	try{
		pEngine->GetScriptingSystem()->AddVFSSharedDataDir( *pEngine->GetVirtualFileSystem() );
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandVFSAddScriptSharedDataDir "
			"failed with exception:" );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandVFSAddDelgaFile(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	decString archivePath;
	decString delgaFile;
	
	try{
		ReadString16FromPipe( delgaFile );
		ReadString16FromPipe( archivePath );
		
		// add delga file archive container
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		
		const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
		
		const deArchiveContainer::Ref container( deArchiveContainer::Ref::New( amgr.CreateContainer(
			decPath::CreatePathUnix( "/" ),
			deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ),
			decPath::CreatePathUnix( archivePath ) ) ) );
		
		const int hiddenPathCount = ReadUShortFromPipe();
		decString hiddenPath;
		int i;
		
		for( i=0; i<hiddenPathCount; i++ ){
			ReadString16FromPipe( hiddenPath );
			container->AddHiddenPath( decPath::CreatePathUnix( hiddenPath ) );
		}
		
		vfs.AddContainer( container );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandVFSAddDelgaFile "
			"failed with exception (delgaFile=%s, archivePath=%s):",
			delgaFile.GetString(), archivePath.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandModulesAddVFSContainers(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString stage;
	
	try{
		ReadString16FromPipe( stage );
		
		deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
		pEngine->GetModuleSystem()->ServicesAddVFSContainers( vfs, stage );
		pEngine->GetScriptingSystem()->AddVFSContainers( vfs, stage );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess. CommandModulesAddVFSContainers"
			"failed with exception (stage='%s'):", stage.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandSetCmdLineArgs()
{
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandSetCmdLineArgs "
			"failed with exception (args=%s):", cmdLineArgs.GetString() );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandCreateRenderWindow(){
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
		
		deImage::Ref icon;
		if( ! iconPath.IsEmpty() ){
			try{
				icon.TakeOver( pEngine->GetImageManager()->LoadImage( iconPath, "/" ) );
				
			}catch( const deException &e ){
				pLogger->LogException( pLogSource, e );
			}
		}
		
		grasys.CreateAndSetRenderWindow( width, height, fullScreen, windowTitle.GetString(), icon );
		
		WriteUCharToPipe( ercSuccess );
		return;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandCreateRenderWindow "
			" failed with exception (width=%d,height=%d,title=%s,fullScreen=%d):",
			width, height, windowTitle.GetString(), fullScreen );
		pLogger->LogException( pLogSource, e );
	}
	
	WriteUCharToPipe( ercFailed );
}

void delEngineProcess::CommandStartGame(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	// prepare for running game. this part sends back the result through the pipe
	decString scriptDirectory;
	decString scriptVersion;
	decString gameObject;
	
	try{
		ReadString16FromPipe( scriptDirectory );
		ReadString16FromPipe( scriptVersion );
		ReadString16FromPipe( gameObject );
		
		// create a thread processing the in pipe while we are spinning the game loop
		pRunGame = new delEngineProcessRunGame( *this );
		
		// now we are ready to start the thread. send success code before thread takes over
		pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame "
			"launching in pipe processing thread" );
		WriteUCharToPipe( ercSuccess );
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandStartGame "
			"failed with exception (scriptDir='%s', scriptVersion='%s', gameObject='%s'):",
			scriptDirectory.GetString(), scriptVersion.GetString(), gameObject.GetString() );
		pLogger->LogException( pLogSource, e );
		
		if( pRunGame ){
			pRunGame->Stop();
			delete pRunGame;
			pRunGame = nullptr;
		}
		
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	// store single type module parameters to compare after engine exits if user changed them
	struct sModuleParamState{
		const deBaseModule *module;
		decStringDictionary parameters;
	};
	sModuleParamState moduleState[ 11 ] = {
		{ pEngine->GetAISystem()->GetActiveModule() },
		{ pEngine->GetAnimatorSystem()->GetActiveModule() },
		{ pEngine->GetAudioSystem()->GetActiveModule() },
		{ pEngine->GetCrashRecoverySystem()->GetActiveModule() },
		{ pEngine->GetGraphicSystem()->GetActiveModule() },
		{ pEngine->GetInputSystem()->GetActiveModule() },
		{ pEngine->GetNetworkSystem()->GetActiveModule() },
		{ pEngine->GetPhysicsSystem()->GetActiveModule() },
		{ pEngine->GetScriptingSystem()->GetActiveModule() },
		{ pEngine->GetSynthesizerSystem()->GetActiveModule() },
		{ pEngine->GetVRSystem()->GetActiveModule() } };
	deModuleParameter moduleParameter;
	int i, j;
	
	for( i=0; i<11; i++ ){
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
		pStopGame = false;
		pRunGame->Start();
		
		pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame launching game" );
		pEngine->Run( scriptDirectory, scriptVersion, gameObject );
		
		// when we get here this can be either because the player quit the game or
		// because we received a quit request from the launcher. either way stop the
		// thread but do not interact with the pipe. we use exit code for this.
		pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame game excited,"
			" stopping in-pipe processing thread" );
		pRunGame->Stop();
		delete pRunGame;
		pRunGame = nullptr;
		
	}catch( const deException &e ){
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandStartGame "
			"failed with exception (scriptDir='%s', scriptVersion='%s', gameObject='%s'):",
			scriptDirectory.GetString(), scriptVersion.GetString(), gameObject.GetString() );
		pLogger->LogException( pLogSource, e );
		if( pRunGame ){
			pRunGame->Stop();
			delete pRunGame;
			pRunGame = nullptr;
		}
	}
	
	// stop game requires an answer
	if(pStopGame){
		pStopGame = false;
		pLogger->LogInfo(pLogSource, "EngineProcess.CommandStartGame answer eccStopGame");
		WriteUCharToPipe(ercSuccess);
	}
	
	// compare module parameters against stored ones and send back those which changed
	pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame sending module parameter changes" );
	WriteUCharToPipe( ercGameExited );
	
	for( i=0; i<11; i++ ){
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
	pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame waiting for acknowledge" );
	ReadUCharFromPipe();
	
	// shut down process
	pLogger->LogInfo( pLogSource, "EngineProcess.CommandStartGame acknowledge received, exiting process" );
	pStopProcess = true;
}

void delEngineProcess::CommandGetDisplayCurrentResolution(){
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetDisplayCurrentResolution "
			"failed with exception (display=%d):", display );
		pLogger->LogException( pLogSource, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void delEngineProcess::CommandGetDisplayResolutions(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decPoint *resolutions = nullptr;
	int display = 0;
	int count = 0;
	
	try{
		display = ReadUCharFromPipe();
		count = ReadUCharFromPipe();
		
		const int resolutionCount = pEngine->GetOS()->GetDisplayResolutionCount( display );
		
		if( count == 0 ){
			WriteUCharToPipe( ercSuccess );
			WriteUCharToPipe( resolutionCount );
			return;
		}
		
		DEASSERT_TRUE( count <= resolutionCount )
		
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandGetDisplayResolutions "
			" failed with exception (display=%d, count=%d):", display, count );
		pLogger->LogException( pLogSource, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void delEngineProcess::CommandGetDisplayCurrentScaleFactor(){
	if(!pEngine){
		WriteUCharToPipe(ercFailed);
		return;
	}
	
	int display = 0;
	
	try{
		display = ReadUCharFromPipe();
		
		const int scale = pEngine->GetOS()->GetDisplayCurrentScaleFactor(display);
		
		WriteUCharToPipe(ercSuccess);
		WriteUShortToPipe(scale);
		
	}catch(const deException &e){
		pLogger->LogErrorFormat(pLogSource,
			"EngineProcess.CommandGetDisplayCurrentScaleFactor "
			" failed with exception (display=%d):", display);
		pLogger->LogException(pLogSource, e);
		
		WriteUCharToPipe(ercFailed);
	}
}

void delEngineProcess::CommandDelgaReadGameDefs(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString gameDef;
	decString delgaFilename;
	decStringList gameDefs;
	
	try{
		ReadString16FromPipe( delgaFilename );
		
		// open delga file
		deArchiveManager &amgr = *pEngine->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
		
		const deArchive::Ref delgaArchive( deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ) );
		
		const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		vfs->AddContainer( deArchiveContainer::Ref::New( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) ) );
		
		deCollectFileSearchVisitor collect( "*.degame", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		const int fileCount = files.GetCount();
		decBaseFileReader::Ref reader;
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandDelgaReadGameDefs "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( pLogSource, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void delEngineProcess::CommandDelgaReadPatchDefs(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
	decString patchDef;
	decString delgaFilename;
	decStringList patchDefs;
	
	try{
		ReadString16FromPipe( delgaFilename );
		
		// open delga file
		deArchiveManager &amgr = *pEngine->GetArchiveManager();
		decPath pathRoot( decPath::CreatePathUnix( "/" ) );
		
		const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
		
		const deArchive::Ref delgaArchive( deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ) );
		
		const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		vfs->AddContainer( deArchiveContainer::Ref::New( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) ) );
		
		deCollectFileSearchVisitor collect( "*.depatch", true );
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		const dePathList &files = collect.GetFiles();
		const int fileCount = files.GetCount();
		decBaseFileReader::Ref reader;
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandDelgaReadPatchDefs "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( pLogSource, e );
		
		WriteUCharToPipe( ercFailed );
	}
}

void delEngineProcess::CommandDelgaReadFiles(){
	if( ! pEngine ){
		WriteUCharToPipe( ercFailed );
		return;
	}
	
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
		
		const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		
		decPath pathDelgaDir( decPath::CreatePathNative( delgaFilename ) );
		const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
		pathDelgaDir.RemoveLastComponent();
		delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
		
		const deArchive::Ref delgaArchive( deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ) );
		
		const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
		vfs->AddContainer( deArchiveContainer::Ref::New( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) ) );
		
		// read files
		decBaseFileReader::Ref reader;
		
		for( i=0; i<fileCount; i++ ){
			const decString &filename = filenames.GetAt( i );
			reader.TakeOver( vfs->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
			const int size = reader->GetLength();
			
			const decMemoryFile::Ref content( decMemoryFile::Ref::New( new decMemoryFile( filename ) ) );
			content->Resize( size );
			reader->Read( content->GetPointer(), size );
			
			filesContent.Add( content );
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
		pLogger->LogErrorFormat( pLogSource, "EngineProcess.CommandDelgaFiles "
			" failed with exception (delgaFilename=%s):", delgaFilename.GetString() );
		pLogger->LogException( pLogSource, e );
		
		WriteUCharToPipe( ercFailed );
	}
}



// Private Functions
//////////////////////

void delEngineProcess::pCreateLogger( const char *logfile ){
	if( strlen( logfile ) == 0 ){
		pLogger.TakeOver( new deLoggerConsoleColor );
		return;
	}
	
	decPath diskPath( decPath::CreatePathNative( logfile ) );
	
	decPath filePath;
	filePath.AddComponent( diskPath.GetLastComponent() );
	
	diskPath.RemoveLastComponent();
	
	const deVFSDiskDirectory::Ref diskDir(
		deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( diskPath ) ) );
	
	pLogger.TakeOver( new deLoggerFile(
		decBaseFileWriter::Ref::New( diskDir->OpenFileForWriting( filePath ) ) ) );
}
