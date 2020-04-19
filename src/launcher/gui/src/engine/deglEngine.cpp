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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef OS_W32
#include <sys/wait.h>
#include <signal.h>
#endif

#include "deglEngine.h"
#include "deglEngineProcess.h"
#include "deglEngineInstance.h"
#include "deglEngineConfigXML.h"
#include "modules/deglEngineModule.h"
#include "modules/deglEngineModuleXML.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"
#include "../game/deglGame.h"
#include "../game/deglGameList.h"
#include "../game/deglGameXML.h"
#include "../game/patch/deglPatch.h"
#include "../game/patch/deglPatchList.h"
#include "../game/patch/deglPatchXML.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReference.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class deglEngine
/////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEngine::deglEngine( deglLauncher &launcher ) :
pLauncher( launcher ),

pLogFile( "delauncher-gui-engine.log" ),

pResolutionCount( 0 ),
pResolutions( NULL ){
}

deglEngine::~deglEngine(){
	if( pResolutions ){
		delete [] pResolutions;
	}
}



// Management
///////////////

void deglEngine::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void deglEngine::SetPathShare( const char *path ){
	pPathShare = path;
}

void deglEngine::SetPathLib( const char *path ){
	pPathLib = path;
}

void deglEngine::SetPathCache( const char *path ){
	pPathCache = path;
}

void deglEngine::SetLogFile( const char *path ){
	if( ! path ) DETHROW( deeInvalidParam );
	
	pLogFile = path;
	
	decPath filePath;
	filePath.SetFromUnix( "/config/user" );
	filePath.AddUnixPath( path );
	
	// using open file for writing instead of delete file to allow applications
	// to keep file open in a text viewer on operating systems preventing file
	// deleting while the file is open in an application (windows for example)
	decBaseFileWriter *writer = NULL;
	try{
		writer = pLauncher.GetFileSystem()->OpenFileForWriting( filePath );
		writer->FreeReference();
		
	}catch( const deException & ){
		if( writer ){
			writer->FreeReference();
		}
		throw;
	}
	
	//if( pLauncher.GetFileSystem()->ExistsFile( filePath ) ){
	//	pLauncher.GetFileSystem()->DeleteFile( filePath );
	//}
}



void deglEngine::ReloadModuleList(){
	pModuleList.RemoveAllModules();
	
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

void deglEngine::CheckModules( deglEngineInstance &instance ){
	int i, count = pModuleList.GetModuleCount();
	
	for( i=0; i<count; i++ ){
		deglEngineModule &module = *pModuleList.GetModuleAt( i );
		
		try{
			module.SetErrorCode( instance.GetModuleStatus( module.GetName(), module.GetVersion() ) );
			
			if( module.GetErrorCode() == deLoadableModule::eecSuccess ){
				module.SetStatus( deglEngineModule::emsReady );
				module.CalcSizeAndHashes( pLauncher );
				
			}else{
				module.SetStatus( deglEngineModule::emsBroken );
			}
			
			instance.GetModuleParamList( module );
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogErrorFormat( LOGSOURCE, "Engine.CheckModules failed with exception (module=%s)", module.GetName().GetString() );
			pLauncher.GetLogger()->LogException( LOGSOURCE, e );
			module.SetStatus( deglEngineModule::emsBroken );
		}
	}
}

void deglEngine::AddModulesFrom( const char *directory, int type ){
	deglEngineModuleXML moduleXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReader *reader;
	deglEngineModule *module;
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
			
			logger.LogInfoFormat( LOGSOURCE, "Reading module definition from '%s'", pattern.GetPathUnix().GetString() );
			reader = NULL;
			module = NULL;
			
			try{
				reader = vfs.OpenFileForReading( pattern );
				
				module = new deglEngineModule;
				
				moduleXML.ReadFromFile( pattern.GetPathUnix(), *reader, *module );
				reader->FreeReference();
				reader = NULL;
				
				pModuleList.AddModule( module );
				module->FreeReference();
				
			}catch( const deException &e ){
				logger.LogErrorFormat( LOGSOURCE, "Engine.AddModulesFrom failed reading module file with exception (dir=%s,type=%i)", directory, type );
				logger.LogException( LOGSOURCE, e );
				if( module ) module->FreeReference();
				if( reader ){
					reader->FreeReference();
				}
			}
		}
	}
}

deglEngineModule *deglEngine::GetBestModuleForType( int moduleType ){
	int i, count = pModuleList.GetModuleCount();
	deglEngineModule *bestModule = NULL;
	deglEngineModule *module;
	
	// for the time being we simply pick the first module which matches the type and is ready
	// to be used. later on this has to be improved to use a matching metrics which tells
	// how well a module matches a given set of feature requirements.
	for( i=0; i<count; i++ ){
		module = pModuleList.GetModuleAt( i );
		
		if( module->GetType() == moduleType && module->GetStatus() == deglEngineModule::emsReady ){
			// non-fallback > fallback > none
			if( module->GetIsFallback() ){
				if( ! bestModule ){
					bestModule = module;
				}
				
			}else{
				if( ! bestModule || bestModule->GetIsFallback() ){
					bestModule = module;
				}
			}
		}
	}
	
	return bestModule;
}



