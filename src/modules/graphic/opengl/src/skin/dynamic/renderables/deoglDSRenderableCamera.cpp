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
