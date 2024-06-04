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

#include "deResourceLoader.h"
#include "deResourceLoaderInfo.h"
#include "tasks/deRLTaskReadAnimation.h"
#include "tasks/deRLTaskReadFont.h"
#include "tasks/deRLTaskReadImage.h"
#include "tasks/deRLTaskReadLanguagePack.h"
#include "tasks/deRLTaskReadModel.h"
#include "tasks/deRLTaskReadOcclusionMesh.h"
#include "tasks/deRLTaskReadRig.h"
#include "tasks/deRLTaskReadSkin.h"
#include "tasks/deRLTaskReadSound.h"
#include "tasks/deRLTaskReadVideo.h"
#include "tasks/deRLTaskWriteAnimation.h"
#include "tasks/deRLTaskWriteImage.h"
#include "tasks/deRLTaskWriteModel.h"
#include "tasks/deRLTaskWriteOcclusionMesh.h"
#include "tasks/deRLTaskWriteRig.h"
#include "../deFileResource.h"
#include "../animation/deAnimation.h"
#include "../animation/deAnimationManager.h"
#include "../font/deFont.h"
#include "../font/deFontManager.h"
#include "../image/deImage.h"
#include "../image/deImageManager.h"
#include "../localization/deLanguagePack.h"
#include "../localization/deLanguagePackManager.h"
#include "../model/deModel.h"
#include "../model/deModelManager.h"
#include "../occlusionmesh/deOcclusionMesh.h"
#include "../occlusionmesh/deOcclusionMeshManager.h"
#include "../rig/deRig.h"
#include "../rig/deRigManager.h"
#include "../skin/deSkin.h"
#include "../skin/deSkinManager.h"
#include "../sound/deSound.h"
#include "../sound/deSoundManager.h"
#include "../video/deVideo.h"
#include "../video/deVideoManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"
#include "../../parallel/deParallelProcessing.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"



// Class deResourceLoader
///////////////////////////

// Constructor, destructor
////////////////////////////

deResourceLoader::deResourceLoader( deEngine &engine ) :
pEngine( engine ),
pLoadAsynchron( true ),
pOutputDebugMessages( false ){
}

deResourceLoader::~deResourceLoader(){
	pCleanUp();
}



// Management
///////////////

void deResourceLoader::SetOutputDebugMessages( bool outputDebugMessages ){
	pOutputDebugMessages = outputDebugMessages;
}

