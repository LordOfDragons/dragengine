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

#include "igdeMetaPropertyVector2Widget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyVector2Undo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyVector2Widget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyVector2Widget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyVector2 &GetPropertyVector2() const{ return pWidget.GetPropertyVector2(); }
	
	void OnValueChanged(const decVector2 &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyVector2();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		auto undo = igdeMetaPropertyVector2Undo::Ref::New(property, context, newValue);
		undo->Redo();
		
		auto * const undoSystem = context->GetUndoSystem();
		if(undoSystem){
			undoSystem->Add(undo);
		}
	}
};


class cListener : public igdeEditVector2Listener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyVector2Widget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnVector2Changed(igdeEditVector2 *editVector2) override{
		pHelper.OnValueChanged(editVector2->GetVector2());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyVector2Widget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyVector2().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyVector2Widget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyVector2Widget::PropertyListener::PropertyListener(
	igdeMetaPropertyVector2Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyVector2Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyVector2Widget::PropertyListener::OnValueChanged(
igdeMetaPropertyVector2*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyVector2Widget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyVector2Widget::igdeMetaPropertyVector2Widget(
	igdeMetaPropertyVector2 &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyVector2(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyVector2.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyVector2Widget::~igdeMetaPropertyVector2Widget(){
	Drop();
	pPropertyVector2.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyVector2Widget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditVector2)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditVector2(container, pPropertyVector2.GetDescription(), 6, 3, pEditVector2, pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyVector2Widget::Drop(){
	if(pEditVector2 && pListener){
		pEditVector2->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditVector2.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyVector2Widget::Update(){
	if(pEditVector2){
		RunWithPreventUpdate([&]{
			pEditVector2->SetVector2(GetContext()
				? pPropertyVector2.GetPropertyValue(GetContext()) : decVector2());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyVector2Widget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyVector2Widget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditVector2){
		pEditVector2->SetVisible(!GetFilteredOut());
	}
}
