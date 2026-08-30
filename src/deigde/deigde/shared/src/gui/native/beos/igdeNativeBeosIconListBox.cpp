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

#include "igdeNativeBeosIconListBox.h"
#include "../../igdeIconListBox.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosIconListBox
////////////////////////////////////

igdeNativeBeosIconListBox::igdeNativeBeosIconListBox() :
BView(BRect(0, 0, 300, 200), "iconlistbox", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS),
pOwner(nullptr),
pItems(),
pSelectedItem(-1),
pColumnCount(4),
pItemSize(64)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

igdeNativeBeosIconListBox::~igdeNativeBeosIconListBox(){
	pItems.RemoveAll();
}

igdeNativeBeosIconListBox* igdeNativeBeosIconListBox::CreateNativeWidget(igdeIconListBox &owner){
	// IconListBox creates a grid view for items with icons
	// Items are displayed in a multi-column grid layout
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosIconListBox();
	widget->pOwner = &owner;
	parent->AddChild(widget);
	
	return widget;
}

void igdeNativeBeosIconListBox::PostCreateNativeWidget(igdeIconListBox &owner, void *native){
	// IconListBox post-creation setup
	auto widget = (igdeNativeBeosIconListBox*)native;
	if(widget){
		widget->pOwner = &owner;
	}
}

void igdeNativeBeosIconListBox::DestroyNativeWidget(igdeIconListBox &owner, void *native){
	auto widget = (igdeNativeBeosIconListBox*)native;
	if(widget){
		widget->RemoveSelf();
		delete widget;
	}
}


// Management
///////////////

void igdeNativeBeosIconListBox::Draw(BRect updateRect){
	BView::Draw(updateRect);
	
	try{
		float x = 5, y = 5;
		int col = 0;
		
		// Draw each item in grid layout
		for(int i=0; i<pItems.CountItems(); i++){
			auto item = (ListItem*)pItems.ItemAt(i);
			if(item){
				DrawItem(updateRect, item, i, x, y);
				
				// Move to next grid position
				x += pItemSize + 10;
				col++;
				
				if(col >= pColumnCount){
					x = 5;
					y += pItemSize + 30;
					col = 0;
				}
			}
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE IconListBox", e);
		}
	}
}

void igdeNativeBeosIconListBox::DrawItem(BRect updateRect, ListItem *item, int index, float x, float y){
	// Draw a single item with icon and label
	BRect itemRect(x, y, x + pItemSize + 10, y + pItemSize + 25);
	
	if(updateRect.Intersects(itemRect)){
		// Draw selection background
		if(index == pSelectedItem){
			SetHighColor(ui_color(B_HIGHLIGHT_BACKGROUND_COLOR));
			FillRect(itemRect);
		}
		
		// Draw icon placeholder (64x64)
		BRect iconRect(x + 5, y + 5, x + pItemSize, y + pItemSize);
		SetHighColor(200, 200, 200);
		StrokeRect(iconRect);
		
		// Draw label
		BRect labelRect(x, y + pItemSize + 5, x + pItemSize + 10, y + pItemSize + 20);
		if(index == pSelectedItem){
			SetHighColor(ui_color(B_HIGHLIGHT_TEXT_COLOR));
			
		}else{
			SetHighColor(ui_color(B_PANEL_TEXT_COLOR));
		}
		DrawString(item->label.String(), BPoint(x + 5, y + pItemSize + 15));
	}
}

void igdeNativeBeosIconListBox::MouseDown(BPoint where){
	BView::MouseDown(where);
	
	try{
		// Calculate which item was clicked
		int col = (int)(where.x / (pItemSize + 10));
		int row = (int)(where.y / (pItemSize + 30));
		int index = row * pColumnCount + col;
		
		if(index >= 0 && index < pItems.CountItems()){
			if(pSelectedItem != index){
				pSelectedItem = index;
				Invalidate();
				
				// Fire selection changed callback
				if(pOwner && pOwner->GetAction()){
					pOwner->GetAction()->OnAction();
				}
			}
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE IconListBox", e);
		}
	}
}

void igdeNativeBeosIconListBox::FrameResized(float width, float height){
	BView::FrameResized(width, height);
	
	try{
		Invalidate();
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE IconListBox", e);
		}
	}
}

void igdeNativeBeosIconListBox::UpdateItemSelection(){
	// Synchronize selection from IGDE to native
	Invalidate();
}

void igdeNativeBeosIconListBox::UpdateItems(){
	// Rebuild item list from IGDE model
	pItems.RemoveAll();
	pSelectedItem = -1;
	Invalidate();
}

#endif
