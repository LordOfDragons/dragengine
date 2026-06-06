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
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyColor::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decColor pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyColor &property, const igdeMetaContext::Ref &context, const decColor &newValue) :
	pProperty(&property),
	pContext(property.Capture(context)),
	pOldValue(property.GetPropertyValue(context)),
	pNewValue(newValue)
	{
		SetShortInfo(property.GetUndoInfoOrLabel());
	}
	
	~cUndo() override = default;
	
	void Undo() override{
		pProperty->SetPropertyValue(pContext, pOldValue);
	}
	
	void Redo() override{
		pProperty->SetPropertyValue(pContext, pNewValue);
	}
};


class cListenerHelper{
	igdeMetaPropertyColorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyColorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyColor &GetPropertyColor() const{ return pWidget.GetPropertyColor(); }
	
	void OnValueChanged(const decColor &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyColor();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		auto undo = deTObjectReference<cUndo>::New(property, context, newValue);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
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


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyColorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyColor().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyColorWidget::PropertyListener
////////////////////////////////////////////////////////

igdeMetaPropertyColorWidget::PropertyListener::PropertyListener(igdeMetaPropertyColorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyColorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyColorWidget::PropertyListener::OnValueChanged(igdeMetaPropertyColor*, const igdeMetaContext::Ref&){
	pWidget.Update();
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

void igdeMetaPropertyColorWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pColorBox)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ColorBox(container, pPropertyColor.GetDescription(), pColorBox, pListener);
	
	CreateContextMenuButton(line, helper);
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
	if(pColorBox){
		RunWithPreventUpdate([&]{
			pColorBox->SetColor(GetContext()
				? pPropertyColor.GetPropertyValue(GetContext())
				: pPropertyColor.GetDefaultValue());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyColorWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyColorWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pColorBox){
		pColorBox->SetVisible(!GetFilteredOut());
	}
}
