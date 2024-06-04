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

#include "igdeSpacer.h"
#include "igdeSpacerReference.h"



// Class igdeSpacerReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpacerReference::igdeSpacerReference(){
}

igdeSpacerReference::igdeSpacerReference( igdeSpacer *spacer ) :
deObjectReference( spacer ){
}

igdeSpacerReference::igdeSpacerReference( const igdeSpacerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpacerReference::TakeOver( igdeSpacer *spacer ){
	deObjectReference::TakeOver( spacer );
}

bool igdeSpacerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpacerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpacerReference::operator igdeSpacer *() const{
	return ( igdeSpacer* )operator deObject *();
}

igdeSpacerReference::operator igdeSpacer &() const{
	return ( igdeSpacer& )operator deObject &();
}

igdeSpacer *igdeSpacerReference::operator->() const{
	return ( igdeSpacer* )deObjectReference::operator->();
}

igdeSpacerReference &igdeSpacerReference::operator=( igdeSpacer *spacer ){
	deObjectReference::operator=( spacer );
	return *this;
}

igdeSpacerReference &igdeSpacerReference::operator=( const igdeSpacerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpacerReference::operator==( igdeSpacer *spacer ) const{
	return deObjectReference::operator==( spacer );
}

bool igdeSpacerReference::operator!=( igdeSpacer *spacer ) const{
	return deObjectReference::operator!=( spacer );
}

bool igdeSpacerReference::operator==( const igdeSpacerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpacerReference::operator!=( const igdeSpacerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
