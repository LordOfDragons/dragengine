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
pCount(0)
{
	const int size = parameterBlock->GetElementCount();
	DEASSERT_TRUE(size >= 1)
	
	pElements = decTList<deoglSharedSPBElement*>(size, nullptr);
}



// Management
///////////////

deoglSharedSPBElement *deoglSharedSPB::GetElementAt(int index) const{
	return pElements.GetAt(index);
}

deoglSharedSPBElement::Ref deoglSharedSPB::AddElement(){
	const int size = pElements.GetCount();
	if(pCount == size){
		return {};
	}
	
	int i;
	for(i=0; i<size; i++){
		if(pElements.GetAt(i)){
			continue;
		}
		
		const auto element = deoglSharedSPBElement::Ref::New(*this, i);
		pElements.SetAt(i, element);
		pCount++;
		return element;
	}
	
	DETHROW(deeInvalidParam);
}

void deoglSharedSPB::RemoveElement(int index){
	DEASSERT_NOTNULL(pElements.GetAt(index))
	
	pElements.SetAt(index, nullptr);
	pCount--;
}
