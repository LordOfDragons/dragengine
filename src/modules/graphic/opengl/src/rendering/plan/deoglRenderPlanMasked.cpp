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
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanMasked.h"
#include "../../component/deoglRComponent.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderPlanMasked
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanMasked::deoglRenderPlanMasked() :
pPlan( NULL ),
pComponent( NULL ),
pComponentTexture( 0 ),
pUseClipPlane( false ),
pClipNormal( 0.0f, 1.0f, 0.0f ),
pClipDistance( 0.0f ),
pClipNormalStereo( 0.0f, 1.0f, 0.0f ),
pClipDistanceStereo( 0.0f ),
pStencilMask( 0 ),
pParentStencilMask( 0 ){
}

deoglRenderPlanMasked::~deoglRenderPlanMasked(){
}



// Management
///////////////

void deoglRenderPlanMasked::SetPlan( deoglRenderPlan *plan ){
	if( plan == pPlan ){
		return;
	}
	
	if( pPlan ){
		// important or strange segfaults can happen. for example lights are stored as
		// pointers without refcounting. if this clean up is missing the light can be
		// destroyed while the plan is not used. then the next time the plan is used
		// during prepare phase it tries to do a clean up to be safe and acesses the
		// now destroyed light
		pPlan->CleanUp();
	}
	
	pPlan = plan;
	
	pComponent = NULL;
	pComponentTexture = 0;
	
	pUseClipPlane = false;
	pClipNormal.Set( 0.0f, 1.0f, 0.0f );
	pClipDistance = 0.0f;
	pClipNormalStereo.Set( 0.0f, 1.0f, 0.0f );
	pClipDistanceStereo = 0.0f;
	
	pStencilMask = 0;
	pParentStencilMask = 0;
}

void deoglRenderPlanMasked::SetComponent( deoglRComponent *component, int texture ){
	pComponent = component;
	pComponentTexture = texture;
}

void deoglRenderPlanMasked::SetUseClipPlane( bool useClipPlane ){
	pUseClipPlane = useClipPlane;
}

void deoglRenderPlanMasked::SetClipNormal( const decVector &normal ){
	pClipNormal = normal;
}

void deoglRenderPlanMasked::SetClipDistance( float distance ){
	pClipDistance = distance;
}

void deoglRenderPlanMasked::SetClipNormalStereo( const decVector &normal ){
	pClipNormalStereo = normal;
}

void deoglRenderPlanMasked::SetClipDistanceStereo( float distance ){
	pClipDistanceStereo = distance;
}

void deoglRenderPlanMasked::SetStencilMask( int mask ){
	pStencilMask = mask;
}

void deoglRenderPlanMasked::SetParentStencilMask( int mask ){
	pParentStencilMask = mask;
}
