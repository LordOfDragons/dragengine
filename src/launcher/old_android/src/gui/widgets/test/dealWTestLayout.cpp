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

#include "dealWTestLayout.h"
#include "../dealWidgetLayoutFlow.h"
#include "../../dealDisplay.h"
#include "../../../common/exceptions.h"
#include "../../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWTestLayout
//////////////////////////

// Constructors, destructors
//////////////////////////////

dealWTestLayout::dealWTestLayout(dealDisplay &display) :
dealWidgetLayout(display){
	const int gap = 50;
	const int height = 55 * 7;
	const int offset = height + gap;
	
	if(true){
		AddLayoutLinesX(0, dealWidgetLayoutFlow::eltFill);
		AddLayoutLinesX(offset, dealWidgetLayoutFlow::eltTop);
		AddLayoutLinesX(offset * 2, dealWidgetLayoutFlow::eltCenter);
		AddLayoutLinesX(offset * 3, dealWidgetLayoutFlow::eltBottom);
		
	}else{
		AddLayoutLinesY(0, 0, dealWidgetLayoutFlow::eltFill);
		AddLayoutLinesY(offset, 0, dealWidgetLayoutFlow::eltTop);
		AddLayoutLinesY(0, (display.GetHeight() - 80) / 2, dealWidgetLayoutFlow::eltCenter);
		AddLayoutLinesY(offset, (display.GetHeight() - 80) / 2, dealWidgetLayoutFlow::eltBottom);
	}
}

dealWTestLayout::~dealWTestLayout(){
}



// Management
///////////////

void dealWTestLayout::AddLayoutLinesX(int top, dealWidgetLayoutFlow::eLayoutTypes layoutY){
	const int screenWidth = GetDisplay().GetWidth() - 40;
	int i;
	
	const dealWidgetLayoutFlow::eLayoutTypes layoutTypes[7] = {
		dealWidgetLayoutFlow::eltFill,
		dealWidgetLayoutFlow::eltTop,
		dealWidgetLayoutFlow::eltTopFill,
		dealWidgetLayoutFlow::eltCenter,
		dealWidgetLayoutFlow::eltCenterFill,
		dealWidgetLayoutFlow::eltBottom,
		dealWidgetLayoutFlow::eltBottomFill
	};
	
	for(i=0; i<7; i++){
		dealWidgetLayoutFlow * const layout = CreateLayoutLineX();
		layout->SetPosition(decPoint(25, top + 55 * i));
		layout->SetSize(decPoint(screenWidth - 50, 50));
		layout->SetHorizontal(true);
		layout->SetGap(5);
		layout->SetLayoutX(layoutTypes[i]);
		layout->SetLayoutY(layoutY);
		AddWidget(layout);
	}
}

void dealWTestLayout::AddLayoutLinesY(int left, int top, dealWidgetLayoutFlow::eLayoutTypes layoutX){
	const int screenHeight = (GetDisplay().GetHeight() - 80) / 2 - 50;
	int i;
	
	const dealWidgetLayoutFlow::eLayoutTypes layoutTypes[7] = {
		dealWidgetLayoutFlow::eltFill,
		dealWidgetLayoutFlow::eltTop,
		dealWidgetLayoutFlow::eltTopFill,
		dealWidgetLayoutFlow::eltCenter,
		dealWidgetLayoutFlow::eltCenterFill,
		dealWidgetLayoutFlow::eltBottom,
		dealWidgetLayoutFlow::eltBottomFill
	};
	
	for(i=0; i<7; i++){
		dealWidgetLayoutFlow * const layout = CreateLayoutLineY();
		layout->SetPosition(decPoint(left + 55 * i, top + 25));
		layout->SetSize(decPoint(50, screenHeight - 50));
		layout->SetHorizontal(false);
		layout->SetGap(5);
		layout->SetLayoutY(layoutTypes[i]);
		layout->SetLayoutX(layoutX);
		AddWidget(layout);
	}
}

dealWidgetLayoutFlow *dealWTestLayout::CreateLayoutLineX(){
	dealWidgetLayoutFlow::Ref layout.TakeOver(new dealWidgetLayoutFlow(GetDisplay()));
	layout->SetBackgroundColor(decColor(0.75f, 0.75f, 0.75f));
	layout->SetPadding(5, 5);
	
	int i;
	for(i=0; i<5; i++){
		dealWidget * const widget = new dealWidget(GetDisplay());
		widget->SetExplicitMinimumSize(decPoint(100, 25));
		widget->SetBackgroundColor(decColor((float)i / 4.0f, 1.0f - (float)i / 4.0f, 0.0f));
		layout->AddWidget(widget);
		widget->FreeReference();
	}
	
	return layout;
}

dealWidgetLayoutFlow *dealWTestLayout::CreateLayoutLineY(){
	dealWidgetLayoutFlow::Ref layout.TakeOver(new dealWidgetLayoutFlow(GetDisplay()));
	layout->SetBackgroundColor(decColor(0.75f, 0.75f, 0.75f));
	layout->SetPadding(5, 5);
	
	int i;
	for(i=0; i<5; i++){
		dealWidget * const widget = new dealWidget(GetDisplay());
		widget->SetExplicitMinimumSize(decPoint(25, 100));
		widget->SetBackgroundColor(decColor((float)i / 4.0f, 1.0f - (float)i / 4.0f, 0.0f));
		layout->AddWidget(widget);
		widget->FreeReference();
	}
	
	return layout;
}
