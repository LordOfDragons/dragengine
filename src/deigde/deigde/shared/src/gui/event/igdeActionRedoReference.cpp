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

#include "igdeActionRedo.h"
#include "igdeActionRedoReference.h"



// Class igdeActionRedoReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionRedoReference::igdeActionRedoReference(){
}

igdeActionRedoReference::igdeActionRedoReference( igdeActionRedo *action ) :
deObjectReference( action ){
}

igdeActionRedoReference::igdeActionRedoReference( const igdeActionRedoReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionRedoReference::TakeOver( igdeActionRedo *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionRedoReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionRedoReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionRedoReference::operator igdeActionRedo *() const{
	return ( igdeActionRedo* )operator deObject *();
}

igdeActionRedoReference::operator igdeActionRedo &() const{
	return ( igdeActionRedo& )operator deObject &();
}

igdeActionRedo *igdeActionRedoReference::operator->() const{
	return ( igdeActionRedo* )deObjectReference::operator->();
}

igdeActionRedoReference &igdeActionRedoReference::operator=( igdeActionRedo *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionRedoReference &igdeActionRedoReference::operator=( const igdeActionRedoReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionRedoReference::operator==( igdeActionRedo *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionRedoReference::operator!=( igdeActionRedo *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionRedoReference::operator==( const igdeActionRedoReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionRedoReference::operator!=( const igdeActionRedoReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
