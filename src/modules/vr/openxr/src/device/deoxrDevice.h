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

#ifndef _DEOXRDEVICE_H_
#define _DEOXRDEVICE_H_

#include "../deVROpenXR.h"
#include "../deoxrPath.h"
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
	
	deoxrAction::Ref pActionPose;
	deoxrAction::Ref pActionHandPose;
	decString pSerialNumber;
	
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
	
	int pBoneCount;
	
	deInputDevicePose pPoseDevice;
	deInputDevicePose *pPoseBones;
	int pPoseBoneCount;
	
	
	
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
	
	/** Pose action handle. */
	inline deoxrAction *GetActionPose() const{ return pActionPose; }
	
	/** Hand pose action handle. */
	inline deoxrAction *GetActionHandPose() const{ return pActionHandPose; }
	
	
	
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
	
	/** Component at index. */
	deoxrDeviceComponent *GetComponentAt( int index ) const;
	
	/** Index of component with identifier or -1 if absent. */
	int IndexOfComponentWithID( const char *id ) const;
	
	
	
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
	
	/** Get bone pose. */
	void GetBonePose( int bone, bool withController, deInputDevicePose &pose );
	/*@}*/
	
	
	
private:
	void pUpdateParametersHMD();
	void pUpdateParametersController();
	void pUpdateParametersHandPose();
	void pUpdateParametersTracker();
	
	deoxrDeviceComponent *pAddComponent( deInputDeviceComponent::eComponentTypes type,
		const char *name, const char *id, const char *displayText );
	
	int pAddButton( deInputDeviceButton::eButtonTypes type, deoxrDeviceComponent *component,
		deVROpenXR::eInputActions actionPress, deVROpenXR::eInputActions actionTouch,
		const char *name, const char *id, const char *displayText );
	
	int pAddAxisTrigger( deInputDeviceAxis::eAxisTypes type, deVROpenXR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	void pAddAxisFinger( deInputDeviceAxis::eAxisTypes type, deoxrDeviceComponent *component,
		int finger, const char *name, const char *id, const char *displayText );
	
	int pAddAxesJoystick( deVROpenXR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	int pAddAxesTrackpad( deVROpenXR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
// 	void pUpdatePose( const vr::TrackedDevicePose_t &in, deInputDevicePose &out ) const;
// 	void pUpdatePose( const vr::VRBoneTransform_t &in, deInputDevicePose &out ) const;
};

#endif
