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

#include "deSkinPropertyNode.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNode
/////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNode::deSkinPropertyNode() :
pSize(256, 256, 1),
pRotation(0.0f),
pShear(0.0f),

pBrightness(0.0f),
pContrast(1.0f),
pGamma(1.0f),
pColorize(1.0f, 1.0f, 1.0f),

pTransparency(1.0f),
pMask(NULL),
pCombineMode(ecmBlend)
{
	int i;
	for(i=0; i<MappedCount; i++){
		pMapped[i] = -1;
	}
}

deSkinPropertyNode::~deSkinPropertyNode(){
	if(pMask){
		delete pMask;
	}
}



// Management
///////////////

void deSkinPropertyNode::SetPosition(const decPoint3 &position){
	pPosition = position;
}

void deSkinPropertyNode::SetSize(const decPoint3 &size){
	pSize = size;
}

void deSkinPropertyNode::SetRotation(float rotation){
	pRotation = rotation;
}

void deSkinPropertyNode::SetShear(float shear){
	pShear = shear;
}



void deSkinPropertyNode::SetBrightness(float brightness){
	pBrightness = brightness;
}

void deSkinPropertyNode::SetContrast(float contrast){
	pContrast = contrast;
}

void deSkinPropertyNode::SetGamma(float gamma){
	pGamma = gamma;
}

void deSkinPropertyNode::SetColorize(const decColor &color){
	pColorize = color;
}



void deSkinPropertyNode::SetTransparency(float transparency){
	pTransparency = transparency;
}

void deSkinPropertyNode::SetMask(deSkinPropertyNode *mask){
	if(mask == pMask){
		return;
	}
	
	if(pMask){
		delete pMask;
	}
	pMask = mask;
}

void deSkinPropertyNode::SetCombineMode(eCombineModes mode){
	pCombineMode = mode;
}



int deSkinPropertyNode::GetMappedFor(eMapped mapped) const{
	return pMapped[mapped];
}

void deSkinPropertyNode::SetMappedFor(eMapped mapped, int index){
	DEASSERT_TRUE(index >= -1)
	pMapped[mapped] = index;
}



// Visiting
/////////////

void deSkinPropertyNode::Visit(deSkinPropertyNodeVisitor &visitor){
	visitor.VisitNode(*this);
}
