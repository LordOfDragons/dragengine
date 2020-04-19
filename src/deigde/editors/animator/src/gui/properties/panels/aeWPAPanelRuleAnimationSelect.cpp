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

#include "aeWPAPanelRuleAnimationSelect.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animatoreditor.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleAnimationSelect.h"
#include "../../../undosys/rule/animselect/aeURuleAnimSelectSetMoves.h"
#include "../../../undosys/rule/animselect/aeURuleAnimSelectToggleEnablePosition.h"
#include "../../../undosys/rule/animselect/aeURuleAnimSelectToggleEnableRotation.h"
#include "../../../undosys/rule/animselect/aeURuleAnimSelectToggleEnableSize.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimationSelect.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleAnimationSelect &pPanel;
	
public:
	cBaseAction( aeWPAPanelRuleAnimationSelect &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationSelect * const rule = ( aeRuleAnimationSelect* )pPanel.GetRule();
		if( ! animator || ! rule ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, rule ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeRuleAnimationSelect *rule ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationSelect * const rule = ( aeRuleAnimationSelect* )pPanel.GetRule();
		if( animator && rule ){
			Update( *animator, *rule );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeRuleAnimationSelect & ){
		SetEnabled( true );
		SetSelected( false );
	}
};



class cActionMoveAdd: public cBaseAction{
public:
	cActionMoveAdd( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add move to list" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		const decString &name = pPanel.GetCBMovesText();
		if( name.IsEmpty() ){
			return NULL;
		}
		
		decStringList moves( rule->GetMoves() );
		moves.Add( name );
		return new aeURuleAnimSelectSetMoves( rule, moves, "Animation select add move" );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimationSelect & ){
		SetEnabled( ! pPanel.GetCBMovesText().IsEmpty() );
	}
};

class cActionMoveRemove: public cBaseAction{
public:
	cActionMoveRemove( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove move to list" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		const int selection = pPanel.GetListMovesSelection();
		if( selection == -1 ){
			return NULL;
		}
		
		decStringList moves( rule->GetMoves() );
		moves.RemoveFrom( selection );
		return new aeURuleAnimSelectSetMoves( rule, moves, "Animation select remove move" );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimationSelect & ){
		SetEnabled( pPanel.GetListMovesSelection() != -1 );
	}
};

class cActionMoveUp: public cBaseAction{
public:
	cActionMoveUp( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel, "Move Up",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ), "Move move up in list" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		const int selection = pPanel.GetListMovesSelection();
		if( selection < 1 ){
			return NULL;
		}
		
		decStringList moves( rule->GetMoves() );
		moves.Move( selection, selection - 1 );
		return new aeURuleAnimSelectSetMoves( rule, moves, "Animation select move move up" );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimationSelect & ){
		SetEnabled( pPanel.GetListMovesSelection() > 0 );
	}
};

class cActionMoveDown: public cBaseAction{
public:
	cActionMoveDown( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel, "Move Down",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ), "Move move down in list" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		const int selection = pPanel.GetListMovesSelection();
		if( selection == -1 || selection >= rule->GetMoves().GetCount() - 1 ){
			return NULL;
		}
		
		decStringList moves( rule->GetMoves() );
		moves.Move( selection, selection + 1 );
		return new aeURuleAnimSelectSetMoves( rule, moves, "Animation select move move down" );
	}
	
	virtual void Update( const aeAnimator & , const aeRuleAnimationSelect &rule ){
		const int selection = pPanel.GetListMovesSelection();
		SetEnabled( selection != -1 && selection < rule.GetMoves().GetCount() - 1 );
	}
};

class cListMoves : public igdeListBoxListener{
	aeWPAPanelRuleAnimationSelect &pPanel;
	
public:
	cListMoves( aeWPAPanelRuleAnimationSelect &panel ) : pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, new cActionMoveAdd( pPanel ), true );
		helper.MenuCommand( menu, new cActionMoveRemove( pPanel ), true );
		helper.MenuCommand( menu, new cActionMoveUp( pPanel ), true );
		helper.MenuCommand( menu, new cActionMoveDown( pPanel ), true );
	}
};



class cActionEnablePosition : public cBaseAction{
public:
	cActionEnablePosition( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel,
		"Enable position manipulation", NULL, "Determines if the position is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		return new aeURuleAnimSelectToggleEnablePosition( rule );
	}
	
