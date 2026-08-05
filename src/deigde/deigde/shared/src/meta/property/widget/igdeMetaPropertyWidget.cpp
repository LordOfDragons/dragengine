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
#include "../../../localization/igdeTranslationManager.h"
#include "../../../utils/igdeFilter.h"


namespace{

class cActionEditMenu : public igdeActionContextMenu{
	igdeMetaPropertyWidget &pWidget;
public:
	cActionEditMenu(igdeMetaPropertyWidget &widget, igdeEnvironment &env) :
	igdeActionContextMenu("", env.GetStockIcon(igdeEnvironment::esiConfig),
		"@Igde.MetaProperty.ContextMenu.ToolTip"),
	pWidget(widget){}
	
	void AddContextMenuEntries(igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntries(menu);
	}
	
	void Update() override{
		SetEnabled(pWidget.IsPropertyValid());
	}
};

}


// Class igdeMetaPropertyWidget::Builder
//////////////////////////////////////////

igdeMetaPropertyWidget::Builder::Builder(igdeUIHelper &helper,
	igdeMetaPropertyWidget::List *collectWidgets, const igdeMetaContext::Ref &context) :
pHelper(helper),
pCollectWidgets(collectWidgets),
pContext(context){
}

igdeMetaPropertyWidget::Builder::~Builder() = default;

void igdeMetaPropertyWidget::Builder::SetCollectWidgets(igdeMetaPropertyWidget::List *collectWidgets){
	pCollectWidgets = collectWidgets;
}

void igdeMetaPropertyWidget::Builder::CollectWidget(const igdeMetaPropertyWidget::Ref &widget){
	if(pCollectWidgets){
		pCollectWidgets->Add(widget);
	}
}

void igdeMetaPropertyWidget::Builder::SetContext(const igdeMetaContext::Ref &context){
	pContext = context;
}


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

void igdeMetaPropertyWidget::UpdateMatchable(){
	pMatchable = pLabel ? igdeFilter::Matchable(pProperty->RealFilter(GetEnvironment())) : igdeFilter::Matchable();
}


void igdeMetaPropertyWidget::Drop(){
	pLabel.Clear();
	pEditContainer.Clear();
	pButtonContextMenu.Clear();
	pContext.Clear();
}

void igdeMetaPropertyWidget::Update(){
	if(pButtonContextMenu){
		pButtonContextMenu->GetAction()->Update();
	}
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


void igdeMetaPropertyWidget::SetContext(const igdeMetaContext::Ref &context){
	if(pContext == context){
		return;
	}
	
	pContext = context;
	OnContextChanged();
}

void igdeMetaPropertyWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	if(!pEditContainer){
		return;
	}
	
	const auto &helpUrl = pProperty->GetHelpUrl();
	if(!helpUrl.IsEmpty()){
		auto &env = menu.GetEnvironment();
		auto &helper = env.GetUIHelper();
		if(menu.GetChildren().IsNotEmpty()){
			helper.MenuSeparator(menu);
		}
		helper.MenuCommand(menu, deTObjectReference<igdeMetaProperty::ActionHelp>::New(env, pProperty));
	}
	
	if(pContext){
		pProperty->AddContextMenuEntries(menu, pContext, pEditContainer);
	}
}

bool igdeMetaPropertyWidget::IsPropertyValid() const{
	return false;
}

void igdeMetaPropertyWidget::OnActivate(){
}

void igdeMetaPropertyWidget::OnDeactivate(){
}


// Protected Functions
////////////////////////

void igdeMetaPropertyWidget::WrapEditWidget(Builder &builder, bool noLabel,
igdeWidget *widget, igdeWidget *sideWidget){
	DEASSERT_NOTNULL(widget)
	DEASSERT_NULL(pLabel)
	DEASSERT_NULL(pButtonContextMenu)
	DEASSERT_NULL(pEditContainer)
	
	auto &helper = builder.GetHelper();
	auto &env = helper.GetEnvironment();
	
	// label on left side
	if(!noLabel){
		helper.Label(pLabel, pProperty->GetLabel(), pProperty->GetDescription(),
			igdeLabel::eaLeft | igdeLabel::eaTop);
	}
	
	// edit widget in the center
	pEditContainer = igdeContainerFlow::Ref::New(env,
		igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 5);
	
	pEditContainer->AddChild(widget);
	
	// context menu button on the right side
	auto column = sideWidget
		? igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 0, true)
		: igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	
	auto action = deTObjectReference<cActionEditMenu>::New(*this, env);
	helper.Button(column, pButtonContextMenu, action);
	pButtonContextMenu->SetStyle(igdeButton::ebsToolBar);
	action->SetWidget(pButtonContextMenu);
	
	if(sideWidget){
		column->AddChild(sideWidget);
	}
	
	pEditContainer->AddChild(column);
	
	if(pLabel){
		builder.AddLine(pLabel, pEditContainer);
		
	}else{
		builder.AddLine(pEditContainer);
	}
}

void igdeMetaPropertyWidget::UpdateFilteredOut(){
	if(pLabel){
		pLabel->SetVisible(!pFilteredOut);
	}
	if(pEditContainer){
		pEditContainer->SetVisible(!pFilteredOut);
	}
}

void igdeMetaPropertyWidget::OnContextChanged(){
}
