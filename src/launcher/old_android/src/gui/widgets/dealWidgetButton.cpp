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
#include "dealWidgetButton.h"
#include "dealWidgetLabel.h"
#include "dealWidgetImage.h"
#include "dealWidgetBorderImage.h"
#include "dealWidgetLayoutFlow.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../dealImage.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLoggerFile.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetButton
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetButton::dealWidgetButton(dealDisplay &display, dealImage *image, const char *label) :
dealWidgetLayoutFlow(display, false, 0, eltFill, eltBottomFill),

pBgColorNormal(1.0f, 1.0f, 1.0f),
pTextColorNormal(0.0f, 0.0f, 0.0f),
pBgColorPressed(0.8f, 0.8f, 1.0f),
pTextColorPressed(0.0f, 0.0f, 0.0f),

pLabel(NULL),
pImage(NULL),
pPressed(false)
{
	SetGap(display.GetDefaultFontSize() / 4);
	SetPadding(display.GetDefaultFontSize() / 2, display.GetDefaultFontSize() / 2);
	pBuildContent();
	SetLabel(label);
	SetImage(image);
}

dealWidgetButton::~dealWidgetButton(){
}



// Management
///////////////

void dealWidgetButton::SetFromGuiTheme(const dealGuiTheme &guitheme){
	dealWidget::SetFromGuiTheme(guitheme);
	
	SetBgColorNormal(guitheme.GetButtonBackgroundColor());
	SetTextColorNormal(guitheme.GetButtonTextColor());
	pBgImageNormal->SetImage(guitheme.GetButtonBackgroundImage());
	pBgImageNormal->SetBorders(
		guitheme.GetButtonBackgroundImageBorder(0),
		guitheme.GetButtonBackgroundImageBorder(1),
		guitheme.GetButtonBackgroundImageBorder(2),
		guitheme.GetButtonBackgroundImageBorder(3));
	pBgImageNormal->SetScaling(guitheme.GetButtonBackgroundImageScale());
	pBgImageNormal->SetColorize(guitheme.GetButtonBackgroundImageColorize());
	
	SetBgColorPressed(guitheme.GetButtonPressedBackgroundColor());
	SetTextColorPressed(guitheme.GetButtonPressedTextColor());
	
	pBgImagePressed->SetImage(guitheme.GetButtonPressedBackgroundImage());
	pBgImagePressed->SetBorders(
		guitheme.GetButtonPressedBackgroundImageBorder(0),
		guitheme.GetButtonPressedBackgroundImageBorder(1),
		guitheme.GetButtonPressedBackgroundImageBorder(2),
		guitheme.GetButtonPressedBackgroundImageBorder(3));
	pBgImagePressed->SetScaling(guitheme.GetButtonPressedBackgroundImageScale());
	pBgImagePressed->SetColorize(guitheme.GetButtonPressedBackgroundImageColorize());
}



void dealWidgetButton::SetBgColorNormal(const decColor &color){
	if(color.IsEqualTo(pBgColorNormal)){
		return;
	}
	
	pBgColorNormal = color;
	pUpdateContentPressedState();
}

void dealWidgetButton::SetTextColorNormal(const decColor &color){
	if(color.IsEqualTo(pTextColorNormal)){
		return;
	}
	
	pTextColorNormal = color;
	pUpdateContentPressedState();
}

const decColor &dealWidgetButton::GetBgImageColorizeNormal() const{
	return pBgImageNormal->GetColorize();
}

void dealWidgetButton::SetBgImageColorizeNormal(const decColor &color){
	pBgImageNormal->SetColorize(color);
}

void dealWidgetButton::SetBgColorPressed(const decColor &color){
	if(color.IsEqualTo(pBgColorPressed)){
		return;
	}
	
	pBgColorPressed = color;
	pUpdateContentPressedState();
}

void dealWidgetButton::SetTextColorPressed(const decColor &color){
	if(color.IsEqualTo(pTextColorPressed)){
		return;
	}
	
	pTextColorPressed = color;
	pUpdateContentPressedState();
}

const decColor &dealWidgetButton::GetBgImageColorizePressed() const{
	return pBgImagePressed->GetColorize();
}

