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
#include <stdlib.h>

#include "aeUSetRuleAddBone.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/component/deComponent.h>



// Class aeUSetRuleAddBone
////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleAddBone::aeUSetRuleAddBone(aeRule *rule, const char *pattern){
	if(! rule || ! pattern){
		DETHROW(deeInvalidParam);
	}
	
	pRule = NULL;
	
	const decStringSet &ruleBoneList = rule->GetListBones();
	const deRig *engRig = rule->GetAnimator()->GetEngineRig();
	
	if(engRig){
		const int boneCount = engRig->GetBoneCount();
		int i;
		
		for(i=0; i<boneCount; i++){
			const decString bone(engRig->GetBoneAt(i).GetName());
			
			if(bone.MatchesPattern(pattern) && ! ruleBoneList.Has(bone)){
				pBones.Add(bone);
			}
		}
		
	}else{
		if(! ruleBoneList.Has(pattern)){
			pBones.Add(pattern);
		}
	}
	
	if(pBones.GetCount() == 1){
		SetShortInfo("Add bone to rule");
		
	}else{
		SetShortInfo("Add bones to rule");
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleAddBone::~aeUSetRuleAddBone(){
	if(pRule){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeUSetRuleAddBone::Undo(){
	const int count = pBones.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRule->RemoveBone(pBones.GetAt(i));
	}
}

void aeUSetRuleAddBone::Redo(){
	const int count = pBones.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRule->AddBone(pBones.GetAt(i));
	}
}
