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
