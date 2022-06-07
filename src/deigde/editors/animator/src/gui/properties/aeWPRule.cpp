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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeWPRule.h"
#include "aeWPRuleListener.h"
#include "aeWindowProperties.h"
#include "panels/aeWPAPanelRule.h"
#include "panels/aeWPAPanelRuleAnimation.h"
#include "panels/aeWPAPanelRuleAnimationDifference.h"
#include "panels/aeWPAPanelRuleAnimationSelect.h"
#include "panels/aeWPAPanelRuleBoneTransformator.h"
#include "panels/aeWPAPanelRuleStateManipulator.h"
#include "panels/aeWPAPanelRuleStateSnapshot.h"
#include "panels/aeWPAPanelRuleInverseKinematic.h"
#include "panels/aeWPAPanelRuleForeignState.h"
#include "panels/aeWPAPanelRuleSubAnimator.h"
#include "panels/aeWPAPanelRuleTrackTo.h"
#include "panels/aeWPAPanelRuleLimit.h"
#include "panels/aeWPAPanelRuleGroup.h"
#include "panels/aeWPAPanelRuleMirror.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleGroup.h"
#include "../../clipboard/aeClipboardDataRule.h"
#include "../../undosys/rule/aeUPasteRule.h"
#include "../../undosys/rule/aeURemoveRule.h"
#include "../../undosys/rule/group/aeURuleGroupPasteRule.h"
#include "../../undosys/rule/group/aeURuleGroupRemoveRule.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/gui/model/igdeTreeItemReference.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPRule &pPanel;
	
public:
	cBaseAction( aeWPRule &panel, const char *text, igdeIcon *icon, const char *description ) :
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


