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

#include "deVFSContainer.h"
#include "deVFSContainerReference.h"



// Class deVFSContainerReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deVFSContainerReference::deVFSContainerReference(){
}

deVFSContainerReference::deVFSContainerReference( deVFSContainer *object ) :
deObjectReference( object ){
}

deVFSContainerReference::deVFSContainerReference( const deVFSContainerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVFSContainerReference::TakeOver( deVFSContainer *object ){
	deObjectReference::TakeOver( object );
}

bool deVFSContainerReference::operator!() const{
	return deObjectReference::operator!();
}

deVFSContainerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVFSContainerReference::operator deVFSContainer *() const{
	return ( deVFSContainer* )operator deObject *();
}

deVFSContainerReference::operator deVFSContainer &() const{
	return ( deVFSContainer& )operator deObject &();
}

deVFSContainer *deVFSContainerReference::operator->() const{
	return ( deVFSContainer* )deObjectReference::operator->();
}

deVFSContainerReference &deVFSContainerReference::operator=( deVFSContainer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVFSContainerReference &deVFSContainerReference::operator=( const deVFSContainerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVFSContainerReference::operator==( deVFSContainer *object ) const{
	return deObjectReference::operator==( object );
}

bool deVFSContainerReference::operator!=( deVFSContainer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVFSContainerReference::operator==( const deVFSContainerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVFSContainerReference::operator!=( const deVFSContainerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
