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
#include "../../../undosys/rule/animselect/aeURuleAnimSelectToggleEnableVertexPositionSet.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

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
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleAnimationSelect &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationSelect * const rule = (aeRuleAnimationSelect*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleAnimationSelect *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleAnimationSelect * const rule = (aeRuleAnimationSelect*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleAnimationSelect &){
		SetEnabled(true);
		SetSelected(false);
	}
};



class cActionMoveAdd: public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMoveAdd>;
	cActionMoveAdd(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveAdd",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveAdd.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		const decString &name = pPanel.GetCBMovesText();
		if(name.IsEmpty()){
			return {};
		}
		
		decStringList moves(rule->GetMoves());
		moves.Add(name);
		return aeURuleAnimSelectSetMoves::Ref::New(rule, moves, "@Animator.Undo.AnimationSelectAddMove");
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationSelect &) override{
		SetEnabled(!pPanel.GetCBMovesText().IsEmpty());
	}
};

class cActionMoveRemove: public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMoveRemove>;
	cActionMoveRemove(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveRemove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveRemove.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		const int selection = pPanel.GetListMovesSelection();
		if(selection == -1){
			return {};
		}
		
		decStringList moves(rule->GetMoves());
		moves.RemoveFrom(selection);
		return aeURuleAnimSelectSetMoves::Ref::New(rule, moves, "@Animator.Undo.AnimationSelectRemoveMove");
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationSelect &) override{
		SetEnabled(pPanel.GetListMovesSelection() != -1);
	}
};

class cActionMoveUp: public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMoveUp>;
	cActionMoveUp(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveUp",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveUp.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		const int selection = pPanel.GetListMovesSelection();
		if(selection < 1){
			return {};
		}
		
		decStringList moves(rule->GetMoves());
		moves.Move(selection, selection - 1);
		return aeURuleAnimSelectSetMoves::Ref::New(rule, moves, "@Animator.Undo.AnimationSelectMoveMoveUp");
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationSelect &) override{
		SetEnabled(pPanel.GetListMovesSelection() > 0);
	}
};

class cActionMoveDown: public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMoveDown>;
	cActionMoveDown(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveDown",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
		"@Animator.WPAPanelRuleAnimationSelect.Action.MoveDown.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		const int selection = pPanel.GetListMovesSelection();
		if(selection == -1 || selection >= rule->GetMoves().GetCount() - 1){
			return {};
		}
		
		decStringList moves(rule->GetMoves());
		moves.Move(selection, selection + 1);
		return aeURuleAnimSelectSetMoves::Ref::New(rule, moves, "@Animator.Undo.AnimationSelectMoveMoveDown");
	}
	
	void Update(const aeAnimator & , const aeRuleAnimationSelect &rule) override{
		const int selection = pPanel.GetListMovesSelection();
		SetEnabled(selection != -1 && selection < rule.GetMoves().GetCount() - 1);
	}
};

class cListMoves : public igdeListBoxListener{
	aeWPAPanelRuleAnimationSelect &pPanel;
	
public:
	using Ref = deTObjectReference<cListMoves>;
	cListMoves(aeWPAPanelRuleAnimationSelect &panel) : pPanel(panel){}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionMoveAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMoveRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMoveUp::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMoveDown::Ref::New(pPanel));
	}
};



class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.EnablePosition", nullptr,
		"@Animator.WPAPanelRuleAnimationSelect.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		return aeURuleAnimSelectToggleEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRuleAnimationSelect &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.EnableRotation", nullptr,
		"@Animator.WPAPanelRuleAnimationSelect.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		return aeURuleAnimSelectToggleEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRuleAnimationSelect &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.EnableSize", nullptr,
		"@Animator.WPAPanelRuleAnimationSelect.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		return aeURuleAnimSelectToggleEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRuleAnimationSelect &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleAnimationSelect &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleAnimationSelect.EnableVertexPositionSet", nullptr,
		"@Animator.WPAPanelRuleAnimationSelect.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleAnimationSelect *rule) override{
		return aeURuleAnimSelectToggleEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRuleAnimationSelect &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

}


// Class aeWPAPanelRuleAnimationSelect
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleAnimationSelect::aeWPAPanelRuleAnimationSelect(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertAnimationSelect)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	
	helper.GroupBoxFlow(*this, groupBox, "@Animator.WPAPanelRuleAnimationSelect.AnimationSelect");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "@Animator.WPAPanelRuleAnimationSelect.Moves.ToolTip", pCBMoves, {});
	pCBMoves->SetDefaultSorter();
	helper.Button(formLine, pBtnMoveAdd, cActionMoveAdd::Ref::New(*this));
	
	helper.ListBox(groupBox, 4, "@Animator.WPAPanelRuleAnimationSelect.ListMoves.ToolTip",
		pListMoves, cListMoves::Ref::New(*this));
	
	helper.CheckBoxOnly(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBoxOnly(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBoxOnly(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBoxOnly(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
}

aeWPAPanelRuleAnimationSelect::~aeWPAPanelRuleAnimationSelect(){
}



// Management
///////////////

void aeWPAPanelRuleAnimationSelect::UpdateAnimMoveList(){
	aeWPAPanelRule::UpdateAnimMoveList();
	
	const decString moveName(pCBMoves->GetText());
	
	pCBMoves->RemoveAllItems();
	
	if(GetAnimator()){
		const deAnimation * const engAnimation = GetAnimator()->GetEngineAnimator()
			? GetAnimator()->GetEngineAnimator()->GetAnimation().Pointer() : nullptr;
		if(engAnimation){
			const int count = engAnimation->GetMoveCount();
			int i;
			for(i=0; i<count; i++){
				pCBMoves->AddItem(engAnimation->GetMove(i)->GetName());
			}
		}
	}
	
	pCBMoves->SortItems();
	pCBMoves->StoreFilterItems();
	
	pCBMoves->SetText(moveName);
}

void aeWPAPanelRuleAnimationSelect::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleAnimationSelect * const rule = (aeRuleAnimationSelect*)GetRule();
	
	const int selectedMove = GetListMovesSelection();
	
	pListMoves->RemoveAllItems();
	
	if(rule){
		const decStringList &moves = rule->GetMoves();
		const int moveCount = moves.GetCount();
		int i;
		for(i=0; i<moveCount; i++){
			pListMoves->AddItem(moves.GetAt(i));
		}
		if(selectedMove != -1 && selectedMove < moveCount - 1){
			pListMoves->SetSelection(selectedMove);
		}
		
	}else{
		pListMoves->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pCBMoves->SetEnabled(enabled);
	pListMoves->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}

void aeWPAPanelRuleAnimationSelect::UpdateTargetList(){
	aeWPAPanelRule::UpdateTargetList();
	
	aeRuleAnimationSelect * const rule = (aeRuleAnimationSelect*)GetRule();
	if(rule){
		AddTarget("@Animator.Target.MoveTime", rule->GetTargetMoveTime());
		AddTarget("@Animator.Target.Select", rule->GetTargetSelect());
	}
}

const decString &aeWPAPanelRuleAnimationSelect::GetCBMovesText() const{
	return pCBMoves->GetText();
}

int aeWPAPanelRuleAnimationSelect::GetListMovesSelection() const{
	return pListMoves->GetSelection();
}
