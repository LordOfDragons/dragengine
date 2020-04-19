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

#include "aeWPAPanelRuleStateManipulator.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleStateManipulator.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinPosition.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxPosition.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinRotation.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxRotation.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMinSize.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModMaxSize.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnablePos.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnableRot.h"
#include "../../../undosys/rule/statemanip/aeUSetRuleSModEnableSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
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

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleStateManipulator &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleStateManipulator &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = ( aeRuleStateManipulator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleStateManipulator *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = ( aeRuleStateManipulator* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleStateManipulator & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleStateManipulator &pPanel;
	
public:
	cBaseEditVectorListener( aeWPAPanelRuleStateManipulator &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleStateManipulator * const rule = ( aeRuleStateManipulator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator *animator, aeRuleStateManipulator *rule ) = 0;
};


class cEditPositionMinimum : public cBaseEditVectorListener{
public:
	cEditPositionMinimum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumPosition() )
			? new aeUSetRuleSModMinPosition( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditPositionMaximum : public cBaseEditVectorListener{
public:
	cEditPositionMaximum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumPosition() )
			? new aeUSetRuleSModMaxPosition( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditRotationMinimum : public cBaseEditVectorListener{
public:
	cEditRotationMinimum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumRotation() )
			? new aeUSetRuleSModMinRotation( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditRotationMaximum : public cBaseEditVectorListener{
public:
	cEditRotationMaximum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumRotation() )
			? new aeUSetRuleSModMaxRotation( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditScalingMinimum : public cBaseEditVectorListener{
public:
	cEditScalingMinimum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumSize() )
			? new aeUSetRuleSModMinSize( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditScalingMaximum : public cBaseEditVectorListener{
public:
	cEditScalingMaximum( aeWPAPanelRuleStateManipulator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleStateManipulator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumSize() )
			? new aeUSetRuleSModMaxSize( rule, editVector->GetVector() ) : NULL;
	}
};


class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleStateManipulator &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateManipulator *rule ){
		return new aeUSetRuleSModEnablePos( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateManipulator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleStateManipulator &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateManipulator *rule ){
		return new aeUSetRuleSModEnableRot( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateManipulator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleStateManipulator &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleStateManipulator *rule ){
		return new aeUSetRuleSModEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleStateManipulator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

}



// Class aeWPAPanelRuleStateManipulator
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleStateManipulator::aeWPAPanelRuleStateManipulator( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertStateManipulator )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "State Manipulator:" );
	
	helper.EditVector( groupBox, "Min Position:", "Minimum position",
		pEditMinPos, new cEditPositionMinimum( *this ) );
	helper.EditVector( groupBox, "Max Position:", "Maximum position",
		pEditMaxPos, new cEditPositionMaximum( *this ) );
	
	helper.EditVector( groupBox, "Min Rotation:", "Minimum rotation",
		pEditMinRot, new cEditRotationMinimum( *this ) );
	helper.EditVector( groupBox, "Max Rotation:", "Maximum rotation",
		pEditMaxRot, new cEditRotationMaximum( *this ) );
	
	helper.EditVector( groupBox, "Min Scaling:", "Minimum scaling",
		pEditMinSize, new cEditScalingMinimum( *this ) );
	helper.EditVector( groupBox, "Max Scaling:", "Maximum scaling",
		pEditMaxSize, new cEditScalingMaximum( *this ) );
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
}

aeWPAPanelRuleStateManipulator::~aeWPAPanelRuleStateManipulator(){
}



// Management
///////////////

void aeWPAPanelRuleStateManipulator::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleStateManipulator * const rule = ( aeRuleStateManipulator* )GetRule();
	
	if( rule ){
		pEditMinPos->SetVector( rule->GetMinimumPosition() );
		pEditMaxPos->SetVector( rule->GetMaximumPosition() );
		pEditMinRot->SetVector( rule->GetMinimumRotation() );
		pEditMaxRot->SetVector( rule->GetMaximumRotation() );
		pEditMinSize->SetVector( rule->GetMinimumSize() );
		pEditMaxSize->SetVector( rule->GetMaximumSize() );
		
	}else{
		pEditMinPos->SetVector( decVector() );
		pEditMaxPos->SetVector( decVector() );
		pEditMinRot->SetVector( decVector() );
		pEditMaxRot->SetVector( decVector() );
		pEditMinSize->SetVector( decVector() );
		pEditMaxSize->SetVector( decVector() );
	}
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
}

void aeWPAPanelRuleStateManipulator::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleStateManipulator * const rule = ( aeRuleStateManipulator* )GetRule();
	if( rule ){
		AddTarget( "Position", &rule->GetTargetPosition() );
		AddTarget( "Rotation ", &rule->GetTargetRotation() );
		AddTarget( "Size", &rule->GetTargetSize() );
	}
}
