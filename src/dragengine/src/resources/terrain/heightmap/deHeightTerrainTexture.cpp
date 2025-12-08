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

#include "deHeightTerrainTexture.h"
#include "../../skin/deSkin.h"
#include "../../image/deImage.h"
#include "../../../common/exceptions.h"



// Class deHeightTerrainTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrainTexture::deHeightTerrainTexture(){
	pSkin = NULL;
	
	pProjScaling.Set(1.0f, 1.0f);
	pProjRotation = 0.0f;
	
	pMaskImage = NULL;
}

deHeightTerrainTexture::~deHeightTerrainTexture(){
}



// Management
///////////////


void deHeightTerrainTexture::SetSkin(deSkin *skin){
	
	pSkin = skin;
	
	if(skin) skin->AddReference();
}



void deHeightTerrainTexture::SetProjectionOffset(const decVector2 &offset){
	pProjOffset = offset;
}

void deHeightTerrainTexture::SetProjectionScaling(const decVector2 &scaling){
	pProjScaling = scaling;
}

void deHeightTerrainTexture::SetProjectionRotation(float rotation){
	pProjRotation = rotation;
}



void deHeightTerrainTexture::SetMaskImage(deImage *image){
	
	pMaskImage = image;
	
	if(image) image->AddReference();
}
