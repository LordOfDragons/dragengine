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

#include "igdeGDClass.h"
#include "igdeGDClassReference.h"



// Class igdeGDClassReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClassReference::igdeGDClassReference(){
}

igdeGDClassReference::igdeGDClassReference( igdeGDClass *gdclass ) :
deObjectReference( gdclass ){
}

igdeGDClassReference::igdeGDClassReference( const igdeGDClassReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDClassReference::TakeOver( igdeGDClass *gdclass ){
	deObjectReference::TakeOver( gdclass );
}

bool igdeGDClassReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDClassReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDClassReference::operator igdeGDClass *() const{
	return ( igdeGDClass* )operator deObject *();
}

igdeGDClassReference::operator igdeGDClass &() const{
	return ( igdeGDClass& )operator deObject &();
}

igdeGDClass *igdeGDClassReference::operator->() const{
	return ( igdeGDClass* )deObjectReference::operator->();
}

igdeGDClassReference &igdeGDClassReference::operator=( igdeGDClass *gdclass ){
	deObjectReference::operator=( gdclass );
	return *this;
}

igdeGDClassReference &igdeGDClassReference::operator=( const igdeGDClassReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDClassReference::operator==( igdeGDClass *gdclass ) const{
	return deObjectReference::operator==( gdclass );
}

bool igdeGDClassReference::operator!=( igdeGDClass *gdclass ) const{
	return deObjectReference::operator!=( gdclass );
}

bool igdeGDClassReference::operator==( const igdeGDClassReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDClassReference::operator!=( const igdeGDClassReference &reference ) const{
	return deObjectReference::operator!=( reference );
}

