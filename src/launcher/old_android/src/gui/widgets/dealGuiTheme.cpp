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

#include "dealGuiTheme.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../dealImage.h"
#include "../../common/exceptions.h"



// Class dealGuiTheme
///////////////////////

// Constructors, destructors
//////////////////////////////

dealGuiTheme::dealGuiTheme(dealDisplay &display) :
pWidgetBackgroundColor(1.0f, 1.0f, 1.0f),
pWidgetTextColor(0.0f, 0.0f, 0.0f),
pWidgetTextFont(NULL),
pWidgetTextFontSize(display.GetDefaultFontSize()),

pButtonBackgroundColor(1.0f, 1.0f, 1.0f, 0.0f),
pButtonTextColor(0.0f, 0.0f, 0.0f),
pButtonBackgroundImage(NULL),
pButtonBackgroundImageScale(2.0f),
pButtonBackgroundImageColorize(1.0f, 1.0f, 1.0f),

pButtonPressedBackgroundColor(1.0f, 1.0f, 1.0f, 0.0f),
pButtonPressedTextColor(0.0f, 0.0f, 0.0f),
pButtonPressedBackgroundImage(NULL),
pButtonPressedBackgroundImageScale(2.0f),
pButtonPressedBackgroundImageColorize(0.95f, 1.25f, 1.25f),

pCheckBoxBackgroundColor(1.0f, 1.0f, 1.0f, 0.0f),
pCheckBoxTextColor(0.0f, 0.0f, 0.0f),
pCheckBoxImageUnchecked(NULL),
pCheckBoxImageChecked(NULL),
pCheckBoxImageScale(3.0f),
pCheckBoxImageUncheckedColorize(1.0f, 1.0f, 1.0f),
pCheckBoxImageCheckedColorize(1.0f, 1.0f, 1.0f),

pOptionBoxBackgroundColor(1.0f, 1.0f, 1.0f, 0.0f),
pOptionBoxTextColor(0.0f, 0.0f, 0.0f),
pOptionBoxImageDeselected(NULL),
pOptionBoxImageSelected(NULL),
pOptionBoxImageScale(3.0f),
pOptionBoxImageDeselectedColorize(1.0f, 1.0f, 1.0f),
pOptionBoxImageSelectedColorize(1.0f, 1.0f, 1.0f)
{
	SetWidgetTextFont(display.GetDefaultFont());
	
	pButtonBackgroundImage.TakeOverWith(display, "guitheme/button_normal.tga");
	pButtonBackgroundImageBorder[0] = 4;
	pButtonBackgroundImageBorder[1] = 4;
	pButtonBackgroundImageBorder[2] = 4;
	pButtonBackgroundImageBorder[3] = 4;
	
	pButtonPressedBackgroundImage.TakeOverWith(display, "guitheme/button_pressed.tga");
	pButtonPressedBackgroundImageBorder[0] = 4;
	pButtonPressedBackgroundImageBorder[1] = 4;
	pButtonPressedBackgroundImageBorder[2] = 4;
	pButtonPressedBackgroundImageBorder[3] = 4;
	
	pCheckBoxImageUnchecked.TakeOverWith(display, "guitheme/checkbox_unchecked.tga");
	pCheckBoxImageChecked.TakeOverWith(display, "guitheme/checkbox_checked.tga");
	
	pOptionBoxImageDeselected.TakeOverWith(display, "guitheme/optionbox_deselected.tga");
	pOptionBoxImageSelected.TakeOverWith(display, "guitheme/optionbox_selected.tga");
}

dealGuiTheme::dealGuiTheme(const dealGuiTheme &copy) :
pWidgetBackgroundColor(copy.pWidgetBackgroundColor),
pWidgetTextColor(copy.pWidgetTextColor),
pWidgetTextFont(NULL),
pWidgetTextFontSize(copy.pWidgetTextFontSize),

pButtonBackgroundColor(copy.pButtonBackgroundColor),
pButtonTextColor(copy.pButtonTextColor),
pButtonBackgroundImage(NULL),
pButtonBackgroundImageScale(copy.pButtonBackgroundImageScale),
pButtonBackgroundImageColorize(copy.pButtonBackgroundImageColorize),

