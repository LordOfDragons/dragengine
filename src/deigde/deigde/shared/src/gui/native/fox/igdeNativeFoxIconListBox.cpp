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

#include "igdeNativeFoxIconListBox.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeIconListBox.h"
#include "../../igdeCommonDialogs.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../model/igdeListHeader.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Icon item
//////////////

class igdeNativeFoxIconListBoxItem : public FXIconItem{
	FXDECLARE(igdeNativeFoxIconListBoxItem)
	
protected:
	igdeNativeFoxIconListBoxItem();
	
private:
	igdeIconListBox *pList;
	igdeListItem *pListItem;
	
public:
	igdeNativeFoxIconListBoxItem(igdeIconListBox &list, igdeListItem &listItem);
	virtual ~igdeNativeFoxIconListBoxItem();
	
	inline igdeIconListBox &GetList() const{ return *pList; }
	inline igdeListItem &GetItem() const{ return *pListItem; }
	
	virtual FXString getTipText() const;
	void UpdateFromItem();
};

FXIMPLEMENT(igdeNativeFoxIconListBoxItem, FXIconItem, nullptr, 0)

igdeNativeFoxIconListBoxItem::igdeNativeFoxIconListBoxItem(){}

igdeNativeFoxIconListBoxItem::igdeNativeFoxIconListBoxItem(igdeIconListBox &list, igdeListItem &listItem) :
FXIconItem(""),
pList(&list),
pListItem(&listItem)
{
	UpdateFromItem();
}

igdeNativeFoxIconListBoxItem::~igdeNativeFoxIconListBoxItem(){
}

FXString igdeNativeFoxIconListBoxItem::getTipText() const{
	if(!pListItem->GetDescription().IsEmpty()){
		return pListItem->GetDescription().GetString();
		
	}else{
		return pList->GetDescription().GetString();
	}
}

void igdeNativeFoxIconListBoxItem::UpdateFromItem(){
	decStringList text;
	text.Add(pListItem->GetText());
	text += pListItem->GetDetails();
	
	FXIcon * const iicon = pListItem->GetIcon() ? (FXIcon*)pListItem->GetIcon()->GetNativeIcon() : NULL;
	
	setText(text.Join("\t").GetString());
	setBigIcon(iicon);
	setMiniIcon(iicon);
	setData(pListItem->GetData());
}



// Native widget
//////////////////

FXDEFMAP(igdeNativeFoxIconListBox) cNativeIgdeListBoxMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListCommand),
	
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListChanged),
	
	FXMAPFUNC(SEL_RIGHTBUTTONPRESS, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListRightMouseDown),
	
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListRightMouseUp),
		
	FXMAPFUNC(SEL_SELECTED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListSelected),
		
	FXMAPFUNC(SEL_DESELECTED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListDeselected),
	
	FXMAPFUNC(SEL_DOUBLECLICKED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListDoubleClicked),
		
	FXMAPFUNC(SEL_CHANGED, igdeNativeFoxIconListBox::ID_HEADER,
		igdeNativeFoxIconListBox::onListHeaderChanged),
		
	FXMAPFUNC(SEL_CLICKED, igdeNativeFoxIconListBox::ID_HEADER,
		igdeNativeFoxIconListBox::onListHeaderClicked),
	
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxIconListBox::ID_RESIZER,
		igdeNativeFoxIconListBox::onResizerDrag),
};


FXIMPLEMENT(igdeNativeFoxIconListBox, FXVerticalFrame,
	cNativeIgdeListBoxMap, ARRAYNUMBER(cNativeIgdeListBoxMap))


// class igdeNativeFoxIconListBox
///////////////////////////////////

igdeNativeFoxIconListBox::igdeNativeFoxIconListBox(){}

igdeNativeFoxIconListBox::igdeNativeFoxIconListBox(igdeIconListBox &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme) :
FXVerticalFrame(pparent, layoutFlags.flags | IconListBoxFlagsBorder(powner), 0, 0, 0, 0, 0, 0, 0, 0),
pOwner(&powner),
pFont(IconListBoxFont(powner, guitheme)),
pListBox(new FXIconList(this, this, ID_LISTBOX, IconListBoxFlags(powner))),
pResizer(NULL)
{
	#ifndef OS_W32_VS
	(void)IconListBoxPadLeft;
	(void)IconListBoxPadRight;
	(void)IconListBoxPadTop;
	(void)IconListBoxPadBottom;
	#endif

	if(!pOwner->GetVisible()){
		hide();
	}
	pListBox->setFont((FXFont*)pFont->GetNativeFont());
	
	if(!powner.GetEnabled()){
		pListBox->disable();
	}
	
	//setTipText( owner.GetDescription().GetString() ); // not supported
	pListBox->setHelpText(powner.GetDescription().GetString());
	
	UpdateMinimumSize();
	
	BuildHeader();
	BuildList();
	pListBox->setCurrentItem(powner.GetSelection());
	
	if(layoutFlags.canResizeVertical || (layoutFlags.flags & LAYOUT_FILL_Y) == 0){
		pResizer = new igdeNativeFoxResizer(this, this, ID_RESIZER);
	}
}

