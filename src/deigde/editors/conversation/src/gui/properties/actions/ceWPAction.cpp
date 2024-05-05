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

#include "ceWPAction.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/coordsystem/ceCoordSystem.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/ceUCActionToggleWaitForActor.h"
#include "../../../undosys/action/ceUCActionSetWaitForActorID.h"
#include "../../../undosys/action/ceUCActionSetDelay.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextDelay : public igdeTextFieldListener {
	ceWPAction &pPanel;
	
public:
	cTextDelay( ceWPAction &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if( ! topic || ! action ){
			return;
		}
		
		const float delay = textField->GetFloat();
		if( fabsf( delay - action->GetDelay() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCActionSetDelay( topic, action, delay ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionWaitForActor : public igdeAction{
	ceWPAction &pPanel;
	
public:
	cActionWaitForActor( ceWPAction &panel ) : igdeAction( "Wait for actor",
		"Wait for actor(s) to finish speaking and return false if asked to wait" ),
		pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if( ! topic || ! action ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCActionToggleWaitForActor( topic, action ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		SetEnabled( action );
		SetSelected( action && action->GetWaitForActor() );
	}
};

class cComboWaitForActorID : public igdeComboBoxListener {
	ceWPAction &pPanel;
	
public:
	cComboWaitForActorID( ceWPAction &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		if( ! topic || ! action || comboBox->GetText() == action->GetWaitForActorID() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCActionSetWaitForActorID( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPAction
/////////////////////

// Constructor, destructor
////////////////////////////

ceWPAction::ceWPAction( ceWPTopic &parentPanel ) :
igdeContainerForm( parentPanel.GetEnvironment() ),
pParentPanel( parentPanel ){
}

ceWPAction::~ceWPAction(){
}



// Management
///////////////

void ceWPAction::CreateGUICommon( igdeContainerForm &container ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	helper.EditFloat( container, "Delay:",
		"Delay the processing of the action after waiting conditions are fulfilled",
		pEditDelay, new cTextDelay( *this ) );
	
	helper.CheckBoxOnly( container, pChkWaitForActor, new cActionWaitForActor( *this ), true );
	helper.ComboBox( container, true, "ID of actor to wait for or empty string to wait for all actors",
		pCBWaitForActorID, new cComboWaitForActorID( *this ) );
}

void ceWPAction::UpdateCommonParams(){
	if( ! pEditDelay ){
		return;
	}
	
	const ceConversationAction * const action = pParentPanel.GetTreeAction();
	
	if( action ){
		pEditDelay->SetFloat( action->GetDelay() );
		pCBWaitForActorID->SetText( action->GetWaitForActorID() );
		
	}else{
		pEditDelay->ClearText();
		pCBWaitForActorID->ClearText();
	}
	
	pChkWaitForActor->GetAction()->Update();
}



void ceWPAction::UpdateComboBoxWithActorIDList( igdeComboBox &combobox ){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection( combobox.GetText() );
	
	combobox.RemoveAllItems();
	
	if( conversation ){
		const ceConversationActorList &list = conversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( ! list.GetAt( i )->GetID().IsEmpty() ){
				const decString &id = list.GetAt( i )->GetID();
				const decString &aliasID = list.GetAt( i )->GetAliasID();
				
				if( ! combobox.HasItem( id ) ){
					combobox.AddItem( id );
				}
				if( ! aliasID.IsEmpty() && ! combobox.HasItem( aliasID ) ){
					combobox.AddItem( aliasID );
				}
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText( selection );
}

void ceWPAction::UpdateComboBoxWithConvoCoordSysIDList( igdeComboBox &combobox ){
	const ceConversation * const conversation = pParentPanel.GetConversation();
	const decString selection( combobox.GetText() );
	
	combobox.RemoveAllItems();
	
	if( conversation ){
		const ceCoordSystemList &list = conversation->GetCoordSystemList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( ! list.GetAt( i )->GetID().IsEmpty() ){
				const decString &id = list.GetAt( i )->GetID();
				const decString &aliasID = list.GetAt( i )->GetAliasID();
				
				if( ! combobox.HasItem( id ) ){
					combobox.AddItem( id );
				}
				if( ! aliasID.IsEmpty() && ! combobox.HasItem( aliasID ) ){
					combobox.AddItem( aliasID );
				}
			}
		}
		
		combobox.SortItems();
	}
	
	combobox.SetText( selection );
}

void ceWPAction::UpdateActorIDLists(){
	// empty on purpose since we do not use the common properties of the base class
}

void ceWPAction::OnConversationPathChanged(){
}
