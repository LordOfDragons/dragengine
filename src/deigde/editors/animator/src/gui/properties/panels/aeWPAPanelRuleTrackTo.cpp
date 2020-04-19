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

#include "aeWPAPanelRuleTrackTo.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleTrackTo.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetLockedAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetTrackAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetTrackBone.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetUpAxis.h"
#include "../../../undosys/rule/trackto/aeURuleTrackToSetUpTarget.h"
#include "../../../animatoreditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleTrackTo &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleTrackTo &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleTrackTo * const rule = ( aeRuleTrackTo* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleTrackTo *rule ) = 0;
};


class cComboTrackBone : public cBaseComboBoxListener{
public:
	cComboTrackBone( aeWPAPanelRuleTrackTo &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule ){
		return rule->GetTrackBone() != comboBox->GetText()
			? new aeURuleTrackToSetTrackBone( rule, comboBox->GetText() ) : NULL;
	}
};

class cComboTrackAxis : public cBaseComboBoxListener{
public:
	cComboTrackAxis( aeWPAPanelRuleTrackTo &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleTrackTo::eTrackAxis value = ( deAnimatorRuleTrackTo::eTrackAxis )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetTrackAxis() != value ? new aeURuleTrackToSetTrackAxis( rule, value ) : NULL;
	}
};

class cComboUpAxis : public cBaseComboBoxListener{
public:
	cComboUpAxis( aeWPAPanelRuleTrackTo &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleTrackTo::eTrackAxis value = ( deAnimatorRuleTrackTo::eTrackAxis )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetUpAxis() != value ? new aeURuleTrackToSetUpAxis( rule, value ) : NULL;
	}
};

class cComboUpTarget : public cBaseComboBoxListener{
public:
	cComboUpTarget( aeWPAPanelRuleTrackTo &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleTrackTo::eUpTarget value = ( deAnimatorRuleTrackTo::eUpTarget )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetUpTarget() != value ? new aeURuleTrackToSetUpTarget( rule, value ) : NULL;
	}
};

class cComboLockedAxis : public cBaseComboBoxListener{
public:
	cComboLockedAxis( aeWPAPanelRuleTrackTo &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleTrackTo *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleTrackTo::eLockedAxis value = ( deAnimatorRuleTrackTo::eLockedAxis )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetLockedAxis() != value ? new aeURuleTrackToSetLockedAxis( rule, value ) : NULL;
	}
};

}



// Class aeWPAPanelRuleTrackTo
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleTrackTo::aeWPAPanelRuleTrackTo( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertTrackTo )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Track To:" );
	
	helper.ComboBoxFilter( groupBox, "Track Bone:", true, "Name of the bone to track to",
		pCBTrackBone, new cComboTrackBone( *this ) );
	pCBTrackBone->SetDefaultSorter();
	
	helper.ComboBox( groupBox, "Track Axis:", "Axis to point at the target",
		pCBTrackAxis, new cComboTrackAxis( *this ) );
	pCBTrackAxis->AddItem( "Positive X", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosX );
	pCBTrackAxis->AddItem( "Positive Y", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosY );
	pCBTrackAxis->AddItem( "Positive Z", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosZ );
	pCBTrackAxis->AddItem( "Negative X", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegX );
	pCBTrackAxis->AddItem( "Negative Y", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegY );
	pCBTrackAxis->AddItem( "Negative Z", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegZ );
	
	helper.ComboBox( groupBox, "Up Axis:", "Up axis to align with the target up axis",
		pCBUpAxis, new cComboUpAxis( *this ) );
	pCBUpAxis->AddItem( "Positive X", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosX );
	pCBUpAxis->AddItem( "Positive Y", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosY );
	pCBUpAxis->AddItem( "Positive Z", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosZ );
	pCBUpAxis->AddItem( "Negative X", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegX );
	pCBUpAxis->AddItem( "Negative Y", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegY );
	pCBUpAxis->AddItem( "Negative Z", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::etaNegZ );
	
	helper.ComboBox( groupBox, "Up Target:", "Target axis to align the up axis with",
		pCBUpTarget, new cComboUpTarget( *this ) );
	pCBUpTarget->AddItem( "World X-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutWorldX );
	pCBUpTarget->AddItem( "World Y-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutWorldY );
	pCBUpTarget->AddItem( "World Z-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutWorldZ );
	pCBUpTarget->AddItem( "Component X-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutComponentX );
	pCBUpTarget->AddItem( "Component Y-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutComponentY );
	pCBUpTarget->AddItem( "Component Z-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutComponentZ );
	pCBUpTarget->AddItem( "Track Bone X-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutTrackBoneX );
	pCBUpTarget->AddItem( "Track Bone Y-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutTrackBoneY );
	pCBUpTarget->AddItem( "Track Bone Z-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutTrackBoneZ );
	pCBUpTarget->AddItem( "Controller", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::eutController );
	
	helper.ComboBox( groupBox, "Lock Axis:", "Axis to lock",
		pCBLockedAxis, new cComboLockedAxis( *this ) );
	pCBLockedAxis->AddItem( "None", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::elaNone );
	pCBLockedAxis->AddItem( "X-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::elaX );
	pCBLockedAxis->AddItem( "Y-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::elaY );
	pCBLockedAxis->AddItem( "Z-Axis", NULL, ( void* )( intptr_t )deAnimatorRuleTrackTo::elaZ );
}

aeWPAPanelRuleTrackTo::~aeWPAPanelRuleTrackTo(){
}



// Management
///////////////

void aeWPAPanelRuleTrackTo::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection( pCBTrackBone->GetText() );
	
	pCBTrackBone->RemoveAllItems();
	
	if( GetAnimator() ){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBTrackBone->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		pCBTrackBone->SortItems();
	}
	
	pCBTrackBone->StoreFilterItems();
	pCBTrackBone->SetText( selection );
}

void aeWPAPanelRuleTrackTo::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleTrackTo * const rule = ( aeRuleTrackTo* )GetRule();
	
	if( rule ){
		pCBTrackBone->SetText( rule->GetTrackBone() );
		pCBTrackAxis->SetSelectionWithData( ( void* )( intptr_t )rule->GetTrackAxis() );
		pCBUpAxis->SetSelectionWithData( ( void* )( intptr_t )rule->GetUpAxis() );
		pCBUpTarget->SetSelectionWithData( ( void* )( intptr_t )rule->GetUpTarget() );
		pCBLockedAxis->SetSelectionWithData( ( void* )( intptr_t )rule->GetLockedAxis() );
		
	}else{
		pCBTrackBone->ClearText();
		pCBTrackAxis->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosZ );
		pCBUpAxis->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleTrackTo::etaPosY );
		pCBUpTarget->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleTrackTo::eutComponentY );
		pCBLockedAxis->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleTrackTo::elaNone );
	}
	
	const bool enabled = rule;
	pCBTrackBone->SetEnabled( enabled );
	pCBTrackAxis->SetEnabled( enabled );
	pCBUpAxis->SetEnabled( enabled );
	pCBUpTarget->SetEnabled( enabled );
	pCBLockedAxis->SetEnabled( enabled );
}

void aeWPAPanelRuleTrackTo::UpdateTargetList(){
	aeRuleTrackTo *rule = ( aeRuleTrackTo* )GetRule();
	
	aeWPAPanelRule::UpdateTargetList();
	
	if( rule ){
		AddTarget( "Position", &rule->GetTargetPosition() );
		AddTarget( "Up", &rule->GetTargetUp() );
	}
}
