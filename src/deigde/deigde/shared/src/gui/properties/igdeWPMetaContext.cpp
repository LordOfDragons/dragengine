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
igdeContainerScroll(environment, false, true),
pContextListener(ContextListener::Ref::New(*this))
{
	pCreateContent();
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

void igdeWPMetaContext::UpdatePropertyWidgets(){
	pCreatePropertyWidgets();
	if(pFilter){
		pFilterPropertyWidgets();
	}
}


// Private Functions
//////////////////////

void igdeWPMetaContext::pCreateContent(){
	pPropertyContainer = igdeContainerFlow::Ref::New(GetEnvironment(), igdeContainerFlow::eaY);
	AddChild(pPropertyContainer);
}

void igdeWPMetaContext::pCreatePropertyWidgets(){
	pClearPropertyWidgets();
	
	if(!pContext){
		return;
	}
	
	if(!pProperties || pProperties->GetData().IsEmpty()){
		return;
	}
	
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	auto form = igdeContainerForm::Ref::New(env);
	pPropertyContainer->AddChild(form);
	
	pProperties->GetData().Visit([&](const igdeMetaProperty::Ref &property){
		auto group = property.DynamicCast<igdeMetaPropertyGroup>();
		
		if(group){
			form.Clear();
			auto widget = pCreatePropertyGroupWidget(*this, group);
			if(widget){
				pPropertyWidgets.Add(widget);
			}
			
		}else{
			auto widget = pPropertyWidgetCache.GetAtOrDefault(property);
			if(!widget){
				widget = property->CreateWidget(pContext);
				if(!widget){
					return;
				}
				pPropertyWidgetCache.SetAt(property, widget);
			}
			
			if(!form){
				form = igdeContainerForm::Ref::New(env);
				pPropertyContainer->AddChild(form);
			}
			widget->Create(form, helper);
			pPropertyWidgets.Add(widget);
		}
	});
}

igdeMetaPropertyWidget::Ref igdeWPMetaContext::pCreatePropertyGroupWidget(
igdeContainer &container, const igdeMetaPropertyGroup::Ref &groupProperty){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	auto groupWidget = pPropertyWidgetCache.GetAtOrDefault(groupProperty);
	if(!groupWidget){
		groupWidget = groupProperty->CreateWidget(pContext);
		if(!groupWidget){
			return {};
		}
		pPropertyWidgetCache.SetAt(groupProperty, groupWidget);
	}
	
	groupWidget->Create(container, helper);
	
	auto groupWidget2 = groupWidget.DynamicCast<igdeMetaPropertyGroupWidget>();
	if(!groupWidget2 || !groupWidget2->GetGroupBox() || !groupWidget2->GetGroupBoxContainer()
	|| groupProperty->GetProperties().IsEmpty()){
		return groupWidget;
	}
	
	igdeContainer &groupContainer = groupWidget2->GetGroupBoxContainer();
	auto &childWidgets = groupWidget2->GetChildWidgets();
	igdeContainerForm::Ref form;
	
	groupProperty->GetProperties().Visit([&](const igdeMetaProperty::Ref &property){
		const auto group = property.DynamicCast<igdeMetaPropertyGroup>();
		
		if(group){
			form.Clear();
			auto widget = pCreatePropertyGroupWidget(groupContainer, group);
			if(widget){
				childWidgets.Add(widget);
			}
			
		}else{
			auto widget = pPropertyWidgetCache.GetAtOrDefault(property);
			if(!widget){
				widget = property->CreateWidget(pContext);
				if(!widget){
					return;
				}
				pPropertyWidgetCache.SetAt(property, widget);
			}
			
			if(!form){
				form = igdeContainerForm::Ref::New(env);
				groupContainer.AddChild(form);
			}
			widget->Create(form, helper);
			childWidgets.Add(widget);
		}
	});
	
	return groupWidget;
}

void igdeWPMetaContext::pClearPropertyWidgets(){
	pPropertyWidgets.Visit([](igdeMetaPropertyWidget &widget){
		widget.Drop();
	});
	pPropertyWidgets.RemoveAll();
	pPropertyContainer->RemoveAllChildren();
}

void igdeWPMetaContext::pFilterPropertyWidgets(){
	pPropertyWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.Filter(pFilter);
	});
}
