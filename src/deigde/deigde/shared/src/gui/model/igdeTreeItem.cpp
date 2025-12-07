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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeTreeItem.h"

#include <dragengine/common/exceptions.h>



// Class igdeTreeItem
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItem::igdeTreeItem(const char *text) :
pText(text),
pParent(NULL),
pPrevious(NULL),
pData(NULL),
pSelected(false),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const char *text, igdeIcon *icon) :
pText(text),
pIcon(icon),
pParent(NULL),
pPrevious(NULL),
pData(NULL),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const char *text, igdeIcon *icon, const char *description) :
pText(text),
pDescription(description),
pIcon(icon),
pParent(NULL),
pPrevious(NULL),
pData(NULL),
pSelected(false),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const char *text, void *data) :
pText(text),
pParent(NULL),
pPrevious(NULL),
pData(data),
pSelected(false),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const char *text, igdeIcon *icon, void *data) :
pText(text),
pIcon(icon),
pParent(NULL),
pPrevious(NULL),
pData(data),
pSelected(false),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const char *text, igdeIcon *icon, const char *description, void *data) :
pText(text),
pDescription(description),
pIcon(icon),
pParent(NULL),
pPrevious(NULL),
pData(data),
pSelected(false),
pExpanded(false),
pNativeWidget(NULL){
}

igdeTreeItem::igdeTreeItem(const igdeTreeItem &treeItem) :
pText(treeItem.pText),
pDescription(treeItem.pDescription),
pIcon(treeItem.pIcon),
pParent(NULL),
pPrevious(NULL),
pData(treeItem.pData),
pSelected(false),
pExpanded(treeItem.pExpanded),
pNativeWidget(NULL){
}

igdeTreeItem::~igdeTreeItem(){
}



// Management
///////////////

void igdeTreeItem::SetText(const char *text){
	pText = text;
}

void igdeTreeItem::SetDescription(const char *description){
	pDescription = description;
}

void igdeTreeItem::SetIcon(igdeIcon *icon){
	pIcon = icon;
}

void igdeTreeItem::SetData(void *data){
	pData = data;
}

void igdeTreeItem::SetSelected(bool selected){
	pSelected = selected;
}

void igdeTreeItem::SetExpanded(bool expanded){
	pExpanded = expanded;
}

void igdeTreeItem::SetParent(igdeTreeItem *item){
	pParent = item;
}

void igdeTreeItem::SetPrevious(igdeTreeItem *item){
	pPrevious = item;
}

void igdeTreeItem::SetNext(igdeTreeItem *item){
	pNext = item;
}

void igdeTreeItem::SetFirstChild(igdeTreeItem *item){
	pFirstChild = item;
}

igdeTreeItem *igdeTreeItem::GetLastChild() const{
	igdeTreeItem *item = pFirstChild;
	igdeTreeItem *lastChild = NULL;
	while(item){
		lastChild = item;
		item = item->pNext;
	}
	return lastChild;
}

int igdeTreeItem::GetChildrenCount() const{
	int count = 0;
	igdeTreeItem *item = pFirstChild;
	while(item){
		count++;
		item = item->pNext;
	}
	return count;
}



igdeTreeItem &igdeTreeItem::operator=(const igdeTreeItem &treeItem){
	pText = treeItem.pText;
	pDescription = treeItem.pDescription;
	pIcon = treeItem.pIcon;
	pData = treeItem.pData;
	pExpanded = treeItem.pExpanded;
	return *this;
}



void igdeTreeItem::SetNativeWidget(void *nativeWidget){
	pNativeWidget = nativeWidget;
}
