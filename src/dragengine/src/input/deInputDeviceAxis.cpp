/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
