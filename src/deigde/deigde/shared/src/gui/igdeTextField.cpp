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

#include "igdeTextField.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeTextFieldListener.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
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
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	igdeNativeFoxTextField * const foxWidget = new igdeNativeFoxTextField(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeTextField::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxTextField* )GetNativeWidget();
	DropNativeWidget();
}

void igdeTextField::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextField* )GetNativeWidget() )->UpdateText();
	}
}

void igdeTextField::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextField* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeTextField::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextField* )GetNativeWidget() )->UpdateEditable();
	}
}

void igdeTextField::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextField* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeTextField::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTextField* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}
