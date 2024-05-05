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

#include "igdeActionExternOpen.h"
#include "igdeActionExternOpenReference.h"



// Class igdeActionExternOpenReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionExternOpenReference::igdeActionExternOpenReference(){
}

igdeActionExternOpenReference::igdeActionExternOpenReference( igdeActionExternOpen *action ) :
deObjectReference( action ){
}

igdeActionExternOpenReference::igdeActionExternOpenReference( const igdeActionExternOpenReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionExternOpenReference::TakeOver( igdeActionExternOpen *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionExternOpenReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionExternOpenReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionExternOpenReference::operator igdeActionExternOpen *() const{
	return ( igdeActionExternOpen* )operator deObject *();
}

igdeActionExternOpenReference::operator igdeActionExternOpen &() const{
	return ( igdeActionExternOpen& )operator deObject &();
}

igdeActionExternOpen *igdeActionExternOpenReference::operator->() const{
	return ( igdeActionExternOpen* )deObjectReference::operator->();
}

igdeActionExternOpenReference &igdeActionExternOpenReference::operator=( igdeActionExternOpen *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionExternOpenReference &igdeActionExternOpenReference::operator=( const igdeActionExternOpenReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionExternOpenReference::operator==( igdeActionExternOpen *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionExternOpenReference::operator!=( igdeActionExternOpen *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionExternOpenReference::operator==( const igdeActionExternOpenReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionExternOpenReference::operator!=( const igdeActionExternOpenReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
