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

#include "deEffect.h"
#include "deEffectReference.h"



// Class deEffectReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deEffectReference::deEffectReference(){
}

deEffectReference::deEffectReference( deEffect *object ) :
deObjectReference( object ){
}

deEffectReference::deEffectReference( const deEffectReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deEffectReference::TakeOver( deEffect *object ){
	deObjectReference::TakeOver( object );
}

bool deEffectReference::operator!() const{
	return deObjectReference::operator!();
}

deEffectReference::operator bool() const{
	return deObjectReference::operator bool();
}

deEffectReference::operator deEffect *() const{
	return ( deEffect* )operator deObject *();
}

deEffectReference::operator deEffect &() const{
	return ( deEffect& )operator deObject &();
}

deEffect *deEffectReference::operator->() const{
	return ( deEffect* )deObjectReference::operator->();
}

deEffectReference &deEffectReference::operator=( deEffect *object ){
	deObjectReference::operator=( object );
	return *this;
}

deEffectReference &deEffectReference::operator=( const deEffectReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deEffectReference::operator==( deEffect *object ) const{
	return deObjectReference::operator==( object );
}

bool deEffectReference::operator!=( deEffect *object ) const{
	return deObjectReference::operator!=( object );
}

bool deEffectReference::operator==( const deEffectReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deEffectReference::operator!=( const deEffectReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
