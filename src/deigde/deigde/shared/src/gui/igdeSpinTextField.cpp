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

#include "igdeSpinTextField.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeSpinTextFieldListener.h"
#include "native/toolkit.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeSpinTextField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextField::igdeSpinTextField(igdeEnvironment &environment,
	int lower, int upper, int columns, const char *description) :
igdeWidget(environment),
pEnabled(true),
pColumns(columns),
pLower(lower),
pUpper(decMath::max(upper, lower)),
pValue(lower),
pDescription(description)
{
	if(columns < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeSpinTextField::~igdeSpinTextField(){
	igdeSpinTextField::DestroyNativeWidget();
}



// Management
///////////////

void igdeSpinTextField::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeSpinTextField::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeSpinTextField::Focus(){
	if(GetNativeWidget()){
		((igdeNativeSpinTextField*)GetNativeWidget())->Focus();
	}
}



void igdeSpinTextField::SetValue(int value){
	value = decMath::clamp(value, pLower, pUpper);
	if(value == pValue){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeSpinTextField::SetRange(int lower, int upper){
	if(lower == pLower && upper == pUpper){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max(upper, lower);
	
	const int oldValue = pValue;
	pValue = decMath::clamp(pValue, lower, upper);
	
	OnRangeChanged();
	
	if(pValue != oldValue){
		OnValueChanged();
	}
}



void igdeSpinTextField::AddListener(igdeSpinTextFieldListener *listener){
	if(! listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeSpinTextField::RemoveListener(igdeSpinTextFieldListener *listener){
	pListeners.Remove(listener);
}

void igdeSpinTextField::NotifyValueChanged(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeSpinTextFieldListener*)listeners.GetAt(i))->OnValueChanged(this);
	}
}



void igdeSpinTextField::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeSpinTextField * const native = igdeNativeSpinTextField::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeSpinTextField::DestroyNativeWidget(){
	if(! GetNativeWidget()){
		return;
	}
	
	((igdeNativeSpinTextField*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeSpinTextField::OnRangeChanged(){
	if(GetNativeWidget()){
		((igdeNativeSpinTextField*)GetNativeWidget())->UpdateRange();
	}
}

void igdeSpinTextField::OnValueChanged(){
	if(GetNativeWidget()){
		((igdeNativeSpinTextField*)GetNativeWidget())->UpdateValue();
	}
}

void igdeSpinTextField::OnEnabledChanged(){
	if(GetNativeWidget()){
		((igdeNativeSpinTextField*)GetNativeWidget())->UpdateEnabled();
	}
}

void igdeSpinTextField::OnDescriptionChanged(){
	if(GetNativeWidget()){
		((igdeNativeSpinTextField*)GetNativeWidget())->UpdateDescription();
	}
}
