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

#include "igdeTimer.h"
#include "igdeTimerReference.h"



// Class igdeTimerReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTimerReference::igdeTimerReference(){
}

igdeTimerReference::igdeTimerReference( igdeTimer *timer ) :
deObjectReference( timer ){
}

igdeTimerReference::igdeTimerReference( const igdeTimerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTimerReference::TakeOver( igdeTimer *timer ){
	deObjectReference::TakeOver( timer );
}

bool igdeTimerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTimerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTimerReference::operator igdeTimer *() const{
	return ( igdeTimer* )operator deObject *();
}

igdeTimerReference::operator igdeTimer &() const{
	return ( igdeTimer& )operator deObject &();
}

igdeTimer *igdeTimerReference::operator->() const{
	return ( igdeTimer* )deObjectReference::operator->();
}

igdeTimerReference &igdeTimerReference::operator=( igdeTimer *timer ){
	deObjectReference::operator=( timer );
	return *this;
}

igdeTimerReference &igdeTimerReference::operator=( const igdeTimerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTimerReference::operator==( igdeTimer *timer ) const{
	return deObjectReference::operator==( timer );
}

bool igdeTimerReference::operator!=( igdeTimer *timer ) const{
	return deObjectReference::operator!=( timer );
}

bool igdeTimerReference::operator==( const igdeTimerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTimerReference::operator!=( const igdeTimerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
