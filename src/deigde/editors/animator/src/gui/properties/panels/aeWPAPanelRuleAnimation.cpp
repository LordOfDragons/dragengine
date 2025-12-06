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

#include "aeWPAPanelRuleAnimation.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleAnimation.h"
#include "../../../undosys/rule/animation/aeUSetRuleAnimMoveName.h"
#include "../../../undosys/rule/animation/aeUSetRuleAnimMoveTime.h"
#include "../../../undosys/rule/animation/aeURuleAnimToggleEnablePosition.h"
#include "../../../undosys/rule/animation/aeURuleAnimToggleEnableRotation.h"
#include "../../../undosys/rule/animation/aeURuleAnimToggleEnableSize.h"
#include "../../../undosys/rule/animation/aeURuleAnimToggleEnableVertexPositionSet.h"
#include "../../../animatoreditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleAnimation &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleAnimation &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimation * const rule = ( aeRuleAnimation* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnAction( animator, rule ) ));
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleAnimation *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimation * const rule = ( aeRuleAnimation* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleAnimation & ){
		SetEnabled( true );
		SetSelected( false );
	}
};


class cComboMoveName : public igdeComboBoxListener{
	aeWPAPanelRuleAnimation &pPanel;
	
public:
	cComboMoveName( aeWPAPanelRuleAnimation &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeRuleAnimation * const rule = ( aeRuleAnimation* )pPanel.GetRule();
		if( ! rule || rule->GetMoveName() == comboBox->GetText() ){
			return;
		}
		
		aeUSetRuleAnimMoveName::Ref undo(aeUSetRuleAnimMoveName::Ref::New(
			new aeUSetRuleAnimMoveName( rule, comboBox->GetText() ) ));
		pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
	}
};

class cTextMoveTime : public igdeTextFieldListener{
	aeWPAPanelRuleAnimation &pPanel;
	
public:
	cTextMoveTime( aeWPAPanelRuleAnimation &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeRuleAnimation * const rule = ( aeRuleAnimation* )pPanel.GetRule();
		const float value = textField->GetFloat();
		if( ! rule || fabsf( rule->GetMoveTime() - value ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		aeUSetRuleAnimMoveTime::Ref undo(aeUSetRuleAnimMoveTime::Ref::New(
			new aeUSetRuleAnimMoveTime( rule, value ) ));
		pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleAnimation &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimation *rule ){
		return new aeURuleAnimToggleEnablePosition( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimation &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleAnimation &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimation *rule ){
		return new aeURuleAnimToggleEnableRotation( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimation &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleAnimation &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimation *rule ){
		return new aeURuleAnimToggleEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimation &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	cActionEnableVertexPositionSet( aeWPAPanelRuleAnimation &panel ) : cBaseAction( panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimation *rule ){
		return new aeURuleAnimToggleEnableVertexPositionSet( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimation &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableVertexPositionSet() );
	}
};

}



// Class aeWPAPanelRuleAnimation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleAnimation::aeWPAPanelRuleAnimation( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertAnimation )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Animation:" );
	
	helper.ComboBoxFilter( groupBox, "Move Name:", true, "Name of animation move to use",
		pCBMoveName, new cComboMoveName( *this ) );
	pCBMoveName->SetDefaultSorter();
	
	helper.EditFloat( groupBox, "Move Time:", "Time relative to move playtime to retrieve",
		pEditMoveTime, new cTextMoveTime( *this ) );
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
	helper.CheckBox( groupBox, pChkEnableVertexPositionSet, new cActionEnableVertexPositionSet( *this ), true );
}

aeWPAPanelRuleAnimation::~aeWPAPanelRuleAnimation(){
}



// Management
///////////////

void aeWPAPanelRuleAnimation::UpdateAnimMoveList(){
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

void aeWPAPanelRuleAnimation::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleAnimation * const rule = ( aeRuleAnimation* )GetRule();
	
	if( rule ){
		pCBMoveName->SetText( rule->GetMoveName() );
		pEditMoveTime->SetFloat( rule->GetMoveTime() );
		
	}else{
		pCBMoveName->ClearText();
		pEditMoveTime->ClearText();
	}
	
	const bool enabled = rule;
	pCBMoveName->SetEnabled( enabled );
	pEditMoveTime->SetEnabled( enabled );
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleAnimation::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleAnimation * const rule = ( aeRuleAnimation* )GetRule();
	if( rule ){
		AddTarget( "Move Time", &rule->GetTargetMoveTime() );
	}
}
