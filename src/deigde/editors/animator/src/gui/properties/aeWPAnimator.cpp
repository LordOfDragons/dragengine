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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "aeWPAnimator.h"
#include "aeWPAnimatorListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../clipboard/aeClipboardDataBones.h"
#include "../../clipboard/aeClipboardDataVertexPositionSets.h"
#include "../../undosys/animator/aeUAnimatorAddBone.h"
#include "../../undosys/animator/aeUAnimatorRemoveBone.h"
#include "../../undosys/animator/aeUAnimatorMirrorBones.h"
#include "../../undosys/animator/aeUAnimatorSetBones.h"
#include "../../undosys/animator/aeUAnimatorSetAnimationPath.h"
#include "../../undosys/animator/aeUAnimatorSetRigPath.h"
#include "../../undosys/animator/aeUAnimatorAddVertexPositionSet.h"
#include "../../undosys/animator/aeUAnimatorRemoveVertexPositionSet.h"
#include "../../undosys/animator/aeUAnimatorMirrorVertexPositionSets.h"
#include "../../undosys/animator/aeUAnimatorSetVertexPositionSets.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAnimator &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAnimator &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			Update(*animator);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &){
		SetEnabled(true);
		SetSelected(false);
	}
};


class cPathRig : public igdeEditPathListener{
	aeWPAnimator &pPanel;
public:
	typedef deTObjectReference<cPathRig> Ref;
	cPathRig(aeWPAnimator &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator || animator->GetRigPath() == editPath->GetPath()){
			return;
		}
		
		aeUAnimatorSetRigPath::Ref undo(aeUAnimatorSetRigPath::Ref::New(
			animator, editPath->GetPath()));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
};

class cPathAnimation : public igdeEditPathListener{
	aeWPAnimator &pPanel;
public:
	typedef deTObjectReference<cPathAnimation> Ref;
	cPathAnimation(aeWPAnimator &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator || animator->GetAnimationPath() == editPath->GetPath()){
			return;
		}
		
		aeUAnimatorSetAnimationPath::Ref undo(aeUAnimatorSetAnimationPath::Ref::New(
			animator, editPath->GetPath()));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
};


class cActionRigBoneAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionRigBoneAdd> Ref;
	
public:
	cActionRigBoneAdd(aeWPAnimator &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add rig bone"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decString &name = pPanel.GetCBRigBoneText();
		return !name.IsEmpty() && !animator->GetListBones().Has(name)
			? aeUAnimatorAddBone::Ref::New(animator, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &animator) override{
		const decString &name = pPanel.GetCBRigBoneText();
		SetEnabled(!name.IsEmpty() && !animator.GetListBones().Has(name));
	}
};

class cActionRigBoneRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionRigBoneRemove> Ref;
	
public:
	cActionRigBoneRemove(aeWPAnimator &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove rig bone"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decString &name = pPanel.GetCBRigBoneText();
		return !name.IsEmpty() && animator->GetListBones().Has(name)
			? aeUAnimatorRemoveBone::Ref::New(animator, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &animator) override{
		const decString &name = pPanel.GetCBRigBoneText();
		SetEnabled(!name.IsEmpty() && animator.GetListBones().Has(name));
	}
};

class cActionMirrorRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionMirrorRigBones> Ref;
	
public:
	cActionMirrorRigBones(aeWPAnimator &panel) : cBaseAction(panel, "Mirror Bones",
		nullptr, "Mirror rig bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return aeUAnimatorMirrorBones::Ref::New(animator);
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListBones().GetCount() > 0);
	}
};

class cActionCopyRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopyRigBones> Ref;
	
public:
	cActionCopyRigBones(aeWPAnimator &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataBones::Ref::New(animator->GetListBones()));
		return {};
	}
};

class cActionPasteRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionPasteRigBones> Ref;
	
public:
	cActionPasteRigBones(aeWPAnimator &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		aeClipboardDataBones * const clip = (aeClipboardDataBones*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataBones::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUAnimatorSetBones::Ref undo = aeUAnimatorSetBones::Ref::New(
			animator, animator->GetListBones() + clip->GetBones());
		undo->SetShortInfo("Animator paste bones");
		return undo;
	}
	
	void Update(const aeAnimator &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(aeClipboardDataBones::TYPE_NAME));
	}
};

class cActionExportRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionExportRigBones> Ref;
	
public:
	cActionExportRigBones(aeWPAnimator &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decStringSet bones = animator->GetListBones();
		const int count = bones.GetCount();
		decString text;
		int i;
		for(i=0; i<count; i++){
			if(i > 0){
				text.AppendCharacter('\n');
			}
			text.Append(bones.GetAt(i));
		}
		igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Export To Text", "Bones", text);
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListBones().GetCount() > 0);
	}
};

class cActionImportRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportRigBones> Ref;
	
public:
	cActionImportRigBones(aeWPAnimator &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Bones. One bone per line.", text)){
				return {};
			}
			break;
		}
		
		const decStringList lines(text.Split('\n'));
		const int count = lines.GetCount();
		decStringSet bones;
		int i;
		
		for(i=0; i<count; i++){
			if(!lines.GetAt(i).IsEmpty()){
				bones.Add(lines.GetAt(i));
			}
		}
		
		const aeUAnimatorSetBones::Ref undo = aeUAnimatorSetBones::Ref::New(animator, animator->GetListBones() + bones);
		undo->SetShortInfo("Animator import bones");
		return undo;
	}
};

class cListRigBones : public igdeListBoxListener{
protected:
	aeWPAnimator &pPanel;
	
public:
	typedef deTObjectReference<cListRigBones> Ref;
	cListRigBones(aeWPAnimator &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		if(pPanel.GetAnimator() && listBox->GetSelectedItem()){
			pPanel.SetCBRigBoneText(listBox->GetSelectedItem()->GetText());
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetAnimator()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionRigBoneAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionRigBoneRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMirrorRigBones::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopyRigBones::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteRigBones::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionExportRigBones::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportRigBones::Ref::New(pPanel));
	}
};


class cActionRigVertexPositionSetAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionRigVertexPositionSetAdd> Ref;
	
public:
	cActionRigVertexPositionSetAdd(aeWPAnimator &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decString &name = pPanel.GetCBModelVertexPositionSetText();
		return !name.IsEmpty() && !animator->GetListVertexPositionSets().Has(name)
			? aeUAnimatorAddVertexPositionSet::Ref::New(animator, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &animator) override{
		const decString &name = pPanel.GetCBModelVertexPositionSetText();
		SetEnabled(!name.IsEmpty() && !animator.GetListVertexPositionSets().Has(name));
	}
};

class cActionRigVertexPositionSetRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionRigVertexPositionSetRemove> Ref;
	
public:
	cActionRigVertexPositionSetRemove(aeWPAnimator &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decString &name = pPanel.GetCBModelVertexPositionSetText();
		return !name.IsEmpty() && animator->GetListVertexPositionSets().Has(name)
			? aeUAnimatorRemoveVertexPositionSet::Ref::New(animator, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &animator) override{
		const decString &name = pPanel.GetCBModelVertexPositionSetText();
		SetEnabled(!name.IsEmpty() && animator.GetListVertexPositionSets().Has(name));
	}
};

class cActionMirrorRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionMirrorRigVertexPositionSets> Ref;
	
public:
	cActionMirrorRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel,
		"Mirror", nullptr, "Mirror vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return aeUAnimatorMirrorVertexPositionSets::Ref::New(animator);
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionCopyRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopyRigVertexPositionSets> Ref;
	
public:
	cActionCopyRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataVertexPositionSets::Ref::New(animator->GetListVertexPositionSets()));
		return {};
	}
};

class cActionPasteRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionPasteRigVertexPositionSets> Ref;
	
public:
	cActionPasteRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		aeClipboardDataVertexPositionSets * const clip = (aeClipboardDataVertexPositionSets*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUAnimatorSetVertexPositionSets::Ref undo(aeUAnimatorSetVertexPositionSets::Ref::New(
			animator, animator->GetListVertexPositionSets() + clip->GetVertexPositionSets()));
		undo->SetShortInfo("Animator paste vertex position sets");
		return undo;
	}
	
	void Update(const aeAnimator &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME));
	}
};

class cActionExportRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionExportRigVertexPositionSets> Ref;
	
