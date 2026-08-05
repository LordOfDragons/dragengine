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

#include "igdeWPMetaContext.h"
#include "../igdeUIHelper.h"
#include "../igdeLabel.h"
#include "../layout/igdeContainerForm.h"
#include "../../meta/property/igdeMetaProperty.h"
#include "../../meta/property/igdeMetaPropertyGroup.h"
#include "../../meta/property/widget/igdeMetaPropertyWidget.h"
#include "../../meta/property/widget/igdeMetaPropertyGroupWidget.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


// Class igdeWPMetaContext::Builder
/////////////////////////////////////

igdeWPMetaContext::Builder::Builder(igdeUIHelper &helper, igdeContainer *container,
	igdeMetaPropertyWidget::List *collectWidgets,
	igdeWPMetaContext::PropertyWidgetCache &widgetCache, const igdeMetaContext::Ref &context) :
igdeMetaPropertyWidget::Builder(helper, collectWidgets, context),
pWidgetCache(widgetCache)
{
	pContainerStack.Add(container);
}

igdeWPMetaContext::Builder::~Builder() = default;


void igdeWPMetaContext::Builder::AddLine(igdeWidget *label, igdeWidget *edit){
	DEASSERT_NOTNULL(label)
	DEASSERT_NOTNULL(edit)
	
	if(!pForm){
		pForm = igdeContainerForm::Ref::New(GetHelper().GetEnvironment());
		pContainerStack.Last()->AddChild(pForm);
	}
	pForm->AddChild(label);
	pForm->AddChild(edit);
}

void igdeWPMetaContext::Builder::AddLine(igdeWidget *edit){
	DEASSERT_NOTNULL(edit)
	
	pForm.Clear();
	pContainerStack.Last()->AddChild(edit);
}

void igdeWPMetaContext::Builder::OpenGroup(igdeWidget *group, igdeContainer *container){
	DEASSERT_NOTNULL(container)
	
	AddLine(group);
	pContainerStack.Add(container);
}

void igdeWPMetaContext::Builder::CloseGroup(){
	pContainerStack.RemoveLast();
	pForm.Clear();
}

void igdeWPMetaContext::Builder::CreatePropertyWidgets(const igdeMetaProperty::List &properties){
	if(properties.IsEmpty()){
		return;
	}
	
	properties.Visit([&](const igdeMetaProperty::Ref &property){
		auto widget = pWidgetCache.GetAtOrDefault(property);
		if(!widget){
			widget = property->CreateWidget();
			if(!widget){
				return;
			}
			pWidgetCache.SetAt(property, widget);
		}
		
		widget->Create(*this, property->GetHideLabel());
		CollectWidget(widget);
	});
}


// Class igdeWPMetaContext::ContextListener
/////////////////////////////////////////////

igdeWPMetaContext::ContextListener::ContextListener(igdeWPMetaContext &panel) :
pPanel(panel){
}

igdeWPMetaContext::ContextListener::~ContextListener() = default;

void igdeWPMetaContext::ContextListener::OnPropertiesChanged(igdeMetaContext*){
	pPanel.UpdatePropertyWidgets();
}


// Class igdeWPMetaContext
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPMetaContext::igdeWPMetaContext(igdeEnvironment &environment) :
igdeContainerFlow(environment, igdeContainerFlow::eaY),
pContextListener(ContextListener::Ref::New(*this)){
}

igdeWPMetaContext::~igdeWPMetaContext(){
	SetContext({});
}


// Management
///////////////

void igdeWPMetaContext::SetContext(const igdeMetaContext::Ref &context){
	if(pContext == context){
		return;
	}
	
	if(pContext){
		pContext->GetListeners().Remove(pContextListener);
	}
	
	pContext = context;
	
	if(context){
		context->GetListeners().Add(pContextListener);
	}
	
	SetProperties(context ? context->GetProperties() : igdeMetaContext::PropertyList::Ref());
	
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.SetContext(context);
	});
}

void igdeWPMetaContext::SetProperties(const igdeMetaContext::PropertyList::Ref &properties){
	if(pProperties == properties){
		return;
	}
	
	pProperties = properties;
	UpdatePropertyWidgets();
}

void igdeWPMetaContext::SetFilter(const igdeFilter &filter){
	if(filter == pFilter){
		return;
	}
	
	pFilter = filter;
	pFilterPropertyWidgets();
}

void igdeWPMetaContext::SetFilter(igdeFilter &&filter){
	if(filter == pFilter){
		return;
	}
	
	pFilter = std::move(filter);
	pFilterPropertyWidgets();
}

bool igdeWPMetaContext::HasVisibleWidgets() const{
	return pPropertyWidgets.HasMatching([](const igdeMetaPropertyWidget &widget){
		return !widget.GetFilteredOut();
	});
}

void igdeWPMetaContext::UpdatePropertyWidgets(){
	pClearPropertyWidgets();
	if(pProperties){
		Builder(GetEnvironment().GetUIHelperProperties(), this, &pPropertyWidgets,
			pPropertyWidgetCache, pContext).CreatePropertyWidgets(pProperties->GetData());
	}
	
	if(pFilter){
		pFilterPropertyWidgets();
	}
}

void igdeWPMetaContext::OnLanguageChanged(){
	igdeContainerFlow::OnLanguageChanged();
	
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.UpdateMatchable();
	});
	pFilterPropertyWidgets();
}

void igdeWPMetaContext::OnActivate(){
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.OnActivate();
	});
}

void igdeWPMetaContext::OnDeactivate(){
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.OnDeactivate();
	});
}


// Private Functions
//////////////////////

void igdeWPMetaContext::pClearPropertyWidgets(){
	pPropertyWidgets.Visit([](igdeMetaPropertyWidget &widget){
		widget.Drop();
	});
	pPropertyWidgets.RemoveAll();
	RemoveAllChildren();
}

void igdeWPMetaContext::pFilterPropertyWidgets(){
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.Filter(pFilter);
	});
}
