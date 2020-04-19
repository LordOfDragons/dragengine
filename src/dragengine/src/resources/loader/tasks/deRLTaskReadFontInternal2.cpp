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

#include "deRLTaskReadFontInternal2.h"
#include "../deResourceLoader.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/font/deBaseFontModule.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"


// Class deRLTaskReadFontInternal2
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadFontInternal2::deRLTaskReadFontInternal2( deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs, const char *path, deFont *font ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertFont ),
pFont( font ),
pAlreadyLoaded( false ),
pTaskImage( NULL )
{
	if( ! font ){
		DETHROW( deeInvalidParam );
	}
	
	LogCreateEnter();
	
	SetEmptyRun( true );
	
	try{
		pLoadFontResources();
		
	}catch( const deException & ){
		SetState( esFailed );
		Cancel();
	}
	
	LogCreateExit();
}

deRLTaskReadFontInternal2::~deRLTaskReadFontInternal2(){
	if( pTaskImage ){
		pTaskImage->FreeReference();
	}
}



// Management
///////////////

void deRLTaskReadFontInternal2::Run(){
}

void deRLTaskReadFontInternal2::Finished(){
	LogFinishedEnter();
	
	deFontManager &fontManager = *GetEngine().GetFontManager();
	deFont * const checkFont = fontManager.GetFontWith( GetPath() );
	
	if( checkFont ){
		pAlreadyLoaded = true;
		
	}else{
		// check if loadimg image finished successfully
		if( pTaskImage ){
			deImage * const image = ( deImage* )pTaskImage->GetResource();
			if( pTaskImage->GetState() != esSucceeded || ! image ){
				SetState( esFailed );
				LogFinishedExit();
				GetResourceLoader().FinishTask( this );
				return;
			}
			
			pFont->SetImage( image );
		}
	}
	
	SetState( esSucceeded );
	
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadFontInternal2::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-FontIntern2";
}



// Private Functions
//////////////////////

void deRLTaskReadFontInternal2::pLoadFontResources(){
	decPath basePath( decPath::CreatePathUnix( pFont->GetFilename() ) );
	basePath.RemoveLastComponent();
	
	decString path;
	
	// load image if present
	if( ! pFont->GetImage() ){
		path = pFont->GetImagePath();
		if( ! path.IsEmpty() ){
			if( ! decPath::IsUnixPathAbsolute( path ) ){
				decPath resourcePath( basePath );
				resourcePath.AddUnixPath( path );
				path = resourcePath.GetPathUnix();
			}
			
			pTaskImage = GetResourceLoader().AddLoadRequest( GetVFS(),
				path, deResourceLoader::ertImage );
			pTaskImage->AddReference(); // this is required. see AddLoadRequest for details
			
			if( pTaskImage->GetState() == esPending && ! DoesDependOn( pTaskImage ) ){
				AddDependsOn( pTaskImage );
			}
		}
	}
}
