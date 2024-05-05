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

#include "igdeEditVector.h"
#include "igdeEditVectorListener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidgetReference.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditVector::cListener
////////////////////////////////////

igdeEditVector::cListener::cListener( igdeEditVector &editVector,
	igdeTextField *textX, igdeTextField *textY, igdeTextField *textZ ) :
pEditVector( editVector ),
pTextX( textX ),
pTextY( textY ),
pTextZ( textZ ){
}

igdeEditVector::cListener::~cListener(){
}

void igdeEditVector::cListener::OnTextChanged( igdeTextField* ){
	pEditVector.SetVector( decVector( pTextX->GetFloat(), pTextY->GetFloat(), pTextZ->GetFloat() ) );
}

void igdeEditVector::cListener::OnTextChanging( igdeTextField* ){
}



// Class igdeEditVector
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector::igdeEditVector( igdeUIHelper &helper, int columns, int precision,
	const char *description ) :
igdeContainerBoxAlternate( helper.GetEnvironment(), igdeContainerBox::eaX, true, 2 ),
pEnabled( true ),
pEditable( true ),
pDescription( description ),
pColumns( columns ),
pPrecision( precision ),
pPreventUpdate( false )
{
	if( columns < 1 || precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	pCreateContent( helper );
}

igdeEditVector::igdeEditVector( igdeUIHelper &helper, int columns, int precision,
	bool editable, const char *description ) :
igdeContainerBoxAlternate( helper.GetEnvironment(), igdeContainerBox::eaX, true, 2 ),
pEnabled( true ),
pEditable( editable ),
pDescription( description ),
pColumns( columns ),
pPrecision( precision ),
pPreventUpdate( false )
{
	if( columns < 1 || precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	pCreateContent( helper );
}

igdeEditVector::~igdeEditVector(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditVector::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditVector::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditVector::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditVector::SetVector( const decVector &vector ){
	if( pPreventUpdate || vector.IsEqualTo( pVector ) ){
		return;
	}
	
	pVector = vector;
	OnVectorChanged();
	NotifyVectorChanged();
}

void igdeEditVector::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeEditVector::Focus(){
	pTextX->Focus();
}



void igdeEditVector::AddListener( igdeEditVectorListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditVector::RemoveListener( igdeEditVectorListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditVector::NotifyVectorChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditVectorListener* )listeners.GetAt( i ) )->OnVectorChanged( this );
	}
}



void igdeEditVector::OnVectorChanged(){
	pPreventUpdate = true;
	try{
// 		if( fabsf( pVector.x - pTextX->GetFloat() ) > FLOAT_SAFE_EPSILON ){
		pTextX->SetFloat( pVector.x );
// 		}
// 		if( fabsf( pVector.y - pTextY->GetFloat() ) > FLOAT_SAFE_EPSILON ){
		pTextY->SetFloat( pVector.y );
// 		}
// 		if( fabsf( pVector.z - pTextY->GetFloat() ) > FLOAT_SAFE_EPSILON ){
		pTextZ->SetFloat( pVector.z );
// 		}
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditVector::OnEnabledChanged(){
	pTextX->SetEnabled( pEnabled );
	pTextY->SetEnabled( pEnabled );
	pTextZ->SetEnabled( pEnabled );
}

void igdeEditVector::OnEditableChanged(){
	pTextX->SetEditable( pEditable );
	pTextY->SetEditable( pEditable );
	pTextZ->SetEditable( pEditable );
}

void igdeEditVector::OnDescriptionChanged(){
	pTextX->SetDescription( pDescription );
	pTextY->SetDescription( pDescription );
	pTextZ->SetDescription( pDescription );
}

void igdeEditVector::OnPrecisionChanged(){
	pTextX->SetPrecision( pPrecision );
	pTextY->SetPrecision( pPrecision );
	pTextZ->SetPrecision( pPrecision );
}



void igdeEditVector::pCreateContent( igdeUIHelper &helper ){
	// create widgets
	helper.Label( *this, "(" );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextX, NULL );
	helper.Label( *this, "," );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextY, NULL );
	helper.Label( *this, "," );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextZ, NULL );
	helper.Label( *this, ")" );
	
	// set values
	pTextX->SetFloat( pVector.x );
	pTextY->SetFloat( pVector.y );
	pTextZ->SetFloat( pVector.z );
	
	// add listener
	igdeTextFieldListenerReference listener;
	listener.TakeOver( new cListener( *this, pTextX, pTextY, pTextZ ) );
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
	pTextZ->AddListener( listener );
}
