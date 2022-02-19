/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deoxrDevice.h"
#include "deoxrDeviceAxis.h"
#include "deoxrDeviceButton.h"
#include "deoxrDeviceFeedback.h"
#include "deoxrDeviceComponent.h"
#include "deoxrDeviceManager.h"
#include "../deVROpenXR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deoxrDevice
//////////////////////

// Constructor, destructor
////////////////////////////

deoxrDevice::deoxrDevice( deVROpenXR &oxr, const deoxrDeviceProfile &profile ) :
pOxr( oxr ),
pProfile( profile ),
pBoneConfiguration( deInputDevice::ebcNone ),
pNameNumber( -1 ){
}

deoxrDevice::~deoxrDevice(){
}



// Management
///////////////

void deoxrDevice::SetIndex( int index ){
	pIndex = index;
}

void deoxrDevice::SetSubactionPath( const deoxrPath &path ){
	pSubactionPath = path;
}

void deoxrDevice::SetActionPose( deoxrAction *action ){
	pActionPose = action;
}

void deoxrDevice::SetSpacePose( deoxrSpace *space ){
	pSpacePose = space;
}

void deoxrDevice::SetType( deInputDevice::eDeviceTypes type ){
	pType = type;
}

void deoxrDevice::SetBoneConfiguration( deInputDevice::eBoneConfigurations config ){
	pBoneConfiguration = config;
}

void deoxrDevice::SetID( const char *id ){
	pID = id;
}

void deoxrDevice::SetName( const char *name ){
	pName = name;
}

void deoxrDevice::SetDisplayImages( const char *name ){
	pDisplayImage = nullptr;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pOxr.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pOxr.GetVFS();
	const char * const basePath = "/share/image/device";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		pDisplayIcons.Add( deImage::Ref::New( imageManager.LoadImage( vfs, filename, "/" ) ) );
	}
}

void deoxrDevice::SetDisplayText( const char *text ){
	pDisplayText = text;
}



int deoxrDevice::GetButtonCount() const{
	return pButtons.GetCount();
}

void deoxrDevice::AddButton( deoxrDeviceButton *button ){
	if( ! button ){
		DETHROW( deeNullPointer );
	}
	pButtons.Add( button );
}

void deoxrDevice::AddButton( deInputDeviceButton::eButtonTypes type, deoxrDeviceComponent *component,
deVROpenXR::eInputActions actionPress, deVROpenXR::eInputActions actionTouch,
const char *name, const char *id, const char *displayText ){
	const deoxrDeviceButton::Ref button( deoxrDeviceButton::Ref::New( new deoxrDeviceButton( *this ) ) );
	button->SetID( id );
	button->SetName( name );
	button->SetType( type );
	button->SetDisplayText( displayText );
	button->SetInputDeviceComponent( component );
	button->SetActionPress( pOxr.GetAction( actionPress ) );
	button->SetActionTouch( pOxr.GetAction( actionTouch ) );
	
	button->SetIndex( pButtons.GetCount() );
	pButtons.Add( button );
}

deoxrDeviceButton *deoxrDevice::GetButtonAt( int index ) const{
	return ( deoxrDeviceButton* )pButtons.GetAt( index );
}

deoxrDeviceButton *deoxrDevice::GetButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoxrDeviceButton * const button = ( deoxrDeviceButton* )pButtons.GetAt( i );
		if( button->GetID() == id ){
			return button;
		}
	}
	
	return nullptr;
}

