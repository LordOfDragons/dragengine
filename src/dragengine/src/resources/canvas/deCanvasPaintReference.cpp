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

#include "deCanvasPaint.h"
#include "deCanvasPaintReference.h"



// Class deCanvasPaintReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasPaintReference::deCanvasPaintReference(){
}

deCanvasPaintReference::deCanvasPaintReference( deCanvasPaint *object ) :
deObjectReference( object ){
}

deCanvasPaintReference::deCanvasPaintReference( const deCanvasPaintReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasPaintReference::TakeOver( deCanvasPaint *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasPaintReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasPaintReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasPaintReference::operator deCanvasPaint *() const{
	return ( deCanvasPaint* )operator deObject *();
}

deCanvasPaintReference::operator deCanvasPaint &() const{
	return ( deCanvasPaint& )operator deObject &();
}

deCanvasPaint *deCanvasPaintReference::operator->() const{
	return ( deCanvasPaint* )deObjectReference::operator->();
}

deCanvasPaintReference &deCanvasPaintReference::operator=( deCanvasPaint *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasPaintReference &deCanvasPaintReference::operator=( const deCanvasPaintReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasPaintReference::operator==( deCanvasPaint *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasPaintReference::operator!=( deCanvasPaint *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasPaintReference::operator==( const deCanvasPaintReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasPaintReference::operator!=( const deCanvasPaintReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
