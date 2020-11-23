/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>

#include "igdeWPTriggerTable.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeContainerReference.h"
#include "../igdeTextField.h"
#include "../igdeListBox.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../model/igdeListItem.h"
#include "../menu/igdeMenuCascade.h"
#include "../resources/igdeIcon.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../triggersystem/igdeTriggerTarget.h"
#include "../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cTextFilter : public igdeTextFieldListener{
	igdeWPTriggerTable &pPanel;
	
public:
	cTextFilter( igdeWPTriggerTable &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanging( igdeTextField* ){
		pPanel.UpdateTable();
	}
	
	virtual void OnTextChanged( igdeTextField* ){
		pPanel.UpdateTable();
	}
};

class cListTable : public igdeListBoxListener{
	igdeWPTriggerTable &pPanel;
	
public:
	cListTable( igdeWPTriggerTable &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox*, int ){
		pPanel.GetActionToggle()->OnAction();
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, pPanel.GetActionClear() );
		helper.MenuSeparator( menu );
		
		helper.MenuCommand( menu, pPanel.GetActionAdd() );
		helper.MenuCommand( menu, pPanel.GetActionRemove() );
		helper.MenuSeparator( menu );
		
		helper.MenuCommand( menu, pPanel.GetActionFire() );
		helper.MenuCommand( menu, pPanel.GetActionReset() );
		helper.MenuCommand( menu, pPanel.GetActionFullReset() );
	}
};


class cActionToggle : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionToggle( igdeWPTriggerTable &panel ) :
	igdeAction( "Toggle State", NULL, "Toggle selected target between fired and reset state" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if( ! target ){
			return;
		}
		
		if( target->GetFired() ){
			target->Reset();
			
		}else{
			target->Fire();
		}
		
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedTarget() );
	}
};

class cActionClear : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionClear( igdeWPTriggerTable &panel ) :
	igdeAction( "Clear Table", NULL, "Remove all targets from the table" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if( ! list ){
			return;
		}
		
		list->RemoveAll();
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTriggerTargetList() && pPanel.GetTriggerTargetList()->GetCount() > 0 );
	}
};

class cActionAdd : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionAdd( igdeWPTriggerTable &panel ) :
	igdeAction( "Add Target...", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add target to table" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if( ! list ){
			return;
		}
		
		decString name( "Target" );
		while( igdeCommonDialogs::GetString( &pPanel, "Add Target", "Name:", name ) ){
			if( list->HasNamed( name ) ){
				igdeCommonDialogs::Information( &pPanel, "Add Target", "A target with this name exists already." );
				continue;
			}
			
			deObjectReference refTarget;
			refTarget.TakeOver( new igdeTriggerTarget( name ) );
			
			igdeTriggerTarget * const target = ( igdeTriggerTarget* )( deObject* )refTarget;
			list->Add( target );
			
			pPanel.SelectTarget( target );
			pPanel.OnAction();
			return;
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTriggerTargetList() );
	}
};

class cActionRemove : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionRemove( igdeWPTriggerTable &panel ) :
	igdeAction( "Remove Target", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove selected target from table" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if( ! list || ! target ){
			return;
		}
		
		list->Remove( target );
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedTarget() );
	}
};

class cActionFire : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionFire( igdeWPTriggerTable &panel ) :
	igdeAction( "Fire Target", NULL, "Fire selected target" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if( ! target ){
			return;
		}
		
		target->Fire();
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedTarget() && ! pPanel.GetSelectedTarget()->GetFired() );
	}
};

class cActionReset : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionReset( igdeWPTriggerTable &panel ) :
	igdeAction( "Reset Target", NULL, "Reset selected target" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if( ! target ){
			return;
		}
		
		target->Reset();
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedTarget() && pPanel.GetSelectedTarget()->GetFired() );
	}
};

