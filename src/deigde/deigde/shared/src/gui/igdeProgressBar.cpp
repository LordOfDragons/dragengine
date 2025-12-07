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
#include <stdlib.h>
#include <string.h>

#include "igdeProgressBar.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeProgressBar
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeProgressBar::igdeProgressBar(igdeEnvironment &environment, int lower, int upper,
	eOrientation orientation, const char *description) :
igdeWidget(environment),
pOrientation(orientation),
pLower(lower),
pUpper(decMath::max(upper, lower)),
pValue(lower),
pDescription(description){
}

igdeProgressBar::~igdeProgressBar(){
	igdeProgressBar::DestroyNativeWidget();
}



// Management
///////////////

void igdeProgressBar::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeProgressBar::SetValue(int value){
	if(value == pValue){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeProgressBar::SetRange(int lower, int upper){
	if(lower == pLower && upper == pUpper){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max(upper, lower);
	OnRangeChanged();
}



void igdeProgressBar::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeProgressBar * const native = igdeNativeProgressBar::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeProgressBar::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeProgressBar*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeProgressBar::OnRangeChanged(){
	if(GetNativeWidget()){
		((igdeNativeProgressBar*)GetNativeWidget())->UpdateRange();
	}
}

void igdeProgressBar::OnValueChanged(){
	if(GetNativeWidget()){
		((igdeNativeProgressBar*)GetNativeWidget())->UpdateValue();
	}
}

void igdeProgressBar::OnDescriptionChanged(){
	if(GetNativeWidget()){
		((igdeNativeProgressBar*)GetNativeWidget())->UpdateDescription();
	}
}
