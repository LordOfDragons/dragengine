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

#include "igdeGDCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Class igdeGDCEnvMapProbe
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCEnvMapProbe::igdeGDCEnvMapProbe(){
	pScaling.Set(1.0f, 1.0f, 1.0f);
	pShapeReflection = NULL;
	pInfluenceBorderSize = 0.1f;
	pInfluencePriority = 0;
}

igdeGDCEnvMapProbe::igdeGDCEnvMapProbe(const igdeGDCEnvMapProbe &probe){
	int i;
	
	pShapeReflection = NULL;
	
	try{
		pPosition = probe.GetPosition();
		pOrientation = probe.GetOrientation();
		pScaling = probe.GetScaling();
		
		pShapeListInfluence = probe.GetShapeListInfluence();
		if(probe.GetShapeReflection()){
			pShapeReflection = probe.GetShapeReflection()->Copy();
		}
		pShapeListReflectionMask = probe.GetShapeListReflectionMask();
		pInfluenceBorderSize = probe.pInfluenceBorderSize;
		pInfluencePriority = probe.pInfluencePriority;
		
		for(i=0; i<=epAttachRotation; i++){
			pPropertyNames[i] = probe.pPropertyNames[i];
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeGDCEnvMapProbe::~igdeGDCEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void igdeGDCEnvMapProbe::SetPosition(const decVector &position){
	pPosition = position;
}

void igdeGDCEnvMapProbe::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void igdeGDCEnvMapProbe::SetScaling(const decVector &scaling){
	pScaling = scaling;
}



void igdeGDCEnvMapProbe::SetShapeListInfluence(const decShapeList &shapeList){
	pShapeListInfluence = shapeList;
}

void igdeGDCEnvMapProbe::SetShapeReflection(decShape *shape){
	if(shape != pShapeReflection){
		if(pShapeReflection){
			delete pShapeReflection;
		}
		
		pShapeReflection = shape;
	}
}

void igdeGDCEnvMapProbe::SetShapeListReflectionMask(const decShapeList &shapeList){
	pShapeListReflectionMask = shapeList;
}

void igdeGDCEnvMapProbe::SetInfluenceBorderSize(float borderSize){
	if(borderSize < 0.0f){
		borderSize = 0.0f;
	}
	
	pInfluenceBorderSize = borderSize;
}

void igdeGDCEnvMapProbe::SetInfluencePriority(int priority){
	if(priority < 0){
		priority = 0;
	}
	
	pInfluencePriority = priority;
}



bool igdeGDCEnvMapProbe::IsPropertySet(int property) const{
	return !pPropertyNames[property].IsEmpty();
}

const decString& igdeGDCEnvMapProbe::GetPropertyName(int property) const{
	return pPropertyNames[property];
}

void igdeGDCEnvMapProbe::SetPropertyName(int property, const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	pPropertyNames[property] = name;
}

bool igdeGDCEnvMapProbe::HasPropertyWithName(const char *name) const{
	int j;
	
	for(j=0; j<=epAttachRotation; j++){
		if(pPropertyNames[j] == name){
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void igdeGDCEnvMapProbe::pCleanUp(){
	if(pShapeReflection){
		delete pShapeReflection;
	}
}
