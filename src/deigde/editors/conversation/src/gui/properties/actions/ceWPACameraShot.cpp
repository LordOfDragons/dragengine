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

#include "ceWPACameraShot.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCACameraShot.h"
#include "../../../conversation/actor/ceConversationActor.h"
#include "../../../conversation/camerashot/ceCameraShot.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/target/ceTarget.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/cameraShot/ceUCACameraShotSetName.h"
#include "../../../undosys/action/cameraShot/ceUCACShotSetDuration.h"
#include "../../../undosys/action/cameraShot/ceUCACShotSetCameraTarget.h"
#include "../../../undosys/action/cameraShot/ceUCACShotSetLookAtTarget.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBoxFilter.h>
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

class cComboName : public igdeComboBoxListener {
	ceWPACameraShot &pPanel;
	
public:
	cComboName( ceWPACameraShot &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACameraShot * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetName() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACameraShotSetName( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cTextDuration : public igdeTextFieldListener {
	ceWPACameraShot &pPanel;
	
public:
	cTextDuration( ceWPACameraShot &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACameraShot * const action = pPanel.GetAction();
		const float duration = textField->GetFloat();
		if( ! topic || ! action || fabsf( duration - action->GetDuration() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACShotSetDuration( topic, action, duration ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboCameraTarget : public igdeComboBoxListener {
	ceWPACameraShot &pPanel;
	
public:
	cComboCameraTarget( ceWPACameraShot &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACameraShot * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetCameraTarget() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACShotSetCameraTarget( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboLookAtTarget : public igdeComboBoxListener {
	ceWPACameraShot &pPanel;
	
public:
	cComboLookAtTarget( ceWPACameraShot &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCACameraShot * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetLookAtTarget() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACShotSetLookAtTarget( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPACameraShot
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPACameraShot::ceWPACameraShot( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	CreateGUICommon( *this );
	
	helper.ComboBoxFilter( *this, "Name:", true, "Name of the camera shot to use or empty to not change",
		pCBName, new cComboName( *this ) );
	pCBName->SetDefaultSorter();
	
	helper.EditFloat( *this, "Duration:", "Duration of the camera shot",
		pEditDuration, new cTextDuration( *this ) );
	
	helper.ComboBoxFilter( *this, "Camera Target:", true, "Target the camera is attached to",
		pCBCameraTarget, new cComboCameraTarget( *this ) );
	pCBCameraTarget->SetDefaultSorter();
	
	helper.ComboBoxFilter( *this, "Look-At Target:", true, "Target to point the camera at",
		pCBLookAtTarget, new cComboLookAtTarget( *this ) );
	pCBLookAtTarget->SetDefaultSorter();
}

ceWPACameraShot::~ceWPACameraShot(){
}



// Management
///////////////

ceCACameraShot *ceWPACameraShot::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatCameraShot ){
		return ( ceCACameraShot* )action;
		
	}else{
		return NULL;
	}
}

void ceWPACameraShot::UpdateAction(){
	const ceCACameraShot * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBName->SetText( action->GetName() );
		pEditDuration->SetFloat( action->GetDuration() );
		pCBCameraTarget->SetText( action->GetCameraTarget() );
		pCBLookAtTarget->SetText( action->GetLookAtTarget() );
		
	}else{
		pCBName->ClearText();
		pEditDuration->ClearText();
		pCBCameraTarget->ClearText();
		pCBLookAtTarget->ClearText();
	}
}

void ceWPACameraShot::UpdateCameraShotList(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	const decString selection( pCBName->GetText() );
	
	pCBName->RemoveAllItems();
	
	if( conversation ){
		const ceCameraShotList &list = conversation->GetCameraShotList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBName->AddItem( list.GetAt( i )->GetName() );
		}
		
		pCBName->SortItems();
	}
	
	pCBName->StoreFilterItems();
	pCBName->SetText( selection );
}

void ceWPACameraShot::UpdateTargetList(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	const decString selectionCamera( pCBCameraTarget->GetText() );
	const decString selectionLookAt( pCBLookAtTarget->GetText() );
	
	pCBCameraTarget->RemoveAllItems();
	pCBLookAtTarget->RemoveAllItems();
	
	if( conversation ){
		const ceTargetList &list = conversation->GetTargetList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( ! list.GetAt( i )->GetName().IsEmpty() ){
				pCBCameraTarget->AddItem( list.GetAt( i )->GetName() );
				pCBLookAtTarget->AddItem( list.GetAt( i )->GetName() );
			}
		}
		
		pCBCameraTarget->SortItems();
		pCBLookAtTarget->SortItems();
	}
	
	pCBCameraTarget->StoreFilterItems();
	pCBLookAtTarget->StoreFilterItems();
	
	pCBCameraTarget->SetText( selectionCamera );
	pCBLookAtTarget->SetText( selectionLookAt );
}
