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

#include "igdeMetaPropertyPathWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyPathUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyPathWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyPathWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyPath &GetPropertyPath() const{ return pWidget.GetPropertyPath(); }
	
	void OnValueChanged(const decString &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyPath();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
	}
};


class cListener : public igdeEditPathListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyPathWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		pHelper.OnValueChanged(editPath->GetPath());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyPathWidget &widget) :
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


// Class igdeMetaPropertyPathWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyPathWidget::PropertyListener::PropertyListener(igdeMetaPropertyPathWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyPathWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyPathWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyPath*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyPathWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyPathWidget::igdeMetaPropertyPathWidget(
	igdeMetaPropertyPath &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyPath(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyPath.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyPathWidget::~igdeMetaPropertyPathWidget(){
	Drop();
	pPropertyPath.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyPathWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditPath)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	
	if(pPropertyPath.GetCustomPatternList().IsNotEmpty()){
		helper.EditPath(container, pPropertyPath.GetDescription(),
			pPropertyPath.GetCustomPatternList(), pEditPath, pListener);
		
	}else{
		helper.EditPath(container, pPropertyPath.GetDescription(),
			pPropertyPath.GetResourceType(), pEditPath, pListener);
	}
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyPathWidget::Drop(){
	if(pEditPath && pListener){
		pEditPath->RemoveListener(pListener);
	}
	
	pListener.Clear();
	pEditPath.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyPathWidget::Update(){
	if(pEditPath){
		RunWithPreventUpdate([&]{
			pEditPath->SetPath(GetContext()
				? pPropertyPath.GetPropertyValue(GetContext()) : decString());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyPathWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyPathWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditPath){
		pEditPath->SetVisible(!GetFilteredOut());
	}
}
