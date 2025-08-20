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

#ifndef _DEOXRDEVICE_H_
#define _DEOXRDEVICE_H_

#include "../deVROpenXR.h"
#include "../deoxrPath.h"
#include "../deoxrSpace.h"
#include "../deoxrHandTracker.h"
#include "../deoxrFaceTracker.h"
#include "../action/deoxrAction.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/rig/deRig.h>

class deoxrDeviceAxis;
class deoxrDeviceButton;
class deoxrDeviceFeedback;
class deoxrDeviceComponent;
class deoxrDeviceProfile;
class deInputDevice;
class deInputDevicePose;


/**
 * VR input device.
 */
class deoxrDevice : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrDevice> Ref;
	
	
	
private:
	deVROpenXR &pOxr;
	
	int pIndex;
	
	const deoxrDeviceProfile &pProfile;
	
	deoxrPath pSubactionPath;
	
	deoxrAction::Ref pActionPose;
	deoxrAction::Ref pActionPoseOrientation;
	deoxrSpace::Ref pSpacePose;
	deoxrSpace::Ref pSpacePoseOrientation;
	decVector pPosePosition;
	decQuaternion pPoseOrientation;
	decVector pPoseLinearVelocity;
	decVector pPoseAngularVelocity;
	
	deInputDevice::eDeviceTypes pType;
	deInputDevice::eBoneConfigurations pBoneConfiguration;
	int pNameNumber;
	decString pID;
	decString pName;
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	decObjectOrderedSet pButtons;
	decObjectOrderedSet pAxes;
	decObjectOrderedSet pFeedbacks;
	decObjectOrderedSet pComponents;
	
	deInputDevicePose pPoseDevice;
	
	deoxrHandTracker::Ref pHandTracker;
	decMatrix pMatrixWristToDevice;
	bool pEnableTwoFingerTriggerSimulation;
	
	deoxrFaceTracker::Ref pFaceTracker;

	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	deoxrDevice( deVROpenXR &oxr, const deoxrDeviceProfile &profile );
	
