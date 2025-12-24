/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeDialogBrowser.h"
#include "igdeBrowseItemGDPreviewListener.h"
#include "../igdeApplication.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeButton.h"
#include "../igdeContainer.h"
#include "../igdeComboBox.h"
#include "../igdeGroupBox.h"
#include "../igdeIconListBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeTreeList.h"
#include "../igdeSwitcher.h"
#include "../event/igdeComboBoxListener.h"
#include "../event/igdeIconListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../event/igdeTreeListListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionContextMenu.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerSplitted.h"
#include "../menu/igdeMenuCascade.h"
#include "../model/igdeListItem.h"
#include "../model/igdeTreeItem.h"
#include "../resources/igdeIcon.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/igdeGDCategory.h"
#include "../../gamedefinition/visitor/igdeGDAddToListVisitor.h"
#include "../../gamedefinition/preview/igdeGDPreviewManager.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeDialogBrowser_TreeCategories : public igdeTreeListListener{
	igdeDialogBrowser &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_TreeCategories> Ref;
	
	igdeDialogBrowser_TreeCategories(igdeDialogBrowser &dialog) : pDialog(dialog){}
	
	virtual void OnSelectionChanged(igdeTreeList*){
		pDialog.UpdateItemList();
	}
};


class igdeDialogBrowser_TextFilter : public igdeTextFieldListener{
	igdeDialogBrowser &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_TextFilter> Ref;
	
	igdeDialogBrowser_TextFilter(igdeDialogBrowser &dialog) : pDialog(dialog){}
	
	virtual void OnTextChanged(igdeTextField*){
		pDialog.UpdateItemList();
	}
	
	virtual void OnTextChanging(igdeTextField*){
		pDialog.UpdateItemList();
	}
};


class igdeDialogBrowser_ActionPreviewSize : public igdeAction{
	igdeDialogBrowser &pDialog;
	const igdeDialogBrowser::ePreviewSize pSize;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_ActionPreviewSize> Ref;
	
	igdeDialogBrowser_ActionPreviewSize(igdeDialogBrowser &dialog, igdeDialogBrowser::ePreviewSize size,
	const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pDialog(dialog), pSize(size){}
	
	void OnAction() override{
		pDialog.SetPreviewSize(pSize);
	}
	
	void Update() override{
		SetSelected(pDialog.GetPreviewSize() == pSize);
	}
};

class igdeDialogBrowser_ActionViewMode : public igdeAction{
	igdeDialogBrowser &pDialog;
	const igdeDialogBrowser::eViewModes pMode;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_ActionViewMode> Ref;
	
	igdeDialogBrowser_ActionViewMode(igdeDialogBrowser &dialog, igdeDialogBrowser::eViewModes mode,
	const char *text, igdeIcon *icon, const char *description) :
		igdeAction(text, icon, description), pDialog(dialog), pMode(mode){}
	
	void OnAction() override{
		pDialog.SetViewMode(pMode);
	}
	
	void Update() override{
		SetSelected(pDialog.GetViewMode() == pMode);
	}
};

class igdeDialogBrowser_ActionPIRebuild : public igdeAction{
	igdeDialogBrowser &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_ActionPIRebuild> Ref;
	
	igdeDialogBrowser_ActionPIRebuild(igdeDialogBrowser &dialog) : igdeAction(
		"Rebuild Preview", nullptr, "Rebuild Preview"), pDialog(dialog){}
	
	void OnAction() override{
		pDialog.RebuildPISelectedItem();
	}
	
	void Update() override{
		SetEnabled(pDialog.GetSelectedListItem());
	}
};

class igdeDialogBrowser_ListItems : public igdeIconListBoxListener{
	igdeDialogBrowser &pDialog;
	
public:
	typedef deTObjectReference<igdeDialogBrowser_ListItems> Ref;
	
	igdeDialogBrowser_ListItems(igdeDialogBrowser &dialog) : pDialog(dialog){}
	
	virtual void OnSelectionChanged(igdeIconListBox*){
		pDialog.CurrentItemChanged();
	}
	
	virtual void OnDoubleClickItem(igdeIconListBox*, int){
		pDialog.Accept();
	}
	
