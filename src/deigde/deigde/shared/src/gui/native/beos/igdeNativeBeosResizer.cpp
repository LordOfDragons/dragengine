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

#include "igdeNativeBeosResizer.h"

#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosResizer
////////////////////////////////

igdeNativeBeosResizer::igdeNativeBeosResizer(BRect frame, eResizeDirection direction) :
BView(frame, "resizer", B_FOLLOW_NONE, B_WILL_DRAW),
pResizeDirection(direction),
pLastMousePos(0, 0),
pResizing(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
}

igdeNativeBeosResizer::~igdeNativeBeosResizer(){
}


// Management
//////////////

void igdeNativeBeosResizer::SetResizeDirection(eResizeDirection direction){
	pResizeDirection = direction;
	Invalidate();
}

void igdeNativeBeosResizer::Draw(BRect updateRect){
	if(updateRect != Bounds()){
		return;
	}
	
	BRect bounds = Bounds();
	const rgb_color darkColor{128, 128, 128, 255};
	const rgb_color lightColor{192, 192, 192, 255};
	const rgb_color backgroundColor = ui_color(B_PANEL_BACKGROUND_COLOR);
	
	// Draw resize handle area
	SetHighColor(backgroundColor);
	FillRect(bounds);
	
	// Draw resize corner indicator
	const int32 handleSize = 5;
	BPoint handlePos;
	
	if(pResizeDirection & erdRight){
		handlePos.x = bounds.right - handleSize;
		
	}else{
		handlePos.x = bounds.left;
	}
	
	if(pResizeDirection & erdBottom){
		handlePos.y = bounds.bottom - handleSize;
		
	}else{
		handlePos.y = bounds.top;
	}
	
	// Draw corner grip pattern
	SetHighColor(darkColor);
	for(int i=0; i<3; i++){
		BPoint p1(handlePos.x + i * 2, handlePos.y + i * 2);
		BPoint p2(p1.x + handleSize, p1.y);
		StrokeLine(p1, p2);
	}
	
	SetHighColor(lightColor);
	for(int i=0; i<3; i++){
		BPoint p1(handlePos.x + 1 + i * 2, handlePos.y + 1 + i * 2);
		BPoint p2(p1.x + handleSize, p1.y);
		StrokeLine(p1, p2);
	}
}

void igdeNativeBeosResizer::MouseDown(BPoint where){
	pLastMousePos = where;
	pResizing = true;
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}

void igdeNativeBeosResizer::MouseUp(BPoint where){
	pResizing = false;
}

void igdeNativeBeosResizer::MouseMoved(BPoint where, uint32 code, const BMessage *dragMessage){
	if(!pResizing){
		return;
	}
	
	// Calculate delta movement
	float deltaX = where.x - pLastMousePos.x;
	float deltaY = where.y - pLastMousePos.y;
	
	// Get parent window for resizing
	BWindow *window = Window();
	if(!window){
		return;
	}
	
	BRect windowFrame = window->Frame();
	
	// Adjust window frame based on resize direction
	if(pResizeDirection & erdLeft){
		windowFrame.left += deltaX;
		
	}else if(pResizeDirection & erdRight){
		windowFrame.right += deltaX;
	}
	
	if(pResizeDirection & erdTop){
		windowFrame.top += deltaY;
		
	}else if(pResizeDirection & erdBottom){
		windowFrame.bottom += deltaY;
	}
	
	// Enforce minimum window size
	const float minWidth = 200.0f;
	const float minHeight = 100.0f;
	if(windowFrame.Width() < minWidth){
		if(pResizeDirection & erdRight){
			windowFrame.right = windowFrame.left + minWidth;
			
		}else{
			windowFrame.left = windowFrame.right - minWidth;
		}
	}
	
	if(windowFrame.Height() < minHeight){
		if(pResizeDirection & erdBottom){
			windowFrame.bottom = windowFrame.top + minHeight;
			
		}else{
			windowFrame.top = windowFrame.bottom - minHeight;
		}
	}
	
	// Move window to new position and size
	window->MoveTo(windowFrame.LeftTop());
	window->ResizeTo(windowFrame.Width(), windowFrame.Height());
	
	pLastMousePos = where;
}

#endif
