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

#include "deoglSharedVBO.h"
#include "deoglSharedVBOList.h"
#include "deoglSharedVBOBlock.h"
#include "deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedVBOList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBOList::deoglSharedVBOList(deoglRenderThread &renderThread,
const deoglVBOLayout &layout, GLenum drawType, int maxSize, int maxIndexSize) :
pRenderThread(renderThread),
pLayout(layout),
pDrawType(drawType)
{
	if(maxSize < 0 || maxIndexSize < 0){
		DETHROW(deeInvalidParam);
	}
	
	pMaxSize = maxSize - maxSize % layout.GetStride();
	pMaxPointCount = pMaxSize / layout.GetStride();
	
	if(layout.GetIndexSize() > 0){
		pMaxIndexSize = maxIndexSize - maxIndexSize % layout.GetIndexSize();
		pMaxIndexCount = pMaxIndexSize / layout.GetIndexSize();
		
	}else{
		pMaxIndexSize = 0;
		pMaxIndexCount = 0;
	}
}

deoglSharedVBOList::~deoglSharedVBOList(){
}



// Management
///////////////

bool deoglSharedVBOList::Matches(const deoglVBOLayout &layout, GLenum drawType) const{
	return drawType == pDrawType && layout == pLayout;
}

int deoglSharedVBOList::GetCount() const{
	return pVBOs.GetCount();
}

deoglSharedVBO *deoglSharedVBOList::GetAt(int index) const{
	return (deoglSharedVBO*)pVBOs.GetAt(index);
}

deoglSharedVBOBlock *deoglSharedVBOList::AddData(int size, int indexCount){
	if(size < 1 || indexCount < 0){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pVBOs.GetCount();
	deoglSharedVBOBlock *block;
	int i;
	
	// if there is a vbo able to host the data add it there
	for(i=0; i<count; i++){
		block = ((deoglSharedVBO*)pVBOs.GetAt(i))->AddBlock(size, indexCount);
		if(block){
			return block;
		}
	}
	
	// otherwise create a new vbo to add the data there
	deoglSharedVBO::Ref vbo;
	
	if(size > pMaxPointCount || indexCount > pMaxIndexCount){
		// a little hack to deal with very large models. often they do not fit into the VBOs
		// we have so for this case we create an oversized VBO for the time being. has to be
		// made better later on
		vbo.TakeOverWith(this, size, indexCount);
		
	}else{
		vbo.TakeOverWith(this, pMaxPointCount, pMaxIndexCount);
	}
	
	pVBOs.Add(vbo);
	
	block = ((deoglSharedVBO*)pVBOs.GetAt(pVBOs.GetCount() - 1))->AddBlock(size, indexCount);
	if(!block){
		DETHROW(deeInvalidAction);
	}
	
	return block;
}



void deoglSharedVBOList::PrepareVBOs(){
	const int count = pVBOs.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((deoglSharedVBO*)pVBOs.GetAt(i))->Prepare();
	}
}