deResourceLoaderTask *deResourceLoader::AddLoadRequest( deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType ){
	// if a tasks exists already use this one. tasks stick around only as long as
	// the script module has not collected them
	deResourceLoaderTask *task = pGetTaskWith( vfs, path, resourceType );
	if( task ){
		return task;
	}
	
	// create and add task
	deFileResource *freeResource = NULL;
	
	try{
		// create task
		switch( resourceType ){
		case ertAnimation:{
			deAnimation *animation = pEngine.GetAnimationManager()->GetAnimationWith( vfs, path );
			if( ! animation && ! pLoadAsynchron ){
				animation = pEngine.GetAnimationManager()->LoadAnimation( vfs, path, "/" );
				freeResource = animation;
			}
			task = new deRLTaskReadAnimation( pEngine, *this, vfs, path, animation );
			}break;
			
		case ertFont:{
			deFont *font = pEngine.GetFontManager()->GetFontWith( vfs, path );
			if( ! font && ! pLoadAsynchron ){
				font = pEngine.GetFontManager()->LoadFont( vfs, path, "/" );
				freeResource = font;
			}
			task = new deRLTaskReadFont( pEngine, *this, vfs, path, font );
			}break;
			
		case ertImage:{
			deImage *image = pEngine.GetImageManager()->GetImageWith( vfs, path );
			if( ! image && ! pLoadAsynchron ){
				image = pEngine.GetImageManager()->LoadImage( vfs, path, "/" );
				freeResource = image;
			}
			task = new deRLTaskReadImage( pEngine, *this, vfs, path, image );
			}break;
			
		case ertModel:{
			deModel *model = pEngine.GetModelManager()->GetModelWith( vfs, path );
			if( ! model && ! pLoadAsynchron ){
				model = pEngine.GetModelManager()->LoadModel( vfs, path, "/" );
				freeResource = model;
			}
			task = new deRLTaskReadModel( pEngine, *this, vfs, path, model );
			}break;
			
		case ertOcclusionMesh:{
			deOcclusionMesh *occlusionMesh = pEngine.GetOcclusionMeshManager()
				->GetOcclusionMeshWith( vfs, path );
			if( ! occlusionMesh && ! pLoadAsynchron ){
				occlusionMesh = pEngine.GetOcclusionMeshManager()->LoadOcclusionMesh(
					vfs, path, "/" );
				freeResource = occlusionMesh;
			}
			task = new deRLTaskReadOcclusionMesh( pEngine, *this, vfs, path, occlusionMesh );
			}break;
			
		case ertRig:{
			deRig *rig = pEngine.GetRigManager()->GetRigWith( vfs, path );
			if( ! rig && ! pLoadAsynchron ){
				rig = pEngine.GetRigManager()->LoadRig( vfs, path, "/" );
				freeResource = rig;
			}
			task = new deRLTaskReadRig( pEngine, *this, vfs, path, rig );
			}break;
			
		case ertSkin:{
			deSkin *skin = pEngine.GetSkinManager()->GetSkinWith( vfs, path );
			if( ! skin && ! pLoadAsynchron ){
				skin = pEngine.GetSkinManager()->LoadSkin( vfs, path, "/" );
				freeResource = skin;
			}
			task = new deRLTaskReadSkin( pEngine, *this, vfs, path, skin );
			}break;
			
		case ertSound:{
			deSound *sound = pEngine.GetSoundManager()->GetSoundWith( vfs, path );
			if( ! sound && ! pLoadAsynchron ){
				sound = pEngine.GetSoundManager()->LoadSound( path, "/", false );
				freeResource = sound;
			}
			task = new deRLTaskReadSound( pEngine, *this, vfs, path, sound );
			}break;
			
		case ertLanguagePack:{
			deLanguagePack *langPack = pEngine.GetLanguagePackManager()->
				GetLanguagePackWith( vfs, path );
			if( ! langPack && ! pLoadAsynchron ){
				langPack = pEngine.GetLanguagePackManager()->LoadLanguagePack( vfs, path, "/" );
				freeResource = langPack;
			}
			task = new deRLTaskReadLanguagePack( pEngine, *this, vfs, path, langPack );
			}break;
			
		case ertVideo:{
			deVideo *video = pEngine.GetVideoManager()->GetVideoWith( vfs, path );
			if( ! video && ! pLoadAsynchron ){
				video = pEngine.GetVideoManager()->LoadVideo( path, "/", false );
				freeResource = video;
			}
			task = new deRLTaskReadVideo( pEngine, *this, vfs, path, video );
			}break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
	}catch( const deException &e ){
		if( freeResource ){
			freeResource->FreeReference();
		}
		pEngine.GetLogger()->LogException( LOGSOURCE, e );
		throw;
	}
	
	if( freeResource ){
		freeResource->FreeReference(); // direct loading adds a reference
	}
	
	// add task to the appropriate list
	if( task->GetState() == deResourceLoaderTask::esPending ){
		if( pOutputDebugMessages ){
			const decString debugName( task->GetDebugName() );
			pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Add Pending Task(%s)[%s]",
				debugName.GetString(), path );
		}
		pPendingTasks.Add( task );
		pEngine.GetParallelProcessing().AddTask( task );
		
	}else{
		if( pOutputDebugMessages ){
			const decString debugName( task->GetDebugName() );
			pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Add Finished Task(%s)[%s]",
				debugName.GetString(), path );
		}
		pFinishedTasks.Add( task );
	}
	
	task->FreeReference();
	return task;
}

deResourceLoaderTask *deResourceLoader::AddSaveRequest( deVirtualFileSystem *vfs,
const char *path, deFileResource *resource ){
	// TODO
	// note. if resource is finished mark created task finished
	return NULL;
}

