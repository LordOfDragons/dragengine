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

#include "igdeMetaPropertyContextWidget.h"
#include "igdeMetaPropertyGroupWidget.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../gui/layout/igdeContainerFlow.h"


// Class igdeMetaPropertyContextWidget::PropertyListener
//////////////////////////////////////////////////////////

igdeMetaPropertyContextWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyContextWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyContextWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyContextWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyContext*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}


// Class igdeMetaPropertyContextWidget
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyContextWidget::igdeMetaPropertyContextWidget(igdeMetaPropertyContext &property) :
igdeMetaPropertyWidget(property),
pPropertyContext(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	pPropertyContext.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyContextWidget::~igdeMetaPropertyContextWidget(){
	Drop();
	pPropertyContext.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyContextWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pContainer)
	
	pPropertyProperties = pPropertyContext.GetProperties();
	
	if(pPropertyProperties){
		pProperties = pPropertyProperties;
		const auto restoreCollectWidgets = builder.GetCollectWidgets();
		const auto restoreContext = builder.GetContext();
		builder.SetCollectWidgets(&pPropertyWidgets);
		builder.SetContext(pPropertyContext.GetDefaultValue());
		pUpdatePropertyWidgets(builder);
		builder.SetContext(restoreContext);
		builder.SetCollectWidgets(restoreCollectWidgets);
		
	}else{
		pContainer = igdeContainerFlow::Ref::New(builder.GetHelper().GetEnvironment(), igdeContainerFlow::eaY);
		builder.AddLine(pContainer);
	}
	
	SetContext(builder.GetContext());
}

void igdeMetaPropertyContextWidget::Drop(){
	pClearPropertyWidgets();
	pContainer.Clear();
	pPropertyProperties.Clear();
	pProperties.Clear();
	pValueContext.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyContextWidget::Update(){
	const bool valid = pPropertyContext.IsValid(GetContext());
	pValueContext = valid ? pPropertyContext.GetPropertyValue(GetContext())
		: pPropertyContext.GetDefaultValue();
	
	const auto properties = pValueContext ? pValueContext->GetProperties() : pPropertyProperties;
	if(pPropertyProperties){
		if(pPropertyProperties != properties){
			return;
		}
		
	}else if(pProperties != properties){
		pProperties = properties;
		
		if(pContainer){
			igdeWPMetaContext::Builder builder(GetEnvironment().GetUIHelperProperties(),
				pContainer, &pPropertyWidgets, pPropertyWidgetCache, pValueContext);
			pUpdatePropertyWidgets(builder);
		}
	}
	
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.SetContext(pValueContext);
	});
}

void igdeMetaPropertyContextWidget::Filter(const igdeFilter &filter){
	if(pFilter == filter){
		return;
	}
	
	pFilter = filter;
	
	if(filter){
		pFilterPropertyWidgets();
	}
	SetFilteredOut(filter && !pPropertyWidgets.HasMatching([](const igdeMetaPropertyWidget &widget){
		return !widget.GetFilteredOut();
	}));
}

igdeEnvironment &igdeMetaPropertyContextWidget::GetEnvironment() const{
	if(pContainer){
		return pContainer->GetEnvironment();
		
	}else{
		return igdeMetaPropertyWidget::GetEnvironment();
	}
}

bool igdeMetaPropertyContextWidget::IsPropertyValid() const{
	return pPropertyContext.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyContextWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pContainer){
		pContainer->SetVisible(!GetFilteredOut());
	}
}

void igdeMetaPropertyContextWidget::OnContextChanged(){
	Update();
}

void igdeMetaPropertyContextWidget::pUpdatePropertyWidgets(Builder &builder){
	pClearPropertyWidgets();
	if(pProperties){
		builder.CreatePropertyWidgets(pProperties->GetData());
	}
	
	if(pFilter){
		pFilterPropertyWidgets();
	}
}

void igdeMetaPropertyContextWidget::pClearPropertyWidgets(){
	pPropertyWidgets.Visit([](igdeMetaPropertyWidget &widget){
		widget.Drop();
	});
	pPropertyWidgets.RemoveAll();
	if(pContainer){
		pContainer->RemoveAllChildren();
	}
}

void igdeMetaPropertyContextWidget::pFilterPropertyWidgets(){
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.Filter(pFilter);
	});
}
