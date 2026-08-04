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
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxAutoScroller.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXIMPLEMENT(igdeNativeFoxAutoScroller, FXComposite, nullptr, 0)



// Class igdeNativeFoxAutoScroller
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxAutoScroller::igdeNativeFoxAutoScroller(){
	flags |= FLAG_SHOWN;
}

igdeNativeFoxAutoScroller::igdeNativeFoxAutoScroller(FXComposite *pparent, int opts, bool horizontal) :
FXComposite(pparent, opts, 0, 0, 0, 0),
pScrollPosition(0),
pHorizontal(horizontal)
{
	flags |= FLAG_SHOWN;
}

igdeNativeFoxAutoScroller::~igdeNativeFoxAutoScroller(){
}



// Management
///////////////

FXWindow *igdeNativeFoxAutoScroller::GetContentWindow() const{
	return numChildren() > 0 ? childAtIndex(0) : nullptr;
}

decPoint igdeNativeFoxAutoScroller::GetContentSize() const{
	FXWindow * const window = GetContentWindow();
	if(!window){
		return decPoint(1, 1);
	}
	
	const int hints = window->getLayoutHints();
	return decPoint(1, 1).Largest(decPoint(
		hints & LAYOUT_FIX_WIDTH ? window->getWidth() : window->getDefaultWidth(),
		hints & LAYOUT_FIX_HEIGHT ? window->getHeight() : window->getDefaultHeight()));
}

void igdeNativeFoxAutoScroller::SetScrollPosition(int position){
	pScrollPosition = position;
	
	FXWindow * const window = GetContentWindow();
	if(window){
		if(pHorizontal){
			window->move(-pScrollPosition, window->getY());
			
		}else{
			window->move(window->getX(), -pScrollPosition);
		}
	}
}



FXint igdeNativeFoxAutoScroller::getDefaultWidth(){
	return pHorizontal ? 1 : GetContentSize().x;
}

FXint igdeNativeFoxAutoScroller::getDefaultHeight(){
	return pHorizontal ? GetContentSize().y : 1;
}

void igdeNativeFoxAutoScroller::layout(){
	FXComposite::layout();
	
	FXWindow * const window = GetContentWindow();
	if(!window){
		return;
	}
	
	const int hints = window->getLayoutHints();
	decPoint size(GetContentSize());
	decPoint position(-pScrollPosition, 0);
	
	if(pHorizontal){
		const int viewSize = getWidth();
		if(size.x < viewSize){
			if(hints & LAYOUT_FILL_X){
				size.x = viewSize;
			}
			
			if(hints & LAYOUT_CENTER_X){
				position.x = (viewSize - size.x) / 2;
				
			}else if(hints & LAYOUT_RIGHT){
				position.x = viewSize - size.x;
				
			}else{
				position.x = 0;
			}
		}
		
	}else{
		const int viewSize = getHeight();
		if(size.y < viewSize){
			if(hints & LAYOUT_FILL_Y){
				size.y = viewSize;
			}
			
			if(hints & LAYOUT_CENTER_Y){
				position.y = (viewSize - size.y) / 2;
				
			}else if(hints & LAYOUT_BOTTOM){
				position.y = viewSize - size.y;
				
			}else{
				position.y = 0;
			}
		}
	}
	
	window->position(position.x, position.y, size.x, size.y);
	recalc();
}

void igdeNativeFoxAutoScroller::MoveContentX(int x){
	if(x == pScrollPosition){
		return;
	}
	
	FXWindow * const window = GetContentWindow();
	if(!window){
		pScrollPosition = x;
		return;
	}
	
	const int hints = window->getLayoutHints();
	const int viewSize = getWidth();
	int size = GetContentSize().x;
	int position = -pScrollPosition;
	
	if(size < viewSize){
		if(hints & LAYOUT_FILL_X){
			size = viewSize;
		}
		
		if(hints & LAYOUT_CENTER_X){
			position = (viewSize - size) / 2;
			
		}else if(hints & LAYOUT_RIGHT){
			position = viewSize - size;
			
		}else{
			position = 0;
		}
	}
	
	window->move(position, 0);
	
	pScrollPosition = x;
}

void igdeNativeFoxAutoScroller::MoveContentY(int y){
	if(y == pScrollPosition){
		return;
	}
	
	FXWindow * const window = GetContentWindow();
	if(!window){
		pScrollPosition = y;
		return;
	}
	
	const int hints = window->getLayoutHints();
	const int viewSize = getHeight();
	int size = GetContentSize().y;
	int position = -pScrollPosition;
	
	if(size < viewSize){
		if(hints & LAYOUT_FILL_Y){
			size = viewSize;
		}
		
		if(hints & LAYOUT_CENTER_Y){
			position = (viewSize - size) / 2;
			
		}else if(hints & LAYOUT_BOTTOM){
			position = viewSize - size;
			
		}else{
			position = 0;
		}
	}
	
	window->move(0, position);
	
	pScrollPosition = y;
}


// Events
///////////

long igdeNativeFoxAutoScroller::onChildMouseMoved(FXObject *sender, FXSelector, void *pdata){
	const FXEvent &event = *((FXEvent*)pdata);
	FXint x, y;
	translateCoordinatesFrom(x, y, (FXWindow*)sender, event.win_x, event.win_y);
	
	if(pHorizontal){
		const int viewSize = getWidth();
		const int childSize = GetContentSize().x;
		if(viewSize < 1 || childSize <= viewSize){
			SetScrollPosition(0);
			return 0; // return 0 to let original implementation run. 1 would stop
		}
		
		SetScrollPosition((childSize - viewSize) * x / viewSize);
		
	}else{
		const int viewSize = getHeight();
		const int childSize = GetContentSize().y;
		if(viewSize < 1 || childSize <= viewSize){
			SetScrollPosition(0);
			return 0; // return 0 to let original implementation run. 1 would stop
		}
		
		SetScrollPosition((childSize - viewSize) * y / viewSize);
	}
	return 0; // return 0 to let original implementation run. 1 would stop
}

#endif
