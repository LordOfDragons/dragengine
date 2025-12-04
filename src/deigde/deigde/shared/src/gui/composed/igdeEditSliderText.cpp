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

#include "igdeEditSliderText.h"
#include "igdeEditSliderTextListener.h"
#include "../igdeSlider.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../event/igdeSliderListener::Ref.h"
#include "../event/igdeTextFieldListener::Ref.h"
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
