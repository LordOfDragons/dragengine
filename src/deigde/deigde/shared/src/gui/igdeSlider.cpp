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
