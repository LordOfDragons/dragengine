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

#include "aeUSetRuleSModMinVertexPositionSet.h"
#include "../../../animator/rule/aeRuleStateManipulator.h"

#include <dragengine/common/exceptions.h>


// Class aeUSetRuleSModMinVertexPositionSet
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleSModMinVertexPositionSet::aeUSetRuleSModMinVertexPositionSet(
	aeRuleStateManipulator *rule, float newMin)
{
	DEASSERT_NOTNULL(rule)
	
	pRule = nullptr;
	
	try{
		pRule = rule;
		pOldMin = rule->GetMinimumVertexPositionSet();
		pNewMin = newMin;
		
		SetShortInfo("Set state manipulator rule minimum vertex position set");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeUSetRuleSModMinVertexPositionSet::~aeUSetRuleSModMinVertexPositionSet(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleSModMinVertexPositionSet::Undo(){
	pRule->SetMinimumVertexPositionSet(pOldMin);
}

void aeUSetRuleSModMinVertexPositionSet::Redo(){
	pRule->SetMinimumVertexPositionSet(pNewMin);
}



// Private Functions
//////////////////////

void aeUSetRuleSModMinVertexPositionSet::pCleanUp(){
}
