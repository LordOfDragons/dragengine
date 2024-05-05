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

#include "igdeToggleTags.h"
#include "igdeToggleTagsReference.h"



// Class igdeToggleTagsReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleTagsReference::igdeToggleTagsReference(){
}

igdeToggleTagsReference::igdeToggleTagsReference( igdeToggleTags *widget ) :
deObjectReference( widget ){
}

igdeToggleTagsReference::igdeToggleTagsReference( const igdeToggleTagsReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeToggleTagsReference::TakeOver( igdeToggleTags *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeToggleTagsReference::operator!() const{
	return deObjectReference::operator!();
}

igdeToggleTagsReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeToggleTagsReference::operator igdeToggleTags *() const{
	return ( igdeToggleTags* )operator deObject *();
}

igdeToggleTagsReference::operator igdeToggleTags &() const{
	return ( igdeToggleTags& )operator deObject &();
}

igdeToggleTags *igdeToggleTagsReference::operator->() const{
	return ( igdeToggleTags* )deObjectReference::operator->();
}

igdeToggleTagsReference &igdeToggleTagsReference::operator=( igdeToggleTags *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeToggleTagsReference &igdeToggleTagsReference::operator=( const igdeToggleTagsReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeToggleTagsReference::operator==( igdeToggleTags *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeToggleTagsReference::operator!=( igdeToggleTags *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeToggleTagsReference::operator==( const igdeToggleTagsReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeToggleTagsReference::operator!=( const igdeToggleTagsReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
