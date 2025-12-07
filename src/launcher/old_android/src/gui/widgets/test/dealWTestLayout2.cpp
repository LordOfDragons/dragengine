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

#include "dealWTestLayout2.h"
#include "../dealWidgetLayoutFlow.h"
#include "../dealWidgetLabel.h"
#include "../../dealDisplay.h"
#include "../../../common/exceptions.h"
#include "../../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWTestLayout2
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWTestLayout2::dealWTestLayout2(dealDisplay &display) :
dealWidgetLayoutFlow(display, false, 20, eltFill, eltFill){
	SetPaddingLeft(50);
	SetPaddingTop(100);
	SetPaddingRight(50);
	SetPaddingBottom(50);
	
// 	AddLayout2( dealWidgetLabel::eaTop );
// 	AddLayout2( dealWidgetLabel::eaMiddle );
// 	AddLayout2( dealWidgetLabel::eaBottom );
	
	AddLayout2(dealWidgetLabel::eaTop);
	AddLayout3(dealWidgetLabel::eaLeft | dealWidgetLabel::eaTop);
}

dealWTestLayout2::~dealWTestLayout2(){
}



// Management
///////////////

void dealWTestLayout2::AddLayout(dealWidgetLabel::eAlignments verticalAlign){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow(GetDisplay(), false, 5, eltFill, eltFill);
	layout->SetBackgroundColor(decColor(0.75f, 0.75f, 0.75f));
	layout->SetPadding(5, 5);
	
	dealWidgetLabel *widget = new dealWidgetLabel(GetDisplay(), "Left aligned");
	widget->SetBackgroundColor(decColor(1.0f, 0.8f, 0.8f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaLeft);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	widget = new dealWidgetLabel(GetDisplay(), "Center aligned");
	widget->SetBackgroundColor(decColor(0.8f, 1.0f, 0.8f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaCenter);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	widget = new dealWidgetLabel(GetDisplay(), "Right aligned");
	widget->SetBackgroundColor(decColor(0.8f, 0.8f, 1.0f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaRight);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	AddWidget(layout);
	layout->FreeReference();
}

void dealWTestLayout2::AddLayout2(dealWidgetLabel::eAlignments verticalAlign){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow(GetDisplay(), false, 5, eltFill, eltFill);
	layout->SetBackgroundColor(decColor(0.75f, 0.75f, 0.75f));
	layout->SetPadding(5, 5);
	
	dealWidgetLabel *widget = new dealWidgetLabel(GetDisplay(), "Left aligned\nMultiline Text with\nthree lines");
	widget->SetBackgroundColor(decColor(1.0f, 0.8f, 0.8f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaLeft);
	widget->SetFontSize(GetDisplay().GetDefaultFontSize() / 2);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	widget = new dealWidgetLabel(GetDisplay(), "Center aligned\nMultiline Text with\nthree lines");
	widget->SetBackgroundColor(decColor(0.8f, 1.0f, 0.8f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaCenter);
	widget->SetFontSize(GetDisplay().GetDefaultFontSize() / 2);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	widget = new dealWidgetLabel(GetDisplay(), "Right aligned\nMultiline Text with\nthree lines");
	widget->SetBackgroundColor(decColor(0.8f, 0.8f, 1.0f));
	widget->SetAlignment(verticalAlign | dealWidgetLabel::eaRight);
	widget->SetFontSize(GetDisplay().GetDefaultFontSize() / 2);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	AddWidget(layout);
	layout->FreeReference();
}

void dealWTestLayout2::AddLayout3(dealWidgetLabel::eAlignments align){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow(GetDisplay(), false, 5, eltFill, eltFill);
	layout->SetBackgroundColor(decColor(0.75f, 0.75f, 0.75f));
	layout->SetPadding(5, 5);
	
	dealWidgetLabel *widget = new dealWidgetLabel(GetDisplay(), "A short line.\n"
		"This is a long line that is word-wrapped to the next line. "
		"It contains a bunch of text which is not much of interest except filling space.\n"
		"Another short line.");
	widget->SetBackgroundColor(decColor(1.0f, 0.8f, 0.8f));
	widget->SetAlignment(align);
	layout->AddWidget(widget);
	widget->FreeReference();
	
	AddWidget(layout);
	layout->FreeReference();
}
