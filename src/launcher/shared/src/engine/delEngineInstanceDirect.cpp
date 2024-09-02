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

#include <stdlib.h>
#include <string.h>

#include "delEngineInstanceDirect.h"
#include "delEngineProcess.h"
#include "modules/delEngineModule.h"
#include "modules/parameter/delEMParameter.h"
#include "../delLauncher.h"
#include "../game/profile/delGPModule.h"
#include "../game/profile/delGPModuleList.h"
#include "../game/profile/delGPMParameter.h"

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
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/exceptions.h>
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



// Class delEngineInstanceDirect::Factory
/////////////////////////////////////////////

delEngineInstanceDirect::Factory::Factory( deLogger *engineLogger ) :
pEngineLogger( engineLogger ),
pUseConsole( false ){
}

delEngineInstanceDirect::Factory::~Factory(){
}

void delEngineInstanceDirect::Factory::SetEngineLogger( deLogger *logger ){
	pEngineLogger = logger;
}

void delEngineInstanceDirect::Factory::SetUseConsole( bool useConsole ){
	pUseConsole = useConsole;
}

delEngineInstance *delEngineInstanceDirect::Factory::CreateEngineInstance(
delLauncher &launcher, const char *logfile ){
	delEngineInstanceDirect * const instance = new delEngineInstanceDirect( launcher, logfile );
	instance->SetEngineLogger( pEngineLogger );
	instance->SetUseConsole( pUseConsole );
	return instance;
}



// Class delEngineInstanceDirect
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineInstanceDirect::delEngineInstanceDirect( delLauncher &launcher, const char *logfile ) :
delEngineInstance( launcher, logfile ),
pEngine( nullptr ),
pEngineRunning( false ),
pGameRunning( false ),
pLogger( launcher.GetLogger() ){
}

delEngineInstanceDirect::~delEngineInstanceDirect(){
	StopEngine();
}



// Management
///////////////

void delEngineInstanceDirect::SetLogger( deLogger *logger ){
	DEASSERT_NOTNULL( logger )
	pLogger = logger;
}

void delEngineInstanceDirect::SetEngineLogger( deLogger *logger ){
	pEngineLogger = logger;
}



bool delEngineInstanceDirect::IsEngineRunning() const{
	return pEngineRunning;
}

bool delEngineInstanceDirect::StartEngine(){
	if( pEngineRunning ){
		return false;
	}
	
	deOS *os = nullptr;
	
	try{
		deLogger::Ref engineLogger( pEngineLogger );
		
		if( ! engineLogger ){
			decPath diskPath( decPath::CreatePathNative( GetLauncher().GetPathLogs() ) );
			diskPath.AddUnixPath( GetLogFile() );
			
			decPath filePath;
			filePath.AddComponent( diskPath.GetLastComponent() );
			
			diskPath.RemoveLastComponent();
			
			const deVFSDiskDirectory::Ref diskDir(
				deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( diskPath ) ) );
			
			engineLogger.TakeOver( new deLoggerFile( decBaseFileWriter::Ref::New(
				diskDir->OpenFileForWriting( filePath ) ) ) );
		}
		
		// create os
		if( GetUseConsole() ){
			#ifdef OS_UNIX
			pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS Console (console requested)" );
			os = new deOSConsole();
			#elif defined OS_W32
			pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS Windows (console requested)" );
			os = new deOSWindows();
			#endif
			
		}else{
			#ifdef OS_BEOS
			pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS BeOS" );
			os = new deOSBeOS();
			#elif defined OS_UNIX
				#ifdef HAS_LIB_X11
				pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS Unix" );
				os = new deOSUnix();
				#else
				pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS Console" );
				os = new deOSConsole();
				#endif
			#elif defined OS_W32
			pLogger->LogInfo( GetLauncher().GetLogSource(), "EngineProcess.StartEngine: Create OS Windows" );
			os = new deOSWindows();
			os->CastToOSWindows()->SetInstApp( GetModuleHandle( NULL ) );
			#endif
		}
		
		// create game engine
		pEngine = new deEngine( os );
		os = nullptr;
		
		pEngine->SetLogger( engineLogger );
		
		pEngineRunning = true;
		return true;
		
	}catch( const deException &e ){
		pLogger->LogError( GetLauncher().GetLogSource(), "EngineProcess.StartEngine failed with exception:" );
		pLogger->LogException( GetLauncher().GetLogSource(), e );
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

void delEngineInstanceDirect::StopEngine(){
	if( pEngine ){
		delete pEngine;
		pEngine = nullptr;
	}
	pEngineRunning = false;
}

void delEngineInstanceDirect::KillEngine(){
	StopEngine();
}



void delEngineInstanceDirect::GetProperty( int property, decString &value ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing GetProperty(property=%i)", property );
	DEASSERT_NOTNULL( pEngine )
	
	switch( property ){
	case delEngineProcess::epPathEngineConfig:
		value = pEngine->GetOS()->GetPathUserConfig();
		break;
		
	case delEngineProcess::epPathEngineShare:
		value = pEngine->GetOS()->GetPathShare();
		break;
		
	case delEngineProcess::epPathEngineLib:
		value = pEngine->GetOS()->GetPathEngine();
		break;
		
	case delEngineProcess::epPathEngineCache:
		value = pEngine->GetOS()->GetPathUserCache();
		break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "invalid property" );
	}
}