igdeNativeFoxIconListBox::~igdeNativeFoxIconListBox(){
}

igdeNativeFoxIconListBox *igdeNativeFoxIconListBox::CreateNativeWidget(igdeIconListBox &powner){
	if(!powner.GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const pparent = (FXComposite*) powner.GetParent()->GetNativeContainer();
	if(!pparent){
		DETHROW(deeInvalidParam);
	}
	
	return new igdeNativeFoxIconListBox(powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll(&powner), *powner.GetGuiTheme());
}

void igdeNativeFoxIconListBox::PostCreateNativeWidget(){
	FXComposite &pparent = *((FXComposite*)pOwner->GetParent()->GetNativeContainer());
	if(pparent.id()){
		create();
	}
}

void igdeNativeFoxIconListBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxIconListBox::BuildHeader(){
	FXHeader &header = *pListBox->getHeader();
	header.setFont(pListBox->getFont());
	header.setPadLeft(3);
	header.setPadTop(0);
	header.setPadRight(3);
	header.setPadBottom(0);
	header.setTarget(this);
	header.setSelector(ID_HEADER);
	
	UpdateHeader();
}

void igdeNativeFoxIconListBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	pListBox->clearItems();
	
	for(i=0; i<count; i++){
		pListBox->appendItem(new igdeNativeFoxIconListBoxItem(*pOwner, *pOwner->GetItemAt(i)));
	}
	
	UpdateSelection();
}

void igdeNativeFoxIconListBox::InsertItem(int index, igdeListItem &item){
	igdeNativeFoxIconListBoxItem * const foxItem = new igdeNativeFoxIconListBoxItem(*pOwner, item);
	
	if(index == pListBox->getNumItems()){
		pListBox->appendItem(foxItem);
		
	}else{
		pListBox->insertItem(index, foxItem);
	}
}

void igdeNativeFoxIconListBox::UpdateItem(int index){
	((igdeNativeFoxIconListBoxItem*)pListBox->getItem(index))->UpdateFromItem();
	pListBox->updateItem(index);
}

void igdeNativeFoxIconListBox::MoveItem(int fromIndex, int toIndex){
	pListBox->moveItem(toIndex, fromIndex);
}

void igdeNativeFoxIconListBox::RemoveItem(int index){
	pListBox->removeItem(index);
}

void igdeNativeFoxIconListBox::UpdateSelection(){
	pListBox->setCurrentItem(pOwner->GetSelection());
	if(pListBox->getCurrentItem() != -1){
		pListBox->makeItemVisible(pListBox->getCurrentItem());
	}
	
	if(pOwner->GetSelectionMode() == igdeIconListBox::esmMultiple){
		const int count = pListBox->getNumItems();
		int i;
		
		for(i=0; i<count; i++){
			igdeNativeFoxIconListBoxItem &item = *((igdeNativeFoxIconListBoxItem*)pListBox->getItem(i));
			const bool selected = item.GetItem().GetSelected();
			if(selected == item.isSelected()){
				continue;
			}
			
			if(selected){
				pListBox->selectItem(i);
				
			}else{
				pListBox->deselectItem(i);
			}
		}
		
	}else if(pListBox->getCurrentItem() != -1){
		pListBox->selectItem(pListBox->getCurrentItem());
	}
}

void igdeNativeFoxIconListBox::UpdateHeader(){
	FXHeader &nativeHeader = *pListBox->getHeader();
	const int count = pOwner->GetHeaderCount();
	int i;
	
	nativeHeader.clearItems();
	
	for(i=0; i<count; i++){
		const igdeListHeader &header = *pOwner->GetHeaderAt(i);
		
		nativeHeader.appendItem(header.GetTitle().GetString(),
			header.GetIcon() ? (FXIcon*)header.GetIcon()->GetNativeIcon() : NULL,
			header.GetSize());
	}
}

void igdeNativeFoxIconListBox::UpdateStyles(){
	pListBox->setListStyle(IconListBoxFlags(*pOwner));
}

void igdeNativeFoxIconListBox::UpdateDescription(){
	//pListBox->setTipText( pOwner->GetDescription().GetString() ); // not supported
	pListBox->setHelpText(pOwner->GetDescription().GetString());
}

void igdeNativeFoxIconListBox::UpdateEnabled(){
	if(pOwner->GetEnabled()){
		pListBox->enable();
		
	}else{
		pListBox->disable();
	}
}

void igdeNativeFoxIconListBox::UpdateMinimumSize(){
	pDefaultSize = pOwner->GetMiniumSize();
	recalc();
}

void igdeNativeFoxIconListBox::Focus(){
	pListBox->setFocus();
}

