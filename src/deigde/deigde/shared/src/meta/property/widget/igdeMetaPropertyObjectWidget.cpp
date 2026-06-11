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

#include "igdeMetaPropertyObjectWidget.h"
#include "../undo/igdeMetaPropertyObjectUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyObjectWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyObjectWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyObjectWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyObject &GetPropertyObject() const{ return pWidget.GetPropertyObject(); }
	
	void OnValueChanged(const deObject::Ref &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyObject();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		if(undoInfo){
			const auto &tm = pWidget.GetLabel()->GetEnvironment().GetTranslationManager();
			undoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8() + ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue, undoInfo);
	}
};


class cListener : public igdeComboBoxListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyObjectWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		pHelper.OnValueChanged(comboBox->GetSelectedItemRefData());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyObjectWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyObjectWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyObject::ClipboardData::Ref::New(
			pWidget.GetPropertyObject().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyObjectWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		const auto &context = pHelper.GetContext();
		if(!context){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyObject::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertyObject::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		const auto &context = pHelper.GetContext();
		if(context){
			const auto cb = context->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyObject::ClipboardData::TypeName));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyObjectWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({}, GetText());
	}
};

}


// Class igdeMetaPropertyObjectWidget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyObjectWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyObjectWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyObjectWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyObjectWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyObject*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyObjectWidget::PropertyListener::OnObjectsChanged(igdeMetaPropertyObject*){
	pWidget.UpdateObjectList();
}


// Class igdeMetaPropertyObjectWidget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObjectWidget::igdeMetaPropertyObjectWidget(
	igdeMetaPropertyObject &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyObject(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyObjectWidget::~igdeMetaPropertyObjectWidget(){
	Drop();
	pPropertyObject.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyObjectWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pComboBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ComboBoxFilter(15, 10, false, pPropertyObject.GetDescription(), pComboBox, pListener);
	WrapEditWidget(container, helper, pComboBox);
	
	UpdateMatchable(container);
	
	UpdateObjectList();
}

void igdeMetaPropertyObjectWidget::Drop(){
	if(pComboBox && pListener){
		pComboBox->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pComboBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyObjectWidget::Update(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		pComboBox->SetSelectionWithRefData(GetContext()
			? pPropertyObject.GetPropertyValue(GetContext()) : deObject::Ref());
	});
}

void igdeMetaPropertyObjectWidget::UpdateObjectList(){
	if(!pComboBox){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto &objects = pPropertyObject.GetObjects();
		igdeMetaContextItemInfo info;
		pComboBox->RemoveAllItems();
		objects.Visit([&](const deObject::Ref &object){
			pPropertyObject.GetObjectItemInfo(object, info);
			auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
			item->SetRefData(object);
			pComboBox->AddItem(item);
		});
		pComboBox->StoreFilterItems();
		
		pComboBox->SetSelectionWithRefData(GetContext()
			? pPropertyObject.GetPropertyValue(GetContext()) : deObject::Ref());
	});
}

void igdeMetaPropertyObjectWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, helper.GetEnvironment()));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}
