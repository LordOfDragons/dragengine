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

#include <stdlib.h>

#include "deInputDeviceAxis.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceAxis
////////////////////////////

deInputDeviceAxis::deInputDeviceAxis() :
pType( eatGeneric ){
}

deInputDeviceAxis::deInputDeviceAxis( const deInputDeviceAxis &axis ) :
pID( axis.pID ),
pName( axis.pName ),
pType( axis.pType ),
pDisplayImage( axis.pDisplayImage ),
pDisplayIcons( axis.pDisplayIcons ),
pDisplayText( axis.pDisplayText ){
}

deInputDeviceAxis::~deInputDeviceAxis(){
}



// Device information
///////////////////////

void deInputDeviceAxis::SetID( const char *id ){
	pID = id;
}

void deInputDeviceAxis::SetName( const char *name ){
	pName = name;
}

void deInputDeviceAxis::SetType( eAxisTypes type ){
	pType = type;
}

void deInputDeviceAxis::SetComponent( const char *component ){
	pComponent = component;
}

void deInputDeviceAxis::SetDisplayImage( deImage *image ){
	pDisplayImage = image;
}

int deInputDeviceAxis::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceAxis::GetDisplayIconAt( int index ) const{
	return ( deImage* )pDisplayIcons.GetAt( index );
}

void deInputDeviceAxis::AddDisplayIcon( deImage *image ){
	if( ! image ){
		DETHROW( deeNullPointer );
	}
	pDisplayIcons.Add( image );
}

void deInputDeviceAxis::SetDisplayText( const char *text ){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceAxis &deInputDeviceAxis::operator=( const deInputDeviceAxis &axis ){
	pID = axis.pID;
	pName = axis.pName;
	pType = axis.pType;
	pComponent = axis.pComponent;
	pDisplayImage = axis.pDisplayImage;
	pDisplayIcons = axis.pDisplayIcons;
	pDisplayText = axis.pDisplayText;
	return *this;
}
