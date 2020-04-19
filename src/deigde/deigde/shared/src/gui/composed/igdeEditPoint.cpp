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

#include "igdeEditPoint.h"
#include "igdeEditPointListener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidgetReference.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditPoint::cListener
///////////////////////////////////

igdeEditPoint::cListener::cListener( igdeEditPoint &editPoint,
	igdeTextField *textX, igdeTextField *textY ) :
pEditPoint( editPoint ),
pTextX( textX ),
pTextY( textY ){
}

igdeEditPoint::cListener::~cListener(){
}

void igdeEditPoint::cListener::OnTextChanged( igdeTextField* ){
	pEditPoint.SetPoint( decPoint( pTextX->GetInteger(), pTextY->GetInteger() ) );
}

void igdeEditPoint::cListener::OnTextChanging( igdeTextField* ){
}



// Class igdeEditPoint
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint::igdeEditPoint( igdeUIHelper &helper, int columns, const char *description ) :
igdeContainerBoxAlternate( helper.GetEnvironment(), igdeContainerBox::eaX, true, 2 ),
pEnabled( true ),
pColumns( columns ),
pEditable( true ),
pDescription( description ),
pPreventUpdate( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
	pCreateContent( helper );
}

igdeEditPoint::igdeEditPoint( igdeUIHelper &helper, int columns, bool editable, const char *description ) :
igdeContainerBoxAlternate( helper.GetEnvironment(), igdeContainerBox::eaX, true, 2 ),
pEnabled( true ),
pColumns( columns ),
pEditable( editable ),
pDescription( description ),
pPreventUpdate( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
	pCreateContent( helper );
}

igdeEditPoint::~igdeEditPoint(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditPoint::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditPoint::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditPoint::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditPoint::SetPoint( const decPoint &point ){
	if( pPreventUpdate || point == pPoint ){
		return;
	}
	
	pPoint = point;
	OnPointChanged();
	NotifyPointChanged();
}

void igdeEditPoint::Focus(){
	pTextX->Focus();
}



void igdeEditPoint::AddListener( igdeEditPointListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditPoint::RemoveListener( igdeEditPointListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditPoint::NotifyPointChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditPointListener* )listeners.GetAt( i ) )->OnPointChanged( this );
	}
}



void igdeEditPoint::OnPointChanged(){
	pPreventUpdate = true;
	try{
// 		if( pPoint.x != pTextX->GetInteger() ){
			pTextX->SetInteger( pPoint.x );
// 		}
// 		if( pPoint.y != pTextY->GetInteger() ){
			pTextY->SetInteger( pPoint.y );
// 		}
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditPoint::OnEnabledChanged(){
	pTextX->SetEnabled( pEnabled );
	pTextY->SetEnabled( pEnabled );
}

void igdeEditPoint::OnEditableChanged(){
	pTextX->SetEditable( pEditable );
	pTextY->SetEditable( pEditable );
}

void igdeEditPoint::OnDescriptionChanged(){
	pTextX->SetDescription( pDescription );
	pTextY->SetDescription( pDescription );
}



void igdeEditPoint::pCreateContent( igdeUIHelper &helper ){
	// create widgets
	helper.Label( *this, "(" );
	helper.EditInteger( *this, pDescription, pColumns, pTextX, NULL );
	helper.Label( *this, "," );
	helper.EditInteger( *this, pDescription, pColumns, pTextY, NULL );
	helper.Label( *this, ")" );
	
	// set value
	pTextX->SetInteger( pPoint.x );
	pTextY->SetInteger( pPoint.y );
	
	// add listener
	igdeTextFieldListenerReference listener;
	listener.TakeOver( new cListener( *this, pTextX, pTextY ) );
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
}
