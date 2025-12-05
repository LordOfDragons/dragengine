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

#include "aeWPAPanelRuleMirror.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../dialog/aeDialogMirrorMatchName.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleMirror.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorAddMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorRemoveAllMatchNames.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorRemoveMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetAxis.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnablePosition.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableRotation.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableSize.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableVertexPositionSet.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetMirrorBone.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleMirror &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleMirror &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = ( aeRuleMirror* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleMirror *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = ( aeRuleMirror* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleMirror & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleMirror &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleMirror &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = ( aeRuleMirror* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleMirror *rule ) = 0;
};


class cComboMirrorAxis : public cBaseComboBoxListener{
public:
	cComboMirrorAxis( aeWPAPanelRuleMirror &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleMirror *rule ){
		if( ! comboBox->GetSelectedItem() ){
			return NULL;
		}
		
		deAnimatorRuleMirror::eMirrorAxis value = ( deAnimatorRuleMirror::eMirrorAxis )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		return rule->GetMirrorAxis() != value ? new aeURuleMirrorSetAxis( rule, value ) : nullptr;
	}
};

class cComboMirrorBone : public cBaseComboBoxListener{
public:
	cComboMirrorBone( aeWPAPanelRuleMirror &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator*, aeRuleMirror *rule ){
		return rule->GetMirrorBone() != comboBox->GetText()
			? new aeURuleMirrorSetMirrorBone( rule, comboBox->GetText() ) : nullptr;
	}
};


class cActionMatchNameAdd : public cBaseAction{
public:
	cActionMatchNameAdd( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel, "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add match name" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		igdeDialog::Ref dialog;
		dialog.TakeOver( new aeDialogMirrorMatchName( pPanel.GetEnvironment(), "Add match name" ) );
		return dialog->Run( &pPanel ) ? new aeURuleMirrorAddMatchName( rule, 
			( ( aeDialogMirrorMatchName& )( igdeDialog& )dialog ).CreateMatchName() ) : nullptr;
	}
};

class cActionMatchNameAddTemplate : public cBaseAction{
	const decString pFirst;
	const decString pSecond;
	const deAnimatorRuleMirror::eMatchNameType pType;
public:
	cActionMatchNameAddTemplate( aeWPAPanelRuleMirror &panel, const char *first, const char *second,
		deAnimatorRuleMirror::eMatchNameType type ) :
	cBaseAction( panel, CreateText( first, second, type ), panel.GetEnvironment().GetStockIcon(
		igdeEnvironment::esiPlus ), "Add match name" ),
	pFirst( first ), pSecond( second ), pType( type ){}
	
	static decString CreateText( const char *first, const char *second, deAnimatorRuleMirror::eMatchNameType type ){
		if( ! first || ! second ){
			DETHROW( deeNullPointer );
		}
		
		const char *typeStr;
		decString text;
		
		switch( type ){
		case deAnimatorRuleMirror::emntFirst:
			typeStr = "Begin";
			break;
			
		case deAnimatorRuleMirror::emntLast:
			typeStr = "End";
			break;
			
		case deAnimatorRuleMirror::emntMiddle:
			typeStr = "Middle";
			break;
			
		default:
			typeStr = "?";
		}
		
		text.Format( "Add: %s '%s' <-> '%s'", typeStr, first, second );
		return text;
	}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return new aeURuleMirrorAddMatchName( rule, aeRuleMirror::cMatchName::Ref::New(
			new aeRuleMirror::cMatchName( pFirst, pSecond, pType ) ) );
	}
};

class cActionMatchNameEdit : public cBaseAction{
public:
	cActionMatchNameEdit( aeWPAPanelRuleMirror &panel ) :
	cBaseAction( panel, "Edit...", nullptr, "Edit match name" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		aeRuleMirror::cMatchName * const matchName = pPanel.GetSelectedMatchBone();
		if( ! matchName ){
			return nullptr;
		}
		
		igdeDialog::Ref dialog;
		dialog.TakeOver( new aeDialogMirrorMatchName( pPanel.GetEnvironment(), "Edit match name" ) );
		( ( aeDialogMirrorMatchName& )( igdeDialog& )dialog ).Set( *matchName );
		if( ! dialog->Run( &pPanel ) ){
			return nullptr;
		}
		
		const aeRuleMirror::cMatchName::Ref edited(
			( ( aeDialogMirrorMatchName& )( igdeDialog& )dialog ).CreateMatchName() );
		
		return edited->operator!=( *matchName ) ? new aeURuleMirrorSetMatchName( rule,
			rule->IndexOfMatchName( matchName ), edited ) : nullptr;
	}
	
	virtual void Update( const aeAnimator &, const aeRuleMirror & ){
		SetEnabled( pPanel.GetSelectedMatchBone() );
	}
};

class cActionMatchNameRemove : public cBaseAction{
public:
	cActionMatchNameRemove( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel, "Remove...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove match name" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		aeRuleMirror::cMatchName * const matchName = pPanel.GetSelectedMatchBone();
		return matchName ? new aeURuleMirrorRemoveMatchName( rule, matchName ) : nullptr;
	}
	
	virtual void Update( const aeAnimator &, const aeRuleMirror & ){
		SetEnabled( pPanel.GetSelectedMatchBone() );
	}
};

class cActionMatchNameRemoveAll : public cBaseAction{
public:
	cActionMatchNameRemoveAll( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel, "Remove All",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all match names" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return rule->GetMatchNameCount() > 0 ? new aeURuleMirrorRemoveAllMatchNames( rule ) : nullptr;
	}
	
