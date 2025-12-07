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

#ifndef _DEVROPENXR_H_
#define _DEVROPENXR_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>
#include <dragengine/threading/deMutex.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

#include "deoxrInstance.h"
#include "deoxrSystem.h"
#include "deoxrSession.h"
#include "deoxrPassthrough.h"
#include "action/deoxrActionSet.h"
#include "device/deoxrDeviceManager.h"
#include "device/profile/deoxrDeviceProfileManager.h"
#include "graphicapi/deoxrGraphicApiOpenGL.h"
#include "parameters/deoxrParameterList.h"

/** input module device identifier prefix. */
#define OXR_DEVID_PREFIX "OXR_"

class deInputEvent;
class deoxrLoader;
class deoxrAction;
class deoxrSwapchain;
class deoxrThreadSync;


/**
 * OpenXR VR Module.
 */
class deVROpenXR : public deBaseVRModule{
public:
	/** Input actions. */
	enum eInputActions{
		eiaTriggerPress,
		eiaTriggerForce,
		eiaTriggerTouch,
		eiaTriggerAnalog,
		eiaTriggerHaptic,
		eiaTriggerCurl,
		eiaTriggerSlide,
		eiaTriggerNear,
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
		eiaThumbrestPress,
		eiaThumbrestNear,
		eiaThumbrestHaptic,
		eiaGripPress,
		eiaGripTouch,
		eiaGripGrab,
		eiaGripSqueeze,
		eiaGripPinch,
		eiaGripHaptic,
		eiaGesturePinch,
		eiaGestureAim,
		eiaGestureGrasp,
		eiaPose,
		eiaPoseLeft,
		eiaPoseRight,
		eiaPoseLeft2,
		eiaPoseRight2
	};
	
	static const int InputActionCount = eiaPoseRight2 + 1;
	
	enum class LogLevel{
		error,
		warning,
		info,
		debug
	};
	
	
private:
	deoxrDeviceProfileManager pDeviceProfiles;
	deoxrDeviceManager pDevices;
	
	deoxrGraphicApiOpenGL pGraphicApiOpenGL;
	
	deoxrLoader *pLoader;
	deoxrInstance::Ref pInstance;
	deoxrSystem::Ref pSystem;
	deoxrSession::Ref pSession;
	deoxrActionSet::Ref pActionSet;
	deoxrPassthrough::Ref pPassthrough;
	
	deoxrAction *pActions[InputActionCount];
	
	deCamera::Ref pCamera;
	deMutex pMutexOpenXR;
	XrSessionState pSessionState;
	bool pShutdownRequested;
	bool pPreventDeletion;
	bool pRestartSession;
	deoxrSystem::eSystem pLastDetectedSystem;
	deoxrThreadSync *pThreadSync;
	
	eFeatureSupportLevel pRequestFeatureEyeGazeTracking;
	eFeatureSupportLevel pRequestFeatureFacialTracking;
	
	LogLevel pLogLevel;
	
	deoxrParameterList pParameters;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create null VR OpenXR. */
	deVROpenXR(deLoadableModule &loadableModule);
	
	/** Clean up null VR OpenXR. */
	virtual ~deVROpenXR();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Loader. */
	inline deoxrLoader *GetLoader() const{return pLoader;}
	
	/** Device profiles manager. */
	inline deoxrDeviceProfileManager &GetDeviceProfiles(){return pDeviceProfiles;}
	inline const deoxrDeviceProfileManager &GetDeviceProfiles() const{return pDeviceProfiles;}
	
	/** Device manager. */
	inline deoxrDeviceManager &GetDevices(){return pDevices;}
	inline const deoxrDeviceManager &GetDevices() const{return pDevices;}
	
	/** Instance or nullptr. */
	inline const deoxrInstance::Ref &GetInstance() const{return pInstance;}
	
	/** System or nullptr. */
	inline const deoxrSystem::Ref &GetSystem() const{return pSystem;}
	
	/** Session or nullptr. */
	inline const deoxrSession::Ref &GetSession() const{return pSession;}
	
	/** Action set or nullptr. */
	inline const deoxrActionSet::Ref &GetActionSet() const{return pActionSet;}
	
	/** Passthrough or nullptr. */
	inline const deoxrPassthrough::Ref &GetPassthrough() const{return pPassthrough;}
	
	/** Action. */
	inline deoxrAction *GetAction(eInputActions inputAction) const{return pActions[inputAction];}
	
	/** Send event. */
	void SendEvent(const deInputEvent &event);
	
	/** Set input event timestamp. */
	void InputEventSetTimestamp(deInputEvent &event) const;
	
	/** Graphic api OpenGL. */
	inline deoxrGraphicApiOpenGL &GetGraphicApiOpenGL(){return pGraphicApiOpenGL;}
	inline const deoxrGraphicApiOpenGL &GetGraphicApiOpenGL() const{return pGraphicApiOpenGL;}
	
	/** Wait until ready exit. */
	void WaitUntilReadyExit();
	
	/** Session swapchain for eye or nullptr. */
	deoxrSwapchain *GetEyeSwapchain(eEye eye) const;
	
	/** Session state. */
	inline XrSessionState GetSessionState() const{return pSessionState;}
	
	/** Shutdown requested. */
	inline bool GetShutdownRequested() const{return pShutdownRequested;}
	
	/** Prevent deletion. */
	inline bool GetPreventDeletion() const{return pPreventDeletion;}
	
	/** Session restart is pending. */
	inline bool GetRestartSession() const{return pRestartSession;}
	
