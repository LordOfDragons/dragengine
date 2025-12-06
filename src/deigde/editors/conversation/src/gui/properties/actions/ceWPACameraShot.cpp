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
		
		ceUCACameraShotSetName::Ref undo(ceUCACameraShotSetName::Ref::NewWith(
			topic, action, comboBox->GetText()));
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
		
		ceUCACShotSetDuration::Ref undo(ceUCACShotSetDuration::Ref::NewWith(topic, action, duration));
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
		
		ceUCACShotSetCameraTarget::Ref undo(ceUCACShotSetCameraTarget::Ref::NewWith(
			topic, action, comboBox->GetText()));
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
		
		ceUCACShotSetLookAtTarget::Ref undo(ceUCACShotSetLookAtTarget::Ref::NewWith(
			topic, action, comboBox->GetText()));
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
		const ceCameraShotList list( conversation->AllCameraShots() );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBName->AddItem( list.GetAt( i )->GetName() );
		}
		
		pCBName->SortItems();
		pCBName->StoreFilterItems();
	}
	
	pCBName->SetText( selection );
}

void ceWPACameraShot::UpdateTargetList(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	const decString selectionCamera( pCBCameraTarget->GetText() );
	const decString selectionLookAt( pCBLookAtTarget->GetText() );
	
	pCBCameraTarget->RemoveAllItems();
	pCBLookAtTarget->RemoveAllItems();
	
	if( conversation ){
		const ceTargetList list( conversation->AllTargets() );
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
		
		pCBCameraTarget->StoreFilterItems();
		pCBLookAtTarget->StoreFilterItems();
	}
	
	pCBCameraTarget->SetText( selectionCamera );
	pCBLookAtTarget->SetText( selectionLookAt );
}
