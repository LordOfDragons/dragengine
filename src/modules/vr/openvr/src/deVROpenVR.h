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

#ifndef _DEVROPENVR_H_
#define _DEVROPENVR_H_

#include "deovrDeviceManager.h"
#include "deovrHiddenMesh.h"

#include <openvr/openvr.h>

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>
#include <dragengine/threading/deMutex.h>


/** input module device identifier prefix. */
#define OVR_DEVID_PREFIX "OVR_"

class deInputEvent;
class deovrRenderModel;
class deovrTextureMap;


/**
 * OpenVR VR Module.
 */
class deVROpenVR : public deBaseVRModule{
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
		eiaJoystickPress,
		eiaJoystickTouch,
		eiaJoystickAnalog,
		eiaTrackpadPress,
		eiaTrackpadTouch,
		eiaTrackpadAnalog,
		eiaGripPress,
		eiaGripTouch,
		eiaGripGrab,
		eiaGripSqueeze,
		eiaGripPinch,
		eiaGripHaptic,
		eiaPose,
		eiaSkeletonHandRight,
		eiaSkeletonHandLeft
	};
	
	static const int InputActionCount = eiaSkeletonHandLeft + 1;
	
	
	
private:
	bool pRuntimeInstalled;
	decString pPathRuntime;
	
	deCamera::Ref pCamera;
	
	deovrDeviceManager pDevices;
	decObjectDictionary pRenderModels;
	decObjectList pTextureMaps;
	
	vr::IVRSystem *pVRSystem;
	vr::IVRInput *pVRInput;
	vr::IVRRenderModels *pVRRenderModels;
	vr::IVRCompositor *pVRCompositor;
	vr::VRActionSetHandle_t pActionSetHandle;
	vr::VRActionHandle_t pActionHandle[ InputActionCount ];
	
	vr::TrackedDevicePose_t pDevicePoses[ vr::k_unMaxTrackedDeviceCount ];
	deMutex pMutexDevicePoses;
	
	deovrHiddenMesh::Ref pHiddenMeshLeftEye;
	deovrHiddenMesh::Ref pHiddenMeshRightEye;
	
	
	
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
	vr::IVRSystem &GetVRSystem() const;
	
	/** VR System. */
	vr::IVRInput &GetVRInput() const;
	
	/** VR Render Models. */
	vr::IVRRenderModels &GetVRRenderModels() const;
	
	/** VR Compositor. */
	vr::IVRCompositor &GetVRCompositor() const;
	
	/** VR Action Set Handle. */
	inline vr::VRActionSetHandle_t GetActionSetHandle() const{ return pActionSetHandle; }
	
	/** VR Action Handle. */
	inline vr::VRActionHandle_t GetActionHandle( eInputActions action ) const{ return pActionHandle[ action ]; }
	
	/** Get render model loading it if required. */
	deovrRenderModel *GetRenderModelNamed( const char *name );
	
	/** Get texture map loading it if required. */
	deovrTextureMap *GetTextureMapWithID( vr::TextureID_t id );
	
	/** Send event. */
	void SendEvent( const deInputEvent &event );
	
	/** Convert matrix. */
	decMatrix ConvertMatrix( const vr::HmdMatrix34_t &matrix ) const;
	
	/** Copy device poses. */
	void CopyDevicesPoses( vr::TrackedDevicePose_t *poses );
	
	/** Set input event timestamp. */
	void InputEventSetTimestamp( deInputEvent &event ) const;
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
	 * \brief VR runtime is running.
	 * \version 1.26
	 */
	virtual bool IsRuntimeRunning();
	
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
	
	/** User finger is near button at index on device at index. */
	virtual bool GetButtonNear(int device, int button);
	
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
	
	/** \brief VR required render format. */
	virtual eVRRenderFormat GetRenderFormat();
	
	/** VR render projection matrix parameters. */
	virtual void GetProjectionParameters( eEye eye, float &left,
		float &right, float &top, float &bottom );
	
	/** VR render matrix transforming from camera space to eye space. */
	virtual decMatrix GetMatrixViewEye( eEye eye );
	
	/** VR render hidden area model or nullptr if not supported. */
	virtual deModel *GetHiddenArea( eEye eye );
	
	/** VR render distortion image or nullptr if not supported. */
	virtual deImage *GetDistortionMap( eEye eye );
	
	/** Start begin frame. */
	virtual void StartBeginFrame();
	
	/** Wait for begin frame to be finished. */
	virtual void WaitBeginFrameFinished();
	
	/** Submit OpenGL rendered image to the HMD. */
	virtual void SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied );
	
	/** End frame. */
	virtual void EndFrame();
	/*@}*/
	
	
	
private:
	vr::Hmd_Eye ConvertEye( eEye eye ) const;
};

#endif
