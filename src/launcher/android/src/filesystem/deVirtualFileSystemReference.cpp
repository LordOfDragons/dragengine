/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
