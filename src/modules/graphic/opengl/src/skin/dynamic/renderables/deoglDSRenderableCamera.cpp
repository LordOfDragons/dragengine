/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglDSRenderableCamera.h"
#include "render/deoglRDSRenderableCamera.h"
#include "../deoglDynamicSkin.h"
#include "../../../world/deoglCamera.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCamera.h>
#include <dragengine/resources/camera/deCamera.h>



// Class deoglDSRenderableCamera
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableCamera::deoglDSRenderableCamera(
deoglDynamicSkin &dynamicSkin, const deDSRenderableCamera &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableCamera( renderable ),
pRRenderableCamera( NULL ),
pCamera( NULL ),
pDirty( true )
{
	try{
		pRRenderableCamera = new deoglRDSRenderableCamera( *dynamicSkin.GetRDynamicSkin() );
		
		if( renderable.GetCamera() ){
			pCamera = ( deoglCamera* )renderable.GetCamera()->GetPeerGraphic();
			pCamera->GetNotifyRenderables().Add( this );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableCamera::~deoglDSRenderableCamera(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableCamera::GetRRenderable() const{
	return pRRenderableCamera;
}

void deoglDSRenderableCamera::RenderableChanged(){
	deoglCamera * const camera = pRenderableCamera.GetCamera()
		? ( deoglCamera* )pRenderableCamera.GetCamera()->GetPeerGraphic() : NULL;
	
	if( camera != pCamera ){
		if( pCamera ){
			pCamera->GetNotifyRenderables().Remove( this );
		}
		
		pCamera = camera;
		
		if( camera ){
			camera->GetNotifyRenderables().Add( this );
		}
		
		pDirty = true;
		
		pDynamicSkin.NotifyRenderableChanged( *this );
	}
	
	if( pRenderableCamera.GetName() != pRRenderableCamera->GetName() ){
		pDynamicSkin.NotifyRenderablesChanged();
	}
}

void deoglDSRenderableCamera::SyncToRender(){
	if( pCamera ){
		pCamera->SyncToRender();
	}
	
	if( pDirty ){
		pRRenderableCamera->SetName( pRenderableCamera.GetName() );
		
		if( pCamera ){
			pRRenderableCamera->SetCamera( pCamera->GetRCamera() );
			
		}else{
			pRRenderableCamera->SetCamera( NULL );
		}
		
		pDirty = false;
	}
}

void deoglDSRenderableCamera::CameraRequiresSync(){
	pDynamicSkin.NotifyRenderableRequiresSync( *this );
}

void deoglDSRenderableCamera::DropCamera(){
	pCamera = NULL;
}



// Private Functions
//////////////////////

void deoglDSRenderableCamera::pCleanUp(){
	if( pRRenderableCamera ){
		pRRenderableCamera->FreeReference();
	}
	
	if( pCamera ){
		pCamera->GetNotifyRenderables().Remove( this );
	}
}
