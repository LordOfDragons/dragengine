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

#include "igdeEditSliderText.h"
#include "igdeEditSliderTextReference.h"



// Class igdeEditSliderTextReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditSliderTextReference::igdeEditSliderTextReference(){
}

igdeEditSliderTextReference::igdeEditSliderTextReference( igdeEditSliderText *editSliderText ) :
deObjectReference( editSliderText ){
}

igdeEditSliderTextReference::igdeEditSliderTextReference( const igdeEditSliderTextReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditSliderTextReference::TakeOver( igdeEditSliderText *editSliderText ){
	deObjectReference::TakeOver( editSliderText );
}

bool igdeEditSliderTextReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditSliderTextReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditSliderTextReference::operator igdeEditSliderText *() const{
	return ( igdeEditSliderText* )operator deObject *();
}

igdeEditSliderTextReference::operator igdeEditSliderText &() const{
	return ( igdeEditSliderText& )operator deObject &();
}

igdeEditSliderText *igdeEditSliderTextReference::operator->() const{
	return ( igdeEditSliderText* )deObjectReference::operator->();
}

igdeEditSliderTextReference &igdeEditSliderTextReference::operator=( igdeEditSliderText *editSliderText ){
	deObjectReference::operator=( editSliderText );
	return *this;
}

igdeEditSliderTextReference &igdeEditSliderTextReference::operator=( const igdeEditSliderTextReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditSliderTextReference::operator==( igdeEditSliderText *editSliderText ) const{
	return deObjectReference::operator==( editSliderText );
}

bool igdeEditSliderTextReference::operator!=( igdeEditSliderText *editSliderText ) const{
	return deObjectReference::operator!=( editSliderText );
}

bool igdeEditSliderTextReference::operator==( const igdeEditSliderTextReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditSliderTextReference::operator!=( const igdeEditSliderTextReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
