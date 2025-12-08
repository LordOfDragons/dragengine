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
	cBaseActionSkin(seWPMapped &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description), pPanel(panel){}
	
	void OnAction() override{
		seSkin * const skin = pPanel.GetSkin();
		if(!skin){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(skin)));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(seSkin *skin) = 0;
	
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
	
	igdeUndo *OnAction(seSkin *skin) override{
		seMapped * const mapped = pPanel.GetMapped();
		return mapped ? OnActionMapped(skin, mapped) : nullptr;
	}
	
	virtual igdeUndo *OnActionMapped(seSkin *skin, seMapped *mapped) = 0;
	
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
	cBaseTextFieldListener(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if(!skin || !mapped){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(*textField, skin, mapped)));
		if(undo){
			skin->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, seSkin *skin, seMapped *mapped) = 0;
};


class cActionCopy : public cBaseActionMapped{
public:
	cActionCopy(seWPMapped &panel) : cBaseActionMapped (panel, "Copy Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy mapped"){}
	
	igdeUndo *OnActionMapped(seSkin*, seMapped *mapped) override{
		igdeClipboardData::Ref data;
		seMappedList list;
		list.Add(mapped);
		data.TakeOver(new seClipboardDataMapped(list));
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(data);
		return nullptr;
	}
};

class cActionCut : public cBaseActionMapped{
public:
	cActionCut(seWPMapped &panel) : cBaseActionMapped (panel, "Cut Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut), "Cut mapped"){}
	
	igdeUndo *OnActionMapped(seSkin*, seMapped *mapped) override{
		igdeClipboardData::Ref data;
		seMappedList list;
		list.Add(mapped);
		data.TakeOver(new seClipboardDataMapped(list));
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(data);
		
		seUMappedRemove * const undo = new seUMappedRemove(mapped);
		
		if(undo->GetDependencyCount() > 0 && igdeCommonDialogs::QuestionFormat(&pPanel,
		igdeCommonDialogs::ebsYesNo, "Cut Mapped", "Mapped is used by %d dependencies. "
		"Cutting mapped will also unset it from all dependencies.", undo->GetDependencyCount())
		== igdeCommonDialogs::ebNo){
			undo->FreeReference();
			return nullptr;
		}
		
		return undo;
	}
};

class cActionPaste : public cBaseActionSkin{
public:
	cActionPaste(seWPMapped &panel) : cBaseActionSkin(panel, "Paste Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste), "Paste mapped"){}
	
	igdeUndo *OnAction(seSkin *skin) override{
		const seClipboardDataMapped * const data = (seClipboardDataMapped*)
			pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			GetWithTypeName(seClipboardDataMapped::TYPE_NAME);
		return data ? new seUMappedPaste(skin, *data) : nullptr;
	}
	
	void Update(const seSkin &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName(seClipboardDataMapped::TYPE_NAME));
	}
};

class cActionDuplicate : public cBaseActionMapped{
public:
	cActionDuplicate(seWPMapped &panel) : cBaseActionMapped(panel, "Duplicate Mapped",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiDuplicate), "Duplicate mapped"){}
	
	igdeUndo *OnActionMapped(seSkin *skin, seMapped *mapped) override{
		return new seUMappedDuplicate(skin, *mapped);
	}
};

class cListMapped : public igdeListBoxListener{
	seWPMapped &pPanel;
public:
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
		helper.MenuCommand(menu, new cActionCopy(pPanel), true);
		helper.MenuCommand(menu, new cActionCut(pPanel), true);
		helper.MenuCommand(menu, new cActionPaste(pPanel), true);
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionDuplicate(pPanel), true);
	}
};

class cTextName : public igdeTextFieldListener{
	seWPMapped &pPanel;
public:
	cTextName(seWPMapped &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		seMapped * const mapped = pPanel.GetMapped();
		const decString &value = textField->GetText();
		if(!mapped || mapped->GetName() == value){
			return;
		}
		
		if(pPanel.GetSkin()->GetMappedList().HasNamed(value)){
			igdeCommonDialogs::Error(&pPanel, "Set Mapped Name", "Mapped name exists already");
			textField->SetText(mapped->GetName());
			return;
		}
		
		pPanel.GetSkin()->GetUndoSystem()->Add(seUMappedSetName::Ref::NewWith(mapped, value));
	}
};

class cComboInputType : public igdeComboBoxListener{
	seWPMapped &pPanel;
public:
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
		
		skin->GetUndoSystem()->Add(seUMappedSetInputType::Ref::NewWith(mapped, type));
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	seWPMapped &pPanel;
	seUMappedSetCurve::Ref pUndo;
	
public:
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
			pUndo.TakeOver(new seUMappedSetCurve(mapped, viewCurveBezier->GetCurve()));
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
			pUndo.TakeOver(new seUMappedSetCurve(mapped, viewCurveBezier->GetCurve()));
		}
	}
};

class cTextInputLower : public cBaseTextFieldListener{
public:
	cTextInputLower(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetInputLower()) > FLOAT_SAFE_EPSILON
			? new seUMappedSetInputLower(mapped, value) : nullptr;
	}
};