bool deResourceLoader::NextFinishedRequest( deResourceLoaderInfo &info ){
	deResourceLoaderTask *task = NULL;
	
	while( true ){
		if( pFinishedTasks.GetCount() > 0 ){
			task = ( deResourceLoaderTask * )pFinishedTasks.GetAt( 0 );
			task->AddReference();
			pFinishedTasks.Remove( task );
		}
		
		if( task ){
			// taskX->GetType() = deResourceLoaderTaskX::{etRead, etWrite}
			info.SetPath( task->GetPath() );
			info.SetResourceType( task->GetResourceType() );
			
			switch( task->GetState() ){
			case deResourceLoaderTask::esSucceeded:
				// NOTE if task is purely internal counting finished files becomes
				//      a problem since the counter is off. should be anyways not
				//      done using counters since this is in general a problem
				//      (etRead)
				info.SetResource( task->GetResource() );
				break;
				
			case deResourceLoaderTask::esFailed:
				info.SetResource( NULL );
				break;
				
			default:{
				const decString debugName( task->GetDebugName() );
				pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Finished Task has invalid state (%s)[%s][%d]",
					debugName.GetString(), task->GetPath().GetString(), task->GetState() );
				DETHROW( deeInvalidParam );
				}
			}
			
			task->FreeReference();
			return true;
		}
		
		break;
	}
	
	return false;
}



void deResourceLoader::RemoveAllTasks(){
	if( pOutputDebugMessages ){
		pEngine.GetLogger()->LogInfo( LOGSOURCE, "Stop tasks in progress and clear all tasks" );
	}
	
	int i, count = pPendingTasks.GetCount();
	for( i=0; i<count; i++ ){
		( ( deResourceLoaderTask* )pPendingTasks.GetAt( i ) )->Cancel();
	}
	
	count = pFinishedTasks.GetCount();
	for( i=0; i<count; i++ ){
		( ( deResourceLoaderTask* )pFinishedTasks.GetAt( i ) )->Cancel();
	}
	
	const bool resumeParallel = ! pEngine.GetParallelProcessing().GetPaused();
	
	if( resumeParallel ){
		pEngine.GetParallelProcessing().Pause();
	}
	
	pPendingTasks.RemoveAll();
	pFinishedTasks.RemoveAll();
	
	if( resumeParallel ){
		pEngine.GetParallelProcessing().Resume();
	}
}



// Internal use only
//////////////////////

void deResourceLoader::FinishTask( deResourceLoaderTask *task ){
	if( ! task ){
		DETHROW( deeInvalidParam );
	}
	pFinishedTasks.AddIfAbsent( task );
	pPendingTasks.RemoveIfPresent( task );
}



// Private Functions
//////////////////////

void deResourceLoader::pCleanUp(){
	RemoveAllTasks();
}

bool deResourceLoader::pHasTaskWith( deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType ) const{
	int i, count = pPendingTasks.GetCount();
	for( i=0; i<count; i++ ){
		if( ( ( deResourceLoaderTask* )pPendingTasks.GetAt( i ) )->Matches( vfs, path, resourceType ) ){
			return true;
		}
	}
	
	count = pFinishedTasks.GetCount();
	for( i=0; i<count; i++ ){
		if( ( ( deResourceLoaderTask* )pFinishedTasks.GetAt( i ) )->Matches( vfs, path, resourceType ) ){
			return true;
		}
	}
	
	return false;
}

deResourceLoaderTask *deResourceLoader::pGetTaskWith( deVirtualFileSystem *vfs,
const char *path, eResourceType resourceType ) const{
	int i, count = pPendingTasks.GetCount();
	for( i=0; i<count; i++ ){
		deResourceLoaderTask * const task = ( deResourceLoaderTask* )pPendingTasks.GetAt( i );
		if( task->Matches( vfs, path, resourceType ) ){
			return task;
		}
	}
	
	count = pFinishedTasks.GetCount();
	for( i=0; i<count; i++ ){
		deResourceLoaderTask * const task = ( deResourceLoaderTask* )pFinishedTasks.GetAt( i );
		if( task->Matches( vfs, path, resourceType ) ){
			return task;
		}
	}
	
	return NULL;
}
