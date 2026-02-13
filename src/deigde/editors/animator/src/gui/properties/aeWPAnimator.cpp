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
	using Ref = deTObjectReference<cBaseAction>;
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
	using Ref = deTObjectReference<cPathRig>;
	cPathRig(aeWPAnimator &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
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
	using Ref = deTObjectReference<cPathAnimation>;
	cPathAnimation(aeWPAnimator &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
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
	using Ref = deTObjectReference<cActionRigBoneAdd>;
	
public:
	cActionRigBoneAdd(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneAdd",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPAnimator.Action.BoneAdd.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionRigBoneRemove>;
	
public:
	cActionRigBoneRemove(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneRemove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Animator.WPAnimator.Action.BoneRemove.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionMirrorRigBones>;
	
public:
	cActionMirrorRigBones(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneMirror",
		nullptr, "@Animator.WPAnimator.Action.BoneMirror.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionCopyRigBones>;
	
public:
	cActionCopyRigBones(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneCopy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPAnimator.Action.BoneCopy.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataBones::Ref::New(animator->GetListBones()));
		return {};
	}
};

class cActionPasteRigBones : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionPasteRigBones>;
	
public:
	cActionPasteRigBones(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BonePaste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPAnimator.Action.BonePaste.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		aeClipboardDataBones * const clip = (aeClipboardDataBones*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataBones::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUAnimatorSetBones::Ref undo = aeUAnimatorSetBones::Ref::New(
			animator, animator->GetListBones() + clip->GetBones());
		undo->SetShortInfo("@Animator.Undo.PasteBones");
		return undo;
	}
	
	void Update(const aeAnimator &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(aeClipboardDataBones::TYPE_NAME));
	}
};

class cActionExportRigBones : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionExportRigBones>;
	
public:
	cActionExportRigBones(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneExport",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"@Animator.WPAnimator.Action.BoneExport.ToolTip"){}
	
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
		igdeCommonDialogs::GetMultilineString(*pPanel.GetParentWindow(),
			"@Animator.WPAnimator.Dialog.BoneExport.Title",
			"@Animator.WPAnimator.Dialog.BoneExport.Bones", text);
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListBones().GetCount() > 0);
	}
};

class cActionImportRigBones : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionImportRigBones>;
	
public:
	cActionImportRigBones(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.BoneImport",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"@Animator.WPAnimator.Action.BoneImport.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(*pPanel.GetParentWindow(),
			"@Animator.WPAnimator.Dialog.BoneImport.Title",
			"@Animator.WPAnimator.Dialog.BoneImport.Message", text)){
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
		undo->SetShortInfo("@Animator.Undo.ImportBones");
		return undo;
	}
};

class cListRigBones : public igdeListBoxListener{
protected:
	aeWPAnimator &pPanel;
	
public:
	using Ref = deTObjectReference<cListRigBones>;
	cListRigBones(aeWPAnimator &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPanel.GetAnimator() && listBox->GetSelectedItem()){
			pPanel.SetCBRigBoneText(listBox->GetSelectedItem()->GetText());
		}
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
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
	using Ref = deTObjectReference<cActionRigVertexPositionSetAdd>;
	
public:
	cActionRigVertexPositionSetAdd(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSAdd",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPAnimator.Action.VPSAdd.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionRigVertexPositionSetRemove>;
	
public:
	cActionRigVertexPositionSetRemove(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSRemove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Animator.WPAnimator.Action.VPSRemove.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionMirrorRigVertexPositionSets>;
	
public:
	cActionMirrorRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel,
		"@Animator.WPAnimator.Action.VPSMirror", nullptr,
		"@Animator.WPAnimator.Action.VPSMirror.ToolTip"){}
	
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
	using Ref = deTObjectReference<cActionCopyRigVertexPositionSets>;
	
public:
	cActionCopyRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSCopy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPAnimator.Action.VPSCopy.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataVertexPositionSets::Ref::New(animator->GetListVertexPositionSets()));
		return {};
	}
};

class cActionPasteRigVertexPositionSets : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionPasteRigVertexPositionSets>;
	
