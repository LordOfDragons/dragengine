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

#include "igdeEditSliderText.h"
#include "igdeEditSliderTextListener.h"
#include "../igdeSlider.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeSliderListenerReference.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>


// Class igdeEditSliderText::cListenerSlider
//////////////////////////////////////////////

igdeEditSliderText::cListenerSlider::cListenerSlider( igdeEditSliderText &sliderText ) :
pSliderText( sliderText ){
}

igdeEditSliderText::cListenerSlider::~cListenerSlider(){
}

void igdeEditSliderText::cListenerSlider::OnValueChanged( igdeSlider *slider ){
	pSliderText.SetValue( slider->GetValue() );
	pSliderText.NotifySliderTextValueChanged();
}

void igdeEditSliderText::cListenerSlider::OnValueChanging( igdeSlider *slider ){
	pSliderText.SetValue( slider->GetValue() );
	pSliderText.NotifySliderTextValueChanging();
}



// Class igdeEditSliderText::cListenerTextField
/////////////////////////////////////////////////

igdeEditSliderText::cListenerTextField::cListenerTextField( igdeEditSliderText &sliderText ) :
pSliderText( sliderText ){
}

igdeEditSliderText::cListenerTextField::~cListenerTextField(){
}

void igdeEditSliderText::cListenerTextField::OnTextChanged( igdeTextField *textField ){
	pSliderText.SetValue( textField->GetText().ToFloat() );
	pSliderText.NotifySliderTextValueChanged();
}

void igdeEditSliderText::cListenerTextField::OnTextChanging( igdeTextField* ){
	// could result in problems so do nothing for the time being
}



// Class igdeEditSliderText
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditSliderText::igdeEditSliderText( igdeUIHelper &helper, float lower, float upper,
	int columns, int precision, float tickSpacing, const char *description ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 2 ),
pValue( lower ),
pPreventNotify( false )
{
	pCreateContent( helper, lower, upper, columns, precision, tickSpacing, description );
}

igdeEditSliderText::~igdeEditSliderText(){
	DestroyNativeWidget();
}



// Management
///////////////

float igdeEditSliderText::GetLower() const{
	return pSlider->GetLower();
}

float igdeEditSliderText::GetUpper() const{
	return pSlider->GetUpper();
}

void igdeEditSliderText::SetRange( float lower, float upper ){
	pSlider->SetRange( lower, upper );
}

float igdeEditSliderText::GetTickSpacing() const{
	return pSlider->GetTickSpacing();
}

void igdeEditSliderText::SetTickSpacing( float spacing ){
	pSlider->SetTickSpacing( spacing );
}

void igdeEditSliderText::SetValue( float value ){
	if( pPreventNotify || fabsf( value - pValue ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = value;
	
	pPreventNotify = true;
	try{
		pSlider->SetValue( value );
		pText->SetFloat( pSlider->GetValue() );
		pPreventNotify = false;
		
	}catch( ... ){
		pPreventNotify = false;
		throw;
	}
}

int igdeEditSliderText::GetPrecision() const{
	return pSlider->GetPrecision();
}

void igdeEditSliderText::SetPrecision( int precision ){
	if( precision == pSlider->GetPrecision() ){
		return;
	}
	
	pSlider->SetPrecision( precision );
	pText->SetPrecision( precision );
}

bool igdeEditSliderText::GetEnabled() const{
	return pSlider->GetEnabled();
}

void igdeEditSliderText::SetEnabled( bool enabled ){
	pSlider->SetEnabled( enabled );
	pText->SetEnabled( enabled );
}

const decString &igdeEditSliderText::GetDescription() const{
	return pSlider->GetDescription();
}

void igdeEditSliderText::SetDescription( const char *description ){
	pSlider->SetDescription( description );
	pText->SetDescription( description );
}

void igdeEditSliderText::Focus(){
	pText->Focus();
}



void igdeEditSliderText::AddListener( igdeEditSliderTextListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditSliderText::RemoveListener( igdeEditSliderTextListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditSliderText::NotifySliderTextValueChanged(){
	if( pPreventNotify ){
		return;
	}
	
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditSliderTextListener* )listeners.GetAt( i ) )->OnSliderTextValueChanged( this );
	}
}

void igdeEditSliderText::NotifySliderTextValueChanging(){
	if( pPreventNotify ){
		return;
	}
	
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditSliderTextListener* )listeners.GetAt( i ) )->OnSliderTextValueChanging( this );
	}
}



void igdeEditSliderText::pCreateContent( igdeUIHelper &helper, float lower, float upper,
int columns, int precision, float tickSpacing, const char *description ){
	helper.Slider( *this, description, lower, upper, precision, tickSpacing,
		pSlider, new cListenerSlider( *this ) );
	helper.EditFloat( *this, description, columns, precision,
		pText, new cListenerTextField( *this ) );
	
	pText->SetFloat( lower );
}
