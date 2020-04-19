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

#include "deCaptureCanvas.h"
#include "deCaptureCanvasReference.h"



// Class deCaptureCanvasReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deCaptureCanvasReference::deCaptureCanvasReference(){
}

deCaptureCanvasReference::deCaptureCanvasReference( deCaptureCanvas *object ) :
deObjectReference( object ){
}

deCaptureCanvasReference::deCaptureCanvasReference( const deCaptureCanvasReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCaptureCanvasReference::TakeOver( deCaptureCanvas *object ){
	deObjectReference::TakeOver( object );
}

bool deCaptureCanvasReference::operator!() const{
	return deObjectReference::operator!();
}

deCaptureCanvasReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCaptureCanvasReference::operator deCaptureCanvas *() const{
	return ( deCaptureCanvas* )operator deObject *();
}

deCaptureCanvasReference::operator deCaptureCanvas &() const{
	return ( deCaptureCanvas& )operator deObject &();
}

deCaptureCanvas *deCaptureCanvasReference::operator->() const{
	return ( deCaptureCanvas* )deObjectReference::operator->();
}

deCaptureCanvasReference &deCaptureCanvasReference::operator=( deCaptureCanvas *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCaptureCanvasReference &deCaptureCanvasReference::operator=( const deCaptureCanvasReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCaptureCanvasReference::operator==( deCaptureCanvas *object ) const{
	return deObjectReference::operator==( object );
}

bool deCaptureCanvasReference::operator!=( deCaptureCanvas *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCaptureCanvasReference::operator==( const deCaptureCanvasReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCaptureCanvasReference::operator!=( const deCaptureCanvasReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
