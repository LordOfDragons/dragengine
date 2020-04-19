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

#include "deCanvasCanvasView.h"
#include "deCanvasCanvasViewReference.h"



// Class deCanvasCanvasViewReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasCanvasViewReference::deCanvasCanvasViewReference(){
}

deCanvasCanvasViewReference::deCanvasCanvasViewReference( deCanvasCanvasView *object ) :
deObjectReference( object ){
}

deCanvasCanvasViewReference::deCanvasCanvasViewReference( const deCanvasCanvasViewReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasCanvasViewReference::TakeOver( deCanvasCanvasView *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasCanvasViewReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasCanvasViewReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasCanvasViewReference::operator deCanvasCanvasView *() const{
	return ( deCanvasCanvasView* )operator deObject *();
}

deCanvasCanvasViewReference::operator deCanvasCanvasView &() const{
	return ( deCanvasCanvasView& )operator deObject &();
}

deCanvasCanvasView *deCanvasCanvasViewReference::operator->() const{
	return ( deCanvasCanvasView* )deObjectReference::operator->();
}

deCanvasCanvasViewReference &deCanvasCanvasViewReference::operator=( deCanvasCanvasView *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasCanvasViewReference &deCanvasCanvasViewReference::operator=( const deCanvasCanvasViewReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasCanvasViewReference::operator==( deCanvasCanvasView *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasCanvasViewReference::operator!=( deCanvasCanvasView *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasCanvasViewReference::operator==( const deCanvasCanvasViewReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasCanvasViewReference::operator!=( const deCanvasCanvasViewReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
