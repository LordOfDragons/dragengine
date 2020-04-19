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

#include "deRLTaskReadSkin.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../skin/deSkin.h"
#include "../../skin/deSkinManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/dePhysicsSystem.h"



// Class deRLTaskReadSkin
///////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkin::deRLTaskReadSkin( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deSkin *skin ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertSkin ),
pInternalTask( NULL ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( skin ){
		SetResource( skin );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	// skins have to be loaded always synchronously. this is because the texture property
	// map is accessed for each loaded texture property. doing this asynchronously has
	// very ill effects up to crashing. loading synchronously is not a problem since skins
	// are small files only defining where to get the resources from, namely images. these
	// are loaded later on in an asynchronous way
	try{
		pInternalTask = new deRLTaskReadSkinInternal( engine, resourceLoader, vfs, path );
		
		switch( pInternalTask->GetState() ){
		case esPending:
			AddDependsOn( pInternalTask );
			engine.GetParallelProcessing().AddTask( pInternalTask );
			break;
			
		case esSucceeded:
			break;
			
		case esFailed:
			SetState( esFailed );
			Cancel();
			break;
		}
		
	}catch( const deException & ){
		// avoid nasty reference bug if exception thrown with bad timing
		//RemoveAllDependsOn(); // if we do not throw we do not need this
		//pCleanUp();
		//throw;
		
		SetState( esFailed );
		Cancel();
	}
	LogCreateExit();
}

deRLTaskReadSkin::~deRLTaskReadSkin(){
	pCleanUp();
}



// Management
///////////////

void deRLTaskReadSkin::Run(){
	LogRunEnter();
	if( ! pInternalTask ){
		DETHROW( deeInvalidParam );
	}
	
	deSkin * const skin = pInternalTask->GetSkin();
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	skin->SetAsynchron( true );
	
	GetEngine().GetGraphicSystem()->LoadSkin( skin );
	GetEngine().GetAudioSystem()->LoadSkin( skin );
	GetEngine().GetPhysicsSystem()->LoadSkin( skin );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadSkin::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded || ! pInternalTask ){
		SetState( esFailed );
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deSkin * const skin = pInternalTask->GetSkin();
	if( pInternalTask->GetState() != esSucceeded || ! skin ){
		SetState( esFailed );
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deSkinManager &skinManager = *GetEngine().GetSkinManager();
	deSkin * const checkSkin = skinManager.GetSkinWith( GetPath() );
	
	if( checkSkin ){
		SetResource( checkSkin );
		
	}else{
		skin->SetAsynchron( false );
		skinManager.AddLoadedSkin( skin );
		SetResource( skin );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadSkin::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Skin-Read";
}



// Private Functions
//////////////////////

void deRLTaskReadSkin::pCleanUp(){
	if( pInternalTask ){
		pInternalTask->FreeReference();
	}
}
