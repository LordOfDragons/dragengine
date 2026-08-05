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

#include "igdeMetaPropertyGroupWidget.h"
#include "state/igdeMetaPropertyWidgetStateGroup.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"


// Class igdeMetaPropertyGroupWidget
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyGroupWidget::igdeMetaPropertyGroupWidget(
	igdeMetaPropertyGroup &property) :
igdeMetaPropertyWidget(property),
pPropertyGroup(property){
}

igdeMetaPropertyGroupWidget::~igdeMetaPropertyGroupWidget(){
	Drop();
}


// Management
///////////////

void igdeMetaPropertyGroupWidget::Create(Builder &builder, bool){
	DEASSERT_NULL(pGroupBox)
	
	igdeEnvironment &env = builder.GetHelper().GetEnvironment();
	
	auto state = pPropertyGroup.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateGroup>();
	if(!state){
		state = igdeMetaPropertyWidgetStateGroup::Ref::New();
		state->collapsed = pPropertyGroup.GetCollapsed();
		pPropertyGroup.SetWidgetState(state);
	}
	
	pGroupBox = igdeGroupBox::Ref::New(env, pPropertyGroup.GetLabel(), "", state->collapsed);
	
	pGroupBoxContainer = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pGroupBox->AddChild(pGroupBoxContainer);
	
	builder.OpenGroup(pGroupBox, pGroupBoxContainer);
	auto restoreCollectWidgets = builder.GetCollectWidgets();
	builder.SetCollectWidgets(&pChildWidgets);
	builder.CreatePropertyWidgets(pPropertyGroup.GetProperties());
	builder.SetCollectWidgets(restoreCollectWidgets);
	builder.CloseGroup();
	
	UpdateMatchable();
	SetContext(builder.GetContext());
}

void igdeMetaPropertyGroupWidget::Filter(const igdeFilter &filter){
	bool anyNotFilteredOut = false;
	pChildWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.Filter(filter);
		if(widget.GetProperty()->GetCanHideGroup()){
			anyNotFilteredOut |= !widget.GetFilteredOut();
		}
	});
	
	if(filter && anyNotFilteredOut){
		if(!pHasRestoreCollapsed){
			pRestoreCollapsed = pGroupBox->GetCollapsed();
			pHasRestoreCollapsed = true;
		}
		pGroupBox->SetCollapsed(false);
		
	}else if(pHasRestoreCollapsed){
		pGroupBox->SetCollapsed(pRestoreCollapsed);
		pHasRestoreCollapsed = false;
	}
	
	SetFilteredOut(filter && !anyNotFilteredOut);
}

void igdeMetaPropertyGroupWidget::Drop(){
	if(pGroupBox){
		auto state = pPropertyGroup.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateGroup>();
		if(state){
			state->collapsed = pGroupBox->GetCollapsed();
		}
	}
	
	pChildWidgets.Visit([](igdeMetaPropertyWidget &widget){
		widget.Drop();
	});
	pChildWidgets.RemoveAll();
	
	pGroupBoxContainer.Clear();
	pGroupBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyGroupWidget::Update(){
}

void igdeMetaPropertyGroupWidget::OnActivate(){
	auto state = pPropertyGroup.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateGroup>();
	if(!state){
		state = igdeMetaPropertyWidgetStateGroup::Ref::New();
		state->collapsed = pPropertyGroup.GetCollapsed();
		pPropertyGroup.SetWidgetState(state);
	}
	
	if(pGroupBox){
		pGroupBox->SetCollapsed(state->collapsed);
	}
	
	pChildWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.OnActivate();
	});
}

void igdeMetaPropertyGroupWidget::OnDeactivate(){
	if(pGroupBox){
		auto state = pPropertyGroup.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateGroup>();
		if(!state){
			state = igdeMetaPropertyWidgetStateGroup::Ref::New();
			pPropertyGroup.SetWidgetState(state);
		}
		state->collapsed = pGroupBox->GetCollapsed();
	}
	
	pChildWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.OnDeactivate();
	});
}


// Protected Functions
////////////////////////

void igdeMetaPropertyGroupWidget::UpdateFilteredOut(){
	igdeMetaPropertyWidget::UpdateFilteredOut();
	if(pGroupBox){
		pGroupBox->SetVisible(!GetFilteredOut());
	}
}

void igdeMetaPropertyGroupWidget::OnContextChanged(){
	Update();
	
	const auto &context = GetContext();
	pChildWidgets.Visit([&](igdeMetaPropertyWidget &widget){
		widget.SetContext(context);
	});
}
