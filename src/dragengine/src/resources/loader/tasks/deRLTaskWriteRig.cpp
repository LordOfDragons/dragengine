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

#include "deRLTaskWriteRig.h"
#include "../deResourceLoader.h"
#include "../../rig/deRig.h"
#include "../../rig/deRigManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/file/decBaseFileWriterReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/rig/deBaseRigModule.h"



// Class deRLTaskWriteRig
///////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskWriteRig::deRLTaskWriteRig( deEngine &engine,
deResourceLoader &resourceLoader, deRig *rig,
deVirtualFileSystem *vfs, const char *path ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertRig ),
pRig( rig ),
pSucceeded( false )
{
	if( ! rig ){
		DETHROW( deeInvalidParam );
	}
	SetType( etWrite );
}

deRLTaskWriteRig::~deRLTaskWriteRig(){
}



// Management
///////////////

void deRLTaskWriteRig::Run(){
	LogRunEnter();
	deBaseRigModule * const module = ( deBaseRigModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtRig, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	decPath path;
	path.SetFromUnix( GetPath() );
	
	decBaseFileWriterReference writer;
	writer.TakeOver( GetVFS()->OpenFileForWriting( path ) );
	module->SaveRig( writer, pRig );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskWriteRig::Finished(){
	LogFinishedEnter();
	if( pSucceeded ){
		SetResource( pRig );
		SetState( esSucceeded );
		
	}else{
		pRig = NULL;
		SetState( esFailed );
	}
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskWriteRig::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Anim-Write";
}
