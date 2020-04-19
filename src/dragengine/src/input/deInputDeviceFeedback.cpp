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

#include "deInputDeviceFeedback.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceFeedback
////////////////////////////////

deInputDeviceFeedback::deInputDeviceFeedback() :
pType( eftGeneric ){
}

deInputDeviceFeedback::deInputDeviceFeedback( const deInputDeviceFeedback &feedback ) :
pID( feedback.pID ),
pName( feedback.pName ),
pType( feedback.pType ),
pDisplayImage( feedback.pDisplayImage ),
pDisplayIcons( feedback.pDisplayIcons ),
pDisplayText( feedback.pDisplayText ){
}

deInputDeviceFeedback::~deInputDeviceFeedback(){
}



// Device information
///////////////////////

void deInputDeviceFeedback::SetID( const char *id ){
	pID = id;
}

void deInputDeviceFeedback::SetName( const char *name ){
	pName = name;
}

void deInputDeviceFeedback::SetType( eFeedbackTypes type ){
	if( type < eftVibration || type > eftGeneric ){
		DETHROW( deeInvalidParam );
	}
}

void deInputDeviceFeedback::SetDisplayImage( deImage *image ){
	pDisplayImage = image;
}

int deInputDeviceFeedback::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDeviceFeedback::GetDisplayIconAt( int index ) const{
	return ( deImage* )pDisplayIcons.GetAt( index );
}

void deInputDeviceFeedback::AddDisplayIcon( deImage *image ){
	if( ! image ){
		DETHROW( deeNullPointer );
	}
	pDisplayIcons.Add( image );
}

void deInputDeviceFeedback::SetDisplayText( const char *text ){
	pDisplayText = text;
}



// Operators
//////////////

deInputDeviceFeedback &deInputDeviceFeedback::operator=( const deInputDeviceFeedback &feedback ){
	pID = feedback.pID;
	pName = feedback.pName;
	pType = feedback.pType;
	pDisplayImage = feedback.pDisplayImage;
	pDisplayIcons = feedback.pDisplayIcons;
	pDisplayText = feedback.pDisplayText;
	return *this;
}
