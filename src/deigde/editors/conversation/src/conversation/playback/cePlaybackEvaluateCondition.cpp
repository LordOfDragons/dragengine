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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cePlayback.h"
#include "cePlaybackActor.h"
#include "cePlaybackEvaluateCondition.h"
#include "command/cePlaybackCommand.h"
#include "../ceConversation.h"
#include "../condition/ceCConditionLogic.h"
#include "../condition/ceConversationCondition.h"
#include "../condition/ceCConditionHasActor.h"
#include "../condition/ceCConditionActorInConversation.h"
#include "../condition/ceCConditionVariable.h"
#include "../condition/ceCConditionActorParameter.h"
#include "../condition/ceCConditionActorCommand.h"
#include "../condition/ceCConditionGameCommand.h"
#include "../condition/ceCConditionTrigger.h"
#include "../actor/ceConversationActor.h"
#include "../file/ceConversationFile.h"
#include "../topic/ceConversationTopic.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class cePlaybackEvaluateCondition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackEvaluateCondition::cePlaybackEvaluateCondition(){
}

cePlaybackEvaluateCondition::~cePlaybackEvaluateCondition(){
}



// Management
///////////////

bool cePlaybackEvaluateCondition::EvaluateCondition(ceConversation &conversation,
const ceConversationCondition &condition){
	switch(condition.GetType()){
	case ceConversationCondition::ectLogic:
		return EvaluateLogic(conversation,
			(const ceCConditionLogic &)condition);
		
	case ceConversationCondition::ectHasActor:
		return EvaluateHasActor(conversation,
			(const ceCConditionHasActor &)condition);
		
	case ceConversationCondition::ectActorInConversation:
		return EvaluateActorInConversation(conversation,
			(const ceCConditionActorInConversation &)condition);
		
	case ceConversationCondition::ectVariable:
		return EvaluateVariable(conversation,
			(const ceCConditionVariable &)condition);
		
	case ceConversationCondition::ectActorParameter:
		return EvaluateActorParameter(conversation,
			(const ceCConditionActorParameter &)condition);
		
	case ceConversationCondition::ectActorCommand:
		return EvaluateActorCommand(conversation,
			(const ceCConditionActorCommand &)condition);
		
	case ceConversationCondition::ectGameCommand:
		return EvaluateGameCommand(conversation,
			(const ceCConditionGameCommand &)condition);
		
	case ceConversationCondition::ectTrigger:
		return EvaluateTrigger(conversation,
			(const ceCConditionTrigger &)condition);
	}
	
	return false;
}



