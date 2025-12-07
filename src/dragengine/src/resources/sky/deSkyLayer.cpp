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

#include "deSkyLayer.h"
#include "deSkyLayerBody.h"
#include "deSkyControllerTarget.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class deSkyLayer
/////////////////////

// Constructor, destructor
////////////////////////////

deSkyLayer::deSkyLayer() :
pColor(1.0f, 1.0f, 1.0f),
pIntensity(1.0f),
pTransparency(1.0f),

pLightColor(1.0f, 1.0f, 1.0f),
pLightIntensity(0.0f),
pAmbientIntensity(0.0f),

pBodies(NULL),
pBodyCount(0),

pMulBySkyLight(false),
pMulBySkyColor(false){
}

deSkyLayer::~deSkyLayer(){
	if(pBodies){
		delete [] pBodies;
	}
}



// Management
///////////////

void deSkyLayer::SetOffset(const decVector &offset){
	pOffset = offset;
}

void deSkyLayer::SetOrientation(const decVector &orientation){
	pOrientation = orientation;
}

void deSkyLayer::SetColor(const decColor &color){
	pColor = color;
}

void deSkyLayer::SetIntensity(float intensity){
	pIntensity = decMath::max(intensity, 0.0f);
}

void deSkyLayer::SetTransparency(float transparency){
	pTransparency = decMath::clamp(transparency, 0.0f, 1.0f);
}

void deSkyLayer::SetSkin(deSkin *skin){
	pSkin = skin;
}



void deSkyLayer::SetLightOrientation(const decQuaternion &orientation){
	pLightOrientation = orientation;
}

void deSkyLayer::SetLightColor(const decColor &color){
	pLightColor = color;
}

void deSkyLayer::SetLightIntensity(float intensity){
	pLightIntensity = decMath::max(intensity, 0.0f);
}

void deSkyLayer::SetAmbientIntensity(float intensity){
	pAmbientIntensity = decMath::max(intensity, 0.0f);
}



void deSkyLayer::SetBodyCount(int count){
	if(pBodies){
		delete [] pBodies;
		pBodies = NULL;
		pBodyCount = 0;
	}
	
	if(count > 0){
		pBodies = new deSkyLayerBody[count];
		pBodyCount = count;
	}
}

deSkyLayerBody &deSkyLayer::GetBodyAt(int index) const{
	if(index < 0 || index >= pBodyCount){
		DETHROW(deeInvalidParam);
	}
	return pBodies[index];
}



const deSkyControllerTarget &deSkyLayer::GetTarget(eTargets target) const{
	if(target < etOffsetX || target > etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	return pTargets[target];
}

deSkyControllerTarget &deSkyLayer::GetTarget(eTargets target){
	if(target < etOffsetX || target > etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	return pTargets[target];
}



void deSkyLayer::SetMultiplyBySkyLight(bool multiply){
	pMulBySkyLight = multiply;
}

void deSkyLayer::SetMultiplyBySkyColor(bool multiply){
	pMulBySkyColor = multiply;
}
