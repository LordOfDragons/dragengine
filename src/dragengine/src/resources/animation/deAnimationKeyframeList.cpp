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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deAnimation.h"
#include "deAnimationKeyframe.h"
#include "deAnimationKeyframeList.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// class deAnimationKeyframeList
//////////////////////////////////

// constructor, destructor
////////////////////////////

deAnimationKeyframeList::deAnimationKeyframeList(){
	pKeyframes = NULL;
	pKeyframeCount = 0;
	pKeyframeSize = 0;
}

deAnimationKeyframeList::~deAnimationKeyframeList(){
	int i;
	if(pKeyframes){
		for(i=0; i<pKeyframeCount; i++){
			if(pKeyframes[i]) delete pKeyframes[i];
		}
		delete [] pKeyframes;
	}
}



// management
///////////////

deAnimationKeyframe *deAnimationKeyframeList::GetKeyframe(int index) const{
	if(index < 0 || index >= pKeyframeCount) DETHROW(deeOutOfBoundary);
	return pKeyframes[index];
}

void deAnimationKeyframeList::AddKeyframe(deAnimationKeyframe *keyframe){
	if(!keyframe) DETHROW(deeInvalidParam);
	if(pKeyframeCount == pKeyframeSize){
		int i, newSize = pKeyframeCount * 3 / 2 + 1;
		deAnimationKeyframe **newArray = new deAnimationKeyframe*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pKeyframes){
			for(i=0; i<pKeyframeCount; i++) newArray[i] = pKeyframes[i];
			delete [] pKeyframes;
		}
		pKeyframes = newArray;
		pKeyframeSize = newSize;
	}
	pKeyframes[pKeyframeCount] = keyframe;
	pKeyframeCount++;
}

int deAnimationKeyframeList::GetLeadingKeyframe(int startkf, float time) const{
	int leadkf = 0;
	if(startkf > 0 && time >= pKeyframes[startkf]->GetTime()){
		leadkf = startkf;
	}
	while(leadkf < pKeyframeCount && pKeyframes[leadkf]->GetTime() < time) leadkf++;
	return leadkf > 0 ? leadkf - 1 : 0;
}
