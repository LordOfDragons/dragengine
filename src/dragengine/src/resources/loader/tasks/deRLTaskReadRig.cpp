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
