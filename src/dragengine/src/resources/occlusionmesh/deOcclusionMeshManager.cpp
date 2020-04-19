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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshManager.h"
#include "deOcclusionMeshBuilder.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decPath.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/occlusionmesh/deBaseOcclusionMeshModule.h"
#include "../../systems/modules/graphic/deBaseGraphicOcclusionMesh.h"



// Class deOcclusionMeshManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMeshManager::deOcclusionMeshManager( deEngine *engine ) :
deFileResourceManager( engine, ertOcclusionMesh ){
	SetLoggingName( "occlusion mesh" );
}

deOcclusionMeshManager::~deOcclusionMeshManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deOcclusionMeshManager::GetOcclusionMeshCount() const{
	return pMeshes.GetCount();
}

deOcclusionMesh *deOcclusionMeshManager::GetRootOcclusionMesh() const{
	return ( deOcclusionMesh* )pMeshes.GetRoot();
}

deOcclusionMesh *deOcclusionMeshManager::GetOcclusionMeshWith( const char *filename ) const{
	return GetOcclusionMeshWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deOcclusionMesh *deOcclusionMeshManager::GetOcclusionMeshWith(
deVirtualFileSystem *vfs, const char *filename ) const{
	deOcclusionMesh * const occmesh = ( deOcclusionMesh* )pMeshes.GetWithFilename( vfs, filename );
	return occmesh && ! occmesh->GetOutdated() ? occmesh : NULL;
}

deOcclusionMesh *deOcclusionMeshManager::CreateOcclusionMesh(
const char *filename, deOcclusionMeshBuilder &builder ){
	return CreateOcclusionMesh( GetEngine()->GetVirtualFileSystem(), filename, builder );
}

deOcclusionMesh *deOcclusionMeshManager::CreateOcclusionMesh( deVirtualFileSystem *vfs,
const char *filename, deOcclusionMeshBuilder &builder ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	deOcclusionMesh *occmesh = NULL;
	deOcclusionMesh *findOccMesh;
	
	try{
		// check if an occlusion mesh with this filename already exists. this check is only done if
		// the filename is not empty in which case an unnamed occlusion mesh is created
		if( filename[ 0 ] != '\0' ){
			findOccMesh = ( deOcclusionMesh* )pMeshes.GetWithFilename( vfs, filename );
			if( findOccMesh && ! findOccMesh->GetOutdated() ){
				DETHROW( deeInvalidParam );
			}
		}
		
		// create occlusion mesh using the builder
		occmesh = new deOcclusionMesh( this, vfs, filename, decDateTime::GetSystemTime() );
		builder.BuildOcclusionMesh( occmesh );
		
		// prepare and check occlusion mesh
		if( ! occmesh->Verify() ){
			DETHROW( deeInvalidParam );
		}
		occmesh->Prepare();
		
		// load system peers
		GetGraphicSystem()->LoadOcclusionMesh( occmesh );
		
		// add occlusion mesh
		pMeshes.Add( occmesh );
		
	}catch( const deException &e ){
		if( occmesh ){
			occmesh->FreeReference();
		}
		LogErrorFormat( "Creating occlusion mesh '%s' failed", filename );
		LogException( e );
		throw;
	}
	
	return occmesh;
}

deOcclusionMesh *deOcclusionMeshManager::LoadOcclusionMesh( const char *filename, const char *basePath ){
	return LoadOcclusionMesh( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deOcclusionMesh *deOcclusionMeshManager::LoadOcclusionMesh( deVirtualFileSystem *vfs,
const char *filename, const char *basePath ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	decBaseFileReader *fileReader = NULL;
	deOcclusionMesh *occmesh = NULL;
	deOcclusionMesh *findOccMesh;
	deBaseOcclusionMeshModule *module;
	decPath path;
	
	try{
		// locate file
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		// check if the model with this filename already exists
		findOccMesh = ( deOcclusionMesh* )pMeshes.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findOccMesh && findOccMesh->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Occlusion Mesh '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findOccMesh->MarkOutdated();
			findOccMesh = NULL;
		}
		
		if( findOccMesh ){
			findOccMesh->AddReference();
			occmesh = findOccMesh;
			
		}else{
			// find the module able to handle this model file
			module = ( deBaseOcclusionMeshModule* )GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtOcclusionMesh, path.GetPathUnix() );
			
			// load the file with it
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix() );
			occmesh = new deOcclusionMesh( this, vfs, path.GetPathUnix(), modificationTime );
			if( ! occmesh ){
				DETHROW( deeOutOfMemory );
			}
			
			occmesh->SetAsynchron( false );
			module->LoadOcclusionMesh( *fileReader, *occmesh );
			
			fileReader->FreeReference();
			fileReader = NULL;
			
			// prepare and check model
			if( ! occmesh->Verify() ){
				DETHROW( deeInvalidParam );
			}
			occmesh->Prepare();
			
			// load system peers
			GetGraphicSystem()->LoadOcclusionMesh( occmesh );
			
			// add model
			pMeshes.Add( occmesh );
		}
		
	}catch( const deException &e ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( occmesh ){
			occmesh->FreeReference();
		}
		LogErrorFormat( "Loading occlusion mesh '%s' (base path '%s') failed", filename, basePath ? basePath : "" );
		LogException( e );
		throw;
	}
	
	if( occmesh ){
		occmesh->SetAsynchron( false );
	}
	
	return occmesh;
}



void deOcclusionMeshManager::AddLoadedOcclusionMesh( deOcclusionMesh *occmesh ){
	if( ! occmesh ){
		DETHROW( deeInvalidParam );
	}
	
	pMeshes.Add( occmesh );
}



void deOcclusionMeshManager::ReleaseLeakingResources(){
	const int count = GetOcclusionMeshCount();
	
	if( count > 0 ){
		deOcclusionMesh *occlusionMesh = ( deOcclusionMesh* )pMeshes.GetRoot();
		int unnamedCount = 0;
		
		LogWarnFormat( "%i leaking occlusion meshes", count );
		
		while( occlusionMesh ){
			if( occlusionMesh->GetFilename().IsEmpty() ){
				unnamedCount++;
				
			}else{
				LogWarnFormat( "- %s", occlusionMesh->GetFilename().GetString() );
			}
			
			occlusionMesh = ( deOcclusionMesh* )occlusionMesh->GetLLManagerNext();
		}
		
		if( unnamedCount > 0 ){
			LogWarnFormat( "%i unnamed occlusion meshes", unnamedCount );
		}
		
		pMeshes.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deOcclusionMeshManager::SystemGraphicLoad(){
	deOcclusionMesh *occmesh = ( deOcclusionMesh* )pMeshes.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( occmesh ){
		if( ! occmesh->GetPeerGraphic() ){
			grasys.LoadOcclusionMesh( occmesh );
		}
		
		occmesh = ( deOcclusionMesh* )occmesh->GetLLManagerNext();
	}
}

void deOcclusionMeshManager::SystemGraphicUnload(){
	deOcclusionMesh *occmesh = ( deOcclusionMesh* )pMeshes.GetRoot();
	
	while( occmesh ){
		occmesh->SetPeerGraphic( NULL );
		occmesh = ( deOcclusionMesh* )occmesh->GetLLManagerNext();
	}
}



void deOcclusionMeshManager::RemoveResource( deResource *resource ){
	pMeshes.RemoveIfPresent( resource );
}
