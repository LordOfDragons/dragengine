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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxListBox.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeListBox.h"
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



FXDEFMAP(igdeNativeFoxListBox) cNativeIgdeListBoxMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListCommand),
	
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListChanged),
	
	FXMAPFUNC(SEL_SELECTED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListSelected),
	
	FXMAPFUNC(SEL_DESELECTED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListDeselected),
	
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListRightMouseDown),
	
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListRightMouseUp),
	
	FXMAPFUNC(SEL_DOUBLECLICKED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListDoubleClicked),
	
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxListBox::ID_RESIZER,
		igdeNativeFoxListBox::onResizerDrag)
};


FXIMPLEMENT(igdeNativeFoxListBox, FXVerticalFrame,
	cNativeIgdeListBoxMap, ARRAYNUMBER(cNativeIgdeListBoxMap))

#define MASK_FLAGS_LIST (LIST_SINGLESELECT | LIST_BROWSESELECT | LIST_MULTIPLESELECT | LIST_AUTOSELECT)


// class igdeNativeFoxListBox
///////////////////////////////

igdeNativeFoxListBox::igdeNativeFoxListBox(){}

igdeNativeFoxListBox::igdeNativeFoxListBox(igdeListBox &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme) :
FXVerticalFrame(pparent, layoutFlags.flags | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
pOwner(&powner),
pFont(ListBoxFont(powner, guitheme)),
pListBox(new FXList(this, this, ID_LISTBOX, LAYOUT_FILL | ListBoxFlags(powner))),
pResizer(NULL)
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
	
	//setTipText( owner.GetDescription().GetString() ); // nto supported
	pListBox->setHelpText(powner.GetDescription().GetString());
	
	BuildList();
	pListBox->setCurrentItem(powner.GetSelection());
	
	if(layoutFlags.canResizeVertical || (layoutFlags.flags & LAYOUT_FILL_Y) == 0){
		pResizer = new igdeNativeFoxResizer(this, this, ID_RESIZER);
	}
}

igdeNativeFoxListBox::~igdeNativeFoxListBox(){
}

igdeNativeFoxListBox *igdeNativeFoxListBox::CreateNativeWidget(igdeListBox &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxListBox(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxListBox::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxListBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxListBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	pListBox->clearItems();
	
	for(i=0; i<count; i++){
		const igdeListItem &item = *pOwner->GetItemAt(i);
		pListBox->appendItem(item.GetText().GetString(),
			item.GetIcon() ? (FXIcon*)item.GetIcon()->GetNativeIcon() : NULL);
	}
	
	UpdateSelection();
}

void igdeNativeFoxListBox::UpdateItem(int index){
	const igdeListItem &item = *pOwner->GetItemAt(index);
	pListBox->setItemText(index, item.GetText().GetString());
	
	if(item.GetIcon()){
		pListBox->setItemIcon(index, (FXIcon*)item.GetIcon()->GetNativeIcon());
		
	}else{
		pListBox->setItemIcon(index, NULL);
	}
	
	if(item.GetSelected()){
		pListBox->selectItem(index);
		
	}else{
		pListBox->deselectItem(index);
	}
}

void igdeNativeFoxListBox::UpdateStyles(){
	pListBox->setListStyle(ListBoxFlags(*pOwner) & MASK_FLAGS_LIST);
}

void igdeNativeFoxListBox::UpdateSelection(){
	pListBox->setCurrentItem(pOwner->GetSelection());
	if(pListBox->getCurrentItem() != -1){
		pListBox->makeItemVisible(pListBox->getCurrentItem());
	}
	
	if(pOwner->GetSelectionMode() == igdeListBox::esmMultiple){
		const int count = pListBox->getNumItems();
		int i;
		
		for(i=0; i<count; i++){
			const bool selected = pOwner->GetItemAt(i)->GetSelected();
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

void igdeNativeFoxListBox::Focus(){
	pListBox->setFocus();
}

void igdeNativeFoxListBox::MakeItemVisible(int index){
	pListBox->makeItemVisible(index);
}

void igdeNativeFoxListBox::InsertItem(int index){
	const igdeListItem &item = *pOwner->GetItemAt(index);
	pListBox->insertItem(index, item.GetText().GetString(),
		item.GetIcon() ? (FXIcon*)item.GetIcon()->GetNativeIcon() : NULL);
}

void igdeNativeFoxListBox::RemoveItem(int index){
	pListBox->removeItem(index);
}

void igdeNativeFoxListBox::RemoveAllItems(){
	pListBox->clearItems();
}

void igdeNativeFoxListBox::MoveItem(int fromIndex, int toIndex){
	pListBox->moveItem(toIndex, fromIndex);
}

void igdeNativeFoxListBox::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		pListBox->enable();
		
	}else{
		pListBox->disable();
	}
}

void igdeNativeFoxListBox::UpdateRowCount(){
	pListBox->setNumVisible(pOwner->GetRows());
}

void igdeNativeFoxListBox::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	//pListBox->setTipText( description ); // not supported
	pListBox->setHelpText(description);
}



int igdeNativeFoxListBox::ListBoxFlags(const igdeListBox &powner){
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

igdeFont *igdeNativeFoxListBox::ListBoxFont(const igdeListBox &powner, const igdeGuiTheme &guitheme){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont(configuration);
	
	if(guitheme.HasProperty(igdeGuiThemePropertyNames::listBoxFontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::listBoxFontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::listBoxFontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::listBoxFontSize, 1.0f);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSizeAbsolute)){
		configuration.size = (float)guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0);
		
	}else if(guitheme.HasProperty(igdeGuiThemePropertyNames::fontSize)){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f);
	}
	
	return powner.GetEnvironment().GetSharedFont(configuration);
}

