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

#include "deCanvasRenderWorld.h"
#include "deCanvasRenderWorldReference.h"



// Class deCanvasRenderWorldReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasRenderWorldReference::deCanvasRenderWorldReference(){
}

deCanvasRenderWorldReference::deCanvasRenderWorldReference( deCanvasRenderWorld *object ) :
deObjectReference( object ){
}

deCanvasRenderWorldReference::deCanvasRenderWorldReference( const deCanvasRenderWorldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasRenderWorldReference::TakeOver( deCanvasRenderWorld *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasRenderWorldReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasRenderWorldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasRenderWorldReference::operator deCanvasRenderWorld *() const{
	return ( deCanvasRenderWorld* )operator deObject *();
}

deCanvasRenderWorldReference::operator deCanvasRenderWorld &() const{
	return ( deCanvasRenderWorld& )operator deObject &();
}

deCanvasRenderWorld *deCanvasRenderWorldReference::operator->() const{
	return ( deCanvasRenderWorld* )deObjectReference::operator->();
}

deCanvasRenderWorldReference &deCanvasRenderWorldReference::operator=( deCanvasRenderWorld *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasRenderWorldReference &deCanvasRenderWorldReference::operator=( const deCanvasRenderWorldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasRenderWorldReference::operator==( deCanvasRenderWorld *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasRenderWorldReference::operator!=( deCanvasRenderWorld *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasRenderWorldReference::operator==( const deCanvasRenderWorldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasRenderWorldReference::operator!=( const deCanvasRenderWorldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
