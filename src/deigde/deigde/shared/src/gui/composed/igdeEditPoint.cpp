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

#include "igdeEditPoint.h"
#include "igdeEditPointListener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidget.h"
#include "../igdeContainer.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListener.h"
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
	cListener::Ref listener(cListener::Ref::NewWith(*this, pTextX, pTextY));
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
}