int deoxrDevice::IndexOfButtonWithID( const char *id ) const{
	const int count = pButtons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoxrDeviceButton &button = *( ( deoxrDeviceButton* )pButtons.GetAt( i ) );
		if( button.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deoxrDevice::GetAxisCount() const{
	return pAxes.GetCount();
}

void deoxrDevice::AddAxis( deoxrDeviceAxis *axis ){
	if( ! axis ){
		DETHROW( deeNullPointer );
	}
	pAxes.Add( axis );
}

void deoxrDevice::AddAxisTrigger( deInputDeviceAxis::eAxisTypes type, deoxrDeviceComponent *component,
deVROpenXR::eInputActions actionAnalog, const char *name, const char *id, const char *displayText ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( *this ) ) );
	axis->SetActionAnalog( pOxr.GetAction( actionAnalog ) );
	axis->SetType( type );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( name );
	axis->SetID( id );
	axis->SetDisplayText( displayText );
	axis->SetIndex( pAxes.GetCount() );
	axis->SetInputDeviceComponent( component );
	pAxes.Add( axis );
}

void deoxrDevice::AddAxisFinger( deInputDeviceAxis::eAxisTypes type, deoxrDeviceComponent *component,
int finger, const char *name, const char *id, const char *displayText ){
	const deoxrDeviceAxis::Ref axis( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( *this ) ) );
	axis->SetType( type );
	axis->SetRange( 0.0f, 1.0f );
	axis->SetCenter( -1.0f );
	axis->SetValue( -1.0f );
	axis->SetName( name );
	axis->SetID( id );
	axis->SetDisplayText( displayText );
	axis->SetIndex( pAxes.GetCount() );
	axis->SetFinger( finger );
	axis->SetInputDeviceComponent( component );
	pAxes.Add( axis );
}

void deoxrDevice::AddAxesJoystick( deoxrDeviceComponent *component,
deVROpenXR::eInputActions actionAnalog, const char *name, const char *id, const char *displayText ){
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	deoxrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( *this ) ) );
		axis->SetActionAnalog( pOxr.GetAction( actionAnalog ) );
		axis->SetType( deInputDeviceAxis::eatStick );
		axis->SetComponent( i );
		text.Format( "%s %s", name, nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "%s%s", id, idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( displayText );
		axis->SetIndex( pAxes.GetCount() );
		axis->SetDeadZone( axis->GetResolution() );
		axis->SetInputDeviceComponent( component );
		pAxes.Add( axis );
	}
}

void deoxrDevice::AddAxesTrackpad( deoxrDeviceComponent *component,
deVROpenXR::eInputActions actionAnalog, const char *name, const char *id, const char *displayText ){
	static const char *nameAxis[ 2 ] = { "X", "Y" };
	static const char *idAxis[ 2 ] = { "x", "y" };
	deoxrDeviceAxis::Ref axis;
	decString text;
	int i;
	
	for( i=0; i<2; i++ ){
		axis.TakeOver( deoxrDeviceAxis::Ref::New( new deoxrDeviceAxis( *this ) ) );
		axis->SetActionAnalog( pOxr.GetAction( actionAnalog ) );
		axis->SetType( deInputDeviceAxis::eatTouchPad );
		axis->SetComponent( i );
		text.Format( "%s %s", name, nameAxis[ i ] );
		axis->SetName( text );
		text.Format( "%s%s", id, idAxis[ i ] );
		axis->SetID( text );
		axis->SetDisplayText( displayText );
		axis->SetIndex( pAxes.GetCount() );
		axis->SetDeadZone( axis->GetResolution() );
		axis->SetInputDeviceComponent( component );
		pAxes.Add( axis );
	}
}

deoxrDeviceAxis *deoxrDevice::GetAxisAt( int index ) const{
	return ( deoxrDeviceAxis* )pAxes.GetAt( index );
}

deoxrDeviceAxis *deoxrDevice::GetAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoxrDeviceAxis * const axis = ( deoxrDeviceAxis* )pAxes.GetAt( i );
		if( axis->GetID() == id ){
			return axis;
		}
	}
	
	return nullptr;
}

