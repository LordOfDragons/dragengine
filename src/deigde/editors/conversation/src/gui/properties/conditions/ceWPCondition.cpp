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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPCondition.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../conversation/actor/ceConversationActor.h"

#include <deigde/gui/igdeComboBox.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceWPCondition
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPCondition::ceWPCondition(ceWPTopic &parentPanel) :
igdeContainerForm(parentPanel.GetEnvironment()),
pParentPanel(parentPanel){
}

ceWPCondition::~ceWPCondition(){
}



// Management
///////////////

void ceWPCondition::UpdateComboBoxWithActorIDList(igdeComboBox &combobox){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection(combobox.GetText());
	
	combobox.RemoveAllItems();
	
	if(conversation){
		const ceConversationActorList &list = conversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			if(list.GetAt(i)->GetID().IsEmpty()){
				continue;
			}
			
			const decString &id = list.GetAt(i)->GetID();
			if(!combobox.HasItem(id)){
				combobox.AddItem(id);
			}
			
			const decString &aliasID = list.GetAt(i)->GetAliasID();
			if(!aliasID.IsEmpty() && !combobox.HasItem(aliasID)){
				combobox.AddItem(aliasID);
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText(selection);
}
