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

void deInputDevice::SetDisplayModel(const deModel::Ref &model){
	pDisplayModel = model;
}

void deInputDevice::SetDisplaySkin(const deSkin::Ref &skin){
	pDisplaySkin = skin;
}

void deInputDevice::SetDisplayImage(const deImage::Ref &image){
	pDisplayImage = image;
}

int deInputDevice::GetDisplayIconCount() const{
	return pDisplayIcons.GetCount();
}

deImage::Ref deInputDevice::GetDisplayIconAt(int index) const{
	return (deImage*)pDisplayIcons.GetAt(index);
}

void deInputDevice::AddDisplayIcon(const deImage::Ref &image){
	DEASSERT_NOTNULL(image)
	pDisplayIcons.Add(image);
}

void deInputDevice::SetDisplayText( const char * text){
	pDisplayText = text;
}

void deInputDevice::SetBoneConfiguration( eBoneConfigurations configuration ){
	pBoneConfiguration = configuration;
}

const decVector &deInputDevice::GetFingerTipOffset( int index ) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 4)
	return pFingerTipOffset[ index ];
}

void deInputDevice::SetFingerTipOffset( int index, const decVector &offset ){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 4)
	pFingerTipOffset[ index ] = offset;
}

void deInputDevice::SetHandRig(const deRig::Ref &rig){
	pHandRig = rig;
}

void deInputDevice::SetSupportsFaceEyeExpressions( bool supportsFaceEyeExpressions ){
	pSupportsFaceEyeExpressions = supportsFaceEyeExpressions;
}

void deInputDevice::SetSupportsFaceMouthExpressions( bool supportsFaceMouthExpressions ){
	pSupportsFaceMouthExpressions = supportsFaceMouthExpressions;
}

void deInputDevice::SetVRModel(const deModel::Ref &model){
	pVRModel = model;
}

void deInputDevice::SetVRSkin(const deSkin::Ref &skin){
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

deInputDeviceButton &deInputDevice::GetButtonAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pButtonCount)
	return pButtons[index];
}

int deInputDevice::IndexOfButtonWithID( const char *id ) const{
	DEASSERT_NOTNULL(id)
	
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

deInputDeviceAxis &deInputDevice::GetAxisAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pAxisCount)
	return pAxes[index];
}

int deInputDevice::IndexOfAxisWithID( const char *id ) const{
	DEASSERT_NOTNULL(id)
	
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

deInputDeviceFeedback &deInputDevice::GetFeedbackAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pFeedbackCount)
	return pFeedbacks[index];
}

int deInputDevice::IndexOfFeedbackWithID( const char *id ) const{
	DEASSERT_NOTNULL(id)
	
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

deInputDeviceComponent &deInputDevice::GetComponentAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pComponentCount)
	return pComponents[index];
}

int deInputDevice::IndexOfComponentWithID( const char *id ) const{
	DEASSERT_NOTNULL(id)
	
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
