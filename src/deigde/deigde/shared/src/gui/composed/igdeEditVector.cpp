/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
