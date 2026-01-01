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

#include "seWPMapped.h"
#include "seWPMappedListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../skin/mapped/seMapped.h"
#include "../../clipboard/seClipboardDataMapped.h"
#include "../../undosys/mapped/seUMappedSetBone.h"
#include "../../undosys/mapped/seUMappedSetCurve.h"
#include "../../undosys/mapped/seUMappedSetInputLower.h"
#include "../../undosys/mapped/seUMappedSetOutputLower.h"
#include "../../undosys/mapped/seUMappedSetInputType.h"
#include "../../undosys/mapped/seUMappedSetInputUpper.h"
#include "../../undosys/mapped/seUMappedSetOutputUpper.h"
#include "../../undosys/mapped/seUMappedSetName.h"
#include "../../undosys/mapped/seUMappedToggleInputClamped.h"
#include "../../undosys/mapped/seUMappedSetRenderable.h"
#include "../../undosys/mapped/seUMappedSetRenderableComponent.h"
#include "../../undosys/mapped/seUMappedPaste.h"
#include "../../undosys/mapped/seUMappedRemove.h"
#include "../../undosys/mapped/seUMappedDuplicate.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>



// Actions
////////////

namespace{

class cBaseActionSkin : public igdeAction{
protected:
	seWPMapped &pPanel;
	
public:
	typedef deTObjectReference<cBaseActionSkin> Ref;
	cBaseActionSkin(seWPMapped &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description), pPanel(panel){}
	
	void OnAction() override{
		seSkin * const skin = pPanel.GetSkin();
		if(!skin){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(skin));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSkin *skin) = 0;
	
	void Update() override{
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			Update(*skin);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const seSkin &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseActionMapped : public cBaseActionSkin{
public:
	cBaseActionMapped(seWPMapped &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseActionSkin(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		seMapped * const mapped = pPanel.GetMapped();
		return mapped ? OnActionMapped(skin, mapped) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionMapped(seSkin *skin, seMapped *mapped) = 0;
	
	void Update(const seSkin &skin) override{
		seMapped * const mapped = pPanel.GetMapped();
		if(mapped){
			UpdateMapped(skin, *mapped);
			
		}else{
			cBaseActionSkin::Update(skin);
		}
	}
	
	virtual void UpdateMapped(const seSkin &, const seMapped &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPMapped &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*textField, skin, mapped));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin *skin, seMapped *mapped) = 0;
};


class cActionCopy : public cBaseActionMapped{
public:
	typedef deTObjectReference<cActionCopy> Ref;
	cActionCopy(seWPMapped &panel) : cBaseActionMapped (panel, "Copy Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy mapped"){}
	
	igdeUndo::Ref OnActionMapped(seSkin*, seMapped *mapped) override{
		igdeClipboardData::Ref data;
		seMapped::List list;
		list.Add(mapped);
		data = seClipboardDataMapped::Ref::New(list);
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(data);
		return {};
	}
};

class cActionCut : public cBaseActionMapped{
public:
	typedef deTObjectReference<cActionCut> Ref;
	cActionCut(seWPMapped &panel) : cBaseActionMapped (panel, "Cut Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut), "Cut mapped"){}
	
	igdeUndo::Ref OnActionMapped(seSkin*, seMapped *mapped) override{
		igdeClipboardData::Ref data;
		seMapped::List list;
		list.Add(mapped);
		data = seClipboardDataMapped::Ref::New(list);
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(data);
		
		const seUMappedRemove::Ref undo(seUMappedRemove::Ref::New(mapped));
		
		if(undo->GetDependencyCount() > 0 && igdeCommonDialogs::QuestionFormat(&pPanel,
		igdeCommonDialogs::ebsYesNo, "Cut Mapped", "Mapped is used by %d dependencies. "
		"Cutting mapped will also unset it from all dependencies.", undo->GetDependencyCount())
		== igdeCommonDialogs::ebNo){
			return {};
		}
		
		return undo;
	}
};

class cActionPaste : public cBaseActionSkin{
public:
	typedef deTObjectReference<cActionPaste> Ref;
	cActionPaste(seWPMapped &panel) : cBaseActionSkin(panel, "Paste Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste), "Paste mapped"){}
	
	igdeUndo::Ref OnAction(seSkin *skin) override{
		const seClipboardDataMapped * const data = (seClipboardDataMapped*)
			pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			GetWithTypeName(seClipboardDataMapped::TYPE_NAME);
		return data ? seUMappedPaste::Ref::New(skin, *data) : seUMappedPaste::Ref();
	}
	
	void Update(const seSkin &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(seClipboardDataMapped::TYPE_NAME));
	}
};

class cActionDuplicate : public cBaseActionMapped{
public:
	typedef deTObjectReference<cActionDuplicate> Ref;
	cActionDuplicate(seWPMapped &panel) : cBaseActionMapped(panel, "Duplicate Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDuplicate), "Duplicate mapped"){}
	
	igdeUndo::Ref OnActionMapped(seSkin *skin, seMapped *mapped) override{
		return seUMappedDuplicate::Ref::New(skin, *mapped);
	}
};

class cListMapped : public igdeListBoxListener{
	seWPMapped &pPanel;
public:
	typedef deTObjectReference<cListMapped> Ref;
	cListMapped(seWPMapped &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		seSkin * const skin = pPanel.GetSkin();
		if(skin){
			skin->SetActiveMapped(listBox->GetSelectedItem()
				? (seMapped*)listBox->GetSelectedItem()->GetData() : nullptr);
		}
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand(menu, windowMain.GetActionMappedAdd());
		helper.MenuCommand(menu, windowMain.GetActionMappedRemove());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopy::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionCut::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPaste::Ref::New(pPanel));
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionDuplicate::Ref::New(pPanel));
	}
};

class cTextName : public igdeTextFieldListener{
	seWPMapped &pPanel;
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		seMapped * const mapped = pPanel.GetMapped();
		const decString &value = textField->GetText();
		if(!mapped || mapped->GetName() == value){
			return;
		}
		
		if(pPanel.GetSkin()->GetMapped().HasMatching([&value, mapped](const seMapped &m){
			return m.GetName() == value;
		})){
			igdeCommonDialogs::Error(&pPanel, "Set Mapped Name", "Mapped name exists already");
			textField->SetText(mapped->GetName());
			return;
		}
		
		pPanel.GetSkin()->GetUndoSystem()->Add(seUMappedSetName::Ref::New(mapped, value));
	}
};

class cComboInputType : public igdeComboBoxListener{
	seWPMapped &pPanel;
public:
	typedef deTObjectReference<cComboInputType> Ref;
	cComboInputType(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped || !comboBox->GetSelectedItem()){
			return;
		}
		
