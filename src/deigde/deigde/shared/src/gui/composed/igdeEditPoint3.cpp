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

#include "igdeEditPoint3.h"
#include "igdeEditPoint3Listener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidgetReference.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditPoint3::cListener
////////////////////////////////////

igdeEditPoint3::cListener::cListener( igdeEditPoint3 &editPoint3,
	igdeTextField *textX, igdeTextField *textY, igdeTextField *textZ ) :
pEditPoint3( editPoint3 ),
pTextX( textX ),
pTextY( textY ),
pTextZ( textZ ){
}

igdeEditPoint3::cListener::~cListener(){
}

void igdeEditPoint3::cListener::OnTextChanged( igdeTextField* ){
	pEditPoint3.SetPoint3( decPoint3( pTextX->GetInteger(), pTextY->GetInteger(), pTextZ->GetInteger() ) );
}

void igdeEditPoint3::cListener::OnTextChanging( igdeTextField* ){
}



// Class igdeEditPoint3
////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint3::igdeEditPoint3( igdeUIHelper &helper, int columns, const char *description ) :
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

igdeEditPoint3::igdeEditPoint3( igdeUIHelper &helper, int columns, bool editable, const char *description ) :
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

igdeEditPoint3::~igdeEditPoint3(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditPoint3::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeEditPoint3::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeEditPoint3::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeEditPoint3::SetPoint3( const decPoint3 &point ){
	if( pPreventUpdate || point == pPoint3 ){
		return;
	}
	
	pPoint3 = point;
	OnPoint3Changed();
	NotifyPoint3Changed();
}

void igdeEditPoint3::Focus(){
	pTextX->Focus();
}



void igdeEditPoint3::AddListener( igdeEditPoint3Listener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditPoint3::RemoveListener( igdeEditPoint3Listener *listener ){
	pListeners.Remove( listener );
}

void igdeEditPoint3::NotifyPoint3Changed(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditPoint3Listener* )listeners.GetAt( i ) )->OnPoint3Changed( this );
	}
}



void igdeEditPoint3::OnPoint3Changed(){
	pPreventUpdate = true;
	try{
// 		if( pPoint3.x != pTextX->GetInteger() ){
			pTextX->SetInteger( pPoint3.x );
// 		}
// 		if( pPoint3.y != pTextY->GetInteger() ){
			pTextY->SetInteger( pPoint3.y );
// 		}
// 		if( pPoint3.z != pTextZ->GetInteger() ){
			pTextZ->SetInteger( pPoint3.z );
// 		}
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void igdeEditPoint3::OnEnabledChanged(){
	pTextX->SetEnabled( pEnabled );
	pTextY->SetEnabled( pEnabled );
	pTextZ->SetEnabled( pEnabled );
}

void igdeEditPoint3::OnEditableChanged(){
	pTextX->SetEditable( pEditable );
	pTextY->SetEditable( pEditable );
	pTextZ->SetEditable( pEditable );
}

void igdeEditPoint3::OnDescriptionChanged(){
	pTextX->SetDescription( pDescription );
	pTextY->SetDescription( pDescription );
	pTextZ->SetDescription( pDescription );
}



void igdeEditPoint3::pCreateContent( igdeUIHelper &helper ){
	// create widgets
	helper.Label( *this, "(" );
	helper.EditInteger( *this, pDescription, pColumns, pTextX, NULL );
	helper.Label( *this, "," );
	helper.EditInteger( *this, pDescription, pColumns, pTextY, NULL );
	helper.Label( *this, "," );
	helper.EditInteger( *this, pDescription, pColumns, pTextZ, NULL );
	helper.Label( *this, ")" );
	
	// set value
	pTextX->SetInteger( pPoint3.x );
	pTextY->SetInteger( pPoint3.y );
	pTextZ->SetInteger( pPoint3.z );
	
	// add listener
	igdeTextFieldListenerReference listener;
	listener.TakeOver( new cListener( *this, pTextX, pTextY, pTextZ ) );
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
	pTextZ->AddListener( listener );
}
