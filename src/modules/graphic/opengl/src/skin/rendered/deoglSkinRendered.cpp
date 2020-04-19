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

#include "deoglSkinRendered.h"
#include "deoglSkinRenderedTexture.h"
#include "../deoglRSkin.h"
#include "../deoglSkinRenderable.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../decal/deoglRDecal.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/plan/deoglRenderPlanMasked.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoglSkinRendered
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinRendered::deoglSkinRendered( deoglRenderThread &renderThread, deoglRComponent &component ) :
pRenderThread( renderThread ),
pOwnerComponent( &component ),
pOwnerBillboard( NULL ),
pOwnerDecal( NULL ),
pDirty( true ){
}

deoglSkinRendered::deoglSkinRendered( deoglRenderThread &renderThread, deoglRBillboard &billboard ) :
pRenderThread( renderThread ),
pOwnerComponent( NULL ),
pOwnerBillboard( &billboard ),
pOwnerDecal( NULL ),
pDirty( true ){
}

deoglSkinRendered::deoglSkinRendered( deoglRenderThread &renderThread, deoglRDecal &decal ) :
pRenderThread( renderThread ),
pOwnerComponent( NULL ),
pOwnerBillboard( NULL ),
pOwnerDecal( &decal ),
pDirty( true ){
}

deoglSkinRendered::~deoglSkinRendered(){
	RemoveAllTextures();
}



// Management
///////////////

deoglRSkin *deoglSkinRendered::GetOwnerSkin() const{
	if( pOwnerComponent ){
		return pOwnerComponent->GetSkin();
		
	}else if( pOwnerBillboard ){
		return pOwnerBillboard->GetSkin();
		
	}else if( pOwnerDecal ){
		return pOwnerDecal->GetSkin();
		
	}else{
		return NULL;
	}
}



int deoglSkinRendered::GetTexturedCount() const{
	return pTextures.GetCount();
}

deoglSkinRenderedTexture *deoglSkinRendered::GetTextureAt( int index ) const{
	return ( deoglSkinRenderedTexture* )pTextures.GetAt( index );
}

deoglSkinRenderedTexture *deoglSkinRendered::AddTexture( deoglRSkin &skin, int texture, int modelTexture ){
	deoglSkinRenderedTexture * const srtexture = new deoglSkinRenderedTexture( *this, skin, texture, modelTexture );
	pTextures.Add( srtexture );
	return srtexture;
}

void deoglSkinRendered::RemoveAllTextures(){
	const int count = pTextures.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglSkinRenderedTexture* )pTextures.GetAt( i );
	}
	pTextures.RemoveAll();
}



void deoglSkinRendered::SetDirty(){
	RemoveAllTextures();
	pDirty = true;
}

void deoglSkinRendered::Prepare(){
	if( ! pDirty ){
		return;
	}
	pDirty = false;
	
	// non-components are handled in a simple way
	if( ! pOwnerComponent ){
		deoglRSkin * const skin = GetOwnerSkin();
		if( ! skin || skin->GetTextureCount() == 0 ){
			return;
		}
		
		pAddTextureIfRequired( *skin, 0, 0 );
		return;
	}
	
	// for components we need to examine the model textures individually
	const int count = pOwnerComponent->GetTextureCount();
	int i;
	for( i=0; i<count; i++ ){
		const deoglRComponentTexture &rctexture = pOwnerComponent->GetTextureAt( i );
		deoglRSkin * const skin = rctexture.GetUseSkin();
		const int skinTexture = rctexture.GetUseTextureNumber();
		if( ! skin || skinTexture == -1 ){
			continue;
		}
		
		pAddTextureIfRequired( *skin, skinTexture, i );
	}
}

void deoglSkinRendered::AddRenderPlans( deoglRenderPlan &plan ){
	// TODO we have a re-entrant problem here. by doing nothing this situation will happen:
	//      - main render plan calls AddRenderPlans
	//      - texture adds a masked plan (lets say this is plan=A)
	//      - texture calls Prepare on plan=A to prepare it
	//      - prepare call results in AddRenderPlans to be called again
	//      - texture adds a masked plan (with existing plan=A)
	//      - hell breaksloose because plan=A is now added as mask to itself
	//      
	//      by using the plan level we can avoid this re-entrance but mirroring more than
	//      once is not possible. multi-depth mirroring is tough problematic by itself.
	if( plan.GetLevel() > 0 ){
		return;
	}
	
	Prepare();
	
	const int count = pTextures.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkinRenderedTexture* )pTextures.GetAt( i ) )->AddRenderPlans( plan );
	}
}



void deoglSkinRendered::DropDelayedDeletionObjects(){
	const int count = pTextures.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkinRenderedTexture* )pTextures.GetAt( i ) )->DropDelayedDeletionObjects();
	}
}



// Private Functions
//////////////////////

void deoglSkinRendered::pAddTextureIfRequired( deoglRSkin &skin, int texture, int modelTexture ){
	const deoglSkinTexture &skinTexture = skin.GetTextureAt( texture );
	if( skinTexture.GetMirror() ){
		AddTexture( skin, texture, modelTexture );
	}
}
