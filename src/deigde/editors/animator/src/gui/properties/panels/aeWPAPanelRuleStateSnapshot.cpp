/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeWPAPanelRuleStateSnapshot.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleStateSnapshot.h"
#include "../../../undosys/rule/snapshot/aeUSetRuleSnapUseLast.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapSetID.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnablePosition.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnableRotation.h"
#include "../../../undosys/rule/snapshot/aeURuleSnapToggleEnableSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleStateSnapshot &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleStateSnapshot &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = ( aeRuleStateSnapshot* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleStateSnapshot *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = ( aeRuleStateSnapshot* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleStateSnapshot & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
	aeWPAPanelRuleStateSnapshot &pPanel;
	
public:
	cBaseTextFieldListener( aeWPAPanelRuleStateSnapshot &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateSnapshot * const rule = ( aeRuleStateSnapshot* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator *animator, aeRuleStateSnapshot *rule ) = 0;
};


class cActionUseLastState : public cBaseAction{
public:
	cActionUseLastState( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel,
		"Use last animation state", NULL,
		"Determines if the animation state from the last frame update is used or the stored snapshot" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateSnapshot *rule ){
		return new aeUSetRuleSnapUseLast( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateSnapshot &rule ){
		SetEnabled( true );
		SetSelected( rule.GetUseLastState() );
	}
};

class cTextID : public cBaseTextFieldListener{
public:
	cTextID( aeWPAPanelRuleStateSnapshot &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleStateSnapshot *rule ){
		const int value = textField->GetInteger();
		return rule->GetID() != value ? new aeURuleSnapSetID( rule, value ) : NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateSnapshot *rule ){
		return new aeURuleSnapToggleEnablePosition( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateSnapshot &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateSnapshot *rule ){
		return new aeURuleSnapToggleEnableRotation( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateSnapshot &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateSnapshot *rule ){
		return new aeURuleSnapToggleEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateSnapshot &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

class cActionSnapshot : public cBaseAction{
public:
	cActionSnapshot( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel, "State", NULL,
		"Takes a snapshot of the current animation state" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleStateSnapshot *rule ){
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		if( engAnimatorInstance ){
			engAnimatorInstance->CaptureStateInto( rule->GetID() );
		}
		return NULL;
	}
};

class cActionGetFrame : public cBaseAction{
public:
	cActionGetFrame( aeWPAPanelRuleStateSnapshot &panel ) : cBaseAction( panel, "Animation", NULL,
		"Takes a snapshot of the selected animation frame" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleStateSnapshot *rule ){
		deAnimatorInstance * const engAnimatorInstance = animator->GetEngineAnimatorInstance();
		const decString moveName( pPanel.GetCBMoveNameText() );
		if( engAnimatorInstance ){
			engAnimatorInstance->StoreFrameInto( rule->GetID(), moveName, pPanel.GetTextMoveTime() );
		}
		return NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRuleStateSnapshot & ){
		SetEnabled( ! pPanel.GetCBMoveNameText().IsEmpty() );
	}
};

}



// Class aeWPAPanelRuleStateSnapshot
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleStateSnapshot::aeWPAPanelRuleStateSnapshot( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertStateSnapshot )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox, formLine;
	
	
	helper.GroupBox( *this, groupBox, "State Snapshot:" );
	
	helper.CheckBox( groupBox, pChkUseLastState, new cActionUseLastState( *this ), true );
	helper.EditInteger( groupBox, "ID:", "Identifier to snap state during runtime",
		pEditID, new cTextID( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Move Name:", true, "Name of animation move to use",
		pCBMoveName, NULL );
	pCBMoveName->SetDefaultSorter();
	
	helper.EditFloat( groupBox, "Move Time:", "Time in seconds of animation frame to retrieve",
		pEditMoveTime, NULL );
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
	
	helper.FormLine( groupBox, "Snapshot:", "Store snapshot (runtime simulation)", formLine );
	helper.Button( formLine, pBtnSnapshot, new cActionSnapshot( *this ), true );
	helper.Button( formLine, pBtnGetFrame, new cActionGetFrame( *this ), true );
}

aeWPAPanelRuleStateSnapshot::~aeWPAPanelRuleStateSnapshot(){
}



// Management
///////////////

void aeWPAPanelRuleStateSnapshot::UpdateAnimMoveList(){
	aeWPAPanelRule::UpdateAnimMoveList();
	
	const decString moveName( pCBMoveName->GetText() );
	
	pCBMoveName->RemoveAllItems();
	
	if( GetAnimator() ){
		const deAnimation * const engAnimation = GetAnimator()->GetEngineAnimator()
			? GetAnimator()->GetEngineAnimator()->GetAnimation() : NULL;
		if( engAnimation ){
			const int count = engAnimation->GetMoveCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBMoveName->AddItem( engAnimation->GetMove( i )->GetName() );
			}
		}
	}
	
	pCBMoveName->SortItems();
	pCBMoveName->StoreFilterItems();
	
	pCBMoveName->SetText( moveName );
}

void aeWPAPanelRuleStateSnapshot::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleStateSnapshot * const rule = ( aeRuleStateSnapshot* )GetRule();
	
	if( rule ){
		pEditID->SetInteger( rule->GetID() );
		
	}else{
		pEditID->ClearText();
	}
	
	const bool enabled = rule;
	pEditID->SetEnabled( enabled );
	pCBMoveName->SetEnabled( enabled );
	pEditMoveTime->SetEnabled( enabled );
	
	pChkUseLastState->GetAction()->Update();
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pBtnSnapshot->GetAction()->Update();
	pBtnGetFrame->GetAction()->Update();
}

const decString &aeWPAPanelRuleStateSnapshot::GetCBMoveNameText() const{
	return pCBMoveName->GetText();
}

float aeWPAPanelRuleStateSnapshot::GetTextMoveTime() const{
	return pEditMoveTime->GetFloat();
}
