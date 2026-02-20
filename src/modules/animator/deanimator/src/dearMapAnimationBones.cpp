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

#include "dearAnimatorInstance.h"
#include "dearBoneState.h"
#include "dearBoneStateList.h"
#include "dearMapAnimationBones.h"
#include "animation/dearAnimation.h"
#include "rule/dearRule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>


// Class dearMapAnimationBones
////////////////////////////////

// Constructor, destructor
////////////////////////////

dearMapAnimationBones::dearMapAnimationBones(){
}

dearMapAnimationBones::~dearMapAnimationBones(){
}



// Management
///////////////

void dearMapAnimationBones::Clear(){
	pIndices.RemoveAll();
}

void dearMapAnimationBones::Init(const dearRule &rule){
	Clear();
	
	const int count = rule.GetBoneMappingCount();
	if(count == 0){
		return;
	}
	
	pIndices.AddRange(count, -1);
	
	const dearAnimation * const animation = rule.GetUseAnimation();
	
	if(animation){
		const dearBoneStateList &boneStates = rule.GetInstance().GetBoneStateList();
		const deAnimation &engAnimation = *animation->GetAnimation();
		
		int i;
		for(i=0; i<count; i++){
			const int ruleBoneIndex = rule.GetBoneMappingFor(i);
			if(ruleBoneIndex != -1){
				pIndices[i] = engAnimation.FindBone(
					boneStates.GetStateAt(ruleBoneIndex).GetRigBoneName());
			}
		}
	}
}

int dearMapAnimationBones::GetAt(int ruleBoneIndex) const{
	return pIndices[ruleBoneIndex];
}
