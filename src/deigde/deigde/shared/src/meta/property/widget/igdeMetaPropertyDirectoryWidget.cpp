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
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


namespace {

class cUndo : public igdeUndo{
	const igdeMetaPropertyDirectory::Ref pProperty;
	const igdeMetaContext::Ref pContext;
	decString pOldValue, pNewValue;
	
public:
	cUndo(igdeMetaPropertyDirectory &property, const igdeMetaContext::Ref &context,
		const char *newValue) :
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
	igdeMetaPropertyDirectoryWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyDirectoryWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyDirectory &GetPropertyDirectory() const{ return pWidget.GetPropertyDirectory(); }
	
	void OnValueChanged(const decString &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		if(!context){
			return;
		}
		
		auto &property = pWidget.GetPropertyDirectory();
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
		pHelper.OnValueChanged(pHelper.GetPropertyDirectory().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyDirectoryWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertyDirectoryWidget::PropertyListener::PropertyListener(igdeMetaPropertyDirectoryWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyDirectoryWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyDirectoryWidget::PropertyListener::OnValueChanged(igdeMetaPropertyDirectory*, const igdeMetaContext::Ref&){
	pWidget.Update();
}

void igdeMetaPropertyDirectoryWidget::PropertyListener::OnPropertyContextChanged(
igdeMetaPropertyDirectory *property, const igdeMetaContext::Ref &context){
	pWidget.SetContext(property->GetPropertyContext(context));
}


// Class igdeMetaPropertyDirectoryWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDirectoryWidget::igdeMetaPropertyDirectoryWidget(igdeMetaPropertyDirectory &property) :
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

void igdeMetaPropertyDirectoryWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditDirectory)
	
	CreateLabel(container, helper);
	
	igdeContainer::Ref line;
	helper.FormLineStretchFirst(container, line);
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditDirectory(container, pPropertyDirectory.GetDescription(),
		pEditDirectory, pListener, pPropertyDirectory.GetUseGameVFS());
	
	CreateContextMenuButton(line, helper);
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
	if(pEditDirectory){
		RunWithPreventUpdate([&]{
			pEditDirectory->SetDirectory(GetContext()
				? pPropertyDirectory.GetPropertyValue(GetContext())
				: pPropertyDirectory.GetDefaultValue());
		});
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyDirectoryWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyDirectoryWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pEditDirectory){
		pEditDirectory->SetVisible(!GetFilteredOut());
	}
}
