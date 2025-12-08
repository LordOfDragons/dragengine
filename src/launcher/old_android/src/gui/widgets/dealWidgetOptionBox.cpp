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
#include <android/native_activity.h>

#include "dealGuiTheme.h"
#include "dealWidgetOptionBox.h"
#include "dealWidgetOptionGroup.h"
#include "dealWidgetLabel.h"
#include "dealWidgetImage.h"
#include "dealWidgetLayoutFlow.h"
#include "dealWidgetLayoutStack.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../dealImage.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLoggerFile.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetOptionBox
//////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetOptionBox::dealWidgetOptionBox(dealDisplay &display, dealImage *image, const char *label) :
dealWidgetLayoutFlow(display, true, 0, eltTop, eltCenterFill),

pLabel(NULL),
pImage(NULL),
pBoxImageDeselected(NULL),
pBoxImageSelected(NULL),
pSelected(false)
{
	SetGap(display.GetDefaultFontSize() / 4);
	SetPadding(display.GetDefaultFontSize() / 4, display.GetDefaultFontSize() / 4);
	pBuildContent();
	SetLabel(label);
	SetImage(image);
}

dealWidgetOptionBox::~dealWidgetOptionBox(){
	SetGroup(NULL);
}



// Management
///////////////

void dealWidgetOptionBox::SetFromGuiTheme(const dealGuiTheme &guitheme){
	dealWidget::SetFromGuiTheme(guitheme);
	
	SetBackgroundColor(guitheme.GetOptionBoxBackgroundColor());
	pLabel->SetColor(guitheme.GetOptionBoxTextColor());
	
	pBoxImageDeselected->SetImage(guitheme.GetOptionBoxImageDeselected());
	pBoxImageDeselected->SetScaling(guitheme.GetOptionBoxImageScale());
	pBoxImageDeselected->SetColorize(guitheme.GetOptionBoxImageDeselectedColorize());
	
	pBoxImageSelected->SetImage(guitheme.GetOptionBoxImageSelected());
	pBoxImageSelected->SetScaling(guitheme.GetOptionBoxImageScale());
	pBoxImageSelected->SetColorize(guitheme.GetOptionBoxImageSelectedColorize());
}



const decColor &dealWidgetOptionBox::GetTextColor() const{
	return pLabel->GetColor();
}

void dealWidgetOptionBox::SetTextColor(const decColor &color){
	pLabel->SetColor(color);
}



const decString &dealWidgetOptionBox::GetLabel() const{
	return pLabel->GetText();
}

void dealWidgetOptionBox::SetLabel(const char *label){
	if(pLabel->GetText() == label){
		return;
	}
	
	pLabel->SetText(label);
	pLabel->SetVisible(!pLabel->GetText().IsEmpty());
}

dealImage *dealWidgetOptionBox::GetImage() const{
	return pImage->GetImage();
}

void dealWidgetOptionBox::SetImage(dealImage *image){
	if(pImage->GetImage() == image){
		return;
	}
	
	pImage->SetImage(image);
	pImage->SetVisible(image != NULL);
}



void dealWidgetOptionBox::SetGroup(dealWidgetOptionGroup::Ref group){
	if(group == pGroup){
		return;
	}
	
	if(pGroup){
		pGroup->Remove(this);
	}
	
	if(group){
		group->Add(this);
	}
	
	pGroup = group;
}

void dealWidgetOptionBox::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	pBoxImageSelected->SetVisible(selected);
	pBoxImageDeselected->SetVisible(!selected);
	
	if(selected && pGroup){
		pGroup->Select(this);
	}
	
	OnStateChanged();
}



void dealWidgetOptionBox::OnEnabledChanged(){
	dealWidgetLayoutFlow::OnEnabledChanged();
	
	const bool enabled = GetEnabled();
	pImage->SetEnabled(enabled);
	pLabel->SetEnabled(enabled);
	pBoxImageDeselected->SetEnabled(enabled);
	pBoxImageSelected->SetEnabled(enabled);
}

void dealWidgetOptionBox::OnStateChanged(){
}



void dealWidgetOptionBox::OnKeyPress(int keycode){
}

void dealWidgetOptionBox::OnKeyRelease(int keycode){
}

void dealWidgetOptionBox::OnMousePress(int buttons, const decPoint &position){
	if(!GetEnabled()){
		return;
	}
	
	if(buttons != 0){
		// buttons selected while touching screen. protects also against multiple touching
		return;
	}
	
	SetSelected(true);
}



// Private functions
//////////////////////

void dealWidgetOptionBox::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	// content
	dealWidgetLayoutStack * const boxes = new dealWidgetLayoutStack(display, false);
	
	pBoxImageDeselected = new dealWidgetImage(display);
	boxes->AddWidget(pBoxImageDeselected);
	
	pBoxImageSelected = new dealWidgetImage(display);
	pBoxImageSelected->SetVisible(false);
	boxes->AddWidget(pBoxImageSelected);
	
	AddWidget(boxes);
	
	pImage = new dealWidgetImage(display);
	pImage->SetVisible(false);
	AddWidget(pImage);
	
	pLabel = new dealWidgetLabel(display, "");
	pLabel->SetVisible(false);
	AddWidget(pLabel);
}
