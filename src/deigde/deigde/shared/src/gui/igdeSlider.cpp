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

#include "igdeSlider.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeSliderListener.h"
#include "native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeSlider
/////////////////////

// Constructor, destructor
////////////////////////////

igdeSlider::igdeSlider( igdeEnvironment &environment, eOrientation orientation,
	const char *description ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( 0.0f ),
pUpper( 10.0f ),
pTickSpacing( 1.0f ),
pValue( 0.0f ),
pPrecision( 0 ),
pEnabled( true ),
pDescription( description ){
}

igdeSlider::igdeSlider( igdeEnvironment &environment, eOrientation orientation,
	float lower, float upper, int precision, float tickSpacing, const char *description ) :
igdeWidget( environment ),
pOrientation( orientation ),
pLower( lower ),
pUpper( decMath::max( upper, lower ) ),
pTickSpacing( decMath::max( tickSpacing, 0.0f ) ),
pValue( lower ),
pPrecision( precision ),
pEnabled( true ),
pDescription( description )
{
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeSlider::~igdeSlider(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSlider::SetRange( float lower, float upper ){
	if( fabsf( pLower - lower ) < FLOAT_SAFE_EPSILON
	&& fabsf( pUpper - upper ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max( upper, lower );
	OnRangeChanged();
}

void igdeSlider::SetTickSpacing( float spacing ){
	if( fabsf( pTickSpacing - spacing ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pTickSpacing = spacing;
	OnRangeChanged();
}

void igdeSlider::SetValue( float value ){
	if( fabsf( pValue - value ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = decMath::clamp( value, pLower, pUpper );
	OnValueChanged();
}

void igdeSlider::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( precision == pPrecision ){
		return;
	}
	
	pPrecision = precision;
	OnPrecisionChanged();
}

void igdeSlider::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeSlider::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeSlider::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeSlider* )GetNativeWidget() )->Focus();
	}
}



void igdeSlider::AddListener( igdeSliderListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeSlider::RemoveListener( igdeSliderListener *listener ){
	pListeners.Remove( listener );
}

void igdeSlider::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSliderListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}

void igdeSlider::NotifyValueChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSliderListener* )listeners.GetAt( i ) )->OnValueChanging( this );
	}
}



void igdeSlider::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeSlider * const native = igdeNativeSlider::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeSlider::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeSlider* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeSlider::OnRangeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSlider* )GetNativeWidget() )->UpdateRange();
	}
}

void igdeSlider::OnValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSlider* )GetNativeWidget() )->UpdateValue();
	}
}

void igdeSlider::OnPrecisionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeSlider &native = *( ( igdeNativeSlider* )GetNativeWidget() );
	native.UpdateScale();
	native.UpdateRange();
	native.UpdateValue();
}

void igdeSlider::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSlider* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeSlider::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSlider* )GetNativeWidget() )->UpdateDescription();
	}
}
