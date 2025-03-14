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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef OS_W32
#include <signal.h>
#endif

#include "delEngine.h"
#include "delEngineProcess.h"
#include "delEngineInstance.h"
#include "delEngineConfigXML.h"
#include "modules/delEngineModule.h"
#include "modules/delEngineModuleXML.h"
#include "../delLauncher.h"
#include "../game/delGame.h"
#include "../game/delGameList.h"
#include "../game/delGameXML.h"
#include "../game/icon/delGameIcon.h"
#include "../game/patch/delPatch.h"
#include "../game/patch/delPatchList.h"
#include "../game/patch/delPatchXML.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class delEngine
////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngine::delEngine( delLauncher &launcher, const char *logFileTitle ) :
pLauncher( launcher ),
pLogFile( decString( logFileTitle ) + ".log" ),
pResolutionCount( 0 ),
pResolutions( NULL ),
pScaleFactor(100){
}

delEngine::~delEngine(){
	if( pResolutions ){
		delete [] pResolutions;
	}
}



// Management
///////////////

void delEngine::SetPathConfig( const char *path ){
	pPathConfig = path;
}

void delEngine::SetPathShare( const char *path ){
	pPathShare = path;
}

void delEngine::SetPathLib( const char *path ){
	pPathLib = path;
}

void delEngine::SetPathCache( const char *path ){
	pPathCache = path;
}

void delEngine::SetLogFile( const char *path ){
	pLogFile = path;
	
	decPath filePath;
	filePath.SetFromUnix( "/config/user" );
	filePath.AddUnixPath( path );
	
	// using open file for writing instead of delete file to allow applications
	// to keep file open in a text viewer on operating systems preventing file
	// deleting while the file is open in an application (windows for example)
	decBaseFileWriter::Ref::New( pLauncher.GetVFS()->OpenFileForWriting( filePath ) );
	
	//if( pLauncher.GetFileSystem()->ExistsFile( filePath ) ){
	//	pLauncher.GetFileSystem()->DeleteFile( filePath );
	//}
}



void delEngine::ReloadModules(){
	pModules.RemoveAll();
	
	pLauncher.GetLogger()->LogInfo( pLauncher.GetLogSource(), "loading module list" );
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
	AddModulesFrom( "/engine/lib/modules/service", deModuleSystem::emtService );
}

void delEngine::CheckModules( delEngineInstance &instance ){
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delEngineModule &module = *pModules.GetAt ( i );
		
		try{
			module.SetErrorCode( instance.GetModuleStatus( module.GetName(), module.GetVersion() ) );
			
			if( module.GetErrorCode() == deLoadableModule::eecSuccess ){
				module.SetStatus( delEngineModule::emsReady );
				module.CalcSizeAndHashes( pLauncher );
				
			}else{
				module.SetStatus( delEngineModule::emsBroken );
			}
			
			instance.GetModuleParams( module );
			
		}catch( const deException &e ){
			pLauncher.GetLogger()->LogErrorFormat( pLauncher.GetLogSource(),
				"Engine.CheckModules failed with exception (module=%s)", module.GetName().GetString() );
			pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
			module.SetStatus( delEngineModule::emsBroken );
		}
	}
}

void delEngine::AddModulesFrom( const char *directory, deModuleSystem::eModuleTypes type ){
	delEngineModuleXML moduleXML( pLauncher.GetLogger(), pLauncher.GetLogSource() );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decBaseFileReader::Ref reader;
	delEngineModule::Ref module;
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
			
			if( ! vfs.ExistsFile( pattern )
			|| vfs.GetFileType( pattern ) != deVFSContainer::eftRegularFile ){
				continue;
			}
			
			logger.LogInfoFormat( pLauncher.GetLogSource(),
				"Reading module definition from '%s'", pattern.GetPathUnix().GetString() );
			
			try{
				reader.TakeOver( vfs.OpenFileForReading( pattern ) );
				module.TakeOver( new delEngineModule );
				moduleXML.ReadFromFile( pattern.GetPathUnix(), reader, module );
				pModules.Add ( module );
				
			}catch( const deException &e ){
				logger.LogErrorFormat( pLauncher.GetLogSource(), "Engine.AddModulesFrom failed"
					" reading module file with exception (dir=%s,type=%i)", directory, type );
				logger.LogException( pLauncher.GetLogSource(), e );
			}
		}
	}
}

