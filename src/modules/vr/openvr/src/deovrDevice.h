/* 
 * Drag[en]gine OpenVR VR Module
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

#ifndef _DEOVRDEVICE_H_
#define _DEOVRDEVICE_H_

#include "deVROpenVR.h"
#include "deovrRenderModel.h"
#include "deovrTextureMap.h"

#include <openvr/openvr.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/resources/image/deImage.h>

class deovrDeviceAxis;
class deovrDeviceButton;
class deovrDeviceFeedback;
class deovrDeviceComponent;
class deInputDevice;
class deInputDevicePose;


/**
 * VR input device.
 */
class deovrDevice : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deovrDevice> Ref;
	
	
	
private:
	deVROpenVR &pOvr;
	
	int pIndex;
	
	const vr::TrackedDeviceIndex_t pDeviceIndex;
	vr::TrackedDeviceClass pDeviceClass;
	vr::ETrackedControllerRole pControllerRole;
	decString  pInputValuePath;
	vr::VRInputValueHandle_t pInputValueHandle;
	vr::VRActionHandle_t pActionPose;
	vr::VRActionHandle_t pActionHandPose;
	decString pSerialNumber;
	
	deInputDevice::eDeviceTypes pType;
	deInputDevice::eBoneConfigurations pBoneConfiguration;
	int pNameNumber;
	decString pID;
	decString pName;
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	deovrRenderModel::Ref pRenderModel;
	deovrTextureMap::Ref pTextureMap;
	
	decObjectOrderedSet pButtons;
	decObjectOrderedSet pAxes;
	decObjectOrderedSet pFeedbacks;
	decObjectOrderedSet pComponents;
	
	vr::VRControllerState_t pState;
	vr::InputPoseActionData_t pDevicePoseData;
	vr::VRSkeletalSummaryData_t pSkeletalSummeryData;
	vr::VRBoneTransform_t *pBoneTransformData;
	int pBoneCount;
	
	deInputDevicePose pPoseDevice;
	deInputDevicePose *pPoseBones;
	int pPoseBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	deovrDevice( deVROpenVR &ovr, vr::TrackedDeviceIndex_t deviceIndex );
	
protected:
	/** Clean up device. */
	virtual ~deovrDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** OpenVR ovr. */
	inline deVROpenVR &GetOvr() const{ return pOvr; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	/** Device index. */
	inline vr::TrackedDeviceIndex_t GetDeviceIndex() const{ return pDeviceIndex; }
	
	/** Device class. */
	inline vr::TrackedDeviceClass GetDeviceClass() const{ return pDeviceClass; }
	
	/** Controller role. */
	inline vr::ETrackedControllerRole GetControllerRole() const{ return pControllerRole; }
	
	/** Input value path. */
	inline const decString &GetInputValuePath() const{ return pInputValuePath; }
	
	/** Input value handler. */
	inline vr::VRInputValueHandle_t GetInputValueHandle() const{ return pInputValueHandle; }
	
	/** Pose action handle. */
	inline vr::VRActionHandle_t GetActionPose() const{ return pActionPose; }
	
	/** Hand pose action handle. */
	inline vr::VRActionHandle_t GetActionHandPose() const{ return pActionHandPose; }
	
	
	
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
	
	/** Render model. */
	inline deovrRenderModel *GetRenderModel() const{ return pRenderModel; }
	
	/** Set render model. */
	void SetRenderModel( deovrRenderModel *renderModel );
	
	/** Texture map. */
	inline deovrTextureMap *GetTextureMap() const{ return pTextureMap; }
	
	/** Set texture map. */
	void SetTextureMap( deovrTextureMap *textureMap );
	
	
	
	/** Number of buttons. */
	int GetButtonCount() const;
	
	/** Add button. */
	void AddButton( deovrDeviceButton *button );
	
	/** Button at index. */
	deovrDeviceButton *GetButtonAt( int index ) const;
	
	/** Button with identifier or nullptr if absent. */
	deovrDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	
	
	/** Number of axiss. */
	int GetAxisCount() const;
	
	/** Add axis. */
	void AddAxis( deovrDeviceAxis *axis );
	
	/** Axis at index. */
	deovrDeviceAxis *GetAxisAt( int index ) const;
	
	/** Axis with identifier or nullptr if absent. */
	deovrDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	
	
	/** Number of feedbacks. */
	int GetFeedbackCount() const;
	
	/** Add feedback. */
	void AddFeedback( deovrDeviceFeedback *feedback );
	
	/** Feedback at index. */
	deovrDeviceFeedback *GetFeedbackAt( int index ) const;
	
	/** Index of feedback with identifier or -1 if absent. */
	int IndexOfFeedbackWithID( const char *id ) const;
	
	
	
	/** Number of components. */
	int GetComponentCount() const;
	
	/** Add component. */
	void AddComponent( deovrDeviceComponent *component );
	
	/** Component at index. */
	deovrDeviceComponent *GetComponentAt( int index ) const;
	
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
	
	/** State. */
	inline const vr::VRControllerState_t &GetState() const{ return pState; }
	
	/** Get device pose. */
	void GetDevicePose( deInputDevicePose &pose );
	
	/** Get bone pose. */
	void GetBonePose( int bone, bool withController, deInputDevicePose &pose );
	
	/** Skeletal summary data. */
	inline const vr::VRSkeletalSummaryData_t &GetSkeletalSummaryData() const{ return pSkeletalSummeryData; }
	/*@}*/
	
	
	
private:
	void pUpdateParametersHMD();
	void pUpdateParametersController();
	void pUpdateParametersHandPose( vr::VRActionHandle_t actionHandle );
	void pUpdateParametersTracker();
	
	deovrDeviceComponent *pAddComponent( deInputDeviceComponent::eComponentTypes type,
		const char *name, const char *id, const char *displayText );
	
	int pAddButton( deInputDeviceButton::eButtonTypes type, deovrDeviceComponent *component,
		deVROpenVR::eInputActions actionPress, deVROpenVR::eInputActions actionTouch,
		const char *name, const char *id, const char *displayText );
	
	int pAddAxisTrigger( deInputDeviceAxis::eAxisTypes type, deVROpenVR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	void pAddAxisFinger( deInputDeviceAxis::eAxisTypes type, deovrDeviceComponent *component,
		int finger, const char *name, const char *id, const char *displayText );
	
	int pAddAxesJoystick( deVROpenVR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	int pAddAxesTrackpad( deVROpenVR::eInputActions actionAnalog,
		const char *name, const char *id, const char *displayText );
	
	void pUpdatePose( const vr::TrackedDevicePose_t &in, deInputDevicePose &out ) const;
	void pUpdatePose( const vr::VRBoneTransform_t &in, deInputDevicePose &out ) const;
};

#endif
