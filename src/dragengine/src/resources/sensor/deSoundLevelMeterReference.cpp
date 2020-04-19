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

#include "deSoundLevelMeter.h"
#include "deSoundLevelMeterReference.h"



// Class deSoundLevelMeterReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSoundLevelMeterReference::deSoundLevelMeterReference(){
}

deSoundLevelMeterReference::deSoundLevelMeterReference( deSoundLevelMeter *object ) :
deObjectReference( object ){
}

deSoundLevelMeterReference::deSoundLevelMeterReference( const deSoundLevelMeterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSoundLevelMeterReference::TakeOver( deSoundLevelMeter *object ){
	deObjectReference::TakeOver( object );
}

bool deSoundLevelMeterReference::operator!() const{
	return deObjectReference::operator!();
}

deSoundLevelMeterReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSoundLevelMeterReference::operator deSoundLevelMeter *() const{
	return ( deSoundLevelMeter* )operator deObject *();
}

deSoundLevelMeterReference::operator deSoundLevelMeter &() const{
	return ( deSoundLevelMeter& )operator deObject &();
}

deSoundLevelMeter *deSoundLevelMeterReference::operator->() const{
	return ( deSoundLevelMeter* )deObjectReference::operator->();
}

deSoundLevelMeterReference &deSoundLevelMeterReference::operator=( deSoundLevelMeter *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSoundLevelMeterReference &deSoundLevelMeterReference::operator=( const deSoundLevelMeterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSoundLevelMeterReference::operator==( deSoundLevelMeter *object ) const{
	return deObjectReference::operator==( object );
}

bool deSoundLevelMeterReference::operator!=( deSoundLevelMeter *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSoundLevelMeterReference::operator==( const deSoundLevelMeterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSoundLevelMeterReference::operator!=( const deSoundLevelMeterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