public:
	cActionExportRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		const decStringSet bones = animator->GetListVertexPositionSets();
		const int count = bones.GetCount();
		decString text;
		int i;
		for(i=0; i<count; i++){
			if(i > 0){
				text.AppendCharacter('\n');
			}
			text.Append(bones.GetAt(i));
		}
		igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Export To Text", "Vertex position sets", text);
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionImportRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportRigVertexPositionSets> Ref;
	
public:
	cActionImportRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Vertex position sets. One vertex position set per line.", text)){
				return {};
			}
			break;
		}
		
		const decStringList lines(text.Split('\n'));
		const int count = lines.GetCount();
		decStringSet sets;
		int i;
		
		for(i=0; i<count; i++){
			if(!lines.GetAt(i).IsEmpty()){
				sets.Add(lines.GetAt(i));
			}
		}
		
		const aeUAnimatorSetVertexPositionSets::Ref undo(new aeUAnimatorSetVertexPositionSets(
			animator, animator->GetListVertexPositionSets() + sets));
		undo->SetShortInfo("Animator import vertex position sets");
		return undo;
	}
};

class cListRigVertexPositionSets : public igdeListBoxListener{
protected:
	aeWPAnimator &pPanel;
	
public:
	typedef deTObjectReference<cListRigVertexPositionSets> Ref;
	cListRigVertexPositionSets(aeWPAnimator &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		if(pPanel.GetAnimator() && listBox->GetSelectedItem()){
			pPanel.SetCBModelVertexPositionSetText(listBox->GetSelectedItem()->GetText());
		}
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetAnimator()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionRigVertexPositionSetAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionRigVertexPositionSetRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMirrorRigVertexPositionSets::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopyRigVertexPositionSets::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteRigVertexPositionSets::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionExportRigVertexPositionSets::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportRigVertexPositionSets::Ref::New(pPanel));
	}
};

}



// Class aeWPAnimator
///////////////////////

// Constructor, destructor
////////////////////////////

aeWPAnimator::aeWPAnimator(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPAnimatorListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// animation
	helper.GroupBox(content, groupBox, "Animator File Path:");
	helper.EditPath(groupBox, "Rig:", "Rig resource used by the animator",
		igdeEnvironment::efpltRig, pEditRigPath, cPathRig::Ref::New(*this));
	helper.EditPath(groupBox, "Animation:", "Aniamtion resource used by the animator",
		igdeEnvironment::efpltAnimation, pEditAnimPath, cPathAnimation::Ref::New(*this));
	
	
	// affected bones
	helper.GroupBoxFlow(content, groupBox, "Affected bones:");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Bone name", pCBBones, {});
	helper.Button(formLine, pBtnBoneAdd, cActionRigBoneAdd::Ref::New(*this));
	helper.Button(formLine, pBtnBoneDel, cActionRigBoneRemove::Ref::New(*this));
	
	helper.ListBox(groupBox, 5, "Affectes bones", pListBones, cListRigBones::Ref::New(*this));
	pListBones->SetDefaultSorter();
	
	
	// affected vertex position sets
	helper.GroupBoxFlow(content, groupBox, "Affected vertex position sets:");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Set name", pCBVertexPositionSets, {});
	helper.Button(formLine, pBtnVertexPositionSetAdd, cActionRigVertexPositionSetAdd::Ref::New(*this));
	helper.Button(formLine, pBtnVertexPositionSetDel, cActionRigVertexPositionSetRemove::Ref::New(*this));
	
	helper.ListBox(groupBox, 5, "Affectes vertex position sets", pListVertexPositionSets, cListRigVertexPositionSets::Ref::New(*this));
	pListVertexPositionSets->SetDefaultSorter();
}

aeWPAnimator::~aeWPAnimator(){
	SetAnimator(nullptr);
}



// Management
///////////////

void aeWPAnimator::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
	}
	
	UpdateRigBoneList();
	UpdateModelVertexPositionSetList();
	UpdateAnimator();
	OnAnimatorPathChanged();
}