	virtual void AddContextMenuEntries(igdeIconListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuOption(menu, pDialog.GetActionPISizeLarge());
		helper.MenuOption(menu, pDialog.GetActionPISizeMedium());
		helper.MenuOption(menu, pDialog.GetActionPISizeSmall());
		
		helper.MenuSeparator(menu);
		helper.MenuOption(menu, pDialog.GetActionPIViewPreview());
		helper.MenuOption(menu, pDialog.GetActionPIViewList());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, igdeDialogBrowser_ActionPIRebuild::Ref::New(pDialog));
	}
};




// Class igdeDialogBrowser
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogBrowser::igdeDialogBrowser(igdeEnvironment &environment, const char *title, bool canResize) :
igdeDialog(environment, title, nullptr, canResize),
pPreviewSize(epsLarge),
pViewMode(evmPreview)
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(1000, 600)));
	
	pActionPISizeSmall = igdeDialogBrowser_ActionPreviewSize::Ref::New(*this, epsSmall,
		"Small Preview Image", nullptr, "Small Preview Image");
	pActionPISizeMedium = igdeDialogBrowser_ActionPreviewSize::Ref::New(*this, epsMedium,
		"Medium Preview Image", nullptr, "Medium Preview Image");
	pActionPISizeLarge = igdeDialogBrowser_ActionPreviewSize::Ref::New(*this, epsLarge,
		"Large Preview Image", nullptr, "Large Preview Image");
	
	pActionPIViewList = igdeDialogBrowser_ActionViewMode::Ref::New(*this, evmList,
		"List Mode", nullptr, "List Mode");
	pActionPIViewPreview = igdeDialogBrowser_ActionViewMode::Ref::New(*this, evmPreview,
		"Preview Mode", nullptr, "Preview Mode");
	
	
	igdeContainerSplitted::Ref content(igdeContainerSplitted::Ref::New(
		environment, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(250)));
	
	// left side: category list with filter
	igdeContainerFlow::Ref panelCategory(igdeContainerFlow::Ref::New(
		environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 3));
	
	igdeContainerForm::Ref filterLine(igdeContainerForm::Ref::New(environment));
	helper.EditString(filterLine, "Filter:", "Show items containing filter case insensitive",
		pEditFilter, igdeDialogBrowser_TextFilter::Ref::New(*this));
	panelCategory->AddChild(filterLine);
	
	helper.TreeList(panelCategory, pTreeCategories, 10, "Categories", igdeDialogBrowser_TreeCategories::Ref::New(*this));
	pTreeCategories->SetDefaultSorter();
	
	content->AddChild(panelCategory, igdeContainerSplitted::eaSide);
	
	// right side: item list with information
	igdeContainerFlow::Ref panelItems(igdeContainerFlow::Ref::New(
		environment, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 3));
	
	const igdeUIHelper::sColumnHeader headers[] = {
		igdeUIHelper::sColumnHeader("Name", nullptr, igdeApplication::app().DisplayScaled(200))
	};
	helper.IconListBox(panelItems, pListItems,
		igdeApplication::app().DisplayScaled(decPoint(100, 200)),
		headers, 1, "Items", igdeDialogBrowser_ListItems::Ref::New(*this));
	pListItems->SetDefaultSorter();
	pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
	
	helper.EditString(panelItems, "Item information", pEditInfos, 50, 5, {});
	pEditInfos->SetEditable(false);
	
	content->AddChild(panelItems, igdeContainerSplitted::eaCenter);
	
	
	// buttons at bottom
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Discard");
	
	AddContent(content, buttonBar);
}

igdeDialogBrowser::~igdeDialogBrowser(){
}



// Management
///////////////

void igdeDialogBrowser::UpdateCategoryList(){
	igdeGDCategory * const selection = GetSelectedCategory();
	
	pTreeCategories->RemoveAllItems();
	UpdateCategoryListWith(GetRootCategory());
	pTreeCategories->SortAllItems();
	
	SelectCategory(selection);
}

