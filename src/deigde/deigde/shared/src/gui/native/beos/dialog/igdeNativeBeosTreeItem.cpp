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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosTreeItem.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosTreeItem
/////////////////////////////////

igdeNativeBeosTreeItem::igdeNativeBeosTreeItem(const char *text, int32 level) :
pText(text ? text : ""),
pLevel(level),
pExpanded(false),
pParent(nullptr){
}

igdeNativeBeosTreeItem::~igdeNativeBeosTreeItem() = default;


// Management
//////////////

void igdeNativeBeosTreeItem::SetText(const char *text){
	if(text){
		pText = text;
	} else {
		pText.Empty();
	}
}

void igdeNativeBeosTreeItem::SetLevel(int32 level){
	DEASSERT_TRUE(level >= 0)
	pLevel = level;
}

void igdeNativeBeosTreeItem::SetExpanded(bool expanded){
	pExpanded = expanded;
}

void igdeNativeBeosTreeItem::SetParent(igdeNativeBeosTreeItem *parent){
	pParent = parent;
}

void igdeNativeBeosTreeItem::DrawItem(BView *owner, BRect frame, bool complete){
	if(!owner){
		return;
	}
	
	const rgb_color backgroundColor{255, 255, 255, 255};
	const rgb_color textColor{0, 0, 0, 255};
	
	if(IsSelected() || complete){
		owner->SetHighColor(IsSelected() ? ui_color(B_HIGHLIGHT_BACKGROUND_COLOR) : backgroundColor);
		owner->FillRect(frame);
	}
	
	// Draw indentation for tree level
	const int32 indent = pLevel * 16;
	frame.left += indent + 4;
	
	// Draw text
	owner->SetHighColor(textColor);
	owner->DrawString(pText.GetString(), BPoint(frame.left, frame.top + 12));
}

void igdeNativeBeosTreeItem::GetHeight(BView *owner, int32 *height) const {
	if(height){
		*height = 16;
	}
}

#endif
