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
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decHelperFunctions.h>



// Class deoglSharedBlockSPB
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedBlockSPB::deoglSharedBlockSPB(deoglShaderParameterBlock *parameterBlock) :
pParameterBlock(parameterBlock),
pSize(parameterBlock->GetElementCount()),
pUsedElementCount(0),
pFreeElementCount(pSize)
{
	DEASSERT_TRUE(parameterBlock->GetElementCount() >= 1)
	
	// add empty block for the element space
	pElements.Add(deoglSharedBlockSPBElement::ObjRef::New(*this, 0, pFreeElementCount));
}

deoglSharedBlockSPB::~deoglSharedBlockSPB(){
}



// Management
///////////////

deoglSharedBlockSPBElement *deoglSharedBlockSPB::GetElement(int count){
	DEASSERT_TRUE(count > 0)
	
	pCheckSize();
	
	// find empty element with enough free space. if found the empty element is removed from
	// the list of empty elements if required
	const int index = pIndexOfEmptyElementWithMinCount(count);
	if(index == -1){
		return nullptr;
	}
	
	deoglSharedBlockSPBElement * const element =
		static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(index));
	
	// if empty block is larger than requested size add empty block with
	// remaining empty space right after this block
	if(element->GetCount() > count){
		const deoglSharedBlockSPBElement::ObjRef emptyElement(
			deoglSharedBlockSPBElement::ObjRef::New(*this,
				element->GetIndex() + count, element->GetCount() - count));
		
		if(index + 1 < pElements.GetCount()){
			// not the last element filling up to the available space
			pElements.Insert(emptyElement, index + 1);
			pEmptyElements.Add(emptyElement);
			
		}else{
			// last element filling up to the available space
			pElements.Add(emptyElement);
		}
	}
	
	// turn the block into a non-empty block with the requested size
	element->SetCount(count);
	element->SetEmpty(false);
	
	// book keeping
	pFreeElementCount -= count;
	pUsedElementCount += count;
	
	return element;
}

void deoglSharedBlockSPB::ReturnElement(deoglSharedBlockSPBElement *element){
	int index = pElements.IndexOf(element);
	DEASSERT_TRUE(index != -1)
	
	bool addToEmptyElements = index < pElements.GetCount() - 1;
	const int count = element->GetCount();
	bool merged = false;
	
	// turn the block into an empty block
	element->SetEmpty(true);
	
	// if the previous block is empty merge this block with the previous block
	if(index > 0){
		deoglSharedBlockSPBElement * const mergeElement =
			static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(index - 1));
		if(mergeElement->GetEmpty()){
			mergeElement->SetCount(mergeElement->GetCount() + count);
			
			pElements.RemoveFrom(index);
			
			addToEmptyElements = false;
			merged = true;
			element = mergeElement;
			index--;
			
			// if the previous empty block becomes the last block filling up space up the to
			// available space remove it from the empty block list
			if(index == pElements.GetCount() - 1){
				pEmptyElements.RemoveFrom(pEmptyElements.IndexOf(mergeElement));
			}
		}
	}
	
	// if the next block is empty merge the next block with this block
	if(index < pElements.GetCount() - 1){
		deoglSharedBlockSPBElement * const mergeElement =
			static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(index + 1));
		if(mergeElement->GetEmpty()){
			element->SetCount(count + mergeElement->GetCount());
			
			// if the next empty block is not the last block filling up space up the to
			// available space remove it from the empty block list
			if(index + 1 < pElements.GetCount() - 1){
				pEmptyElements.RemoveFrom(pEmptyElements.IndexOf(mergeElement));
			}
			
			pElements.RemoveFrom(index + 1);
			
			// if the merged empty block is the last block filling up space up to available
			// space do not add it to the empty block list
			if(index == pElements.GetCount() - 1){
				addToEmptyElements = false;
				
				if(merged){
					// in this situation the element has been merge with both the previous and
					// next empty element. in this case the previous empty element becomes the
					// last element filling up space up to available space and has to be
					// removed from the empty element list
					pEmptyElements.RemoveFrom(pEmptyElements.IndexOf(element));
				}
			}
		}
	}
	
	if(addToEmptyElements){
		pEmptyElements.Add(element);
	}
	
	// book keeping
	pUsedElementCount -= count;
	pFreeElementCount += count;
}

int deoglSharedBlockSPB::GetFreeElementCountAtEnd() const{
	const deoglSharedBlockSPBElement &element =
		*static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(pElements.GetCount() - 1));
	return element.GetEmpty() ? element.GetCount() : 0;
}

void deoglSharedBlockSPB::DebugPrint(deoglRTLogger &logger) const{
	const int count = pElements.GetCount();
	decStringList list;
	decString string;
	int i;
	
	logger.LogInfoFormat("SPB: size=%d used=%d free=%d freeAtEnd=%d",
		pSize, pUsedElementCount, pFreeElementCount, GetFreeElementCountAtEnd());
	for(i=0; i<count; i++){
		const deoglSharedBlockSPBElement * const element =
			static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(i));
		string.Format("[%c:%d,%d]", element->GetEmpty() ? 'E' : 'U', element->GetIndex(), element->GetCount());
		list.Add(string);
	}
	logger.LogInfo(DEJoin(list, " "));
}

int deoglSharedBlockSPB::pIndexOfEmptyElementWithMinCount(int count){
	// check if an empty block with enough space can be reused. this list does not include
	// the last empty block filling the space up to the available space. this is done like
	// this to ensure anything but the last space filler block is used if possible
	const int emptyCount = pEmptyElements.GetCount();
	int i;
	
	for(i=0; i<emptyCount; i++){
		deoglSharedBlockSPBElement * const element =
			static_cast<deoglSharedBlockSPBElement*>(pEmptyElements.GetAt(i));
		if(element->GetCount() >= count){
			pEmptyElements.RemoveFrom(i);
			return pElements.IndexOf(element);
		}
	}
	
	// check if the last empty block filling up space up to the available space is usable
	deoglSharedBlockSPBElement * const element =
		static_cast<deoglSharedBlockSPBElement*>(pElements.GetAt(pElements.GetCount() - 1));
	if(element->GetEmpty() && element->GetCount() >= count){
		return pElements.GetCount() - 1;
	}
	
	// no empty block found with enough free space
	return -1;
}

void deoglSharedBlockSPB::pCheckSize(){
	const int change = pParameterBlock->GetElementCount() - pSize;
	if(change == 0){
		return;
	}
	
	DEASSERT_TRUE(change > 0)
	
	deoglSharedBlockSPBElement * const element =
		(deoglSharedBlockSPBElement*)pElements.GetAt(pElements.GetCount() - 1);
	if(element->GetEmpty()){
		element->SetCount(element->GetCount() + change);
		
	}else{
		pElements.Add(deoglSharedBlockSPBElement::ObjRef::New(*this, pSize, change));
	}
	
	pFreeElementCount += change;
	pSize = pParameterBlock->GetElementCount();
}
