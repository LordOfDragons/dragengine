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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffectChain.h"
#include "deEffectDistortImage.h"
#include "deEffectVisitor.h"
#include "../image/deImage.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectDistortImage
///////////////////////////////

// constructor, destructor
////////////////////////////

deEffectDistortImage::deEffectDistortImage(deEffectManager *manager) : deEffect(manager){
	pTexCoords[0].Set(0.0f, 0.0f);
	pTexCoords[1].Set(1.0f, 0.0f);
	pTexCoords[2].Set(1.0f, 1.0f);
	pTexCoords[3].Set(0.0f, 1.0f);
	pStrengthU = 0.01f;
	pStrengthV = 0.01f;
}

deEffectDistortImage::~deEffectDistortImage(){
}



// Management
///////////////

void deEffectDistortImage::SetImage(deImage *image){
	if(image == pImage){
		return;
	}
	
	pImage = image;
	
	deBaseGraphicEffect *graEff = GetPeerGraphic();
	if(graEff){
		graEff->ImageChanged();
	}
}

const decVector2 &deEffectDistortImage::GetTextureCoordinatesFor(int corner) const{
	if(corner < 0 || corner > 3) DETHROW(deeInvalidParam);
	return pTexCoords[corner];
}

void deEffectDistortImage::SetTextureCoordinatesFor(int corner, const decVector2 &textureCoordinates){
	if(corner < 0 || corner > 3) DETHROW(deeInvalidParam);
	
	if(! textureCoordinates.IsEqualTo(pTexCoords[corner])){
		pTexCoords[corner] = textureCoordinates;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if(graEffect) graEffect->TextureCoordinatesChanged();
	}
}

void deEffectDistortImage::SetStrengthU(float strength){
	if(fabs(strength - pStrengthU) > 1e-5){
		pStrengthU = strength;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if(graEffect) graEffect->StrengthChanged();
	}
}

void deEffectDistortImage::SetStrengthV(float strength){
	if(fabs(strength - pStrengthV) > 1e-5){
		pStrengthV = strength;
		
		deBaseGraphicEffect *graEffect = GetPeerGraphic();
		if(graEffect) graEffect->StrengthChanged();
	}
}



// Visiting
/////////////

void deEffectDistortImage::Visit(deEffectVisitor &visitor){
	visitor.VisitDistortImage(*this);
}
