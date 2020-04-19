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

#include "deRLTaskReadRig.h"
#include "../deResourceLoader.h"
#include "../../rig/deRig.h"
#include "../../rig/deRigManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/dePhysicsSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/rig/deBaseRigModule.h"



// Class deRLTaskReadRig
//////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadRig::deRLTaskReadRig( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deRig *rig ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertRig ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( rig ){
		pRig = rig;
		SetResource( rig );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pRig.TakeOver( new deRig( engine.GetRigManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadRig::~deRLTaskReadRig(){
}



// Management
///////////////

void deRLTaskReadRig::Run(){
	LogRunEnter();
	deBaseRigModule * const module = ( deBaseRigModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtRig, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pRig->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pRig->SetAsynchron( true );
	module->LoadRig( reader, pRig );
	reader = NULL;
	
	if( ! pRig->Verify() ){
		DETHROW( deeInvalidParam );
	}
	pRig->Prepare();
	
	GetEngine().GetPhysicsSystem()->LoadRig( pRig );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadRig::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pRig = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deRigManager &rigManager = *GetEngine().GetRigManager();
	deRig * const checkRig = rigManager.GetRigWith( GetPath() );
	
	if( checkRig ){
		SetResource( checkRig );
		
	}else{
		pRig->SetAsynchron( false );
		rigManager.AddLoadedRig( pRig );
		SetResource( pRig );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadRig::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Rig-Read";
}
