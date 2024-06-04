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

#include "igdeSlider.h"
#include "igdeSliderReference.h"



// Class igdeSliderReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSliderReference::igdeSliderReference(){
}

igdeSliderReference::igdeSliderReference( igdeSlider *slider ) :
deObjectReference( slider ){
}

igdeSliderReference::igdeSliderReference( const igdeSliderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSliderReference::TakeOver( igdeSlider *slider ){
	deObjectReference::TakeOver( slider );
}

bool igdeSliderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSliderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSliderReference::operator igdeSlider *() const{
	return ( igdeSlider* )operator deObject *();
}

igdeSliderReference::operator igdeSlider &() const{
	return ( igdeSlider& )operator deObject &();
}

igdeSlider *igdeSliderReference::operator->() const{
	return ( igdeSlider* )deObjectReference::operator->();
}

igdeSliderReference &igdeSliderReference::operator=( igdeSlider *slider ){
	deObjectReference::operator=( slider );
	return *this;
}

igdeSliderReference &igdeSliderReference::operator=( const igdeSliderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSliderReference::operator==( igdeSlider *slider ) const{
	return deObjectReference::operator==( slider );
}

bool igdeSliderReference::operator!=( igdeSlider *slider ) const{
	return deObjectReference::operator!=( slider );
}

bool igdeSliderReference::operator==( const igdeSliderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSliderReference::operator!=( const igdeSliderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