void dealWidgetButton::SetBgImageColorizePressed(const decColor &color){
	pBgImagePressed->SetColorize(color);
}



const decString &dealWidgetButton::GetLabel() const{
	return pLabel->GetText();
}

void dealWidgetButton::SetLabel(const char *label){
	if(pLabel->GetText() == label){
		return;
	}
	
	pLabel->SetText(label);
	pLabel->SetVisible(!pLabel->GetText().IsEmpty());
}

dealImage *dealWidgetButton::GetImage() const{
	return pImage->GetImage();
}

void dealWidgetButton::SetImage(dealImage *image){
	if(pImage->GetImage() == image){
		return;
	}
	
	pImage->SetImage(image);
	pImage->SetVisible(image != NULL);
}



void dealWidgetButton::SetPressed(bool pressed){
	if(pressed == pPressed){
		return;
	}
	
	pPressed = pressed;
	
	pUpdateContentPressedState();
	OnStateChanged();
}



void dealWidgetButton::RenderBackground(const sRenderContext &context){
	if(pPressed){
		if(pBgImagePressed->GetImage()){
			pBgImagePressed->Render(context);
			
		}else{
			dealWidgetLayoutFlow::RenderBackground(context);
		}
		
	}else{
		if(pBgImageNormal->GetImage()){
			pBgImageNormal->Render(context);
			
		}else{
			dealWidgetLayoutFlow::RenderBackground(context);
		}
	}
}



void dealWidgetButton::OnSizeChanged(){
	dealWidgetLayoutFlow::OnSizeChanged();
	
	pBgImageNormal->SetSize(GetSize());
	pBgImagePressed->SetSize(GetSize());
}

void dealWidgetButton::OnEnabledChanged(){
	dealWidgetLayoutFlow::OnEnabledChanged();
	
	const bool enabled = GetEnabled();
	pImage->SetEnabled(enabled);
	pLabel->SetEnabled(enabled);
	pBgImageNormal->SetEnabled(enabled);
	pBgImagePressed->SetEnabled(enabled);
	
	pUpdateContentPressedState();
}

void dealWidgetButton::OnStateChanged(){
}

void dealWidgetButton::OnAction(){
}



void dealWidgetButton::OnKeyPress(int keycode){
}

void dealWidgetButton::OnKeyRelease(int keycode){
}

void dealWidgetButton::OnMousePress(int buttons, const decPoint &position){
	if(!GetEnabled()){
		return;
	}
	
	if(buttons != 0){
		// buttons pressed while touching screen. protects also against multiple touching
		return;
	}
	
	CaptureInput();
	SetPressed(true);
}

void dealWidgetButton::OnMouseRelease(int buttons, const decPoint &position){
	if(buttons != 0){
		// buttons pressed while touching screen. protects also against multiple touching
		return;
	}
	
	ReleaseCapture();
	
	if(GetEnabled() && IsPointInside(position)){
		OnAction();
	}
	
	SetPressed(false);
}

void dealWidgetButton::OnMouseMove(int buttons, const decPoint &position){
	SetPressed(IsPointInside(position));
}



// Private functions
//////////////////////

void dealWidgetButton::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	// background images
	pBgImageNormal.TakeOver(new dealWidgetBorderImage(display));
	pBgImagePressed.TakeOver(new dealWidgetBorderImage(display));
	
	// content
	pImage = new dealWidgetImage(display);
	pImage->SetVisible(false);
	AddWidget(pImage);
	
	pLabel = new dealWidgetLabel(display, "");
	pLabel->SetVisible(false);
	AddWidget(pLabel);
	
	// update pressed state
	pUpdateContentPressedState();
}

void dealWidgetButton::pUpdateContentPressedState(){
	if(GetEnabled()){
		if(pPressed){
			SetBackgroundColor(pBgColorPressed);
			pLabel->SetColor(pTextColorPressed);
			
		}else{
			SetBackgroundColor(pBgColorNormal);
			pLabel->SetColor(pTextColorNormal);
		}
		
	}else{
		SetBackgroundColor(pBgColorNormal);
		pLabel->SetColor(pTextColorNormal);
	}
}
