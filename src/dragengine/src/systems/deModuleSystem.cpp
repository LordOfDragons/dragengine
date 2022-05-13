/* 
 * Drag[en]gine Game Engine
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/deLibraryModule.h"
#include "modules/deBaseModule.h"
#include "../deEngine.h"
#include "../app/deOS.h"
#include "../common/file/decPath.h"
#include "../common/exceptions.h"
#include "../filesystem/dePathList.h"
#include "../filesystem/deCollectDirectorySearchVisitor.h"
#include "../filesystem/deVFSDiskDirectory.h"
#include "../filesystem/deVFSContainerReference.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../filesystem/deVirtualFileSystemReference.h"
#include "../logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "Dragengine"



// Class deModuleSystem
/////////////////////////

// Constructor, destructor
////////////////////////////

deModuleSystem::deModuleSystem( deEngine *engine ){
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	pEngine = engine;
}

deModuleSystem::~deModuleSystem(){
	if( pModules.GetCount() == 0 ){
		return;
	}
	
	deLogger &logger = *pEngine->GetLogger();
	
	while( pModules.GetCount() > 0 ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( pModules.GetCount() - 1 );
		const decString moduleVersion( module->GetVersion() );
		const decString moduleName( module->GetName() );
		
		if( module->IsLocked() ){
			logger.LogWarnFormat( LOGSOURCE, "Module %s %s has not been unlocked properly",
				moduleName.GetString(), moduleVersion.GetString() );
		}
		if( module->GetRefCount() > 1 ){
			logger.LogWarnFormat( LOGSOURCE, "Module %s %s has reference count greater than 1",
				moduleName.GetString(), moduleVersion.GetString() );
		}
		logger.LogInfoFormat( LOGSOURCE, "Release module %s %s",
			moduleName.GetString(), moduleVersion.GetString() );
		
		try{
			pModules.RemoveFrom( pModules.GetCount() - 1 );
			
		}catch( const deException &e ){
			logger.LogErrorFormat( LOGSOURCE, "Releasing module %s %s failed with exception:",
				moduleName.GetString(), moduleVersion.GetString() );
			logger.LogException( LOGSOURCE, e );
		}
	}
}



// Management
///////////////

void deModuleSystem::DetectModules(){
	deLogger &logger = *pEngine->GetLogger();
	decPath searchPath;
	
	searchPath.SetFromNative( pEngine->GetOS()->GetPathEngine() );
	searchPath.AddUnixPath( DEGS_MODULES_PATH );
	
	try{
		logger.LogInfoFormat( LOGSOURCE, "Loading Crash Recovery modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "crashrecovery", emtCrashRecovery );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Graphic modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "graphic", emtGraphic );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Input modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "input", emtInput );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Physics modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "physics", emtPhysics );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Audio modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "audio", emtAudio );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Network modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "network", emtNetwork );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Scripting modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "scripting", emtScript );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Animator modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "animator", emtAnimator );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading AI modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "ai", emtAI );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Synthesizer modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "synthesizer", emtSynthesizer );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading VR modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "vr", emtVR );
		
		
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Archive modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "archive", emtArchive );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Animation modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "animation", emtAnimation );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Font modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "font", emtFont );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Image modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "image", emtImage );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Model modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "model", emtModel );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Rig modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "rig", emtRig );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Skin modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "skin", emtSkin );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Language Pack modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "langpack", emtLanguagePack );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Sound modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "sound", emtSound );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Video modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "video", emtVideo );
		
		logger.LogInfoFormat( LOGSOURCE, "Loading Occlusion Mesh modules" );
		pDetectModulesIn( searchPath.GetPathNative(), "occlusionmesh", emtOcclusionMesh );
		
		logger.LogInfoFormat( LOGSOURCE, "Finished loading modules" );
		
	}catch( const deException &e ){
		logger.LogException( LOGSOURCE, e );
	}
}



// Module management
//////////////////////

int deModuleSystem::GetModuleCountFor( eModuleTypes type ) const{
	int count = 0;
	int i;
	
	for( i=0; i<pModules.GetCount(); i++ ){
		if( ( ( deLoadableModule* )pModules.GetAt( i ) )->GetType() == type ){
			count++;
		}
	}
	
	return count;
}

int deModuleSystem::GetLoadedModuleCountFor( eModuleTypes type ) const{
	int count = 0;
	int i;
	
	for( i=0; i<pModules.GetCount(); i++ ){
		deLoadableModule &module = *( ( deLoadableModule* )pModules.GetAt( i ) );
		if( module.GetType() == type && module.IsLoaded() && module.GetEnabled() ){
			count++;
		}
	}
	
	return count;
}

int deModuleSystem::GetModuleCount() const{
	return pModules.GetCount();
}

deLoadableModule *deModuleSystem::GetModuleAt( int index ) const{
	return ( deLoadableModule* )pModules.GetAt( index );
}

deLoadableModule *deModuleSystem::GetModuleNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	deLoadableModule *latestModule = NULL;
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( i );
		if( module->GetName() != name || ! module->GetEnabled() ){
			continue;
		}
		if( latestModule && CompareVersion( module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

deLoadableModule *deModuleSystem::GetModuleNamed( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( i );
		if( module->GetName() == name && module->GetVersion() == version ){
			return module;
		}
	}
	
	return NULL;
}

deLoadableModule *deModuleSystem::GetModuleNamedAtLeast( const char *name, const char *version ) const{
	if( ! name || ! version ){
		DETHROW( deeInvalidParam );
	}
	
	deLoadableModule *latestModule = NULL;
	const int count = pModules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( i );
		if( module->GetName() != name || ! module->GetEnabled()
		|| CompareVersion( module->GetVersion(), version ) < 0 ){
			continue;
		}
		if( latestModule && CompareVersion( module->GetVersion(), latestModule->GetVersion() ) <= 0 ){
			continue;
		}
		
		latestModule = module;
	}
	
	return latestModule;
}

deLoadableModule *deModuleSystem::GetFirstLoadedModuleFor( eModuleTypes type ) const{
	deLoadableModule *useModule = NULL;
	int i;
	
	for( i=0; i<pModules.GetCount(); i++ ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( i );
		if( module->GetType() == type && module->IsLoaded() && module->GetEnabled() ){
			useModule = module;
			if( ! module->GetIsFallback() ){
				break;
			}
		}
	}
	
	return useModule;
}

void deModuleSystem::AddModule( deLoadableModule *module ){
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	if( GetModuleNamed( module->GetName(), module->GetVersion() ) ){
		DETHROW( deeInvalidParam );
	}
	if( module->GetSystem() != this ){
		DETHROW( deeInvalidParam );
	}
	
	pModules.Add( module );
}

deLoadableModule *deModuleSystem::FindMatching( eModuleTypes type, const char *filename ) const{
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	deLoadableModule *latestModule = NULL;
	int i, j;
	
	for( i=0; i<pModules.GetCount(); i++ ){
		deLoadableModule * const module = ( deLoadableModule* )pModules.GetAt( i );
		if( module->GetType() != type ){
			continue;
		}
		
		const decStringList &patternList = module->GetPatternList();
		const int patternCount = patternList.GetCount();
		
		for( j=0; j<patternCount; j++ ){
			if( ! MatchesPattern( filename, patternList.GetAt( j ) ) || ! module->GetEnabled() ){
				continue;
			}
			
			if( latestModule && module->GetName() != latestModule->GetName() ){
				// in case different modules match same pattern stick with the same
				// module for version checking
				continue;
			}
			
			if( ! latestModule || CompareVersion( module->GetVersion(), latestModule->GetVersion() ) > 0 ){
				latestModule = module;
			}
		}
	}
	
	return latestModule;
}

deBaseModule *deModuleSystem::GetModuleAbleToLoad( eModuleTypes type, const char *filename ) const{
	deLoadableModule * const module = FindMatching( type, filename );
	
	if( ! module ){
		GetEngine()->GetLogger()->LogErrorFormat( LOGSOURCE, "No %s module found able to handle file '%s'",
			GetTypeDirectory( type ), filename );
		DETHROW( deeInvalidParam );
	}
	
	if( ! module->IsLoaded() ){
		GetEngine()->GetLogger()->LogErrorFormat( LOGSOURCE, "Module %s would be able to handle file '%s' but is not loaded",
			module->GetName().GetString(), filename );
		DETHROW( deeInvalidParam );
	}
	
	return module->GetModule();
}



// Helper functions
/////////////////////

int deModuleSystem::CompareVersion( const char *version1, const char *version2 ){
	if( ! version1 || ! version2 ){
		DETHROW( deeInvalidParam );
	}
	
	const int len1 = strlen( version1 );
	const int len2 = strlen( version2 );
	int last1, last2;
	int pos1 = 0;
	int pos2 = 0;
	
	while( pos1 < len1 && pos2 < len2 ){
		// find next version parts
		last1 = pos1;
		while( pos1 < len1 && version1[ pos1 ] != '.' ){
			pos1++;
		}
		
		last2 = pos2;
		while( pos2 < len2 && version2[ pos2 ] != '.' ){
			pos2++;
		}
		
		// check if they differ. a missing version part is assumed to be 0
		const long part1 = strtol( version1 + last1, NULL, 10 );
		const long part2 = strtol( version2 + last2, NULL, 10 );
		
		if( part1 < part2 ){
			return -1;
		}
		if( part1 > part2 ){
			return 1;
		}
		
		// skip the dots
		pos1++;
		pos2++;
	}
	
	return 0;
}

bool deModuleSystem::MatchesPattern( const char *filename, const char *pattern ) const{
	if( ! filename || ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	const char *deli, *fileExt;
	
	// find file extension. we have to determine first how many dots the pattern contains.
	// this is required since otherwise a wrong extension could be compared. just taking
	// the first dot from the end could be too short while taking the first dot from the
	// beginning could be too long
	int dotCount = 0;
	
	fileExt = pattern;
	while( *fileExt ){
		if( *fileExt == '.' ){
			dotCount++;
		}
		fileExt++;
	}
	if( dotCount < 0 ){
		dotCount = 1;
	}
	
	fileExt = filename + ( strlen( filename ) - 1 );
	while( fileExt != filename ){
		if( *fileExt == '.' ){
			dotCount--;
			if( dotCount == 0 ){
				break;
			}
		}
		fileExt--;
	}
	
	// check with patterns
	while( *pattern ){
		while( *pattern && *pattern != '.' ){
			pattern++;
		}
		if( ! *pattern ) break;
		
		deli = pattern;
		while( *deli && ! isspace( *deli ) ){
			deli++;
		}
		
		if( StrEqual( fileExt, pattern, ( int )( deli - pattern ) ) ){
			return true;
		}
		
		if( *deli ){
			pattern = deli + 1;
			
		}else{
			pattern = deli;
		}
	}
	
	return false;
}

bool deModuleSystem::StrEqual(const char *str1, const char *str2, int length) const{
	for(int i=0; i<length; i++){
		if( tolower(str1[i]) != tolower(str2[i]) ) return false;
		if( !str1[i] || !str2[i] ) return false;
	}
	return str1[length] == '\0';
}

deModuleSystem::eModuleTypes deModuleSystem::GetTypeFromString( const char *typeString ){
	if( strcmp( typeString, "Graphic" ) == 0 ){
		return emtGraphic;
		
	}else if( strcmp( typeString, "Audio" ) == 0 ){
		return emtAudio;
		
	}else if( strcmp( typeString, "Input" ) == 0 ){
		return emtInput;
		
	}else if( strcmp( typeString, "Network" ) == 0 ){
		return emtNetwork;
		
	}else if( strcmp( typeString, "Physics" ) == 0 ){
		return emtPhysics;
		
	}else if( strcmp( typeString, "Image" ) == 0 ){
		return emtImage;
		
	}else if( strcmp( typeString, "Video" ) == 0 ){
		return emtVideo;
		
	}else if( strcmp( typeString, "Script" ) == 0 ){
		return emtScript;
		
	}else if( strcmp( typeString, "Model" ) == 0 ){
		return emtModel;
		
	}else if( strcmp( typeString, "Rig" ) == 0 ){
		return emtRig;
		
	}else if( strcmp( typeString, "Skin" ) == 0 ){
		return emtSkin;
		
	}else if( strcmp( typeString, "Animation" ) == 0 ){
		return emtAnimation;
		
	}else if( strcmp( typeString, "Font" ) == 0 ){
		return emtFont;
		
	}else if( strcmp( typeString, "CrashRecovery" ) == 0 ){
		return emtCrashRecovery;
		
	}else if( strcmp( typeString, "LanguagePack" ) == 0 ){
		return emtLanguagePack;
		
	}else if( strcmp( typeString, "Animator" ) == 0 ){
		return emtAnimator;
		
	}else if( strcmp( typeString, "Sound" ) == 0 ){
		return emtSound;
		
	}else if( strcmp( typeString, "AI" ) == 0 ){
		return emtAI;
		
	}else if( strcmp( typeString, "OcclusionMesh" ) == 0 ){
		return emtOcclusionMesh;
		
	}else if( strcmp( typeString, "Synthesizer" ) == 0 ){
		return emtSynthesizer;
		
	}else if( strcmp( typeString, "Archive" ) == 0 ){
		return emtArchive;
		
	}else if( strcmp( typeString, "VR" ) == 0 ){
		return emtVR;
		
	}else{
		return emtUnknown;
	}
}

const char *deModuleSystem::GetTypeDirectory( eModuleTypes type ){
	switch( type ){
	case emtGraphic:
		return "graphic";
		
	case emtAudio:
		return "audio";
		
	case emtInput:
		return "input";
		
	case emtNetwork:
		return "network";
		
	case emtPhysics:
		return "physics";
		
	case emtImage:
		return "image";
		
	case emtVideo:
		return "video";
		
	case emtScript:
		return "scripting";
		
	case emtModel:
		return "model";
		
	case emtRig:
		return "rig";
		
	case emtSkin:
		return "skin";
		
	case emtAnimation:
		return "animation";
		
	case emtFont:
		return "font";
		
	case emtCrashRecovery:
		return "crashrecovery";
		
	case emtLanguagePack:
		return "langpack";
		
	case emtAnimator:
		return "animator";
		
	case emtSound:
		return "sound";
		
	case emtAI:
		return "ai";
		
	case emtOcclusionMesh:
		return "occlusionmesh";
		
	case emtSynthesizer:
		return "synthesizer";
		
	case emtArchive:
		return "archive";
		
	case emtVR:
		return "vr";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

bool deModuleSystem::IsSingleType( eModuleTypes type ){
	switch( type ){
	case emtAI:
	case emtAnimator:
	case emtAudio:
	case emtCrashRecovery:
	case emtGraphic:
	case emtInput:
	case emtNetwork:
	case emtPhysics:
	case emtScript:
	case emtSynthesizer:
	case emtVR:
		return true;
		
	default:
		return false;
	}
}



// Private Functions
//////////////////////

void deModuleSystem::pDetectModulesIn( const char *basePath, const char *directory, eModuleTypes type ){
	deLogger &logger = *pEngine->GetLogger();
	decPath searchPath, modulePath;
	deLibraryModule *module = NULL;
	int i, j;
	
	try{
		// find directories
		deVirtualFileSystemReference vfs;
		vfs.TakeOver( new deVirtualFileSystem );
		
		deVFSContainerReference container;
		searchPath.SetFromNative( basePath );
		searchPath.AddUnixPath( directory );
		container.TakeOver( new deVFSDiskDirectory( searchPath ) );
		vfs->AddContainer( container );
		
		deCollectDirectorySearchVisitor collect;
		vfs->SearchFiles( decPath::CreatePathUnix( "/" ), collect );
		
		const dePathList &pathList = collect.GetDirectories();
		for( i=0; i<pathList.GetCount(); i++ ){
			// search for version directories
			deCollectDirectorySearchVisitor collect2;
			vfs->SearchFiles( pathList.GetAt( i ), collect2 );
			
			const dePathList &pathListVersion = collect2.GetDirectories();
			for( j=0; j<pathListVersion.GetCount(); j++ ){
				// create path for module definition file
				modulePath.SetFrom( pathListVersion.GetAt( j ) );
				modulePath.AddUnixPath( "module.xml" );
				
				// test if module file exists and is a regular file
				if( ! vfs->ExistsFile( modulePath )
				|| vfs->GetFileType( modulePath ) != deVFSContainer::eftRegularFile ){
					continue;
				}
				
				decPath logPathModule( pathList.GetAt( i ) );
				logPathModule.SetPrefix( "" );
				
				logger.LogInfoFormat( LOGSOURCE, "- loading module %s %s",
					logPathModule.GetPathUnix().GetString(),
					pathListVersion.GetAt( j ).GetLastComponent().GetString() );
				
				// create native path for module definition file
				modulePath = searchPath + pathListVersion.GetAt( j );
				modulePath.AddUnixPath( "module.xml" );
				
				// try loading module. use an own try-catch to continue loading other modules in case this one fails badly
				try{
					module = new deLibraryModule( this, modulePath.GetPathNative() );
					
					// load module
					module->LoadModule();
					
					switch( module->GetErrorCode() ){
					case deLoadableModule::eecSuccess:
						break;
						
					case deLibraryModule::eecLibFileNotFound:
						logger.LogErrorFormat( LOGSOURCE, "File %s not found", module->GetLibFileName().GetString() );
						break;
						
					case deLibraryModule::eecLibFileNotRegularFile:
						logger.LogErrorFormat( LOGSOURCE, "File %s is not a regular file", module->GetLibFileName().GetString() );
						break;
						
					case deLibraryModule::eecLibFileSizeMismatch:
						logger.LogErrorFormat( LOGSOURCE, "File size check for %s failed", module->GetLibFileName().GetString() );
						break;
						
					case deLibraryModule::eecLibFileCheckSumMismatch:
						logger.LogErrorFormat( LOGSOURCE, "File checksum check for %s failed", module->GetLibFileName().GetString() );
						break;
						
					case deLibraryModule::eecLibFileOpenFailed:
						logger.LogErrorFormat( LOGSOURCE, "Library %s could not be opened", module->GetLibFileName().GetString() );
						break;
						
					case deLibraryModule::eecLibFileEntryPointNotFound:
						logger.LogErrorFormat( LOGSOURCE, "Library %s entry point %s not found",
							module->GetLibFileName().GetString(), module->GetLibFileEntryPoint().GetString() );
						break;
						
					case deLibraryModule::eecLibFileCreateModuleFailed:
					default:
						logger.LogError( LOGSOURCE, "Creating module failed" );
					}
					
					// verify that the module type matches
					if( module->GetType() != type ){
						logger.LogWarnFormat( LOGSOURCE, "Module %s %s has wrong type. Place the module in the correct directory",
							module->GetName().GetString(), module->GetVersion().GetString() );
					}
					
					// add module and free filename
					AddModule( module );
					module->FreeReference();
					module = NULL;
					
				}catch( const deException &e ){
					if( module ){
						module->FreeReference();
						module = NULL;
					}
					logger.LogException( LOGSOURCE, e );
				}
			}
		}
		
	}catch( const deException &e ){
		if( module ){
			module->FreeReference();
		}
		logger.LogException( LOGSOURCE, e );
	}
}
