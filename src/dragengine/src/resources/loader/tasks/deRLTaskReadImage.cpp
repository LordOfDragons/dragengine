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

#include "deRLTaskReadImage.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/image/deBaseImageInfo.h"
#include "../../../systems/modules/image/deBaseImageModule.h"



// Class deRLTaskReadImage
////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadImage::deRLTaskReadImage( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deImage *image ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertImage ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( image ){
		pImage = image;
		SetResource( image );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pImage.TakeOver( new deImage( engine.GetImageManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadImage::~deRLTaskReadImage(){
}



// Management
///////////////

void deRLTaskReadImage::Run(){
	LogRunEnter();
	deBaseImageModule * const module = ( deBaseImageModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtImage, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	deBaseImageInfo *infos = NULL;
	
	try{
		decBaseFileReader::Ref reader(decBaseFileReader::Ref::New(
			 GetVFS()->OpenFileForReading( vfsPath ) ));
		
		infos = module->InitLoadImage( reader );
		if( ! infos ){
			DETHROW( deeInvalidParam );
		}
		
		pImage->FinalizeConstruction( infos->GetWidth(), infos->GetHeight(),
			infos->GetDepth(), infos->GetComponentCount(), infos->GetBitCount() );
		pImage->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
		pImage->SetAsynchron( true );
		
		module->LoadImage( reader, pImage, *infos );
		
		delete infos;
		
	}catch( const deException & ){
		if( infos ){
			delete infos;
		}
		throw;
	}
	
	GetEngine().GetGraphicSystem()->LoadImage( pImage );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadImage::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pImage = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deImageManager &imageManager = *GetEngine().GetImageManager();
	deImage * const checkImage = imageManager.GetImageWith( GetPath() );
	
	if( checkImage ){
		SetResource( checkImage );
		
	}else{
		imageManager.AddLoadedImage( pImage );
		
		pImage->SetAsynchron( false );
		pImage->PeersRetainImageData();
		
		SetResource( pImage );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadImage::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Image-Read";
}
