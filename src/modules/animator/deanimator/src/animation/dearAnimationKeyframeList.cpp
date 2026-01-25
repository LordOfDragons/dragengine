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

#include "dearAnimationKeyframeList.h"
#include "dearAnimationKeyframe.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimationMove.h>




// Class dearAnimationKeyframeList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimationKeyframeList::dearAnimationKeyframeList(const deAnimationKeyframe::List &list){
	try{
		pCreateKeyframes(list);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dearAnimationKeyframeList::~dearAnimationKeyframeList(){
	pCleanUp();
}



// Management
///////////////

const dearAnimationKeyframe *dearAnimationKeyframeList::GetWithTime(float time) const{
	if(pKeyframes.IsEmpty()){
		return nullptr;
	}
	
	if(time <= pKeyframes.First().GetTime()){
		return &pKeyframes.First();
	}
	
	int i;
	for(i=1; i<pKeyframes.GetCount(); i++){
		if(time < pKeyframes[i].GetTime()){
			return &pKeyframes[i - 1];
		}
	}
	
	return &pKeyframes.Last();
}



// Private functions
//////////////////////

void dearAnimationKeyframeList::pCleanUp(){
}



void dearAnimationKeyframeList::pCreateKeyframes(const deAnimationKeyframe::List &list){
	const int count = list.GetCount();
	if(count == 0){
		return;
	}
	
	pKeyframes.AddRange(count, {});
	bool negate = false;
	
	int i;
	for(i=0; i<count; i++){
		if(i < count - 1){
			pKeyframes[i].Set(list[i], list[i + 1], negate);
			
		}else{
			pKeyframes[i].Set(list[i], negate);
		}
	}
}
