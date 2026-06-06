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

#include "igdeMetaPropertyToggleTagsWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyToggleTags::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decStringSet pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyToggleTags &property, const igdeMetaContext::Ref &context,
		const decStringSet &newValue) :
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
	igdeMetaPropertyToggleTagsWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyToggleTagsWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyToggleTagsWidget &GetWidget() const{ return pWidget; }
	inline igdeMetaPropertyToggleTags &GetPropertyToggleTags() const{ return pWidget.GetPropertyToggleTags(); }
	
	void OnValueChanged(const decStringSet &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyToggleTags();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue == oldValue){
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


class cAction : public igdeAction{
	cListenerHelper pHelper;
	
public:
	explicit cAction(igdeMetaPropertyToggleTagsWidget &widget) :
	pHelper(widget){
	}
	
	~cAction() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetWidget().GetToggleTags()->GetEnabledTags());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyToggleTagsWidget &widget) :
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


// Class igdeMetaPropertyToggleTagsWidget::PropertyListener
/////////////////////////////////////////////////////////////

igdeMetaPropertyToggleTagsWidget::PropertyListener::PropertyListener(igdeMetaPropertyToggleTagsWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyToggleTagsWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyToggleTagsWidget::PropertyListener::OnValueChanged(igdeMetaPropertyToggleTags*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyToggleTagsWidget
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyToggleTagsWidget::igdeMetaPropertyToggleTagsWidget(igdeMetaPropertyToggleTags &property) :
igdeMetaPropertyWidget(property),
pPropertyToggleTags(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyToggleTags.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyToggleTagsWidget::~igdeMetaPropertyToggleTagsWidget(){
	Drop();
	pPropertyToggleTags.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyToggleTagsWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pToggleTags)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pAction = deTObjectReference<cAction>::New(*this);
	helper.ToggleTags(container, pToggleTags, pAction);
	
	CreateContextMenuButton(line, helper);
}

void igdeMetaPropertyToggleTagsWidget::Drop(){
	pAction.Clear();
	pToggleTags.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyToggleTagsWidget::Update(){
	if(pToggleTags){
		RunWithPreventUpdate([&]{
			pToggleTags->SetTags(GetContext()
				? pPropertyToggleTags.GetPropertyValue(GetContext()) : decStringSet());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyToggleTagsWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyToggleTagsWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pToggleTags){
		pToggleTags->SetVisible(!GetFilteredOut());
	}
}
