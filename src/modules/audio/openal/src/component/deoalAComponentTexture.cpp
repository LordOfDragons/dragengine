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

#include "deoalAComponent.h"
#include "deoalAComponentTexture.h"
#include "../skin/deoalASkin.h"
#include "../skin/deoalSkinTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoalAComponentTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAComponentTexture::deoalAComponentTexture( deoalAComponent &component, int index ) :
pComponent( component ),
pIndex( index ),
pSkin( NULL ),
pUseSkin( NULL ),
pUseTexture( NULL ),

pAffectsSound( false ),
pAbsorptionLow( 0.0f ),
pAbsorptionMedium( 0.0f ),
pAbsorptionHigh( 0.0f ),
pTransmissionLow( 1.0f ),
pTransmissionMedium( 1.0f ),
pTransmissionHigh( 1.0f ){
}

deoalAComponentTexture::~deoalAComponentTexture(){
	if( pSkin ){
		pSkin->FreeReference();
	}
}



// Management
///////////////

void deoalAComponentTexture::SetSkin( deoalASkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddReference();
	}
}



void deoalAComponentTexture::UpdateUseTexture(){
	pUseSkin = NULL;
	pUseTexture = NULL;
	pAffectsSound = false;
	
	if( pSkin ){
		const int textureIndex = 0; // can be set by user in deComponentTexture but not used yet
		if( textureIndex >= 0 && textureIndex < pSkin->GetTextureCount() ){
			pUseSkin = pSkin;
			pUseTexture = &pSkin->GetTextureAt( textureIndex );
			pAffectsSound = pUseTexture->GetAffectsSound();
		}
		return;
	}
	
	deoalASkin * const skin = pComponent.GetSkin();
	if( skin ){
		const int mappedTexture = pComponent.GetTextureSkinMappings().GetAt( pIndex );
		if( mappedTexture != -1 ){
			pUseSkin = skin;
			pUseTexture = &skin->GetTextureAt( mappedTexture );
			pAffectsSound = pUseTexture->GetAffectsSound();
		}
	}
}

void deoalAComponentTexture::UpdateSoundParameters(){
	pAffectsSound = false;
	
	if( ! pUseTexture || pUseTexture->GetNoSound() ){
		return;
	}
	
	pAffectsSound = true;
	
	pAbsorptionMedium = pUseTexture->GetAbsorption();
	pAbsorptionLow = pAbsorptionMedium * pUseTexture->GetAbsorptionLower();
	pAbsorptionHigh = pAbsorptionMedium * pUseTexture->GetAbsorptionUpper();
	
	pTransmissionMedium = pUseTexture->GetTransmission();
	pTransmissionLow = pTransmissionMedium * pUseTexture->GetTransmissionLower();
	pTransmissionHigh = pTransmissionMedium * pUseTexture->GetTransmissionUpper();
}