void delEngineInstanceDirect::LoadModules(){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing LoadModules" );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->LoadModules();
}

int delEngineInstanceDirect::GetModuleStatus( const char *moduleName, const char *moduleVersion ){
	DEASSERT_NOTNULL( moduleName )
	DEASSERT_NOTNULL( moduleVersion )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing GetModuleStatus(module='%s':%s)", moduleName, moduleVersion );
	DEASSERT_NOTNULL( pEngine )
	
	const deLoadableModule * const module = pEngine->GetModuleSystem()->
		GetModuleNamed( moduleName, moduleVersion );
	DEASSERT_NOTNULL( module )
	
	return module->GetErrorCode();
}

void delEngineInstanceDirect::GetModuleParams( delEngineModule &module ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing GetModuleParamList(moduleName='%s')", module.GetName().GetString() );
	DEASSERT_NOTNULL( pEngine )
	
	const deLoadableModule * const engModule = pEngine->GetModuleSystem()->GetModuleNamed( module.GetName() );
	DEASSERT_NOTNULL( engModule )
	
	const deBaseModule * const baseModule = engModule->GetModule();
	DEASSERT_NOTNULL( baseModule )
	
	delEMParameterList &parameters = module.GetParameters();
	const int count = baseModule->GetParameterCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deModuleParameter parameter;
		baseModule->GetParameterInfo( i, parameter );
		
		parameters.Add( delEMParameter::Ref::New( new delEMParameter( i, parameter,
			baseModule->GetParameterValue( parameter.GetName() ) ) ) );
	}
}

void delEngineInstanceDirect::SetModuleParameter( const char *moduleName, const char *moduleVersion,
const char *parameter, const char *value ){
	DEASSERT_NOTNULL( moduleName )
	DEASSERT_NOTNULL( moduleVersion )
	DEASSERT_NOTNULL( parameter )
	DEASSERT_NOTNULL( value )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetModuleParameter(module='%s':%s,parameter=%s,value='%s')",
		moduleName, moduleVersion, parameter, value );
	DEASSERT_NOTNULL( pEngine )
	
	deLoadableModule *module = nullptr;
	if( strlen( moduleVersion ) == 0 ){
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
		
	}else{
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
	}
	DEASSERT_NOTNULL( module )
	
	deBaseModule * const baseModule = module->GetModule();
	DEASSERT_NOTNULL( baseModule )
	
	baseModule->SetParameterValue( parameter, value );
}

void delEngineInstanceDirect::ActivateModule( const char *moduleName, const char *moduleVersion ){
	DEASSERT_NOTNULL( moduleName )
	DEASSERT_NOTNULL( moduleVersion )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing ActivateModule(module='%s':%s)",
		moduleName, moduleVersion );
	DEASSERT_NOTNULL( pEngine )
	
	deLoadableModule *module = nullptr;
	if( strlen( moduleVersion ) == 0 ){
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName );
		
	}else{
		module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
	}
	DEASSERT_NOTNULL( module )
	
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
		DETHROW_INFO( deeInvalidParam, "invalid module type" );
	}
}

void delEngineInstanceDirect::EnableModule( const char *moduleName, const char *moduleVersion, bool enable ){
	DEASSERT_NOTNULL( moduleName )
	DEASSERT_NOTNULL( moduleVersion )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing EnableModule(module='%s':%s %s)",
		moduleName, moduleVersion, enable ? "enable" : "disable" );
	DEASSERT_NOTNULL( pEngine )
	
	deLoadableModule * const module = pEngine->GetModuleSystem()->GetModuleNamed( moduleName, moduleVersion );
	DEASSERT_NOTNULL( module )
	
	module->SetEnabled( enable );
}

