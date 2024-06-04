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

#include "igdeAction.h"
#include "igdeActionReference.h"



// Class igdeActionReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionReference::igdeActionReference(){
}

igdeActionReference::igdeActionReference( igdeAction *action ) :
deObjectReference( action ){
}

igdeActionReference::igdeActionReference( const igdeActionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionReference::TakeOver( igdeAction *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionReference::operator igdeAction *() const{
	return ( igdeAction* )operator deObject *();
}

igdeActionReference::operator igdeAction &() const{
	return ( igdeAction& )operator deObject &();
}

igdeAction *igdeActionReference::operator->() const{
	return ( igdeAction* )deObjectReference::operator->();
}

igdeActionReference &igdeActionReference::operator=( igdeAction *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionReference &igdeActionReference::operator=( const igdeActionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionReference::operator==( igdeAction *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionReference::operator!=( igdeAction *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionReference::operator==( const igdeActionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionReference::operator!=( const igdeActionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
