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

#include "deoglSkinState.h"
#include "deoglSkinStateRenderable.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../dynamic/deoglRDynamicSkin.h"
#include "../dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/plan/deoglRenderPlanMasked.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinStateRenderable
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateRenderable::deoglSkinStateRenderable( deoglSkinState &skinState, int index ) :
pSkinState( skinState ),
pIndex( index ),
pHostRenderable( -1 ),
pPlan( NULL ),
pTexture( NULL ){
}

deoglSkinStateRenderable::~deoglSkinStateRenderable(){
	Clear();
	
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglSkinStateRenderable::SetHostRenderable( int hostRenderable ){
	pHostRenderable = hostRenderable;
}

void deoglSkinStateRenderable::SetPlan( deoglRenderPlan *plan ){
	if( plan == pPlan ){
		return;
	}
	
	if( pPlan ){
		pPlan->SetWorld( NULL );
		delete pPlan;
		pPlan = NULL;
	}
	pPlan = plan;
}

void deoglSkinStateRenderable::SetTexture( deoglTexture* texture ) {
	if( texture == pTexture ){
		return;
	}
	
	if( pTexture ){
		delete pTexture;
	}
	pTexture = texture;
}

void deoglSkinStateRenderable::Clear(){
	SetPlan( NULL );
}



void deoglSkinStateRenderable::AddRenderPlans( deoglRenderPlan &plan ){
	if( pHostRenderable == -1 ){
		return;
	}
	
	deoglRDynamicSkin * const dynamicSkin = pSkinState.GetOwnerDynamicSkin();
	deoglRSkin * const skin = pSkinState.GetOwnerSkin();
	if( ! skin || ! dynamicSkin ){
		return;
	}
	
	deoglRenderPlan * const renderablePlan = dynamicSkin->GetRenderableAt( pHostRenderable )->GetRenderPlan();
	if( ! renderablePlan ){
		return;
	}
	
	// this looks strange but we have to add a render plan for each individual texture
	// using it. this is required for the time being to mask out those textures.
	// TODO allow rendering to block out multiple textures not only a single one
	// TODO we need to check each channel to see if a texture is affected. this is a lot
	//      of testing that should be done better
	const int textureCount = skin->GetTextureCount();
	int t, c;
	
	for( t=0; t<textureCount; t++ ){
		const deoglSkinTexture &texture = skin->GetTextureAt( t );
		
		for( c=0; c<deoglSkinChannel::CHANNEL_COUNT; c++ ){
			deoglSkinChannel * const channel = texture.GetChannelAt( ( deoglSkinChannel::eChannelTypes )c );
			if( channel && channel->GetRenderable() == pIndex ){
				break;
			}
		}
		
		if( c < deoglSkinChannel::CHANNEL_COUNT ){
			deoglRenderPlanMasked * const maskedPlan = plan.AddMaskedPlanFor( renderablePlan );
			
			if( pSkinState.GetOwnerComponent() ){
				maskedPlan->SetComponent( pSkinState.GetOwnerComponent(), pSkinState.GetOwnerComponentTexture() );
			}
		}
	}
}
