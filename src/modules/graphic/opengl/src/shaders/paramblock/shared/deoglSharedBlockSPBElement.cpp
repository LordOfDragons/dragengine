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

#include "deoglSharedBlockSPB.h"
#include "deoglSharedBlockSPBElement.h"
#include "../deoglShaderParameterBlock.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedBlockSPBElement::Ref
//////////////////////////////////////////

deoglSharedBlockSPBElement::Ref::Ref() :
pElement(nullptr){
}

deoglSharedBlockSPBElement::Ref::Ref(const Ref &){
	DETHROW(deeInvalidAction);
}

deoglSharedBlockSPBElement::Ref::~Ref(){
	if(pElement){
		pElement->Return();
	}
}

deoglSharedBlockSPBElement::Ref::operator deoglSharedBlockSPBElement *() const{
	return pElement;
}

deoglSharedBlockSPBElement::Ref::operator deoglSharedBlockSPBElement &() const{
	DEASSERT_NOTNULL(pElement);
	return *pElement;
}

deoglSharedBlockSPBElement *deoglSharedBlockSPBElement::Ref::operator->() const{
	DEASSERT_NOTNULL(pElement);
	return pElement;
}

deoglSharedBlockSPBElement::Ref &deoglSharedBlockSPBElement::Ref::operator=(deoglSharedBlockSPBElement *element){
	if(element == pElement){
		return *this;
	}
	
	if(pElement){
		pElement->Return();
	}
	
	pElement = element;
	return *this;
}

bool deoglSharedBlockSPBElement::Ref::operator!() const{
	return pElement == nullptr;
}

deoglSharedBlockSPBElement::Ref::operator bool() const{
	return pElement != nullptr;
}



// Class deoglSharedBlockSPBElement
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedBlockSPBElement::deoglSharedBlockSPBElement(deoglSharedBlockSPB &spb, int index, int count) :
pSPB(spb),
pIndex(index),
pCount(count),
pEmpty(true)
{
	DEASSERT_TRUE(count > 0)
}

deoglSharedBlockSPBElement::~deoglSharedBlockSPBElement(){
}



// Management
///////////////

void deoglSharedBlockSPBElement::SetCount(int count){
	DEASSERT_TRUE(count > 0)
	pCount = count;
}

void deoglSharedBlockSPBElement::SetEmpty(bool empty){
	pEmpty = empty;
}

deoglShaderParameterBlock &deoglSharedBlockSPBElement::MapBuffer() const{
	pSPB.GetParameterBlock()->MapBuffer(pIndex, pCount);
	return *pSPB.GetParameterBlock();
}

void deoglSharedBlockSPBElement::Return() {
	pSPB.ReturnElement(this);
}
