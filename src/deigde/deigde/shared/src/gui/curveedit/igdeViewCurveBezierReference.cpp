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

#include "igdeViewCurveBezier.h"
#include "igdeViewCurveBezierReference.h"



// Class igdeViewCurveBezierReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewCurveBezierReference::igdeViewCurveBezierReference(){
}

igdeViewCurveBezierReference::igdeViewCurveBezierReference( igdeViewCurveBezier *viewCurveBezier ) :
deObjectReference( viewCurveBezier ){
}

igdeViewCurveBezierReference::igdeViewCurveBezierReference( const igdeViewCurveBezierReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewCurveBezierReference::TakeOver( igdeViewCurveBezier *viewCurveBezier ){
	deObjectReference::TakeOver( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewCurveBezierReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewCurveBezierReference::operator igdeViewCurveBezier *() const{
	return ( igdeViewCurveBezier* )operator deObject *();
}

igdeViewCurveBezierReference::operator igdeViewCurveBezier &() const{
	return ( igdeViewCurveBezier& )operator deObject &();
}

igdeViewCurveBezier *igdeViewCurveBezierReference::operator->() const{
	return ( igdeViewCurveBezier* )deObjectReference::operator->();
}

igdeViewCurveBezierReference &igdeViewCurveBezierReference::operator=( igdeViewCurveBezier *viewCurveBezier ){
	deObjectReference::operator=( viewCurveBezier );
	return *this;
}

igdeViewCurveBezierReference &igdeViewCurveBezierReference::operator=( const igdeViewCurveBezierReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewCurveBezierReference::operator==( igdeViewCurveBezier *viewCurveBezier ) const{
	return deObjectReference::operator==( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator!=( igdeViewCurveBezier *viewCurveBezier ) const{
	return deObjectReference::operator!=( viewCurveBezier );
}

bool igdeViewCurveBezierReference::operator==( const igdeViewCurveBezierReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewCurveBezierReference::operator!=( const igdeViewCurveBezierReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
