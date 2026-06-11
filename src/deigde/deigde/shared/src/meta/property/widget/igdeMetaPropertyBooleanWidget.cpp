/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeMetaPropertyBooleanWidget.h"
#include "../undo/igdeMetaPropertyBooleanUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyBooleanWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyBooleanWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyBoolean &GetPropertyBoolean() const{ return pWidget.GetPropertyBoolean(); }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	
	void OnValueChanged(bool newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyBoolean();
		if(newValue == property.GetPropertyValue(context)){
			return;
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue, undoInfo);
	}
};


class cAction : public igdeAction{
	cListenerHelper pHelper;
	
public:
	explicit cAction(igdeMetaPropertyBooleanWidget &widget) :
	igdeAction("", nullptr, widget.GetProperty()->GetDescription()),
	pHelper(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		if(pHelper.GetContext()){
			pHelper.OnValueChanged(!pHelper.GetPropertyBoolean().GetPropertyValue(pHelper.GetContext()));
		}
	}
	
	void Update() override{
		const auto &property = pHelper.GetPropertyBoolean();
		const auto &context = pHelper.GetContext();
		
		SetText(property.GetLabel());
		SetDescription(property.GetDescription());
		SetSelected(context ? property.GetPropertyValue(context) : property.GetDefaultValue());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyBooleanWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyBoolean().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyBooleanWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyBooleanWidget::PropertyListener::PropertyListener(igdeMetaPropertyBooleanWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyBooleanWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyBooleanWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyBoolean*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyBooleanWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyBooleanWidget::igdeMetaPropertyBooleanWidget(
	igdeMetaPropertyBoolean &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyBoolean(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyBoolean.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyBooleanWidget::~igdeMetaPropertyBooleanWidget(){
	Drop();
	pPropertyBoolean.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyBooleanWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pCheckBox)
	
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.CheckBox(pCheckBox, pAction);
	WrapEditWidget(container, helper, pCheckBox);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyBooleanWidget::Drop(){
	if(pCheckBox){
		pCheckBox->SetAction(nullptr);
	}
	
	pCheckBox.Clear();
	pAction.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyBooleanWidget::Update(){
	if(pAction){
		RunWithPreventUpdate([&]{
			pAction->Update();
		});
	}
}

void igdeMetaPropertyBooleanWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	igdeMetaPropertyWidget::AddContextMenuEntries(contextMenu);
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}
