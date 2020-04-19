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

#include "deRLTaskWriteOcclusionMesh.h"
#include "../deResourceLoader.h"
#include "../../occlusionmesh/deOcclusionMesh.h"
#include "../../occlusionmesh/deOcclusionMeshManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/file/decBaseFileWriterReference.h"
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
	
	decBaseFileWriterReference writer;
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
