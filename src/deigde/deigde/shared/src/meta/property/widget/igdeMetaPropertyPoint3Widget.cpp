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

#include "igdeMetaPropertyPoint3Widget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyPoint3Undo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyPoint3Widget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPoint3Widget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPoint3 &GetPropertyPoint3() const{ return pWidget.GetPropertyPoint3(); }
	
	void OnValueChanged(const decPoint3 &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPoint3();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
	}
};


class cListener : public igdeEditPoint3Listener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPoint3Widget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnPoint3Changed(igdeEditPoint3 *editPoint3) override{
		pHelper.OnValueChanged(editPoint3->GetPoint3());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPoint3Widget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyPoint3().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyPoint3Widget::PropertyListener
/////////////////////////////////////////////////////////

igdeMetaPropertyPoint3Widget::PropertyListener::PropertyListener(
	igdeMetaPropertyPoint3Widget &widget) :
pWidget(widget){
}

igdeMetaPropertyPoint3Widget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPoint3Widget::PropertyListener::OnValueChanged(
igdeMetaPropertyPoint3*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyPoint3Widget
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPoint3Widget::igdeMetaPropertyPoint3Widget(
	igdeMetaPropertyPoint3 &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyPoint3(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPoint3.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPoint3Widget::~igdeMetaPropertyPoint3Widget(){
	Drop();
	pPropertyPoint3.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPoint3Widget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditPoint3)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditPoint3(container, pPropertyPoint3.GetDescription(), pEditPoint3, pListener);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyPoint3Widget::Drop(){
	if(pEditPoint3 && pListener){
		pEditPoint3->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditPoint3.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPoint3Widget::Update(){
	if(pEditPoint3){
		RunWithPreventUpdate([&]{
			pEditPoint3->SetPoint3(GetContext()
				? pPropertyPoint3.GetPropertyValue(GetContext())
				: decPoint3());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyPoint3Widget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyPoint3Widget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditPoint3){
		pEditPoint3->SetVisible(!GetFilteredOut());
	}
}
