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
	
	void pHasHandDevices( deoxrDevice *deviceLeft, deoxrDevice *deviceRight,
		bool &hasLeft, bool &hasRight ) const;
	
	bool pHasAnyHandDevice( deoxrDevice *deviceLeft, deoxrDevice *deviceRight ) const;
	
	deVROpenXR::eInputActions pPoseAction( bool left ) const;
	deVROpenXR::eInputActions pPoseAction2( bool left ) const;
	
	void pAdd( deoxrInstance::sSuggestBinding *&bindings,
		deVROpenXR::eInputActions inputAction, const char *path ) const;
	
	bool pMatchesProfile( const deoxrPath &path ) const;
	
	/** Add device with basic parameters for left or right hand. */
	void pCreateDevice( deoxrDevice::Ref &device, bool leftHand,
		const char *idPrefix, bool withOrientationAction );
	
	/** Add component. */
	deoxrDeviceComponent *pAddComponent( deoxrDevice &device,
		deInputDeviceComponent::eComponentTypes type,
		const char *name, const char *id, const char *displayText );
	
	deoxrDeviceComponent *pAddComponentTrigger( deoxrDevice &device );
	void pAddAxisTrigger( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonTrigger(deoxrDevice &device, deoxrDeviceComponent *component,
		bool withTouch, bool withApproach = false);
	void pAddAxisTriggerForce(deoxrDevice &device, deoxrDeviceComponent *component);
	void pAddAxisTriggerCurl(deoxrDevice &device, deoxrDeviceComponent *component);
	void pAddAxisTriggerSlide(deoxrDevice &device, deoxrDeviceComponent *component);
	
	deoxrDeviceComponent *pAddComponentTrackpad( deoxrDevice &device );
	void pAddAxesTrackpad( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonTrackpad( deoxrDevice &device, deoxrDeviceComponent *component, bool withPress, bool withTouch );
	
	deoxrDeviceComponent *pAddComponentJoystick( deoxrDevice &device );
	void pAddAxesJoystick( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonJoystick( deoxrDevice &device, deoxrDeviceComponent *component, bool withPress, bool withTouch );
	
	deoxrDeviceComponent *pAddComponentThumbrest( deoxrDevice &device );
	void pAddButtonThumbrest(deoxrDevice &device, deoxrDeviceComponent *component,
		bool withPress = false, bool withApproach = false);
	void pAddAxesThumbrestPress(deoxrDevice &device, deoxrDeviceComponent *component);
	
	deoxrDeviceComponent *pAddComponentGrip( deoxrDevice &device );
	void pAddAxisGripGrab( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddAxisGripSqueeze( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddAxisGripPinch( deoxrDevice &device, deoxrDeviceComponent *component );
	void pAddButtonGrip( deoxrDevice &device, deoxrDeviceComponent *component, bool withTouch );
	
	void pAddButton( deoxrDevice &device, eButtonAction action, eButtonLabel label, bool withTouch );
	
	const char *pButtonName( eButtonLabel label ) const;
	const char *pButtonId( eButtonLabel label ) const;
	const char *pButtonDisplayText( eButtonLabel label ) const;
	
	deoxrHandTracker *pAddHandTracker(deoxrDevice &device, bool leftHand, bool withInputSimulation);
};

#endif

