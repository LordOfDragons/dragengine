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

#include "igdeSpinTextField.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeSpinTextFieldListener.h"
#include "native/toolkit.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeSpinTextField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextField::igdeSpinTextField( igdeEnvironment &environment,
	int lower, int upper, int columns, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pLower( lower ),
pUpper( decMath::max( upper, lower ) ),
pValue( lower ),
pDescription( description )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeSpinTextField::~igdeSpinTextField(){
	igdeSpinTextField::DestroyNativeWidget();
}



// Management
///////////////

void igdeSpinTextField::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeSpinTextField::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeSpinTextField::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeSpinTextField* )GetNativeWidget() )->Focus();
	}
}



void igdeSpinTextField::SetValue( int value ){
	value = decMath::clamp( value, pLower, pUpper );
	if( value == pValue ){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeSpinTextField::SetRange( int lower, int upper ){
	if( lower == pLower && upper == pUpper ){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max( upper, lower );
	
	const int oldValue = pValue;
	pValue = decMath::clamp( pValue, lower, upper );
	
	OnRangeChanged();
	
	if( pValue != oldValue ){
		OnValueChanged();
	}
}



void igdeSpinTextField::AddListener( igdeSpinTextFieldListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeSpinTextField::RemoveListener( igdeSpinTextFieldListener *listener ){
	pListeners.Remove( listener );
}

void igdeSpinTextField::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSpinTextFieldListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}



void igdeSpinTextField::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeSpinTextField * const native = igdeNativeSpinTextField::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeSpinTextField::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeSpinTextField* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeSpinTextField::OnRangeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSpinTextField* )GetNativeWidget() )->UpdateRange();
	}
}

void igdeSpinTextField::OnValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSpinTextField* )GetNativeWidget() )->UpdateValue();
	}
}

void igdeSpinTextField::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSpinTextField* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeSpinTextField::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSpinTextField* )GetNativeWidget() )->UpdateDescription();
	}
}
