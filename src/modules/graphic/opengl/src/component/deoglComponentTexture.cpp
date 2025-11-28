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

#include <stdlib.h>
#include <stdio.h>

#include "deoglComponent.h"
#include "deoglComponentTexture.h"
#include "deoglRComponentTexture.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglComponentTexture
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentTexture::deoglComponentTexture( deoglComponent &component, int index ) :
pComponent( component ),
pIndex( index ),
pRTexture( NULL ),

pSkinStateController( NULL ),

pSkin( NULL ),
pDynamicSkin( NULL ),

pDirtyTexture( true ),
pDynamicSkinRenderablesChanged( true ),
pDirtyRenderableMapping( true )
{
	try{
		pRTexture = new deoglRComponentTexture( *component.GetRComponent(), index );
		pSkinStateController = new deoglSkinStateController;
		
		TextureChanged( component.GetComponent().GetTextureAt( index ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglComponentTexture::~deoglComponentTexture(){
	pCleanUp();
}



// Management
///////////////

void deoglComponentTexture::SyncToRender(){
	if( pDirtyTexture ){
		const deComponentTexture &texture = pComponent.GetComponent().GetTextureAt( pIndex );
		
		// update skin
		if( pSkin ){
			pRTexture->SetSkin( pSkin->GetRSkin() );
			
		}else{
			pRTexture->SetSkin( nullptr );
		}
		
		// update dynamic skin
		if( pDynamicSkin ){
			pRTexture->SetDynamicSkin( pDynamicSkin->GetRDynamicSkin() );
			
		}else{
			pRTexture->SetDynamicSkin( nullptr );
		}
		
		// other stuff
		pRTexture->SetTransform( texture.GetTransform() );
		
		pRTexture->UpdateSkinState( pComponent );
		
		InitSkinState();
		
		pDirtyTexture = false;
		pDirtyRenderableMapping = true; // to be on the safe side
	}
	
	if( pDirtyRenderableMapping ){
		pRTexture->UpdateRenderableMapping();
		pDirtyRenderableMapping = false;
	}
	
	pSkinStateController->SyncToRender();
}

void deoglComponentTexture::SetDynamicSkinRenderablesChanged( bool changed ){
	pDynamicSkinRenderablesChanged = changed;
	pDirtyRenderableMapping = true;
}



void deoglComponentTexture::DirtyRenderableMapping(){
	pDirtyRenderableMapping = true;
}

void deoglComponentTexture::InitSkinState(){
	if(pRTexture->GetSkinState()){
		pSkinStateController->Init(*pRTexture->GetSkinState(),
			pRTexture->GetSkin(), 0, pComponent.GetParentWorld());
		
	}else{
		pSkinStateController->Clear();
	}
}

void deoglComponentTexture::AdvanceTime( float timeStep ){
	if( ! pRTexture->GetSkinState() ){
		return;
	}
	pSkinStateController->AdvanceTime( timeStep );
}

void deoglComponentTexture::ClearSkinStateController(){
	pSkinStateController->Clear();
}



// Dynamic skin listener
//////////////////////////

void deoglComponentTexture::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglComponentTexture::DynamicSkinRenderablesChanged(){
	pDynamicSkinRenderablesChanged = true;
	pDirtyRenderableMapping = true;
	pComponent.TextureDynamicSkinRenderableChanged();
}

void deoglComponentTexture::DynamicSkinRenderableChanged( deoglDSRenderable& ){
	pDynamicSkinRenderablesChanged = true;
	pDirtyRenderableMapping = true;
	pComponent.TextureDynamicSkinRenderableChanged();
}

void deoglComponentTexture::DynamicSkinRenderableRequiresSync( deoglDSRenderable& ){
	pComponent.TextureDynamicSkinRequiresSync();
}



// Notifications
//////////////////

void deoglComponentTexture::TextureChanged( const deComponentTexture &texture ){
	// skin
	deoglSkin *skin = nullptr;
	if( texture.GetSkin() ){
		skin = ( deoglSkin* )texture.GetSkin()->GetPeerGraphic();
	}
	
	if( skin != pSkin ){
		pSkin = skin;
		pDirtyRenderableMapping = true;
		pComponent.DirtyTextureUseSkin();
	}
	
	// dynamic skin
	deoglDynamicSkin *dynamicSkin = nullptr;
	if( texture.GetDynamicSkin() ){
		dynamicSkin = ( deoglDynamicSkin* )texture.GetDynamicSkin()->GetPeerGraphic();
	}
	
	if( dynamicSkin != pDynamicSkin ){
		if( pDynamicSkin ){
			pDynamicSkin->RemoveListener( this );
		}
		
		pDynamicSkin = dynamicSkin;
		
		if( dynamicSkin ){
			dynamicSkin->AddListener( this );
		}
		
		pDirtyRenderableMapping = true;
		pComponent.DirtyTextureUseSkin();
	}
	
	pDirtyTexture = true;
}



// Private Functions
//////////////////////

void deoglComponentTexture::pCleanUp(){
	if( pSkinStateController ){
		delete pSkinStateController;
	}
	
	if( pRTexture ){
		pRTexture->FreeReference();
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->RemoveListener( this );
	}
}
