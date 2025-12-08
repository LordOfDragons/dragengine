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
#include <stdlib.h>
#include <string.h>

#include "ceCConditionLogic.h"

#include <dragengine/common/exceptions.h>



// Class ceCConditionLogic
////////////////////////////

// Constructor, destructor
////////////////////////////

ceCConditionLogic::ceCConditionLogic() :
ceConversationCondition(ectLogic),
pOperator(eopAll),
pTIMExpanded(true){
}

ceCConditionLogic::ceCConditionLogic(const ceCConditionLogic &condition) :
ceConversationCondition(condition),
pOperator(condition.GetOperator()),
pTIMExpanded(condition.pTIMExpanded)
{
	const ceConversationConditionList &conditions = condition.GetConditions();
	ceConversationCondition::Ref newCondition = NULL;
	int i, count;
	
	try{
		count = conditions.GetCount();
		for(i=0; i<count; i++){
			newCondition = conditions.GetAt(i)->CreateCopy();
			pConditions.Add(newCondition);
			newCondition = NULL;
		}
		
	}catch(const deException &){
		pConditions.RemoveAll();
		throw;
	}
}

ceCConditionLogic::~ceCConditionLogic(){
}



// Management
///////////////

void ceCConditionLogic::SetOperator(eOperators aOperator){
	if(aOperator < eopNone || aOperator > eopAll){
		DETHROW(deeInvalidParam);
	}
	
	pOperator = aOperator;
}



ceConversationCondition *ceCConditionLogic::CreateCopy() const{
	return new ceCConditionLogic(*this);
}



// UI
///////

void ceCConditionLogic::SetTIMExpanded(bool expanded){
	pTIMExpanded = expanded;
}
