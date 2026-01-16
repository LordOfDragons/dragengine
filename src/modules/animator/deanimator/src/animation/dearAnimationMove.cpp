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
#include <string.h>

#include "dearAnimationMove.h"
#include "dearAnimationKeyframeList.h"
#include "dearAnimationKeyframeVPSList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Class dearAnimationMove
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationMove::dearAnimationMove(const deAnimationMove &move) :
pName(move.GetName()),
pPlaytime(move.GetPlaytime())
{
	try{
		pCreateKeyframeLists(move);
		pCreateKeyframeVPSLists(move);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dearAnimationMove::~dearAnimationMove(){
	pCleanUp();
}



// Management
///////////////

dearAnimationKeyframeList *dearAnimationMove::GetKeyframeListAt(int index) const{
	return pKeyframeLists.GetAt(index);
}

dearAnimationKeyframeVPSList *dearAnimationMove::GetKeyframeVPSListAt(int index) const{
	return pKeyframeVPSLists.GetAt(index);
}



// Private functions
//////////////////////

void dearAnimationMove::pCleanUp(){
}



void dearAnimationMove::pCreateKeyframeLists(const deAnimationMove &move){
	const int count = move.GetKeyframeListCount();
	int i;
	
	for(i=0; i<count; i++){
		pKeyframeLists.Add(deTUniqueReference<dearAnimationKeyframeList>::New(move.GetKeyframeList(i)));
	}
}

void dearAnimationMove::pCreateKeyframeVPSLists(const deAnimationMove &move){
	const int count = move.GetVertexPositionSetKeyframeListCount();
	int i;
	
	for(i=0; i<count; i++){
		pKeyframeVPSLists.Add(deTUniqueReference<dearAnimationKeyframeVPSList>::New(
			move.GetVertexPositionSetKeyframeList(i)));
	}
}
