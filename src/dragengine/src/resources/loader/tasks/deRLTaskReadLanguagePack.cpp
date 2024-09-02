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