		const deSkinMapped::eInputTypes type = (deSkinMapped::eInputTypes)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(type == mapped->GetInputType()){
			return;
		}
		
		skin->GetUndoSystem()->Add(seUMappedSetInputType::Ref::New(mapped, type));
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	seWPMapped &pPanel;
	seUMappedSetCurve::Ref pUndo;
	
public:
	typedef deTObjectReference<cEditCurve> Ref;
	cEditCurve(seWPMapped &panel) : pPanel(panel){}
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped){
			return;
		}
		
		if(pUndo){
			pUndo->SetCurve(viewCurveBezier->GetCurve());
			
		}else{
			if(mapped->GetCurve() == viewCurveBezier->GetCurve()){
				return;
			}
			pUndo = seUMappedSetCurve::Ref::New(mapped, viewCurveBezier->GetCurve());
		}
		
		skin->GetUndoSystem()->Add(pUndo);
		pUndo = nullptr;
	}
	
	void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped){
			return;
		}
		
		if(pUndo){
			pUndo->SetCurve(viewCurveBezier->GetCurve());
			pUndo->Redo();
			
		}else{
			if(mapped->GetCurve() == viewCurveBezier->GetCurve()){
				return;
			}
			pUndo = seUMappedSetCurve::Ref::New(mapped, viewCurveBezier->GetCurve());
		}
	}
};

class cTextInputLower : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextInputLower> Ref;
	cTextInputLower(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetInputLower()) > FLOAT_SAFE_EPSILON
			? seUMappedSetInputLower::Ref::New(mapped, value) : igdeUndo::Ref();
	}
};

class cTextInputUpper : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextInputUpper> Ref;
	cTextInputUpper(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetInputUpper()) > FLOAT_SAFE_EPSILON
			? seUMappedSetInputUpper::Ref::New(mapped, value) : igdeUndo::Ref();
	}
};

class cActionInputClamped : public cBaseActionMapped {
public:
	typedef deTObjectReference<cActionInputClamped> Ref;
	cActionInputClamped(seWPMapped &panel) : cBaseActionMapped (panel,
		"Input Clamped:", nullptr, "Input value is clamperd to input range instead od wrapping around"){}
	
	igdeUndo::Ref OnActionMapped(seSkin*, seMapped *mapped) override{
		return seUMappedToggleInputClamped::Ref::New(mapped);
	}
	
