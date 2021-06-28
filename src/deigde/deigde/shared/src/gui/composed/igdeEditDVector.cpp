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

#include "igdeEditDVector.h"
#include "igdeEditDVectorListener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidgetReference.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditDVector::cListener
/////////////////////////////////////

igdeEditDVector::cListener::cListener( igdeEditDVector &editDVector,
	igdeTextField *textX, igdeTextField *textY, igdeTextField *textZ ) :
pEditDVector( editDVector ),
pTextX( textX ),
pTextY( textY ),
pTextZ( textZ ){
}

igdeEditDVector::cListener::~cListener(){
}

void igdeEditDVector::cListener::OnTextChanged( igdeTextField* ){
	pEditDVector.SetDVector( decDVector( pTextX->GetDouble(), pTextY->GetDouble(), pTextZ->GetDouble() ) );
}

void igdeEditDVector::cListener::OnTextChanging( igdeTextField* ){
}



// Class igdeEditDVector
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVector::igdeEditDVector( igdeUIHelper &helper, int columns, int precision,
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

igdeEditDVector::igdeEditDVector( igdeUIHelper &helper, int columns, int precision,
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

igdeEditDVector::~igdeEditDVector(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditDVector::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditDVector::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditDVector::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditDVector::SetDVector( const decDVector &vector ){
	if( pPreventUpdate || vector.IsEqualTo( pDVector ) ){
		return;
	}
	
	pDVector = vector;
	OnDVectorChanged();
	NotifyDVectorChanged();
}

void igdeEditDVector::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeEditDVector::Focus(){
	pTextX->Focus();
}



void igdeEditDVector::AddListener( igdeEditDVectorListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditDVector::RemoveListener( igdeEditDVectorListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditDVector::NotifyDVectorChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditDVectorListener* )listeners.GetAt( i ) )->OnDVectorChanged( this );
	}
}



void igdeEditDVector::OnDVectorChanged(){
	pPreventUpdate = true;
	try{
// 		if( fabsf( pDVector.x - pTextX->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextX->SetDouble( pDVector.x );
// 		}
// 		if( fabsf( pDVector.y - pTextY->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextY->SetDouble( pDVector.y );
// 		}
// 		if( fabsf( pDVector.z - pTextY->GetDouble() ) > DOUBLE_SAFE_EPSILON ){
		pTextZ->SetDouble( pDVector.z );
// 		}
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditDVector::OnEnabledChanged(){
	pTextX->SetEnabled( pEnabled );
	pTextY->SetEnabled( pEnabled );
	pTextZ->SetEnabled( pEnabled );
}

void igdeEditDVector::OnEditableChanged(){
	pTextX->SetEditable( pEditable );
	pTextY->SetEditable( pEditable );
	pTextZ->SetEditable( pEditable );
}

void igdeEditDVector::OnDescriptionChanged(){
	pTextX->SetDescription( pDescription );
	pTextY->SetDescription( pDescription );
	pTextZ->SetDescription( pDescription );
}

void igdeEditDVector::OnPrecisionChanged(){
	pTextX->SetPrecision( pPrecision );
	pTextY->SetPrecision( pPrecision );
	pTextZ->SetPrecision( pPrecision );
}



void igdeEditDVector::pCreateContent( igdeUIHelper &helper ){
	// create widgets
	helper.Label( *this, "(" );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextX, NULL );
	helper.Label( *this, "," );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextY, NULL );
	helper.Label( *this, "," );
	helper.EditFloat( *this, pDescription, pColumns, pPrecision, pTextZ, NULL );
	helper.Label( *this, ")" );
	
	// set values
	pTextX->SetDouble( pDVector.x );
	pTextY->SetDouble( pDVector.y );
	pTextZ->SetDouble( pDVector.z );
	
	// add listener
	igdeTextFieldListenerReference listener;
	listener.TakeOver( new cListener( *this, pTextX, pTextY, pTextZ ) );
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
	pTextZ->AddListener( listener );
}
