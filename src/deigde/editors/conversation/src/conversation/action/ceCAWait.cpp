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

#include "ceCAWait.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class ceCAWait
///////////////////

// Constructor, destructor
////////////////////////////

ceCAWait::ceCAWait() :
ceConversationAction(eatWait),
pInterval(0.0f),
pTIMExpanded(true),
pTIMConditionExpanded(true),
pTIMActionsExpanded(true){
}

ceCAWait::ceCAWait(const ceCAWait &action) :
ceConversationAction(action),
pInterval(action.GetInterval()),
pTIMExpanded(action.pTIMExpanded),
pTIMConditionExpanded(action.pTIMConditionExpanded),
pTIMActionsExpanded(action.pTIMActionsExpanded)
{
	if(action.GetCondition()){
		pCondition = action.GetCondition()->CreateCopy();
	}
	action.GetActions().Visit([&](const ceConversationAction &a){
		pActions.Add(a.CreateCopy());
	});
}

ceCAWait::~ceCAWait(){
}



// Management
///////////////

void ceCAWait::SetCondition(ceConversationCondition *condition){
	pCondition = condition;
}

void ceCAWait::SetInterval(float interval){
	pInterval = decMath::max(interval, 0.0f);
}


ceConversationAction::Ref ceCAWait::CreateCopy() const{
	return ceCAWait::Ref::New(*this);
}


// UI
///////

void ceCAWait::SetTIMExpanded(bool expanded){
	pTIMExpanded = expanded;
}

void ceCAWait::SetTIMConditionExpanded(bool expanded){
	pTIMConditionExpanded = expanded;
}

void ceCAWait::SetTIMActionsExpanded(bool expanded){
	pTIMActionsExpanded = expanded;
}
