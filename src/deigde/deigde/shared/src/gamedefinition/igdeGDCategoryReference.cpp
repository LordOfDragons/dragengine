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

#include "igdeGDCategory.h"
#include "igdeGDCategoryReference.h"



// Class igdeGDCategoryReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCategoryReference::igdeGDCategoryReference(){
}

igdeGDCategoryReference::igdeGDCategoryReference( igdeGDCategory *object ) :
deObjectReference( object ){
}

igdeGDCategoryReference::igdeGDCategoryReference( const igdeGDCategoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDCategoryReference::TakeOver( igdeGDCategory *object ){
	deObjectReference::TakeOver( object );
}

bool igdeGDCategoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDCategoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDCategoryReference::operator igdeGDCategory *() const{
	return ( igdeGDCategory* )operator deObject *();
}

igdeGDCategoryReference::operator igdeGDCategory &() const{
	return ( igdeGDCategory& )operator deObject &();
}

igdeGDCategory *igdeGDCategoryReference::operator->() const{
	return ( igdeGDCategory* )deObjectReference::operator->();
}

igdeGDCategoryReference &igdeGDCategoryReference::operator=( igdeGDCategory *object ){
	deObjectReference::operator=( object );
	return *this;
}

igdeGDCategoryReference &igdeGDCategoryReference::operator=( const igdeGDCategoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDCategoryReference::operator==( igdeGDCategory *object ) const{
	return deObjectReference::operator==( object );
}

bool igdeGDCategoryReference::operator!=( igdeGDCategory *object ) const{
	return deObjectReference::operator!=( object );
}

bool igdeGDCategoryReference::operator==( const igdeGDCategoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDCategoryReference::operator!=( const igdeGDCategoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
