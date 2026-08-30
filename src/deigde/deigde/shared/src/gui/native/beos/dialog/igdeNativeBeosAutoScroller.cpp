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

#include "igdeNativeBeosAutoScroller.h"

#include <dragengine/common/exceptions.h>
#include <ScrollView.h>


// Scroll direction constants
#define SCROLL_UP    0x01
#define SCROLL_DOWN  0x02
#define SCROLL_LEFT  0x04
#define SCROLL_RIGHT 0x08


// Class igdeNativeBeosAutoScroller
/////////////////////////////////////

igdeNativeBeosAutoScroller::igdeNativeBeosAutoScroller(BView *view) :
pView(view),
pLastMousePos(0, 0),
pScrollDirection(0),
pScrollSpeed(5)
{
	DEASSERT_NOTNULL(view)
}

igdeNativeBeosAutoScroller::~igdeNativeBeosAutoScroller() = default;


// Management
//////////////

void igdeNativeBeosAutoScroller::SetView(BView *view){
	DEASSERT_NOTNULL(view)
	pView = view;
}

void igdeNativeBeosAutoScroller::UpdateScroll(BPoint mousePos, BRect scrollArea){
	if(!pView){
		return;
	}
	
	pLastMousePos = mousePos;
	pScrollDirection = 0;
	
	// Determine scroll direction based on mouse position relative to scroll area
	const int32 scrollMargin = 20;
	
	if(mousePos.y < scrollArea.top + scrollMargin){
		pScrollDirection |= SCROLL_UP;
	}
	if(mousePos.y > scrollArea.bottom - scrollMargin){
		pScrollDirection |= SCROLL_DOWN;
	}
	if(mousePos.x < scrollArea.left + scrollMargin){
		pScrollDirection |= SCROLL_LEFT;
	}
	if(mousePos.x > scrollArea.right - scrollMargin){
		pScrollDirection |= SCROLL_RIGHT;
	}
	
	// Perform actual scrolling
	if(pScrollDirection & SCROLL_UP){
		BPoint scrollOffset = pView->Bounds().LeftTop();
		scrollOffset.y -= pScrollSpeed;
		pView->ScrollTo(scrollOffset);
	}
	if(pScrollDirection & SCROLL_DOWN){
		BPoint scrollOffset = pView->Bounds().LeftTop();
		scrollOffset.y += pScrollSpeed;
		pView->ScrollTo(scrollOffset);
	}
	if(pScrollDirection & SCROLL_LEFT){
		BPoint scrollOffset = pView->Bounds().LeftTop();
		scrollOffset.x -= pScrollSpeed;
		pView->ScrollTo(scrollOffset);
	}
	if(pScrollDirection & SCROLL_RIGHT){
		BPoint scrollOffset = pView->Bounds().LeftTop();
		scrollOffset.x += pScrollSpeed;
		pView->ScrollTo(scrollOffset);
	}
}

void igdeNativeBeosAutoScroller::StartAutoScroll(){
	pScrollDirection = 0;
}

void igdeNativeBeosAutoScroller::StopAutoScroll(){
	pScrollDirection = 0;
}

void igdeNativeBeosAutoScroller::SetScrollSpeed(int32 speed){
	if(speed < 1){
		pScrollSpeed = 1;
		
	}else if(speed > 100){
		pScrollSpeed = 100;
		
	}else{
		pScrollSpeed = speed;
	}
}

#endif
