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

#include "deRLTaskReadLanguagePack.h"
#include "../deResourceLoader.h"
#include "../../localization/deLanguagePack.h"
#include "../../localization/deLanguagePackManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/langpack/deBaseLanguagePackModule.h"



// Class deRLTaskReadLanguagePack
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadLanguagePack::deRLTaskReadLanguagePack( deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path,
deLanguagePack *languagePack ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertLanguagePack ),
pSucceeded( false )
{
	LogCreateEnter();
	
	// if already loaded set finished
	if( languagePack ){
		pLanguagePack = languagePack;
		SetResource( languagePack );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pLanguagePack.TakeOver( new deLanguagePack( engine.GetLanguagePackManager(), vfs, path, 0 ) );
	
	LogCreateExit();
}

deRLTaskReadLanguagePack::~deRLTaskReadLanguagePack(){
}



// Management
///////////////

void deRLTaskReadLanguagePack::Run(){
	LogRunEnter();
	
	deBaseLanguagePackModule * const module = ( deBaseLanguagePackModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtLanguagePack, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pLanguagePack->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pLanguagePack->SetAsynchron( true );
	module->LoadLanguagePack( reader, pLanguagePack );
	reader = NULL;
	
	if( ! pLanguagePack->Verify() ){
		DETHROW( deeInvalidParam );
	}
	pLanguagePack->BuildLookupTable();
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadLanguagePack::Finished(){
	LogFinishedEnter();
	
	if( ! pSucceeded ){
		SetState( esFailed );
		pLanguagePack = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deLanguagePackManager &manager = *GetEngine().GetLanguagePackManager();
	deLanguagePack * const checkLangPack = manager.GetLanguagePackWith( GetPath() );
	
	if( checkLangPack ){
		SetResource( checkLangPack );
		
	}else{
		pLanguagePack->SetAsynchron( false );
		manager.AddLoadedLanguagePack( pLanguagePack );
		SetResource( pLanguagePack );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadLanguagePack::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-LangPack-Read";
}