pButtonPressedBackgroundColor(copy.pButtonPressedBackgroundColor),
pButtonPressedTextColor(copy.pButtonPressedTextColor),
pButtonPressedBackgroundImage(NULL),
pButtonPressedBackgroundImageScale(copy.pButtonPressedBackgroundImageScale),
pButtonPressedBackgroundImageColorize(copy.pButtonPressedBackgroundImageColorize),

pCheckBoxBackgroundColor(copy.pCheckBoxBackgroundColor),
pCheckBoxTextColor(copy.pCheckBoxTextColor),
pCheckBoxImageUnchecked(NULL),
pCheckBoxImageChecked(NULL),
pCheckBoxImageScale(copy.pCheckBoxImageScale),
pCheckBoxImageUncheckedColorize(copy.pCheckBoxImageUncheckedColorize),
pCheckBoxImageCheckedColorize(copy.pCheckBoxImageCheckedColorize),

pOptionBoxBackgroundColor(copy.pOptionBoxBackgroundColor),
pOptionBoxTextColor(copy.pOptionBoxTextColor),
pOptionBoxImageDeselected(NULL),
pOptionBoxImageSelected(NULL),
pOptionBoxImageScale(copy.pOptionBoxImageScale),
pOptionBoxImageDeselectedColorize(copy.pOptionBoxImageDeselectedColorize),
pOptionBoxImageSelectedColorize(copy.pOptionBoxImageSelectedColorize)
{
	SetWidgetTextFont(copy.pWidgetTextFont);
	
	SetButtonBackgroundImage(copy.pButtonBackgroundImage);
	pButtonBackgroundImageBorder[0] = copy.pButtonBackgroundImageBorder[0];
	pButtonBackgroundImageBorder[1] = copy.pButtonBackgroundImageBorder[1];
	pButtonBackgroundImageBorder[2] = copy.pButtonBackgroundImageBorder[2];
	pButtonBackgroundImageBorder[3] = copy.pButtonBackgroundImageBorder[3];
	
	SetButtonPressedBackgroundImage(copy.pButtonPressedBackgroundImage);
	pButtonPressedBackgroundImageBorder[0] = copy.pButtonPressedBackgroundImageBorder[0];
	pButtonPressedBackgroundImageBorder[1] = copy.pButtonPressedBackgroundImageBorder[1];
	pButtonPressedBackgroundImageBorder[2] = copy.pButtonPressedBackgroundImageBorder[2];
	pButtonPressedBackgroundImageBorder[3] = copy.pButtonPressedBackgroundImageBorder[3];
	
	SetCheckBoxImageUnchecked(copy.pCheckBoxImageUnchecked);
	SetCheckBoxImageChecked(copy.pCheckBoxImageChecked);
	
	SetOptionBoxImageDeselected(copy.pOptionBoxImageDeselected);
	SetOptionBoxImageSelected(copy.pOptionBoxImageSelected);
}

dealGuiTheme::~dealGuiTheme(){
	
	
	
}



// Management
///////////////

void dealGuiTheme::SetWidgetBackgroundColor(const decColor &color){
	pWidgetBackgroundColor = color;
}

void dealGuiTheme::SetWidgetTextColor(const decColor &color){
	pWidgetTextColor = color;
}

void dealGuiTheme::SetWidgetTextFont(dealFont *font){
	if(font == pWidgetTextFont){
		return;
	}
	
	
	pWidgetTextFont = font;
}

void dealGuiTheme::SetWidgetTextFontSize(int size){
	pWidgetTextFontSize = decMath::max(size, 1);
}



void dealGuiTheme::SetButtonBackgroundColor(const decColor &color){
	pButtonBackgroundColor = color;
}

void dealGuiTheme::SetButtonTextColor(const decColor &color){
	pButtonTextColor = color;
}

void dealGuiTheme::SetButtonBackgroundImage(dealImage *image){
	if(image == pButtonBackgroundImage){
		return;
	}
	
	
	pButtonBackgroundImage = image;
}

int dealGuiTheme::GetButtonBackgroundImageBorder(int border) const{
	if(border < 0 || border > 3){
		DETHROW(deeInvalidParam);
	}
	
	return pButtonBackgroundImageBorder[border];
}

