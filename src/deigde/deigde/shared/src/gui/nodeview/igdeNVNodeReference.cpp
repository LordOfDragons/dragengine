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

#include "igdeNVNode.h"
#include "igdeNVNodeReference.h"



// Class igdeNVNodeReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNodeReference::igdeNVNodeReference(){
}

igdeNVNodeReference::igdeNVNodeReference( igdeNVNode *widget ) :
deObjectReference( widget ){
}

igdeNVNodeReference::igdeNVNodeReference( const igdeNVNodeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVNodeReference::TakeOver( igdeNVNode *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeNVNodeReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVNodeReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVNodeReference::operator igdeNVNode *() const{
	return ( igdeNVNode* )operator deObject *();
}

igdeNVNodeReference::operator igdeNVNode &() const{
	return ( igdeNVNode& )operator deObject &();
}

igdeNVNode *igdeNVNodeReference::operator->() const{
	return ( igdeNVNode* )deObjectReference::operator->();
}

igdeNVNodeReference &igdeNVNodeReference::operator=( igdeNVNode *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeNVNodeReference &igdeNVNodeReference::operator=( const igdeNVNodeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVNodeReference::operator==( igdeNVNode *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeNVNodeReference::operator!=( igdeNVNode *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeNVNodeReference::operator==( const igdeNVNodeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVNodeReference::operator!=( const igdeNVNodeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