	/** Request session restart the next time possible. */
	void RequestRestartSession();
	
	/** Last detected system. */
	inline deoxrSystem::eSystem GetLastDetectedSystem() const{return pLastDetectedSystem;}
	
	/** Direct mutex access for special use. */
	inline deMutex &GetMutexOpenXR(){return pMutexOpenXR;}
	
	/** Requested feature levels. */
	inline eFeatureSupportLevel GetRequestFeatureEyeGazeTracking() const{return pRequestFeatureEyeGazeTracking;}
	inline eFeatureSupportLevel GetRequestFeatureFacialTracking() const{return pRequestFeatureFacialTracking;}
	
	/** Log level. */
	inline LogLevel GetLogLevel() const{return pLogLevel;}
	void SetLogLevel(LogLevel level){pLogLevel = level;}
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
	
	/** Set feature request level for eye gaze tracking. */
	virtual void RequestFeatureEyeGazeTracking(eFeatureSupportLevel level);
	
	/** Set feature request level for facial tracking. */
	virtual void RequestFeatureFacialTracking(eFeatureSupportLevel level);
	
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
	inline deCamera *GetCamera() const{return pCamera;}
	
	/** Set camera to render on head mounted display. */
	virtual void SetCamera(deCamera *camera);
	
	/** VR Runtime supports presenting user environment inside the rendered world. */
	virtual bool SupportsPassthrough();
	
	/** Enable presenting user environment inside the rendered world. */
	virtual void SetEnablePassthrough(bool enable);
	
	/** Set transparency of user environment presented inside the rendered world. */
	virtual void SetPassthroughTransparency(float transparency);
	
	/**
	 * \brief Center playspace with forward direction matching looking direction.
	 * \version 1.28
	 * 
	 * Playspace is not automatically centered after starting the VR runtime. Call this function
	 * any time later to center the playspace, for example if the player adjusted seating position
	 * or if the VR runtime uses a broken playspace orientation.
	 */
	virtual void CenterPlayspace();
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** Number of input devices. */
	virtual int GetDeviceCount();
	
	/** Information for input device at index. */
	virtual deInputDevice *GetDeviceAt(int index);
	
	/** Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID(const char *id);
	
	/** Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID(int device, const char *id);
	
	/** Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID(int device, const char *id);
	
	/** Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID(int device, const char *id);
	
	/** Index of component with identifier on device at index or -1 if absent. */
	virtual int IndexOfComponentWithID(int device, const char *id);
	
	/** Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed(int device, int button);
	
	/** Button at index on device at index is touched. */
	virtual bool GetButtonTouched(int device, int button);
	
	/** User finger is near button at index on device at index. */
	virtual bool GetButtonNear(int device, int button);
	
	/** Value of axis at index on device at index. */
	virtual float GetAxisValue(int device, int axis);
	
	/** Value of feedback at index on device at index. */
	virtual float GetFeedbackValue(int device, int feedback);
	
	/** Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue(int device, int feedback, float value);
	
	/** Device pose or identity if not supported. */
	virtual void GetDevicePose(int device, deInputDevicePose &pose);
	
	/** Device bone pose or identity if not supported. */
	virtual void GetDeviceBonePose(int device, int bone,
		bool withController, deInputDevicePose &pose);
	
	/** Device face expression or 0 if not supported. */
	virtual float GetDeviceFaceExpression(int device, int expression);
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
	
	/** VR required render format. */
	virtual eVRRenderFormat GetRenderFormat();
	
	/** VR render projection matrix parameters. */
	virtual void GetProjectionParameters(eEye eye, float &left, float &right, float &top, float &bottom);
	
	/** VR render matrix transforming from camera space to eye space. */
	virtual decMatrix GetMatrixViewEye(eEye eye);
	
	/** VR render hidden area model or nullptr if not supported. */
	virtual deModel *GetHiddenArea(eEye eye);
	
	/** VR render distortion image or nullptr if not supported. */
	virtual deImage *GetDistortionMap(eEye eye);
	
	/** Get eye view images to use for rendering. */
	virtual int GetEyeViewImages(eEye eye, int count, void *views);
	
	/** Get eye view render texture coordinates. */
	virtual void GetEyeViewRenderTexCoords(eEye eye, decVector2 &tcFrom, decVector2 &tcTo);
	
	/** Start begin frame. */
	virtual void StartBeginFrame();
	
	/** Wait for begin frame to be finished. */
	virtual void WaitBeginFrameFinished();
	
	/** Acquire eye view image to render into. */
	virtual int AcquireEyeViewImage(eEye eye);
	
	/** Release eye view image after render into. */
	virtual void ReleaseEyeViewImage(eEye eye);
	
	/** Submit OpenGL rendered image to the HMD. */
	virtual void SubmitOpenGLTexture2D(eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied);
	
	/** End frame. */
	virtual void EndFrame();
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	virtual void GetParameterInfo(int index, deModuleParameter &parameter) const;
	
	/** Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed(const char *name) const;
	
	/** Value of named parameter. */
	virtual decString GetParameterValue(const char *name) const;
	
	/** Set value of named parameter. */
	virtual void SetParameterValue(const char *name, const char *value);
	/*@}*/
	
	
	
private:
	void pRealShutdown();
	void pCreateActionSet();
	void pDestroyActionSet();
	void pCreateDeviceProfiles();
	void pSuggestBindings();
	bool pBeginFrame();
	void pCreateParameters();
};

#endif
