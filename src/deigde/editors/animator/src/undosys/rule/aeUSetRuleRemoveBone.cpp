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

#include "aeUSetRuleRemoveBone.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleRemoveBone
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleRemoveBone::aeUSetRuleRemoveBone(aeRule* rule, const char* pattern){
	if(! rule || ! pattern){
		DETHROW(deeInvalidParam);
	}
	
	pRule = NULL;
	
	const decStringSet &ruleBoneList = rule->GetListBones();
	const int boneCount = ruleBoneList.GetCount();
	int i;
	
	for(i=0; i<boneCount; i++){
		const decString &bone = ruleBoneList.GetAt(i);
		
		if(bone.MatchesPattern(pattern)){
			pBones.Add(bone);
		}
	}
	
	if(pBones.GetCount() == 1){
		SetShortInfo("Remove bone from rule");
		
	}else{
		SetShortInfo("Remove bones from rule");
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleRemoveBone::~aeUSetRuleRemoveBone(){
	if(pRule){
		pRule->FreeReference();
	}
}



// Management
///////////////

bool aeUSetRuleRemoveBone::HasBones() const{
	return pBones.GetCount() > 0;
}

void aeUSetRuleRemoveBone::Undo(){
	const int count = pBones.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRule->AddBone(pBones.GetAt(i));
	}
}

void aeUSetRuleRemoveBone::Redo(){
	const int count = pBones.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pRule->RemoveBone(pBones.GetAt(i));
	}
}