bool cePlaybackEvaluateCondition::EvaluateLogic(ceConversation &conversation,
const ceCConditionLogic &condition){
	switch(condition.GetOperator()){
	case ceCConditionLogic::eopNone:
		for(auto c : condition.GetConditions()){
			if(EvaluateCondition(conversation, c)){
				return false;
			}
		}
		return true;
		
	case ceCConditionLogic::eopAny:
		for(auto c : condition.GetConditions()){
			if(EvaluateCondition(conversation, c)){
				return true;
			}
		}
		return false;
		
	case ceCConditionLogic::eopAll:
		for(auto c : condition.GetConditions()){
			if(!EvaluateCondition(conversation, c)){
				return false;
			}
		}
		return condition.GetConditions().IsNotEmpty();
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateHasActor(ceConversation &conversation,
const ceCConditionHasActor &condition){
	const bool result = conversation.GetActorList().GetWithIDOrAliasID(condition.GetActor());
	
	if(condition.GetNegate()){
		return !result;
		
	}else{
		return result;
	}
}

bool cePlaybackEvaluateCondition::EvaluateActorInConversation(ceConversation &conversation,
const ceCConditionActorInConversation &condition){
	const bool result = conversation.GetActorList().GetWithIDOrAliasID(condition.GetActor());
	
	if(condition.GetNegate()){
		return !result;
		
	}else{
		return result;
	}
}

bool cePlaybackEvaluateCondition::EvaluateVariable(ceConversation &conversation,
const ceCConditionVariable &condition){
	const cePlayback::VariableMap &variables = conversation.GetPlayback()->GetVariables();
	
	int testValue = condition.GetTestValue();
	if(!condition.GetTestVariable().IsEmpty()){
		testValue = variables.GetAtOrDefault(condition.GetTestVariable(), testValue);
	}
	
	int variableValue = variables.GetAtOrDefault(condition.GetVariable(), 0);
	
	switch(condition.GetOperator()){
	case ceCConditionVariable::eopEqual:
		return variableValue == testValue;
		
	case ceCConditionVariable::eopNotEqual:
		return variableValue != testValue;
		
	case ceCConditionVariable::eopLess:
		return variableValue < testValue;
		
	case ceCConditionVariable::eopLessEqual:
		return variableValue <= testValue;
		
	case ceCConditionVariable::eopGreater:
		return variableValue > testValue;
		
	case ceCConditionVariable::eopGreaterEqual:
		return variableValue >= testValue;
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateActorParameter(ceConversation &conversation,
const ceCConditionActorParameter &condition){
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = condition.GetActor();
	const decString &name = condition.GetParameter();
	int paramValue = 0;
	
	int testValue = condition.GetTestValue();
	if(!condition.GetTestVariable().IsEmpty()){
		testValue = playback.GetVariables().GetAtOrDefault(condition.GetTestVariable(), testValue);
	}
	
	ceConversationActor const * conversationActor = conversation.GetActorList().GetWithIDOrAliasID(actorID);
	if(conversationActor){
		paramValue = conversationActor->GetParameter().GetAtOrDefault(name, paramValue);
	}
	
	switch(condition.GetOperator()){
	case ceCConditionActorParameter::eopEqual:
		return paramValue == testValue;
		
	case ceCConditionActorParameter::eopNotEqual:
		return paramValue != testValue;
		
	case ceCConditionActorParameter::eopLess:
		return paramValue < testValue;
		
	case ceCConditionActorParameter::eopLessEqual:
		return paramValue <= testValue;
		
	case ceCConditionActorParameter::eopGreater:
		return paramValue > testValue;
		
	case ceCConditionActorParameter::eopGreaterEqual:
		return paramValue >= testValue;
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateActorCommand(ceConversation &conversation,
const ceCConditionActorCommand &condition){
	const decString &actorID = condition.GetActor();
	
	ceConversationActor const * conversationActor = conversation.GetActorList().GetWithIDOrAliasID(actorID);
	if(!conversationActor){
		return condition.GetNegate();
	}
	
	const cePlaybackCommand * const pcommand = conversationActor->GetCommands().FindOrDefault(
		[&](cePlaybackCommand *c){ return c->GetCommand() == condition.GetCommand(); });
	
	if(!pcommand){
		return condition.GetNegate();
	}
	
	return condition.GetNegate() ? !pcommand->GetValue() : pcommand->GetValue();
}

bool cePlaybackEvaluateCondition::EvaluateGameCommand(ceConversation &conversation,
const ceCConditionGameCommand &condition){
	const cePlaybackCommand * const pcommand = conversation.GetPlayback()->GetCommands().FindOrDefault(
		[&](cePlaybackCommand *c){ return c->GetCommand() == condition.GetCommand(); });
	
	bool result = false;
	if(pcommand){
		result = pcommand->GetValue();
	}
	
	return condition.GetNegate() ? !result : result;
}

bool cePlaybackEvaluateCondition::EvaluateTrigger(ceConversation &conversation,
const ceCConditionTrigger &condition){
	igdeTriggerTarget * const target = conversation.GetPlayback()->GetTriggerTable().GetNamed(condition.GetTrigger());
	
	switch(condition.GetTestMode()){
	case ceCConditionTrigger::etmFired:
		return target && target->GetFired();
		
	case ceCConditionTrigger::etmNotFired:
		return !target || !target->GetFired();
		
	case ceCConditionTrigger::etmEverFired:
		return target && target->GetHasFired();
		
	case ceCConditionTrigger::etmNeverFired:
		return !target || !target->GetHasFired();
	}
	
	return false;
}
