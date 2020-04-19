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

#include "deEnvMapProbe.h"
#include "deEnvMapProbeReference.h"



// Class deEnvMapProbeReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deEnvMapProbeReference::deEnvMapProbeReference(){
}

deEnvMapProbeReference::deEnvMapProbeReference( deEnvMapProbe *object ) :
deObjectReference( object ){
}

deEnvMapProbeReference::deEnvMapProbeReference( const deEnvMapProbeReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deEnvMapProbeReference::TakeOver( deEnvMapProbe *object ){
	deObjectReference::TakeOver( object );
}

bool deEnvMapProbeReference::operator!() const{
	return deObjectReference::operator!();
}

deEnvMapProbeReference::operator bool() const{
	return deObjectReference::operator bool();
}

deEnvMapProbeReference::operator deEnvMapProbe *() const{
	return ( deEnvMapProbe* )operator deObject *();
}

deEnvMapProbeReference::operator deEnvMapProbe &() const{
	return ( deEnvMapProbe& )operator deObject &();
}

deEnvMapProbe *deEnvMapProbeReference::operator->() const{
	return ( deEnvMapProbe* )deObjectReference::operator->();
}

deEnvMapProbeReference &deEnvMapProbeReference::operator=( deEnvMapProbe *object ){
	deObjectReference::operator=( object );
	return *this;
}

deEnvMapProbeReference &deEnvMapProbeReference::operator=( const deEnvMapProbeReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deEnvMapProbeReference::operator==( deEnvMapProbe *object ) const{
	return deObjectReference::operator==( object );
}

bool deEnvMapProbeReference::operator!=( deEnvMapProbe *object ) const{
	return deObjectReference::operator!=( object );
}

bool deEnvMapProbeReference::operator==( const deEnvMapProbeReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deEnvMapProbeReference::operator!=( const deEnvMapProbeReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
