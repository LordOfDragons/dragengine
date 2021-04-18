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

#include "deoglRDSRenderableCamera.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../rendering/plan/deoglRenderPlan.h"
#include "../../../../world/deoglRCamera.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableCamera
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableCamera::deoglRDSRenderableCamera( deoglRDynamicSkin &dynamicSkin ) :
deoglRDSRenderable( dynamicSkin ),
pCamera( NULL )
{
	LEAK_CHECK_CREATE( dynamicSkin.GetRenderThread(), DSRenderableCamera );
}

deoglRDSRenderableCamera::~deoglRDSRenderableCamera(){
	LEAK_CHECK_FREE( GetDynamicSkin().GetRenderThread(), DSRenderableCamera );
	pCleanUp();
}



// Management
///////////////

void deoglRDSRenderableCamera::SetCamera( deoglRCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	if( pCamera ){
		pCamera->FreeReference();
	}
	
	pCamera = camera;
	
	if( camera ){
		camera->AddReference();
	}
	
		GetDynamicSkin().TextureConfigurationChanged(); // deprecated
}

void deoglRDSRenderableCamera::PrepareForRender(){
	if( ! pCamera ){
		return;
	}
	
	pCamera->GetPlan().PrepareRender();
}

deoglRenderPlan *deoglRDSRenderableCamera::GetRenderPlan(){
	if( ! pCamera ){
		return NULL;
	}
	
	return &pCamera->GetPlan();
}



// Private Functions
//////////////////////

void deoglRDSRenderableCamera::pCleanUp(){
	SetCamera( NULL );
}
