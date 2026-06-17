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

#include "igdeMetaPropertyDirectoryWidget.h"
#include "../igdeMetaPropertyString.h"
#include "../undo/igdeMetaPropertyDirectoryUndo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyDirectoryWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyDirectoryWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyDirectoryWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyDirectory &GetPropertyDirectory() const{ return pWidget.GetPropertyDirectory(); }
	inline bool IsValid() const{ return GetPropertyDirectory().IsValid(GetContext()); }
	
	void OnValueChanged(const decString &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyDirectory();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		decString strUndoInfo;
		if(undoInfo){
			const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
			strUndoInfo = tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(undoInfo).ToUTF8();
		}
		property.ChangePropertyValue(context, newValue,
			undoInfo ? strUndoInfo.GetString() : nullptr);
	}
};


class cListener : public igdeEditDirectoryListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyDirectoryWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnEditDirectoryChanged(igdeEditDirectory *editDirectory) override{
		pHelper.OnValueChanged(editDirectory->GetDirectory());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyDirectoryWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyDirectoryWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyDirectory();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			pWidget.GetPropertyDirectory().GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyDirectoryWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		if(!pHelper.IsValid()){
			return;
		}
		
		const auto clipboard = pHelper.GetContext()->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName)
			.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyString::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyDirectoryWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyDirectory().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyDirectoryWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertyDirectoryWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyDirectoryWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyDirectoryWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyDirectoryWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyDirectory*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyDirectoryWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDirectoryWidget::igdeMetaPropertyDirectoryWidget(
	igdeMetaPropertyDirectory &property) :
igdeMetaPropertyWidget(property),
pPropertyDirectory(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyDirectory.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyDirectoryWidget::~igdeMetaPropertyDirectoryWidget(){
	Drop();
	pPropertyDirectory.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyDirectoryWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pEditDirectory)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditDirectory(pPropertyDirectory.GetDescription(), pEditDirectory, pListener,
		pPropertyDirectory.GetUseGameVFS());
	WrapEditWidget(container, helper, noLabel, pEditDirectory);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyDirectoryWidget::Drop(){
	if(pEditDirectory && pListener){
		pEditDirectory->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pEditDirectory.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyDirectoryWidget::Update(){
	if(!pEditDirectory){
		return;
	}
	
	const bool valid = pPropertyDirectory.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pEditDirectory->SetDirectory(valid
			? pPropertyDirectory.GetPropertyValue(GetContext())
			: pPropertyDirectory.GetDefaultValue());
		pEditDirectory->SetEnabled(valid);
	});
}

void igdeMetaPropertyDirectoryWidget::AddContextMenuEntries(igdeMenuCascade &menu){
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


// Protected Functions
////////////////////////

void igdeMetaPropertyDirectoryWidget::OnContextChanged(){
	Update();
}