	void UpdateMapped(const seSkin &skin, const seMapped &mapped) override{
		cBaseActionMapped::UpdateMapped(skin, mapped);
		SetSelected(mapped.GetInputClamped());
	}
};

class cTextOutputLower : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextOutputLower> Ref;
	cTextOutputLower(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetOutputLower()) > FLOAT_SAFE_EPSILON
			? seUMappedSetOutputLower::Ref::New(mapped, value) : igdeUndo::Ref();
	}
};

class cTextOutputUpper : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextOutputUpper> Ref;
	cTextOutputUpper(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetOutputUpper()) > FLOAT_SAFE_EPSILON
			? seUMappedSetOutputUpper::Ref::New(mapped, value) : igdeUndo::Ref();
	}
};

class cTextBone : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBone> Ref;
	cTextBone(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const decString &bone = textField.GetText();
		return bone != mapped->GetBone() ? seUMappedSetBone::Ref::New(mapped, bone) : seUMappedSetBone::Ref();
	}
};

class cTextRenderable : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRenderable> Ref;
	cTextRenderable(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const decString &renderable = textField.GetText();
		return renderable != mapped->GetRenderable() ? seUMappedSetRenderable::Ref::New(mapped, renderable) : seUMappedSetRenderable::Ref();
	}
};

class cComboRenderableComponent : public igdeComboBoxListener{
	seWPMapped &pPanel;
public:
	typedef deTObjectReference<cComboRenderableComponent> Ref;
	cComboRenderableComponent(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped || !comboBox->GetSelectedItem()){
			return;
		}
		
		const deSkinMapped::eRenderableComponent component = (deSkinMapped::eRenderableComponent)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(component == mapped->GetRenderableComponent()){
			return;
		}
		
		skin->GetUndoSystem()->Add(seUMappedSetRenderableComponent::Ref::New(mapped, component));
	}
};

}



// Class seWPMapped
/////////////////////

// Constructor, destructor
////////////////////////////

