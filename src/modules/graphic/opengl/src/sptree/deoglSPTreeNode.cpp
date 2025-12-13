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
#include "deoglSPTreeNode.h"
#include <dragengine/common/exceptions.h>



// Class deoglSPTreeNode
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglSPTreeNode::deoglSPTreeNode(const decVector &center, const decVector &halfSize){
	pCenter = center;
	pHalfSize = halfSize;
	pSplitState = essNone;
	pChildLeft = NULL;
	pChildRight = NULL;
}

deoglSPTreeNode::~deoglSPTreeNode(){
	if(pChildRight) delete pChildRight;
	if(pChildLeft) delete pChildLeft;
}



// Management
///////////////

void deoglSPTreeNode::SetSplitState(int splitState){
	if(splitState < essNone || splitState > essZAxis) DETHROW(deeInvalidParam);
	
	if(splitState != pSplitState){
		pSplitState = splitState;
		
		if(pChildRight){
			delete pChildRight;
			pChildRight = NULL;
		}
		if(pChildLeft){
			delete pChildLeft;
			pChildLeft = NULL;
		}
		
		if(splitState == essXAxis){
			float halfSize = pHalfSize.x * 0.5f;
			
			pChildLeft = new deoglSPTreeNode(decVector(pCenter.x - halfSize, pCenter.y, pCenter.z),
				decVector(halfSize, pHalfSize.y, pHalfSize.z));
			if(!pChildLeft) DETHROW(deeOutOfMemory);
			
			pChildRight = new deoglSPTreeNode(decVector(pCenter.x + halfSize, pCenter.y, pCenter.z),
				decVector(halfSize, pHalfSize.y, pHalfSize.z));
			if(!pChildRight) DETHROW(deeOutOfMemory);
			
		}else if(splitState == essYAxis){
			float halfSize = pHalfSize.y * 0.5f;
			
			pChildLeft = new deoglSPTreeNode(decVector(pCenter.x, pCenter.y - halfSize, pCenter.z),
				decVector(pHalfSize.x, halfSize, pHalfSize.z));
			if(!pChildLeft) DETHROW(deeOutOfMemory);
			
			pChildRight = new deoglSPTreeNode(decVector(pCenter.x, pCenter.y + halfSize, pCenter.z),
				decVector(pHalfSize.x, halfSize, pHalfSize.z));
			if(!pChildRight) DETHROW(deeOutOfMemory);
			
		}else if(splitState == essZAxis){
			float halfSize = pHalfSize.z * 0.5f;
			
			pChildLeft = new deoglSPTreeNode(decVector(pCenter.x, pCenter.y, pCenter.z - halfSize),
				decVector(pHalfSize.x, pHalfSize.y, halfSize));
			if(!pChildLeft) DETHROW(deeOutOfMemory);
			
			pChildRight = new deoglSPTreeNode(decVector(pCenter.x, pCenter.y, pCenter.z + halfSize),
				decVector(pHalfSize.x, pHalfSize.y, halfSize));
			if(!pChildRight) DETHROW(deeOutOfMemory);
		}
	}
}
