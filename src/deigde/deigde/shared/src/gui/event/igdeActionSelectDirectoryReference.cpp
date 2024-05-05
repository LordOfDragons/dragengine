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

#include "igdeActionSelectDirectory.h"
#include "igdeActionSelectDirectoryReference.h"



// Class igdeActionSelectDirectoryReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference(){
}

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference( igdeActionSelectDirectory *action ) :
deObjectReference( action ){
}

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference( const igdeActionSelectDirectoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionSelectDirectoryReference::TakeOver( igdeActionSelectDirectory *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionSelectDirectoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionSelectDirectoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionSelectDirectoryReference::operator igdeActionSelectDirectory *() const{
	return ( igdeActionSelectDirectory* )operator deObject *();
}

igdeActionSelectDirectoryReference::operator igdeActionSelectDirectory &() const{
	return ( igdeActionSelectDirectory& )operator deObject &();
}

igdeActionSelectDirectory *igdeActionSelectDirectoryReference::operator->() const{
	return ( igdeActionSelectDirectory* )deObjectReference::operator->();
}

igdeActionSelectDirectoryReference &igdeActionSelectDirectoryReference::operator=( igdeActionSelectDirectory *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionSelectDirectoryReference &igdeActionSelectDirectoryReference::operator=( const igdeActionSelectDirectoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionSelectDirectoryReference::operator==( igdeActionSelectDirectory *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionSelectDirectoryReference::operator!=( igdeActionSelectDirectory *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionSelectDirectoryReference::operator==( const igdeActionSelectDirectoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionSelectDirectoryReference::operator!=( const igdeActionSelectDirectoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