seWPMapped::seWPMapped(seWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(seWPMappedListener::Ref::New(*this)),
pSkin(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Mapped:");
	helper.ListBox(groupBox, 8, "Mapped", pListMapped, cListMapped::Ref::New(*this));
	pListMapped->SetDefaultSorter();
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	helper.EditString(form, "Name:", "Name of mapped", pEditName, cTextName::Ref::New(*this));
	
	
	helper.ComboBox(form, "Input Type:", "Type of input to use for curve",
		pCBInputType, cComboInputType::Ref::New(*this));
	
	pCBInputType->AddItem("Time", nullptr, (void*)(intptr_t)deSkinMapped::eitTime);
	pCBInputType->AddItem("BonePositionX", nullptr, (void*)(intptr_t)deSkinMapped::eitBonePositionX);
	pCBInputType->AddItem("BonePositionY", nullptr, (void*)(intptr_t)deSkinMapped::eitBonePositionY);
	pCBInputType->AddItem("BonePositionZ", nullptr, (void*)(intptr_t)deSkinMapped::eitBonePositionZ);
	pCBInputType->AddItem("BoneRotationX", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneRotationX);
	pCBInputType->AddItem("BoneRotationY", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneRotationY);
	pCBInputType->AddItem("BoneRotationZ", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneRotationZ);
	pCBInputType->AddItem("BoneScaleX", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneScaleX);
	pCBInputType->AddItem("BoneScaleY", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneScaleY);
	pCBInputType->AddItem("BoneScaleZ", nullptr, (void*)(intptr_t)deSkinMapped::eitBoneScaleZ);
	pCBInputType->AddItem("Renderable", nullptr, (void*)(intptr_t)deSkinMapped::eitRenderable);
	
	helper.EditFloat(form, "Input Lower:", "Lower input range", pEditInputLower, cTextInputLower::Ref::New(*this));
	helper.EditFloat(form, "Input Upper:", "Upper input range", pEditInputUpper, cTextInputUpper::Ref::New(*this));
	helper.CheckBox(form, pChkInputClamped, cActionInputClamped::Ref::New(*this));
	
	helper.EditFloat(form, "Output Lower:", "Lower output range. 0 Y-curve value maps to lower value."
		" Values outside range are extrapolated", pEditOutputLower, cTextOutputLower::Ref::New(*this));
	helper.EditFloat(form, "Output Upper:", "Upper output range. 1 Y-curve value maps to upper value."
		" Values outside range are extrapolated", pEditOutputUpper, cTextOutputUpper::Ref::New(*this));
	
	helper.EditString(form, "Bone:", "Name of the bone to use if bone related input type is used",
		pEditBone, cTextBone::Ref::New(*this));
	
	helper.EditString(form, "Renderable:", "Name of the renderable to use if renderable input type is used",
		pEditRenderable, cTextRenderable::Ref::New(*this));
	
	helper.ComboBox(form, "Renderable Component:",
		"Color component to use if renderable is used and is of color type",
		pCBRenderableComponent, cComboRenderableComponent::Ref::New(*this));
	pCBRenderableComponent->AddItem("Red", nullptr, (void*)(intptr_t)deSkinMapped::ercRed);
	pCBRenderableComponent->AddItem("Green", nullptr, (void*)(intptr_t)deSkinMapped::ercGreen);
	pCBRenderableComponent->AddItem("Blue", nullptr, (void*)(intptr_t)deSkinMapped::ercBlue);
	pCBRenderableComponent->AddItem("Alpha", nullptr, (void*)(intptr_t)deSkinMapped::ercAlpha);
	
	helper.ViewCurveBezier(groupBox, pEditCurve, cEditCurve::Ref::New(*this));
	pEditCurve->SetDefaultSize(decPoint(200, 250));
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp(true);
	pEditCurve->SetClampMin(decVector2(0.0f, 0.0f));
	pEditCurve->SetClampMax(decVector2(1.0f, 1.0f));
}

seWPMapped::~seWPMapped(){
	SetSkin(nullptr);
}



// Management
///////////////

void seWPMapped::SetSkin(seSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	if(pSkin){
		pSkin->RemoveListener(pListener);
	}
	
	pSkin = skin;
	
	if(skin){
		skin->AddListener(pListener);
	}
	
	UpdateMappedList();
	SelectActiveMapped();
	UpdateMapped();
	FitViewToCurve();
}

seMapped *seWPMapped::GetMapped() const{
	return pSkin ? pSkin->GetActiveMapped() : nullptr;
}

void seWPMapped::UpdateMappedList(){
	seMapped * const selection = GetMapped();
	
	pListMapped->RemoveAllItems();
	
	if(pSkin){
		pSkin->GetMapped().Visit([&](seMapped *mapped){
			pListMapped->AddItem(mapped->GetName(), nullptr, mapped);
		});
		
		pListMapped->SortItems();
	}
	
	pListMapped->SetSelectionWithData(selection);
	UpdateMapped();
}

void seWPMapped::SelectActiveMapped(){
	pListMapped->SetSelectionWithData(GetMapped());
}

void seWPMapped::UpdateMapped(){
	const seMapped * const mapped = GetMapped();
	
	if(mapped){
		pEditName->SetText(mapped->GetName());
		pEditCurve->SetCurve(mapped->GetCurve());
		pCBInputType->SetSelectionWithData((void*)(intptr_t)mapped->GetInputType());
		pEditInputLower->SetFloat(mapped->GetInputLower());
		pEditInputUpper->SetFloat(mapped->GetInputUpper());
		pEditOutputLower->SetFloat(mapped->GetOutputLower());
		pEditOutputUpper->SetFloat(mapped->GetOutputUpper());
		pEditBone->SetText(mapped->GetBone());
		pEditRenderable->SetText(mapped->GetRenderable());
		pCBRenderableComponent->SetSelectionWithData((void*)(intptr_t)mapped->GetRenderableComponent());
		
	}else{
		pEditName->ClearText();
		pEditCurve->ClearCurve();
		pCBInputType->SetSelectionWithData((void*)(intptr_t)deSkinMapped::eitTime);
		pEditInputLower->ClearText();
		pEditInputUpper->ClearText();
		pEditOutputLower->ClearText();
		pEditOutputUpper->ClearText();
		pEditBone->ClearText();
		pEditRenderable->ClearText();
		pCBRenderableComponent->SetSelectionWithData((void*)(intptr_t)deSkinMapped::ercRed);
	}
	
	const bool enabled = mapped;
	
	pEditName->SetEnabled(enabled);
	
	pChkInputClamped->GetAction()->Update();
	
	pEditCurve->SetEnabled(enabled);
	pCBInputType->SetEnabled(enabled);
	pEditInputLower->SetEnabled(enabled);
	pEditInputUpper->SetEnabled(enabled);
	pEditOutputLower->SetEnabled(enabled);
	pEditOutputUpper->SetEnabled(enabled);
	pEditBone->SetEnabled(enabled);
	pEditRenderable->SetEnabled(enabled);
	pCBRenderableComponent->SetEnabled(enabled);
}

void seWPMapped::FitViewToCurve(){
	pEditCurve->FitViewToCurve();
}
