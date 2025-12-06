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
#include <stdint.h>

#include "aeWPAPanelRuleBoneTransformator.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleBoneTransformator.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetAxis.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetMinAngle.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetMaxAngle.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetTransMin.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetTransMax.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetRotMin.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetRotMax.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetScaleMin.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetScaleMax.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetCFrame.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetEnablePos.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetEnableOrien.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetEnableSize.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetUseAxis.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetTargetBone.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetInputBone.h"
#include "../../../undosys/rule/bonetrans/aeURuleBTransSetInputSource.h"
#include "../../../animatoreditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleBoneTransformator &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleBoneTransformator &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnAction( animator, rule ) ));
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleBoneTransformator *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleBoneTransformator & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleBoneTransformator &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleBoneTransformator &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( comboBox, animator, rule ) ));
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleBoneTransformator *rule ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPAPanelRuleBoneTransformator &pPanel;
	
public:
	cBaseEditVectorListener( aeWPAPanelRuleBoneTransformator &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( editVector, animator, rule ) ));
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator *animator, aeRuleBoneTransformator *rule ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRuleBoneTransformator &pPanel;
	
public:
	cBaseTextFieldListener( aeWPAPanelRuleBoneTransformator &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New( OnChanged( textField, animator, rule ) ));
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator *animator, aeRuleBoneTransformator *rule ) = 0;
};


