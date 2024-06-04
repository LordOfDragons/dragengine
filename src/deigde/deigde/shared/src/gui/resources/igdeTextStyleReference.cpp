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

#include "igdeTextStyle.h"
#include "igdeTextStyleReference.h"



// Class igdeTextStyleReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextStyleReference::igdeTextStyleReference(){
}

igdeTextStyleReference::igdeTextStyleReference( igdeTextStyle *textStyle ) :
deObjectReference( textStyle ){
}

igdeTextStyleReference::igdeTextStyleReference( const igdeTextStyleReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextStyleReference::TakeOver( igdeTextStyle *textStyle ){
	deObjectReference::TakeOver( textStyle );
}

bool igdeTextStyleReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextStyleReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextStyleReference::operator igdeTextStyle *() const{
	return ( igdeTextStyle* )operator deObject *();
}

igdeTextStyleReference::operator igdeTextStyle &() const{
	return ( igdeTextStyle& )operator deObject &();
}

igdeTextStyle *igdeTextStyleReference::operator->() const{
	return ( igdeTextStyle* )deObjectReference::operator->();
}

igdeTextStyleReference &igdeTextStyleReference::operator=( igdeTextStyle *textStyle ){
	deObjectReference::operator=( textStyle );
	return *this;
}

igdeTextStyleReference &igdeTextStyleReference::operator=( const igdeTextStyleReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextStyleReference::operator==( igdeTextStyle *textStyle ) const{
	return deObjectReference::operator==( textStyle );
}

bool igdeTextStyleReference::operator!=( igdeTextStyle *textStyle ) const{
	return deObjectReference::operator!=( textStyle );
}

bool igdeTextStyleReference::operator==( const igdeTextStyleReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextStyleReference::operator!=( const igdeTextStyleReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
