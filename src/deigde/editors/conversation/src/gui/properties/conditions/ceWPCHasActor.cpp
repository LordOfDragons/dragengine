/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPCHasActor.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/condition/ceCConditionHasActor.h"
#include "../../../undosys/condition/hasActor/ceUCCHasActorSetActor.h"
#include "../../../undosys/condition/hasActor/ceUCCHasActorToggleNegate.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboActor : public igdeComboBoxListener {
	ceWPCHasActor &pPanel;
	
public:
	cComboActor( ceWPCHasActor &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionHasActor * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || comboBox->GetText() == condition->GetActor() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCHasActorSetActor( topic, action, condition, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionNegate : public igdeAction {
	ceWPCHasActor &pPanel;
	
public:
	cActionNegate( ceWPCHasActor &panel ) : igdeAction( "Negate", NULL,
		"True if the information is missing instead of existing" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionHasActor * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCHasActorToggleNegate( topic, action, condition ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPCHasActor
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPCHasActor::ceWPCHasActor( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	helper.ComboBox( *this, "Actor ID:", true, "Actor ID to test", pCBActor, new cComboActor( *this ) );
	pCBActor->SetDefaultSorter();
		
	helper.CheckBox( *this, pChkNegate, new cActionNegate( *this ), true );
}

ceWPCHasActor::~ceWPCHasActor(){
}



// Management
///////////////

ceCConditionHasActor *ceWPCHasActor::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectHasActor ){
		return ( ceCConditionHasActor* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCHasActor::UpdateCondition(){
	const ceCConditionHasActor * const condition = GetCondition();
	
	if( condition ){
		pCBActor->SetText( condition->GetActor() );
		pChkNegate->SetChecked( condition->GetNegate() );
		
	}else{
		pCBActor->ClearText();
		pChkNegate->SetChecked( false );
	}
}

void ceWPCHasActor::UpdateActorList(){
	UpdateComboBoxWithActorIDList( pCBActor );
}
