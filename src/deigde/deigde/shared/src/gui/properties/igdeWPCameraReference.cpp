/* 
 * Drag[en]gine IGDE
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

#include "igdeWPCamera.h"
#include "igdeWPCameraReference.h"



// Class igdeWPCameraReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPCameraReference::igdeWPCameraReference(){
}

igdeWPCameraReference::igdeWPCameraReference( igdeWPCamera *widget ) :
deObjectReference( widget ){
}

igdeWPCameraReference::igdeWPCameraReference( const igdeWPCameraReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeWPCameraReference::TakeOver( igdeWPCamera *widget ){
	deObjectReference::TakeOver( widget );
}

bool igdeWPCameraReference::operator!() const{
	return deObjectReference::operator!();
}

igdeWPCameraReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeWPCameraReference::operator igdeWPCamera *() const{
	return ( igdeWPCamera* )operator deObject *();
}

igdeWPCameraReference::operator igdeWPCamera &() const{
	return ( igdeWPCamera& )operator deObject &();
}

igdeWPCamera *igdeWPCameraReference::operator->() const{
	return ( igdeWPCamera* )deObjectReference::operator->();
}

igdeWPCameraReference &igdeWPCameraReference::operator=( igdeWPCamera *widget ){
	deObjectReference::operator=( widget );
	return *this;
}

igdeWPCameraReference &igdeWPCameraReference::operator=( const igdeWPCameraReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeWPCameraReference::operator==( igdeWPCamera *widget ) const{
	return deObjectReference::operator==( widget );
}

bool igdeWPCameraReference::operator!=( igdeWPCamera *widget ) const{
	return deObjectReference::operator!=( widget );
}

bool igdeWPCameraReference::operator==( const igdeWPCameraReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeWPCameraReference::operator!=( const igdeWPCameraReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