class cEditTranslationMinimum : public cBaseEditVectorListener{
public:
	cEditTranslationMinimum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumTranslation() )
			? new aeURuleBTransSetTransMin( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditTranslationMaximum : public cBaseEditVectorListener{
public:
	cEditTranslationMaximum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumTranslation() )
			? new aeURuleBTransSetTransMax( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditRotationMinimum : public cBaseEditVectorListener{
public:
	cEditRotationMinimum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumRotation() )
			? new aeURuleBTransSetRotMin( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditRotationMaximum : public cBaseEditVectorListener{
public:
	cEditRotationMaximum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumRotation() )
			? new aeURuleBTransSetRotMax( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditScalingMinimum : public cBaseEditVectorListener{
public:
	cEditScalingMinimum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMinimumScaling() )
			? new aeURuleBTransSetScaleMin( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditScalingMaximum : public cBaseEditVectorListener{
public:
	cEditScalingMaximum( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetMaximumScaling() )
			? new aeURuleBTransSetScaleMax( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditAxis : public cBaseEditVectorListener{
public:
	cEditAxis( aeWPAPanelRuleBoneTransformator &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector *editVector, aeAnimator*, aeRuleBoneTransformator *rule ){
		return ! editVector->GetVector().IsEqualTo( rule->GetAxis() )
			? new aeURuleBTransSetAxis( rule, editVector->GetVector() ) : NULL;
	}
};

class cEditMinimumAngle : public cBaseTextFieldListener{
public:
	cEditMinimumAngle( aeWPAPanelRuleBoneTransformator &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleBoneTransformator *rule ){
		const float value = textField->GetFloat();
		return fabsf( value - rule->GetMinimumAngle() ) > FLOAT_SAFE_EPSILON
			? new aeURuleBTransSetMinAngle( rule, value ) : nullptr;
	}
};

class cEditMaximumAngle : public cBaseTextFieldListener{
public:
	cEditMaximumAngle( aeWPAPanelRuleBoneTransformator &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRuleBoneTransformator *rule ){
		const float value = textField->GetFloat();
		return fabsf( value - rule->GetMaximumAngle() ) > FLOAT_SAFE_EPSILON
			? new aeURuleBTransSetMaxAngle( rule, value ) : nullptr;
	}
};

class cComboCoordFrame : public cBaseComboBoxListener{
public:
	cComboCoordFrame( aeWPAPanelRuleBoneTransformator &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleBoneTransformator *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleBoneTransformator::eCoordinateFrames value =
			( deAnimatorRuleBoneTransformator::eCoordinateFrames )( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetCoordinateFrame() != value ? new aeURuleBTransSetCFrame( rule, value ) : NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleBoneTransformator &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleBoneTransformator *rule ){
		return new aeURuleBTransSetEnablePos( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleBoneTransformator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleBoneTransformator &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleBoneTransformator *rule ){
		return new aeURuleBTransSetEnableOrien( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleBoneTransformator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleBoneTransformator &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleBoneTransformator *rule ){
		return new aeURuleBTransSetEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleBoneTransformator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

class cActionUseAxis : public cBaseAction{
public:
	cActionUseAxis( aeWPAPanelRuleBoneTransformator &panel ) : cBaseAction( panel,
		"Use rotation axis", NULL, "Use rotation axis instead of direct rotation" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleBoneTransformator *rule ){
		return new aeURuleBTransSetUseAxis( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleBoneTransformator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetUseAxis() );
	}
};

class cComboTargetBone : public cBaseComboBoxListener{
public:
	cComboTargetBone( aeWPAPanelRuleBoneTransformator &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleBoneTransformator *rule ){
		return comboBox->GetText() != rule->GetTargetBone()
			? new aeURuleBTransSetTargetBone( rule, comboBox->GetText() ) : NULL;
	}
};

class cComboInputBone : public cBaseComboBoxListener{
public:
	cComboInputBone(aeWPAPanelRuleBoneTransformator &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleBoneTransformator *rule) override{
		return comboBox->GetText() != rule->GetInputBone()
			? new aeURuleBTransSetInputBone(rule, comboBox->GetText()) : nullptr;
	}
};

class cComboInputSource : public cBaseComboBoxListener{
public:
	cComboInputSource(aeWPAPanelRuleBoneTransformator &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleBoneTransformator *rule) override{
		if(!comboBox->GetSelectedItem()){
			return nullptr;
		}
		
		deAnimatorRuleBoneTransformator::eInputSources value =
			(deAnimatorRuleBoneTransformator::eInputSources)(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetInputSource() != value ? new aeURuleBTransSetInputSource(rule, value) : nullptr;
	}
};

}



// Class aeWPAPanelRuleBoneTransformator
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleBoneTransformator::aeWPAPanelRuleBoneTransformator( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertBoneTransformator )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Bone Manipulator:" );
	
	helper.EditVector( groupBox, "Min Translation:", "Minimum translation",
		pEditMinTrans, new cEditTranslationMinimum( *this ) );
	helper.EditVector( groupBox, "Max Translation:", "Maximum translation",
		pEditMaxTrans, new cEditTranslationMaximum( *this ) );
	
	helper.EditVector( groupBox, "Min Rotation:", "Minimum rotation",
		pEditMinRot, new cEditRotationMinimum( *this ) );
	helper.EditVector( groupBox, "Max Rotation:", "Maximum rotation",
		pEditMaxRot, new cEditRotationMaximum( *this ) );
	
	helper.EditVector( groupBox, "Min Scaling:", "Minimum scaling",
		pEditMinScale, new cEditScalingMinimum( *this ) );
	helper.EditVector( groupBox, "Max Scaling:", "Maximum scaling",
		pEditMaxScale, new cEditScalingMaximum( *this ) );
	
	helper.CheckBox( groupBox, pChkUseAxis, new cActionUseAxis( *this ), true );
	helper.EditVector( groupBox, "Axis:", "Rotation axis", pEditAxis, new cEditAxis( *this ) );
	helper.EditFloat( groupBox, "Min Angle:", "Minimum axis rotation angle",
		pEditMinAngle, new cEditMinimumAngle( *this ) );
	helper.EditFloat( groupBox, "Max Angle:", "Maximum axis rotation angle",
		pEditMaxAngle, new cEditMaximumAngle( *this ) );
	
	helper.ComboBox( groupBox, "Coord-Frame:", "Sets the coordinate frame to use for rotation",
		pCBCoordFrame, new cComboCoordFrame( *this ) );
	pCBCoordFrame->AddItem( "Bone Local", NULL, ( void* )( intptr_t )deAnimatorRuleBoneTransformator::ecfBoneLocal );
	pCBCoordFrame->AddItem( "Component", NULL, ( void* )( intptr_t )deAnimatorRuleBoneTransformator::ecfComponent );
	pCBCoordFrame->AddItem( "Target Bone", NULL, ( void* )( intptr_t )deAnimatorRuleBoneTransformator::ecfTargetBone );
	
	helper.ComboBoxFilter( groupBox, "Target Bone:", true, "Set bone to use as coordinate frame",
		pCBTargetBone, new cComboTargetBone( *this ) );
	pCBTargetBone->SetDefaultSorter();
	
	helper.ComboBoxFilter(groupBox, "Input Bone:", true, "Set bone to use as input",
		pCBInputBone, new cComboInputBone(*this));
	pCBInputBone->SetDefaultSorter();
	
	helper.ComboBox(groupBox, "Input Source:", "Source to use for input.",
		pCBInputSource, new cComboInputSource(*this));
	pCBInputSource->AddItem("Target Blend", nullptr,
		(void*)(intptr_t)deAnimatorRuleBoneTransformator::eisTargetBlend);
	pCBInputSource->AddItem("Target Direct", nullptr,
		(void*)(intptr_t)deAnimatorRuleBoneTransformator::eisTargetDirect);
	pCBInputSource->AddItem("Bone State", nullptr,
		(void*)(intptr_t)deAnimatorRuleBoneTransformator::eisBoneState);
	pCBInputSource->AddItem("Bone State Inverse", nullptr,
		(void*)(intptr_t)deAnimatorRuleBoneTransformator::eisBoneStateInverse);
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
}

aeWPAPanelRuleBoneTransformator::~aeWPAPanelRuleBoneTransformator(){
}



// Management
///////////////

void aeWPAPanelRuleBoneTransformator::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selectionTargetBone(pCBTargetBone->GetText());
	const decString selectionInputBone(pCBInputBone->GetText());
	
	pCBTargetBone->RemoveAllItems();
	pCBInputBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				const decString &name = rig->GetBoneAt(i).GetName();
				pCBTargetBone->AddItem(name);
				pCBInputBone->AddItem(name);
			}
		}
		pCBTargetBone->SortItems();
		pCBInputBone->SortItems();
	}
	
	pCBTargetBone->StoreFilterItems();
	pCBTargetBone->SetText(selectionTargetBone);
	
	pCBInputBone->StoreFilterItems();
	pCBInputBone->SetText(selectionInputBone);
}

void aeWPAPanelRuleBoneTransformator::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )GetRule();
	
	if( rule ){
		pEditMinTrans->SetVector( rule->GetMinimumTranslation() );
		pEditMaxTrans->SetVector( rule->GetMaximumTranslation() );
		pEditMinRot->SetVector( rule->GetMinimumRotation() );
		pEditMaxRot->SetVector( rule->GetMaximumRotation() );
		pEditMinScale->SetVector( rule->GetMinimumScaling() );
		pEditMaxScale->SetVector( rule->GetMaximumScaling() );
		pEditAxis->SetVector( rule->GetAxis() );
		pEditMinAngle->SetFloat( rule->GetMinimumAngle() );
		pEditMaxAngle->SetFloat( rule->GetMaximumAngle() );
		pCBCoordFrame->SetSelectionWithData( ( void* )( intptr_t )rule->GetCoordinateFrame() );
		pCBTargetBone->SetText( rule->GetTargetBone() );
		pCBInputBone->SetText(rule->GetInputBone());
		pCBInputSource->SetSelectionWithData((void*)(intptr_t)rule->GetInputSource());
		
	}else{
		pEditMinTrans->SetVector( decVector() );
		pEditMaxTrans->SetVector( decVector() );
		pEditMinRot->SetVector( decVector() );
		pEditMaxRot->SetVector( decVector() );
		pEditMinScale->SetVector( decVector() );
		pEditMaxScale->SetVector( decVector() );
		pEditAxis->SetVector( decVector( 0.0f, 0.0f, 1.0f ) );
		pEditMinAngle->SetFloat( 0.0f );
		pEditMaxAngle->SetFloat( 0.0f );
		pCBCoordFrame->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleBoneTransformator::ecfComponent );
		pCBTargetBone->ClearText();
		pCBInputBone->ClearText();
		pCBInputSource->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleBoneTransformator::eisTargetBlend);
	}
	
	const bool enabled = rule;
	pEditMinTrans->SetEnabled( enabled );
	pEditMaxTrans->SetEnabled( enabled );
	pEditMinRot->SetEnabled( enabled );
	pEditMaxRot->SetEnabled( enabled );
	pEditMinScale->SetEnabled( enabled );
	pEditMaxScale->SetEnabled( enabled );
	pChkUseAxis->GetAction()->Update();
	pEditAxis->SetEnabled( enabled );
	pEditMinAngle->SetEnabled( enabled );
	pEditMaxAngle->SetEnabled( enabled );
	pCBCoordFrame->SetEnabled( enabled );
	pCBTargetBone->SetEnabled( enabled );
	pCBInputBone->SetEnabled(enabled);
	pCBInputSource->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
}

void aeWPAPanelRuleBoneTransformator::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleBoneTransformator * const rule = ( aeRuleBoneTransformator* )GetRule();
	if( rule ){
		AddTarget( "Translation", &rule->GetTargetTranslation() );
		AddTarget( "Rotation", &rule->GetTargetRotation() );
		AddTarget( "Scaling", &rule->GetTargetScaling() );
	}
}
