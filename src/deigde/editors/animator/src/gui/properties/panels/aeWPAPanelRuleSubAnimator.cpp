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

#include "aeWPAPanelRuleSubAnimator.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleSubAnimator.h"
#include "../../../undosys/rule/subanimator/aeURuleSASetPathAnimator.h"
#include "../../../undosys/rule/subanimator/aeURuleSASetConController.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnablePosition.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnableRotation.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnableSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleSubAnimator &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleSubAnimator *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleSubAnimator & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	cBaseComboBoxListener( aeWPAPanelRuleSubAnimator &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, aeAnimator *animator, aeRuleSubAnimator *rule ) = 0;
};


class cPathAnimator : public igdeEditPathListener{
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	cPathAnimator( aeWPAPanelRuleSubAnimator &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )pPanel.GetRule();
		if( ! rule || rule->GetPathSubAnimator() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeURuleSASetPathAnimator( rule, editPath->GetPath() ) );
		pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
	}
};

class cComboConnection : public cBaseComboBoxListener{
public:
	cComboConnection( aeWPAPanelRuleSubAnimator &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox*, aeAnimator*, aeRuleSubAnimator* ){
		pPanel.UpdateConnection();
		return NULL;
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleSubAnimator &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleSubAnimator *rule ){
		return new aeURuleSubAnimToggleEnablePosition( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleSubAnimator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleSubAnimator &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleSubAnimator *rule ){
		return new aeURuleSubAnimToggleEnableRotation( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleSubAnimator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleSubAnimator &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleSubAnimator *rule ){
		return new aeURuleSubAnimToggleEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleSubAnimator &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

class cComboConnectionController : public cBaseComboBoxListener{
	bool &pPreventUpdate;
	
public:
	cComboConnectionController( aeWPAPanelRuleSubAnimator &panel, bool &preventUpdate ) :
	cBaseComboBoxListener( panel ), pPreventUpdate( preventUpdate ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox*, aeAnimator*, aeRuleSubAnimator *rule ){
		if( pPreventUpdate ){
			return NULL;
		}
		
		const int connection = pPanel.GetCBConnection();
		aeController * const controller = pPanel.GetCBConnectionController();
		return connection != -1 && rule->GetControllerAt( connection ) != controller
			? new aeURuleSASetConController( rule, connection, controller ) : NULL;
	}
};

}



// Class aeWPAPanelRuleSubAnimator
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleSubAnimator::aeWPAPanelRuleSubAnimator( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertSubAnimator ),
pPreventUpdate( false )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox, formLine;
	
	
	helper.GroupBox( *this, groupBox, "Sub Animator:" );
	
	helper.EditPath( groupBox, "Path:", "Sets the animator to use", igdeEnvironment::efpltAnimator,
		pEditPathAnimator, new cPathAnimator( *this ) );
	
	helper.CheckBox( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBox( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBox( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
	
	
	helper.GroupBox( *this, groupBox, "Connections:" );
	
	helper.Label( groupBox, "" );
	helper.Label( groupBox, "Auto-Connected if empty", "Automatically creates connections by"
		" matching controller names if no custom connections are defined" );
	
	helper.ComboBox( groupBox, "Target:", "Target controller", pCBConnection, new cComboConnection( *this ) );
	
	helper.ComboBox( groupBox, "Controller:", "Controller to take value from",
		pCBConnectionController, new cComboConnectionController( *this, pPreventUpdate ) );
	
	
	UpdateControllerList();
}

aeWPAPanelRuleSubAnimator::~aeWPAPanelRuleSubAnimator(){
}



// Management
///////////////

void aeWPAPanelRuleSubAnimator::OnAnimatorPathChanged(){
	if( GetAnimator() ){
		pEditPathAnimator->SetBasePath( GetAnimator()->GetDirectoryPath() );
		
	}else{
		pEditPathAnimator->SetBasePath( "" );
	}
}

void aeWPAPanelRuleSubAnimator::UpdateConnectionList(){
	const aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )GetRule();
	const int selection = pCBConnection->GetSelection();
	const decString selectionText( pCBConnection->GetText() );
	
	pCBConnection->RemoveAllItems();
	
	if( rule && rule->GetSubAnimator() ){
		const deAnimator &subAnimator = *rule->GetSubAnimator();
		const int nameCount = subAnimator.GetControllerCount();
		const int count = rule->GetConnectionCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			if( i < nameCount ){
				text.Format( "%d: %s", i, subAnimator.GetControllerAt( i )->GetName().GetString() );
				
			}else{
				text.Format( "%d: < invalid >", i );
			}
			
			pCBConnection->AddItem( text );
		}
	}
	
	if( selection < pCBConnection->GetItemCount() ){
		pCBConnection->SetSelection( selection );
		
	}else if( pCBConnectionController->GetItemCount() > 0 ){
		pCBConnection->SetSelection( pCBConnection->GetItemCount() - 1 );
	}
	
	if( pCBConnection->GetText() != selectionText ){
		pCBConnection->SetSelection( -1 );
	}
}

void aeWPAPanelRuleSubAnimator::UpdateControllerList(){
	aeController * const selection = GetCBConnectionController();
	pPreventUpdate = true;
	
	try{
		pCBConnectionController->RemoveAllItems();
		pCBConnectionController->AddItem( "< not assigned >", NULL );
		
		if( GetAnimator() ){
			const aeControllerList &list = GetAnimator()->GetControllers();
			const int count = list.GetCount();
			decString text;
			int i;
			
			for( i=0; i<count; i++ ){
				aeController * const controller = list.GetAt( i );
				text.Format( "%d: %s", i, controller->GetName().GetString() );
				pCBConnectionController->AddItem( text, NULL, controller );
			}
		}
		
		pCBConnectionController->SetSelectionWithData( selection );
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void aeWPAPanelRuleSubAnimator::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )GetRule();
	
	if( rule ){
		pEditPathAnimator->SetPath( rule->GetPathSubAnimator() );
		
	}else{
		pEditPathAnimator->ClearPath();
	}
	
	const bool enabled = rule;
	pEditPathAnimator->SetEnabled( enabled );
	pCBConnection->SetEnabled( enabled );
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	
	UpdateConnectionList();
}

void aeWPAPanelRuleSubAnimator::UpdateConnection(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleSubAnimator * const rule = ( aeRuleSubAnimator* )GetRule();
	const int connection = GetCBConnection();
	
	if( rule && connection != -1 ){
		pCBConnectionController->SetSelectionWithData( rule->GetControllerAt( connection ) );
		
	}else{
		pCBConnectionController->SetSelectionWithData( NULL );
	}
	
	pCBConnectionController->SetEnabled( rule && connection != -1 );
}

int aeWPAPanelRuleSubAnimator::GetCBConnection() const{
	return pCBConnection->GetSelection();
}

aeController *aeWPAPanelRuleSubAnimator::GetCBConnectionController() const{
	return pCBConnectionController->GetSelectedItem() ?
		( aeController* )pCBConnectionController->GetSelectedItem()->GetData() : NULL;
}
