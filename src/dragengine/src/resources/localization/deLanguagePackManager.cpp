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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deLanguagePack.h"
#include "deLanguagePackBuilder.h"
#include "deLanguagePackManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/langpack/deBaseLanguagePackModule.h"



// Class deLanguagePackManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deLanguagePackManager::deLanguagePackManager( deEngine *engine ) :
deFileResourceManager( engine, ertLanguagePack )
{
	SetLoggingName( "language pack" );
}

deLanguagePackManager::~deLanguagePackManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deLanguagePackManager::GetLanguagePackCount() const{
	return pLangPacks.GetCount();
}

deLanguagePack *deLanguagePackManager::GetRootLanguagePack() const{
	return ( deLanguagePack* )pLangPacks.GetRoot();
}

deLanguagePack *deLanguagePackManager::GetLanguagePackWith( const char *filename ) const{
	return GetLanguagePackWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deLanguagePack *deLanguagePackManager::GetLanguagePackWith( deVirtualFileSystem *vfs,
const char *filename ) const{
	deLanguagePack * const langpack = ( deLanguagePack* )pLangPacks.GetWithFilename( vfs, filename );
	return langpack && ! langpack->GetOutdated() ? langpack : NULL;
}

deLanguagePack *deLanguagePackManager::CreateLanguagePack(
const char *filename, deLanguagePackBuilder &builder ){
	return CreateLanguagePack( GetEngine()->GetVirtualFileSystem(), filename, builder );
}

deLanguagePack *deLanguagePackManager::CreateLanguagePack( deVirtualFileSystem *vfs,
const char *filename, deLanguagePackBuilder &builder ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	deLanguagePack *langPack = NULL;
	
	try{
		deLanguagePack * const findLangPack = ( deLanguagePack* )
			pLangPacks.GetWithFilename( vfs, filename );
		if( findLangPack ){
			DETHROW( deeInvalidParam );
		}
		
		langPack = new deLanguagePack( this, vfs, filename, decDateTime::GetSystemTime() );
		builder.BuildLanguagePack( *langPack );
		
		if( ! langPack->Verify() ){
			DETHROW( deeInvalidParam );
		}
		langPack->BuildLookupTable();
		
		pLangPacks.Add( langPack );
		
	}catch( const deException & ){
		LogErrorFormat( "Creating language pack '%s' failed", filename );
		if( langPack ){
			langPack->FreeReference();
		}
		throw;
	}
	
	return langPack;
}

deLanguagePack *deLanguagePackManager::LoadLanguagePack(
const char *filename, const char *basePath ){
	return LoadLanguagePack( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deLanguagePack *deLanguagePackManager::LoadLanguagePack( deVirtualFileSystem *vfs,
const char *filename, const char *basePath ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReader *fileReader = NULL;
	deLanguagePack *langPack = NULL;
	deLanguagePack *findLangPack;
	decPath path;
	
	try{
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		findLangPack = ( deLanguagePack* )pLangPacks.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findLangPack && findLangPack->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Language Pack '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findLangPack->MarkOutdated();
			findLangPack = NULL;
		}
		
		if( findLangPack ){
			findLangPack->AddReference();
			langPack = findLangPack;
			
		}else{
			deBaseLanguagePackModule * const module = ( deBaseLanguagePackModule* )
				GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtLanguagePack,
					path.GetPathUnix() );
			
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix() );
			
			langPack = new deLanguagePack( this, vfs, path.GetPathUnix(), modificationTime );
			module->LoadLanguagePack( *fileReader, *langPack );
			
			fileReader->FreeReference();
			fileReader = NULL;
			
			if( ! langPack->Verify() ){
				DETHROW( deeInvalidParam );
			}
			langPack->BuildLookupTable();
			
			pLangPacks.Add( langPack );
		}
		
	}catch( const deException & ){
		LogErrorFormat( "Loading language pack '%s' (base path '%s') failed",
			filename, basePath ? basePath : "" );
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( langPack ){
			langPack->FreeReference();
		}
		throw;
	}
	
	return langPack;
}



void deLanguagePackManager::AddLoadedLanguagePack( deLanguagePack *languagePack ){
	if( ! languagePack ){
		DETHROW( deeInvalidParam );
	}
	pLangPacks.Add( languagePack );
}

void deLanguagePackManager::ReleaseLeakingResources(){
	const int count = GetLanguagePackCount();
	
	if( count > 0 ){
		deLanguagePack *langPack = ( deLanguagePack* )pLangPacks.GetRoot();
		
		LogWarnFormat( "%i leaking language packs", count );
		
		while( langPack ){
			LogWarnFormat( "- %s", langPack->GetFilename().GetString() );
			langPack = ( deLanguagePack* )langPack->GetLLManagerNext();
		}
		
		pLangPacks.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deLanguagePackManager::RemoveResource( deResource *resource ){
	pLangPacks.RemoveIfPresent( resource );
}
