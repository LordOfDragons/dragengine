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
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(value != newValue && pWidget.GetEditDirectory()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetEditDirectory()->SetDirectory(value);
			});
		}
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
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
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
		
		context->GetClipboard().Set(igdeMetaPropertyString::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyDirectoryWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pHelper(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		if(!pHelper.IsValid()){
			return;
		}
		
		const auto clip = pHelper.GetContext()->GetClipboard().
			GetWithTypeName(pHelper.GetPropertyDirectory().GetClipboardDataTypeName())
				.DynamicCast<igdeMetaPropertyString::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		SetEnabled(pHelper.IsValid() && pHelper.GetContext()->GetClipboard().
			HasWithTypeName(pHelper.GetPropertyDirectory().GetClipboardDataTypeName()));
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyDirectoryWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyDirectory().GetDefaultValue(), GetText());
	}
};


class cActionPreset : public igdeAction{
	cListenerHelper pHelper;
	igdeMetaPropertyDirectory::Preset::Ref pPreset;
	
public:
	cActionPreset(igdeMetaPropertyDirectoryWidget &widget,
		const igdeMetaPropertyDirectory::Preset::Ref &preset) :
	igdeAction(preset->GetName(), preset->GetIcon(), preset->GetDescription()),
	pHelper(widget),
	pPreset(preset){
	}
	
	~cActionPreset() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pPreset->GetValue(), GetText());
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

void igdeMetaPropertyDirectoryWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pEditDirectory)
	
	pListener = deTObjectReference<cListener>::New(*this);
	builder.GetHelper().EditDirectory(pPropertyDirectory.GetDescription(),
		pEditDirectory, pListener, pPropertyDirectory.GetUseGameVFS());
	pEditDirectory->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pEditDirectory);
	
	UpdateMatchable();
	SetContext(builder.GetContext());
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
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyDirectoryWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
	helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
	helper.MenuSeparator(menu);
	
	const auto presets = pPropertyDirectory.GetPropertyPresets(context);
	if(presets.IsNotEmpty()){
		auto submenu = igdeMenuCascade::Ref::New(env, "@Igde.MetaProperty.Action.Presets");
		presets.Visit([&](const igdeMetaPropertyDirectory::Preset::Ref &preset){
			helper.MenuCommand(submenu, deTObjectReference<cActionPreset>::New(*this, preset));
		});
		menu.AddChild(submenu);
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertyDirectoryWidget::IsPropertyValid() const{
	return pPropertyDirectory.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyDirectoryWidget::OnContextChanged(){
	Update();
}
