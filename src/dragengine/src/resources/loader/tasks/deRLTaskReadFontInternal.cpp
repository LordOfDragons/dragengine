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

#include "deRLTaskReadFont.h"
#include "deRLTaskReadFontInternal.h"
#include "deRLTaskReadFontInternal2.h"
#include "../deResourceLoader.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../parallel/deParallelProcessing.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/font/deBaseFontModule.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"


// Class deRLTaskReadFontInternal
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadFontInternal::deRLTaskReadFontInternal( deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path,
deRLTaskReadFont *parentTask ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertFont ),
pSucceeded( false ),
pAlreadyLoaded( false ),
pParentTask( parentTask ),
pInternalTask( NULL )
{
	if( ! parentTask ){
		DETHROW( deeInvalidParam );
	}
	
	LogCreateEnter();
	pFont.TakeOver( new deFont( engine.GetFontManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadFontInternal::~deRLTaskReadFontInternal(){
	if( pInternalTask ){
		pInternalTask->FreeReference();
	}
}



// Management
///////////////

void deRLTaskReadFontInternal::Run(){
	LogRunEnter();
	
	deBaseFontModule * const module = ( deBaseFontModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtFont, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	pFont->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pFont->SetAsynchron( true );
	module->LoadFont( reader, pFont );
	reader = NULL;
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadFontInternal::Finished(){
	LogFinishedEnter();
	
	if( ! pSucceeded ){
		SetState( esFailed );
		pFont = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deEngine &engine = GetEngine();
	deFontManager &fontManager = *engine.GetFontManager();
	deFont * const checkFont = fontManager.GetFontWith( GetPath() );
	
	if( checkFont ){
		pAlreadyLoaded = true;
		SetResource( checkFont );
		SetState( esSucceeded );
		
	}else{
		pFont->SetAsynchron( false );
		SetResource( pFont );
		SetState( esSucceeded );
		
		try{
			pInternalTask = new deRLTaskReadFontInternal2( engine,
				GetResourceLoader(), GetVFS(), GetPath(), pFont );
			
			switch( pInternalTask->GetState() ){
			case esPending:
				pParentTask->AddDependsOn( pInternalTask );
				engine.GetParallelProcessing().AddTask( pInternalTask );
				break;
				
			case esSucceeded:
				break;
				
			case esFailed:
				SetState( esFailed );
				break;
			}
			
		}catch( const deException & ){
			SetState( esFailed );
		}
	}
	
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadFontInternal::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-FontIntern";
}