class cActionFullReset : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	cActionFullReset( igdeWPTriggerTable &panel ) :
	igdeAction( "Full Reset Target", NULL, "Full reset selected target" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if( ! target ){
			return;
		}
		
		target->FullReset();
		pPanel.OnAction();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedTarget() && ( pPanel.GetSelectedTarget()->GetFired()
			|| pPanel.GetSelectedTarget()->GetHasFired() ) );
	}
};

}



// Class igdeWPTriggerTable
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPTriggerTable::igdeWPTriggerTable( igdeEnvironment &environment ) :
igdeContainerFlow( environment, eaY ),
pTriggerTargetList( NULL )
{
	pCreateContent();
}

igdeWPTriggerTable::igdeWPTriggerTable( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, eaY ),
pTriggerTargetList( NULL )
{
	pCreateContent();
	SetAction( action );
}

igdeWPTriggerTable::~igdeWPTriggerTable(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeWPTriggerTable::SetTriggerTargetList( igdeTriggerTargetList *triggerTargetList ){
	if( triggerTargetList == pTriggerTargetList ){
		return;
	}
	
	pTriggerTargetList = triggerTargetList;
	UpdateTable();
}

igdeTriggerTarget *igdeWPTriggerTable::GetSelectedTarget() const{
	const igdeListItem * const selection = pListTriggerTable->GetSelectedItem();
	return selection ? ( igdeTriggerTarget* )selection->GetData() : NULL;
}

void igdeWPTriggerTable::SelectTarget( igdeTriggerTarget *target ){
	pListTriggerTable->SetSelectionWithData( target );
}

void igdeWPTriggerTable::UpdateTable(){
	const decString filter( pEditTriggerTableFilter->GetText().GetLower() );
	igdeTriggerTarget *selectedTarget = GetSelectedTarget();
	const bool hasFilter = ! filter.IsEmpty();
	
	pListTriggerTable->RemoveAllItems();
	
	if( pTriggerTargetList ){
		const int count = pTriggerTargetList->GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			igdeTriggerTarget * const target = pTriggerTargetList->GetAt( i );
			
			if( hasFilter && target->GetName().GetLower().FindString( filter ) == -1 ){
				continue;
			}
			
			text.Format( "%s (%s)", target->GetName().GetString(),
				target->GetHasFired() ? "has fired" : "never fired" );
			pListTriggerTable->AddItem( text, target->GetFired() ? pIconFired : pIconNotFired, target );
		}
	}
	
	pListTriggerTable->SortItems();
	
	pListTriggerTable->SetSelectionWithData( selectedTarget );
	if( ! pListTriggerTable->GetSelectedItem() && pListTriggerTable->GetItemCount() > 0 ){
		pListTriggerTable->SetSelection( 0 );
	}
}



void igdeWPTriggerTable::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeWPTriggerTable::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeWPTriggerTable::OnParameterChanged( igdeAction *action ){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPTriggerTable::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeWPTriggerTable::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeWPTriggerTable::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pIconFired = env.GetStockIcon( igdeEnvironment::esiSmallPlus );
	pIconNotFired = env.GetStockIcon( igdeEnvironment::esiSmallMinus );
	
	pActionToggle.TakeOver( new cActionToggle( *this ) );
	pActionClear.TakeOver( new cActionClear( *this ) );
	pActionAdd.TakeOver( new cActionAdd( *this ) );
	pActionRemove.TakeOver( new cActionRemove( *this ) );
	pActionFire.TakeOver( new cActionFire( *this ) );
	pActionReset.TakeOver( new cActionReset( *this ) );
	pActionFullReset.TakeOver( new cActionFullReset( *this ) );
	
	helper.ListBox( *this, 8, "List of targets in the table", pListTriggerTable, new cListTable( *this ) );
	pListTriggerTable->SetDefaultSorter();
	
	igdeContainerReference frameLine;
	frameLine.TakeOver( new igdeContainerFlow( env, eaX, esLast ) );
	helper.Label( frameLine, "Filter:" );
	helper.EditString( frameLine, "Filter targets in the list case insensitive",
		pEditTriggerTableFilter, new cTextFilter( *this ) );
	AddChild( frameLine );
}
