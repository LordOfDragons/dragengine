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

#include "igdeEditVector2.h"
#include "igdeEditVector2Listener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidget.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditVector2::cListener
/////////////////////////////////////

igdeEditVector2::cListener::cListener(igdeEditVector2 &editVector2,
	igdeTextField *textX, igdeTextField *textY) :
pEditVector2(editVector2),
pTextX(textX),
pTextY(textY){
}

igdeEditVector2::cListener::~cListener(){
}

void igdeEditVector2::cListener::OnTextChanged(igdeTextField*){
	pEditVector2.SetVector2(decVector2(pTextX->GetFloat(), pTextY->GetFloat()));
}

void igdeEditVector2::cListener::OnTextChanging(igdeTextField*){
}



// Class igdeEditVector2
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector2::igdeEditVector2(igdeUIHelper &helper, int columns, int precision,
	const char *description) :
igdeContainerBoxAlternate(helper.GetEnvironment(), igdeContainerBox::eaX, true, 2),
pEnabled(true),
pColumns(columns),
pEditable(true),
pDescription(description),
pPrecision(precision),
pPreventUpdate(false)
{
	if(columns < 1 || precision < 0){
		DETHROW(deeInvalidParam);
	}
	pCreateContent(helper);
}

igdeEditVector2::igdeEditVector2(igdeUIHelper &helper, int columns, int precision,
	bool editable, const char *description) :
igdeContainerBoxAlternate(helper.GetEnvironment(), igdeContainerBox::eaX, true, 2),
pEnabled(true),
pColumns(columns),
pEditable(editable),
pDescription(description),
pPrecision(precision),
pPreventUpdate(false)
{
	if(columns < 1 || precision < 0){
		DETHROW(deeInvalidParam);
	}
	pCreateContent(helper);
}

igdeEditVector2::~igdeEditVector2(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditVector2::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditVector2::SetEditable(bool editable){
	if(pEditable == editable){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditVector2::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditVector2::SetVector2(const decVector2 &vector){
	if(pPreventUpdate || vector.IsEqualTo(pVector2)){
		return;
	}
	
	pVector2 = vector;
	OnVector2Changed();
	NotifyVector2Changed();
}

void igdeEditVector2::SetPrecision(int precision){
	if(precision < 0){
		DETHROW(deeInvalidParam);
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeEditVector2::Focus(){
	pTextX->Focus();
}



void igdeEditVector2::AddListener(igdeEditVector2Listener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeEditVector2::RemoveListener(igdeEditVector2Listener *listener){
	pListeners.Remove(listener);
}

void igdeEditVector2::NotifyVector2Changed(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeEditVector2Listener*)listeners.GetAt(i))->OnVector2Changed(this);
	}
}



void igdeEditVector2::OnVector2Changed(){
	pPreventUpdate = true;
	try{
// 		if( fabsf( pVector2.x - pTextX->GetFloat() ) > FLOAT_SAFE_EPSILON ){
			pTextX->SetFloat(pVector2.x);
// 		}
// 		if( fabsf( pVector2.y - pTextY->GetFloat() ) > FLOAT_SAFE_EPSILON ){
			pTextY->SetFloat(pVector2.y);
// 		}
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditVector2::OnEnabledChanged(){
	pTextX->SetEnabled(pEnabled);
	pTextY->SetEnabled(pEnabled);
}

void igdeEditVector2::OnEditableChanged(){
	pTextX->SetEditable(pEditable);
	pTextY->SetEditable(pEditable);
}

void igdeEditVector2::OnDescriptionChanged(){
	pTextX->SetDescription(pDescription);
	pTextY->SetDescription(pDescription);
}

void igdeEditVector2::OnPrecisionChanged(){
	pTextX->SetPrecision(pPrecision);
	pTextY->SetPrecision(pPrecision);
}



void igdeEditVector2::pCreateContent(igdeUIHelper &helper){
	// create widgets
	helper.Label(*this, "(");
	helper.EditFloat(*this, pDescription, pColumns, pPrecision, pTextX, nullptr);
	helper.Label(*this, ",");
	helper.EditFloat(*this, pDescription, pColumns, pPrecision, pTextY, nullptr);
	helper.Label(*this, ")");
	
	// set values
	pTextX->SetFloat(pVector2.x);
	pTextY->SetFloat(pVector2.y);
	
	// add listener
	cListener::Ref listener(cListener::Ref::New(*this, pTextX, pTextY));
	pTextX->AddListener(listener);
	pTextY->AddListener(listener);
}
