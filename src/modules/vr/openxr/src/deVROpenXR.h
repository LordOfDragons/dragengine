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
	~deVROpenXR() override;
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
	
	/** Passthrough or nullptr. */
	inline const deoxrPassthrough::Ref &GetPassthrough() const{ return pPassthrough; }
	
	/** Action. */
	inline deoxrAction *GetAction(eInputActions inputAction) const{ return pActions[inputAction]; }
	
	/** Send event. */
	void SendEvent(const deInputEvent &event);
	
	/** Set input event timestamp. */
	void InputEventSetTimestamp(deInputEvent &event) const;
	
	/** Graphic api OpenGL. */
	inline deoxrGraphicApiOpenGL &GetGraphicApiOpenGL(){ return pGraphicApiOpenGL; }
	inline const deoxrGraphicApiOpenGL &GetGraphicApiOpenGL() const{ return pGraphicApiOpenGL; }
	
	/** Wait until ready exit. */
	void WaitUntilReadyExit();
	
	/** Session swapchain for eye or nullptr. */
	deoxrSwapchain *GetEyeSwapchain(eEye eye) const;
	
	/** Session state. */
	inline XrSessionState GetSessionState() const{ return pSessionState; }
	
	/** Shutdown requested. */
	inline bool GetShutdownRequested() const{ return pShutdownRequested; }
	
	/** Prevent deletion. */
	inline bool GetPreventDeletion() const{ return pPreventDeletion; }
	
	/** Session restart is pending. */
	inline bool GetRestartSession() const{ return pRestartSession; }
	
	/** Request session restart the next time possible. */
	void RequestRestartSession();
	
	/** Last detected system. */
	inline deoxrSystem::eSystem GetLastDetectedSystem() const{ return pLastDetectedSystem; }
	
	/** Direct mutex access for special use. */
	inline deMutex &GetMutexOpenXR(){ return pMutexOpenXR; }
	
	/** Requested feature levels. */
	inline eFeatureSupportLevel GetRequestFeatureEyeGazeTracking() const{ return pRequestFeatureEyeGazeTracking; }
	inline eFeatureSupportLevel GetRequestFeatureFacialTracking() const{ return pRequestFeatureFacialTracking; }
	
	/** Log level. */
	inline LogLevel GetLogLevel() const{ return pLogLevel; }
	void SetLogLevel(LogLevel level){pLogLevel = level;}
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * Init the ovr.
	 * \returns true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	bool Init() override;
	
	/** Clean up ovr. */
	void CleanUp() override;
	/*@}*/
	
	
	
	/** \name Runtime */
	/*@{*/
	/**
	 * VR Runtime is usable.
	 * 
	 * Returns true if a call to StartRuntime() is likely to succeed or not.
	 */
	bool RuntimeUsable() override;
	
	/** Set feature request level for eye gaze tracking. */
	void RequestFeatureEyeGazeTracking(eFeatureSupportLevel level) override;
	
	/** Set feature request level for facial tracking. */
	void RequestFeatureFacialTracking(eFeatureSupportLevel level) override;
	
	/**
	 * Start VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various ovrs. VR mode can be started and
	 * stopped at any time.
	 */
	void StartRuntime() override;
	
	/**
	 * Stop VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various ovrs. VR mode can be started and
	 * stopped at any time.
	 */
	void StopRuntime() override;
	
	/**
	 * \brief VR runtime is running.
	 * \version 1.26
	 */
	bool IsRuntimeRunning() override;
	
	/** Camera or nullptr. */
	inline const deCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** Set camera to render on head mounted display. */
	void SetCamera(deCamera *camera) override;
	
	/** VR Runtime supports presenting user environment inside the rendered world. */
	bool SupportsPassthrough() override;
	
	/** Enable presenting user environment inside the rendered world. */
	void SetEnablePassthrough(bool enable) override;
	
	/** Set transparency of user environment presented inside the rendered world. */
	void SetPassthroughTransparency(float transparency) override;
	
	/**
	 * \brief Center playspace with forward direction matching looking direction.
	 * \version 1.28
	 * 
	 * Playspace is not automatically centered after starting the VR runtime. Call this function
	 * any time later to center the playspace, for example if the player adjusted seating position
	 * or if the VR runtime uses a broken playspace orientation.
	 */
	void CenterPlayspace() override;
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** Number of input devices. */
	int GetDeviceCount() override;
	
	/** Information for input device at index. */
	deInputDevice::Ref GetDeviceAt(int index) override;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfDeviceWithID(const char *id) override;
	
	/** Index of button with identifier on device at index or -1 if absent. */
	int IndexOfButtonWithID(int device, const char *id) override;
	
	/** Index of axis with identifier on device at index or -1 if absent. */
	int IndexOfAxisWithID(int device, const char *id) override;
	
	/** Index of feedback with identifier on device at index or -1 if absent. */
	int IndexOfFeedbackWithID(int device, const char *id) override;
	
	/** Index of component with identifier on device at index or -1 if absent. */
	int IndexOfComponentWithID(int device, const char *id) override;
	
	/** Button at index on device at index is pressed down. */
	bool GetButtonPressed(int device, int button) override;
	
	/** Button at index on device at index is touched. */
	bool GetButtonTouched(int device, int button) override;
	
	/** User finger is near button at index on device at index. */
	bool GetButtonNear(int device, int button) override;
	
	/** Value of axis at index on device at index. */
	float GetAxisValue(int device, int axis) override;
	
	/** Value of feedback at index on device at index. */
	float GetFeedbackValue(int device, int feedback) override;
	
	/** Set value of feedback at index on device at index. */
	void SetFeedbackValue(int device, int feedback, float value) override;
	
	/** Device pose or identity if not supported. */
	void GetDevicePose(int device, deInputDevicePose &pose) override;
	
	/** Device bone pose or identity if not supported. */
	void GetDeviceBonePose(int device, int bone,
		bool withController, deInputDevicePose &pose) override;
	
	/** Device face expression or 0 if not supported. */
	float GetDeviceFaceExpression(int device, int expression) override;
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
	void ProcessEvents() override;
	/*@}*/
	
	
	
	/** \name Graphic Module use only */
	/*@{*/
	/** VR recommended render target size. */
	decPoint GetRenderSize() override;
	
	/** VR required render format. */
	eVRRenderFormat GetRenderFormat() override;
	
	/** VR render projection matrix parameters. */
	void GetProjectionParameters(eEye eye, float &left, float &right, float &top, float &bottom) override;
	
	/** VR render matrix transforming from camera space to eye space. */
	decMatrix GetMatrixViewEye(eEye eye) override;
	
	/** VR render hidden area model or nullptr if not supported. */
	deModel *GetHiddenArea(eEye eye) override;
	
	/** VR render distortion image or nullptr if not supported. */
	deImage *GetDistortionMap(eEye eye) override;
	
	/** Get eye view images to use for rendering. */
	int GetEyeViewImages(eEye eye, int count, void *views) override;
	
	/** Get eye view render texture coordinates. */
	void GetEyeViewRenderTexCoords(eEye eye, decVector2 &tcFrom, decVector2 &tcTo) override;
	
	/** Start begin frame. */
	void StartBeginFrame() override;
	
	/** Wait for begin frame to be finished. */
	void WaitBeginFrameFinished() override;
	
	/** Acquire eye view image to render into. */
	int AcquireEyeViewImage(eEye eye) override;
	
	/** Release eye view image after render into. */
	void ReleaseEyeViewImage(eEye eye) override;
	
	/** Submit OpenGL rendered image to the HMD. */
	void SubmitOpenGLTexture2D(eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied) override;
	
	/** End frame. */
	void EndFrame() override;
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const override;
	
	/**
	 * Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	void GetParameterInfo(int index, deModuleParameter &parameter) const override;
	
	/** Index of named parameter or -1 if not found. */
	int IndexOfParameterNamed(const char *name) const override;
	
	/** Value of named parameter. */
	decString GetParameterValue(const char *name) const override;
	
	/** Set value of named parameter. */
	void SetParameterValue(const char *name, const char *value) override;
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
