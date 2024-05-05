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

#include "igdeEditTags.h"
#include "igdeEditTagsReference.h"



// Class igdeEditTagsReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditTagsReference::igdeEditTagsReference(){
}

igdeEditTagsReference::igdeEditTagsReference( igdeEditTags *widget ) :
deObjectReference( widget ){
}

igdeEditTagsReference::igdeEditTagsReference( const igdeEditTagsReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditTagsReference::TakeOver( igdeEditTags *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeEditTagsReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditTagsReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditTagsReference::operator igdeEditTags *() const{
	return ( igdeEditTags* )operator deObject *();
}

igdeEditTagsReference::operator igdeEditTags &() const{
	return ( igdeEditTags& )operator deObject &();
}

igdeEditTags *igdeEditTagsReference::operator->() const{
	return ( igdeEditTags* )deObjectReference::operator->();
}

igdeEditTagsReference &igdeEditTagsReference::operator=( igdeEditTags *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeEditTagsReference &igdeEditTagsReference::operator=( const igdeEditTagsReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditTagsReference::operator==( igdeEditTags *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeEditTagsReference::operator!=( igdeEditTags *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeEditTagsReference::operator==( const igdeEditTagsReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditTagsReference::operator!=( const igdeEditTagsReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