class cActionCopy : public cBaseAction{
public:
	cActionCopy( aeWPRule &panel ) : cBaseAction( panel, "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy rule to clipboard" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		igdeClipboardDataReference cdata;
		cdata.TakeOver( new aeClipboardDataRule( rule ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( cdata );
		return NULL;
	}
};

class cActionCut : public cBaseAction{
public:
	cActionCut( aeWPRule &panel ) : cBaseAction( panel, "Cut",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut rule into clipboard" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRule *rule ){
		igdeClipboardDataReference cdata;
		cdata.TakeOver( new aeClipboardDataRule( rule ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( cdata );
		
		if( rule->GetParentGroup() ){
			return new aeURuleGroupRemoveRule( rule->GetParentGroup(), rule );
			
		}else{
			return new aeURemoveRule( animator, rule );
		}
	}
};

class cActionPaste : public cBaseAction{
protected:
	const bool pInsert;
	
public:
	cActionPaste( aeWPRule &panel, const char *name, igdeIcon *icon, const char *description, bool insert ) :
		cBaseAction( panel, name, icon, description ), pInsert( insert ){ }
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRule *rule ){
		aeClipboardDataRule * const cdata = ( aeClipboardDataRule* )pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName( aeClipboardDataRule::TYPE_NAME );
		if( ! cdata ){
			return NULL;
		}
		
		if( rule->GetParentGroup() ){
			const aeRuleList &list = rule->GetParentGroup()->GetRules();
			return new aeURuleGroupPasteRule( rule->GetParentGroup(), cdata->GetRules(),
				pInsert ? list.IndexOf( rule ) : list.GetCount() );
			
		}else{
			const aeRuleList &list = animator->GetRules();
			return new aeUPasteRule( animator, cdata->GetRules(),
				pInsert ? list.IndexOf( rule ) : list.GetCount() );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRule & ){
		SetEnabled( pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName( aeClipboardDataRule::TYPE_NAME ) );
	}
};

class cActionPasteAppend : public cActionPaste{
public:
	cActionPasteAppend( aeWPRule &panel ) : cActionPaste( panel, "Paste Append",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste and append rule from clipboard", false ){ }
};

class cActionPasteInsert : public cActionPaste{
public:
	cActionPasteInsert( aeWPRule &panel ) : cActionPaste( panel, "Paste Insert",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste and insert rule from clipboard ", true ){ }
};

class cActionPasteIntoGroup : public cActionPaste{
public:
	cActionPasteIntoGroup( aeWPRule &panel ) : cActionPaste( panel, "Paste Into Group",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste rules from clipboard into group", false ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRule *rule ){
		aeClipboardDataRule * const cdata = ( aeClipboardDataRule* )pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName( aeClipboardDataRule::TYPE_NAME );
		if( ! cdata || rule->GetType() != deAnimatorRuleVisitorIdentify::ertGroup ){
			return NULL;
		}
		
		aeRuleGroup * const group = ( aeRuleGroup* )rule;
		const aeRuleList &list = group->GetRules();
		return new aeURuleGroupPasteRule( group, cdata->GetRules(), list.GetCount() );
	}
	
	virtual void Update( const aeAnimator & , const aeRule &rule ){
		SetEnabled( rule.GetType() == deAnimatorRuleVisitorIdentify::ertGroup
			&& pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
				.GetWithTypeName( aeClipboardDataRule::TYPE_NAME ) );
	}
};

class cTreeRules : public igdeTreeListListener{
	aeWPRule &pPanel;
	
public:
	cTreeRules( aeWPRule &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeTreeList *treeList ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			animator->SetActiveRule( treeList->GetSelection()
				? ( aeRule* )treeList->GetSelection()->GetData() : NULL );
		}
	}
	
	virtual void OnItemExpanded( igdeTreeList*, igdeTreeItem *item ){
		aeRule * const rule = ( aeRule* )item->GetData();
		if( rule && rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup ){
			( ( aeRuleGroup* )rule )->SetTreeListExpanded( true );
		}
	}
	
	virtual void OnItemCollapsed( igdeTreeList*, igdeTreeItem *item ){
		aeRule * const rule = ( aeRule* )item->GetData();
		if( rule && rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup ){
			( ( aeRuleGroup* )rule )->SetTreeListExpanded( false );
		}
	}
	
	virtual void AddContextMenuEntries( igdeTreeList*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		const aeWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeMenuCascadeReference submenu;
		submenu.TakeOver( new igdeMenuCascade( menu.GetEnvironment(), "Add" ) );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddAnim() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddAnimDiff() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddAnimSelect() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddBoneRot() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddInvKin() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddStateManip() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddStateSnap() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddForeignState() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddGroup() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddSubAnimator() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddTrackTo() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddLimit() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddMirror() );
		menu.AddChild( submenu );
		
		submenu.TakeOver( new igdeMenuCascade( menu.GetEnvironment(), "Add Into Group" ) );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupAnim() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupAnimDiff() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupAnimSelect() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupBoneRot() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupInvKin() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupStateManip() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupStateSnap() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupForeignState() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupGroup() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupSubAnimator() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupTrackTo() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupLimit() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleAddIntoGroupMirror() );
		menu.AddChild( submenu );
		
		submenu.TakeOver( new igdeMenuCascade( menu.GetEnvironment(), "Insert" ) );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertAnim() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertAnimDiff() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertAnimSelect() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertBoneRot() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertInvKin() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertStateManip() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertStateSnap() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertForeignState() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertGroup() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertSubAnimator() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertTrackTo() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertLimit() );
		helper.MenuCommand( submenu, windowMain.GetActionRuleInsertMirror() );
		menu.AddChild( submenu );
		
		helper.MenuCommand( menu, windowMain.GetActionRuleRemove() );
		helper.MenuCommand( menu, windowMain.GetActionRuleUp() );
		helper.MenuCommand( menu, windowMain.GetActionRuleDown() );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCopy( pPanel ), true );
		helper.MenuCommand( menu, new cActionCut( pPanel ), true );
		helper.MenuCommand( menu, new cActionPasteAppend( pPanel ), true );
		helper.MenuCommand( menu, new cActionPasteInsert( pPanel ), true );
		helper.MenuCommand( menu, new cActionPasteIntoGroup( pPanel ), true );
	}
};

}



// Class aeWPRule
////////////////////

// Constructor, destructor
////////////////////////////