delEngineModule *delEngine::GetBestModuleForType( deModuleSystem::eModuleTypes moduleType ) const{
	const int count = pModules.GetCount();
	delEngineModule *bestModule = nullptr;
	int i;
	
	// for the time being we simply pick the first module which matches the type and is ready
	// to be used. later on this has to be improved to use a matching metrics which tells
	// how well a module matches a given set of feature requirements.
	for( i=0; i<count; i++ ){
		delEngineModule * const module = pModules.GetAt ( i );
		
		if( module->GetType() != moduleType ){
			continue;
		}
		if( module->GetStatus() != delEngineModule::emsReady ){
			continue;
		}
		
		// no best module found. use this module
		if( ! bestModule ){
			bestModule = module;
			
		// best module has been found and this module is fallback. skip module
		}else if( module->GetIsFallback() ){
			
		// best module has same name as this module
		}else if( module->GetName() == bestModule->GetName() ){
			// use this module if it has higher version than the best module
			if( deModuleSystem::CompareVersion( module->GetVersion(), bestModule->GetVersion() ) > 0 ){
				bestModule = module;
			}
			
		// best module has different name than this module. use this module if
		// it has higher priority than the best module or best module is fallback
		}else if( module->GetPriority() > bestModule->GetPriority() || bestModule->GetIsFallback() ){
			bestModule = module;
		}
	}
	
	return bestModule;
}



void delEngine::PutEngineIntoVFS( delEngineInstance &instance ){
	try{
		// get the properties from the engine
		instance.GetProperty( delEngineProcess::epPathEngineConfig, pPathConfig );
		instance.GetProperty( delEngineProcess::epPathEngineShare, pPathShare );
		instance.GetProperty( delEngineProcess::epPathEngineLib, pPathLib );
		instance.GetProperty( delEngineProcess::epPathEngineCache, pPathCache );
		
		// add the directories so they can be easily used later on.
		deVirtualFileSystem &vfs = *pLauncher.GetVFS();
		
		if( ! pPathConfig.IsEmpty() ){
			vfs.AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
				decPath::CreatePathUnix( "/engine/config" ),
				decPath::CreatePathNative( pPathConfig ), false ) ) );
		}
		
		if( ! pPathShare.IsEmpty() ){
			vfs.AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
				decPath::CreatePathUnix( "/engine/share" ),
				decPath::CreatePathNative( pPathShare ), false ) ) );
		}
		
		if( ! pPathLib.IsEmpty() ){
			vfs.AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
				decPath::CreatePathUnix( "/engine/lib" ),
				decPath::CreatePathNative( pPathLib ), false ) ) );
		}
		
		if( ! pPathCache.IsEmpty() ){
			vfs.AddContainer( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory(
				decPath::CreatePathUnix( "/engine/cache" ),
				decPath::CreatePathNative( pPathCache ), false ) ) );
		}
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( pLauncher.GetLogSource(),
			"Engine.PutEngineIntoVFS failed with exception:" );
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		throw;
	}
}



const char *delEngine::GetModuleTypeText( deModuleSystem::eModuleTypes moduleType ) const{
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
		
	case deModuleSystem::emtService:
		return "Service";
		
	case deModuleSystem::emtVR:
		return "VR";
		
	default:
		return "Unknown";
	}
}



void delEngine::UpdateResolutions( delEngineInstance &instance ){
	if( pResolutions ){
		delete [] pResolutions;
		pResolutions = NULL;
		pResolutionCount = 0;
	}
	
	const int display = 0;
	
	try{
		pCurrentResolution = instance.GetDisplayCurrentResolution( display );
		
		const int requiredCount = instance.GetDisplayResolutions( display, nullptr, 0 );
		if( requiredCount > 0 ){
			pResolutions = new decPoint[ requiredCount ];
			pResolutionCount = instance.GetDisplayResolutions( display, pResolutions, requiredCount );
		}
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( pLauncher.GetLogSource(),
			"Engine.UpdateResolutions failed with exception" );
		pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
		
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

const decPoint &delEngine::GetResolutionAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pResolutionCount ){
		DETHROW_INFO( deeInvalidParam, "index >= resolutionCount" );
	}
	return pResolutions[ index ];
}

