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

#include "igdeEditVector2.h"
#include "igdeEditVector2Reference.h"



// Class igdeEditVector2Reference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector2Reference::igdeEditVector2Reference(){
}

igdeEditVector2Reference::igdeEditVector2Reference( igdeEditVector2 *editVector2 ) :
deObjectReference( editVector2 ){
}

igdeEditVector2Reference::igdeEditVector2Reference( const igdeEditVector2Reference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVector2Reference::TakeOver( igdeEditVector2 *editVector2 ){
	deObjectReference::TakeOver( editVector2 );
}

bool igdeEditVector2Reference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVector2Reference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVector2Reference::operator igdeEditVector2 *() const{
	return ( igdeEditVector2* )operator deObject *();
}

igdeEditVector2Reference::operator igdeEditVector2 &() const{
	return ( igdeEditVector2& )operator deObject &();
}

igdeEditVector2 *igdeEditVector2Reference::operator->() const{
	return ( igdeEditVector2* )deObjectReference::operator->();
}

igdeEditVector2Reference &igdeEditVector2Reference::operator=( igdeEditVector2 *editVector2 ){
	deObjectReference::operator=( editVector2 );
	return *this;
}

igdeEditVector2Reference &igdeEditVector2Reference::operator=( const igdeEditVector2Reference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVector2Reference::operator==( igdeEditVector2 *editVector2 ) const{
	return deObjectReference::operator==( editVector2 );
}

bool igdeEditVector2Reference::operator!=( igdeEditVector2 *editVector2 ) const{
	return deObjectReference::operator!=( editVector2 );
}

bool igdeEditVector2Reference::operator==( const igdeEditVector2Reference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVector2Reference::operator!=( const igdeEditVector2Reference &reference ) const{
	return deObjectReference::operator!=( reference );
}
