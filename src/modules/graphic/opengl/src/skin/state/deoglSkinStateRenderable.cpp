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
