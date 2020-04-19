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

#include "deLogger.h"
#include "deLoggerReference.h"



// Class deLoggerReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerReference::deLoggerReference(){
}

deLoggerReference::deLoggerReference( deLogger *object ) :
deObjectReference( object ){
}

deLoggerReference::deLoggerReference( const deLoggerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLoggerReference::TakeOver( deLogger *object ){
	deObjectReference::TakeOver( object );
}

bool deLoggerReference::operator!() const{
	return deObjectReference::operator!();
}

deLoggerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLoggerReference::operator deLogger *() const{
	return ( deLogger* )operator deObject *();
}

deLoggerReference::operator deLogger &() const{
	return ( deLogger& )operator deObject &();
}

deLogger *deLoggerReference::operator->() const{
	return ( deLogger* )deObjectReference::operator->();
}

deLoggerReference &deLoggerReference::operator=( deLogger *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLoggerReference &deLoggerReference::operator=( const deLoggerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLoggerReference::operator==( deLogger *object ) const{
	return deObjectReference::operator==( object );
}

bool deLoggerReference::operator!=( deLogger *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLoggerReference::operator==( const deLoggerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLoggerReference::operator!=( const deLoggerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
