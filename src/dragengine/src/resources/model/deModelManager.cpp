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

#include "deModel.h"
#include "deModelBuilder.h"
#include "deModelManager.h"
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/model/deBaseModelModule.h"



// Class deModelManager
/////////////////////////

// Constructor, destructor
////////////////////////////

deModelManager::deModelManager( deEngine *engine ) :
deFileResourceManager( engine, ertModel )
{
	SetLoggingName( "model" );
}

deModelManager::~deModelManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deModelManager::GetModelCount() const{
	return pModels.GetCount();
}

deModel *deModelManager::GetRootModel() const{
	return ( deModel* )pModels.GetRoot();
}

deModel *deModelManager::GetModelWith( const char *filename ) const{
	return GetModelWith( GetEngine()->GetVirtualFileSystem(), filename );
}

deModel *deModelManager::GetModelWith( deVirtualFileSystem *vfs, const char *filename ) const{
	deModel * const model = ( deModel* )pModels.GetWithFilename( vfs, filename );
	return model && ! model->GetOutdated() ? model : NULL;
}

deModel *deModelManager::CreateModel( const char *filename, deModelBuilder &builder ){
	return CreateModel( GetEngine()->GetVirtualFileSystem(), filename, builder );
}

deModel *deModelManager::CreateModel( deVirtualFileSystem *vfs, const char *filename,
deModelBuilder &builder ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	deModel *model = NULL;
	deModel *findModel;
	
	try{
		// check if a model with this filename already exists. this check is only done if
		// the filename is not empty in which case an unnamed model is created
		if( filename[ 0 ] != '\0' ){
			findModel = ( deModel* )pModels.GetWithFilename( vfs, filename );
			if( findModel ){
				findModel->MarkOutdated();
				findModel = NULL;
			}
		}
		
		// create model using the builder
		model = new deModel( this, vfs, filename, decDateTime::GetSystemTime() );
		builder.BuildModel( model );
		
		// prepare and check model
		if( ! model->Verify() ){
			DETHROW( deeInvalidParam );
		}
		model->Prepare();
		
		// load system peers
		GetGraphicSystem()->LoadModel( model );
		GetPhysicsSystem()->LoadModel( model );
		GetAudioSystem()->LoadModel( model );
		
		// add model
		pModels.Add( model );
		
	}catch( const deException &e ){
		if( model ){
			model->FreeReference();
		}
		LogErrorFormat( "Creating model '%s' failed", filename );
		LogException( e );
		throw;
	}
	
	return model;
}

deModel *deModelManager::LoadModel( const char *filename, const char *basePath ){
	return LoadModel( GetEngine()->GetVirtualFileSystem(), filename, basePath );
}

deModel *deModelManager::LoadModel( deVirtualFileSystem *vfs, const char *filename, const char *basePath ){
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReader *fileReader = NULL;
	deBaseModelModule *module;
	deModel *model = NULL;
	decPath path;
	deModel *findModel;
	
	try{
		// locate file
		if( ! FindFileForReading( path, *vfs, filename, basePath ) ){
			DETHROW_INFO( deeFileNotFound, filename );
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime( path );
		
		// check if the model with this filename already exists
		findModel = ( deModel* )pModels.GetWithFilename( vfs, path.GetPathUnix() );
		
		if( findModel && findModel->GetModificationTime() != modificationTime ){
			LogInfoFormat( "Model '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "" );
			findModel->MarkOutdated();
			findModel = NULL;
		}
		
		if( findModel ){
			findModel->AddReference();
			model = findModel;
			
		}else{
			// find the module able to handle this model file
			module = ( deBaseModelModule* )GetModuleSystem()->GetModuleAbleToLoad(
				deModuleSystem::emtModel, path.GetPathUnix() );
			
			// load the file with it
			fileReader = OpenFileForReading( *vfs, path.GetPathUnix() );
			model = new deModel( this, vfs, path.GetPathUnix(), modificationTime );
			if( ! model ){
				DETHROW( deeOutOfMemory );
			}
			
			model->SetAsynchron( false );
			module->LoadModel( *fileReader, *model );
			
			fileReader->FreeReference();
			fileReader = NULL;
			
			// prepare and check model
			if( ! model->Verify() ){
				DETHROW( deeInvalidParam );
			}
			model->Prepare();
			
			// load system peers
			GetGraphicSystem()->LoadModel( model );
			GetPhysicsSystem()->LoadModel( model );
			GetAudioSystem()->LoadModel( model );
			
			// add model
			pModels.Add( model );
			//LogInfoFormat( "Loading '%s' succeeded", path->GetPathUnix() );
		}
		
	}catch( const deException &e ){
		if( fileReader ){
			fileReader->FreeReference();
		}
		if( model ){
			model->FreeReference();
		}
		LogErrorFormat( "Loading model '%s' (base path '%s') failed", filename, basePath ? basePath : "" );
		LogException( e );
		throw;
	}
	
	if( model ){
		model->SetAsynchron( false );
	}
	
	return model;
}

void deModelManager::AddLoadedModel( deModel *model ){
	if( ! model ){
		DETHROW( deeInvalidParam );
	}
	
	pModels.Add( model );
}



void deModelManager::ReleaseLeakingResources(){
	const int count = GetModelCount();
	
	if( count > 0 ){
		deModel *model = ( deModel* )pModels.GetRoot();
		int unnamedModelCount = 0;
		
		LogWarnFormat( "%i leaking models", count );
		
		while( model ){
			if( model->GetFilename().IsEmpty() ){
				unnamedModelCount++;
				
			}else{
				LogWarnFormat( "- %s", model->GetFilename().GetString() );
			}
			
			model = ( deModel* )model->GetLLManagerNext();
		}
		
		if( unnamedModelCount > 0 ){
			LogWarnFormat( "%i unnamed models", unnamedModelCount );
		}
		
		pModels.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deModelManager::SystemGraphicLoad(){
	deModel *model = ( deModel* )pModels.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while( model ){
		if( ! model->GetPeerGraphic() ){
			graSys.LoadModel( model );
		}
		
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::SystemGraphicUnload(){
	deModel *model = ( deModel* )pModels.GetRoot();
	
	while( model ){
		model->SetPeerGraphic( NULL );
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::SystemPhysicsLoad(){
	deModel *model = ( deModel* )pModels.GetRoot();
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	
	while( model ){
		if( ! model->GetPeerPhysics() ){
			phySys.LoadModel( model );
		}
		
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::SystemPhysicsUnload(){
	deModel *model = ( deModel* )pModels.GetRoot();
	
	while( model ){
		model->SetPeerPhysics( NULL );
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::SystemAudioLoad(){
	deModel *model = ( deModel* )pModels.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while( model ){
		if( ! model->GetPeerAudio() ){
			audSys.LoadModel( model );
		}
		
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::SystemAudioUnload(){
	deModel *model = ( deModel* )pModels.GetRoot();
	
	while( model ){
		model->SetPeerAudio ( NULL );
		model = ( deModel* )model->GetLLManagerNext();
	}
}

void deModelManager::RemoveResource( deResource *resource ){
	pModels.RemoveIfPresent( resource );
}
