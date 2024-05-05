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

#include "igdeActionSelectFile.h"
#include "igdeActionSelectFileReference.h"



// Class igdeActionSelectFileReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectFileReference::igdeActionSelectFileReference(){
}

igdeActionSelectFileReference::igdeActionSelectFileReference( igdeActionSelectFile *action ) :
deObjectReference( action ){
}

igdeActionSelectFileReference::igdeActionSelectFileReference( const igdeActionSelectFileReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionSelectFileReference::TakeOver( igdeActionSelectFile *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionSelectFileReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionSelectFileReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionSelectFileReference::operator igdeActionSelectFile *() const{
	return ( igdeActionSelectFile* )operator deObject *();
}

igdeActionSelectFileReference::operator igdeActionSelectFile &() const{
	return ( igdeActionSelectFile& )operator deObject &();
}

igdeActionSelectFile *igdeActionSelectFileReference::operator->() const{
	return ( igdeActionSelectFile* )deObjectReference::operator->();
}

igdeActionSelectFileReference &igdeActionSelectFileReference::operator=( igdeActionSelectFile *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionSelectFileReference &igdeActionSelectFileReference::operator=( const igdeActionSelectFileReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionSelectFileReference::operator==( igdeActionSelectFile *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionSelectFileReference::operator!=( igdeActionSelectFile *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionSelectFileReference::operator==( const igdeActionSelectFileReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionSelectFileReference::operator!=( const igdeActionSelectFileReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
