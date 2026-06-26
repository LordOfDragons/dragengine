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

#include "igdeMetaPropertyTreeListWidget.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cTreeListListener : public igdeTreeListListener{
	igdeMetaPropertyTreeListWidget &pWidget;
	
public:
	explicit cTreeListListener(igdeMetaPropertyTreeListWidget &widget) :
	pWidget(widget){
	}
	
	~cTreeListListener() override = default;
	
	void OnSelectionChanged(igdeTreeList *treeList) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreActiveObject();
		}
	}
	
	void OnItemExpanded(igdeTreeList*, igdeTreeItem *item) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreExpandedState(item->GetRefData(), true);
		}
	}
	
	void OnItemCollapsed(igdeTreeList*, igdeTreeItem *item) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreExpandedState(item->GetRefData(), false);
		}
	}
	
	void OnDoubleClickItem(igdeTreeList *treeList, igdeTreeItem *item) override{
	}
	
	void AddContextMenuEntries(igdeTreeList *treeList, igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntries(menu);
	}
};

}


// Class igdeMetaPropertyTreeListWidget::PropertyListener
///////////////////////////////////////////////////////////

igdeMetaPropertyTreeListWidget::PropertyListener::PropertyListener(
igdeMetaPropertyTreeListWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyTreeListWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyTreeListWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyTreeList*, const igdeMetaContext::Ref &context){
	if(!context || pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyTreeListWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyTreeList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveObject();
	}
}

void igdeMetaPropertyTreeListWidget::PropertyListener::OnExpandedChanged(
igdeMetaPropertyTreeList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreExpanded();
	}
}

void igdeMetaPropertyTreeListWidget::PropertyListener::OnObjectItemInfoChanged(
igdeMetaPropertyTreeList*, const igdeMetaContext::Ref &context){
	if(!context || pWidget.GetContext() == context){
		pWidget.UpdateItemInfo();
	}
}


// Class igdeMetaPropertyTreeListWidget
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyTreeListWidget::igdeMetaPropertyTreeListWidget(igdeMetaPropertyTreeList &property) :
igdeMetaPropertyWidget(property),
pPropertyTreeList(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyTreeListWidget::~igdeMetaPropertyTreeListWidget(){
	Drop();
	pPropertyTreeList.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyTreeListWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pTreeList);
	
	auto &helper = builder.GetHelper();
	pTreeListListener = deTObjectReference<cTreeListListener>::New(*this);
	helper.TreeList(pPropertyTreeList.GetRows(), pPropertyTreeList.GetDescription(),
		pTreeList, pTreeListListener);
	if(pPropertyTreeList.GetSorted()){
		pTreeList->SetDefaultSorter();
	}
	pTreeList->SetEnabled(false);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyTreeList::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyTreeList::TargetButton::remove, helper, buttons);
	pAddButton(igdeMetaPropertyTreeList::TargetButton::moveUp, helper, buttons);
	pAddButton(igdeMetaPropertyTreeList::TargetButton::moveDown, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(builder, noLabel, pTreeList, buttons);
	
	UpdateMatchable();
}

