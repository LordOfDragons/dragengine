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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "aeWPAPanelRule.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animatoreditor.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/controller/aeControllerTarget.h"
#include "../../../animator/link/aeLink.h"
#include "../../../animator/rule/aeRule.h"
#include "../../../clipboard/aeClipboardDataBones.h"
#include "../../../undosys/rule/aeURuleMirrorBones.h"
#include "../../../undosys/rule/aeURuleTargetAddLink.h"
#include "../../../undosys/rule/aeURuleTargetRemoveLink.h"
#include "../../../undosys/rule/aeUSetRuleAddBone.h"
#include "../../../undosys/rule/aeUSetRuleBlendFactor.h"
#include "../../../undosys/rule/aeUSetRuleBlendMode.h"
#include "../../../undosys/rule/aeUSetRuleEnabled.h"
#include "../../../undosys/rule/aeUSetRuleName.h"
#include "../../../undosys/rule/aeUSetRuleRemoveBone.h"
#include "../../../undosys/rule/aeUSetRuleRemoveAllBones.h"
#include "../../../undosys/rule/aeUSetRuleBones.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseAction( aeWPAPanelRule &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRule *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRule & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseTextFieldListener( aeWPAPanelRule &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator *animator, aeRule *rule ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRule &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRule *rule ) = 0;
};


class cTextName : public cBaseTextFieldListener{
public:
	cTextName( aeWPAPanelRule &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRule *rule ){
		return textField->GetText() != rule->GetName() ? new aeUSetRuleName( rule, textField->GetText() ) : NULL;
	}
};

class cComboBlendMode : public cBaseComboBoxListener{
public:
	cComboBlendMode( aeWPAPanelRule &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRule *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		const deAnimatorRule::eBlendModes mode =
			( deAnimatorRule::eBlendModes )( intptr_t )comboBox->GetSelectedItem()->GetData();
		return mode != rule->GetBlendMode() ? new aeUSetRuleBlendMode( rule, mode ) : NULL;
	}
};

class cTextBlendFactor : public cBaseTextFieldListener{
public:
	cTextBlendFactor( aeWPAPanelRule &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, aeAnimator*, aeRule *rule ){
		const float factor = textField->GetFloat();
		return fabsf( factor - rule->GetBlendFactor() ) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleBlendFactor( rule, factor ) : NULL;
	}
};

class cActionEnabled : public cBaseAction{
public:
	cActionEnabled( aeWPAPanelRule &panel ) : cBaseAction( panel, "Enable Rule", NULL,
		"Determines if the rule is affecting animation" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		return new aeUSetRuleEnabled( rule );
	}
	
	virtual void Update( const aeAnimator &, const aeRule &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnabled() );
	}
};


class cActionBoneAdd : public cBaseAction{
public:
	cActionBoneAdd( aeWPAPanelRule &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add bone" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		const decString &name = pPanel.GetCBBoneText();
		return ! name.IsEmpty() && ! rule->GetListBones().Has( name )
			? new aeUSetRuleAddBone( rule, name ) : NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRule &rule ){
		const decString &name = pPanel.GetCBBoneText();
		SetEnabled( ! name.IsEmpty() && ! rule.GetListBones().Has( name ) );
	}
};

class cActionBoneRemove : public cBaseAction{
public:
	cActionBoneRemove( aeWPAPanelRule &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove bone" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		const decString &name = pPanel.GetCBBoneText();
		return ! name.IsEmpty() && rule->GetListBones().Has( name )
			? new aeUSetRuleRemoveBone( rule, name ) : NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRule &rule ){
		const decString &name = pPanel.GetCBBoneText();
		SetEnabled( ! name.IsEmpty() && rule.GetListBones().Has( name ) );
	}
};

