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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshReference.h"



// Class deOcclusionMeshReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMeshReference::deOcclusionMeshReference(){
}

deOcclusionMeshReference::deOcclusionMeshReference( deOcclusionMesh *object ) :
deObjectReference( object ){
}

deOcclusionMeshReference::deOcclusionMeshReference( const deOcclusionMeshReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deOcclusionMeshReference::TakeOver( deOcclusionMesh *object ){
	deObjectReference::TakeOver( object );
}

bool deOcclusionMeshReference::operator!() const{
	return deObjectReference::operator!();
}

deOcclusionMeshReference::operator bool() const{
	return deObjectReference::operator bool();
}

deOcclusionMeshReference::operator deOcclusionMesh *() const{
	return ( deOcclusionMesh* )operator deObject *();
}

deOcclusionMeshReference::operator deOcclusionMesh &() const{
	return ( deOcclusionMesh& )operator deObject &();
}

deOcclusionMesh *deOcclusionMeshReference::operator->() const{
	return ( deOcclusionMesh* )deObjectReference::operator->();
}

deOcclusionMeshReference &deOcclusionMeshReference::operator=( deOcclusionMesh *object ){
	deObjectReference::operator=( object );
	return *this;
}

deOcclusionMeshReference &deOcclusionMeshReference::operator=( const deOcclusionMeshReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deOcclusionMeshReference::operator==( deOcclusionMesh *object ) const{
	return deObjectReference::operator==( object );
}

bool deOcclusionMeshReference::operator!=( deOcclusionMesh *object ) const{
	return deObjectReference::operator!=( object );
}

bool deOcclusionMeshReference::operator==( const deOcclusionMeshReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deOcclusionMeshReference::operator!=( const deOcclusionMeshReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
