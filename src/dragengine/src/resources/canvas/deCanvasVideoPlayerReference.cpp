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

#include "deCanvasVideoPlayer.h"
#include "deCanvasVideoPlayerReference.h"



// Class deCanvasVideoPlayerReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference(){
}

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference( deCanvasVideoPlayer *object ) :
deObjectReference( object ){
}

deCanvasVideoPlayerReference::deCanvasVideoPlayerReference( const deCanvasVideoPlayerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deCanvasVideoPlayerReference::TakeOver( deCanvasVideoPlayer *object ){
	deObjectReference::TakeOver( object );
}

bool deCanvasVideoPlayerReference::operator!() const{
	return deObjectReference::operator!();
}

deCanvasVideoPlayerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deCanvasVideoPlayerReference::operator deCanvasVideoPlayer *() const{
	return ( deCanvasVideoPlayer* )operator deObject *();
}

deCanvasVideoPlayerReference::operator deCanvasVideoPlayer &() const{
	return ( deCanvasVideoPlayer& )operator deObject &();
}

deCanvasVideoPlayer *deCanvasVideoPlayerReference::operator->() const{
	return ( deCanvasVideoPlayer* )deObjectReference::operator->();
}

deCanvasVideoPlayerReference &deCanvasVideoPlayerReference::operator=( deCanvasVideoPlayer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deCanvasVideoPlayerReference &deCanvasVideoPlayerReference::operator=( const deCanvasVideoPlayerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deCanvasVideoPlayerReference::operator==( deCanvasVideoPlayer *object ) const{
	return deObjectReference::operator==( object );
}

bool deCanvasVideoPlayerReference::operator!=( deCanvasVideoPlayer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deCanvasVideoPlayerReference::operator==( const deCanvasVideoPlayerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deCanvasVideoPlayerReference::operator!=( const deCanvasVideoPlayerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
