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
#include "dealWidgetCheckBox.h"
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



// Class dealWidgetCheckBox
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetCheckBox::dealWidgetCheckBox(dealDisplay &display, dealImage *image, const char *label) :
dealWidgetLayoutFlow(display, true, 0, eltTop, eltCenterFill),

pLabel(NULL),
pImage(NULL),
pBoxImageUnchecked(NULL),
pBoxImageChecked(NULL),
pChecked(false)
{
	SetGap(display.GetDefaultFontSize() / 4);
	SetPadding(display.GetDefaultFontSize() / 4, display.GetDefaultFontSize() / 4);
	pBuildContent();
	SetLabel(label);
	SetImage(image);
}

dealWidgetCheckBox::~dealWidgetCheckBox(){
}



// Management
///////////////

void dealWidgetCheckBox::SetFromGuiTheme(const dealGuiTheme &guitheme){
	dealWidget::SetFromGuiTheme(guitheme);
	
	SetBackgroundColor(guitheme.GetCheckBoxBackgroundColor());
	pLabel->SetColor(guitheme.GetCheckBoxTextColor());
	
	pBoxImageUnchecked->SetImage(guitheme.GetCheckBoxImageUnchecked());
	pBoxImageUnchecked->SetScaling(guitheme.GetCheckBoxImageScale());
	pBoxImageUnchecked->SetColorize(guitheme.GetCheckBoxImageUncheckedColorize());
	
	pBoxImageChecked->SetImage(guitheme.GetCheckBoxImageChecked());
	pBoxImageChecked->SetScaling(guitheme.GetCheckBoxImageScale());
	pBoxImageChecked->SetColorize(guitheme.GetCheckBoxImageCheckedColorize());
}



const decColor &dealWidgetCheckBox::GetTextColor() const{
	return pLabel->GetColor();
}

void dealWidgetCheckBox::SetTextColor(const decColor &color){
	pLabel->SetColor(color);
}



const decString &dealWidgetCheckBox::GetLabel() const{
	return pLabel->GetText();
}

void dealWidgetCheckBox::SetLabel(const char *label){
	if(pLabel->GetText() == label){
		return;
	}
	
	pLabel->SetText(label);
	pLabel->SetVisible(! pLabel->GetText().IsEmpty());
}

dealImage *dealWidgetCheckBox::GetImage() const{
	return pImage->GetImage();
}

void dealWidgetCheckBox::SetImage(dealImage *image){
	if(pImage->GetImage() == image){
		return;
	}
	
	pImage->SetImage(image);
	pImage->SetVisible(image != NULL);
}



void dealWidgetCheckBox::SetChecked(bool checked){
	if(checked == pChecked){
		return;
	}
	
	pChecked = checked;
	pBoxImageChecked->SetVisible(checked);
	pBoxImageUnchecked->SetVisible(! checked);
	
	OnStateChanged();
}



void dealWidgetCheckBox::OnEnabledChanged(){
	dealWidgetLayoutFlow::OnEnabledChanged();
	
	const bool enabled = GetEnabled();
	pImage->SetEnabled(enabled);
	pLabel->SetEnabled(enabled);
	pBoxImageUnchecked->SetEnabled(enabled);
	pBoxImageChecked->SetEnabled(enabled);
}

void dealWidgetCheckBox::OnStateChanged(){
}



void dealWidgetCheckBox::OnKeyPress(int keycode){
}

void dealWidgetCheckBox::OnKeyRelease(int keycode){
}

void dealWidgetCheckBox::OnMousePress(int buttons, const decPoint &position){
	if(! GetEnabled()){
		return;
	}
	
	if(buttons != 0){
		// buttons checked while touching screen. protects also against multiple touching
		return;
	}
	
	SetChecked(! pChecked);
}



// Private functions
//////////////////////

void dealWidgetCheckBox::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	// content
	dealWidgetLayoutStack * const boxes = new dealWidgetLayoutStack(display, false);
	
	pBoxImageUnchecked = new dealWidgetImage(display);
	boxes->AddWidget(pBoxImageUnchecked);
	
	pBoxImageChecked = new dealWidgetImage(display);
	pBoxImageChecked->SetVisible(false);
	boxes->AddWidget(pBoxImageChecked);
	
	AddWidget(boxes);
	
	pImage = new dealWidgetImage(display);
	pImage->SetVisible(false);
	AddWidget(pImage);
	
	pLabel = new dealWidgetLabel(display, "");
	pLabel->SetVisible(false);
	AddWidget(pLabel);
}
