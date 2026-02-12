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

#include "aeWPLink.h"
#include "aeWPLinkListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"
#include "../../clipboard/aeClipboardDataLink.h"
#include "../../undosys/link/aeULinkSetName.h"
#include "../../undosys/link/aeULinkSetController.h"
#include "../../undosys/link/aeULinkSetCurve.h"
#include "../../undosys/link/aeULinkAdd.h"
#include "../../undosys/link/aeULinkRemove.h"
#include "../../undosys/link/aeULinkSetRepeat.h"
#include "../../undosys/link/aeULinkSetBone.h"
#include "../../undosys/link/aeULinkSetBoneParameter.h"
#include "../../undosys/link/aeULinkSetBoneMinimum.h"
#include "../../undosys/link/aeULinkSetBoneMaximum.h"
#include "../../undosys/link/aeULinkSetVertexPositionSet.h"
#include "../../undosys/link/aeULinkSetVertexPositionSetMinimum.h"
#include "../../undosys/link/aeULinkSetVertexPositionSetMaximum.h"
#include "../../undosys/link/aeULinkToggleWrapY.h"
#include "../../undosys/link/aeULinkPaste.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
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
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPLink &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeLink * const link = pPanel.GetLink();
		if(!animator || !link){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, link));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeLink *link) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeLink * const link = pPanel.GetLink();
		if(animator && link){
			Update(*animator, *link);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeLink &){
		SetEnabled(true);
		SetSelected(false);
	}
};


class cActionCopy : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCopy>;
	
public:
	cActionCopy(aeWPLink &panel) : cBaseAction(panel, "@Animator.WPLink.Action.Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Animator.WPLink.Action.Copy.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeLink *link) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataLink::Ref::New(link));
		return {};
	}
};

class cActionCut : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCut>;
	
public:
	cActionCut(aeWPLink &panel) : cBaseAction(panel, "@Animator.WPLink.Action.Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"@Animator.WPLink.Action.Cut.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeLink *link) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataLink::Ref::New(link));
		return aeULinkRemove::Ref::New(link);
	}
};

class cActionPaste : public igdeAction{
public:
	using Ref = deTObjectReference<cActionPaste>;
	
private:
	aeWPLink &pPanel;
	
public:
	cActionPaste(aeWPLink &panel) : igdeAction("@Animator.WPLink.Action.Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Animator.WPLink.Action.Paste.ToolTip"), pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator){
			return;
		}
		
		aeClipboardDataLink * const cdata = (aeClipboardDataLink*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataLink::TYPE_NAME);
		if(!cdata){
			return;
		}
		
		animator->GetUndoSystem()->Add(aeULinkPaste::Ref::New(animator, cdata->GetLinks()));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName(aeClipboardDataLink::TYPE_NAME) && pPanel.GetAnimator());
	}
};

class cListLinks : public igdeListBoxListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cListLinks>;
	cListLinks(aeWPLink &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPanel.GetAnimator()){
			pPanel.GetAnimator()->SetActiveLink(listBox->GetSelectedItem()
				? (aeLink*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
		const aeWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, windowMain.GetActionLinkAdd());
		helper.MenuCommand(menu, windowMain.GetActionLinkDuplicate());
		helper.MenuCommand(menu, windowMain.GetActionLinkRemove());
		helper.MenuCommand(menu, windowMain.GetActionLinkRemoveUnused());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopy::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionCut::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPaste::Ref::New(pPanel));
	}
};

class cTextName : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextName>;
	cTextName(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const decString value = textField->GetText();
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetName() == value){
			return;
		}
		
		aeULinkSetName::Ref undo(aeULinkSetName::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};
 
class cComboBone : public igdeComboBoxListener{
	aeWPLink &pPanel;
	bool &pPreventUpdate;
	
public:
	using Ref = deTObjectReference<cComboBone>;
	cComboBone(aeWPLink &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pPreventUpdate){
			return;
		}
		
