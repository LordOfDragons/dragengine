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

#include "aeURuleLimitSetVertexPositionSetMax.h"
#include "../../../animator/rule/aeRuleLimit.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleLimitSetVertexPositionSetMax
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleLimitSetVertexPositionSetMax::aeURuleLimitSetVertexPositionSetMax(
	aeRuleLimit *rule, float newMax)
{
	DEASSERT_NOTNULL(rule)
	
	pRule = nullptr;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMax = rule->GetMaximumVertexPositionSet();
		pNewMax = newMax;
		
		SetShortInfo("Limit Set maximum vertex position set");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeURuleLimitSetVertexPositionSetMax::~aeURuleLimitSetVertexPositionSetMax(){
	pCleanUp();
}



// Management
///////////////

void aeURuleLimitSetVertexPositionSetMax::Undo(){
	pRule->SetMaximumVertexPositionSet(pOldMax);
}

void aeURuleLimitSetVertexPositionSetMax::Redo(){
	pRule->SetMaximumVertexPositionSet(pNewMax);
}



// Private Functions
//////////////////////

void aeURuleLimitSetVertexPositionSetMax::pCleanUp(){
	if(pRule){
		pRule->FreeReference();
	}
}
