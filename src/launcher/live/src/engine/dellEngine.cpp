/* 
 * Drag[en]gine Live Launcher
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef OS_W32
#include <sys/wait.h>
#include <signal.h>
#endif

#include "dellEngine.h"
#include "dellEngineConfigXML.h"
#include "modules/dellEngineModule.h"
#include "modules/dellEngineModuleXML.h"
#include "../dellLauncher.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_W32
#	include <dragengine/app/deOSWindows.h>
#elif defined OS_MACOS
#	include <dragengine/app/deOSMacOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_UNIX
#	include <dragengine/app/deOSUnix.h>
#	include <dragengine/app/deOSConsole.h>
#else
#	error OS not supported!
#endif

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class dellEngine
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEngine::dellEngine( dellLauncher &launcher ) :
pLauncher( launcher ),
pResolutions( NULL ),
pResolutionCount( 0 ),
pEngine( NULL ){
}

dellEngine::~dellEngine(){
	if( pResolutions ){
		delete [] pResolutions;
	}
	if( pEngine ){
		delete pEngine;
	}
}



// Management
///////////////

void dellEngine::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void dellEngine::SetPathShare( const char *path ){
	pPathShare = path;
}

void dellEngine::SetPathLib( const char *path ){
	pPathLib = path;
}



void dellEngine::Start( deLogger *logger, const char *cacheAppID ){
	if( pEngine || ! cacheAppID ){
		DETHROW( deeInvalidParam );
	}
	
	deOS *os = NULL;
	bool useConsole = false;
	
	try{
		// create os
		if( useConsole ){
			#if defined OS_W32
			pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS Windows" );
			os = new deOSWindows();
			
			#else
			pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS Console" );
			os = new deOSConsole();
			#endif
			
		}else{
			#if defined OS_BEOS
			pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS BeOS" );
			os = new deOSBeOS();
			
			#elif defined OS_W32
			pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS Windows" );
			os = new deOSWindows();
			os->CastToOSWindows()->SetInstApp( GetModuleHandle( NULL ) );
			
			#elif defined OS_MACOS
			pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS MacOS" );
			os = new deOSMacOS();
			
			#elif defined OS_UNIX
				#ifdef HAS_LIB_X11
				pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS Unix" );
				os = new deOSUnix();
				
				#else
				pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating OS Console" );
				os = new deOSConsole();
				#endif
			#endif
		}
		if( ! os ){
			DETHROW( deeOutOfMemory );
		}
		
		// create game engine
		pLauncher.GetLogger()->LogInfo( LOGSOURCE, "Creating game engine" );
		pEngine = new deEngine( os );
		os = NULL;
		
		pEngine->SetLogger( logger );
		pEngine->SetCacheAppID( cacheAppID );
		
		pEngine->LoadModules();
		
		UpdateResolutions();
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( LOGSOURCE, "EngineProcess.StartEngine failed with exception:" );
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
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

void dellEngine::Stop(){
	if( pEngine ){
		delete pEngine;
		pEngine = NULL;
	}
}

void dellEngine::LoadModuleList(){
	pModuleList.RemoveAll();
	
	pLauncher.GetLogger()->LogInfo( LOGSOURCE, "loading module list" );
	AddModulesFrom( "/engine/lib/modules/crashrecovery", deModuleSystem::emtCrashRecovery );
	AddModulesFrom( "/engine/lib/modules/graphic", deModuleSystem::emtGraphic );
	AddModulesFrom( "/engine/lib/modules/input", deModuleSystem::emtInput );
	AddModulesFrom( "/engine/lib/modules/physics", deModuleSystem::emtPhysics );
	AddModulesFrom( "/engine/lib/modules/audio", deModuleSystem::emtAudio );
	AddModulesFrom( "/engine/lib/modules/network", deModuleSystem::emtNetwork );
	AddModulesFrom( "/engine/lib/modules/scripting", deModuleSystem::emtScript );
	AddModulesFrom( "/engine/lib/modules/animator", deModuleSystem::emtAnimator );
	AddModulesFrom( "/engine/lib/modules/ai", deModuleSystem::emtAI );
	AddModulesFrom( "/engine/lib/modules/synthesizer", deModuleSystem::emtSynthesizer );
	AddModulesFrom( "/engine/lib/modules/vr", deModuleSystem::emtVR );
	
	AddModulesFrom( "/engine/lib/modules/archive", deModuleSystem::emtArchive );
	AddModulesFrom( "/engine/lib/modules/animation", deModuleSystem::emtAnimation );
	AddModulesFrom( "/engine/lib/modules/font", deModuleSystem::emtFont );
	AddModulesFrom( "/engine/lib/modules/image", deModuleSystem::emtImage );
	AddModulesFrom( "/engine/lib/modules/model", deModuleSystem::emtModel );
	AddModulesFrom( "/engine/lib/modules/rig", deModuleSystem::emtRig );
	AddModulesFrom( "/engine/lib/modules/skin", deModuleSystem::emtSkin );
	AddModulesFrom( "/engine/lib/modules/langpack", deModuleSystem::emtLanguagePack );
	AddModulesFrom( "/engine/lib/modules/sound", deModuleSystem::emtSound );
	AddModulesFrom( "/engine/lib/modules/video", deModuleSystem::emtVideo );
	AddModulesFrom( "/engine/lib/modules/occlusionmesh", deModuleSystem::emtOcclusionMesh );
}

void dellEngine::AddModulesFrom( const char *directory, int type ){
	dellEngineModuleXML moduleXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReaderReference reader;
	dellEngineModule *module;
	decPath pattern;
	int i, j;
	
	deCollectDirectorySearchVisitor collect;
	vfs.SearchFiles( decPath::CreatePathUnix( directory ), collect );
	
	const dePathList &moduleDirs = collect.GetDirectories();
	const int count = moduleDirs.GetCount();
	for( i=0; i<count; i++ ){
		deCollectDirectorySearchVisitor collect2;
		vfs.SearchFiles( moduleDirs.GetAt( i ), collect2 );
		
		const dePathList &versionDirs = collect2.GetDirectories();
		const int count2 = versionDirs.GetCount();
		for( j=0; j<count2; j++ ){
			const decPath &versionDir = versionDirs.GetAt( j );
			
			pattern.SetFrom( versionDir );
			pattern.AddComponent( "module.xml" );
			
			if( ! vfs.ExistsFile( pattern ) || vfs.GetFileType( pattern ) != deVFSContainer::eftRegularFile ){
				continue;
			}
			
			logger.LogInfoFormat( LOGSOURCE, "Reading module definition from %s", pattern.GetPathUnix().GetString() );
			module = NULL;
			
			try{
				// read module definition file
				reader.TakeOver( vfs.OpenFileForReading( pattern ) );
				module = new dellEngineModule;
				moduleXML.ReadFromFile( reader, *module );
				
				// check if the module is working
				module->SetLoadableModule( pEngine->GetModuleSystem()->GetModuleNamed( module->GetName().GetString() ) );
				
				if( module->GetLoadableModule() ){
					module->SetErrorCode( module->GetLoadableModule()->GetErrorCode() );
					
				}else{
					module->SetErrorCode( deLoadableModule::eecCreateModuleFailed );
				}
				
				if( module->GetErrorCode() == deLoadableModule::eecSuccess ){
					module->SetStatus( dellEngineModule::emsReady );
					
				}else{
					module->SetStatus( dellEngineModule::emsBroken );
				}
				
				// get parameter lists
				module->GetParamListFromEngineModule();
				
				// done
				pModuleList.Add( module );
				module->FreeReference();
				
			}catch( const deException &e ){
				logger.LogErrorFormat( LOGSOURCE, "Engine.AddModulesFrom failed reading module file with exception (dir=%s,type=%i)", directory, type );
				logger.LogException( LOGSOURCE, e );
				if( module ){
					module->FreeReference();
				}
			}
		}
	}
}

dellEngineModule *dellEngine::GetBestModuleForType( int moduleType ){
	if( ! pEngine ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModuleList.GetCount();
	dellEngineModule *bestModule = NULL;
	int i;
	
	// for the time being we simply pick the first module which matches the type and is ready
	// to be used. later on this has to be improved to use a matching metrics which tells
	// how well a module matches a given set of feature requirements.
	for( i=0; i<count; i++ ){
		dellEngineModule * const module = pModuleList.GetAt( i );
		
		if( module->GetType() != moduleType || module->GetStatus() != dellEngineModule::emsReady ){
			continue;
		}
		
		// non-fallback > fallback > none
		if( module->GetIsFallback() ){
			if( ! bestModule ){
				bestModule = module;
			}
			
		// for non-fallback pick the highest version of the first module
		}else if( ! bestModule || bestModule->GetIsFallback()
		|| ( module->GetName() == bestModule->GetName()
		&& deModuleSystem::CompareVersion( module->GetVersion(), bestModule->GetVersion() ) > 0 ) ){
			bestModule = module;
		}
	}
	
	return bestModule;
}



void dellEngine::PutEngineIntoVFS(){
	if( ! pEngine ){
		DETHROW( deeInvalidParam );
	}
	
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deVFSDiskDirectory *diskDir = NULL;
	decPath pathRootDir, pathDiskDir;
	
	try{
		// get the properties from the engine
		SetPathConfig( pEngine->GetOS()->GetPathUserConfig() );
		SetPathShare( pEngine->GetOS()->GetPathShare() );
		SetPathLib( pEngine->GetOS()->GetPathEngine() );
		
		// add the directories so they can be easily used later on.
		if( ! pEngine->GetOS()->GetPathSystemConfig().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/configSystem" );
			pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathSystemConfig() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pPathConfig.IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/config" );
			pathDiskDir.SetFromNative( pPathConfig );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pPathShare.IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/share" );
			pathDiskDir.SetFromNative( pPathShare );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pPathLib.IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/lib" );
			pathDiskDir.SetFromNative( pPathLib );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pEngine->GetOS()->GetPathUserCache().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/cache" );
			pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathUserCache() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! pEngine->GetOS()->GetPathUserCapture().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/capture" );
			pathDiskDir.SetFromNative( pEngine->GetOS()->GetPathUserCapture() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( LOGSOURCE, "Engine.PutEngineIntoVFS failed with exception:" );
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		if( diskDir ){
			diskDir->FreeReference();
		}
		throw;
	}
}



const char *dellEngine::GetModuleTypeText( int moduleType ) const{
	switch( moduleType ){
	case deModuleSystem::emtAI:
		return "AI";
		
	case deModuleSystem::emtAnimation:
		return "Animation";
		
	case deModuleSystem::emtAnimator:
		return "Animator";
		
	case deModuleSystem::emtAudio:
		return "Audio";
		
	case deModuleSystem::emtCrashRecovery:
		return "Crash Recovery";
		
	case deModuleSystem::emtFont:
		return "Font";
		
	case deModuleSystem::emtGraphic:
		return "Graphic";
		
	case deModuleSystem::emtImage:
		return "Image";
		
	case deModuleSystem::emtInput:
		return "Input";
		
	case deModuleSystem::emtLanguagePack:
		return "Language Pack";
		
	case deModuleSystem::emtModel:
		return "Model";
		
	case deModuleSystem::emtNetwork:
		return "Network";
		
	case deModuleSystem::emtPhysics:
		return "Physics";
		
	case deModuleSystem::emtRig:
		return "Rig";
		
	case deModuleSystem::emtScript:
		return "Script";
		
	case deModuleSystem::emtSkin:
		return "Skin";
		
	case deModuleSystem::emtSound:
		return "Sound";
		
	case deModuleSystem::emtVideo:
		return "Video";
		
	case deModuleSystem::emtOcclusionMesh:
		return "Occlusion Mesh";
		
	case deModuleSystem::emtSynthesizer:
		return "Synthesizer";
		
	case deModuleSystem::emtArchive:
		return "Archive";
		
	case deModuleSystem::emtVR:
		return "VR";
		
	default:
		return "Unknown";
	}
}



void dellEngine::LoadConfig(){
	dellEngineConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReaderReference reader;
	decString filename;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfo( LOGSOURCE, "Reading engine configuration file" );
			reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
			configXML.ReadFromFile( reader, pLauncher );
			
		}else{
			logger.LogErrorFormat( LOGSOURCE, "Engine configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "Engine configuration file not found, will be created upon exiting" );
	}
}



const decPoint &dellEngine::GetResolutionAt( int index ) const{
	if( index < 0 || index >= pResolutionCount ){
		DETHROW( deeInvalidParam );
	}
	return pResolutions[ index ];
}

void dellEngine::UpdateResolutions(){
	if( pResolutions ){
		delete [] pResolutions;
		pResolutions = NULL;
		pResolutionCount = 0;
	}
	
	if( ! pEngine ){
		return;
	}
	
	const int display = 0;
	
	// store display resolutions
	const int count = pEngine->GetOS()->GetDisplayResolutionCount( display );
	if( count > 0 ){
		pResolutions = new decPoint[ count ];
		for( pResolutionCount=0; pResolutionCount<count; pResolutionCount++ ){
			pResolutions[ pResolutionCount ] = pEngine->GetOS()->GetDisplayResolution( display, pResolutionCount );
		}
	}
	
	pCurrentResolution = pEngine->GetOS()->GetDisplayCurrentResolution( 0 );
	
	// sort resolutions by decreasing pixel count
	int i;
	for( i=1; i<pResolutionCount; i++ ){
		const int pixelCountCur = pResolutions[ i ].x * pResolutions[ i ].y;
		const int pixelCountPrev = pResolutions[ i - 1 ].x * pResolutions[ i - 1 ].y;
		if( pixelCountPrev >= pixelCountCur ){
			continue;
		}
		
		const decPoint swap( pResolutions[ i - 1 ] );
		pResolutions[ i - 1 ] = pResolutions[ i ];
		pResolutions[ i ] = swap;
		if( i > 1 ){
			i--;
		}
	}
}
