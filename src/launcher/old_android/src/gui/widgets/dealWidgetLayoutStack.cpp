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

#include "dealWidgetLayoutStack.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLayoutStack
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLayoutStack::dealWidgetLayoutStack(dealDisplay &display) :
dealWidgetLayout(display),
pFullSize(true){
}

dealWidgetLayoutStack::dealWidgetLayoutStack(dealDisplay &display, bool fullSize) :
dealWidgetLayout(display),
pFullSize(fullSize){
}

dealWidgetLayoutStack::~dealWidgetLayoutStack(){
}



// Management
///////////////

void dealWidgetLayoutStack::SetFullSize(bool fullSize){
	if(fullSize == pFullSize){
		return;
	}
	
	pFullSize = fullSize;
	DirtyLayout();
}



decPoint dealWidgetLayoutStack::GetMinimumSize(){
	if(GetHasExplicitMinimumSize()){
		return GetExplicitMinimumSize();
	}
	return GetRealMinimumSize();
}



void dealWidgetLayoutStack::LayoutWidgets(){
	const int count = GetWidgetCount();
	if(count == 0){
		return;
	}
	
	const decPoint contentArea(GetContentArea());
	int i;
	
	if(pFullSize){
		const decPoint position(GetPaddingLeft(), GetPaddingTop());
		
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(!widget.GetVisible()){
				continue;
			}
			
			widget.SetPosition(position);
			widget.SetSize(contentArea);
		}
		
	}else{
		const decPoint center(decPoint(GetPaddingLeft(), GetPaddingTop()) + contentArea / 2);
		
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(!widget.GetVisible()){
				continue;
			}
			
			decPoint widgetSize(widget.GetMinimumSize());
			
			widget.SetPosition(center - widgetSize / 2);
			widget.SetSize(widgetSize);
		}
	}
}

decPoint dealWidgetLayoutStack::GetRealMinimumSize(){
	const int count = GetWidgetCount();
	if(count == 0){
		return decPoint(GetPaddingLeft() + GetPaddingRight(), GetPaddingTop() + GetPaddingBottom());
	}
	
	decPoint size;
	int i;
	
	for(i=0; i<count; i++){
		dealWidget &widget = *GetWidgetAt(i);
		if(!widget.GetVisible()){
			continue;
		}
		
		size.SetLargest(widget.GetMinimumSize());
	}
	
	size.x += GetPaddingLeft() + GetPaddingRight();
	size.y += GetPaddingTop() + GetPaddingBottom();
	
	return decPoint().Largest(size);
}