	virtual void Update( const aeAnimator &, const aeRuleAnimationSelect &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnablePosition() );
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	cActionEnableRotation( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel,
		"Enable rotation manipulation", NULL, "Determines if the rotation is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		return new aeURuleAnimSelectToggleEnableRotation( rule );
	}
	
	virtual void Update( const aeAnimator &, const aeRuleAnimationSelect &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableOrientation() );
	}
};

class cActionEnableSize : public cBaseAction{
public:
	cActionEnableSize( aeWPAPanelRuleAnimationSelect &panel ) : cBaseAction( panel,
		"Enable size manipulation", NULL, "Determines if the size is modified or kept as it is" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeRuleAnimationSelect *rule ){
		return new aeURuleAnimSelectToggleEnableSize( rule );
	}
	
	virtual void Update( const aeAnimator &, const aeRuleAnimationSelect &rule ){
		SetEnabled( true );
		SetSelected( rule.GetEnableSize() );
	}
};

}


// Class aeWPAPanelRuleAnimationSelect
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleAnimationSelect::aeWPAPanelRuleAnimationSelect( aeWPRule &wpRule ) :
aeWPAPanelRule( wpRule, deAnimatorRuleVisitorIdentify::ertAnimationSelect )
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox, formLine;
	
	
	helper.GroupBoxFlow( *this, groupBox, "Animation Select:" );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( formLine );
	helper.ComboBoxFilter( formLine, true, "Moves", pCBMoves, NULL );
	pCBMoves->SetDefaultSorter();
	helper.Button( formLine, pBtnMoveAdd, new cActionMoveAdd( *this ), true );
	
	helper.ListBox( groupBox, 4, "Moves to select from", pListMoves, new cListMoves( *this ) );
	
	helper.CheckBoxOnly( groupBox, pChkEnablePosition, new cActionEnablePosition( *this ), true );
	helper.CheckBoxOnly( groupBox, pChkEnableRotation, new cActionEnableRotation( *this ), true );
	helper.CheckBoxOnly( groupBox, pChkEnableSize, new cActionEnableSize( *this ), true );
}

aeWPAPanelRuleAnimationSelect::~aeWPAPanelRuleAnimationSelect(){
}



// Management
///////////////

void aeWPAPanelRuleAnimationSelect::UpdateAnimMoveList(){
	aeWPAPanelRule::UpdateAnimMoveList();
	
	const decString moveName( pCBMoves->GetText() );
	
	pCBMoves->RemoveAllItems();
	
	if( GetAnimator() ){
		const deAnimation * const engAnimation = GetAnimator()->GetEngineAnimator()
			? GetAnimator()->GetEngineAnimator()->GetAnimation() : NULL;
		if( engAnimation ){
			const int count = engAnimation->GetMoveCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBMoves->AddItem( engAnimation->GetMove( i )->GetName() );
			}
		}
	}
	
	pCBMoves->SortItems();
	pCBMoves->StoreFilterItems();
	
	pCBMoves->SetText( moveName );
}

void aeWPAPanelRuleAnimationSelect::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleAnimationSelect * const rule = ( aeRuleAnimationSelect* )GetRule();
	
	const int selectedMove = GetListMovesSelection();
	
	pListMoves->RemoveAllItems();
	
	if( rule ){
		const decStringList &moves = rule->GetMoves();
		const int moveCount = moves.GetCount();
		int i;
		for( i=0; i<moveCount; i++ ){
			pListMoves->AddItem( moves.GetAt( i ) );
		}
		if( selectedMove != -1 && selectedMove < moveCount - 1 ){
			pListMoves->SetSelection( selectedMove );
		}
		
	}else{
		pListMoves->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pCBMoves->SetEnabled( enabled );
	pListMoves->SetEnabled( enabled );
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
}

void aeWPAPanelRuleAnimationSelect::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleAnimationSelect * const rule = ( aeRuleAnimationSelect* )GetRule();
	if( rule ){
		AddTarget( "Move Time", &rule->GetTargetMoveTime() );
		AddTarget( "Select", &rule->GetTargetSelect() );
	}
}

const decString &aeWPAPanelRuleAnimationSelect::GetCBMovesText() const{
	return pCBMoves->GetText();
}

int aeWPAPanelRuleAnimationSelect::GetListMovesSelection() const{
	return pListMoves->GetSelection();
}
