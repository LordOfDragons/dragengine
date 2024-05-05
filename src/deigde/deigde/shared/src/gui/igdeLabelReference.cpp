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

#include "igdeLabel.h"
#include "igdeLabelReference.h"



// Class igdeLabelReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLabelReference::igdeLabelReference(){
}

igdeLabelReference::igdeLabelReference( igdeLabel *label ) :
deObjectReference( label ){
}

igdeLabelReference::igdeLabelReference( const igdeLabelReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeLabelReference::TakeOver( igdeLabel *label ){
	deObjectReference::TakeOver( label );
}

bool igdeLabelReference::operator!() const{
	return deObjectReference::operator!();
}

igdeLabelReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeLabelReference::operator igdeLabel *() const{
	return ( igdeLabel* )operator deObject *();
}

igdeLabelReference::operator igdeLabel &() const{
	return ( igdeLabel& )operator deObject &();
}

igdeLabel *igdeLabelReference::operator->() const{
	return ( igdeLabel* )deObjectReference::operator->();
}

igdeLabelReference &igdeLabelReference::operator=( igdeLabel *label ){
	deObjectReference::operator=( label );
	return *this;
}

igdeLabelReference &igdeLabelReference::operator=( const igdeLabelReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeLabelReference::operator==( igdeLabel *label ) const{
	return deObjectReference::operator==( label );
}

bool igdeLabelReference::operator!=( igdeLabel *label ) const{
	return deObjectReference::operator!=( label );
}

bool igdeLabelReference::operator==( const igdeLabelReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeLabelReference::operator!=( const igdeLabelReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
