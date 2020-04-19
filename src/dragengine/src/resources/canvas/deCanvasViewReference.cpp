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

#include "deCanvasView.h"
#include "deCanvasViewReference.h"



// Class deCanvasViewReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasViewReference::deCanvasViewReference(){
}

deCanvasViewReference::deCanvasViewReference( deCanvasView *object ) :
deObjectReference( object ){
}

deCanvasViewReference::deCanvasViewReference( const deCanvasViewReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasViewReference::TakeOver( deCanvasView *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasViewReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasViewReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasViewReference::operator deCanvasView *() const{
	return ( deCanvasView* )operator deObject *();
}

deCanvasViewReference::operator deCanvasView &() const{
	return ( deCanvasView& )operator deObject &();
}

deCanvasView *deCanvasViewReference::operator->() const{
	return ( deCanvasView* )deObjectReference::operator->();
}

deCanvasViewReference &deCanvasViewReference::operator=( deCanvasView *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasViewReference &deCanvasViewReference::operator=( const deCanvasViewReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasViewReference::operator==( deCanvasView *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasViewReference::operator!=( deCanvasView *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasViewReference::operator==( const deCanvasViewReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasViewReference::operator!=( const deCanvasViewReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