void igdeMetaPropertyTreeListWidget::Drop(){
	if(pTreeList && pTreeListListener){
		pTreeList->RemoveListener(pTreeListListener);
	}
	
	pButtonActions.RemoveAll();
	pTreeListListener.Clear();
	pTreeList.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyTreeListWidget::Update(){
	if(!pTreeList){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyTreeList.IsValid(context);
	RunWithPreventUpdate([&]{
		if(valid){
			auto walker = pPropertyTreeList.CreateWalker(context);
			walker->MoveTreeFirst();
			
			pUpdateTree(*walker, pPropertyTreeList.GetExpanded(context));
			
			if(pPropertyTreeList.GetSorted()){
				pTreeList->SortAllItems();
			}
			
		}else{
			pTreeList->RemoveAllItems();
		}
		pTreeList->SetEnabled(valid);
	});
	
	SelectActiveObject();
	RestoreExpanded();
	
	// the tree list automatically selects the first item if the list is not empty.
	// this potentially changes the active object without notification due to
	// RunWithPreventUpdate(). store the current active object to properly
	// synchronize if anything changed
	StoreActiveObject();
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.Update();
	});
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyTreeListWidget::UpdateItemInfo(){
	if(!pTreeList){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyTreeList.IsValid(context);
	if(!valid){
		return;
	}
	
	igdeTreeItem *item = pTreeList->GetFirstChild();
	while(item){
		pUpdateTreeItem(item);
		item = item->GetNext();
	}
	
	if(pPropertyTreeList.GetSorted()){
		pTreeList->SortAllItems();
	}
}

void igdeMetaPropertyTreeListWidget::SelectActiveObject(){
	const auto &context = GetContext();
	if(!pTreeList || !pPropertyTreeList.IsValid(context)){
		return;
	}
	
	auto active = pPropertyTreeList.GetActiveObject(context);
	if(active && pTreeList->HasItemWithRefData(active)){
		pTreeList->SetSelectionWithRefData(active);
		pTreeList->MakeSelectionVisible();
	}
}

void igdeMetaPropertyTreeListWidget::StoreActiveObject(){
	if(!pTreeList || !pPropertyTreeList.IsValid(GetContext())){
		return;
	}
	
	const auto selection = pTreeList->GetSelection();
	pPropertyTreeList.SetActiveObject(GetContext(),
		selection ? selection->GetRefData() : deObject::Ref());
}

void igdeMetaPropertyTreeListWidget::StoreExpandedState(const deObject::Ref &object, bool isExpanded){
	if(!pTreeList || !pPropertyTreeList.IsValid(GetContext()) || !object){
		return;
	}
	
	const auto &context = GetContext();
	auto expanded = pPropertyTreeList.GetExpanded(context);
	if(isExpanded){
		if(!expanded.Add(object)){
			return;
		}
		
	}else{
		if(!expanded.Remove(object)){
			return;
		}
	}
	
	pPropertyTreeList.SetExpanded(context, expanded);
}

void igdeMetaPropertyTreeListWidget::RestoreExpanded(){
	if(!pTreeList || !pPropertyTreeList.IsValid(GetContext())){
		return;
	}
	
	const auto &expanded = pPropertyTreeList.GetExpanded(GetContext());
	igdeTreeItem *item = pTreeList->GetFirstChild();
	while(item){
		pRestoreExpanded(item, expanded);
		item = item->GetNext();
	}
}

void igdeMetaPropertyTreeListWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
}

bool igdeMetaPropertyTreeListWidget::IsPropertyValid() const{
	return pPropertyTreeList.IsValid(GetContext());
}


// Protected Functions
////////////////////////

void igdeMetaPropertyTreeListWidget::OnContextChanged(){
	Update();
	SelectActiveObject();
	RestoreExpanded();
	
	auto &context = GetContext();
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(context);
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyTreeListWidget::pAddButton(igdeMetaPropertyTreeList::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertyTreeList.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}

void igdeMetaPropertyTreeListWidget::pUpdateTree(igdeMetaPropertyTreeList::Walker &walker,
const igdeMetaPropertyTreeList::ExpandedSet &expanded){
	pUpdateTree(walker, expanded, nullptr, pTreeList->GetFirstChild());
}

void igdeMetaPropertyTreeListWidget::pUpdateTree(igdeMetaPropertyTreeList::Walker &walker,
const igdeMetaPropertyTreeList::ExpandedSet &expanded, igdeTreeItem *parent, igdeTreeItem *item){
	auto object = walker.GetCurrent();
	while(object){
		if(!item){
			item = pTreeList->AppendItem(parent, "");
		}
		
		item->SetRefData(object);
		pUpdateTreeItem(item, expanded.Has(object));
		
		if(walker.HasChildren()){
			DEASSERT_TRUE(walker.MoveChildren())
			DEASSERT_NOTNULL(walker.GetCurrent())
			
			pUpdateTree(walker, expanded, item, item->GetFirstChild());
			
			DEASSERT_TRUE(walker.MoveParent())
			DEASSERT_TRUE(walker.GetCurrent() == object)
			
		}else{
			while(item->GetFirstChild()){
				pTreeList->RemoveItem(item->GetFirstChild());
			}
		}
		
		item = item->GetNext();
		
		if(walker.HasNext()){
			DEASSERT_TRUE(walker.MoveNext())
			object = walker.GetCurrent();
			DEASSERT_NOTNULL(object)
			
		}else{
			object.Clear();
		}
	}
	
	while(item){
		auto removeItem = item;
		item = item->GetNext();
		pTreeList->RemoveItem(removeItem);
	}
}

void igdeMetaPropertyTreeListWidget::pUpdateTreeItem(igdeTreeItem *item){
	pUpdateTreeItem(item, item->GetExpanded());
}

void igdeMetaPropertyTreeListWidget::pUpdateTreeItem(igdeTreeItem *item, bool isExpanded){
	pPropertyTreeList.GetObjectItemInfo(GetContext(), item->GetRefData(), pItemInfo);
	
	const auto &text = pItemInfo.GetText();
	const auto &icon = pItemInfo.GetIcon();
	const auto &description = pItemInfo.GetDescription();
	
	if(item->GetText() != text || item->GetIcon() != icon
	|| item->GetDescription() != description || item->GetExpanded() != isExpanded){
		item->SetText(text);
		item->SetIcon(icon);
		item->SetDescription(description);
		item->SetExpanded(isExpanded);
		pTreeList->ItemChanged(item);
	}
}

void igdeMetaPropertyTreeListWidget::pRestoreExpanded(igdeTreeItem *item,
const igdeMetaPropertyTreeList::ExpandedSet &expanded){
	const bool isExpanded = expanded.Has(item->GetRefData());
	if(item->GetExpanded() != isExpanded){
		item->SetExpanded(isExpanded);
		pTreeList->ItemChanged(item);
	}
	
	igdeTreeItem *child = item->GetFirstChild();
	while(child){
		pRestoreExpanded(child, expanded);
		child = child->GetNext();
	}
}
