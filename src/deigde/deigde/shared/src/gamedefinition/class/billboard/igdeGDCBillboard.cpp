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

#include "igdeGDCBillboard.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>



// Class igdeGDCBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCBillboard::igdeGDCBillboard() :
pAxis(0.0f, 1.0f, 0.0f),
pSize(1.0f, 1.0f),
pLocked(true),
pSpherical(true),
pSizeFixedToScreen(false),

pDoNotScale(false),
pPartialHide(false),
pRenderEnvMap(true){
}

igdeGDCBillboard::igdeGDCBillboard(const igdeGDCBillboard &billboard) :
pSkinPath(billboard.pSkinPath),
pAxis(billboard.pAxis),
pSize(billboard.pSize),
pOffset(billboard.pOffset),
pLocked(billboard.pLocked),
pSpherical(billboard.pSpherical),
pSizeFixedToScreen(billboard.pSizeFixedToScreen),

pDoNotScale(billboard.pDoNotScale),
pPartialHide(billboard.pPartialHide),
pRenderEnvMap(billboard.pRenderEnvMap),
pPosition(billboard.pPosition),
pBoneName(billboard.pBoneName)
{
	int i;
	for(i=0; i<=epAttachPosition; i++){
		pPropertyNames[i] = billboard.pPropertyNames[i];
	}
}

igdeGDCBillboard::~igdeGDCBillboard(){
}



// Management
///////////////

void igdeGDCBillboard::SetSkinPath(const char *path){
	pSkinPath = path;
}

void igdeGDCBillboard::SetAxis(const decVector &axis){
	pAxis = axis;
}

void igdeGDCBillboard::SetSize(const decVector2 &size){
	pSize = decVector2(FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON).Largest(size);
}

void igdeGDCBillboard::SetOffset(const decVector2 &offset){
	pOffset = offset;
}

void igdeGDCBillboard::SetLocked(bool locked){
	pLocked = locked;
}

void igdeGDCBillboard::SetSpherical(bool spherical){
	pSpherical = spherical;
}

void igdeGDCBillboard::SetSizeFixedToScreen(bool sizeFixedToScreen){
	pSizeFixedToScreen = sizeFixedToScreen;
}



void igdeGDCBillboard::SetDoNotScale(bool doNotScale){
	pDoNotScale = doNotScale;
}

void igdeGDCBillboard::SetPartialHide(bool partiallyHidden){
	pPartialHide = partiallyHidden;
}

void igdeGDCBillboard::SetRenderEnvMap(bool renderEnvMap){
	pRenderEnvMap = renderEnvMap;
}

void igdeGDCBillboard::SetPosition(const decDVector &position){
	pPosition = position;
}

void igdeGDCBillboard::SetBoneName(const char *boneName){
	pBoneName = boneName;
}



bool igdeGDCBillboard::IsPropertySet(eProperties property) const{
	return ! pPropertyNames[property].IsEmpty();
}

const decString &igdeGDCBillboard::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void igdeGDCBillboard::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool igdeGDCBillboard::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<=epAttachPosition; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}
