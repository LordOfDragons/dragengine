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

#include "aeWPAPanelRuleInverseKinematic.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleInverseKinematic.h"
#include "../../../undosys/rule/ik/aeURuleIKSetGoalPosition.h"
#include "../../../undosys/rule/ik/aeURuleIKSetGoalRotation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetLocalPosition.h"
#include "../../../undosys/rule/ik/aeURuleIKSetLocalRotation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetAdjustOrientation.h"
#include "../../../undosys/rule/ik/aeURuleIKSetSolverBone.h"
#include "../../../undosys/rule/ik/aeURuleIKSetUseSolverBone.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachCenter.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachRange.h"
#include "../../../undosys/rule/ik/aeURuleIKSetReachBone.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleInverseKinematic &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleInverseKinematic *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleInverseKinematic & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleInverseKinematic &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleInverseKinematic *rule ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	cBaseTextFieldListener( aeWPAPanelRuleInverseKinematic &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator *animator, aeRuleInverseKinematic *rule ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleInverseKinematic &pPanel;
	
public:
	cBaseEditVectorListener( aeWPAPanelRuleInverseKinematic &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator *animator, aeRuleInverseKinematic *rule ) = 0;
};


class cEditGoalPosition : public cBaseEditVectorListener{
public:
	cEditGoalPosition( aeWPAPanelRuleInverseKinematic &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetGoalPosition() )
			? new aeURuleIKSetGoalPosition( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditGoalRotation : public cBaseEditVectorListener{
public:
	cEditGoalRotation( aeWPAPanelRuleInverseKinematic &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetGoalOrientation() )
			? new aeURuleIKSetGoalRotation( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditLocalPosition : public cBaseEditVectorListener{
public:
	cEditLocalPosition( aeWPAPanelRuleInverseKinematic &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetLocalPosition() )
			? new aeURuleIKSetLocalPosition( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditLocalRotation : public cBaseEditVectorListener{
public:
	cEditLocalRotation( aeWPAPanelRuleInverseKinematic &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetLocalOrientation() )
			? new aeURuleIKSetLocalRotation( rule, editVector->GetVector() ) : NULL;
	}
};

class cActionAdjustRotation : public cBaseAction{
public:
	cActionAdjustRotation( aeWPAPanelRuleInverseKinematic &panel ) : cBaseAction( panel,
		"Adjust Orientation", NULL, "Determines if the bone orientation is adjusted" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleInverseKinematic *rule ){
		return new aeURuleIKSetAdjustOrientation( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleInverseKinematic &rule ){
		SetEnabled( true );
		SetSelected( rule.GetAdjustOrientation() );
	}
};

class cComboSolverBone : public cBaseComboBoxListener{
public:
	cComboSolverBone( aeWPAPanelRuleInverseKinematic &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleInverseKinematic *rule ){
		return comboBox->GetText() != rule->GetSolverBone()
			? new aeURuleIKSetSolverBone( rule, comboBox->GetText() ) : NULL;
	}
};

class cActionUseSolverBone : public cBaseAction{
public:
	cActionUseSolverBone( aeWPAPanelRuleInverseKinematic &panel ) : cBaseAction( panel,
		"Use solver bone", NULL, "Determines if the solver bone is used as reference "
		"coordinate system or the component itself" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleInverseKinematic *rule ){
		return new aeURuleIKSetUseSolverBone( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleInverseKinematic &rule ){
		SetEnabled( true );
		SetSelected( rule.GetUseSolverBone() );
	}
};

class cTextReachRange : public cBaseTextFieldListener{
public:
	cTextReachRange( aeWPAPanelRuleInverseKinematic &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleInverseKinematic *rule ){
		const float value = textField->GetFloat();
		return fabsf( rule->GetReachRange() - value ) > FLOAT_SAFE_EPSILON
			? new aeURuleIKSetReachRange( rule, value ) : NULL;
	}
};

class cComboReachBone : public cBaseComboBoxListener{
public:
	cComboReachBone( aeWPAPanelRuleInverseKinematic &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleInverseKinematic *rule ){
		return comboBox->GetText() != rule->GetReachBone()
			? new aeURuleIKSetReachBone( rule, comboBox->GetText() ) : NULL;
	}
};

class cEditReachCenter : public cBaseEditVectorListener{
public:
	cEditReachCenter( aeWPAPanelRuleInverseKinematic &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleInverseKinematic *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetReachCenter() )
			? new aeURuleIKSetReachCenter( rule, editVector->GetVector() ) : NULL;
	}
};

}



// Class aeWPAPanelRuleInverseKinematic
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleInverseKinematic::aeWPAPanelRuleInverseKinematic( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertInverseKinematic )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Inverse Kinematic:" );
	
	helper.EditVector( groupBox, "Goal Position:", "Sets the position to reach",
		pEditGoalPos, new cEditGoalPosition( *this ) );
	helper.EditVector( groupBox, "Goal Orientation:", "Sets the orientation to reach",
		pEditGoalRot, new cEditGoalRotation( *this ) );
	helper.CheckBox( groupBox, pChkAdjustRotation, new cActionAdjustRotation( *this ), true );
	
	helper.EditVector( groupBox, "Local Position:",
		"Sets the goal position relative to the bone local coordinate system",
		pEditLocalPos, new cEditLocalPosition( *this ) );
	helper.EditVector( groupBox, "Local Orientation:",
		"Sets the goal orientation relative to the bone local coordinate system",
		pEditLocalRot, new cEditLocalRotation( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Solver Bone:", true,
		"Sets the solver bone to use as reference coordinate system",
		pCBSolverBone, new cComboSolverBone( *this ) );
	pCBSolverBone->SetDefaultSorter();
	helper.CheckBox( groupBox, pChkUseSolverBone, new cActionUseSolverBone( *this ), true );
	
	
	// reach
	helper.GroupBox( *this, groupBox, "Reach:", true );
	
	helper.EditFloat( groupBox, "Range:",
		"Maximum allowed range for IK target to be located from the reach center",
		pEditReachRange, new cTextReachRange( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Bone:", true,
		"Bone to use as center for reach calculation or empty string to use constant center",
		pCBReachBone, new cComboReachBone( *this ) );
	pCBReachBone->SetDefaultSorter();
	
	helper.EditVector( groupBox, "Center:", "Center to use for reach calculation if bone is not set",
		pEditReachCenter, new cEditReachCenter( *this ) );
}

aeWPAPanelRuleInverseKinematic::~aeWPAPanelRuleInverseKinematic(){
}



// Management
///////////////

void aeWPAPanelRuleInverseKinematic::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selectionSolver( pCBSolverBone->GetText() );
	const decString selectionReach( pCBReachBone->GetText() );
	
	pCBSolverBone->RemoveAllItems();
	pCBReachBone->RemoveAllItems();
	
	if( GetAnimator() ){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBSolverBone->AddItem( rig->GetBoneAt( i ).GetName() );
				pCBReachBone->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		pCBSolverBone->SortItems();
		pCBReachBone->SortItems();
	}
	
	pCBSolverBone->StoreFilterItems();
	pCBSolverBone->SetText( selectionSolver );
	
	pCBReachBone->StoreFilterItems();
	pCBReachBone->SetText( selectionReach );
}

void aeWPAPanelRuleInverseKinematic::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )GetRule();
	
	if( rule ){
		pEditGoalPos->SetVector( rule->GetGoalPosition() );
		pEditGoalRot->SetVector( rule->GetGoalOrientation() );
		pEditLocalPos->SetVector( rule->GetLocalPosition() );
		pEditLocalRot->SetVector( rule->GetLocalOrientation() );
		pCBSolverBone->SetText( rule->GetSolverBone() );
		pEditReachRange->SetFloat( rule->GetReachRange() );
		pEditReachCenter->SetVector( rule->GetReachCenter() );
		pCBReachBone->SetText( rule->GetReachBone() );
		
	}else{
		pEditGoalPos->SetVector( decVector() );
		pEditGoalRot->SetVector( decVector() );
		pEditLocalPos->SetVector( decVector() );
		pEditLocalRot->SetVector( decVector() );
		pCBSolverBone->ClearText();
		pEditReachRange->ClearText();
		pEditReachCenter->SetVector( decVector() );
		pCBReachBone->ClearText();
	}
	
	const bool enabled = rule;
	pEditGoalPos->SetEnabled( enabled );
	pEditGoalRot->SetEnabled( enabled );
	pEditLocalPos->SetEnabled( enabled );
	pEditLocalRot->SetEnabled( enabled );
	pCBSolverBone->SetEnabled( enabled );
	pEditReachRange->SetEnabled( enabled );
	pEditReachCenter->SetEnabled( enabled );
	pCBReachBone->SetEnabled( enabled );
	
	pChkAdjustRotation->GetAction()->Update();
	pChkUseSolverBone->GetAction()->Update();
}

void aeWPAPanelRuleInverseKinematic::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleInverseKinematic * const rule = ( aeRuleInverseKinematic* )GetRule();
	if( rule ){
		AddTarget( "Goal Position", &rule->GetTargetGoalPosition() );
		AddTarget( "Goal Orientation", &rule->GetTargetGoalOrientation() );
		AddTarget( "Local Position", &rule->GetTargetLocalPosition() );
		AddTarget( "Local Orientation", &rule->GetTargetLocalOrientation() );
		AddTarget( "Reach Range", &rule->GetTargetReachRange() );
		AddTarget( "Reach Center", &rule->GetTargetReachCenter() );
	}
}
