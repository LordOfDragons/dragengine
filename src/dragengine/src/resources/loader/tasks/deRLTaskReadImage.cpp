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

#include "deRLTaskReadImage.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
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
		decBaseFileReaderReference reader;
		reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
		
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
