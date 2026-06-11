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

#include "igdeMetaPropertyTagsWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyTagsUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyTagsWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyTagsWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyTagsWidget &GetWidget() const{ return pWidget; }
	inline igdeMetaPropertyTags &GetPropertyTags() const{ return pWidget.GetPropertyTags(); }
	
	void OnValueChanged(const decStringSet &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyTags();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
	}
};


class cAction : public igdeAction{
	cListenerHelper pHelper;
	
public:
	explicit cAction(igdeMetaPropertyTagsWidget &widget) :
	pHelper(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetWidget().GetEditTags()->GetTags());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyTagsWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged({});
	}
};

}


// Class igdeMetaPropertyTagsWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyTagsWidget::PropertyListener::PropertyListener(igdeMetaPropertyTagsWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyTagsWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyTagsWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyTags*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyTagsWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTagsWidget::igdeMetaPropertyTagsWidget(
	igdeMetaPropertyTags &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyTags(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyTags.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyTagsWidget::~igdeMetaPropertyTagsWidget(){
	Drop();
	pPropertyTags.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyTagsWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditTags)
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.EditTags(pEditTags, pAction);
	WrapEditWidget(container, helper, pEditTags);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyTagsWidget::Drop(){
	pAction.Clear();
	pEditTags.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyTagsWidget::Update(){
	if(pEditTags){
		RunWithPreventUpdate([&]{
			pEditTags->SetTags(GetContext()
				? pPropertyTags.GetPropertyValue(GetContext()) : decStringSet());
		});
	}
}

void igdeMetaPropertyTagsWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	igdeMetaPropertyWidget::AddContextMenuEntries(contextMenu);
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}
