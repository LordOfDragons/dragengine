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

#include "igdeEditPoint3.h"
#include "igdeEditPoint3Listener.h"
#include "../igdeLabel.h"
#include "../igdeTextField.h"
#include "../igdeWidget::Ref.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeTextFieldListener::Ref.h"
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
	igdeTextFieldListener::Ref listener;
	listener.TakeOver( new cListener( *this, pTextX, pTextY, pTextZ ) );
	pTextX->AddListener( listener );
	pTextY->AddListener( listener );
	pTextZ->AddListener( listener );
}
