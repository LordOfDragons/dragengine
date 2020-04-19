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

#include "deRLTaskReadOcclusionMesh.h"
#include "../deResourceLoader.h"
#include "../../occlusionmesh/deOcclusionMesh.h"
#include "../../occlusionmesh/deOcclusionMeshManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/occlusionmesh/deBaseOcclusionMeshModule.h"



// Class deRLTaskReadOcclusionMesh
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadOcclusionMesh::deRLTaskReadOcclusionMesh( deEngine &engine,
	deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path,
	deOcclusionMesh *occlusionMesh ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertOcclusionMesh ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( occlusionMesh ){
		pOcclusionMesh = occlusionMesh;
		SetResource( occlusionMesh );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pOcclusionMesh.TakeOver( new deOcclusionMesh( engine.GetOcclusionMeshManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadOcclusionMesh::~deRLTaskReadOcclusionMesh(){
}



// Management
///////////////

void deRLTaskReadOcclusionMesh::Run(){
	LogRunEnter();
	deBaseOcclusionMeshModule * const module = ( deBaseOcclusionMeshModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtOcclusionMesh, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pOcclusionMesh->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pOcclusionMesh->SetAsynchron( true );
	module->LoadOcclusionMesh( reader, pOcclusionMesh );
	reader = NULL;
	
	if( ! pOcclusionMesh->Verify() ){
		DETHROW( deeInvalidParam );
	}
	pOcclusionMesh->Prepare();
	
	GetEngine().GetGraphicSystem()->LoadOcclusionMesh( pOcclusionMesh );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadOcclusionMesh::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pOcclusionMesh = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deOcclusionMeshManager &occlusionMeshManager = *GetEngine().GetOcclusionMeshManager();
	deOcclusionMesh * const checkOcclusionMesh = occlusionMeshManager.GetOcclusionMeshWith( GetPath() );
	
	if( checkOcclusionMesh ){
		SetResource( checkOcclusionMesh );
		
	}else{
		pOcclusionMesh->SetAsynchron( false );
		occlusionMeshManager.AddLoadedOcclusionMesh( pOcclusionMesh );
		SetResource( pOcclusionMesh );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadOcclusionMesh::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-OccMesh-Read";
}