class cActionBoneRemoveAll : public cBaseAction{
public:
	cActionBoneRemoveAll( aeWPAPanelRule &panel ) : cBaseAction( panel, "Remove All",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		return rule->GetListBones().GetCount() > 0
			? new aeUSetRuleRemoveAllBones( rule ) : NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRule &rule ){
		SetEnabled( rule.GetListBones().GetCount() > 0 );
	}
};

class cActionMirrorRigBones : public cBaseAction{
public:
	cActionMirrorRigBones( aeWPAPanelRule &panel ) : cBaseAction( panel, "Mirror Bones",
		NULL, "Mirror rig bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return new aeURuleMirrorBones( rule );
	}
	
	virtual void Update( const aeAnimator &, const aeRule &rule ){
		SetEnabled( rule.GetListBones().GetCount() > 0 );
	}
};

class cActionCopyBones : public cBaseAction{
public:
	cActionCopyBones( aeWPAPanelRule &panel ) : cBaseAction( panel, "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		igdeClipboardDataReference clip;
		clip.TakeOver( new aeClipboardDataBones( rule->GetListBones() ) );
		pPanel.GetWindowMain().GetClipboard().Set( clip );
		return NULL;
	}
};

class cActionPasteBones : public cBaseAction{
public:
	cActionPasteBones( aeWPAPanelRule &panel ) : cBaseAction( panel, "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		aeClipboardDataBones * const clip = ( aeClipboardDataBones* )pPanel.GetWindowMain()
			.GetClipboard().GetWithTypeName( aeClipboardDataBones::TYPE_NAME  );
		if( ! clip ){
			return NULL;
		}
		
		aeUSetRuleBones * const undo = new aeUSetRuleBones( rule, rule->GetListBones() + clip->GetBones() );
		undo->SetShortInfo( "Rule paste bones" );
		return undo;
	}
	
	virtual void Update( const aeAnimator &, const aeRule & ){
		SetEnabled( pPanel.GetWindowMain().GetClipboard().HasWithTypeName( aeClipboardDataBones::TYPE_NAME ) );
	}
};


class cListBones : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	cListBones( aeWPAPanelRule &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( pPanel.GetRule() && listBox->GetSelectedItem() ){
			pPanel.SetCBBoneText( listBox->GetSelectedItem()->GetText() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		if( ! pPanel.GetRule() ){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, new cActionBoneAdd( pPanel ), true );
		helper.MenuCommand( menu, new cActionBoneRemove( pPanel ), true );
		helper.MenuCommand( menu, new cActionBoneRemoveAll( pPanel ), true );
		helper.MenuCommand( menu, new cActionMirrorRigBones( pPanel ), true );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCopyBones( pPanel ), true );
		helper.MenuCommand( menu, new cActionPasteBones( pPanel ), true );
	}
};