void igdeNativeFoxIconListBox::MakeItemVisible(int index){
	pListBox->makeItemVisible(index);
}

void igdeNativeFoxIconListBox::RemoveAllItems(){
	pListBox->clearItems();
}



int igdeNativeFoxIconListBox::IconListBoxFlagsBorder(const igdeIconListBox &){
	return FRAME_SUNKEN;
}

int igdeNativeFoxIconListBox::IconListBoxFlags(const igdeIconListBox &powner){
	int fflags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	
	switch(powner.GetViewMode()){
	case igdeIconListBox::evmList:
		fflags |= ICONLIST_DETAILED | ICONLIST_ROWS;
		break;
		
	case igdeIconListBox::evmIconHorizontal:
		fflags |= ICONLIST_BIG_ICONS | ICONLIST_ROWS;
		break;
		
	case igdeIconListBox::evmIconVertical:
		fflags |= ICONLIST_BIG_ICONS | ICONLIST_COLUMNS;
		break;
		
	default:
		fflags |= ICONLIST_DETAILED | ICONLIST_ROWS;
	}
	
	switch(powner.GetSelectionMode()){
	case igdeIconListBox::esmSingle:
		fflags |= FOLDINGLIST_BROWSESELECT /*ICONLIST_SINGLESELECT*/;
		break;
		
	case igdeIconListBox::esmMultiple:
		fflags |= ICONLIST_EXTENDEDSELECT;//ICONLIST_MULTIPLESELECT;
		break;
		
	default:
		fflags |= FOLDINGLIST_BROWSESELECT;
		break;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxIconListBox::IconListBoxFont(const igdeIconListBox &powner, const igdeGuiTheme &guitheme){
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

int igdeNativeFoxIconListBox::IconListBoxPadLeft(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingLeft, DEFAULT_PAD);
}

int igdeNativeFoxIconListBox::IconListBoxPadRight(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingRight, DEFAULT_PAD);
}

int igdeNativeFoxIconListBox::IconListBoxPadTop(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingTop, DEFAULT_PAD);
}

int igdeNativeFoxIconListBox::IconListBoxPadBottom(const igdeGuiTheme &guitheme){
	return guitheme.GetIntProperty(igdeGuiThemePropertyNames::listBoxPaddingBottom, DEFAULT_PAD);
}



FXint igdeNativeFoxIconListBox::getDefaultWidth(){
	return pDefaultSize.x;
}

FXint igdeNativeFoxIconListBox::getDefaultHeight(){
	return pDefaultSize.y;
}



// Events
///////////

long igdeNativeFoxIconListBox::onListCommand(FXObject*, FXSelector, void*){
	// send if user clicks on an item. also sends sends SEL_CLICKED, SEL_DOUBLECLICKED
	// and SEL_TRIPLECLICKED.
	return 1;
}

long igdeNativeFoxIconListBox::onListChanged(FXObject*, FXSelector, void*){
	if(!pOwner->GetEnabled()){
		return 1;
	}
	
	try{
		pOwner->SetSelection(pListBox->getCurrentItem());
		
	}catch(const deException &e){
		pOwner->GetLogger()->LogException("IGDE", e);
		igdeCommonDialogs::Exception(pOwner, e);
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListSelected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index >= 0 and index < pOwner->GetItemCount()){
		pOwner->GetItemAt(index)->SetSelected(true);
		pOwner->NotifyItemSelected(index);
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListDeselected(FXObject*, FXSelector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index >= 0 and index < pOwner->GetItemCount()){
		pOwner->GetItemAt(index)->SetSelected(false);
		pOwner->NotifyItemDeselected(index);
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListHeaderChanged(FXObject *sender, FXSelector selector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index >= 0 and index < pOwner->GetHeaderCount()){
		pOwner->GetHeaderAt(index)->SetSize(pListBox->getHeader()->getItemSize(index));
	}
	return pListBox->onChgHeader(sender, selector, pdata);
}

long igdeNativeFoxIconListBox::onListHeaderClicked(FXObject *sender, FXSelector selector, void *pdata){
	const int index = (int)(intptr_t)pdata;
	if(index >= 0 and index < pOwner->GetHeaderCount()){
		pOwner->NotifyHeaderClicked(index);
	}
	return pListBox->onClkHeader(sender, selector, pdata);
}

long igdeNativeFoxIconListBox::onListRightMouseDown(FXObject*, FXSelector, void *pdata){
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

long igdeNativeFoxIconListBox::onListRightMouseUp(FXObject*, FXSelector, void*){
	return 1;
}

long igdeNativeFoxIconListBox::onListDoubleClicked(FXObject*, FXSelector, void *pdata){
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



long igdeNativeFoxIconListBox::onResizerDrag(FXObject*, FXSelector, void *pdata){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance(pdata);
	pDefaultSize.y = decMath::max(50, pDefaultSize.y + distance);
	recalc();
	return 0;
}

#endif
