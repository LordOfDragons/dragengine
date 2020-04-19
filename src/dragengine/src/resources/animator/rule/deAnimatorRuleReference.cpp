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

#include "deAnimatorRule.h"
#include "deAnimatorRuleReference.h"



// Class deAnimatorRuleReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleReference::deAnimatorRuleReference(){
}

deAnimatorRuleReference::deAnimatorRuleReference( deAnimatorRule *object ) :
deObjectReference( object ){
}

deAnimatorRuleReference::deAnimatorRuleReference( const deAnimatorRuleReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorRuleReference::TakeOver( deAnimatorRule *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorRuleReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorRuleReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorRuleReference::operator deAnimatorRule *() const{
	return ( deAnimatorRule* )operator deObject *();
}

deAnimatorRuleReference::operator deAnimatorRule &() const{
	return ( deAnimatorRule& )operator deObject &();
}

deAnimatorRule *deAnimatorRuleReference::operator->() const{
	return ( deAnimatorRule* )deObjectReference::operator->();
}

deAnimatorRuleReference &deAnimatorRuleReference::operator=( deAnimatorRule *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorRuleReference &deAnimatorRuleReference::operator=( const deAnimatorRuleReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorRuleReference::operator==( deAnimatorRule *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorRuleReference::operator!=( deAnimatorRule *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorRuleReference::operator==( const deAnimatorRuleReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorRuleReference::operator!=( const deAnimatorRuleReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