void dealGuiTheme::SetButtonBackgroundImageBorder(int border, int size){
	if(border < 0 || border > 3){
		DETHROW(deeInvalidParam);
	}
	
	pButtonBackgroundImageBorder[border] = decMath::max(size, 0);
}

void dealGuiTheme::SetButtonBackgroundImageScale(float scale){
	pButtonBackgroundImageScale = decMath::max(scale, 1.0f);
}

void dealGuiTheme::SetButtonBackgroundImageColorize(const decColor &color){
	pButtonBackgroundImageColorize = color;
}



void dealGuiTheme::SetButtonPressedBackgroundColor(const decColor &color){
	pButtonPressedBackgroundColor = color;
}

void dealGuiTheme::SetButtonPressedTextColor(const decColor &color){
	pButtonPressedTextColor = color;
}

void dealGuiTheme::SetButtonPressedBackgroundImage(dealImage *image){
	if(image == pButtonPressedBackgroundImage){
		return;
	}
	
	
	pButtonPressedBackgroundImage = image;
}

int dealGuiTheme::GetButtonPressedBackgroundImageBorder(int border) const{
	if(border < 0 || border > 3){
		DETHROW(deeInvalidParam);
	}
	
	return pButtonPressedBackgroundImageBorder[border];
}

void dealGuiTheme::SetButtonPressedBackgroundImageBorder(int border, int size){
	if(border < 0 || border > 3){
		DETHROW(deeInvalidParam);
	}
	
	pButtonPressedBackgroundImageBorder[border] = decMath::max(size, 0);
}

void dealGuiTheme::SetButtonPressedBackgroundImageScale(float scale){
	pButtonPressedBackgroundImageScale = decMath::max(scale, 1.0f);
}

void dealGuiTheme::SetButtonPressedBackgroundImageColorize(const decColor &color){
	pButtonPressedBackgroundImageColorize = color;
}



void dealGuiTheme::SetCheckBoxBackgroundColor(const decColor &color){
	pCheckBoxBackgroundColor = color;
}

void dealGuiTheme::SetCheckBoxTextColor(const decColor &color){
	pCheckBoxTextColor = color;
}

void dealGuiTheme::SetCheckBoxImageUnchecked(dealImage *image){
	if(image == pCheckBoxImageUnchecked){
		return;
	}
	
	
	pCheckBoxImageUnchecked = image;
}

void dealGuiTheme::SetCheckBoxImageChecked(dealImage *image){
	if(image == pCheckBoxImageChecked){
		return;
	}
	
	
	pCheckBoxImageChecked = image;
}

void dealGuiTheme::SetCheckBoxImageScale(float scale){
	pCheckBoxImageScale = decMath::max(scale, 1.0f);
}

void dealGuiTheme::SetCheckBoxImageUncheckedColorize(const decColor &color){
	pCheckBoxImageUncheckedColorize = color;
}

void dealGuiTheme::SetCheckBoxImageCheckedColorize(const decColor &color){
	pCheckBoxImageCheckedColorize = color;
}



void dealGuiTheme::SetOptionBoxBackgroundColor(const decColor &color){
	pOptionBoxBackgroundColor = color;
}

void dealGuiTheme::SetOptionBoxTextColor(const decColor &color){
	pOptionBoxTextColor = color;
}

void dealGuiTheme::SetOptionBoxImageDeselected(dealImage *image){
	if(image == pOptionBoxImageDeselected){
		return;
	}
	
	
	pOptionBoxImageDeselected = image;
}

void dealGuiTheme::SetOptionBoxImageSelected(dealImage *image){
	if(image == pOptionBoxImageSelected){
		return;
	}
	
	
	pOptionBoxImageSelected = image;
}

void dealGuiTheme::SetOptionBoxImageScale(float scale){
	pOptionBoxImageScale = decMath::max(scale, 1.0f);
}

void dealGuiTheme::SetOptionBoxImageDeselectedColorize(const decColor &color){
	pOptionBoxImageDeselectedColorize = color;
}

void dealGuiTheme::SetOptionBoxImageSelectedColorize(const decColor &color){
	pOptionBoxImageSelectedColorize = color;
}