int igdeNativeFoxListBox::ListBoxPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxListBox::ListBoxPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxListBox::ListBoxPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxListBox::ListBoxPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingBottom, DEFAULT_PAD);
}



// Events
///////////

long igdeNativeFoxListBox::onListCommand(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled() || pOwner->GetSelection() == pListBox->getCurrentItem()){
		return 1;
	}
	
	try{
		pOwner->SetSelection(pListBox->getCurrentItem());
		pOwner->NotifySelectionChanged();
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBox::onListChanged(FXObject *sender, FXSelector selector, void *pdata){
	return onListCommand(sender, selector, pdata);
}

long igdeNativeFoxListBox::onListSelected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItemCount()){
		return 1;
	}
	
	try{
		pOwner->GetItemAt(index)->SetSelected(true);
		pOwner->NotifyItemSelected(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBox::onListDeselected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItemCount()){
		return 1;
	}
	
	try{
		pOwner->GetItemAt(index)->SetSelected(false);
		pOwner->NotifyItemDeselected(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	return 1;
}

long igdeNativeFoxListBox::onListRightMouseDown(FXObject*, FXSelector, void *pdata){
	if(!pOwner->GetEnabled()){
		return 1;
	}
	
	const FXEvent &event = *((FXEvent*)pdata);
	
	const int index = pListBox->getItemAt(event.win_x, event.win_y);
	if(index != -1){
		pListBox->selectItem(index, true);
		pListBox->setCurrentItem(index, true); // changes selected state only in browse mode
	}
	
	pOwner->ShowContextMenu(decPoint(event.win_x, event.win_y));
	return 1;
}

long igdeNativeFoxListBox::onListRightMouseUp(FXObject*, FXSelector, void*){
	return 1;
}

long igdeNativeFoxListBox::onListDoubleClicked(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index < 0 || index >= pOwner->GetItemCount()){
		return 1;
	}
	
	try{
		pOwner->NotifyDoubleClickItem(index);
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	return 1;
}



long igdeNativeFoxListBox::onResizerDrag(FXObject*, FXSelector, void *pdata){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance(pdata);
	const int newHeight = getHeight() + distance;
	
	const int LINE_SPACING = 4;  // hard coded in FXList
	const int itemHeight = LINE_SPACING + pListBox->getFont()->getFontHeight();
	
	pOwner->SetRows(decMath::max((newHeight + itemHeight / 2) / itemHeight, 1));
	
	return 0;
}

#endif
