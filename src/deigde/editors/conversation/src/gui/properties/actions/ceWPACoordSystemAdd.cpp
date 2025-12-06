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

#include "ceWPACoordSystemAdd.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCACoordSystemAdd.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/coordSystemAdd/ceUCACoordSysAddSetCoordSysID.h"
#include "../../../undosys/action/coordSystemAdd/ceUCACoordSysAddSetAliasID.h"

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
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboCoordSystemID : public igdeComboBoxListener {
	ceWPACoordSystemAdd &pPanel;
	
public:
	cComboCoordSystemID( ceWPACoordSystemAdd &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACoordSystemAdd * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetCoordSystemID() ){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCACoordSysAddSetCoordSysID::Ref::NewWith(topic, action, comboBox->GetText()));
	}
};

class cComboAliasID : public igdeComboBoxListener {
	ceWPACoordSystemAdd &pPanel;
	
public:
	cComboAliasID( ceWPACoordSystemAdd &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACoordSystemAdd * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetAliasID() ){
			return;
		}
		
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(
			ceUCACoordSysAddSetAliasID::Ref::NewWith(topic, action, comboBox->GetText()));
	}
};

}



// Class ceWPACoordSystemAdd
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPACoordSystemAdd::ceWPACoordSystemAdd( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.ComboBox( *this, "Coord System:", true, "ID of the coordinate system to add to conversation",
		pCBCoordSystemID, new cComboCoordSystemID( *this ) );
	pCBCoordSystemID->SetDefaultSorter();
	
	helper.ComboBox( *this, "Alias:", true, "Optional alias ID to set for the added coordinate system",
		pCBAliasID, new cComboAliasID( *this ) );
	pCBAliasID->SetDefaultSorter();
}

ceWPACoordSystemAdd::~ceWPACoordSystemAdd(){
}



// Management
///////////////

ceCACoordSystemAdd *ceWPACoordSystemAdd::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatCoordSystemAdd ){
		return ( ceCACoordSystemAdd* )action;
		
	}else{
		return NULL;
	}
}

void ceWPACoordSystemAdd::UpdateAction(){
	const ceCACoordSystemAdd * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBCoordSystemID->SetText( action->GetCoordSystemID() );
		pCBAliasID->SetText( action->GetAliasID() );
		
	}else{
		pCBCoordSystemID->ClearText();
		pCBAliasID->ClearText();
	}
}



void ceWPACoordSystemAdd::UpdateConvoCoordSysIDLists(){
	UpdateComboBoxWithConvoCoordSysIDList( pCBCoordSystemID );
	UpdateComboBoxWithConvoCoordSysIDList( pCBAliasID );
}
