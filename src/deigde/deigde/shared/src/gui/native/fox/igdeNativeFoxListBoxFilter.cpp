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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxListBoxFilter.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeListBoxFilter.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../model/igdeListItem.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxListBoxFilter) cNativeIgdeListBoxFilterMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListCommand),
	
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListChanged),
	
	FXMAPFUNC(SEL_SELECTED, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListSelected),
	
	FXMAPFUNC(SEL_DESELECTED, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListDeselected),
	
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListRightMouseDown),
	
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListRightMouseUp),
	
	FXMAPFUNC(SEL_DOUBLECLICKED, igdeNativeFoxListBoxFilter::ID_LISTBOX,
		igdeNativeFoxListBoxFilter::onListDoubleClicked),
	
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxListBoxFilter::ID_EDIT_FILTER,
		igdeNativeFoxListBoxFilter::onEditFilterChanged),
	
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxListBoxFilter::ID_RESIZER,
		igdeNativeFoxListBoxFilter::onResizerDrag)
};



FXIMPLEMENT(igdeNativeFoxListBoxFilter, FXVerticalFrame,
	cNativeIgdeListBoxFilterMap, ARRAYNUMBER(cNativeIgdeListBoxFilterMap))

#define MASK_FLAGS_LIST (LIST_SINGLESELECT | LIST_BROWSESELECT | LIST_MULTIPLESELECT | LIST_AUTOSELECT)



// class igdeNativeFoxListBoxFilterItem
/////////////////////////////////////////

class igdeNativeFoxListBoxFilterItem : public FXListItem{
	FXDECLARE(igdeNativeFoxListBoxFilterItem)
	
protected:
	igdeNativeFoxListBoxFilterItem() = default;
	
public:
	igdeNativeFoxListBoxFilterItem(const FXString &text, FXIcon *ic = nullptr, FXptr ptr = nullptr) :
	FXListItem(text, ic, ptr){}
	
	inline FXint getX() const{ return x; }
	inline FXint getY() const{ return y; }
};

FXIMPLEMENT(igdeNativeFoxListBoxFilterItem, FXListItem, nullptr, 0)



// class igdeNativeFoxListBoxFilter
/////////////////////////////////////

igdeNativeFoxListBoxFilter::igdeNativeFoxListBoxFilter(){}

igdeNativeFoxListBoxFilter::igdeNativeFoxListBoxFilter(igdeListBoxFilter &powner,
	FXComposite *pparent, const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags,
	const igdeGuiTheme &guitheme) :
