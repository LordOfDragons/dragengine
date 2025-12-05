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

#include "igdeTextField.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeTextFieldListener.h"
#include "native/toolkit.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeTextField
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTextField::igdeTextField( igdeEnvironment &environment, int columns, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pEditable( true ),
pDescription( description ),
pPrecision( 3 ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextField::igdeTextField( igdeEnvironment &environment, int columns, bool editable,
	const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pEditable( editable ),
pDescription( description ),
pPrecision( 3 ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextField::~igdeTextField(){
	igdeTextField::DestroyNativeWidget();
}



// Management
///////////////

void igdeTextField::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTextField::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeTextField::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeTextField::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	pPrecision = precision;
}

void igdeTextField::SetInvalidValue( bool invalidValue ){
	if( pInvalidValue == invalidValue ){
		return;
	}
	
	pInvalidValue = invalidValue;
	OnInvalidValueChanged();
}



void igdeTextField::SetText( const char *text, bool changing, bool forceNotify ){
	if( pText == text && ! forceNotify ){
		return;
	}
	
	pText = text;
	OnTextChanged();
	
	if( changing ){
		NotifyTextChanging();
		
	}else{
		NotifyTextChanged();
	}
}

void igdeTextField::ClearText(){
	if( pText.IsEmpty() ){
		return;
	}
	
	pText.Empty();
	OnTextChanged();
}

int igdeTextField::GetInteger() const{
	return pText.ToInt();
}

void igdeTextField::SetInteger( int value, bool changing ){
	decString text;
	text.Format( "%d", value );
	SetText( text, changing );
}

float igdeTextField::GetFloat() const{
	return pText.ToFloat();
}

void igdeTextField::SetFloat( float value, bool changing ){
	decString format, text;
	format.Format( "%%.%df", pPrecision );
	text.Format( format, value );
	SetText( text, changing );
}

double igdeTextField::GetDouble() const{
	return pText.ToDouble();
}

void igdeTextField::SetDouble( double value, bool changing ){
	decString format, text;
	format.Format( "%%.%df", pPrecision );
	text.Format( format, value );
	SetText( text, changing );
}



int igdeTextField::GetCursorPosition() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	return ( ( igdeNativeTextField* )GetNativeWidget() )->GetCursorPosition();
}

void igdeTextField::SetCursorPosition( int position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( position < 0 || position >= pText.GetLength() ){
		DETHROW( deeInvalidParam );
	}
	
	( ( igdeNativeTextField* )GetNativeWidget() )->SetCursorPosition( position );
}

void igdeTextField::Focus(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeTextField* )GetNativeWidget() )->Focus();
}



void igdeTextField::AddListener( igdeTextFieldListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeTextField::RemoveListener( igdeTextFieldListener *listener ){
	pListeners.Remove( listener );
}

void igdeTextField::NotifyTextChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnTextChanged( this );
	}
}

void igdeTextField::NotifyTextChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnTextChanging( this );
	}
}

void igdeTextField::NotifyEnterKey(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnEnterKey( this );
	}
}



void igdeTextField::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeTextField * const native = igdeNativeTextField::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeTextField::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeTextField* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeTextField::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeTextField* )GetNativeWidget() )->UpdateText();
	}
}

void igdeTextField::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeTextField* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeTextField::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeTextField* )GetNativeWidget() )->UpdateEditable();
	}
}

void igdeTextField::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeTextField* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeTextField::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeTextField* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}
