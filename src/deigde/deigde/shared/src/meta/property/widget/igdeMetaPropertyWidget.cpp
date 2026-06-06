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

#include "igdeMetaPropertyWidget.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../utils/igdeFilter.h"


// Class igdeMetaPropertyWidget
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyWidget::igdeMetaPropertyWidget(igdeMetaProperty &property) :
pProperty(&property){
}

igdeMetaPropertyWidget::~igdeMetaPropertyWidget() = default;


// Management
///////////////

void igdeMetaPropertyWidget::SetContext(const igdeMetaContext::Ref &context){
	if(pContext == context){
		return;
	}
	
	pContext = context;
	OnContextChanged();
	Update();
}

void igdeMetaPropertyWidget::SetFilteredOut(bool filteredOut){
	if(pFilteredOut == filteredOut){
		return;
	}
	
	pFilteredOut = filteredOut;
	UpdateFilteredOut();
}

void igdeMetaPropertyWidget::Filter(const igdeFilter &filter){
	SetFilteredOut(filter && filter.MatchesNot(GetProperty()->GetMatchable()));
}


void igdeMetaPropertyWidget::Drop(){
	pLabel.Clear();
	pButtonContextMenu.Clear();
}


void igdeMetaPropertyWidget::RunWithPreventUpdate(const std::function<void()> &function){
	const bool previousPreventUpdate = pPreventUpdate;
	try{
		pPreventUpdate = true;
		function();
		pPreventUpdate = previousPreventUpdate;
		
	}catch(...){
		pPreventUpdate = previousPreventUpdate;
		throw;
	}
}


// Protected Functions
////////////////////////

void igdeMetaPropertyWidget::OnContextChanged(){
}

void igdeMetaPropertyWidget::CreateLabel(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pLabel)
	
	helper.Label(container, pLabel, pProperty->GetLabel(), pProperty->GetDescription(),
		igdeLabel::eaLeft | igdeLabel::eaMiddle);
}

void igdeMetaPropertyWidget::CreateContextMenuButton(igdeContainer &container, igdeUIHelper &helper){
	DEASSERT_NULL(pButtonContextMenu)
	
	auto action = igdeActionContextMenu::Ref::New("",
		container.GetEnvironment().GetStockIcon(igdeEnvironment::esiEdit),
		"@Igde.MetaProperty.ContextMenu.ToolTip");
	
	helper.Button(container, pButtonContextMenu, action);
	action->SetWidget(pButtonContextMenu);
}

void igdeMetaPropertyWidget::OnContextMenuButton(){
	auto menu = igdeMenuCascade::Ref::New(pButtonContextMenu->GetEnvironment());
	AddContextMenuEntries(*menu);
	pProperty->AddContextMenuEntries(*menu, pContext);
	
	if(menu->GetChildren().IsNotEmpty()){
		menu->Popup(pButtonContextMenu);
	}
}

void igdeMetaPropertyWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
}

void igdeMetaPropertyWidget::UpdateFilteredOut(){
	if(pLabel){
		pLabel->SetVisible(!pFilteredOut);
	}
	if(pButtonContextMenu){
		pButtonContextMenu->SetVisible(!pFilteredOut);
	}
}
