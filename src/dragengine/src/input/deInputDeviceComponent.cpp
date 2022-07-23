/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deInputDeviceComponent.h"
#include "../common/exceptions.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceComponent
//////////////////////////////

deInputDeviceComponent::deInputDeviceComponent() :
pType( ectGeneric ){
}

deInputDeviceComponent::deInputDeviceComponent( const deInputDeviceComponent &component ) :
pID( component.pID ),
pName( component.pName ),
pType( component.pType ),
pDisplayImage( component.pDisplayImage ),
pDisplayIcons( component.pDisplayIcons ),
pDisplayText( component.pDisplayText ){
}

deInputDeviceComponent::~deInputDeviceComponent(){
}



// Device information
///////////////////////

void deInputDeviceComponent::SetID( const char *id ){
	pID = id;
}

void deInputDeviceComponent::SetName( const char *name ){
	pName = name;
}

void deInputDeviceComponent::SetType(deInputDeviceComponent:: eComponentTypes type ){
	pType = type;
}

void deInputDeviceComponent::SetDisplayImage( deImage *image ){
	pDisplayImage = image;
}

int deInputDeviceComponent::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceComponent::GetDisplayIconAt( int index ) const{
	return ( deImage* )pDisplayIcons.GetAt( index );
}

void deInputDeviceComponent::AddDisplayIcon( deImage *image ){
	if( ! image ){
		DETHROW( deeNullPointer );
	}
	pDisplayIcons.Add( image );
}

void deInputDeviceComponent::SetDisplayText( const char *text ){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceComponent &deInputDeviceComponent::operator=( const deInputDeviceComponent &component ){
	pID = component.pID;
	pName = component.pName;
	pType = component.pType;
	pDisplayImage = component.pDisplayImage;
	pDisplayIcons = component.pDisplayIcons;
	pDisplayText = component.pDisplayText;
	return *this;
}
