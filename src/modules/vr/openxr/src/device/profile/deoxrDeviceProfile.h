/* 
 * Drag[en]gine OpenXR
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

#ifndef _DEOXRDEVICEPROFILE_H_
#define _DEOXRDEVICEPROFILE_H_

#include "../deoxrDevice.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceButton.h"
#include "../../deoxrBasics.h"
#include "../../deoxrInstance.h"
#include "../../deoxrPath.h"
#include "../../deVROpenXR.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoxrInstance;
class deoxrSession;
class deoxrHandTracker;
class deoxrActionSet;


/**
 * Oxr device profile.
 * 
 * Used device prefixes:
 * - gdc : deoxrDPGoogleDaydreamController
 * - hci : deoxrDPHUAWEIControllerInteraction
 * - hmd : deoxrDPHMD
 * - hpmrc : deoxrDPHPMixedRealityController
 * - htcvcc : deoxrDPHTCViveCosmosControllerInteraction
 * - htcvfc : deoxrDPHTCViveFocus3ControllerInteraction
 * - ic : deoxrDPValveIndexController
 * - mmrmc : deoxrDPMicrosoftMixedRealityMotionController
 * - msfthc : deoxrDPMSFTHandInteraction
 * - nch : deoxrDPNoControllerHands
 * - ogc : deoxrDPOculusGoController
 * - otc : deoxrDPOculusTouchController
 * - sc : deoxrDPSimpleController
 * - soc : deoxrDPSamsungOdysseyController
 * - tr : deoxrDPHtcViveTracker
 * - vc : deoxrDPHTCViveController
 */
class deoxrDeviceProfile : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrDeviceProfile> Ref;
	
	
	
protected:
	enum eButtonLabel{
		eblA,
		eblB,
		eblX,
		eblY,
		eblHome,
		eblSystem,
		eblShoulder,
		eblBack,
		eblVolumeUp,
		eblVolumeDown
	};
	
	enum eButtonAction{
		ebaPrimary,
		ebaSecondary,
		ebaAuxiliary1,
		ebaAuxiliary2
	};
	
	
	
private:
	deoxrInstance &pInstance;
	
	const deoxrPath pPath;
	const decString pName;
	
	decVector pDeviceRotation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDeviceProfile( deoxrInstance &instance, const deoxrPath &path, const char *name );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDeviceProfile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** Path. */
	inline const deoxrPath &GetPath() const{ return pPath; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Device rotation. */
	inline const decVector &GetDeviceRotation() const{ return pDeviceRotation; }
	
	/** Set device rotation. */
	void SetDeviceRotation( const decVector &rotation );
	
	/** Create actions for action set. */
	virtual void CreateActions( deoxrActionSet &actionSet );
	
	/** On actions synced. */
	virtual void OnActionsSynced();
	
	/** On session state changed. */
	virtual void OnSessionStateChanged();
	
	/** Check attached. */
	virtual void CheckAttached();
	
	/** Suggest bindings. */
	virtual void SuggestBindings() = 0;
	
	/** Clear actions. */
	virtual void ClearActions();
	
	/** Remove device if matching type. */
	virtual void RemoveDevice( deInputDevice::eDeviceTypes type );
	/*@}*/
	
	
	
protected:
	deoxrSession *pGetSession() const;
	
	deVROpenXR::eInputActions pGripPoseAction( bool left ) const;
	
	void pAdd( deoxrInstance::sSuggestBinding *&bindings,
		deVROpenXR::eInputActions inputAction, const char *path ) const;
	
	bool pMatchesProfile( const deoxrPath &path ) const;
	
	/** Add device with basic parameters for left or right hand. */
	void pCreateDevice( deoxrDevice::Ref &device, bool leftHand, const char *idPrefix );
	
	/** Add component. */
	deoxrDeviceComponent *pAddComponent( deoxrDevice &device,
		deInputDeviceComponent::eComponentTypes type,
		const char *name, const char *id, const char *displayText );
	
	deoxrDeviceComponent *pAddComponentTrigger( deoxrDevice &device );
	void pAddAxisTrigger( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonTrigger( deoxrDevice &device, deoxrDeviceComponent *component, bool withTouch );
	
	deoxrDeviceComponent *pAddComponentTrackpad( deoxrDevice &device );
	void pAddAxesTrackpad( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonTrackpad( deoxrDevice &device, deoxrDeviceComponent *component, bool withPress, bool withTouch );
	
	deoxrDeviceComponent *pAddComponentJoystick( deoxrDevice &device );
	void pAddAxesJoystick( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonJoystick( deoxrDevice &device, deoxrDeviceComponent *component, bool withPress, bool withTouch );
	
	deoxrDeviceComponent *pAddComponentThumbrest( deoxrDevice &device );
	void pAddButtonThumbrest( deoxrDevice &device, deoxrDeviceComponent *component );
	
	deoxrDeviceComponent *pAddComponentGrip( deoxrDevice &device );
	void pAddAxesGripGrab( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddAxesGripSqueeze( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddAxesGripPinch( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonGrip( deoxrDevice &device, deoxrDeviceComponent *component, bool withTouch );
	
	void pAddButton( deoxrDevice &device, eButtonAction action, eButtonLabel label, bool withTouch );
	
	const char *pButtonName( eButtonLabel label ) const;
	const char *pButtonId( eButtonLabel label ) const;
	const char *pButtonDisplayText( eButtonLabel label ) const;
	
	deoxrHandTracker *pAddHandTracker( deoxrDevice &device, bool leftHand );
};

#endif

