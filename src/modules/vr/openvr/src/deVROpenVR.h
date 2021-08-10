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

#ifndef _DEVROPENVR_H_
#define _DEVROPENVR_H_

#include "deovrDeviceManager.h"

#include <openvr/openvr.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>


/** input module device identifier prefix. */
#define OVR_DEVID_PREFIX "OVR_"

class deInputEvent;


/**
 * OpenVR VR Module.
 */
class deVROpenVR : public deBaseVRModule{
private:
	bool pRuntimeInstalled;
	decString pPathRuntime;
	
	deovrDeviceManager pDevices;
	
	vr::IVRSystem *pSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create null VR ovr. */
	deVROpenVR( deLoadableModule &loadableModule );
	
	/** Clean up null VR ovr. */
	virtual ~deVROpenVR();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Devices. */
	inline deovrDeviceManager &GetDevices(){ return pDevices; }
	inline const deovrDeviceManager &GetDevices() const{ return pDevices; }
	
	/** VR System. */
	vr::IVRSystem &GetSystem() const;
	
	/** Send event. */
	void SendEvent( const deInputEvent &event );
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * Init the ovr.
	 * \returns true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	virtual bool Init();
	
	/** Clean up ovr. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Runtime */
	/*@{*/
	/**
	 * VR Runtime is usable.
	 * 
	 * Returns true if a call to StartRuntime() is likely to succeed or not.
	 */
	virtual bool RuntimeUsable();
	
	/**
	 * Start VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various ovrs. VR mode can be started and
	 * stopped at any time.
	 */
	virtual void StartRuntime();
	
	/**
	 * Stop VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various ovrs. VR mode can be started and
	 * stopped at any time.
	 */
	virtual void StopRuntime();
	
	/**
	 * Set camera to render on head mounted display.
	 * 
	 * If set to nullptr fades back to safe scene as defined by VR Runtime.
	 */
	virtual void SetCamera( deCamera *camera );
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** Number of input devices. */
	virtual int GetDeviceCount();
	
	/** Information for input device at index. */
	virtual deInputDevice *GetDeviceAt( int index );
	
	/** Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID( const char *id );
	
	/** Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID( int device, const char *id );
	
	/** Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID( int device, const char *id );
	
	/** Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID( int device, const char *id );
	
	/** Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed( int device, int button );
	
	/** Button at index on device at index is touched. */
	virtual bool GetButtonTouched( int device, int button );
	
	/** Value of axis at index on device at index. */
	virtual float GetAxisValue( int device, int axis );
	
	/** Value of feedback at index on device at index. */
	virtual float GetFeedbackValue( int device, int feedback );
	
	/** Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue( int device, int feedback, float value );
	
	/** \brief Device pose or identity if not supported. */
	virtual void GetDevicePose( int device, deInputDevicePose &pose );
	
	/** \brief Device bone pose or identity if not supported. */
	virtual void GetDeviceBonePose( int device, int bone, deInputDevicePose &pose );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/**
	 * Add events to the VR System event queue.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	virtual void ProcessEvents();
	/*@}*/
	
	
	
private:
	void pButtonPress( vr::TrackedDeviceIndex_t deviceIndex, vr::EVRButtonId buttonType );
	void pButtonRelease( vr::TrackedDeviceIndex_t deviceIndex, vr::EVRButtonId buttonType );
	void pButtonTouch( vr::TrackedDeviceIndex_t deviceIndex, vr::EVRButtonId buttonType );
	void pButtonUntouch( vr::TrackedDeviceIndex_t deviceIndex, vr::EVRButtonId buttonType );
};

#endif