protected:
	/** Clean up device. */
	virtual ~deoxrDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Oxr. */
	inline deVROpenXR &GetOxr() const{ return pOxr; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	/** Profile. */
	inline const deoxrDeviceProfile &GetProfile() const{ return pProfile; }
	
	/** Subaction path. */
	inline const deoxrPath &GetSubactionPath() const{ return pSubactionPath; }
	
	/** Set subaction path. */
	void SetSubactionPath( const deoxrPath &path );
	
	/** Pose action. */
	inline const deoxrAction::Ref &GetActionPose() const{ return pActionPose; }
	
	/** Set pose action. */
	void SetActionPose( deoxrAction *action );
	
	/** Orientation pose action. */
	inline const deoxrAction::Ref &GetActionPoseOrientation() const{ return pActionPoseOrientation; }
	
	/** Set orientation pose action. */
	void SetActionPoseOrientation( deoxrAction *action );
	
	/** Pose space. */
	inline const deoxrSpace::Ref &GetSpacePose() const{ return pSpacePose; }
	
	/** Set pose space. */
	void SetSpacePose( deoxrSpace *space );
	
	/** Pose space orientation. */
	inline const deoxrSpace::Ref &GetSpacePoseOrientation() const{ return pSpacePoseOrientation; }
	
	/** Set pose space orientation. */
	void SetSpacePoseOrientation( deoxrSpace *space );
	
	
	
	/** Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{ return pType; }
	
	/** Set device type. */
	void SetType( deInputDevice::eDeviceTypes type );
	
	/** Name number or -1 it not set. */
	inline int GetNameNumber() const{ return pNameNumber; }
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Bone configuration. */
	inline deInputDevice::eBoneConfigurations GetBoneConfiguration() const{ return pBoneConfiguration; }
	
	/** Set bone configuration. */
	void SetBoneConfiguration( deInputDevice::eBoneConfigurations config );
	
	
	
	/** Number of buttons. */
	int GetButtonCount() const;
	
	/** Add button. */
	void AddButton( deoxrDeviceButton *button );
	
	/** Add button. */
	void AddButton( deInputDeviceButton::eButtonTypes type, deoxrDeviceComponent *component,
		deVROpenXR::eInputActions actionPress, deVROpenXR::eInputActions actionTouch,
		const char *name, const char *id, const char *displayText );
	
	/** Add button. */
	void AddButton( deInputDeviceButton::eButtonTypes type, deoxrDeviceComponent *component,
		deVROpenXR::eInputActions actionPress, deVROpenXR::eInputActions actionTouch,
		deVROpenXR::eInputActions actionNear, const char *name, const char *id,
		const char *displayText );
	
	/** Button at index. */
	deoxrDeviceButton *GetButtonAt( int index ) const;
	
	/** Button with identifier or nullptr if absent. */
	deoxrDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	
	
	/** Number of axiss. */
	int GetAxisCount() const;
	
	/** Add axis. */
	void AddAxis( deoxrDeviceAxis *axis );
	
	/** Add trigger axis. */
	void AddAxisTrigger( deInputDeviceAxis::eAxisTypes type, deoxrDeviceComponent *component,
		deVROpenXR::eInputActions actionAnalog, const char *name,
		const char *id, const char *displayText );
	
	/** Add finger axis. */
	void AddAxisFinger( deInputDeviceAxis::eAxisTypes type, deoxrDeviceComponent *component,
		int finger, const char *name, const char *id, const char *displayText );
	
	/** Add joystick axis. */
	void AddAxesJoystick( deoxrDeviceComponent *component, deVROpenXR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	/** Add trackpad axis. */
	void AddAxesTrackpad( deoxrDeviceComponent *component, deVROpenXR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	/** Axis at index. */
	deoxrDeviceAxis *GetAxisAt( int index ) const;
	
	/** Axis with identifier or nullptr if absent. */
	deoxrDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	
	
	/** Number of feedbacks. */
	int GetFeedbackCount() const;
	
	/** Add feedback. */
	void AddFeedback( deoxrDeviceFeedback *feedback );
	
	/** Feedback at index. */
	deoxrDeviceFeedback *GetFeedbackAt( int index ) const;
	
	/** Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID( const char *id ) const;
	
	
	
	/** Number of components. */
	int GetComponentCount() const;
	
	/** Add component. */
	void AddComponent( deoxrDeviceComponent *component );
	
	/** Add component. */
	deoxrDeviceComponent *AddComponent( deInputDeviceComponent::eComponentTypes type,
		const char *name, const char *id, const char *displayText );
	
	/** Component at index. */
	deoxrDeviceComponent *GetComponentAt( int index ) const;
	
	/** Index of component with identifier or -1 if absent. */
	int IndexOfComponentWithID( const char *id ) const;
	
	
	
	/** Hand tracker or nullptr. */
	inline const deoxrHandTracker::Ref &GetHandTracker() const{ return pHandTracker; }
	
	/** Set hand tracker or nullptr. */
	void SetHandTracker( deoxrHandTracker *handTracker );
	
	/** Matrix transforming from wrist space to device space. */
	inline const decMatrix &GetMatrixWristToDevice() const{ return pMatrixWristToDevice; }
	
	/** Set matrix transforming from wrist space to device space. */
	void SetMatrixWristToDevice( const decMatrix &matrix );
	
	/** Enable trigger input simulation using two-finger input of index finger. */
	inline bool GetEnableTwoFingerTriggerSimulation() const{
		return pEnableTwoFingerTriggerSimulation; }
	
	/** Set enable trigger input simulation using two-finger input of index finger. */
	void SetEnableTwoFingerTriggerSimulation(bool enable);
	
	
	/** Face tracker or nullptr. */
	inline const deoxrFaceTracker::Ref &GetFaceTracker() const{ return pFaceTracker; }
	
	/** Set face tracker or nullptr. */
	void SetFaceTracker( deoxrFaceTracker *faceTracker );
	
	
	
	/** Update engine input device information. */
	void GetInfo( deInputDevice &info ) const;
	
	/** Update device parameters. */
	void UpdateParameters();
	
	/** Track states. */
	void TrackStates();
	
	/** Reset states. */
	void ResetStates();
	
	/** Get device pose. */
	void GetDevicePose( deInputDevicePose &pose );

	/** Direct device pose access for internal use. */
	inline const deInputDevicePose &GetDirectDevicePose() const{ return pPoseDevice; }
	
	/** Reference space changed. */
	void ReferenceSpaceChanged();
	/*@}*/
};

#endif
