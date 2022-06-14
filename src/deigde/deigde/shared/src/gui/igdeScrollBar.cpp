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

#include "igdeScrollBar.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeScrollBarListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeScrollBar
////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBar::igdeScrollBar( igdeEnvironment &environment, eOrientation orientation ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( 0 ),
pUpper( 100 ),
pPageSize( 10 ),
pValue( 0 ),
pEnabled( true ){
}

igdeScrollBar::igdeScrollBar( igdeEnvironment &environment, eOrientation orientation,
	int lower, int upper, int pageSize, int value ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( lower ),
pUpper( upper ),
pPageSize( pageSize ),
pValue( value ),
pEnabled( true )
{
	if( upper < lower ){
		DETHROW_INFO( deeInvalidParam, "upper < lower" );
	}
	if( pageSize < 1 ){
		DETHROW_INFO( deeInvalidParam, "pageSize < 1" );
	}
}

igdeScrollBar::~igdeScrollBar(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeScrollBar::SetLower( int lower ){
	if( lower > pUpper ){
		DETHROW_INFO( deeInvalidParam, "lower > upper" );
	}
	if( pLower == lower ){
		return;
	}
	
	pLower = lower;
	OnRangeChanged();
	
	SetValue( pValue );
}

void igdeScrollBar::SetUpper( int upper ){
	if( upper < pLower ){
		DETHROW_INFO( deeInvalidParam, "upper < lower" );
	}
	if( pUpper == upper ){
		return;
	}
	
	pUpper = upper;
	OnRangeChanged();
	
	SetValue( pValue );
}

void igdeScrollBar::SetRange( int lower, int upper ){
	if( upper < lower ){
		DETHROW_INFO( deeInvalidParam, "upper < lower" );
	}
	if( pLower == lower && pUpper == upper ){
		return;
	}
	
	pLower = lower;
	pUpper = upper;
	OnRangeChanged();
	
	SetValue( pValue );
}

void igdeScrollBar::SetPageSize( int pageSize ){
	if( pageSize < 1 ){
		DETHROW_INFO( deeInvalidParam, "pageSize < 1" );
	}
	if( pPageSize == pageSize ){
		return;
	}
	
	pPageSize = pageSize;
	OnRangeChanged();
	
	SetValue( pValue );
}

void igdeScrollBar::SetValue( int value ){
	value = decMath::max( decMath::min( value, pUpper - pPageSize ), pLower );
	if( pValue == value ){
		return;
	}
	
	pValue = value;
	OnValueChanged();
	NotifyValueChanged();
}

void igdeScrollBar::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



void igdeScrollBar::AddListener( igdeScrollBarListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeScrollBar::RemoveListener( igdeScrollBarListener *listener ){
	pListeners.Remove( listener );
}

void igdeScrollBar::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeScrollBarListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}



void igdeScrollBar::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeScrollBar * const native = igdeNativeScrollBar::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeScrollBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeScrollBar* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeScrollBar::OnRangeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeScrollBar* )GetNativeWidget() )->UpdateRange();
	}
}

void igdeScrollBar::OnValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeScrollBar* )GetNativeWidget() )->UpdateValue();
	}
}

void igdeScrollBar::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeScrollBar* )GetNativeWidget() )->UpdateEnabled();
	}
}
