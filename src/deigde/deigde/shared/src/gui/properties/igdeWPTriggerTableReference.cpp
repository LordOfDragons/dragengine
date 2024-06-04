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

#include "igdeWPTriggerTable.h"
#include "igdeWPTriggerTableReference.h"



// Class igdeWPTriggerTableReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPTriggerTableReference::igdeWPTriggerTableReference(){
}

igdeWPTriggerTableReference::igdeWPTriggerTableReference( igdeWPTriggerTable *widget ) :
deObjectReference( widget ){
}

igdeWPTriggerTableReference::igdeWPTriggerTableReference( const igdeWPTriggerTableReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPTriggerTableReference::TakeOver( igdeWPTriggerTable *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPTriggerTableReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPTriggerTableReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPTriggerTableReference::operator igdeWPTriggerTable *() const{
	return ( igdeWPTriggerTable* )operator deObject *();
}

igdeWPTriggerTableReference::operator igdeWPTriggerTable &() const{
	return ( igdeWPTriggerTable& )operator deObject &();
}

igdeWPTriggerTable *igdeWPTriggerTableReference::operator->() const{
	return ( igdeWPTriggerTable* )deObjectReference::operator->();
}

igdeWPTriggerTableReference &igdeWPTriggerTableReference::operator=( igdeWPTriggerTable *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPTriggerTableReference &igdeWPTriggerTableReference::operator=( const igdeWPTriggerTableReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPTriggerTableReference::operator==( igdeWPTriggerTable *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPTriggerTableReference::operator!=( igdeWPTriggerTable *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPTriggerTableReference::operator==( const igdeWPTriggerTableReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPTriggerTableReference::operator!=( const igdeWPTriggerTableReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