class cTextInputUpper : public cBaseTextFieldListener{
public:
	cTextInputUpper(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetInputUpper()) > FLOAT_SAFE_EPSILON
			? new seUMappedSetInputUpper(mapped, value) : nullptr;
	}
};

class cActionInputClamped : public cBaseActionMapped {
public:
	cActionInputClamped(seWPMapped &panel) : cBaseActionMapped (panel,
		"Input Clamped:", nullptr, "Input value is clamperd to input range instead od wrapping around"){}
	
	igdeUndo *OnActionMapped(seSkin*, seMapped *mapped) override{
		return new seUMappedToggleInputClamped(mapped);
	}
	
	void UpdateMapped(const seSkin &skin, const seMapped &mapped) override{
		cBaseActionMapped::UpdateMapped(skin, mapped);
		SetSelected(mapped.GetInputClamped());
	}
};

class cTextOutputLower : public cBaseTextFieldListener{
public:
	cTextOutputLower(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetOutputLower()) > FLOAT_SAFE_EPSILON
			? new seUMappedSetOutputLower(mapped, value) : nullptr;
	}
};

class cTextOutputUpper : public cBaseTextFieldListener{
public:
	cTextOutputUpper(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const float value = textField.GetFloat();
		return fabsf(value - mapped->GetOutputUpper()) > FLOAT_SAFE_EPSILON
			? new seUMappedSetOutputUpper(mapped, value) : nullptr;
	}
};

class cTextBone : public cBaseTextFieldListener{
public:
	cTextBone(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const decString &bone = textField.GetText();
		return bone != mapped->GetBone() ? new seUMappedSetBone(mapped, bone) : nullptr;
	}
};

class cTextRenderable : public cBaseTextFieldListener{
public:
	cTextRenderable(seWPMapped &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField &textField, seSkin*, seMapped *mapped) override{
		const decString &renderable = textField.GetText();
		return renderable != mapped->GetRenderable() ? new seUMappedSetRenderable(mapped, renderable) : nullptr;
	}
};

class cComboRenderableComponent : public igdeComboBoxListener{
	seWPMapped &pPanel;
public:
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
		
		skin->GetUndoSystem()->Add(seUMappedSetRenderableComponent::Ref::NewWith(mapped, component));
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
pListener(seWPMappedListener::Ref::NewWith(*this)),
pSkin(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer *content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Mapped:");
	helper.ListBox(groupBox, 8, "Mapped", pListMapped, new cListMapped(*this));
	pListMapped->SetDefaultSorter();
	
	form.TakeOver(new igdeContainerForm(env));
	groupBox->AddChild(form);
	helper.EditString(form, "Name:", "Name of mapped", pEditName, new cTextName(*this));
	
	
	helper.ComboBox(form, "Input Type:", "Type of input to use for curve",
		pCBInputType, new cComboInputType(*this));
	
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
	
	helper.EditFloat(form, "Input Lower:", "Lower input range", pEditInputLower, new cTextInputLower(*this));
	helper.EditFloat(form, "Input Upper:", "Upper input range", pEditInputUpper, new cTextInputUpper(*this));
	helper.CheckBox(form, pChkInputClamped, new cActionInputClamped(*this), true);
	
	helper.EditFloat(form, "Output Lower:", "Lower output range. 0 Y-curve value maps to lower value."
		" Values outside range are extrapolated", pEditOutputLower, new cTextOutputLower(*this));
	helper.EditFloat(form, "Output Upper:", "Upper output range. 1 Y-curve value maps to upper value."
		" Values outside range are extrapolated", pEditOutputUpper, new cTextOutputUpper(*this));
	
	helper.EditString(form, "Bone:", "Name of the bone to use if bone related input type is used",
		pEditBone, new cTextBone(*this));
	
	helper.EditString(form, "Renderable:", "Name of the renderable to use if renderable input type is used",
		pEditRenderable, new cTextRenderable(*this));
	
	helper.ComboBox(form, "Renderable Component:",
		"Color component to use if renderable is used and is of color type",
		pCBRenderableComponent, new cComboRenderableComponent(*this));
	pCBRenderableComponent->AddItem("Red", nullptr, (void*)(intptr_t)deSkinMapped::ercRed);
	pCBRenderableComponent->AddItem("Green", nullptr, (void*)(intptr_t)deSkinMapped::ercGreen);
	pCBRenderableComponent->AddItem("Blue", nullptr, (void*)(intptr_t)deSkinMapped::ercBlue);
	pCBRenderableComponent->AddItem("Alpha", nullptr, (void*)(intptr_t)deSkinMapped::ercAlpha);
	
	helper.ViewCurveBezier(groupBox, pEditCurve, new cEditCurve(*this));
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
		const seMappedList &list = pSkin->GetMappedList();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			seMapped * const mapped = list.GetAt(i);
			pListMapped->AddItem(mapped->GetName(), nullptr, mapped);
		}
		
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
