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

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionReference.h"



// Class igdeTriggerExpressionReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionReference::igdeTriggerExpressionReference(){
}

igdeTriggerExpressionReference::igdeTriggerExpressionReference( igdeTriggerExpression *expression ) :
deObjectReference( expression ){
}

igdeTriggerExpressionReference::igdeTriggerExpressionReference( const igdeTriggerExpressionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTriggerExpressionReference::TakeOver( igdeTriggerExpression *expression ){
	deObjectReference::TakeOver( expression );
}

bool igdeTriggerExpressionReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTriggerExpressionReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTriggerExpressionReference::operator igdeTriggerExpression *() const{
	return ( igdeTriggerExpression* )operator deObject *();
}

igdeTriggerExpressionReference::operator igdeTriggerExpression &() const{
	return ( igdeTriggerExpression& )operator deObject &();
}

igdeTriggerExpression *igdeTriggerExpressionReference::operator->() const{
	return ( igdeTriggerExpression* )deObjectReference::operator->();
}

igdeTriggerExpressionReference &igdeTriggerExpressionReference::operator=( igdeTriggerExpression *expression ){
	deObjectReference::operator=( expression );
	return *this;
}

igdeTriggerExpressionReference &igdeTriggerExpressionReference::operator=( const igdeTriggerExpressionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTriggerExpressionReference::operator==( igdeTriggerExpression *expression ) const{
	return deObjectReference::operator==( expression );
}

bool igdeTriggerExpressionReference::operator!=( igdeTriggerExpression *expression ) const{
	return deObjectReference::operator!=( expression );
}

bool igdeTriggerExpressionReference::operator==( const igdeTriggerExpressionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTriggerExpressionReference::operator!=( const igdeTriggerExpressionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