void delEngineInstanceDirect::SetDataDirectory( const char *directory ){
	DEASSERT_NOTNULL( directory )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetDataDir(directory='%s')", directory );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->SetDataDir( directory );
}

void delEngineInstanceDirect::SetCacheAppID( const char *cacheAppID ){
	DEASSERT_NOTNULL( cacheAppID )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetCacheAppID(id='%s')", cacheAppID );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->SetCacheAppID( cacheAppID );
}

void delEngineInstanceDirect::SetPathOverlay( const char* path ){
	DEASSERT_NOTNULL( path )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetPathOverlay(path='%s')", path );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->SetPathOverlay( path );
}

void delEngineInstanceDirect::SetPathCapture( const char* path ){
	DEASSERT_NOTNULL( path )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetPathCapture(path='%s')", path );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->SetPathCapture( path );
}

void delEngineInstanceDirect::SetPathConfig( const char* path ){
	DEASSERT_NOTNULL( path )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetPathConfig(path='%s')", path );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->SetPathConfig( path );
}

void delEngineInstanceDirect::VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ){
	VFSAddDiskDir( vfsRoot, nativeDirectory, readOnly, decStringSet() );
}

void delEngineInstanceDirect::VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory,
bool readOnly, const decStringSet &hiddenPath ){
	DEASSERT_NOTNULL( vfsRoot )
	DEASSERT_NOTNULL( nativeDirectory )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing VFSAddDiskDir(vfsRoot='%s',nativeDirectory='%s',readOnly=%c,hiddenPath=%d)",
		vfsRoot, nativeDirectory, readOnly?'y':'n', hiddenPath.GetCount() );
	DEASSERT_NOTNULL( pEngine )
	
	const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New(
		new deVFSDiskDirectory( decPath::CreatePathUnix( vfsRoot ),
			decPath::CreatePathNative( nativeDirectory ), readOnly ) ) );
	
	const int count = hiddenPath.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		container->AddHiddenPath( decPath::CreatePathUnix( hiddenPath.GetAt( i ) ) );
	}
	
	pEngine->GetVirtualFileSystem()->AddContainer( container );
}

void delEngineInstanceDirect::VFSAddScriptSharedDataDir(){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing VFSAddScriptSharedDataDir" );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->GetScriptingSystem()->AddVFSSharedDataDir( *pEngine->GetVirtualFileSystem() );
}

void delEngineInstanceDirect::VFSAddDelgaFile( const char *delgaFile, const char *archivePath ){
	VFSAddDelgaFile( delgaFile, archivePath, decStringSet() );
}

void delEngineInstanceDirect::VFSAddDelgaFile( const char *delgaFile,
const char *archivePath, const decStringSet &hiddenPath ){
	DEASSERT_NOTNULL( delgaFile )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing VFSAddDelga(delgaFile='%s', archivePath=%s, hiddenPath=%d)",
		delgaFile, archivePath, hiddenPath.GetCount() );
	DEASSERT_NOTNULL( pEngine )
	
	decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
	const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
	pathDelgaDir.RemoveLastComponent();
	
	const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	
	const deArchiveContainer::Ref container( deArchiveContainer::Ref::New( amgr.CreateContainer(
		decPath::CreatePathUnix( "/" ),
		deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ),
		decPath::CreatePathUnix( archivePath ) ) ) );
	
	const int count = hiddenPath.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		container->AddHiddenPath( decPath::CreatePathUnix( hiddenPath.GetAt( i ) ) );
	}
	
	vfs.AddContainer( container );
}

void delEngineInstanceDirect::ModulesAddVFSContainers( const char *stage ){
	DEASSERT_NOTNULL( stage )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing ModulesAddVFSContainers(stage='%s')", stage );
	DEASSERT_NOTNULL( pEngine )
	
	deVirtualFileSystem &vfs = *pEngine->GetVirtualFileSystem();
	pEngine->GetModuleSystem()->ServicesAddVFSContainers( vfs, stage );
	pEngine->GetScriptingSystem()->AddVFSContainers( vfs, stage );
}

void delEngineInstanceDirect::SetCmdLineArgs(const char *arguments)
{
	DEASSERT_NOTNULL( arguments )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing SetCmdLineArgs(arguments='%s')", arguments );
	DEASSERT_NOTNULL( pEngine )
	
	pEngine->GetArguments()->AddArgsSplit( arguments );
}