int deoxrDevice::IndexOfAxisWithID( const char *id ) const{
	const int count = pAxes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deoxrDeviceAxis &axis = *( ( deoxrDeviceAxis* )pAxes.GetAt( i ) );
		if( axis.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deoxrDevice::GetFeedbackCount() const{
	return pFeedbacks.GetCount();
}

void deoxrDevice::AddFeedback( deoxrDeviceFeedback *feedback ){
	if( ! feedback ){
		DETHROW( deeNullPointer );
	}
	pFeedbacks.Add( feedback );
}

deoxrDeviceFeedback *deoxrDevice::GetFeedbackAt( int index ) const{
	return ( deoxrDeviceFeedback* )pFeedbacks.GetAt( index );
}

int deoxrDevice::IndexOfFeedbackWithID( const char *id ) const{
	const int count = pFeedbacks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deoxrDeviceFeedback &feedback = *( ( deoxrDeviceFeedback* )pFeedbacks.GetAt( i ) );
		if( feedback.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



int deoxrDevice::GetComponentCount() const{
	return pComponents.GetCount();
}

void deoxrDevice::AddComponent( deoxrDeviceComponent *component ){
	if( ! component ){
		DETHROW( deeNullPointer );
	}
	pComponents.Add( component );
}

deoxrDeviceComponent *deoxrDevice::AddComponent( deInputDeviceComponent::eComponentTypes type,
const char *name, const char *id, const char *displayText ){
	const deoxrDeviceComponent::Ref component( deoxrDeviceComponent::Ref::New( new deoxrDeviceComponent( *this ) ) );
	component->SetID( id );
	component->SetName( name );
	component->SetType( type );
	component->SetDisplayText( displayText );
	component->SetIndex( pComponents.GetCount() );
	pComponents.Add( component );
	return component;
}

deoxrDeviceComponent *deoxrDevice::GetComponentAt( int index ) const{
	return ( deoxrDeviceComponent* )pComponents.GetAt( index );
}

int deoxrDevice::IndexOfComponentWithID( const char *id ) const{
	const int count = pComponents.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deoxrDeviceComponent &component = *( ( deoxrDeviceComponent* )pComponents.GetAt( i ) );
		if( component.GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void deoxrDevice::SetHandTracker( deoxrHandTracker *handTracker ){
	pHandTracker = handTracker;
}

void deoxrDevice::SetMatrixWristToDevice( const decMatrix &matrix ){
	pMatrixWristToDevice = matrix;
}



void deoxrDevice::GetInfo( deInputDevice &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
	
	info.SetDisplayModel( nullptr );
	info.SetDisplaySkin( nullptr );
	
	info.SetBoneConfiguration( pBoneConfiguration );
// 	if( pRenderModel ){
// 		info.SetVRModel( pRenderModel->GetModel() );
// 	}
// 	if( pTextureMap ){
// 		info.SetVRSkin( pTextureMap->GetSkin() );
// 	}
	
	const int buttonCount = pButtons.GetCount();
	info.SetButtonCount( buttonCount );
	for( i=0; i<buttonCount; i++ ){
		( ( deoxrDeviceButton* )pButtons.GetAt( i ) )->GetInfo( info.GetButtonAt( i ) );
	}
	
	const int axisCount = pAxes.GetCount();
	info.SetAxisCount( axisCount );
	for( i=0; i<axisCount; i++ ){
		( ( deoxrDeviceAxis* )pAxes.GetAt( i ) )->GetInfo( info.GetAxisAt( i ) );
	}
	
	const int feedbackCount = pFeedbacks.GetCount();
	info.SetFeedbackCount( feedbackCount );
	for( i=0; i<feedbackCount; i++ ){
		( ( deoxrDeviceFeedback* )pFeedbacks.GetAt( i ) )->GetInfo( info.GetFeedbackAt( i ) );
	}
}

void deoxrDevice::TrackStates(){
	const deoxrInstance &instance = pOxr.GetInstance();
	const deoxrSession &session = pOxr.GetSession();
	
	if( pType == deInputDevice::edtVRHMD ){
		pPosePosition = session.GetHeadPosition();
		pPoseOrientation = session.GetHeadOrientation();
		pPoseLinearVelocity = session.GetHeadLinearVelocity();
		pPoseAngularVelocity = session.GetHeadAngularVelocity();
		
		pPoseDevice.SetPosition( pPosePosition );
		pPoseDevice.SetOrientation( pPoseOrientation );
		pPoseDevice.SetLinearVelocity( pPoseLinearVelocity );
		pPoseDevice.SetAngularVelocity( pPoseAngularVelocity );
	}
	
	if( pActionPose && pSpacePose ){
		XrActionStateGetInfo getInfo;
		memset( &getInfo, 0, sizeof( getInfo ) );
		getInfo.type = XR_TYPE_ACTION_STATE_GET_INFO;
		getInfo.action = pActionPose->GetAction();
		getInfo.subactionPath = pSubactionPath;
		
		XrActionStatePose state;
		memset( &state, 0, sizeof( state ) );
		state.type = XR_TYPE_ACTION_STATE_POSE;
		
		if( XR_SUCCEEDED( instance.xrGetActionStatePose( session.GetSession(), &getInfo, &state ) )
		&& state.isActive ){
			pSpacePose->LocateSpace( session.GetSpace(), session.GetPredictedDisplayTime(),
				pPosePosition, pPoseOrientation, pPoseLinearVelocity, pPoseAngularVelocity );
			
			pPoseDevice.SetPosition( pPosePosition );
			pPoseDevice.SetOrientation( pPoseOrientation );
			pPoseDevice.SetLinearVelocity( pPoseLinearVelocity );
			pPoseDevice.SetAngularVelocity( pPoseAngularVelocity );
		}
	}
	
	if( pHandTracker ){
		pHandTracker->Locate();
		
		// if no space pose exists use hand tracker wrist position to calculate a reasonable
		// device location which is at a similar location of true controller hand poses
		if( ! pSpacePose ){
			deInputDevicePose &wrist = pHandTracker->GetPoseBoneAt( deInputDevice::ehbWrist );
			
			const decMatrix wristMatrix( decMatrix::CreateWorld( wrist.GetPosition(), wrist.GetOrientation() ) );
			const decMatrix matrix( pMatrixWristToDevice * wristMatrix );
			const decMatrix invMatrix( matrix.QuickInvert() );
			const decQuaternion invQuat( invMatrix.ToQuaternion() );
			
			pPoseDevice.SetPosition( matrix.GetPosition() );
			pPoseDevice.SetOrientation( matrix.ToQuaternion() );
			
			const decMatrix velocityMatrix( wristMatrix.QuickInvert() * pMatrixWristToDevice * wristMatrix );
			pPoseDevice.SetLinearVelocity( velocityMatrix.TransformNormal( wrist.GetLinearVelocity() ) );
			pPoseDevice.SetAngularVelocity( velocityMatrix.TransformNormal( wrist.GetAngularVelocity() ) );
			
			int i;
			for( i=0; i<deInputDevice::HandBoneCount; i++ ){
				deInputDevicePose &pose = pHandTracker->GetPoseBoneAt( i );
				pose.SetPosition( invMatrix * pose.GetPosition() );
				pose.SetOrientation( pose.GetOrientation() * invQuat );
				pose.SetLinearVelocity( invMatrix.TransformNormal( pose.GetLinearVelocity() ) );
				pose.SetAngularVelocity( invMatrix.TransformNormal( pose.GetAngularVelocity() ) );
			}
		}
	}
	
	int i, count = pButtons.GetCount();
	for( i=0; i<count; i++ ){
		GetButtonAt( i )->TrackState();
	}
	
	count = pAxes.GetCount();
	for( i=0; i<count; i++ ){
		GetAxisAt( i )->TrackState();
	}
}

void deoxrDevice::ResetStates(){
	const int axisCount = pAxes.GetCount();
	int i;
	
	for( i=0; i<axisCount; i++ ){
		GetAxisAt( i )->ResetState();
	}
}

void deoxrDevice::GetDevicePose( deInputDevicePose &pose ){
	pose = pPoseDevice;
}
