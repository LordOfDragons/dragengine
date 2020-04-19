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

#include "deCanvasImage.h"
#include "deCanvasImageReference.h"



// Class deCanvasImageReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasImageReference::deCanvasImageReference(){
}

deCanvasImageReference::deCanvasImageReference( deCanvasImage *object ) :
deObjectReference( object ){
}

deCanvasImageReference::deCanvasImageReference( const deCanvasImageReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasImageReference::TakeOver( deCanvasImage *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasImageReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasImageReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasImageReference::operator deCanvasImage *() const{
	return ( deCanvasImage* )operator deObject *();
}

deCanvasImageReference::operator deCanvasImage &() const{
	return ( deCanvasImage& )operator deObject &();
}

deCanvasImage *deCanvasImageReference::operator->() const{
	return ( deCanvasImage* )deObjectReference::operator->();
}

deCanvasImageReference &deCanvasImageReference::operator=( deCanvasImage *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasImageReference &deCanvasImageReference::operator=( const deCanvasImageReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasImageReference::operator==( deCanvasImage *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasImageReference::operator!=( deCanvasImage *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasImageReference::operator==( const deCanvasImageReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasImageReference::operator!=( const deCanvasImageReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