	virtual void Update( const aeAnimator &, const aeRuleMirror &rule ){
		SetEnabled( rule.GetMatchNameCount() > 0 );
	}
};

class cListMatchNames : public igdeListBoxListener{
	aeWPAPanelRuleMirror &pPanel;
	
public:
	cListMatchNames( aeWPAPanelRuleMirror &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox*, int ){
		igdeAction::Ref action;
		action.TakeOver( new cActionMatchNameEdit( pPanel ) );
		action->OnAction();
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		if( ! pPanel.GetRule() ){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, new cActionMatchNameAdd( pPanel ), true );
		helper.MenuCommand( menu, new cActionMatchNameAddTemplate( pPanel,
			".l", ".r", deAnimatorRuleMirror::emntLast ), true );
		helper.MenuCommand( menu, new cActionMatchNameAddTemplate( pPanel,
			".L", ".R", deAnimatorRuleMirror::emntLast ), true );
		helper.MenuCommand( menu, new cActionMatchNameAddTemplate( pPanel,
			"Left", "Right", deAnimatorRuleMirror::emntMiddle ), true );
		helper.MenuCommand( menu, new cActionMatchNameAddTemplate( pPanel,
			"LEFT", "RIGHT", deAnimatorRuleMirror::emntMiddle ), true );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionMatchNameEdit( pPanel ), true );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionMatchNameRemove( pPanel ), true );
		helper.MenuCommand( menu, new cActionMatchNameRemoveAll( pPanel ), true );
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel,
		"Enable position manipulation", nullptr,
		"Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return new aeURuleMirrorSetEnablePosition( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleMirror &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", nullptr,
		"Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return new aeURuleMirrorSetEnableRotation( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleMirror &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel,
		"Enable size manipulation", nullptr,
		"Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return new aeURuleMirrorSetEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleMirror &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	cActionEnableVertexPositionSet( aeWPAPanelRuleMirror &panel ) : cBaseAction( panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if the vertex position set is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleMirror *rule ){
		return new aeURuleMirrorSetEnableVertexPositionSet( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleMirror &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableVertexPositionSet() );
	}
};

}



// Class aeWPAPanelRuleMirror
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleMirror::aeWPAPanelRuleMirror( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertMirror )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Mirror:" );
	
	helper.ComboBox( groupBox, "Axis:", "Mirror axis", pCBMirrorAxis, new cComboMirrorAxis( *this ) );
	pCBMirrorAxis->AddItem( "X", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emaX );
	pCBMirrorAxis->AddItem( "Y", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emaY );
	pCBMirrorAxis->AddItem( "Z", nullptr, ( void* )( intptr_t )deAnimatorRuleMirror::emaZ );
	
	helper.ComboBoxFilter( groupBox, "Bone:", true, "Mirror bone or empty string to use component",
		pCBMirrorBone, new cComboMirrorBone( *this ) );
	pCBMirrorBone->SetDefaultSorter();
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
	helper.CheckBox( groupBox, pChkEnableVertexPositionSet, new cActionEnableVertexPositionSet( *this ), true );
	
	helper.ListBox( groupBox, "Pairs:", 3, "Match bone pairs by name.",
		pListMatchName, new cListMatchNames( *this ) );
}

aeWPAPanelRuleMirror::~aeWPAPanelRuleMirror(){
}



// Management
///////////////

aeRuleMirror::cMatchName *aeWPAPanelRuleMirror::GetSelectedMatchBone() const{
	return pListMatchName->GetSelectedItem()
		? ( aeRuleMirror::cMatchName* )pListMatchName->GetSelectedItem()->GetData() : nullptr;
}

void aeWPAPanelRuleMirror::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection( pCBMirrorBone->GetText() );
	
	pCBMirrorBone->RemoveAllItems();
	
	if( GetAnimator() ){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBMirrorBone->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		pCBMirrorBone->SortItems();
	}
	
	pCBMirrorBone->StoreFilterItems();
	pCBMirrorBone->SetText( selection );
}

void aeWPAPanelRuleMirror::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleMirror * const rule = ( aeRuleMirror* )GetRule();
	
	if( rule ){
		pCBMirrorAxis->SetSelectionWithData( ( void* )( intptr_t )rule->GetMirrorAxis() );
		pCBMirrorBone->SetText( rule->GetMirrorBone() );
		
		aeRuleMirror::cMatchName * const selection = GetSelectedMatchBone();
		pListMatchName->RemoveAllItems();
		
		const int count = rule->GetMatchNameCount();
		const char *typeStr;
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			aeRuleMirror::cMatchName * const matchName = rule->GetMatchNameAt( i );
			
			switch( matchName->GetType() ){
			case deAnimatorRuleMirror::emntFirst:
				typeStr = "Begin";
				break;
				
			case deAnimatorRuleMirror::emntLast:
				typeStr = "End";
				break;
				
			case deAnimatorRuleMirror::emntMiddle:
				typeStr = "Middle";
				break;
				
			default:
				typeStr = "?";
			}
			
			text.Format( "%s: '%s' <-> '%s'", typeStr,
				matchName->GetFirst().GetString(), matchName->GetSecond().GetString() );
			pListMatchName->AddItem( text, nullptr, matchName );
		}
		
		pListMatchName->SetSelectionWithData( selection );
		
	}else{
		pCBMirrorAxis->SetSelectionWithData( ( void* )( intptr_t )deAnimatorRuleMirror::emaX );
		pCBMirrorBone->ClearText();
		pListMatchName->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pCBMirrorAxis->SetEnabled( enabled );
	pCBMirrorBone->SetEnabled( enabled );
	pListMatchName->SetEnabled( enabled );
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}
