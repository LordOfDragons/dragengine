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

#include "deoglDynamicTBO.h"
#include "deoglDynamicTBOBlock.h"
#include "deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOBlock
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOBlock::deoglDynamicTBOBlock(deoglDynamicTBOShared *tbo) :
pSharedTBO(tbo),
pOffset(0),
pSize(0),
pEmpty(true)
{
	if(!tbo){
		DETHROW(deeInvalidParam);
	}
}

deoglDynamicTBOBlock::deoglDynamicTBOBlock(deoglDynamicTBOShared *tbo, int offset, int size) :
pSharedTBO(tbo),
pOffset(offset),
pSize(size),
pEmpty(true)
{
	if(!tbo || offset < 0 || size < 0){
		DETHROW(deeInvalidParam);
	}
}

deoglDynamicTBOBlock::~deoglDynamicTBOBlock(){
}



// Management
///////////////

void deoglDynamicTBOBlock::DropSharedTBO(){
	pSharedTBO = NULL;
}

void deoglDynamicTBOBlock::SetOffset(int offset){
	if(offset < 0){
		DETHROW(deeInvalidParam);
	}
	if(offset == pOffset){
		return;
	}
	
	pOffset = offset;
}

void deoglDynamicTBOBlock::SetSize(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	if(size == pSize){
		return;
	}
	
	pSize = size;
	pData = NULL;
	pData2 = NULL;
}

void deoglDynamicTBOBlock::SetData(deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2){
	pData = tbo;
	pData2 = tbo2;
}

void deoglDynamicTBOBlock::SetEmpty(bool empty){
	pEmpty = empty;
	pData = NULL;
	pData2 = NULL;
}

void deoglDynamicTBOBlock::Drop(){
	if(pSharedTBO && !pEmpty){
		pSharedTBO->RemoveBlock(this);
	}
}

void deoglDynamicTBOBlock::WriteToTBO(){
	if(pEmpty || !pSharedTBO){
		return;
	}
	
	// write data to first TBO
	if(pData){
		deoglDynamicTBO &tbo = *pSharedTBO->GetTBO();
		const int stride = tbo.GetComponentCount() * tbo.GetDataTypeSize() * pSharedTBO->GetStride();
		uint8_t * const tboData = tbo.GetData();
		
		//const int offset = pOffset * componentCount;
		//pTBO->SetTBO( offset, *pData );
		//pTBO->Update( offset, pSize );
		const uint8_t * const sourceData = ((deoglDynamicTBO*)(deObject*)pData)->GetData();
		if(sourceData){
			memcpy(tboData + pOffset * stride, sourceData, pSize * stride);
		}
	}
	
	// write data to second TBO if present
	if(pData2){
		deoglDynamicTBO * const tbo2 = pSharedTBO->GetTBO2();
		if(tbo2){
			const int stride2 = tbo2->GetComponentCount() * tbo2->GetDataTypeSize() * pSharedTBO->GetStride2();
			uint8_t * const tboData2 = tbo2->GetData();
			
			const uint8_t * const sourceData = ((deoglDynamicTBO*)(deObject*)pData2)->GetData();
			if(sourceData){
				memcpy(tboData2 + pOffset * stride2, sourceData, pSize * stride2);
			}
		}
	}
	
	// mark shared TBO dirty
	pSharedTBO->MarkDirty();
}
