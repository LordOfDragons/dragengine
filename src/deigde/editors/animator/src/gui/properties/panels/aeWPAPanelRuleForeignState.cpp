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

#include "aeWPAPanelRuleForeignState.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleForeignState.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaBone.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaPosition.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaRotation.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaSize.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnablePos.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnableRot.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaEnableSize.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaSrcCFrame.h"
#include "../../../undosys/rule/foreignstate/aeUSetRuleFStaDestCFrame.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
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
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleForeignState &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = ( aeRuleForeignState* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleForeignState *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = ( aeRuleForeignState* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleForeignState & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleForeignState &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = ( aeRuleForeignState* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleForeignState *rule ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleForeignState &pPanel;
	
public:
	cBaseTextFieldListener( aeWPAPanelRuleForeignState &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleForeignState * const rule = ( aeRuleForeignState* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator *animator, aeRuleForeignState *rule ) = 0;
};


class cComboBone : public cBaseComboBoxListener{
public:
	cComboBone( aeWPAPanelRuleForeignState &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule ){
		return rule->GetForeignBone() != comboBox->GetText()
			? new aeUSetRuleFStaBone( rule, comboBox->GetText() ) : NULL;
	}
};

class cComboCoordFrameSource : public cBaseComboBoxListener{
public:
	cComboCoordFrameSource( aeWPAPanelRuleForeignState &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleForeignState::eCoordinateFrames value =
			( deAnimatorRuleForeignState::eCoordinateFrames )( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetSourceCoordinateFrame() != value ? new aeUSetRuleFStaSrcCFrame( rule, value ) : NULL;
	}
};

class cComboCoordFrameDestination : public cBaseComboBoxListener{
public:
	cComboCoordFrameDestination( aeWPAPanelRuleForeignState &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleForeignState *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleForeignState::eCoordinateFrames value =
			( deAnimatorRuleForeignState::eCoordinateFrames )( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetDestCoordinateFrame() != value ? new aeUSetRuleFStaDestCFrame( rule, value ) : NULL;
	}
};

class cTextScalePosition : public cBaseTextFieldListener{
public:
	cTextScalePosition( aeWPAPanelRuleForeignState &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule ){
		const float value = textField->GetFloat();
		return fabsf( rule->GetScalePosition() - value ) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleFStaPosition( rule, value ) : NULL;
	}
};

class cTextScaleRotation : public cBaseTextFieldListener{
public:
	cTextScaleRotation( aeWPAPanelRuleForeignState &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule ){
		const float value = textField->GetFloat();
		return fabsf( rule->GetScaleOrientation() - value ) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleFStaRotation( rule, value ) : NULL;
	}
};

class cTextScaleSize : public cBaseTextFieldListener{
public:
	cTextScaleSize( aeWPAPanelRuleForeignState &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleForeignState *rule ){
		const float value = textField->GetFloat();
		return fabsf( rule->GetScaleSize() - value ) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleFStaSize( rule, value ) : NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleForeignState &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleForeignState *rule ){
		return new aeUSetRuleFStaEnablePos( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleForeignState &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleForeignState &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleForeignState *rule ){
		return new aeUSetRuleFStaEnableRot( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleForeignState &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleForeignState &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleForeignState *rule ){
		return new aeUSetRuleFStaEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleForeignState &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

}



// Class aeWPAPanelRuleForeignState
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleForeignState::aeWPAPanelRuleForeignState( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertForeignState )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Foreign State:" );
	
	helper.ComboBoxFilter( groupBox, "Bone:", true, "Sets the bone to retrieve the state from",
		pCBBone, new cComboBone( *this ) );
	pCBBone->SetDefaultSorter();
	
	helper.EditFloat( groupBox, "Scale Position:", "Scaling applied to the foreign position",
		pEditPosition, new cTextScalePosition( *this ) );
	helper.EditFloat( groupBox, "Scale Rotation:", "Scaling applied to the foreign orientation",
		pEditRotation, new cTextScaleRotation( *this ) );
	helper.EditFloat( groupBox, "Scale Size:", "Scaling applied to the foreign size",
		pEditSize, new cTextScaleSize( *this ) );
	
	helper.ComboBox( groupBox, "Src. Coord-Frame:", "Sets the source coordinate frame to use",
		pCBSrcCFrame, new cComboCoordFrameSource( *this ) );
	pCBSrcCFrame->AddItem( "Bone Local", NULL, ( void* )( intptr_t )deAnimatorRuleForeignState::ecfBoneLocal );
	pCBSrcCFrame->AddItem( "Component", NULL, ( void* )( intptr_t )deAnimatorRuleForeignState::ecfComponent );
	
	helper.ComboBox( groupBox, "Dest. Coord-Frame:", "Sets the destination coordinate frame to use",
		pCBDestCFrame, new cComboCoordFrameDestination( *this ) );
	pCBDestCFrame->AddItem( "Bone Local", NULL, ( void* )( intptr_t )deAnimatorRuleForeignState::ecfBoneLocal );
	pCBDestCFrame->AddItem( "Component", NULL, ( void* )( intptr_t )deAnimatorRuleForeignState::ecfComponent );
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
}

aeWPAPanelRuleForeignState::~aeWPAPanelRuleForeignState(){
}



// Management
///////////////

void aeWPAPanelRuleForeignState::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection( pCBBone->GetText() );
	
	pCBBone->RemoveAllItems();
	
	if( GetAnimator() ){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBBone->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		pCBBone->SortItems();
	}
	
	pCBBone->StoreFilterItems();
	pCBBone->SetText( selection );
}

void aeWPAPanelRuleForeignState::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleForeignState * const rule = ( aeRuleForeignState* )GetRule();
	
	if( rule ){
		pCBBone->SetText( rule->GetForeignBone() );
		pEditPosition->SetFloat( rule->GetScalePosition() );
		pEditRotation->SetFloat( rule->GetScaleOrientation() );
		pEditSize->SetFloat( rule->GetScaleSize() );
		pCBSrcCFrame->SetSelectionWithData( ( void* )( intptr_t )rule->GetSourceCoordinateFrame() );
		pCBDestCFrame->SetSelectionWithData( ( void* )( intptr_t )rule->GetDestCoordinateFrame() );
		
	}else{
		pCBBone->ClearText();
		pEditPosition->ClearText();
		pEditRotation->ClearText();
		pEditSize->ClearText();
		pCBSrcCFrame->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleForeignState::ecfComponent );
		pCBDestCFrame->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleForeignState::ecfComponent );
	}
	
	const bool enabled = rule;
	pCBBone->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditSize->SetEnabled( enabled );
	pCBSrcCFrame->SetEnabled( enabled );
	pCBDestCFrame->SetEnabled( enabled );
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
}

void aeWPAPanelRuleForeignState::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleForeignState * const rule = ( aeRuleForeignState* )GetRule();
	if( rule ){
		AddTarget( "Position", &rule->GetTargetPosition() );
		AddTarget( "Orientation", &rule->GetTargetOrientation() );
		AddTarget( "Size", &rule->GetTargetSize() );
	}
}
