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

#include "deInputDeviceButton.h"
#include "../common/exceptions.h"
#include "../resources/image/deImage.h"



// Class deInputDeviceButton
//////////////////////////////

deInputDeviceButton::deInputDeviceButton() :
pType( ebtGeneric ),
pTouchable( false ){
}

deInputDeviceButton::deInputDeviceButton( const deInputDeviceButton &button ) :
pID( button.pID ),
pName( button.pName ),
pType( button.pType ),
pDisplayImage( button.pDisplayImage ),
pDisplayIcons( button.pDisplayIcons ),
pDisplayText( button.pDisplayText ),
pTouchable( button.pTouchable ){
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

void deInputDeviceButton::SetType(deInputDeviceButton:: eButtonTypes type ){
	pType = type;
}

void deInputDeviceButton::SetComponent( const char *component ){
	pComponent = component;
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

void deInputDeviceButton::SetTouchable( bool touchable ){
	pTouchable = touchable;
}



// Operators
//////////////

deInputDeviceButton &deInputDeviceButton::operator=( const deInputDeviceButton &button ){
	pID = button.pID;
	pName = button.pName;
	pType = button.pType;
	pComponent = button.pComponent;
	pDisplayImage = button.pDisplayImage;
	pDisplayIcons = button.pDisplayIcons;
	pDisplayText = button.pDisplayText;
	pTouchable = button.pTouchable;
	return *this;
}