void delEngineInstanceDirect::CreateRenderWindow( int width, int height, bool fullScreen,
const char *windowTitle, const char *iconPath ){
	DEASSERT_TRUE( width > 0 )
	DEASSERT_TRUE( height > 0 )
	DEASSERT_NOTNULL( windowTitle )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Processing CreateRenderWindow("
		"width=%d,height=%d,fullScreen=%c,windowTitle='%s',iconPath='%s')",
		width, height, fullScreen?'y':'n', windowTitle, iconPath );
	DEASSERT_NOTNULL( pEngine )
	
	deImage::Ref icon;
	if( strlen( iconPath ) > 0 ){
		try{
			icon.TakeOver( pEngine->GetImageManager()->LoadImage( iconPath, "/" ) );
			
		}catch( const deException &e ){
			pLogger->LogException( GetLauncher().GetLogSource(), e );
		}
	}
	
	deGraphicSystem &grasys = *pEngine->GetGraphicSystem();
	grasys.CreateAndSetRenderWindow( width, height, fullScreen, windowTitle, icon );
}

void delEngineInstanceDirect::StartGame( const char *scriptDirectory, const char *gameObject,
delGPModuleList *collectChangedParams ){
	StartGame( scriptDirectory, "", gameObject, collectChangedParams );
}

void delEngineInstanceDirect::StartGame( const char *scriptDirectory, const char *scriptVersion,
const char *gameObject, delGPModuleList *collectChangedParams ){
	DEASSERT_NOTNULL( scriptDirectory )
	DEASSERT_NOTNULL( scriptVersion )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(), "Processing StartGame("
		"scriptDirectory='%s' scriptVersion='%s' gameObject='%s')",
		scriptDirectory, scriptVersion, gameObject );
	DEASSERT_NOTNULL( pEngine )
	
	pGameRunning = true;
	if( collectChangedParams ){
		collectChangedParams->RemoveAll();
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
	
	// run game. this blocks until finished
	bool success = true;
	
	try{
		pLogger->LogInfo( GetLauncher().GetLogSource(), "Game started" );
		pEngine->Run( scriptDirectory, scriptVersion, gameObject );
		pLogger->LogInfo( GetLauncher().GetLogSource(), "Game excited" );
		
	}catch( const deException &e ){
		GetLauncher().GetLogger()->LogException( GetLauncher().GetLogSource(), e );
		success = false;
	}
	
	// compare module parameters against stored ones
	if( collectChangedParams ){
		for( i=0; i<11; i++ ){
			if( ! moduleState[ i ].module ){
				continue;
			}
			
			const decStringList keys( moduleState[ i ].parameters.GetKeys() );
			const int count = keys.GetCount();
			for( j=0; j<count; j++ ){
				const decString &name = keys.GetAt( j );
				const decString value( moduleState[ i ].module->GetParameterValue( name ) );
				if( value == moduleState[ i ].parameters.GetAt( name ) ){
					continue;
				}
				
				const decString &moduleName = moduleState[ i ].module->GetLoadableModule().GetName();
				
				delGPModule::Ref module( collectChangedParams->GetNamed( moduleName ) );
				if( ! module ){
					module.TakeOver( new delGPModule( moduleName ) );
					collectChangedParams->Add( module );
				}
				
				module->GetParameters().Add( delGPMParameter::Ref::New( new delGPMParameter( name, value ) ) );
			}
		}
	}
	
	// finished
	pGameRunning = false;
	DEASSERT_TRUE( success )
}

void delEngineInstanceDirect::StopGame(){
	GetLauncher().GetLogger()->LogInfo( GetLauncher().GetLogSource(), "Processing StopGame" );
	
	if( pEngine ){
		pEngine->Quit();
	}
}

int delEngineInstanceDirect::IsGameRunning(){
	return pGameRunning && IsEngineRunning() ? 1 : 0;
}

decPoint delEngineInstanceDirect::GetDisplayCurrentResolution( int display ){
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing GetDisplayCurrentResolution(display=%d)"
		"", display );
	DEASSERT_NOTNULL( pEngine )
	
	return pEngine->GetOS()->GetDisplayCurrentResolution( display );
}