FXVerticalFrame(pparent, layoutFlags.flags | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
pOwner(&powner),
pFont(ListBoxFont(powner, guitheme)),
pListBox(new FXList(this, this, ID_LISTBOX, LAYOUT_FILL | ListBoxFlags(powner))),
pLabelFilter(nullptr),
pEditFilter(nullptr),
pResizer(nullptr)
{
	#ifndef OS_W32_VS
	(void)ListBoxPadLeft;
	(void)ListBoxPadRight;
	(void)ListBoxPadTop;
	(void)ListBoxPadBottom;
	#endif
	
	if(!pOwner->GetVisible()){
		hide();
	}
	pListBox->setFont((FXFont*)pFont->GetNativeFont());
	
	pListBox->setNumVisible(powner.GetRows());
	if(!powner.GetEnabled()){
		pListBox->disable();
	}
	
	const FXString description(igdeUIFoxHelper::TranslateIf(powner, powner.GetDescription()));
	//setTipText(description); // not supported
	pListBox->setHelpText(description);
	
	BuildList();
	pListBox->setCurrentItem(powner.GetSelection());
	
	// filter edit field at the bottom
	FXHorizontalFrame * const lineFilter = new FXHorizontalFrame(
		this, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
	pLabelFilter = new FXLabel(lineFilter,
		pOwner->Translate("Igde.FoxComboBoxFilter.Filter").ToUTF8().GetString(),
		nullptr, LABEL_NORMAL, 0, 0, 0, 0, 0, 2);
	pLabelFilter->setJustify(JUSTIFY_LEFT | JUSTIFY_CENTER_Y);
	pEditFilter = new FXTextField(lineFilter, 5, this, ID_EDIT_FILTER, FRAME_SUNKEN | LAYOUT_FILL_X);
	
	if(layoutFlags.canResizeVertical || (layoutFlags.flags & LAYOUT_FILL_Y) == 0){
		pResizer = new igdeNativeFoxResizer(this, this, ID_RESIZER);
	}
}

igdeNativeFoxListBoxFilter::~igdeNativeFoxListBoxFilter(){
}

igdeNativeFoxListBoxFilter *igdeNativeFoxListBoxFilter::CreateNativeWidget(igdeListBoxFilter &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxListBoxFilter(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxListBoxFilter::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxListBoxFilter::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxListBoxFilter::BuildList(){
	pListBox->clearItems();
	
	const bool autoTranslate = pOwner->GetAutoTranslateItems();
	pOwner->GetItems().VisitIndexed([&](int i, const igdeListItem &item){
		pListBox->appendItem(new igdeNativeFoxListBoxFilterItem(
			autoTranslate
				? igdeUIFoxHelper::TranslateIf(*pOwner, item.GetText())
				: item.GetText().GetString(),
			item.GetIcon() ? (FXIcon*)item.GetIcon()->GetNativeIcon() : nullptr));
	});
	
	UpdateSelection();
}

void igdeNativeFoxListBoxFilter::UpdateItem(int index){
	const igdeListItem &item = *pOwner->GetItems().GetAt(index);
	pListBox->setItemText(index, pOwner->GetAutoTranslateItems()
		? igdeUIFoxHelper::TranslateIf(*pOwner, item.GetText())
		: item.GetText().GetString());
	
	if(item.GetIcon()){
		pListBox->setItemIcon(index, (FXIcon*)item.GetIcon()->GetNativeIcon());
		
	}else{
		pListBox->setItemIcon(index, nullptr);
	}
	
	if(item.GetSelected()){
		pListBox->selectItem(index);
		
	}else{
		pListBox->deselectItem(index);
	}
}

void igdeNativeFoxListBoxFilter::UpdateStyles(){
	pListBox->setListStyle(ListBoxFlags(*pOwner) & MASK_FLAGS_LIST);
}

void igdeNativeFoxListBoxFilter::UpdateSelection(){
	pListBox->setCurrentItem(pOwner->GetSelection());
	if(pListBox->getCurrentItem() != -1){
		MakeItemVisible(pListBox->getCurrentItem());
	}
	
	if(pOwner->GetSelectionMode() == igdeListBox::esmMultiple){
		const int count = pListBox->getNumItems();
		int i;
		
		for(i=0; i<count; i++){
			const bool selected = pOwner->GetItems().GetAt(i)->GetSelected();
			FXListItem &item = *pListBox->getItem(i);
			
			if(selected == item.isSelected()){
				continue;
			}
			
			if(selected){
				pListBox->selectItem(i);
				
			}else{
				pListBox->deselectItem(i);
			}
		}
	}
}

void igdeNativeFoxListBoxFilter::Focus(){
	pListBox->setFocus();
}

decPoint igdeNativeFoxListBoxFilter::GetContentPosition() const{
	return decPoint(-pListBox->getContentX(), -pListBox->getContentY());
}

void igdeNativeFoxListBoxFilter::SetContentPosition(const decPoint &position){
	pListBox->layout();
	pListBox->setPosition(-position.x, -position.y);
}

void igdeNativeFoxListBoxFilter::MakeItemVisible(int index){
	if(!pListBox->isItemVisible(index)){
		pListBox->makeItemVisible(index);
	}
}

void igdeNativeFoxListBoxFilter::InsertItem(int index){
	const igdeListItem &item = *pOwner->GetItems().GetAt(index);
	pListBox->insertItem(index, new igdeNativeFoxListBoxFilterItem(
		pOwner->GetAutoTranslateItems()
			? igdeUIFoxHelper::TranslateIf(*pOwner, item.GetText())
			: item.GetText().GetString(),
		item.GetIcon() ? (FXIcon*)item.GetIcon()->GetNativeIcon() : nullptr));
}

void igdeNativeFoxListBoxFilter::RemoveItem(int index){
	pListBox->removeItem(index);
}

void igdeNativeFoxListBoxFilter::RemoveAllItems(){
	pListBox->clearItems();
}

void igdeNativeFoxListBoxFilter::MoveItem(int fromIndex, int toIndex){
	pListBox->moveItem(toIndex, fromIndex);
}

void igdeNativeFoxListBoxFilter::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		pListBox->enable();
		
	}else{
		pListBox->disable();
	}
}

void igdeNativeFoxListBoxFilter::UpdateRowCount(){
	pListBox->setNumVisible(pOwner->GetRows());
}

void igdeNativeFoxListBoxFilter::UpdateDescription(){
	const FXString description(igdeUIFoxHelper::TranslateIf(*pOwner, pOwner->GetDescription()));
	//pListBox->setTipText(description); // not supported
	pListBox->setHelpText(description);
}

void igdeNativeFoxListBoxFilter::UpdateFilterString(){
	pEditFilter->setText(pOwner->GetFilterString().GetString());
}



int igdeNativeFoxListBoxFilter::ListBoxFlags(const igdeListBoxFilter &powner){
	int fflags = FRAME_SUNKEN;
	
	switch(powner.GetSelectionMode()){
	case igdeListBox::esmSingle:
		fflags |= LIST_BROWSESELECT;
		break;
		
	case igdeListBox::esmMultiple:
		fflags |= LIST_EXTENDEDSELECT;
		break;
		
	default:
		break;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxListBoxFilter::ListBoxFont(const igdeListBoxFilter &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::listBoxFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::listBoxFontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::listBoxFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::listBoxFontSize, 1.0f);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.GetProperties().Has(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxListBoxFilter::ListBoxPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxListBoxFilter::ListBoxPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxListBoxFilter::ListBoxPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxListBoxFilter::ListBoxPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxListBoxFilter::onListCommand(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled() || pOwner->GetSelection() == pListBox->getCurrentItem()){
		return 1;
	}
	
	try{
		pOwner->SetSelection(pListBox->getCurrentItem());
		pOwner->NotifySelectionChanged();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBoxFilter::onListChanged(FXObject *sender, FXSelector selector, void *pdata){
	return onListCommand(sender, selector, pdata);
}

long igdeNativeFoxListBoxFilter::onListSelected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItems().GetCount()){
		return 1;
	}
	
	try{
		pOwner->GetItems().GetAt(index)->SetSelected(true);
		pOwner->NotifyItemSelected(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBoxFilter::onListDeselected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItems().GetCount()){
		return 1;
	}
	
	try{
		pOwner->GetItems().GetAt(index)->SetSelected(false);
		pOwner->NotifyItemDeselected(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBoxFilter::onListRightMouseDown(FXObject*, FXSelector, void *pdata){
	if(!pOwner->GetEnabled()){
		return 1;
	}
	
	const FXEvent &event = *((FXEvent*)pdata);
	
	const int index = pListBox->getItemAt(event.win_x, event.win_y);
	if(index != -1){
		pListBox->selectItem(index, true);
		pListBox->setCurrentItem(index, true);
	}
	
	pOwner->ShowContextMenu(decPoint(event.win_x, event.win_y));
	return 1;
}

long igdeNativeFoxListBoxFilter::onListRightMouseUp(FXObject*, FXSelector, void*){
	return 1;
}

long igdeNativeFoxListBoxFilter::onListDoubleClicked(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItems().GetCount()){
		return 1;
	}
	
	try{
		pOwner->NotifyDoubleClickItem(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBoxFilter::onEditFilterChanged(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled()){
		return 0;
	}
	
	try{
		pOwner->SetFilterString(pEditFilter->getText().text());
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(*pOwner, e);
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxListBoxFilter::onResizerDrag(FXObject*, FXSelector, void *pdata){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance(pdata);
	const int newHeight = getHeight() + distance;
	
	const int LINE_SPACING = 4;  // hard coded in FXList
	const int itemHeight = LINE_SPACING + pListBox->getFont()->getFontHeight();
	
	pOwner->SetRows(decMath::max((newHeight + itemHeight / 2) / itemHeight, 1));
	
	return 0;
}

#endif
