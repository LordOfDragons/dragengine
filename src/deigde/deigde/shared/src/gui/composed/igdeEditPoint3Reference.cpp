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

#include "igdeEditPoint3.h"
#include "igdeEditPoint3Reference.h"



// Class igdeEditPoint3Reference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint3Reference::igdeEditPoint3Reference(){
}

igdeEditPoint3Reference::igdeEditPoint3Reference( igdeEditPoint3 *editPoint3 ) :
deObjectReference( editPoint3 ){
}

igdeEditPoint3Reference::igdeEditPoint3Reference( const igdeEditPoint3Reference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPoint3Reference::TakeOver( igdeEditPoint3 *editPoint3 ){
	deObjectReference::TakeOver( editPoint3 );
}

bool igdeEditPoint3Reference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPoint3Reference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPoint3Reference::operator igdeEditPoint3 *() const{
	return ( igdeEditPoint3* )operator deObject *();
}

igdeEditPoint3Reference::operator igdeEditPoint3 &() const{
	return ( igdeEditPoint3& )operator deObject &();
}

igdeEditPoint3 *igdeEditPoint3Reference::operator->() const{
	return ( igdeEditPoint3* )deObjectReference::operator->();
}

igdeEditPoint3Reference &igdeEditPoint3Reference::operator=( igdeEditPoint3 *editPoint3 ){
	deObjectReference::operator=( editPoint3 );
	return *this;
}

igdeEditPoint3Reference &igdeEditPoint3Reference::operator=( const igdeEditPoint3Reference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPoint3Reference::operator==( igdeEditPoint3 *editPoint3 ) const{
	return deObjectReference::operator==( editPoint3 );
}

bool igdeEditPoint3Reference::operator!=( igdeEditPoint3 *editPoint3 ) const{
	return deObjectReference::operator!=( editPoint3 );
}

bool igdeEditPoint3Reference::operator==( const igdeEditPoint3Reference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPoint3Reference::operator!=( const igdeEditPoint3Reference &reference ) const{
	return deObjectReference::operator!=( reference );
}