class cComboTarget : public cBaseComboBoxListener{
public:
	cComboTarget( aeWPAPanelRule &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRule* ){
		pPanel.SetTarget( comboBox->GetSelectedItem()
			? ( aeControllerTarget* )comboBox->GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};


class cActionLinkAdd : public cBaseAction{
public:
	cActionLinkAdd( aeWPAPanelRule &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add link" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		return target && link && ! target->HasLink( link )
			? new aeURuleTargetAddLink( rule, target, link ) : NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRule & ){
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		SetEnabled( target && link && ! target->HasLink( link ) );
	}
};

class cActionLinkRemove : public cBaseAction{
public:
	cActionLinkRemove( aeWPAPanelRule &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove link" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		return target && link && target->HasLink( link )
			? new aeURuleTargetRemoveLink( rule, target, link ) : NULL;
	}
	
	virtual void Update( const aeAnimator &, const aeRule & ){
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		SetEnabled( target && link && target->HasLink( link ) );
	}
};


class cListLinks : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	cListLinks( aeWPAPanelRule &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( listBox->GetSelectedItem() ){
			pPanel.SetCBLinkSelection( ( aeLink* )listBox->GetSelectedItem()->GetData() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		if( ! pPanel.GetRule() ){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, new cActionLinkAdd( pPanel ), true );
		helper.MenuCommand( menu, new cActionLinkRemove( pPanel ), true );
	}
};

}



// Class aeWPAPanelRule
/////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRule::aeWPAPanelRule( aeWPRule &wpRule, deAnimatorRuleVisitorIdentify::eRuleTypes requiredType ) :
igdeContainerFlow( wpRule.GetEnvironment(), igdeContainerFlow::eaY ),
pWPRule( wpRule ),
pRequiredType( requiredType ),
pTarget( NULL )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox, formLine;
	
	
	// animation
	helper.GroupBox( *this, groupBox, "General Settings:" );
	
	helper.EditString( groupBox, "Name:", "Name of the rule", pEditName, new cTextName( *this ) );
	
	helper.ComboBox( groupBox, "Blend Mode:", "Sets the blend mode",
		pCBBlendMode, new cComboBlendMode( *this ) );
	pCBBlendMode->AddItem( "Blend", NULL, ( void* )( intptr_t )deAnimatorRule::ebmBlend );
	pCBBlendMode->AddItem( "Overlay", NULL, ( void* )( intptr_t )deAnimatorRule::ebmOverlay );
	
	helper.EditFloat( groupBox, "Blend Factor:", "Sets the blend factor",
		pEditBlendFactor, new cTextBlendFactor( *this ) );
	
	helper.CheckBox( groupBox, pChkEnabled, new cActionEnabled( *this ), true );
	
	
	// affected bones
	helper.GroupBoxFlow( *this, groupBox, "Affected bones:", false, true );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( formLine );
	helper.ComboBoxFilter( formLine, true, "Bones", pCBBones, NULL );
	pCBBones->SetDefaultSorter();
	helper.Button( formLine, pBtnBoneAdd, new cActionBoneAdd( *this ), true );
	helper.Button( formLine, pBtnBoneDel, new cActionBoneRemove( *this ), true );
	
	helper.ListBox( groupBox, 4, "Bones affected by rule", pListBones, new cListBones( *this ) );
	pListBones->SetDefaultSorter();
	
	
	// targets
	helper.GroupBoxFlow( *this, groupBox, "Targets and Links:" );
	
	formLine.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( formLine );
	helper.ComboBox( formLine, "Target:", "Targets", pCBTarget, new cComboTarget( *this ) );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( formLine );
	helper.ComboBoxFilter( formLine, "Links", pCBLinks, NULL );
	pCBLinks->SetDefaultSorter();
	helper.Button( formLine, pBtnLinkAdd, new cActionLinkAdd( *this ), true );
	
	helper.ListBox( groupBox, 4, "Links affecting target", pListLinks, new cListLinks( *this ) );
	pListLinks->SetDefaultSorter();
}

aeWPAPanelRule::~aeWPAPanelRule(){
}



// Management
///////////////

aeWindowMain &aeWPAPanelRule::GetWindowMain() const{
	return pWPRule.GetWindowProperties().GetWindowMain();
}

aeAnimator *aeWPAPanelRule::GetAnimator() const{
	return pWPRule.GetAnimator();
}

aeRule *aeWPAPanelRule::GetRule() const{
	aeRule * const rule = pWPRule.GetRule();
	return rule && rule->GetType() == pRequiredType ? rule : NULL;
}

void aeWPAPanelRule::SetTarget( aeControllerTarget *target ){
	if( target == pTarget ){
		return;
	}
	
	pTarget = target;
	UpdateTarget();
}



void aeWPAPanelRule::OnActivated(){
	SetTarget( NULL );
	
	UpdateRigBoneList();
	UpdateAnimMoveList();
	
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	if( pCBTarget->GetItemCount() > 0 ){
		pCBTarget->SetSelection( 0 );
	}
}

void aeWPAPanelRule::OnAnimatorChanged(){
	SetTarget( NULL );
}



void aeWPAPanelRule::UpdateControllerList(){
}

void aeWPAPanelRule::UpdateLinkList(){
	aeLink * const selection = GetCBLinkSelection();
	
	pCBLinks->RemoveAllItems();
	
	if( GetAnimator() ){
		const aeLinkList &list = GetAnimator()->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			aeLink * const link = list.GetAt( i );
			pCBLinks->AddItem( link->GetName(), NULL, link );
		}
		pCBLinks->SortItems();
	}
	
