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

#include "deoalAComponent.h"
#include "deoalAComponentTexture.h"
#include "../skin/deoalASkin.h"
#include "../skin/deoalSkinTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoalAComponentTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAComponentTexture::deoalAComponentTexture(deoalAComponent &component, int index) :
pComponent(component),
pIndex(index),
pSkin(NULL),
pUseSkin(NULL),
pUseTexture(NULL),

pAffectsSound(false),
pAbsorptionLow(0.0f),
pAbsorptionMedium(0.0f),
pAbsorptionHigh(0.0f),
pTransmissionLow(1.0f),
pTransmissionMedium(1.0f),
pTransmissionHigh(1.0f){
}

deoalAComponentTexture::~deoalAComponentTexture(){
	if(pSkin){
		pSkin->FreeReference();
	}
}



// Management
///////////////

void deoalAComponentTexture::SetSkin(deoalASkin *skin){
	if(skin == pSkin){
		return;
	}
	
	if(pSkin){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddReference();
	}
}



void deoalAComponentTexture::UpdateUseTexture(){
	pUseSkin = NULL;
	pUseTexture = NULL;
	pAffectsSound = false;
	
	if(pSkin){
		const int textureIndex = 0; // can be set by user in deComponentTexture but not used yet
		if(textureIndex >= 0 && textureIndex < pSkin->GetTextureCount()){
			pUseSkin = pSkin;
			pUseTexture = &pSkin->GetTextureAt(textureIndex);
			pAffectsSound = pUseTexture->GetAffectsSound();
		}
		return;
	}
	
	deoalASkin * const skin = pComponent.GetSkin();
	if(skin){
		const int mappedTexture = pComponent.GetTextureSkinMappings().GetAt(pIndex);
		if(mappedTexture != -1){
			pUseSkin = skin;
			pUseTexture = &skin->GetTextureAt(mappedTexture);
			pAffectsSound = pUseTexture->GetAffectsSound();
		}
	}
}

void deoalAComponentTexture::UpdateSoundParameters(){
	pAffectsSound = false;
	
	if(!pUseTexture || pUseTexture->GetNoSound()){
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
