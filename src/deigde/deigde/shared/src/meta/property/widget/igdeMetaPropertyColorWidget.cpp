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

#include "igdeMetaPropertyColorWidget.h"
#include "../undo/igdeMetaPropertyColorUndo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyColorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyColorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyColorWidget &GetWidget() const{ return pWidget; }
	inline const igdeMetaContext::Ref &GetContext() const{ return pWidget.GetContext(); }
	inline igdeMetaPropertyColor &GetPropertyColor() const{ return pWidget.GetPropertyColor(); }
	inline bool IsValid() const{ return GetPropertyColor().IsValid(GetContext()); }
	
	void OnValueChanged(const decColor &newValue, const char *undoInfo = nullptr){
		if(pWidget.GetPreventUpdate() || !IsValid()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyColor();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, undoInfo
			? property.RealUndoInfo(context, undoInfo).GetString() : nullptr);
		
		const auto &value = property.GetPropertyValue(context);
		if(!value.IsEqualTo(newValue) && pWidget.GetColorBox()){
			pWidget.RunWithPreventUpdate([&]{
				pWidget.GetColorBox()->SetColor(value);
			});
		}
	}
};


class cListener : public igdeColorBoxListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyColorWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnColorChanged(igdeColorBox *colorBox) override{
		pHelper.OnValueChanged(colorBox->GetColor());
	}
};


class ActionCopy : public igdeAction{
	igdeMetaPropertyColorWidget &pWidget;
	
public:
	ActionCopy(igdeMetaPropertyColorWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyColor();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		clipboard->Set(igdeMetaPropertyColor::ClipboardData::Ref::New(
			property, property.GetPropertyValue(context)));
	}
};


class ActionPaste : public igdeAction{
	cListenerHelper pHelper;
	
public:
	ActionPaste(igdeMetaPropertyColorWidget &widget, const igdeMetaContext::Ref &context,
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
		
		const auto clipboard = pHelper.GetContext()->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(pHelper.GetPropertyColor().GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertyColor::ClipboardData>();
		if(!clip){
			return;
		}
		
		pHelper.OnValueChanged(clip->GetData(), GetText());
	}
	
	void Update() override{
		if(pHelper.IsValid()){
			const auto cb = pHelper.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pHelper.GetPropertyColor().GetClipboardDataTypeName()));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyColorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyColor().GetDefaultValue(), GetText());
	}
};

}


// Class igdeMetaPropertyColorWidget::PropertyListener
////////////////////////////////////////////////////////

igdeMetaPropertyColorWidget::PropertyListener::PropertyListener(igdeMetaPropertyColorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyColorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyColorWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyColor*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyColorWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyColorWidget::igdeMetaPropertyColorWidget(igdeMetaPropertyColor &property) :
igdeMetaPropertyWidget(property),
pPropertyColor(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyColor.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyColorWidget::~igdeMetaPropertyColorWidget(){
	Drop();
	pPropertyColor.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyColorWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pColorBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	builder.GetHelper().ColorBox(pPropertyColor.GetDescription(), pColorBox, pListener);
	pColorBox->SetEnabled(false);
	WrapEditWidget(builder, noLabel, pColorBox);
	
	UpdateMatchable();
	SetContext(builder.GetContext());
}

void igdeMetaPropertyColorWidget::Drop(){
	if(pColorBox && pListener){
		pColorBox->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pColorBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyColorWidget::Update(){
	if(!pColorBox){
		return;
	}
	
	const bool valid = pPropertyColor.IsValid(GetContext());
	RunWithPreventUpdate([&]{
		pColorBox->SetColor(valid
			? pPropertyColor.GetPropertyValue(GetContext())
			: pPropertyColor.GetDefaultValue());
		pColorBox->SetEnabled(valid);
	});
	
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyColorWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

bool igdeMetaPropertyColorWidget::IsPropertyValid() const{
	return pPropertyColor.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyColorWidget::OnContextChanged(){
	Update();
}
