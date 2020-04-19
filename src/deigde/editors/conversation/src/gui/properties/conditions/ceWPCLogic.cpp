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

#include "ceWPCLogic.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/condition/ceCConditionLogic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/condition/logic/ceUCCLogicSetOperator.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

class cComboOperator : public igdeComboBoxListener {
	ceWPCLogic &pPanel;
	
public:
	cComboOperator( ceWPCLogic &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceConversationAction * const action = pPanel.GetParentPanel().GetAction();
		ceCConditionLogic * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionLogic::eOperators newOperator =
			( ceCConditionLogic::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == condition->GetOperator() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCCLogicSetOperator( topic, action, condition, newOperator ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};



// Class ceWPCLogic
/////////////////////

// Constructor, destructor
////////////////////////////

ceWPCLogic::ceWPCLogic( ceWPTopic &parentPanel ) : ceWPCondition( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainerReference formLine;
	
	helper.ComboBox( *this, "Operator:", "Logic to apply to the conditions",
		pCBOperator, new cComboOperator( *this ) );
	pCBOperator->AddItem( "None", NULL, ( void* )( intptr_t )ceCConditionLogic::eopNone );
	pCBOperator->AddItem( "Any", NULL, ( void* )( intptr_t )ceCConditionLogic::eopAny );
	pCBOperator->AddItem( "All", NULL, ( void* )( intptr_t )ceCConditionLogic::eopAll );
}

ceWPCLogic::~ceWPCLogic(){
}



// Management
///////////////

ceCConditionLogic *ceWPCLogic::GetCondition() const{
	ceConversationCondition * const condition = pParentPanel.GetCondition();
	
	if( condition && condition->GetType() == ceConversationCondition::ectLogic ){
		return ( ceCConditionLogic* )condition;
		
	}else{
		return NULL;
	}
}

void ceWPCLogic::UpdateCondition(){
	const ceCConditionLogic * const condition = GetCondition();
	
	if( condition ){
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )condition->GetOperator() );
		
	}else{
		pCBOperator->SetSelectionWithData( ( void* )( intptr_t )ceCConditionLogic::eopAll );
	}
}
