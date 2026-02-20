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

#include "igdeEditDVector.h"
#include "igdeEditDVectorListener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidget.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditDVector::cListener
/////////////////////////////////////

igdeEditDVector::cListener::cListener(igdeEditDVector &editDVector,
	igdeTextField *textX, igdeTextField *textY, igdeTextField *textZ) :
pEditDVector(editDVector),
pTextX(textX),
pTextY(textY),
pTextZ(textZ){
}

igdeEditDVector::cListener::~cListener(){
}

void igdeEditDVector::cListener::OnTextChanged(igdeTextField*){
	pEditDVector.SetDVector(decDVector(pTextX->GetDouble(), pTextY->GetDouble(), pTextZ->GetDouble()));
}

void igdeEditDVector::cListener::OnTextChanging(igdeTextField*){
}



// Class igdeEditDVector
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVector::igdeEditDVector(igdeUIHelper &helper, int columns, int precision,
	const char *description) :
igdeContainerBoxAlternate(helper.GetEnvironment(), igdeContainerBox::eaX, true, 2),
pEnabled(true),
pEditable(true),
pDescription(description),
pColumns(columns),
pPrecision(precision),
pPreventUpdate(false)
{
	if(columns < 1 || precision < 0){
		DETHROW(deeInvalidParam);
	}
	pCreateContent(helper);
}

igdeEditDVector::igdeEditDVector(igdeUIHelper &helper, int columns, int precision,
	bool editable, const char *description) :
igdeContainerBoxAlternate(helper.GetEnvironment(), igdeContainerBox::eaX, true, 2),
pEnabled(true),
pEditable(editable),
pDescription(description),
pColumns(columns),
pPrecision(precision),
pPreventUpdate(false)
{
	if(columns < 1 || precision < 0){
		DETHROW(deeInvalidParam);
	}
	pCreateContent(helper);
}

igdeEditDVector::~igdeEditDVector(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditDVector::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditDVector::SetEditable(bool editable){
	if(pEditable == editable){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditDVector::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditDVector::SetDVector(const decDVector &vector){
	if(pPreventUpdate || vector.IsEqualTo(pDVector)){
		return;
	}
	
	pDVector = vector;
	OnDVectorChanged();
	NotifyDVectorChanged();
}

void igdeEditDVector::SetPrecision(int precision){
	if(precision < 0){
		DETHROW(deeInvalidParam);
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeEditDVector::Focus(){
	pTextX->Focus();
}



void igdeEditDVector::AddListener(igdeEditDVectorListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeEditDVector::RemoveListener(igdeEditDVectorListener *listener){
	pListeners.Remove(listener);
}

void igdeEditDVector::NotifyDVectorChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeEditDVectorListener &l){
		l.OnDVectorChanged(this);
	});
}



void igdeEditDVector::OnDVectorChanged(){
	pPreventUpdate = true;
	try{
// 		if( fabsf( pDVector.x - pTextX->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextX->SetDouble(pDVector.x);
// 		}
// 		if( fabsf( pDVector.y - pTextY->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextY->SetDouble(pDVector.y);
// 		}
// 		if( fabsf( pDVector.z - pTextY->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextZ->SetDouble(pDVector.z);
// 		}
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditDVector::OnEnabledChanged(){
	pTextX->SetEnabled(pEnabled);
	pTextY->SetEnabled(pEnabled);
	pTextZ->SetEnabled(pEnabled);
}

void igdeEditDVector::OnEditableChanged(){
	pTextX->SetEditable(pEditable);
	pTextY->SetEditable(pEditable);
	pTextZ->SetEditable(pEditable);
}

void igdeEditDVector::OnDescriptionChanged(){
	pTextX->SetDescription(pDescription);
	pTextY->SetDescription(pDescription);
	pTextZ->SetDescription(pDescription);
}

void igdeEditDVector::OnPrecisionChanged(){
	pTextX->SetPrecision(pPrecision);
	pTextY->SetPrecision(pPrecision);
	pTextZ->SetPrecision(pPrecision);
}



void igdeEditDVector::pCreateContent(igdeUIHelper &helper){
	// create widgets
	helper.Label(*this, "(");
	helper.EditFloat(*this, pDescription, pColumns, pPrecision, pTextX, {});
	helper.Label(*this, ",");
	helper.EditFloat(*this, pDescription, pColumns, pPrecision, pTextY, {});
	helper.Label(*this, ",");
	helper.EditFloat(*this, pDescription, pColumns, pPrecision, pTextZ, {});
	helper.Label(*this, ")");
	
	// set values
	pTextX->SetDouble(pDVector.x);
	pTextY->SetDouble(pDVector.y);
	pTextZ->SetDouble(pDVector.z);
	
	// add listener
	cListener::Ref listener(cListener::Ref::New(*this, pTextX, pTextY, pTextZ));
	pTextX->AddListener(listener);
	pTextY->AddListener(listener);
	pTextZ->AddListener(listener);
}
