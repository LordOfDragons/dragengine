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

#include "igdeGDSky.h"
#include "igdeGDSkyReference.h"



// Class igdeGDSkyReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyReference::igdeGDSkyReference(){
}

igdeGDSkyReference::igdeGDSkyReference( igdeGDSky *gdsky ) :
deObjectReference( gdsky ){
}

igdeGDSkyReference::igdeGDSkyReference( const igdeGDSkyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeGDSkyReference::TakeOver( igdeGDSky *gdsky ){
	deObjectReference::TakeOver( gdsky );
}

bool igdeGDSkyReference::operator!() const{
	return deObjectReference::operator!();
}

igdeGDSkyReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeGDSkyReference::operator igdeGDSky *() const{
	return ( igdeGDSky* )operator deObject *();
}

igdeGDSkyReference::operator igdeGDSky &() const{
	return ( igdeGDSky& )operator deObject &();
}

igdeGDSky *igdeGDSkyReference::operator->() const{
	return ( igdeGDSky* )deObjectReference::operator->();
}

igdeGDSkyReference &igdeGDSkyReference::operator=( igdeGDSky *gdsky ){
	deObjectReference::operator=( gdsky );
	return *this;
}

igdeGDSkyReference &igdeGDSkyReference::operator=( const igdeGDSkyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeGDSkyReference::operator==( igdeGDSky *gdsky ) const{
	return deObjectReference::operator==( gdsky );
}

bool igdeGDSkyReference::operator!=( igdeGDSky *gdsky ) const{
	return deObjectReference::operator!=( gdsky );
}

bool igdeGDSkyReference::operator==( const igdeGDSkyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeGDSkyReference::operator!=( const igdeGDSkyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}

