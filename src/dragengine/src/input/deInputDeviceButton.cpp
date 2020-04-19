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

#include "deInputDeviceButton.h"
#include "../common/exceptions.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceButton
//////////////////////////////

deInputDeviceButton::deInputDeviceButton(){
}

deInputDeviceButton::deInputDeviceButton( const deInputDeviceButton &button ) :
pID( button.pID ),
pName( button.pName ),
pDisplayImage( button.pDisplayImage ),
pDisplayIcons( button.pDisplayIcons ),
pDisplayText( button.pDisplayText ){
}

deInputDeviceButton::~deInputDeviceButton(){
}



// Device information
///////////////////////

void deInputDeviceButton::SetID( const char *id ){
	pID = id;
}

void deInputDeviceButton::SetName( const char *name ){
	pName = name;
}

void deInputDeviceButton::SetDisplayImage( deImage *image ){
	pDisplayImage = image;
}

int deInputDeviceButton::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceButton::GetDisplayIconAt( int index ) const{
	return ( deImage* )pDisplayIcons.GetAt( index );
}

void deInputDeviceButton::AddDisplayIcon( deImage *image ){
	if( ! image ){
		DETHROW( deeNullPointer );
	}
	pDisplayIcons.Add( image );
}

void deInputDeviceButton::SetDisplayText( const char *text ){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceButton &deInputDeviceButton::operator=( const deInputDeviceButton &button ){
	pID = button.pID;
	pName = button.pName;
	pDisplayImage = button.pDisplayImage;
	pDisplayIcons = button.pDisplayIcons;
	pDisplayText = button.pDisplayText;
	return *this;
}
