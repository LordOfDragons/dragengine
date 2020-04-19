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

#include "deCanvas.h"
#include "deCanvasReference.h"



// Class deCanvasReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasReference::deCanvasReference(){
}

deCanvasReference::deCanvasReference( deCanvas *object ) :
deObjectReference( object ){
}

deCanvasReference::deCanvasReference( const deCanvasReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasReference::TakeOver( deCanvas *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasReference::operator deCanvas *() const{
	return ( deCanvas* )operator deObject *();
}

deCanvasReference::operator deCanvas &() const{
	return ( deCanvas& )operator deObject &();
}

deCanvas *deCanvasReference::operator->() const{
	return ( deCanvas* )deObjectReference::operator->();
}

deCanvasReference &deCanvasReference::operator=( deCanvas *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasReference &deCanvasReference::operator=( const deCanvasReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasReference::operator==( deCanvas *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasReference::operator!=( deCanvas *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasReference::operator==( const deCanvasReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasReference::operator!=( const deCanvasReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
