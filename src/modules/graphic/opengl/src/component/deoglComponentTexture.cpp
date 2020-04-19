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

pDirtyTexture( true )
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
			pRTexture->SetSkin( NULL );
		}
		
		// update dynamic skin
		if( pDynamicSkin ){
			pRTexture->SetDynamicSkin( pDynamicSkin->GetRDynamicSkin() );
			
		}else{
			pRTexture->SetDynamicSkin( NULL );
		}
		
		// other stuff
		pRTexture->SetTransform( texture.GetTransform() );
		
		pRTexture->UpdateSkinState();
		
		InitSkinState();
		
		pDirtyTexture = false;
	}
	
	pSkinStateController->SyncToRender();
}



void deoglComponentTexture::InitSkinState(){
	if( pRTexture->GetSkinState() ){
		pSkinStateController->Init( *pRTexture->GetSkinState(),
			pRTexture->GetSkin(), 0, pComponent.GetParentWorld() );
		
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

void deoglComponentTexture::PrepareSkinStateRenderables(){
}

void deoglComponentTexture::ClearSkinStateController(){
	pSkinStateController->Clear();
}



// Notifications
//////////////////

void deoglComponentTexture::TextureChanged( const deComponentTexture &texture ){
	// skin
	if( texture.GetSkin() ){
		pSkin = ( deoglSkin* )texture.GetSkin()->GetPeerGraphic();
		
	}else{
		pSkin = NULL;
	}
	
	// dynamic skin
	if( pDynamicSkin ){
		pDynamicSkin->GetNotifyComponentTextures().Remove( this );
	}
	
	if( texture.GetDynamicSkin() ){
		pDynamicSkin = ( deoglDynamicSkin* )texture.GetDynamicSkin()->GetPeerGraphic();
		pDynamicSkin->GetNotifyComponentTextures().Add( this );
		
	}else{
		pDynamicSkin = NULL;
	}
	
	pDirtyTexture = true;
}

void deoglComponentTexture::DropDynamicSkin(){
	pDynamicSkin = NULL;
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
		pDynamicSkin->GetNotifyComponentTextures().Remove( this );
	}
}