int delEngineInstanceDirect::GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount ){
	DEASSERT_TRUE( resolutionCount >= 0 )
	DEASSERT_TRUE( resolutionCount == 0 || resolutions )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing GetDisplayResolutions(display=%d, resolutions=%d, resolutionCount=%d)",
		display, resolutions ? 1 : 0, resolutionCount );
	DEASSERT_NOTNULL( pEngine )
	
	const int realCount = pEngine->GetOS()->GetDisplayResolutionCount( display );
	
	if( resolutionCount == 0 ){
		return realCount;
	}
	
	DEASSERT_TRUE( resolutionCount <= realCount )
	
	int i;
	for( i=0; i<resolutionCount; i++ ){
		resolutions[ i ] = pEngine->GetOS()->GetDisplayResolution( display, i );
	}
	return resolutionCount;
}

void delEngineInstanceDirect::ReadDelgaGameDefs( const char *delgaFile, decStringList &list ){
	DEASSERT_NOTNULL( delgaFile )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing ReadDelgaGameDefs (delgaFile=%s)", delgaFile );
	DEASSERT_NOTNULL( pEngine )
	
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	
	const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	
	decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
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
	decString gameDef;
	int i;
	
	for( i=0; i<fileCount; i++ ){
		const decPath &path = files.GetAt( i );
		
		reader.TakeOver( vfs->OpenFileForReading( path ) );
		const int size = reader->GetLength();
		gameDef.Set( ' ', size );
		reader->Read( ( void* )gameDef.GetString(), size );
		
		list.Add( gameDef );
	}
}

void delEngineInstanceDirect::ReadDelgaPatchDefs( const char *delgaFile, decStringList &list ){
	DEASSERT_NOTNULL( delgaFile )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing ReadDelgaPatchDefs (delgaFile=%s)", delgaFile );
	DEASSERT_NOTNULL( pEngine )
	
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	
	const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	
	decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
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
	decString patchDef;
	int i;
	
	for( i=0; i<fileCount; i++ ){
		const decPath &path = files.GetAt( i );
		
		reader.TakeOver( vfs->OpenFileForReading( path ) );
		const int size = reader->GetLength();
		patchDef.Set( ' ', size );
		reader->Read( ( void* )patchDef.GetString(), size );
		
		list.Add( patchDef );
	}
}

void delEngineInstanceDirect::ReadDelgaFiles( const char *delgaFile,
const decStringList &filenames, decObjectOrderedSet &filesContent ){
	DEASSERT_NOTNULL( delgaFile )
	
	GetLauncher().GetLogger()->LogInfoFormat( GetLauncher().GetLogSource(),
		"Processing ReadDelgaFiles (delgaFile=%s)", delgaFile );
	DEASSERT_NOTNULL( pEngine )
	
	const int fileCount = filenames.GetCount();
	filesContent.RemoveAll();
	if( fileCount == 0 ){
		return;
	}
	
	// open delga file
	deArchiveManager &amgr = *pEngine->GetArchiveManager();
	const decPath pathRoot( decPath::CreatePathUnix( "/" ) );
	
	const deVirtualFileSystem::Ref delgaVfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	
	decPath pathDelgaDir( decPath::CreatePathNative( delgaFile ) );
	const decString delgaFileTitle( pathDelgaDir.GetLastComponent() );
	pathDelgaDir.RemoveLastComponent();
	delgaVfs->AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( pathDelgaDir ) ) );
	
	const deArchive::Ref delgaArchive( deArchive::Ref::New( amgr.OpenArchive( delgaVfs, delgaFileTitle, "/" ) ) );
	
	const deVirtualFileSystem::Ref vfs( deVirtualFileSystem::Ref::New( new deVirtualFileSystem ) );
	vfs->AddContainer( deArchiveContainer::Ref::New( amgr.CreateContainer( pathRoot, delgaArchive, pathRoot ) ) );
	
	// read files
	decBaseFileReader::Ref reader;
	int i;
	
	for( i=0; i<fileCount; i++ ){
		const decString &filename = filenames.GetAt( i );
		reader.TakeOver( vfs->OpenFileForReading( decPath::CreatePathUnix( filename ) ) );
		const int size = reader->GetLength();
		
		const decMemoryFile::Ref content( decMemoryFile::Ref::New( new decMemoryFile( filename ) ) );
		content->Resize( size );
		reader->Read( content->GetPointer(), size );
		
		filesContent.Add( content );
	}
}

#ifdef OS_BEOS
void delEngineInstanceDirect::BeosMessageReceived( BMessage *message ){
	if( pEngine ){
		pEngine->GetOS()->CastToOSBeOS()->MessageReceived( message );
	}
}
#endif
