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

#include "deRLTaskReadOcclusionMesh.h"
#include "../deResourceLoader.h"
#include "../../occlusionmesh/deOcclusionMesh.h"
#include "../../occlusionmesh/deOcclusionMeshManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReader::Ref.h"
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
	
	decBaseFileReader::Ref reader;
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