aeWPRule::aeWPRule( aeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pAnimator( NULL ),
pPanelAnim( NULL ),
pPanelAnimDiff( NULL ),
pPanelAnimSelect( NULL ),
pPanelBoneRot( NULL ),
pPanelFState( NULL ),
pPanelGroup( NULL ),
pPanelIK( NULL ),
pPanelSManip( NULL ),
pPanelSSnapshot( NULL ),
pPanelSubAnimator( NULL ),
pPanelTrackTo( NULL ),
pPanelLimit( NULL ),
pPanelMirror( nullptr ),
pActivePanel( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new aeWPRuleListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	helper.GroupBoxFlow( content, groupBox, "Rules:" );
	helper.TreeList( groupBox, pTreeRule, 8, "Rules", new cTreeRules( *this ) );
	
	
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	content->AddChild( pSwitcher );
	
	igdeWidgetReference panel;
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelAnim = new aeWPAPanelRuleAnimation( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelAnimDiff = new aeWPAPanelRuleAnimationDifference( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelAnimSelect = new aeWPAPanelRuleAnimationSelect( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelBoneRot = new aeWPAPanelRuleBoneTransformator( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelFState = new aeWPAPanelRuleForeignState( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelGroup = new aeWPAPanelRuleGroup( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelIK = new aeWPAPanelRuleInverseKinematic( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelSManip = new aeWPAPanelRuleStateManipulator( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelSSnapshot = new aeWPAPanelRuleStateSnapshot( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelSubAnimator = new aeWPAPanelRuleSubAnimator( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelTrackTo = new aeWPAPanelRuleTrackTo( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelLimit = new aeWPAPanelRuleLimit( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelMirror = new aeWPAPanelRuleMirror( *this ) );
	pSwitcher->AddChild( panel );
	
	pSwitcher->SetCurrent( 0 );  // empty
}

aeWPRule::~aeWPRule(){
	SetAnimator( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void aeWPRule::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	if( pAnimator ){
		pAnimator->RemoveNotifier( pListener );
		pAnimator->FreeReference();
	}
	
	pAnimator = animator;
	
	if( animator ){
		animator->AddNotifier( pListener );
		animator->AddReference();
	}
	
	if( pActivePanel ){
		pActivePanel->OnAnimatorChanged();
	}
	
	UpdateRuleTree();
	UpdateRule();
	UpdateRuleMoveList();
	UpdateRuleBoneList();
	UpdateLinkList();
	UpdateControllerList();
}

aeRule *aeWPRule::GetRule() const{
	return pAnimator ? pAnimator->GetActiveRule() : NULL;
}



void aeWPRule::UpdateControllerList(){
	if( pActivePanel ){
		pActivePanel->UpdateControllerList();
	}
}

void aeWPRule::UpdateLinkList(){
	if( pActivePanel ){
		pActivePanel->UpdateLinkList();
	}
}

void aeWPRule::UpdateRuleTree(){
	if( pAnimator ){
		const aeRuleList &list = pAnimator->GetRules();
		const int count = list.GetCount();
		igdeTreeItem *nextItem = pTreeRule->GetFirstChild();
		int i;
		
		for( i=0; i<count; i++ ){
			aeRule * const rule = list.GetAt( i );
			
			if( ! nextItem ){
				igdeTreeItemReference newItem;
				newItem.TakeOver( new igdeTreeItem( "" ) );
				pTreeRule->AppendItem( NULL, newItem );
				nextItem = newItem;
			}
			
			UpdateRuleTreeItem( nextItem, rule );
			
			nextItem = nextItem->GetNext();
		}
		
		while( nextItem ){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeRule->RemoveItem( removeItem );
		}
		
	}else{
		pTreeRule->RemoveAllItems();
	}
	
	SelectActiveRule();
	if( ! pTreeRule->GetSelection() && pTreeRule->GetFirstChild() ){
		pTreeRule->SetSelection( pTreeRule->GetFirstChild() );
	}
}

void aeWPRule::UpdateRuleTreeItem( igdeTreeItem *item, aeRule *rule ){
	if( ! item || ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	item->SetData( rule );
	
	int order = 0;
	const igdeTreeItem *findOrderItem = item->GetPrevious();
	while( findOrderItem ){
		order++;
		findOrderItem = findOrderItem->GetPrevious();
	}
	
	decString text;
	text.Format( "%i: %s", order, rule->GetName().GetString() );
	item->SetText( text );
	item->SetIcon( pWindowProperties.GetWindowMain().GetRuleIcon( rule->GetType() ) );
	pTreeRule->ItemChanged( item );
	
	if( rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup ){
		const aeRuleGroup &ruleGroup = *( ( aeRuleGroup* )rule );
		const aeRuleList &list = ruleGroup.GetRules();
		const int count = list.GetCount();
		igdeTreeItem *nextItem = item->GetFirstChild();
		int i;
		
		if( item->GetExpanded() != ruleGroup.GetTreeListExpanded() ){
			item->SetExpanded( ruleGroup.GetTreeListExpanded() );
			pTreeRule->ItemChanged( item );
		}
		
		for( i=0; i<count; i++ ){
			aeRule * const rule2 = list.GetAt( i );
			
			if( ! nextItem ){
				igdeTreeItemReference newItem;
				newItem.TakeOver( new igdeTreeItem( "" ) );
				pTreeRule->AppendItem( item, newItem );
				nextItem = newItem;
			}
			
			UpdateRuleTreeItem( nextItem, rule2 );
			
			nextItem = nextItem->GetNext();
		}
		
		while( nextItem ){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeRule->RemoveItem( removeItem );
		}
		
	}else{
		pTreeRule->RemoveAllItems( item );
	}
}

void aeWPRule::SelectActiveRule(){
	pTreeRule->SetSelectionWithData( GetRule() );
}

void aeWPRule::ShowActiveSourcePanel(){
	const aeRule * const rule = GetRule();
	if( ! rule ){
		pSwitcher->SetCurrent( 0 );  // empty
		return;
	}
	
	const deAnimatorRuleVisitorIdentify::eRuleTypes type = rule->GetType();
	
	if( type == pPanelAnim->GetRequiredType() ){
		pSwitcher->SetCurrent( 1 );
		pActivePanel = pPanelAnim;
		
	}else if( type == pPanelAnimDiff->GetRequiredType() ){
		pSwitcher->SetCurrent( 2 );
		pActivePanel = pPanelAnimDiff;
		
	}else if( type == pPanelAnimSelect->GetRequiredType() ){
		pSwitcher->SetCurrent( 3 );
		pActivePanel = pPanelAnimSelect;
		
	}else if( type == pPanelBoneRot->GetRequiredType() ){
		pSwitcher->SetCurrent( 4 );
		pActivePanel = pPanelBoneRot;
		
	}else if( type == pPanelFState->GetRequiredType() ){
		pSwitcher->SetCurrent( 5 );
		pActivePanel = pPanelFState;
		
	}else if( type == pPanelGroup->GetRequiredType() ){
		pSwitcher->SetCurrent( 6 );
		pActivePanel = pPanelGroup;
		
	}else if( type == pPanelIK->GetRequiredType() ){
		pSwitcher->SetCurrent( 7 );
		pActivePanel = pPanelIK;
		
	}else if( type == pPanelSManip->GetRequiredType() ){
		pSwitcher->SetCurrent( 8 );
		pActivePanel = pPanelSManip;
		
	}else if( type == pPanelSSnapshot->GetRequiredType() ){
		pSwitcher->SetCurrent( 9 );
		pActivePanel = pPanelSSnapshot;
		
	}else if( type == pPanelSubAnimator->GetRequiredType() ){
		pSwitcher->SetCurrent( 10 );
		pActivePanel = pPanelSubAnimator;
		
	}else if( type == pPanelTrackTo->GetRequiredType() ){
		pSwitcher->SetCurrent( 11 );
		pActivePanel = pPanelTrackTo;
		
	}else if( type == pPanelLimit->GetRequiredType() ){
		pSwitcher->SetCurrent( 12 );
		pActivePanel = pPanelLimit;
		
	}else if( type == pPanelMirror->GetRequiredType() ){
		pSwitcher->SetCurrent( 13 );
		pActivePanel = pPanelMirror;
		
	}else{
		pSwitcher->SetCurrent( 0 );  // empty
	}
	
	if( pActivePanel ){
		pActivePanel->OnActivated();
	}
}

void aeWPRule::UpdateRule(){
	if( ! pActivePanel ){
		return;
	}
	
	aeRule * const rule = GetRule();
	igdeTreeItem * const item = pTreeRule->GetItemWithData( rule );
	if( item ){
		UpdateRuleTreeItem( item, rule );
	}
	
	pActivePanel->UpdateRule();
}

void aeWPRule::UpdateRuleBoneList(){
	if( pActivePanel ){
		pActivePanel->UpdateRigBoneList();
	}
}

void aeWPRule::UpdateRuleMoveList(){
	if( pActivePanel ){
		pActivePanel->UpdateAnimMoveList();
	}
}