void igdeDialogBrowser::UpdateCategoryListWith(igdeGDCategory *category){
	DEASSERT_NOTNULL(category)
	
	pTreeCategories->AppendItem(nullptr, "< No Category >");
	
	category->GetCategories().Visit([&](igdeGDCategory *c){
		if(!c->GetHidden()){
			AddCategoryToList(c, nullptr);
		}
	});
}

void igdeDialogBrowser::AddCategoryToList(igdeGDCategory *category, igdeTreeItem *parent){
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	igdeTreeItem::Ref item(igdeTreeItem::Ref::New(category->GetName(), category));
	pTreeCategories->AppendItem(parent, item);
	
	category->GetCategories().Visit([&](igdeGDCategory *c){
		if(!c->GetHidden()){
			AddCategoryToList(c, item);
		}
	});
}

void igdeDialogBrowser::UpdateItemList(){
	void * const selection = pListItems->GetSelectedItem() ? pListItems->GetSelectedItem()->GetData() : nullptr;
	
	pListItems->RemoveAllItems();
	
	igdeGDAddToListVisitor visitor(GetEnvironment(), pListItems, GetPreviewIconSize());
	AddItemsToList(visitor);
	pListItems->SortItems();
	
	pListItems->SetSelectionWithData(selection);
	if(!pListItems->GetSelectedItem() && pListItems->GetItems().IsNotEmpty()){
		pListItems->SetSelection(0);
	}
}

void igdeDialogBrowser::RebuildPISelectedItem(){
	igdeListItem * const item = GetSelectedListItem();
	if(!item){
		return;
	}
	
	igdeGDPreviewManager &pvmgr = *GetEnvironment().GetGDPreviewManager();
	
	igdeIcon::Ref icon;
	const int iconSize = GetPreviewIconSize();
	icon = igdeIcon::Ref::New(*pvmgr.GetImageCreating(), iconSize, iconSize);
	
	const igdeBrowseItemGDPreviewListener::Ref listener(
		igdeBrowseItemGDPreviewListener::Ref::New(pListItems, item, iconSize));
	
	item->SetIcon(icon);
	pListItems->ItemChangedAt(pListItems->GetSelection());
	RebuildItemPreview(pvmgr, (igdeGDPreviewListener*)(deObject*)listener);
}

void igdeDialogBrowser::CurrentItemChanged(){
	decString info;
	GetSelectedItemInfo(info);
	pEditInfos->SetText(info);
}

igdeGDCategory *igdeDialogBrowser::GetSelectedCategory() const{
	const igdeTreeItem * const selection = pTreeCategories->GetSelection();
	return selection ? (igdeGDCategory*)selection->GetData() : nullptr;
}

void igdeDialogBrowser::SelectCategory(igdeGDCategory *category){
	pTreeCategories->SetSelectionWithData(category);
	if(!pTreeCategories->GetSelection() && pTreeCategories->GetFirstChild()){
		pTreeCategories->SetSelection(pTreeCategories->GetFirstChild());
	}
}

igdeListItem *igdeDialogBrowser::GetSelectedListItem() const{
	return pListItems->GetSelectedItem();
}

void igdeDialogBrowser::SelectListItemWithData(void *data){
	pListItems->SetSelectionWithData(data);
}

void igdeDialogBrowser::SetPreviewSize(ePreviewSize size){
	if(size == pPreviewSize){
		return;
	}
	
	pPreviewSize = size;
	UpdateItemList();
}

void igdeDialogBrowser::SetViewMode(eViewModes viewMode){
	if(viewMode == pViewMode){
		return;
	}
	
	pViewMode = viewMode;
	
	switch(viewMode){
	case evmList:
		pListItems->SetViewMode(igdeIconListBox::evmList);
		break;
		
	case evmPreview:
		pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
		break;
		
	default:
		pListItems->SetViewMode(igdeIconListBox::evmIconVertical);
	}
}

int igdeDialogBrowser::GetPreviewIconSize() const{
	switch(pPreviewSize){
	case epsLarge:
		return 128;
		
	case epsMedium:
		return 96;
		
	case epsSmall:
		return 64;
	}
	
	return 128;
}

const decString &igdeDialogBrowser::GetFilter() const{
	return pEditFilter->GetText();
}

void igdeDialogBrowser::SetFilter(const char *filter){
	pEditFilter->SetText(filter);
}
