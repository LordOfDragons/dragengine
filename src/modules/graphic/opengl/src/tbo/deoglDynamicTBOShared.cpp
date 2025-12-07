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
#include "deoglDynamicTBOShared.h"
#include "deoglDynamicTBOBlock.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOShared
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOShared::deoglDynamicTBOShared(deoglDynamicTBO *tbo, int stride,
	deoglDynamicTBO *tbo2, int stride2) :
pTBO(tbo),
pTBO2(tbo2),
pStride(stride),
pStride2(stride2),
pUsedSize(0),
pDirty(true)
{
	if(!tbo || stride < 1 || stride2 < 1){
		DETHROW(deeInvalidParam);
	}
}

deoglDynamicTBOShared::~deoglDynamicTBOShared(){
	const int count = pBlocks.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoglDynamicTBOBlock*)pBlocks.GetAt(i))->DropSharedTBO();
	}
	pBlocks.RemoveAll();
}



// Management
///////////////

void deoglDynamicTBOShared::Prepare(){
	if(!pDirty){
		return;
	}
	
	deoglDynamicTBO &tbo = *GetTBO();
// 	if( pUsedSize * pStride > tbo.GetPixelCount() ){
// 		tbo.IncreasePixelCount( pUsedSize * pStride - tbo.GetPixelCount() );
// 	}
	
	deoglDynamicTBO * const tbo2 = GetTBO2();
// 	if( tbo2 && pUsedSize * pStride2 > tbo2->GetPixelCount() ){
// 		tbo2->IncreasePixelCount( pUsedSize * pStride2 - tbo2->GetPixelCount() );
// 	}
	
	//const int componentCount = pTBO->GetComponentCount();
// 	const int stride = tbo.GetComponentCount() * tbo.GetDataTypeSize() * pStride;
// 	uint8_t * const tboData = tbo.GetData();
// 	const int count = pBlocks.GetCount();
// 	int i;
// 	
// 	for( i=0; i<count; i++ ){
// 		const deoglDynamicTBOBlock &block = *( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) );
// 		if( block.GetEmpty() || ! block.GetData() ){
// 			continue;
// 		}
// 		
// 		//const int offset = block.GetOffset() * componentCount;
// 		//pTBO->SetTBO( offset, *block.GetData() );
// 		//pTBO->Update( offset, block.GetSize() );
// 		memcpy( tboData + block.GetOffset() * stride, block.GetData()->GetData(), block.GetSize() * stride );
// 	}
	
	tbo.Update();
	
	if(tbo2){
// 		const int stride2 = tbo2->GetComponentCount() * tbo2->GetDataTypeSize() * pStride2;
// 		uint8_t * const tboData2 = tbo2->GetData();
// 		
// 		for( i=0; i<count; i++ ){
// 			const deoglDynamicTBOBlock &block = *( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) );
// 			if( block.GetEmpty() || ! block.GetData2() ){
// 				continue;
// 			}
// 			
// 			memcpy( tboData2 + block.GetOffset() * stride2, block.GetData2()->GetData(), block.GetSize() * stride2 );
// 		}
		
		tbo2->Update();
	}
	
	pDirty = false;
}

void deoglDynamicTBOShared::UpdateUsedSizes(){
	const int count = pBlocks.GetCount();
	int i;
	
	pUsedSize = 0;
	
	for(i=count-1; i>=0; i--){
		const deoglDynamicTBOBlock &block = *((deoglDynamicTBOBlock*)pBlocks.GetAt(i));
		if(block.GetEmpty()){
			continue;
		}
		
		pUsedSize = block.GetOffset() + block.GetSize();
		break;
	}
}

void deoglDynamicTBOShared::MarkDirty(){
	pDirty = true;
}



int deoglDynamicTBOShared::GetBlockCount() const{
	return pBlocks.GetCount();
}

deoglDynamicTBOBlock *deoglDynamicTBOShared::GetBlockAt(int index) const{
	return (deoglDynamicTBOBlock*)pBlocks.GetAt(index);
}

