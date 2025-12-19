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

#include "ceUCTargetSetName.h"
#include "../action/ceUndoCAction.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/target/ceTarget.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCTargetSetName
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCTargetSetName::ceUCTargetSetName(ceTarget *target, const char *newName){
	if(!target || !newName) DETHROW(deeInvalidParam);
	
	SetShortInfo("Target Set Name");
	
	pOldName = target->GetName();
	pNewName = newName;
	
	pTarget = target;
	target->GetConversation()->GetFiles().Visit([&](const ceConversationFile &f){
		f.GetTopics().Visit([&](ceConversationTopic *t){
			pAddActions(t, t->GetActions());
		});
	});
	
	pCameraShots = target->GetConversation()->GetCameraShotList().Collect([&](ceCameraShot *c){
		return c->GetCameraTarget() == pOldName || c->GetLookAtTarget() == pOldName;
	});
}

ceUCTargetSetName::~ceUCTargetSetName(){
}



// Management
///////////////

void ceUCTargetSetName::Undo(){
	pSetName(pNewName, pOldName);
}

void ceUCTargetSetName::Redo(){
	pSetName(pOldName, pNewName);
}



// Private Functions
//////////////////////

void ceUCTargetSetName::pSetName(const char *oldName, const char *newName){
	pTarget->SetName(newName);
	
	pActions.Visit([&](ceUndoCAction &a){
		if(a.GetAction()->GetType() == ceConversationAction::eatCameraShot){
			ceCACameraShot &ac = (ceCACameraShot&)*a.GetAction();
			
			if(ac.GetCameraTarget() == oldName){
				ac.SetCameraTarget(newName);
			}
			if(ac.GetLookAtTarget() == oldName){
				ac.SetLookAtTarget(newName);
			}
		}
		
		a.GetTopic()->NotifyActionChanged(a.GetAction());
	});
	
	pCameraShots.Visit([&](ceCameraShot &c){
		if(c.GetCameraTarget() == oldName){
			c.SetCameraTarget(newName);
		}
		if(c.GetLookAtTarget() == oldName){
			c.SetLookAtTarget(newName);
		}
	});
}

void ceUCTargetSetName::pAddActions(ceConversationTopic *topic, const ceConversationAction::List &list){
	list.Visit([&](ceConversationAction *a){
		if(a->GetType() == ceConversationAction::eatCameraShot){
			const ceCACameraShot &target = *((ceCACameraShot*)a);
			
			if(target.GetCameraTarget() == pOldName || target.GetLookAtTarget() == pOldName){
				pActions.Add(ceUndoCAction::Ref::New(a, topic));
			}
			
		}else if(a->GetType() == ceConversationAction::eatIfElse){
			const ceCAIfElse &ifElse = *((ceCAIfElse*)a);
			ifElse.GetCases().Visit([&](const ceCAIfElseCase &c){
				pAddActions(topic, c.GetActions());
			});
			pAddActions(topic, ifElse.GetElseActions());
			
		}else if(a->GetType() == ceConversationAction::eatPlayerChoice){
			const ceCAPlayerChoice &playerChoice = *((ceCAPlayerChoice*)a);
			playerChoice.GetOptions().Visit([&](const ceCAPlayerChoiceOption &o){
				pAddActions(topic, o.GetActions());
			});
			pAddActions(topic, playerChoice.GetActions());
			
		}else if(a->GetType() == ceConversationAction::eatWait){
			pAddActions(topic, ((ceCAWait*)a)->GetActions());
		}
	});
}
