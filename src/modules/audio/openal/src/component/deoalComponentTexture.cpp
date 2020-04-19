/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalComponent.h"
#include "deoalComponentTexture.h"
#include "deoalAComponentTexture.h"
#include "../skin/deoalSkin.h"
#include "../skin/deoalSkinTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoalComponentTexture
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalComponentTexture::deoalComponentTexture( deoalComponent &component, int index ) :
pComponent( component ),
pIndex( index ),
pATexture( NULL ),
pSkin( NULL ),
pDirtyTexture( true )
{
	try{
		pATexture = new deoalAComponentTexture( *component.GetAComponent(), index );
		TextureChanged( component.GetComponent().GetTextureAt( index ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalComponentTexture::~deoalComponentTexture(){
	pCleanUp();
}



// Management
///////////////

void deoalComponentTexture::Synchronize(){
	if( ! pDirtyTexture ){
		return;
	}
	
	if( pSkin ){
		pATexture->SetSkin( pSkin->GetASkin() );
		
	}else{
		pATexture->SetSkin( NULL );
	}
	
	// update dynamic skin
//	if( pDynamicSkin ){
//		pATexture->SetDynamicSkin( pDynamicSkin->GetADynamicSkin() );
//		
//	}else{
//		pATexture->SetDynamicSkin( NULL );
//	}
	
	// other stuff
//	const deComponentTexture &texture = pComponent.GetComponent().GetTextureAt( pIndex );
//	pATexture->SetTransform( texture.GetTransform() );
	
	pATexture->UpdateUseTexture();
	pATexture->UpdateSoundParameters();
	
	pDirtyTexture = false;
}



void deoalComponentTexture::TextureChanged( const deComponentTexture &texture ){
	// skin
	if( texture.GetSkin() ){
		pSkin = ( deoalSkin* )texture.GetSkin()->GetPeerAudio();
		
	}else{
		pSkin = NULL;
	}
	
	// dynamic skin
// 	if( pDynamicSkin ){
// 		pDynamicSkin->GetNotifyComponentTextures().Remove( this );
// 	}
// 	
// 	if( texture.GetDynamicSkin() ){
// 		pDynamicSkin = ( deoalDynamicSkin* )texture.GetDynamicSkin()->GetPeerAudio();
// 		pDynamicSkin->GetNotifyComponentTextures().Add( this );
// 		
// 	}else{
// 		pDynamicSkin = NULL;
// 	}
	
	pDirtyTexture = true;
}

void deoalComponentTexture::MarkDirty(){
	pDirtyTexture = true;
}



// Private Functions
//////////////////////

void deoalComponentTexture::pCleanUp(){
	if( pATexture ){
		pATexture->FreeReference();
	}
}
