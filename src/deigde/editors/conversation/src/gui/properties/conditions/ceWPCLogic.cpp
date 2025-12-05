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
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
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
		ceConversationAction * const action = pPanel.GetParentPanel().GetTreeAction();
		ceCConditionLogic * const condition = pPanel.GetCondition();
		if( ! topic || ! action || ! condition || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const ceCConditionLogic::eOperators newOperator =
			( ceCConditionLogic::eOperators )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( newOperator == condition->GetOperator() ){
			return;
		}
		
		igdeUndo::Ref undo;
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
	igdeContainer::Ref formLine;
	
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
	ceConversationCondition * const condition = pParentPanel.GetTreeCondition();
	
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
