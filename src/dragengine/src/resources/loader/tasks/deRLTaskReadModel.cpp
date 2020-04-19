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

#include "deRLTaskReadModel.h"
#include "../deResourceLoader.h"
#include "../../model/deModel.h"
#include "../../model/deModelManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/dePhysicsSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/model/deBaseModelModule.h"



// Class deRLTaskReadModel
////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadModel::deRLTaskReadModel( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deModel *model ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertModel ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( model ){
		pModel = model;
		SetResource( model );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pModel.TakeOver( new deModel( engine.GetModelManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadModel::~deRLTaskReadModel(){
}



// Management
///////////////

void deRLTaskReadModel::Run(){
	LogRunEnter();
	deBaseModelModule * const module = ( deBaseModelModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtModel, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pModel->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pModel->SetAsynchron( true );
	module->LoadModel( reader, pModel );
	reader = NULL;
	
	if( ! pModel->Verify() ){
		DETHROW( deeInvalidParam );
	}
	pModel->Prepare();
	
	GetEngine().GetGraphicSystem()->LoadModel( pModel );
	GetEngine().GetPhysicsSystem()->LoadModel( pModel );
	GetEngine().GetAudioSystem()->LoadModel( pModel );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadModel::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pModel = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deModelManager &modelManager = *GetEngine().GetModelManager();
	deModel * const checkModel = modelManager.GetModelWith( GetPath() );
	
	if( checkModel ){
		SetResource( checkModel );
		
	}else{
		pModel->SetAsynchron( false );
		modelManager.AddLoadedModel( pModel );
		SetResource( pModel );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadModel::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Model-Read";
}
