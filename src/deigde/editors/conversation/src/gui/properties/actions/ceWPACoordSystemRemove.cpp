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

#include "ceWPACoordSystemRemove.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCACoordSystemRemove.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/coordSystemRemove/ceUCACoordSysRemoveSetCoordSysID.h"

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
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboCoordSystemID : public igdeComboBoxListener {
	ceWPACoordSystemRemove &pPanel;
	
public:
	cComboCoordSystemID( ceWPACoordSystemRemove &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACoordSystemRemove * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetCoordSystemID() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACoordSysRemoveSetCoordSysID( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPACoordSystemRemove
/////////////////////////////////

// Constructor, destructor
////////////////////////////

ceWPACoordSystemRemove::ceWPACoordSystemRemove( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.ComboBox( *this, "Coord System:", true, "ID of the coordinate system to remove from conversation",
		pCBCoordSystemID, new cComboCoordSystemID( *this ) );
	pCBCoordSystemID->SetDefaultSorter();
}

ceWPACoordSystemRemove::~ceWPACoordSystemRemove(){
}



// Management
///////////////

ceCACoordSystemRemove *ceWPACoordSystemRemove::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetAction();
	
	if( action && action->GetType() == ceConversationAction::eatCoordSystemRemove ){
		return ( ceCACoordSystemRemove* )action;
		
	}else{
		return NULL;
	}
}

void ceWPACoordSystemRemove::UpdateAction(){
	const ceCACoordSystemRemove * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBCoordSystemID->SetText( action->GetCoordSystemID() );
		
	}else{
		pCBCoordSystemID->ClearText();
	}
}



void ceWPACoordSystemRemove::UpdateConvoCoordSysIDLists(){
	UpdateComboBoxWithConvoCoordSysIDList( pCBCoordSystemID );
}
