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

#include "deoglSharedSPB.h"
#include "deoglSharedSPBElement.h"
#include "../deoglShaderParameterBlock.h"
#include "../../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedSPB
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPB::deoglSharedSPB(const deoglShaderParameterBlock::Ref &parameterBlock) :
pParameterBlock(parameterBlock),
pElements(nullptr),
pSize(parameterBlock->GetElementCount()),
pCount(0)
{
	DEASSERT_TRUE(parameterBlock->GetElementCount() >= 1)
	
	pElements = new deoglSharedSPBElement*[pSize];
	memset(pElements, '\0', sizeof(deoglSharedSPB*) * pSize);
}

deoglSharedSPB::~deoglSharedSPB(){
	if(pElements){
		delete [] pElements;
	}
}



// Management
///////////////

deoglSharedSPBElement *deoglSharedSPB::GetElementAt(int index) const{
	return pElements[index];
}

deoglSharedSPBElement::Ref deoglSharedSPB::AddElement(){
	if(pCount == pSize){
		return {};
	}
	
	int i;
	for(i=0; i<pSize; i++){
		if(pElements[i]){
			continue;
		}
		
		const deoglSharedSPBElement::Ref element(deoglSharedSPBElement::Ref::NewWith(*this, i));
		pElements[i] = element;
		pCount++;
		return element;
	}
	
	DETHROW(deeInvalidParam);
}

void deoglSharedSPB::RemoveElement(int index){
	if(index < 0 || index >= pSize || !pElements[index]){
		DETHROW(deeInvalidParam);
	}
	
	pElements[index] = nullptr;
	pCount--;
}