public:
	cActionPasteRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSPaste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPAnimator.Action.VPSPaste.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		aeClipboardDataVertexPositionSets * const clip = (aeClipboardDataVertexPositionSets*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUAnimatorSetVertexPositionSets::Ref undo(aeUAnimatorSetVertexPositionSets::Ref::New(
			animator, animator->GetListVertexPositionSets() + clip->GetVertexPositionSets()));
		undo->SetShortInfo("@Animator.Undo.PasteVertexPositionSets");
		return undo;
	}
	
	void Update(const aeAnimator &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME));
	}
};

class cActionExportRigVertexPositionSets : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionExportRigVertexPositionSets>;
	
public:
	cActionExportRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSExport",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
		"@Animator.WPAnimator.Action.VPSExport.ToolTip"){}
	
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
		igdeCommonDialogs::GetMultilineString(*pPanel.GetParentWindow(),
			"@Animator.WPAnimator.Dialog.VPSExport.Title",
			"@Animator.WPAnimator.Dialog.VPSExport.Sets", text);
		return {};
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(animator.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionImportRigVertexPositionSets : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionImportRigVertexPositionSets>;
	
public:
	cActionImportRigVertexPositionSets(aeWPAnimator &panel) : cBaseAction(panel, "@Animator.WPAnimator.Action.VPSImport",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
		"@Animator.WPAnimator.Action.VPSImport.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(*pPanel.GetParentWindow(),
			"@Animator.WPAnimator.Dialog.VPSImport.Title",
			"@Animator.WPAnimator.Dialog.VPSImport.Message", text)){
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
		
		const aeUAnimatorSetVertexPositionSets::Ref undo(aeUAnimatorSetVertexPositionSets::Ref::New(
			animator, animator->GetListVertexPositionSets() + sets));
		undo->SetShortInfo("@Animator.Undo.ImportVertexPositionSets");
		return undo;
	}
};

class cListRigVertexPositionSets : public igdeListBoxListener{
protected:
	aeWPAnimator &pPanel;
	
public:
	using Ref = deTObjectReference<cListRigVertexPositionSets>;
	cListRigVertexPositionSets(aeWPAnimator &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPanel.GetAnimator() && listBox->GetSelectedItem()){
			pPanel.SetCBModelVertexPositionSetText(listBox->GetSelectedItem()->GetText());
		}
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
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
	helper.GroupBox(content, groupBox, "@Animator.WPAnimator.AnimatorFilePath.Label");
	helper.EditPath(groupBox, "@Animator.WPAnimator.Rig.Label",
		"@Animator.WPAnimator.Rig.ToolTip", igdeEnvironment::efpltRig, pEditRigPath, cPathRig::Ref::New(*this));
	helper.EditPath(groupBox, "@Animator.WPAnimator.Animation.Label",
		"@Animator.WPAnimator.Animation.ToolTip", igdeEnvironment::efpltAnimation, pEditAnimPath, cPathAnimation::Ref::New(*this));
	
	
	// affected bones
	helper.GroupBoxFlow(content, groupBox, "@Animator.WPAnimator.AffectedBones.Label");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "@Animator.WPAnimator.BoneName.ToolTip", pCBBones, {});
	helper.Button(formLine, pBtnBoneAdd, cActionRigBoneAdd::Ref::New(*this));
	helper.Button(formLine, pBtnBoneDel, cActionRigBoneRemove::Ref::New(*this));
	
	helper.ListBox(groupBox, 5, "@Animator.WPAnimator.AffectedBones.ToolTip", pListBones, cListRigBones::Ref::New(*this));
	pListBones->SetDefaultSorter();
	
	
	// affected vertex position sets
	helper.GroupBoxFlow(content, groupBox, "@Animator.WPAnimator.AffectedVertexPositionSets.Label");
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "@Animator.WPAnimator.SetName.ToolTip", pCBVertexPositionSets, {});
	helper.Button(formLine, pBtnVertexPositionSetAdd, cActionRigVertexPositionSetAdd::Ref::New(*this));
	helper.Button(formLine, pBtnVertexPositionSetDel, cActionRigVertexPositionSetRemove::Ref::New(*this));
	
	helper.ListBox(groupBox, 5, "@Animator.WPAnimator.AffectedVertexPositionSets.ToolTip",
		pListVertexPositionSets, cListRigVertexPositionSets::Ref::New(*this));
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
				pCBBones->AddItem(rig->GetBoneAt(i)->GetName());
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
		const deModel * const model = component ? component->GetModel().Pointer() : nullptr;
		
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
