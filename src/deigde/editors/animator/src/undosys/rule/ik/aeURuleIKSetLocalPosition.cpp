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

#include "aeURuleIKSetLocalPosition.h"
#include "../../../animator/rule/aeRuleInverseKinematic.h"

#include <dragengine/common/exceptions.h>

// Class aeURuleIKSetLocalPosition
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleIKSetLocalPosition::aeURuleIKSetLocalPosition(aeRuleInverseKinematic *rule, const decVector &newPos){
	if(!rule) DETHROW(deeInvalidParam);
	
	pRule = NULL;
	
	try{
		pRule = rule;
		
		pOldPos = rule->GetLocalPosition();
		pNewPos = newPos;
		
		SetShortInfo("Set inverse kinematic rule local position");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeURuleIKSetLocalPosition::~aeURuleIKSetLocalPosition(){
	pCleanUp();
}

// Management
///////////////

void aeURuleIKSetLocalPosition::Undo(){
	pRule->SetLocalPosition(pOldPos);
}

void aeURuleIKSetLocalPosition::Redo(){
	pRule->SetLocalPosition(pNewPos);
}

// Private Functions
//////////////////////

void aeURuleIKSetLocalPosition::pCleanUp(){

}
