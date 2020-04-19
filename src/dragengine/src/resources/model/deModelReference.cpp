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

#include "deModel.h"
#include "deModelReference.h"



// Class deModelReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deModelReference::deModelReference(){
}

deModelReference::deModelReference( deModel *object ) :
deObjectReference( object ){
}

deModelReference::deModelReference( const deModelReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deModelReference::TakeOver( deModel *object ){
	deObjectReference::TakeOver( object );
}

bool deModelReference::operator!() const{
	return deObjectReference::operator!();
}

deModelReference::operator bool() const{
	return deObjectReference::operator bool();
}

deModelReference::operator deModel *() const{
	return ( deModel* )operator deObject *();
}

deModelReference::operator deModel &() const{
	return ( deModel& )operator deObject &();
}

deModel *deModelReference::operator->() const{
	return ( deModel* )deObjectReference::operator->();
}

deModelReference &deModelReference::operator=( deModel *object ){
	deObjectReference::operator=( object );
	return *this;
}

deModelReference &deModelReference::operator=( const deModelReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deModelReference::operator==( deModel *object ) const{
	return deObjectReference::operator==( object );
}

bool deModelReference::operator!=( deModel *object ) const{
	return deObjectReference::operator!=( object );
}

bool deModelReference::operator==( const deModelReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deModelReference::operator!=( const deModelReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
