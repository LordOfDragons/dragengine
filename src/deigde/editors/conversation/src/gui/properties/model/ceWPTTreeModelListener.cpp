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

#include "ceWPTTreeItem.h"
#include "ceWPTTreeModel.h"
#include "ceWPTTreeModelListener.h"
#include "action/ceWPTTIMAction.h"
#include "condition/ceWPTTIMCondition.h"
#include "../ceWPTopic.h"
#include "../ceWindowProperties.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Class ceWPTTreeModelListener
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPTTreeModelListener::ceWPTTreeModelListener(ceWPTTreeModel &model) :
pModel(model){
}

ceWPTTreeModelListener::~ceWPTTreeModelListener(){
}



// Management
///////////////

void ceWPTTreeModelListener::TargetStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::TargetChanged(ceConversation*, ceTarget*){
}



void ceWPTTreeModelListener::CameraShotStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::CameraShotChanged(ceConversation*, ceCameraShot*){
}



void ceWPTTreeModelListener::GestureStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::GestureChanged(ceConversation*, ceGesture*){
}



void ceWPTTreeModelListener::FacePoseStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::FacePoseChanged(ceConversation*, ceFacePose*){
}



void ceWPTTreeModelListener::FileStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::FileChanged(ceConversation*, ceConversationFile*){
}

void ceWPTTreeModelListener::ActiveFileChanged(ceConversation *conversation){
	if(conversation != pModel.GetConversation()){
		return;
	}
	
	pModel.UpdateActions();
	pModel.SelectTopicActive();
	
	pModel.GetForwardListener().ActiveFileChanged(conversation);
}



void ceWPTTreeModelListener::TopicStructureChanged(ceConversation*, ceConversationFile*){
}

void ceWPTTreeModelListener::TopicChanged(ceConversation*, ceConversationFile*, ceConversationTopic*){
}

void ceWPTTreeModelListener::ActiveTopicChanged(ceConversation *conversation, ceConversationFile *file){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()){
		return;
	}
	
	pModel.UpdateActions();
	pModel.SelectTopicActive();
	
	pModel.GetForwardListener().ActiveTopicChanged(conversation, file);
}



void ceWPTTreeModelListener::ActionStructureChanged(ceConversation *conversation,
ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic()){
		return;
	}
	
	if(action){
		ceWPTTIMAction * const model = pModel.DeepFindAction(action);
		if(model){
			model->Update();
		}
		
	}else{
		pModel.UpdateActions();
	}
	
	// the problem here is that listeners are run in any order. when we add items to the
	// tree the listener of the WPTopic is called but no model is yet assigned. this will
	// cause the WPTopic tree listener to miss out on an update. do it here to ensure
	// the WPTopic properly updates
	pModel.GetWindowMain().GetWindowProperties().GetPanelTopic().SelectActivePanel();
}

void ceWPTTreeModelListener::ActionChanged(ceConversation *conversation,
ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic()){
		return;
	}
	
	ceWPTTIMAction * const model = pModel.DeepFindAction(action);
	if(model){
		model->Update();
	}
}

void ceWPTTreeModelListener::ConditionStructureChanged(ceConversation *conversation,
ceConversationFile *file, ceConversationTopic *topic, ceConversationAction *action){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic()){
		return;
	}
	
	if(action){
		ceWPTTIMAction * const model = pModel.DeepFindAction(action);
		if(model){
			model->Update();
		}
		
	}else{
		pModel.UpdateActions();
	}
	
	// the problem here is that listeners are run in any order. when we add items to the
	// tree the listener of the WPTopic is called but no model is yet assigned. this will
	// cause the WPTopic tree listener to miss out on an update. do it here to ensure
	// the WPTopic properly updates
	pModel.GetWindowMain().GetWindowProperties().GetPanelTopic().SelectActivePanel();
}

void ceWPTTreeModelListener::ConditionChanged(ceConversation *conversation, ceConversationFile *file,
ceConversationTopic *topic, ceConversationAction*, ceConversationCondition *condition){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic()){
		return;
	}
	
	ceWPTTIMCondition * const model = pModel.DeepFindCondition(condition);
	if(model){
		model->Update();
	}
}

void ceWPTTreeModelListener::ActiveChanged(ceConversation *conversation,
ceConversationFile *file, ceConversationTopic *topic){
	if(conversation != pModel.GetConversation() || file != conversation->GetActiveFile()
	|| topic != file->GetActiveTopic() || ! pModel.GetTreeList() || pModel.GetPreventUpdate()){
		return;
	}
	
	if(topic->GetActiveCondition()){
		ceWPTTIMCondition * const condition = pModel.DeepFindCondition(topic->GetActiveCondition());
		if(condition){
			condition->SetAsCurrentItem();
			return;
		}
	}
	
	if(topic->GetActiveAction()){
		ceWPTTIMAction * const action = pModel.DeepFindAction(topic->GetActiveAction());
		if(action){
			action->SetAsCurrentItem();
			return;
		}
	}
	
	pModel.GetTreeList()->SetSelection(NULL);
}



void ceWPTTreeModelListener::ActorStructureChanged(ceConversation*){
}

void ceWPTTreeModelListener::ActorChanged(ceConversation*, ceConversationActor*){
}

void ceWPTTreeModelListener::LanguagePackChanged(ceConversation *conversation){
	if(conversation != pModel.GetConversation()){
		return;
	}
	
	pModel.UpdateActions();
}
