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