int delEngine::IndexOfClosestResolutionTo( int width, int height ) const{
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

void delEngine::UpdateScaleFactor(delEngineInstance& instance){
	const int display = 0;
	
	try{
		pScaleFactor = instance.GetDisplayCurrentScaleFactor(display);
		
	}catch(const deException &e){
		pLauncher.GetLogger()->LogError(pLauncher.GetLogSource(),
			"Engine.UpdateScaleFactor failed with exception");
		pLauncher.GetLogger()->LogException(pLauncher.GetLogSource(), e);
		pScaleFactor = 100;
	}
}



void delEngine::ReadDelgaGameDefs( delEngineInstance &instance, const char *filename, delGameList &list ){
	// read content of all game definition files in delga file
	decStringList fileContents;
	
	try{
		instance.ReadDelgaGameDefs( filename, fileContents );
		
	}catch(const deException &e){
		pLauncher.GetLogger()->LogError(pLauncher.GetLogSource(),
			"Engine.ReadDelgaGameDefs failed with exception");
		pLauncher.GetLogger()->LogException(pLauncher.GetLogSource(), e);
	}
	
	// parse file contents into game instances
	list.RemoveAll();
	
	const int count = fileContents.GetCount();
	if( count == 0 ){
		return;
	}
	
	delGameXML gameXML( pLauncher.GetLogger(), pLauncher.GetLogSource() );
	const decMemoryFile::Ref memoryFile( decMemoryFile::Ref::New( new decMemoryFile( "DelgaGameDefinition" ) ) );
	delGame::Ref game;
	int i;
	
	for( i=0; i<count; i++ ){
		const decString &fileContent = fileContents.GetAt( i );
		const int lenContent = fileContent.GetLength();
		
		memoryFile->Resize( lenContent, false );
		memcpy( memoryFile->GetPointer(), fileContent.GetString(), lenContent );
		
		game.TakeOver( pLauncher.CreateGame() );
		gameXML.ReadFromFile( decMemoryFileReader::Ref::New( new decMemoryFileReader( memoryFile ) ), game );
		
		game->SetDefaultLogFile();
		game->SetDelgaFile( filename );
		
		list.Add( game );
		
		// read icons
		const delGameIconList &icons = game->GetIcons();
		const int iconCount = icons.GetCount();
		int j;
		
		for( j=0; j<iconCount; j++ ){
			delGameIcon &icon = *icons.GetAt( j );
			if( icon.GetPath().IsEmpty() ){
				continue;
			}
			
			try{
				decStringList readFilenames;
				readFilenames.Add( icon.GetPath() );
				
				decObjectOrderedSet filesContent;
				instance.ReadDelgaFiles( filename, readFilenames, filesContent );
				
				icon.SetContent( ( decMemoryFile* )filesContent.GetAt( 0 ) );
				
			}catch( const deException &e ){
				pLauncher.GetLogger()->LogErrorFormat( pLauncher.GetLogSource(),
					"Exception while reading icon '%s' for game '%s'",
					icon.GetPath().GetString(), game->GetTitle().ToUTF8().GetString() );
				pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
			}
		}
	}
}

void delEngine::ReadDelgaPatchDefs( delEngineInstance &instance, const char *filename, delPatchList &list ){
	// read content of all patch definition files in delga file
	decStringList fileContents;
	
	try{
		instance.ReadDelgaPatchDefs( filename, fileContents );
		
	}catch( const deException &e ){
		pLauncher.GetLogger()->LogError( pLauncher.GetLogSource(),
			"Engine.ReadDelgaPatchDefs failed with exception" );
			pLauncher.GetLogger()->LogException( pLauncher.GetLogSource(), e );
	}
	
	// parse file contents into patch instances
	list.RemoveAll();
	
	const int count = fileContents.GetCount();
	if( count == 0 ){
		return;
	}
	
	delPatchXML patchXML( pLauncher.GetLogger(), pLauncher.GetLogSource() );
	decMemoryFile::Ref memoryFile( decMemoryFile::Ref::New( new decMemoryFile( "DelgaPatchDefinition" ) ) );
	delPatch::Ref patch;
	int i;
	
	for( i=0; i<count; i++ ){
		const decString &fileContent = fileContents.GetAt( i );
		const int lenContent = fileContent.GetLength();
		
		memoryFile->Resize( lenContent, false );
		memcpy( memoryFile->GetPointer(), fileContent.GetString(), lenContent );
		
		patch.TakeOver( new delPatch );
		patchXML.ReadFromFile( decMemoryFileReader::Ref::New( new decMemoryFileReader( memoryFile ) ), patch );
		
		patch->SetDelgaFile( filename );
		
		list.Add( patch );
	}
}

#ifdef OS_ANDROID
void delEngine::ReadDelgaGameDefsVfs(delEngineInstance &instance,
const deVFSContainer::Ref &container, const char *filename, delGameList &list){
	// read content of all game definition files in delga file
	decStringList fileContents;
	
	try{
		instance.ReadDelgaGameDefsVfs(container, filename, fileContents);
		
	}catch(const deException &e){
		pLauncher.GetLogger()->LogError(pLauncher.GetLogSource(),
			"Engine.ReadDelgaGameDefsVfs failed with exception");
		pLauncher.GetLogger()->LogException(pLauncher.GetLogSource(), e);
	}
	
	// parse file contents into game instances
	list.RemoveAll();
	
	const int count = fileContents.GetCount();
	if(count == 0){
		return;
	}
	
	delGameXML gameXML(pLauncher.GetLogger(), pLauncher.GetLogSource());
	const decMemoryFile::Ref memoryFile(decMemoryFile::Ref::New(
		new decMemoryFile("DelgaGameDefinition")));
	int i;
	
	for(i=0; i<count; i++){
		const decString &fileContent = fileContents.GetAt(i);
		const int lenContent = fileContent.GetLength();
		
		memoryFile->Resize(lenContent, false);
		memcpy(memoryFile->GetPointer(), fileContent.GetString(), lenContent);
		
		const delGame::Ref game(delGame::Ref::New(pLauncher.CreateGame()));
		gameXML.ReadFromFile(decMemoryFileReader::Ref::New(
			new decMemoryFileReader(memoryFile)), game);
		
		game->SetDefaultLogFile();
		game->SetDelgaFile(filename);
		
		list.Add(game);
		
		// read icons
		const delGameIconList &icons = game->GetIcons();
		const int iconCount = icons.GetCount();
		int j;
		
		for(j=0; j<iconCount; j++){
			delGameIcon &icon = *icons.GetAt(j);
			if(icon.GetPath().IsEmpty()){
				continue;
			}
			
			try{
				decStringList readFilenames;
				readFilenames.Add(icon.GetPath());
				
				decObjectOrderedSet filesContent;
				instance.ReadDelgaFilesVfs(container, filename, readFilenames, filesContent);
				
				icon.SetContent((decMemoryFile*)filesContent.GetAt(0));
				
			}catch(const deException &e){
				pLauncher.GetLogger()->LogErrorFormat(pLauncher.GetLogSource(),
					"Exception while reading icon '%s' for game '%s'",
					icon.GetPath().GetString(), game->GetTitle().ToUTF8().GetString());
				pLauncher.GetLogger()->LogException(pLauncher.GetLogSource(), e);
			}
		}
	}
}
#endif



void delEngine::LoadConfig(){
	const decPath pathFile( decPath::CreatePathUnix( "/config/user/launcher.xml" ) );
	delEngineConfigXML configXML( pLauncher.GetLogger(), pLauncher.GetLogSource() );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	
	if( ! vfs.ExistsFile( pathFile ) ){
		logger.LogInfo( pLauncher.GetLogSource(),
			"Engine configuration file not found, will be created upon exiting" );
		return;
	}
	
	if( vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
		logger.LogErrorFormat( pLauncher.GetLogSource(), "Engine configuration file is not a regular file" );
		DETHROW_INFO( deeInvalidParam, "engine config file is not regular file" );
	}
	
	logger.LogInfo( pLauncher.GetLogSource(), "Reading engine configuration file" );
	configXML.ReadFromFile( decBaseFileReader::Ref::New( vfs.OpenFileForReading( pathFile ) ), pLauncher );
}

void delEngine::SaveConfig(){
	const decPath pathFile( decPath::CreatePathUnix( "/config/user/launcher.xml" ) );
	delEngineConfigXML configXML( pLauncher.GetLogger(), pLauncher.GetLogSource() );
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	
	if( ! vfs.CanWriteFile( pathFile ) ){
		logger.LogErrorFormat( pLauncher.GetLogSource(),
			"Failed to write engine configuration file (writing file problem)" );
		// DIALOG BOX
		// "Engine configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Engine configuration has not been saved!\n"
	}
	
	logger.LogInfo( pLauncher.GetLogSource(), "Writing engine configuration file" );
	
	try{
		configXML.WriteToFile( decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( pathFile ) ), pLauncher );
		
	}catch( const deException &e ){
		logger.LogErrorFormat( pLauncher.GetLogSource(), "Failed to write engine configuration file (file permission problem)" );
		logger.LogException( pLauncher.GetLogSource(), e );
		// DIALOG BOX
		// "Engine configuration can not be written!\n"
		// "Make sure you have write permission for the file and parent directory.\n"
		// "Engine configuration has not been saved!\n"
	}
}

void delEngine::Clear(){
}
