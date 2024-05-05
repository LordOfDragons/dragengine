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

#include "igdeNVSlot.h"
#include "igdeNVSlotReference.h"



// Class igdeNVSlotReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVSlotReference::igdeNVSlotReference(){
}

igdeNVSlotReference::igdeNVSlotReference( igdeNVSlot *widget ) :
deObjectReference( widget ){
}

igdeNVSlotReference::igdeNVSlotReference( const igdeNVSlotReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVSlotReference::TakeOver( igdeNVSlot *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeNVSlotReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVSlotReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVSlotReference::operator igdeNVSlot *() const{
	return ( igdeNVSlot* )operator deObject *();
}

igdeNVSlotReference::operator igdeNVSlot &() const{
	return ( igdeNVSlot& )operator deObject &();
}

igdeNVSlot *igdeNVSlotReference::operator->() const{
	return ( igdeNVSlot* )deObjectReference::operator->();
}

igdeNVSlotReference &igdeNVSlotReference::operator=( igdeNVSlot *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeNVSlotReference &igdeNVSlotReference::operator=( const igdeNVSlotReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVSlotReference::operator==( igdeNVSlot *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeNVSlotReference::operator!=( igdeNVSlot *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeNVSlotReference::operator==( const igdeNVSlotReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVSlotReference::operator!=( const igdeNVSlotReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
