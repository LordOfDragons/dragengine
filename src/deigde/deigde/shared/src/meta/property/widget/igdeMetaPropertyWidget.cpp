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


namespace{

class cActionEditMenu : public igdeActionContextMenu{
	igdeMetaPropertyWidget &pWidget;
public:
	cActionEditMenu(igdeMetaPropertyWidget &widget, igdeEnvironment &env) :
	igdeActionContextMenu("", env.GetStockIcon(igdeEnvironment::esiEdit),
		"@Igde.MetaProperty.ContextMenu.ToolTip"),
	pWidget(widget){}
	
	void AddContextMenuEntries(igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntries(menu);
	}
};

}


// Class igdeMetaPropertyWidget
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyWidget::igdeMetaPropertyWidget(
	igdeMetaProperty &property, const igdeMetaContext::Ref &context) :
pProperty(&property),
pContext(context){
}

igdeMetaPropertyWidget::~igdeMetaPropertyWidget() = default;


// Management
///////////////

igdeEnvironment &igdeMetaPropertyWidget::GetEnvironment() const{
	if(pLabel){
		return pLabel->GetEnvironment();
		
	}else if(pEditContainer){
		return pEditContainer->GetEnvironment();
		
	}else if(pButtonContextMenu){
		return pButtonContextMenu->GetEnvironment();
		
	}else{
		DETHROW(deeInvalidAction);
	}
}

void igdeMetaPropertyWidget::SetFilteredOut(bool filteredOut){
	if(pFilteredOut == filteredOut){
		return;
	}
	
	pFilteredOut = filteredOut;
	UpdateFilteredOut();
}

void igdeMetaPropertyWidget::Filter(const igdeFilter &filter){
	SetFilteredOut(filter && pMatchable && filter.MatchesNot(pMatchable));
}

void igdeMetaPropertyWidget::UpdateMatchable(igdeContainer &container){
	if(pLabel){
		pMatchable = igdeFilter::Matchable(container.TranslateIf(pProperty->GetFilter().IsEmpty()
			? pProperty->GetLabel() : pProperty->GetFilter()).ToUTF8());
		
	}else{
		pMatchable = {};
	}
}

void igdeMetaPropertyWidget::Drop(){
	pLabel.Clear();
	pEditContainer.Clear();
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


void igdeMetaPropertyWidget::AddContextMenuEntries(igdeMenuCascade &contextMenu){
	pProperty->AddContextMenuEntries(contextMenu, pContext, pEditContainer);
}


// Protected Functions
////////////////////////

void igdeMetaPropertyWidget::WrapEditWidget(igdeContainer &container, igdeUIHelper &helper,
bool noLabel, igdeWidget *widget, igdeWidget *sideWidget){
	DEASSERT_NOTNULL(widget)
	DEASSERT_NULL(pLabel)
	DEASSERT_NULL(pButtonContextMenu)
	DEASSERT_NULL(pEditContainer)
	
	// label on left side
	if(!noLabel){
		helper.Label(container, pLabel, pProperty->GetLabel(), pProperty->GetDescription(),
			igdeLabel::eaLeft | igdeLabel::eaTop);
	}
	
	// edit widget in the center
	pEditContainer = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 5);
	
	pEditContainer->AddChild(widget);
	
	// context menu button on the right side
	auto column = igdeContainerFlow::Ref::New(helper.GetEnvironment(), igdeContainerFlow::eaY,
		sideWidget ? igdeContainerFlow::esLast : igdeContainerFlow::esNone);
	
	auto action = deTObjectReference<cActionEditMenu>::New(*this, container.GetEnvironment());
	helper.Button(column, pButtonContextMenu, action);
	pButtonContextMenu->SetStyle(igdeButton::ebsToolBar);
	action->SetWidget(pButtonContextMenu);
	
	if(sideWidget){
		column->AddChild(sideWidget);
	}
	
	pEditContainer->AddChild(column);
	
	container.AddChild(pEditContainer);
}

void igdeMetaPropertyWidget::UpdateFilteredOut(){
	if(pLabel){
		pLabel->SetVisible(!pFilteredOut);
	}
	if(pEditContainer){
		pEditContainer->SetVisible(!pFilteredOut);
	}
}