deoglDynamicTBOBlock *deoglDynamicTBOShared::AddBlock(deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2){
	if(!tbo){
		DETHROW(deeInvalidParam);
	}
	if(tbo2 && tbo2->GetPixelCount() / pStride2 != tbo->GetPixelCount() / pStride){
		DETHROW(deeInvalidParam);
	}
	
	// find block
	const int tboSize = tbo->GetPixelCount() / pStride;
	int index = FirstMatchingBlock(tbo);
	deoglDynamicTBOBlock *block = NULL;
	
	if(index != -1){
		block = (deoglDynamicTBOBlock*)pBlocks.GetAt(index);
		
		// if the empty block is larger than the requested size add a new empty block with the
		// remaining empty space right after this block
		if(block->GetSize() > tboSize){
			const deoglDynamicTBOBlock::Ref emptyBlock(deoglDynamicTBOBlock::Ref::NewWith(this,
				block->GetOffset() + tboSize, block->GetSize() - tboSize));
			pBlocks.Insert(emptyBlock, index + 1);
		}
		
	}else{
		block = pAddEmptyBlock();
	}
	
	// turn the block into a non-empty block with the requested size
	block->SetSize(tboSize);
	block->SetEmpty(false);
	block->SetData(tbo, tbo2); // has to come last
	
	// update used size. if the size is larger than the TBO size increase the TBO size
	UpdateUsedSizes();
	pEnsureTBOSize();
	
	// copy the data to the TBO data. this does not yet upload the data
	block->WriteToTBO();
	
	// mark shared TBO dirty. this will cause TBO data to be uploaded to the TBO
	pDirty = true;
	
	// return block. caller takes over reference
	block->AddReference();
	return block;
}

void deoglDynamicTBOShared::RemoveBlock(deoglDynamicTBOBlock *block){
	int index = pBlocks.IndexOf(block);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	// turn the block into an empty block
	block->SetEmpty(true);
	
	// if the previous block is empty merge this block with the previous block
	deoglDynamicTBOBlock *mergeBlock = NULL;
	
	if(index > 0){
		mergeBlock = (deoglDynamicTBOBlock*)pBlocks.GetAt(index - 1);
		
		if(mergeBlock->GetEmpty()){
			mergeBlock->SetSize(mergeBlock->GetSize() + block->GetSize());
			
			block->DropSharedTBO();
			pBlocks.RemoveFrom(index);
			
			block = mergeBlock;
			index--;
		}
	}
	
	// if the next block is empty merge the next block with this block
	if(index < pBlocks.GetCount() - 1){
		mergeBlock = (deoglDynamicTBOBlock*)pBlocks.GetAt(index + 1);
		
		if(mergeBlock->GetEmpty()){
			block->SetSize(block->GetSize() + mergeBlock->GetSize());
			
			mergeBlock->DropSharedTBO();
			pBlocks.RemoveFrom(index + 1);
		}
	}
	
	UpdateUsedSizes();
}

int deoglDynamicTBOShared::FirstMatchingBlock(deoglDynamicTBO *tbo){
	const int tboSize = tbo->GetPixelCount() / pStride;
	const int count = pBlocks.GetCount();
	const int last = count - 1;
	int i;
	
	for(i=0; i<count; i++){
		const deoglDynamicTBOBlock &block = *((deoglDynamicTBOBlock*)pBlocks.GetAt(i));
		if(block.GetEmpty() && (i == last || block.GetSize() >= tboSize)){
			return i;
		}
	}
	
	return -1;
}



void deoglDynamicTBOShared::DebugPrint(deoglRTLogger &logger) const{
	logger.LogInfoFormat("DynamicTBOShared: tbo=%p tbo2=%p stride=%d stride2=%d usedSize=%d",
		(deObject*)pTBO, (deObject*)pTBO2, pStride, pStride2, pUsedSize);
	const int count = pBlocks.GetCount();
	int i;
	for(i=0; i<count; i++){
		const deoglDynamicTBOBlock &block = *((deoglDynamicTBOBlock*)pBlocks.GetAt(i));
		logger.LogInfoFormat("- Block %d: offset=%d size=%d empty=%d",
			i, block.GetOffset(), block.GetSize(), block.GetEmpty());
	}
}



// Private Functions
//////////////////////

deoglDynamicTBOBlock *deoglDynamicTBOShared::pAddEmptyBlock(){
	const int count = pBlocks.GetCount();
	int offset = 0;
	
	if(count > 0){
		const deoglDynamicTBOBlock &block = *((deoglDynamicTBOBlock*)pBlocks.GetAt(count - 1));
		offset = block.GetOffset() + block.GetSize();
	}
	
	const deoglDynamicTBOBlock::Ref block(deoglDynamicTBOBlock::Ref::NewWith(this, offset, 0));
	pBlocks.Add(block);
	
	return block; // valid because pBlocks holds reference
}

void deoglDynamicTBOShared::pEnsureTBOSize(){
	deoglDynamicTBO &tbo = *GetTBO();
	if(pUsedSize * pStride > tbo.GetPixelCount()){
		tbo.IncreasePixelCount(pUsedSize * pStride - tbo.GetPixelCount());
	}
	
	deoglDynamicTBO * const tbo2 = GetTBO2();
	if(tbo2 && pUsedSize * pStride2 > tbo2->GetPixelCount()){
		tbo2->IncreasePixelCount(pUsedSize * pStride2 - tbo2->GetPixelCount());
	}
}
