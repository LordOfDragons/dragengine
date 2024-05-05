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

#include "deVirtualFileSystem.h"
#include "deVirtualFileSystemReference.h"



// Class deVirtualFileSystemReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deVirtualFileSystemReference::deVirtualFileSystemReference(){
}

deVirtualFileSystemReference::deVirtualFileSystemReference( deVirtualFileSystem *object ) :
deObjectReference( object ){
}

deVirtualFileSystemReference::deVirtualFileSystemReference( const deVirtualFileSystemReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVirtualFileSystemReference::TakeOver( deVirtualFileSystem *object ){
	deObjectReference::TakeOver( object );
}

bool deVirtualFileSystemReference::operator!() const{
	return deObjectReference::operator!();
}

deVirtualFileSystemReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVirtualFileSystemReference::operator deVirtualFileSystem *() const{
	return ( deVirtualFileSystem* )operator deObject *();
}

deVirtualFileSystemReference::operator deVirtualFileSystem &() const{
	return ( deVirtualFileSystem& )operator deObject &();
}

deVirtualFileSystem *deVirtualFileSystemReference::operator->() const{
	return ( deVirtualFileSystem* )deObjectReference::operator->();
}

deVirtualFileSystemReference &deVirtualFileSystemReference::operator=( deVirtualFileSystem *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVirtualFileSystemReference &deVirtualFileSystemReference::operator=( const deVirtualFileSystemReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVirtualFileSystemReference::operator==( deVirtualFileSystem *object ) const{
	return deObjectReference::operator==( object );
}

bool deVirtualFileSystemReference::operator!=( deVirtualFileSystem *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVirtualFileSystemReference::operator==( const deVirtualFileSystemReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVirtualFileSystemReference::operator!=( const deVirtualFileSystemReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
