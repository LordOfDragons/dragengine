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

#include "igdeListItem.h"

#include <dragengine/common/exceptions.h>



// Class igdeListItem
///////////////////////

// Constructor, destructor
////////////////////////////

igdeListItem::igdeListItem(const char *text) :
pText(text),
pData(NULL),
pSelected(false){
}

igdeListItem::igdeListItem(const char *text, igdeIcon *icon) :
pText(text),
pIcon(icon),
pData(NULL){
}

igdeListItem::igdeListItem(const char *text, igdeIcon *icon, const char *description) :
pText(text),
pDescription(description),
pIcon(icon),
pData(NULL),
pSelected(false){
}

igdeListItem::igdeListItem(const char *text, void *data) :
pText(text),
pData(data),
pSelected(false){
}

igdeListItem::igdeListItem(const char *text, igdeIcon *icon, void *data) :
pText(text),
pIcon(icon),
pData(data),
pSelected(false){
}

igdeListItem::igdeListItem(const char *text, igdeIcon *icon, const char *description, void *data) :
pText(text),
pDescription(description),
pIcon(icon),
pData(data),
pSelected(false){
}

igdeListItem::igdeListItem(const igdeListItem &listItem) :
pText(listItem.pText),
pDescription(listItem.pDescription),
pIcon(listItem.pIcon),
pData(listItem.pData),
pDetails(listItem.pDetails),
pSelected(false){
}

igdeListItem::~igdeListItem(){
}



// Management
///////////////

void igdeListItem::SetText(const char *text){
	pText = text;
}

void igdeListItem::SetDescription(const char *description){
	pDescription = description;
}

void igdeListItem::SetIcon(igdeIcon *icon){
	pIcon = icon;
}

void igdeListItem::SetData(void *data){
	pData = data;
}

void igdeListItem::SetSelected(bool selected){
	pSelected = selected;
}



igdeListItem &igdeListItem::operator=(const igdeListItem &listItem){
	pText = listItem.pText;
	pDescription = listItem.pDescription;
	pIcon = listItem.pIcon;
	pData = listItem.pData;
	pDetails = listItem.pDetails;
	return *this;
}
