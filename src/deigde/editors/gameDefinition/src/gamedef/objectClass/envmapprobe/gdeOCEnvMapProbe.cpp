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

#include "gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Class gdeOCEnvMapProbe
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCEnvMapProbe::gdeOCEnvMapProbe() :
pScaling(1.0f, 1.0f, 1.0f),
pShapeReflection(NULL),
pInfluenceBorderSize(0.1f),
pInfluencePriority(0){
}

gdeOCEnvMapProbe::gdeOCEnvMapProbe(const gdeOCEnvMapProbe &probe) :
pPosition(probe.GetPosition()),
pRotation(probe.GetRotation()),
pScaling(probe.GetScaling()),
pShapeListInfluence(probe.GetShapeListInfluence()),
pShapeReflection(NULL),
pShapeListReflectionMask(probe.GetShapeListReflectionMask()),
pInfluenceBorderSize(probe.pInfluenceBorderSize),
pInfluencePriority(probe.pInfluencePriority)
{
	int i;
	
	try{
		if(probe.GetShapeReflection()){
			pShapeReflection = probe.GetShapeReflection()->Copy();
		}
		
		for(i=0; i<=epAttachRotation; i++){
			pPropertyNames[i] = probe.pPropertyNames[i];
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeOCEnvMapProbe::~gdeOCEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void gdeOCEnvMapProbe::SetPosition(const decVector &position){
	pPosition = position;
}

void gdeOCEnvMapProbe::SetRotation(const decVector &orientation){
	pRotation = orientation;
}

void gdeOCEnvMapProbe::SetScaling(const decVector &scaling){
	pScaling = scaling;
}



void gdeOCEnvMapProbe::SetShapeReflection(decShape *shape){
	if(shape == pShapeReflection){
		return;
	}
	
	if(pShapeReflection){
		delete pShapeReflection;
	}
	
	pShapeReflection = shape;
}

void gdeOCEnvMapProbe::SetInfluenceBorderSize(float borderSize){
	pInfluenceBorderSize = decMath::max(borderSize, 0.0f);
}

void gdeOCEnvMapProbe::SetInfluencePriority(int priority){
	pInfluencePriority = decMath::max(priority, 0);
}



bool gdeOCEnvMapProbe::IsPropertySet(int property) const{
	if(property < 0 || property > epAttachRotation){
		DETHROW(deeInvalidParam);
	}
	return !pPropertyNames[property].IsEmpty();
}

const decString& gdeOCEnvMapProbe::GetPropertyName(int property) const{
	if(property < 0 || property > epAttachRotation){
		DETHROW(deeInvalidParam);
	}
	return pPropertyNames[property];
}

void gdeOCEnvMapProbe::SetPropertyName(int property, const char *name){
	if(property < 0 || property > epAttachRotation || !name){
		DETHROW(deeInvalidParam);
	}
	pPropertyNames[property] = name;
}

bool gdeOCEnvMapProbe::HasPropertyWithName(const char *name) const{
	int i;
	
	for(i=0; i<=epAttachRotation; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void gdeOCEnvMapProbe::pCleanUp(){
	if(pShapeReflection){
		delete pShapeReflection;
	}
}
