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

#include "deVideoPlayer.h"
#include "deVideoPlayerReference.h"



// Class deVideoPlayerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deVideoPlayerReference::deVideoPlayerReference(){
}

deVideoPlayerReference::deVideoPlayerReference( deVideoPlayer *object ) :
deObjectReference( object ){
}

deVideoPlayerReference::deVideoPlayerReference( const deVideoPlayerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoPlayerReference::TakeOver( deVideoPlayer *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoPlayerReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoPlayerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoPlayerReference::operator deVideoPlayer *() const{
	return ( deVideoPlayer* )operator deObject *();
}

deVideoPlayerReference::operator deVideoPlayer &() const{
	return ( deVideoPlayer& )operator deObject &();
}

deVideoPlayer *deVideoPlayerReference::operator->() const{
	return ( deVideoPlayer* )deObjectReference::operator->();
}

deVideoPlayerReference &deVideoPlayerReference::operator=( deVideoPlayer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoPlayerReference &deVideoPlayerReference::operator=( const deVideoPlayerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoPlayerReference::operator==( deVideoPlayer *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoPlayerReference::operator!=( deVideoPlayer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoPlayerReference::operator==( const deVideoPlayerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoPlayerReference::operator!=( const deVideoPlayerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
