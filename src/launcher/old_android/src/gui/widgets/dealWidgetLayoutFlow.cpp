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

#include "dealWidgetLayoutFlow.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLayoutFlow
///////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLayoutFlow::dealWidgetLayoutFlow(dealDisplay &display) :
dealWidgetLayout(display),
pHorizontal(false),
pGap(0),
pLayoutX(eltFill),
pLayoutY(eltFill){
}

dealWidgetLayoutFlow::dealWidgetLayoutFlow(dealDisplay &display, bool horizontal,
int gap, eLayoutTypes layoutX, eLayoutTypes layoutY) :
dealWidgetLayout(display),
pHorizontal(horizontal),
pGap(gap),
pLayoutX(eltFill),
pLayoutY(eltFill)
{
	SetLayoutX(layoutX);
	SetLayoutY(layoutY);
}

dealWidgetLayoutFlow::~dealWidgetLayoutFlow(){
}



// Management
///////////////

void dealWidgetLayoutFlow::SetHorizontal(bool horizontal){
	if(horizontal == pHorizontal){
		return;
	}
	
	pHorizontal = horizontal;
	DirtyLayout();
}

void dealWidgetLayoutFlow::SetGap(int gap){
	if(gap == pGap){
		return;
	}
	
	pGap = gap;
	DirtyLayout();
}

void dealWidgetLayoutFlow::SetLayoutX(eLayoutTypes type){
	if(type < eltFill || type > eltBottomFill){
		DETHROW(deeInvalidParam);
	}
	if(type == pLayoutX){
		return;
	}
	
	pLayoutX = type;
	DirtyLayout();
}

void dealWidgetLayoutFlow::SetLayoutY(eLayoutTypes type){
	if(type < eltFill || type > eltBottomFill){
		DETHROW(deeInvalidParam);
	}
	if(type == pLayoutY){
		return;
	}
	
	pLayoutY = type;
	DirtyLayout();
}



decPoint dealWidgetLayoutFlow::GetMinimumSize(){
	if(GetHasExplicitMinimumSize()){
		return GetExplicitMinimumSize();
	}
	return GetRealMinimumSize();
}



void dealWidgetLayoutFlow::LayoutWidgets(){
	const int count = GetWidgetCount();
	if(count == 0){
		return;
	}
	
	const decPoint minSize(GetRealMinimumSize());
	const decPoint contentArea(GetContentArea());
	const decPoint size(GetSize());
	
	decPoint curPos(GetPaddingLeft(), GetPaddingTop());
	int i, sizeGain;
	
	if(pHorizontal){
		int availSpace = size.x - minSize.x;
		
		switch(pLayoutX){
		case eltBottom:
		case eltBottomFill:
			break;
			
		default:
			availSpace = decMath::max(availSpace, 0);
		}
		
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(! widget.GetVisible()){
				continue;
			}
			
			decPoint widgetSize(widget.GetMinimumSize());
			decPoint widgetPos(curPos);
			
			// horizontal align
			switch(pLayoutX){
			case eltFill:
				sizeGain = availSpace / (count - i);
				widgetSize.x += sizeGain;
				availSpace -= sizeGain;
				break;
				
			case eltTop:
				break;
				
			case eltTopFill:
				if(i == count - 1){
					widgetSize.x += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltCenter:
				if(i == 0){
					widgetPos.x += availSpace / 2;
					availSpace = 0;
				}
				break;
				
			case eltCenterFill:
				if(i == 0){
					widgetSize.x += availSpace / 2;
					availSpace -= availSpace / 2;
				}
				if(i == count - 1){
					widgetSize.x += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltBottom:
				if(i == 0){
					widgetPos.x += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltBottomFill:
				if(i == 0){
					widgetSize.x += availSpace;
					availSpace = 0;
				}
				break;
			}
			
			// vertical align
			switch(pLayoutY){
			case eltFill:
			case eltTopFill:
			case eltCenterFill:
			case eltBottomFill:
				widgetSize.y = contentArea.y;
				break;
				
			case eltTop:
				break;
				
			case eltCenter:
				widgetPos.y += (contentArea.y - widgetSize.y) / 2;
				break;
				
			case eltBottom:
				widgetPos.y = size.y - GetPaddingBottom() - widgetSize.y;
				break;
			}
			
			// apply layout
			widget.SetPosition(widgetPos);
			widget.SetSize(widgetSize);
			
			curPos.x = widgetPos.x + widgetSize.x + pGap;
		}
		
	}else{
		int availSpace = size.y - minSize.y;
		
		switch(pLayoutY){
		case eltBottom:
		case eltBottomFill:
			break;
			
		default:
			availSpace = decMath::max(availSpace, 0);
		}
		
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(! widget.GetVisible()){
				continue;
			}
			
			decPoint widgetSize(widget.GetMinimumSize());
			decPoint widgetPos(curPos);
			
			// vertical align
			switch(pLayoutY){
			case eltFill:
				sizeGain = availSpace / (count - i);
				widgetSize.y += sizeGain;
				availSpace -= sizeGain;
				break;
				
			case eltTop:
				break;
				
			case eltTopFill:
				if(i == count - 1){
					widgetSize.y += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltCenter:
				if(i == 0){
					widgetPos.y += availSpace / 2;
					availSpace = 0;
				}
				break;
				
			case eltCenterFill:
				if(i == 0){
					widgetSize.y += availSpace / 2;
					availSpace -= availSpace / 2;
				}
				if(i == count - 1){
					widgetSize.y += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltBottom:
				if(i == 0){
					widgetPos.y += availSpace;
					availSpace = 0;
				}
				break;
				
			case eltBottomFill:
				if(i == 0){
					widgetSize.y += availSpace;
					availSpace = 0;
				}
				break;
			}
			
			// horizontal align
			switch(pLayoutX){
			case eltFill:
			case eltTopFill:
			case eltCenterFill:
			case eltBottomFill:
				widgetSize.x = contentArea.x;
				break;
				
			case eltTop:
				break;
				
			case eltCenter:
				widgetPos.x += (contentArea.x - widgetSize.x) / 2;
				break;
				
			case eltBottom:
				widgetPos.x = size.x - GetPaddingRight() - widgetSize.x;
				break;
			}
			
			// apply layout
			widget.SetPosition(widgetPos);
			widget.SetSize(widgetSize);
			
			curPos.y = widgetPos.y + widgetSize.y + pGap;
		}
	}
}

decPoint dealWidgetLayoutFlow::GetRealMinimumSize(){
	const int count = GetWidgetCount();
	if(count == 0){
		return decPoint(GetPaddingLeft() + GetPaddingRight(), GetPaddingTop() + GetPaddingBottom());
	}
	
	decPoint size;
	int i;
	
	if(pHorizontal){
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(! widget.GetVisible()){
				continue;
			}
			
			const decPoint widgetSize = widget.GetMinimumSize();
			size.x += widgetSize.x + pGap;
			if(widgetSize.y > size.y){
				size.y = widgetSize.y;
			}
		}
		size.x -= pGap; // because we added one gap too much
		
	}else{
		for(i=0; i<count; i++){
			dealWidget &widget = *GetWidgetAt(i);
			if(! widget.GetVisible()){
				continue;
			}
			
			const decPoint widgetSize = widget.GetMinimumSize();
			size.y += widgetSize.y + pGap;
			if(widgetSize.x > size.x){
				size.x = widgetSize.x;
			}
		}
		size.y -= pGap; // because we added one gap too much
	}
	
	size.x += GetPaddingLeft() + GetPaddingRight();
	size.y += GetPaddingTop() + GetPaddingBottom();
	
	return decPoint().Largest(size);
}
