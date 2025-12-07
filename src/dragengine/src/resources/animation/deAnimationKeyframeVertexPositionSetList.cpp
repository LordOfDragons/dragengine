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

#include "deAnimation.h"
#include "deAnimationKeyframeVertexPositionSet.h"
#include "deAnimationKeyframeVertexPositionSetList.h"
#include "../../common/exceptions.h"



// class deAnimationKeyframeVertexPositionSetList
///////////////////////////////////////////////////

// constructor, destructor
////////////////////////////

deAnimationKeyframeVertexPositionSetList::deAnimationKeyframeVertexPositionSetList() :
pKeyframes(nullptr),
pKeyframeCount(0),
pKeyframeSize(0){
}

deAnimationKeyframeVertexPositionSetList::~deAnimationKeyframeVertexPositionSetList(){
	int i;
	if(pKeyframes){
		for(i=0; i<pKeyframeCount; i++){
			if(pKeyframes[i]){
				delete pKeyframes[i];
			}
		}
		delete [] pKeyframes;
	}
}



// Management
///////////////

deAnimationKeyframeVertexPositionSet *deAnimationKeyframeVertexPositionSetList::GetKeyframe(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pKeyframeCount);
	
	return pKeyframes[index];
}

void deAnimationKeyframeVertexPositionSetList::AddKeyframe(deAnimationKeyframeVertexPositionSet *keyframe){
	DEASSERT_NOTNULL(keyframe)
	
	if(pKeyframeCount == pKeyframeSize){
		const int newSize = pKeyframeCount * 3 / 2 + 1;
		int i;
		deAnimationKeyframeVertexPositionSet ** const newArray = new deAnimationKeyframeVertexPositionSet*[newSize];
		if(pKeyframes){
			for(i=0; i<pKeyframeCount; i++){
				newArray[i] = pKeyframes[i];
			}
			delete [] pKeyframes;
		}
		pKeyframes = newArray;
		pKeyframeSize = newSize;
	}
	
	pKeyframes[pKeyframeCount] = keyframe;
	pKeyframeCount++;
}

int deAnimationKeyframeVertexPositionSetList::GetLeadingKeyframe(int startkf, float time) const{
	int leadkf = 0;
	if(startkf > 0 && time >= pKeyframes[startkf]->GetTime()){
		leadkf = startkf;
	}
	while(leadkf < pKeyframeCount && pKeyframes[leadkf]->GetTime() < time){
		leadkf++;
	}
	return leadkf > 0 ? leadkf - 1 : 0;
}
