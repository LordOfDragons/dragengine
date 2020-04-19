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
