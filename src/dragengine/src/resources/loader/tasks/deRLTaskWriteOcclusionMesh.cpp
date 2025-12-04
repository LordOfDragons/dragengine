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

#include "deRLTaskWriteOcclusionMesh.h"
#include "../deResourceLoader.h"
#include "../../occlusionmesh/deOcclusionMesh.h"
#include "../../occlusionmesh/deOcclusionMeshManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/file/decBaseFileWriter::Ref.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/occlusionmesh/deBaseOcclusionMeshModule.h"



// Class deRLTaskWriteOcclusionMesh
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskWriteOcclusionMesh::deRLTaskWriteOcclusionMesh( deEngine &engine,
deResourceLoader &resourceLoader, deOcclusionMesh *occlusionMesh,
deVirtualFileSystem *vfs, const char *path ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertOcclusionMesh ),
pOcclusionMesh( occlusionMesh ),
pSucceeded( false )
{
	if( ! occlusionMesh ){
		DETHROW( deeInvalidParam );
	}
	SetType( etWrite );
}

deRLTaskWriteOcclusionMesh::~deRLTaskWriteOcclusionMesh(){
}



// Management
///////////////

void deRLTaskWriteOcclusionMesh::Run(){
	LogRunEnter();
	deBaseOcclusionMeshModule * const module = ( deBaseOcclusionMeshModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtOcclusionMesh, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	decPath path;
	path.SetFromUnix( GetPath() );
	
	decBaseFileWriter::Ref writer;
	writer.TakeOver( GetVFS()->OpenFileForWriting( path ) );
	module->SaveOcclusionMesh( writer, pOcclusionMesh );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskWriteOcclusionMesh::Finished(){
	LogFinishedEnter();
	if( pSucceeded ){
		SetResource( pOcclusionMesh );
		SetState( esSucceeded );
		
	}else{
		pOcclusionMesh = NULL;
		SetState( esFailed );
	}
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskWriteOcclusionMesh::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Anim-Write";
}