void aeWPAnimator::UpdateAnimator(){
	if(pAnimator){
		pEditRigPath->SetPath(pAnimator->GetRigPath());
		pEditAnimPath->SetPath(pAnimator->GetAnimationPath());
		
		// bones
		const decStringSet &boneList = pAnimator->GetListBones();
		const int boneCount = boneList.GetCount();
		const decString boneSelection(pListBones->GetSelectedItem()
			? pListBones->GetSelectedItem()->GetText() : decString());
		int i;
		
		pListBones->RemoveAllItems();
		for(i=0; i<boneCount; i++){
			pListBones->AddItem(boneList.GetAt(i));
		}
		pListBones->SortItems();
		pListBones->SetSelection(pListBones->IndexOfItem(boneSelection));
		if(!pListBones->GetSelectedItem() && pListBones->GetItems().IsNotEmpty()){
			pListBones->SetSelection(0);
		}
		
		// vertex position sets
		const decStringSet &vpsList = pAnimator->GetListVertexPositionSets();
		const int vpsCount = vpsList.GetCount();
		const decString vpsSelection(pListVertexPositionSets->GetSelectedItem()
			? pListVertexPositionSets->GetSelectedItem()->GetText() : decString());
		
		pListVertexPositionSets->RemoveAllItems();
		for(i=0; i<vpsCount; i++){
			pListVertexPositionSets->AddItem(vpsList.GetAt(i));
		}
		pListVertexPositionSets->SortItems();
		pListVertexPositionSets->SetSelection(pListVertexPositionSets->IndexOfItem(vpsSelection));
		if(!pListVertexPositionSets->GetSelectedItem() && pListVertexPositionSets->GetItems().IsNotEmpty()){
			pListVertexPositionSets->SetSelection(0);
		}
		
	}else{
		pEditRigPath->ClearPath();
		pEditAnimPath->ClearPath();
		
		pListBones->RemoveAllItems();
		pListVertexPositionSets->RemoveAllItems();
	}
	
	const bool enabled = pAnimator;
	pEditRigPath->SetEnabled(enabled);
	pEditAnimPath->SetEnabled(enabled);
	pCBBones->SetEnabled(enabled);
	pListBones->SetEnabled(enabled);
	pCBVertexPositionSets->SetEnabled(enabled);
	pListVertexPositionSets->SetEnabled(enabled);
	
	pBtnBoneAdd->GetAction()->Update();
	pBtnBoneDel->GetAction()->Update();
	pBtnVertexPositionSetAdd->GetAction()->Update();
	pBtnVertexPositionSetDel->GetAction()->Update();
}

void aeWPAnimator::OnAnimatorPathChanged(){
	if(pAnimator){
		pEditRigPath->SetBasePath(pAnimator->GetDirectoryPath());
		pEditAnimPath->SetBasePath(pAnimator->GetDirectoryPath());
		
	}else{
		pEditRigPath->SetBasePath("");
		pEditAnimPath->SetBasePath("");
	}
}

void aeWPAnimator::UpdateRigBoneList(){
	const decString selection(GetCBRigBoneText());
	
	pCBBones->RemoveAllItems();
	
	if(pAnimator){
		const deRig * const rig = pAnimator->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBBones->AddItem(rig->GetBoneAt(i).GetName());
			}
		}
		
		pCBBones->SortItems();
		pCBBones->StoreFilterItems();
	}
	
	pCBBones->SetText(selection);
}

const decString &aeWPAnimator::GetCBRigBoneText() const{
	return pCBBones->GetText();
}

void aeWPAnimator::SetCBRigBoneText(const char *text){
	pCBBones->SetText(text);
}

void aeWPAnimator::UpdateModelVertexPositionSetList(){
	const decString selection(GetCBModelVertexPositionSetText());
	
	pCBVertexPositionSets->RemoveAllItems();
	
	if(pAnimator){
		const deComponent * const component = pAnimator->GetEngineComponent();
		const deModel * const model = component ? component->GetModel() : nullptr;
		
		if(model){
			const int count = model->GetVertexPositionSetCount();
			int i;
			for(i=0; i<count; i++){
				pCBVertexPositionSets->AddItem(model->GetVertexPositionSetAt(i)->GetName());
			}
		}
		
		pCBVertexPositionSets->SortItems();
		pCBVertexPositionSets->StoreFilterItems();
	}
	
	pCBVertexPositionSets->SetText(selection);
}

const decString &aeWPAnimator::GetCBModelVertexPositionSetText() const{
	return pCBVertexPositionSets->GetText();
}

void aeWPAnimator::SetCBModelVertexPositionSetText(const char *text){
	pCBVertexPositionSets->SetText(text);
}