void deglEngine::PutEngineIntoVFS( deglEngineInstance &instance ){
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deglEngine &launcherEngine = *pLauncher.GetEngine();
	deVFSDiskDirectory *diskDir = NULL;
	decPath pathRootDir, pathDiskDir;
	decString propertyValue;
	
	try{
		// get the properties from the engine
		instance.GetProperty( deglEngineProcess::epPathEngineConfig, propertyValue );
		launcherEngine.SetPathConfig( propertyValue );
		
		instance.GetProperty( deglEngineProcess::epPathEngineShare, propertyValue );
		launcherEngine.SetPathShare( propertyValue );
		
		instance.GetProperty( deglEngineProcess::epPathEngineLib, propertyValue );
		launcherEngine.SetPathLib( propertyValue );
		
		instance.GetProperty( deglEngineProcess::epPathEngineCache, propertyValue );
		launcherEngine.SetPathCache( propertyValue );
		
		// add the directories so they can be easily used later on.
		if( ! launcherEngine.GetPathConfig().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/config" );
			pathDiskDir.SetFromNative( launcherEngine.GetPathConfig() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! launcherEngine.GetPathShare().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/share" );
			pathDiskDir.SetFromNative( launcherEngine.GetPathShare() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! launcherEngine.GetPathLib().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/lib" );
			pathDiskDir.SetFromNative( launcherEngine.GetPathLib() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
			vfs.AddContainer( diskDir );
			diskDir->FreeReference();
			diskDir = NULL;
		}
		
		if( ! launcherEngine.GetPathCache().IsEmpty() ){
			pathRootDir.SetFromUnix( "/engine/cache" );
			pathDiskDir.SetFromNative( launcherEngine.GetPathCache() );
			diskDir = new deVFSDiskDirectory( pathRootDir, pathDiskDir );
			diskDir->SetReadOnly( false );
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



const char *deglEngine::GetModuleTypeText( int moduleType ) const{
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
		
	default:
		return "Unknown";
	}
}



void deglEngine::UpdateResolutions( deglEngineInstance &instance ){
	if( pResolutions ){
		delete [] pResolutions;
		pResolutions = NULL;
		pResolutionCount = 0;
	}
	
	const int display = 0;
	
	try{
		pCurrentResolution = instance.GetDisplayCurrentResolution( display );
		
		pResolutions = new decPoint[ 255 ]; // maximum number of entries is 255
		pResolutionCount = instance.GetDisplayResolutions( display, pResolutions, 255 );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( LOGSOURCE, "Engine.UpdateResolutions failed with exception" );
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
		
		if( pResolutions ){
			delete [] pResolutions;
			pResolutions = NULL;
		}
		pResolutionCount = 0;
	}
	
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

const decPoint &deglEngine::GetResolutionAt( int index ) const{
	if( index < 0 || index >= pResolutionCount ){
		DETHROW( deeInvalidParam );
	}
	return pResolutions[ index ];
}

int deglEngine::IndexOfClosestResolutionTo( int width, int height ) const{
	// resolutions are sorted in decreasing order of their pixel count. this way
	// we can look for the first resolution less than or equal to the desired
	// size without having to check the entire list
	const int pixelCount = width * height;
	int i;
	
	for( i=0; i<pResolutionCount; i++ ){
		const int resolutionPixelCount = pResolutions[ i ].x * pResolutions[ i ].y;
		if( resolutionPixelCount <= pixelCount ){
			return i;
		}
	}
	
	return pResolutionCount - 1; // -1 if empty is correct
}



void deglEngine::ReadDelgaGameDefs( deglEngineInstance &instance, const char *filename, deglGameList &list ){
	// read content of all game definition files in delga file
	decStringList fileContents;
	
	try{
		instance.ReadDelgaGameDefs( filename, fileContents );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( LOGSOURCE,
			"Engine.ReadDelgaGameDefs failed with exception" );
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// parse file contents into game instances
	list.RemoveAll();
	
	const int count = fileContents.GetCount();
	if( count == 0 ){
		return;
	}
	
	deglGameXML gameXML( pLauncher.GetLogger(), LOGSOURCE );
	decBaseFileReaderReference memoryFileReader;
	decMemoryFileReference memoryFile;
	deObjectReference refGame;
	int i;
	
	memoryFile.TakeOver( new decMemoryFile( "DelgaGameDefinition" ) );
	
	for( i=0; i<count; i++ ){
		const decString &fileContent = fileContents.GetAt( i );
		const int lenContent = fileContent.GetLength();
		
		memoryFile->Resize( lenContent, false );
		memcpy( memoryFile->GetPointer(), fileContent.GetString(), lenContent );
		memoryFileReader.TakeOver( new decMemoryFileReader( memoryFile ) );
		
		refGame.TakeOver( new deglGame( pLauncher ) );
		deglGame &game = ( deglGame& )( deObject& )refGame;
		gameXML.ReadFromFile( memoryFileReader, game );
		
		game.SetDefaultLogFile();
		game.SetDelgaFile( filename );
		
		list.Add( &game );
		
		// read icon
		if( ! game.GetIconPath().IsEmpty() ){
			try{
				decStringList readFilenames;
				readFilenames.Add( game.GetIconPath() );
				decObjectOrderedSet filesContent;
				instance.ReadDelgaFiles( filename, readFilenames, filesContent );
				game.SetIconFileContent( ( decMemoryFile* )filesContent.GetAt( 0 ) );
				
			}catch( const deException &e ){
				pLauncher.GetLogger()->LogErrorFormat( LOGSOURCE,
					"Exception while reading icon for game '%s'",
					game.GetTitle().ToUTF8().GetString() );
				pLauncher.GetLogger()->LogException( LOGSOURCE, e );
			}
			
			game.CreateFoxIcons();
		}
	}
}

void deglEngine::ReadDelgaPatchDefs( deglEngineInstance &instance, const char *filename, deglPatchList &list ){
	// read content of all patch definition files in delga file
	decStringList fileContents;
	
	try{
		instance.ReadDelgaPatchDefs( filename, fileContents );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( LOGSOURCE,
			"Engine.ReadDelgaPatchDefs failed with exception" );
		pLauncher.GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// parse file contents into patch instances
	list.RemoveAll();
	
	const int count = fileContents.GetCount();
	if( count == 0 ){
		return;
	}
	
	deglPatchXML patchXML( pLauncher.GetLogger(), LOGSOURCE );
	decBaseFileReaderReference memoryFileReader;
	decMemoryFileReference memoryFile;
	deObjectReference refPatch;
	int i;
	
	memoryFile.TakeOver( new decMemoryFile( "DelgaPatchDefinition" ) );
	
	for( i=0; i<count; i++ ){
		const decString &fileContent = fileContents.GetAt( i );
		const int lenContent = fileContent.GetLength();
		
		memoryFile->Resize( lenContent, false );
		memcpy( memoryFile->GetPointer(), fileContent.GetString(), lenContent );
		memoryFileReader.TakeOver( new decMemoryFileReader( memoryFile ) );
		
		refPatch.TakeOver( new deglPatch );
		deglPatch &patch = ( deglPatch& )( deObject& )refPatch;
		patchXML.ReadFromFile( memoryFileReader, patch );
		
		patch.SetDelgaFile( filename );
		
		list.Add( &patch );
	}
}



void deglEngine::LoadConfig(){
	deglEngineConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReader *reader = NULL;
	decString filename;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/launcher.xml" );
	
	if( vfs.ExistsFile( pathFile ) ){
		if( vfs.GetFileType( pathFile ) == deVFSContainer::eftRegularFile ){
			logger.LogInfo( LOGSOURCE, "Reading engine configuration file" );
			reader = NULL;
			
			try{
				reader = vfs.OpenFileForReading( pathFile );
				configXML.ReadFromFile( *reader, pLauncher );
				
				reader->FreeReference();
				
			}catch( const deException & ){
				if( reader ){
					reader->FreeReference();
				}
				throw;
			}
			
		}else{
			logger.LogErrorFormat( LOGSOURCE, "Engine configuration file is not a regular file" );
			DETHROW( deeInvalidParam );
		}
		
	}else{
		logger.LogInfo( LOGSOURCE, "Engine configuration file not found, will be created upon exiting" );
	}
}

void deglEngine::SaveConfig(){
	deglEngineConfigXML configXML( pLauncher.GetLogger(), LOGSOURCE );
	deVirtualFileSystem &vfs = *pLauncher.GetFileSystem();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileWriter *writer = NULL;
	decString filename;
	decPath pathFile;
	
	pathFile.SetFromUnix( "/config/user/launcher.xml" );
	
	if( vfs.CanWriteFile( pathFile ) ){
		logger.LogInfo( LOGSOURCE, "Writing engine configuration file" );
		
		try{
			writer = vfs.OpenFileForWriting( pathFile );
			configXML.WriteToFile( *writer, pLauncher );
			
			writer->FreeReference();
			
		}catch( const deException &e ){
			if( writer ){
				writer->FreeReference();
			}
			logger.LogErrorFormat( LOGSOURCE, "Failed to write engine configuration file (file permission problem)" );
			logger.LogException( LOGSOURCE, e );
			// DIALOG BOX
			// "Engine configuration can not be written!\n"
			// "Make sure you have write permission for the file and parent directory.\n"
			// "Engine configuration has not been saved!\n"
		}
		
	}else{
		logger.LogErrorFormat( LOGSOURCE, "Failed to write engine configuration file (writing file problem)" );
		// DIALOG BOX
		// "Engine configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Engine configuration has not been saved!\n"
	}
}