		const decString &bone = comboBox->GetText();
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetBone() == bone){
			return;
		}
		
		aeULinkSetBone::Ref undo(aeULinkSetBone::Ref::New(link, bone));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cComboConnectionController : public igdeComboBoxListener{
	aeWPLink &pPanel;
	bool &pPreventUpdate;
	
public:
	using Ref = deTObjectReference<cComboConnectionController>;
	cComboConnectionController(aeWPLink &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pPreventUpdate){
			return;
		}
		
		aeController * const controller = comboBox->GetSelectedItem()
			? (aeController*)comboBox->GetSelectedItem()->GetData() : nullptr;
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetController() == controller){
			return;
		}
		
		aeULinkSetController::Ref undo(aeULinkSetController::Ref::New(link, controller));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cSpinRepeat : public igdeSpinTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cSpinRepeat>;
	cSpinRepeat(aeWPLink &panel) : pPanel(panel){}
	
	void OnValueChanged(igdeSpinTextField *textField) override{
		const int value = textField->GetValue();
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetRepeat() == value){
			return;
		}
		
		aeULinkSetRepeat::Ref undo(aeULinkSetRepeat::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cActionCurveInsertAt : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionCurveInsertAt>;
	
public:
	cActionCurveInsertAt(aeWPLink &panel) : cBaseAction(panel, "@Animator.WPLink.Action.CurveInsertAt",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPLink.Action.CurveInsertAt.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeLink *link) override{
		if(!link->GetController()){
			return {};
		}
		
		const aeController &controller = *link->GetController();
		const float x = decMath::linearStep(controller.GetCurrentValue(),
			controller.GetMinimumValue(), controller.GetMaximumValue());
		float y = 0.0f;
		if(!igdeCommonDialogs::GetFloat(pPanel, "@Animator.WPLink.Dialog.InsertCurveValue.Title",
		"@Animator.WPLink.Dialog.InsertCurveValue.YValue", y)){
			return {};
		}
		
		decCurveBezier curve(link->GetCurve());
		curve.AddPoint(decVector2(x, y));
		return aeULinkSetCurve::Ref::New(link, curve);
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	aeWPLink &pPanel;
	igdeUndo::Ref pUndo;
	
public:
	using Ref = deTObjectReference<cEditCurve>;
	cEditCurve(aeWPLink &panel) : pPanel(panel){}
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier) override{
		if(pUndo){
			((aeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			
		}else if(!pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve()){
			return;
			
		}else{
			pUndo = aeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
		
		pPanel.GetAnimator()->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier) override{
		if(pUndo){
			((aeULinkSetCurve&)(igdeUndo&)pUndo).SetNewCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else if(pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve()){
			pUndo = aeULinkSetCurve::Ref::New(pPanel.GetLink(), viewCurveBezier->GetCurve());
		}
	}
	
	void AddContextMenuEntries(igdeViewCurveBezier*, igdeMenuCascade &menu) override{
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCurveInsertAt::Ref::New(pPanel));
	}
};

class cTextBone : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextBone>;
	cTextBone(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const decString value(textField->GetText());
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetBone() == value){
			return;
		}
		
		aeULinkSetBone::Ref undo(aeULinkSetBone::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cComboBoneParameter : public igdeComboBoxListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cComboBoneParameter>;
	cComboBoneParameter(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		deAnimatorLink::eBoneParameter parameter = comboBox->GetSelectedItem()
			? (deAnimatorLink::eBoneParameter)(intptr_t)comboBox->GetSelectedItem()->GetData()
			: deAnimatorLink::ebpPositionZ;
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetBoneParameter() == parameter){
			return;
		}
		
		aeULinkSetBoneParameter::Ref undo(aeULinkSetBoneParameter::Ref::New(link, parameter));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextBoneMinimum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextBoneMinimum>;
	cTextBoneMinimum(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if(!link || fabsf(link->GetBoneMinimum() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		aeULinkSetBoneMinimum::Ref undo(aeULinkSetBoneMinimum::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextBoneMaximum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextBoneMaximum>;
	cTextBoneMaximum(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if(!link || fabsf(link->GetBoneMaximum() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		aeULinkSetBoneMaximum::Ref undo(aeULinkSetBoneMaximum::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};


class cComboVertexPositionSet : public igdeComboBoxListener{
	aeWPLink &pPanel;
	bool &pPreventUpdate;
	
public:
	using Ref = deTObjectReference<cComboVertexPositionSet>;
	cComboVertexPositionSet(aeWPLink &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(pPreventUpdate){
			return;
		}
		
		const decString &vps = comboBox->GetText();
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetVertexPositionSet() == vps){
			return;
		}
		
		aeULinkSetVertexPositionSet::Ref undo(aeULinkSetVertexPositionSet::Ref::New(link, vps));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextVertexPositionSet : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextVertexPositionSet>;
	cTextVertexPositionSet(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const decString value(textField->GetText());
		aeLink * const link = pPanel.GetLink();
		if(!link || link->GetVertexPositionSet() == value){
			return;
		}
		
		aeULinkSetVertexPositionSet::Ref undo(aeULinkSetVertexPositionSet::Ref::New(link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextVertexPositionSetMinimum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextVertexPositionSetMinimum>;
	cTextVertexPositionSetMinimum(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if(!link || fabsf(link->GetVertexPositionSetMinimum() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		aeULinkSetVertexPositionSetMinimum::Ref undo(aeULinkSetVertexPositionSetMinimum::Ref::New(
			link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextVertexPositionSetMaximum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	using Ref = deTObjectReference<cTextVertexPositionSetMaximum>;
	cTextVertexPositionSetMaximum(aeWPLink &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if(!link || fabsf(link->GetVertexPositionSetMaximum() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		aeULinkSetVertexPositionSetMaximum::Ref undo(aeULinkSetVertexPositionSetMaximum::Ref::New(
			link, value));
		if(undo){
			pPanel.GetAnimator()->GetUndoSystem()->Add(undo);
		}
	}
};


class cCheckWrapY : public cBaseAction{
public:
	using Ref = deTObjectReference<cCheckWrapY>;
	
public:
	cCheckWrapY(aeWPLink &panel) : cBaseAction(panel, "@Animator.WPLink.WrapY.Label", nullptr,
		"@Animator.WPLink.WrapY.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeLink *link) override{
		return aeULinkToggleWrapY::Ref::New(link);
	}
	
	void Update(const aeAnimator &animator, const aeLink &link) override{
		cBaseAction::Update(animator, link);
		SetSelected(link.GetWrapY());
	}
};
}


// Class aeWPLink
///////////////////

// Constructor, destructor
////////////////////////////

aeWPLink::aeWPLink(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPLinkListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// links
	helper.GroupBoxFlow(content, groupBox, "@Animator.WPLink.Links.Label");
	helper.ListBox(groupBox, 8, "@Animator.WPLink.Links.ToolTip", pListLink, cListLinks::Ref::New(*this));
	pListLink->SetDefaultSorter();
	
	
	// link settings
	helper.GroupBox(content, groupBox, "@Animator.WPLink.LinkSettings.Label");
	helper.EditString(groupBox, "@Animator.WPLink.Name.Label",
		"@Animator.WPLink.Name.ToolTip", pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Animator.WPLink.Controller.Label",
		"@Animator.WPLink.Controller.ToolTip", pCBController, cComboConnectionController::Ref::New(*this, pPreventUpdate));
	pCBController->SetDefaultSorter();
	
	helper.EditSpinInteger(groupBox, "@Animator.WPLink.Repeat.Label",
		"@Animator.WPLink.Repeat.ToolTip", 1, 99, pSpinRepeat, cSpinRepeat::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPLink.Bone.Label", true,
		"@Animator.WPLink.Bone.ToolTip", pCBBone, cComboBone::Ref::New(*this, pPreventUpdate));
	pCBBone->SetDefaultSorter();
	
	helper.ComboBox(groupBox, "@Animator.WPLink.BoneParameter.Label",
		"@Animator.WPLink.BoneParameter.ToolTip", pCBBoneParameter, cComboBoneParameter::Ref::New(*this));
	pCBBoneParameter->SetAutoTranslateItems(true);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.PositionX", nullptr, (void*)(intptr_t)deAnimatorLink::ebpPositionX);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.PositionY", nullptr, (void*)(intptr_t)deAnimatorLink::ebpPositionY);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.PositionZ", nullptr, (void*)(intptr_t)deAnimatorLink::ebpPositionZ);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.RotationX", nullptr, (void*)(intptr_t)deAnimatorLink::ebpRotationX);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.RotationY", nullptr, (void*)(intptr_t)deAnimatorLink::ebpRotationY);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.RotationZ", nullptr, (void*)(intptr_t)deAnimatorLink::ebpRotationZ);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.ScaleX", nullptr, (void*)(intptr_t)deAnimatorLink::ebpScaleX);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.ScaleY", nullptr, (void*)(intptr_t)deAnimatorLink::ebpScaleY);
	pCBBoneParameter->AddItem("@Animator.WPLink.BoneParameter.ScaleZ", nullptr, (void*)(intptr_t)deAnimatorLink::ebpScaleZ);
	
	helper.EditFloat(groupBox, "@Animator.WPLink.BoneMinimumValue.Label",
		"@Animator.WPLink.BoneMinimumValue.ToolTip", pEditBoneMinimum, cTextBoneMinimum::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPLink.BoneMaximumValue.Label",
		"@Animator.WPLink.BoneMaximumValue.ToolTip", pEditBoneMaximum, cTextBoneMaximum::Ref::New(*this));
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPLink.VertexPositionSet.Label", true,
		"@Animator.WPLink.VertexPositionSet.ToolTip", pCBVertexPositionSet, cComboVertexPositionSet::Ref::New(*this, pPreventUpdate));
	pCBVertexPositionSet->SetDefaultSorter();
	
	helper.EditFloat(groupBox, "@Animator.WPLink.VPSMinimumValue.Label",
		"@Animator.WPLink.VPSMinimumValue.ToolTip", pEditVertexPositionSetMinimum, cTextVertexPositionSetMinimum::Ref::New(*this));
	helper.EditFloat(groupBox, "@Animator.WPLink.VPSMaximumValue.Label",
		"@Animator.WPLink.VPSMaximumValue.ToolTip", pEditVertexPositionSetMaximum, cTextVertexPositionSetMaximum::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkWrapY, cCheckWrapY::Ref::New(*this));
	
	
	helper.GroupBoxFlow(content, groupBox, "@Animator.WPLink.LinkCurve.Label");
	
	helper.ViewCurveBezier(groupBox, pEditCurve, cEditCurve::Ref::New(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
}

aeWPLink::~aeWPLink(){
	SetAnimator(nullptr);
}



// Management
///////////////

void aeWPLink::SetAnimator(aeAnimator *animator){
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
	UpdateControllerList();
	UpdateLinkList();
}

aeLink *aeWPLink::GetLink() const{
	return pAnimator ? pAnimator->GetActiveLink() : nullptr;
}

void aeWPLink::SelectActiveLink(){
	pListLink->SetSelectionWithData(GetLink());
}

void aeWPLink::UpdateLinkList(){
	pListLink->UpdateRestoreSelection([&](){
		pListLink->RemoveAllItems();
		
		if(pAnimator){
			pAnimator->GetLinks().Visit([&](aeLink *link){
				pListLink->AddItem(link->GetName(), nullptr, link);
			});
			pListLink->SortItems();
		}
	}, 0);
	
	UpdateLink();
}

void aeWPLink::UpdateLink(){
	const aeLink * const link = GetLink();
	
	if(link){
		pEditName->SetText(link->GetName());
		pCBController->SetSelectionWithData(link->GetController());
		pSpinRepeat->SetValue(link->GetRepeat());
		pCBBone->SetText(link->GetBone());
		pCBBoneParameter->SetSelectionWithData((void*)(intptr_t)link->GetBoneParameter());
		pEditBoneMinimum->SetFloat(link->GetBoneMinimum());
		pEditBoneMaximum->SetFloat(link->GetBoneMaximum());
		pCBVertexPositionSet->SetText(link->GetVertexPositionSet());
		pEditVertexPositionSetMinimum->SetFloat(link->GetVertexPositionSetMinimum());
		pEditVertexPositionSetMaximum->SetFloat(link->GetVertexPositionSetMaximum());
		pEditCurve->SetCurve(link->GetCurve());
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData(nullptr);
		pSpinRepeat->SetValue(1);
		pCBBone->ClearText();
		pCBBoneParameter->SetSelectionWithData((void*)(intptr_t)deAnimatorLink::ebpPositionZ);
		pEditBoneMinimum->ClearText();
		pEditBoneMaximum->ClearText();
		pCBVertexPositionSet->ClearText();
		pEditVertexPositionSetMinimum->ClearText();
		pEditVertexPositionSetMaximum->ClearText();
		pEditCurve->ClearCurve();
	}
	
	const bool enabled = link;
	pEditName->SetEnabled(enabled);
	pCBController->SetEnabled(enabled);
	pSpinRepeat->SetEnabled(enabled);
	pCBBone->SetEnabled(enabled);
	pCBBoneParameter->SetEnabled(enabled);
	pEditBoneMinimum->SetEnabled(enabled);
	pEditBoneMaximum->SetEnabled(enabled);
	pCBVertexPositionSet->SetEnabled(enabled);
	pEditVertexPositionSetMinimum->SetEnabled(enabled);
	pEditVertexPositionSetMaximum->SetEnabled(enabled);
	pEditCurve->SetEnabled(enabled);
	
	pChkWrapY->GetAction()->Update();
}

void aeWPLink::UpdateRigBoneList(){
	const decString selection(pCBBone->GetText());
	
	pPreventUpdate = true;
	try{
		pCBBone->RemoveAllItems();
		
		if(pAnimator){
			const deRig * const rig = pAnimator->GetEngineRig();
			if(rig){
				const int count = rig->GetBoneCount();
				int i;
				for(i=0; i<count; i++){
					pCBBone->AddItem(rig->GetBoneAt(i)->GetName());
				}
			}
			pCBBone->SortItems();
		}
		
		pCBBone->StoreFilterItems();
		pCBBone->SetText(selection);
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void aeWPLink::UpdateModelVertexPositionSetList(){
	const decString selection(pCBVertexPositionSet->GetText());
	
	pPreventUpdate = true;
	try{
		pCBVertexPositionSet->RemoveAllItems();
		
		if(pAnimator){
			const deComponent * const component = pAnimator->GetEngineComponent();
			const deModel * const model = component ? component->GetModel().Pointer() : nullptr;
			if(model){
				const int count = model->GetVertexPositionSetCount();
				int i;
				for(i=0; i<count; i++){
					pCBVertexPositionSet->AddItem(model->GetVertexPositionSetAt(i)->GetName());
				}
			}
			pCBVertexPositionSet->SortItems();
		}
		
		pCBVertexPositionSet->StoreFilterItems();
		pCBVertexPositionSet->SetText(selection);
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void aeWPLink::UpdateControllerList(){
	const igdeUIHelper::EnableBoolGuard pu(pPreventUpdate);
	void * const selection = pCBController->GetSelectedItemData();
	
	pCBController->RemoveAllItems();
	pCBController->AddItem(Translate("Animator.WPLink.Controller.NoController").ToUTF8(), nullptr, nullptr);
	
	if(pAnimator){
		decString text;
		pAnimator->GetControllers().VisitIndexed([&](int i, aeController *controller){
			text.Format("%d: %s", i, controller->GetName().GetString());
			pCBController->AddItem(text, nullptr, controller);
		});
	}
	
	pCBController->SetSelectionWithData(selection);
}

void aeWPLink::OnLanguageChanged(){
	UpdateControllerList();
}
