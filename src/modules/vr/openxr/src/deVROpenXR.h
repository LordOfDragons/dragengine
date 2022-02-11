/* 
 * Drag[en]gine OpenXR VR Module
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

#ifndef _DEVROPENXR_H_
#define _DEVROPENXR_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>
#include <dragengine/threading/deMutex.h>

#include "deoxrInstance.h"
#include "deoxrSystem.h"
#include "deoxrSession.h"
#include "action/deoxrActionSet.h"
#include "device/deoxrDeviceManager.h"
#include "device/profile/deoxrDeviceProfileManager.h"
#include "graphicapi/deoxrGraphicApiOpenGL.h"

/** input module device identifier prefix. */
#define OXR_DEVID_PREFIX "OXR_"

class deInputEvent;
class deoxrLoader;
class deoxrAction;
class deoxrSwapchain;


/**
 * OpenXR VR Module.
 */
class deVROpenXR : public deBaseVRModule{
public:
	/** Input actions. */
	enum eInputActions{
		eiaTriggerPress,
		eiaTriggerTouch,
		eiaTriggerAnalog,
		eiaTriggerHaptic,
		eiaButtonPrimaryPress,
		eiaButtonPrimaryTouch,
		eiaButtonSecondaryPress,
		eiaButtonSecondaryTouch,
		eiaButtonAuxiliary1Press,
		eiaButtonAuxiliary1Touch,
		eiaButtonAuxiliary2Press,
		eiaButtonAuxiliary2Touch,
		eiaJoystickPress,
		eiaJoystickTouch,
		eiaJoystickAnalog,
		eiaTrackpadPress,
		eiaTrackpadTouch,
		eiaTrackpadAnalog,
		eiaThumbrestTouch,
		eiaGripPress,
		eiaGripTouch,
		eiaGripGrab,
		eiaGripSqueeze,
		eiaGripPinch,
		eiaGripHaptic,
		eiaPose,
		eiaPoseLeft,
		eiaPoseRight,
		eiaSkeletonHandLeft,
		eiaSkeletonHandRight
	};
	
	static const int InputActionCount = eiaSkeletonHandLeft + 1;
	
	
	
private:
	deoxrDeviceProfileManager pDeviceProfiles;
	deoxrDeviceManager pDevices;
	
	deoxrGraphicApiOpenGL pGraphicApiOpenGL;
	
	deoxrLoader *pLoader;
	deoxrInstance::Ref pInstance;
	deoxrSystem::Ref pSystem;
	deoxrSession::Ref pSession;
	deoxrActionSet::Ref pActionSet;
	
	deoxrAction *pActions[ InputActionCount ];
	
	deCamera::Ref pCamera;
	deMutex pMutexOpenXR;
	XrSessionState pSessionState;
	bool pShutdownRequested;
	bool pPreventDeletion;
	deoxrSystem::eSystem pLastDetectedSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create null VR OpenXR. */
	deVROpenXR( deLoadableModule &loadableModule );
	
	/** Clean up null VR OpenXR. */
	virtual ~deVROpenXR();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Loader. */
	inline deoxrLoader *GetLoader() const{ return pLoader; }
	
	/** Device profiles manager. */
	inline deoxrDeviceProfileManager &GetDeviceProfiles(){ return pDeviceProfiles; }
	inline const deoxrDeviceProfileManager &GetDeviceProfiles() const{ return pDeviceProfiles; }
	
	/** Device manager. */
	inline deoxrDeviceManager &GetDevices(){ return pDevices; }
	inline const deoxrDeviceManager &GetDevices() const{ return pDevices; }
	
	/** Instance or nullptr. */
	inline const deoxrInstance::Ref &GetInstance() const{ return pInstance; }
	
	/** System or nullptr. */
	inline const deoxrSystem::Ref &GetSystem() const{ return pSystem; }
	
	/** Session or nullptr. */
	inline const deoxrSession::Ref &GetSession() const{ return pSession; }
	
	/** Action set or nullptr. */
	inline const deoxrActionSet::Ref &GetActionSet() const{ return pActionSet; }
	
	/** Action. */
	inline deoxrAction *GetAction( eInputActions inputAction ) const{ return pActions[ inputAction ]; }
	
	/** Send event. */
	void SendEvent( const deInputEvent &event );
	
	/** Set input event timestamp. */
	void InputEventSetTimestamp( deInputEvent &event ) const;
	
	/** Graphic api OpenGL. */
	inline deoxrGraphicApiOpenGL &GetGraphicApiOpenGL(){ return pGraphicApiOpenGL; }
	inline const deoxrGraphicApiOpenGL &GetGraphicApiOpenGL() const{ return pGraphicApiOpenGL; }
	
	/** Wait until ready exit. */
	void WaitUntilReadyExit();
	
	/** Session swapchain for eye or nullptr. */
	deoxrSwapchain *GetEyeSwapchain( eEye eye ) const;
	
	/** Session state. */
	inline XrSessionState GetSessionState() const{ return pSessionState; }
	
	/** Shutdown requested. */
	inline bool GetShutdownRequested() const{ return pShutdownRequested; }
	
	/** Prevent deletion. */
	inline bool GetPreventDeletion() const{ return pPreventDeletion; }
	
	/** Last detected system. */
	deoxrSystem::eSystem GetLastDetectedSystem() const{ return pLastDetectedSystem; }
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
	
	/** Camera or nullptr. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/** Set camera to render on head mounted display. */
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
	
	/** Index of component with identifier on device at index or -1 if absent. */
	virtual int IndexOfComponentWithID( int device, const char *id );
	
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
	virtual void GetDeviceBonePose( int device, int bone,
		bool withController, deInputDevicePose &pose );
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
	
	
	
	/** \name Graphic Module use only */
	/*@{*/
	/** VR recommended render target size. */
	virtual decPoint GetRenderSize();
	
	/** VR render projection matrix parameters. */
	virtual void GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom );
	
	/** VR render matrix transforming from camera space to eye space. */
	virtual decMatrix GetMatrixViewEye( eEye eye );
	
	/** VR render hidden area model or nullptr if not supported. */
	virtual deModel *GetHiddenArea( eEye eye );
	
	/** VR render distortion image or nullptr if not supported. */
	virtual deImage *GetDistortionMap( eEye eye );
	
	/** Get eye view images to use for rendering. */
	virtual int GetEyeViewImages( eEye eye, int count, void *views );
	
	/** \brief Get eye view render texture coordinates. */
	virtual void GetEyeViewRenderTexCoords( eEye eye, decVector2 &tcFrom, decVector2 &tcTo );
	
	/** Begin frame. */
	virtual void BeginFrame();
	
	/** \brief Acquire eye view image to render into. */
	virtual int AcquireEyeViewImage( eEye eye );
	
	/** Release eye view image after render into. */
	virtual void ReleaseEyeViewImage( eEye eye );
	
	/** Submit OpenGL rendered image to the HMD. */
	virtual void SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied );
	
	/** End frame. */
	virtual void EndFrame();
	/*@}*/
	
	
	
private:
	void pRealShutdown();
	void pCreateActionSet();
	void pDestroyActionSet();
	void pCreateDeviceProfiles();
	void pSuggestBindings();
};

#endif
