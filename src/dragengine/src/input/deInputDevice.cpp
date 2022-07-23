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

#include "deInputDevice.h"
#include "deInputDeviceAxis.h"
#include "deInputDeviceButton.h"
#include "deInputDeviceFeedback.h"
#include "deInputDeviceComponent.h"
#include "../common/exceptions.h"
#include "../resources/model/deModel.h"
#include "../resources/skin/deSkin.h"
#include "../resources/image/deImage.h"



// Class deInputDevice
////////////////////////

deInputDevice::deInputDevice() :
pType( edtGeneric ),
pButtons( NULL ),
pButtonCount( 0 ),
pAxes( NULL ),
pAxisCount( 0 ),
pFeedbacks( NULL ),
pFeedbackCount( 0 ),
pComponents( nullptr ),
pComponentCount( 0 ),
pBoneConfiguration( ebcNone ),
pSupportsFaceEyeExpressions( false ),
pSupportsFaceMouthExpressions( false ){
}

deInputDevice::~deInputDevice(){
	pCleanUp();
}



// Device information
///////////////////////

void deInputDevice::SetID( const char *id ){
	pID = id;
}

void deInputDevice::SetName( const char *name ){
	pName = name;
}

void deInputDevice::SetType( eDeviceTypes type ){
	pType = type;
}

void deInputDevice::SetDisplayModel( deModel *model ){
	pDisplayModel = model;
}

void deInputDevice::SetDisplaySkin( deSkin *skin ){
	pDisplaySkin = skin;
}

void deInputDevice::SetDisplayImage( deImage *image ){
	pDisplayImage = image;
}

int deInputDevice::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage *deInputDevice::GetDisplayIconAt( int index ) const{
	return ( deImage* )pDisplayIcons.GetAt( index );
}

void deInputDevice::AddDisplayIcon( deImage *image ){
	if( ! image ){
		DETHROW( deeNullPointer );
	}
	pDisplayIcons.Add( image );
}

void deInputDevice::SetDisplayText( const char * text){
	pDisplayText = text;
}

void deInputDevice::SetBoneConfiguration( eBoneConfigurations configuration ){
	pBoneConfiguration = configuration;
}

const decVector &deInputDevice::GetFingerTipOffset( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index > 4 ){
		DETHROW_INFO( deeInvalidParam, "index > 4" );
	}
	return pFingerTipOffset[ index ];
}

void deInputDevice::SetFingerTipOffset( int index, const decVector &offset ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index > 4 ){
		DETHROW_INFO( deeInvalidParam, "index > 4" );
	}
	pFingerTipOffset[ index ] = offset;
}

void deInputDevice::SetHandRig( deRig *rig ){
	pHandRig = rig;
}

void deInputDevice::SetSupportsFaceEyeExpressions( bool supportsFaceEyeExpressions ){
	pSupportsFaceEyeExpressions = supportsFaceEyeExpressions;
}

void deInputDevice::SetSupportsFaceMouthExpressions( bool supportsFaceMouthExpressions ){
	pSupportsFaceMouthExpressions = supportsFaceMouthExpressions;
}

void deInputDevice::SetVRModel( deModel *model ){
	pVRModel = model;
}

void deInputDevice::SetVRSkin( deSkin *skin ){
	pVRSkin = skin;
}



// Buttons
////////////

void deInputDevice::SetButtonCount( int count ){
	if( pButtons ){
		delete [] pButtons;
		pButtons = NULL;
		pButtonCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pButtons = new deInputDeviceButton[ count ];
	pButtonCount = count;
}

deInputDeviceButton &deInputDevice::GetButtonAt( int index ) const{
	if( index < 0 || index >= pButtonCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pButtons[ index ];
}

int deInputDevice::IndexOfButtonWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pButtonCount; i++ ){
		if( pButtons[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



// Axes
/////////

void deInputDevice::SetAxisCount( int count ){
	if( pAxes ){
		delete [] pAxes;
		pAxes = NULL;
		pAxisCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pAxes = new deInputDeviceAxis[ count ];
	pAxisCount = count;
}

deInputDeviceAxis &deInputDevice::GetAxisAt( int index ) const{
	if( index < 0 || index >= pAxisCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pAxes[ index ];
}

int deInputDevice::IndexOfAxisWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pAxisCount; i++ ){
		if( pAxes[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



// Feedbacks
//////////////

void deInputDevice::SetFeedbackCount( int count ){
	if( pFeedbacks ){
		delete [] pFeedbacks;
		pFeedbacks = NULL;
		pFeedbackCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pFeedbacks = new deInputDeviceFeedback[ count ];
	pFeedbackCount = count;
}

deInputDeviceFeedback &deInputDevice::GetFeedbackAt( int index ) const{
	if( index < 0 || index >= pFeedbackCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pFeedbacks[ index ];
}

int deInputDevice::IndexOfFeedbackWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pFeedbackCount; i++ ){
		if( pFeedbacks[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



// Components
//////////////

void deInputDevice::SetComponentCount( int count ){
	if( pComponents ){
		delete [] pComponents;
		pComponents = NULL;
		pComponentCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pComponents = new deInputDeviceComponent[ count ];
	pComponentCount = count;
}

deInputDeviceComponent &deInputDevice::GetComponentAt( int index ) const{
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pComponents[ index ];
}

int deInputDevice::IndexOfComponentWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pComponentCount; i++ ){
		if( pComponents[ i ].GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



// Privat functions
/////////////////////

void deInputDevice::pCleanUp(){
	if( pComponents ){
		delete [] pComponents;
	}
	if( pFeedbacks ){
		delete [] pFeedbacks;
	}
	if( pAxes ){
		delete [] pAxes;
	}
	if( pButtons ){
		delete [] pButtons;
	}
}
