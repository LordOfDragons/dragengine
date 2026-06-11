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

#include "igdeMetaPropertyDVectorWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../undo/igdeMetaPropertyDVectorUndo.h"


namespace {

class cListenerHelper{
	igdeMetaPropertyDVectorWidget &pWidget;
	
public:
	explicit cListenerHelper(igdeMetaPropertyDVectorWidget &widget) :
	pWidget(widget){
	}
	
	inline igdeMetaPropertyDVector &GetPropertyDVector() const{ return pWidget.GetPropertyDVector(); }
	
	void OnValueChanged(const decDVector &newValue){
		if(pWidget.GetPreventUpdate()){
			return;
		}
		
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyDVector();
		const auto &oldValue = property.GetPropertyValue(context);
		if(newValue.IsEqualTo(oldValue)){
			return;
		}
		
		property.ChangePropertyValue(context, newValue);
	}
};


class cListener : public igdeEditDVectorListener{
	cListenerHelper pHelper;
	
public:
	explicit cListener(igdeMetaPropertyDVectorWidget &widget) :
	pHelper(widget){
	}
	
	~cListener() override = default;
	
	void OnDVectorChanged(igdeEditDVector *editDVector) override{
		pHelper.OnValueChanged(editDVector->GetDVector());
	}
};


class cActionResetToDefault : public igdeAction{
	cListenerHelper pHelper;
	
public:
	cActionResetToDefault(igdeMetaPropertyDVectorWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pHelper(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		pHelper.OnValueChanged(pHelper.GetPropertyDVector().GetDefaultValue());
	}
};

}


// Class igdeMetaPropertyDVectorWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyDVectorWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyDVectorWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyDVectorWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyDVectorWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyDVector*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyDVectorWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyDVectorWidget::igdeMetaPropertyDVectorWidget(
	igdeMetaPropertyDVector &property, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget(property, context),
pPropertyDVector(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyDVector.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyDVectorWidget::~igdeMetaPropertyDVectorWidget(){
	Drop();
	pPropertyDVector.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyDVectorWidget::Create(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pEditDVector)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.EditDVector(pPropertyDVector.GetDescription(), 6, pPropertyDVector.GetPrecision(),
		pEditDVector, pListener);
	WrapEditWidget(container, helper, pEditDVector);
	
	UpdateMatchable(container);
	
	Update();
}

void igdeMetaPropertyDVectorWidget::Drop(){
	if(pEditDVector && pListener){
		pEditDVector->RemoveListener(pListener);
	}
	pListener.Clear();
	pEditDVector.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyDVectorWidget::Update(){
	if(pEditDVector){
		RunWithPreventUpdate([&]{
			pEditDVector->SetDVector(GetContext()
				? pPropertyDVector.GetPropertyValue(GetContext())
				: pPropertyDVector.GetDefaultValue());
		});
	}
}

void igdeMetaPropertyDVectorWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	igdeMetaPropertyWidget::AddContextMenuEntries(contextMenu);
	contextMenu.GetEnvironment().GetUIHelper().MenuCommand(contextMenu,
		deTObjectReference<cActionResetToDefault>::New(*this));
}