	pCBLinks->StoreFilterItems();
	pCBLinks->SetSelectionWithData( selection );
}

void aeWPAPanelRule::UpdateRigBoneList(){
	const decString selection( GetCBBoneText() );
	
	pCBBones->RemoveAllItems();
	
	if( GetAnimator() ){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			
			for( i=0; i<count; i++ ){
				pCBBones->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		pCBBones->SortItems();
	}
	
	pCBBones->StoreFilterItems();
	pCBBones->SetText( selection );
}

void aeWPAPanelRule::UpdateAnimMoveList(){
}

void aeWPAPanelRule::UpdateRule(){
	const aeRule * const rule = GetRule();
	
	if( rule ){
		// update rule
		pEditName->SetText( rule->GetName() );
		pCBBlendMode->SetSelectionWithData( ( void* )( intptr_t )rule->GetBlendMode() );
		pEditBlendFactor->SetFloat( rule->GetBlendFactor() );
		
		// update affected bone list
		const decStringSet &boneList = rule->GetListBones();
		const int boneCount = boneList.GetCount();
		int i;
		
		pListBones->RemoveAllItems();
		for( i=0; i<boneCount; i++ ){
			pListBones->AddItem( boneList.GetAt( i ) );
		}
		pListBones->SortItems();
		
	}else{
		pEditName->ClearText();
		pCBBlendMode->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRule::ebmBlend );
		pEditBlendFactor->ClearText();
		pListBones->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pEditName->SetEnabled( enabled );
	pCBBlendMode->SetEnabled( enabled );
	pEditBlendFactor->SetEnabled( enabled );
	pListBones->SetEnabled( enabled );
	
	pChkEnabled->GetAction()->Update();
	pBtnBoneAdd->GetAction()->Update();
	pBtnBoneDel->GetAction()->Update();
	
	UpdateTarget();
}

void aeWPAPanelRule::UpdateTargetList(){
	pCBTarget->RemoveAllItems();
	if( GetRule() ){
		AddTarget( "Blend Factor", &GetRule()->GetTargetBlendFactor() );
	}
}

void aeWPAPanelRule::UpdateTarget(){
	if( pTarget ){
		const int linkCount = pTarget->GetLinkCount();
		int i;
		
		pListLinks->RemoveAllItems();
		for( i=0; i<linkCount; i++ ){
			aeLink * const link = pTarget->GetLinkAt( i );
			pListLinks->AddItem( link->GetName(), NULL, link );
		}
		pListLinks->SortItems();
		
	}else{
		pListLinks->RemoveAllItems();
	}
	
	pListLinks->SetEnabled( pTarget );
	
	pBtnLinkAdd->GetAction()->Update();
}

const decString &aeWPAPanelRule::GetCBBoneText() const{
	return pCBBones->GetText();
}

void aeWPAPanelRule::SetCBBoneText( const char *text ){
	pCBBones->SetText( text );
}

aeLink *aeWPAPanelRule::GetCBLinkSelection() const{
	return pCBLinks->GetSelectedItem() ? ( aeLink* )pCBLinks->GetSelectedItem()->GetData() : NULL;
}

void aeWPAPanelRule::SetCBLinkSelection( aeLink *selection ){
	pCBLinks->SetSelectionWithData( selection );
}



void aeWPAPanelRule::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void aeWPAPanelRule::AddTarget( const char *name, aeControllerTarget *target ){
	pCBTarget->AddItem( name, NULL, target );
}
