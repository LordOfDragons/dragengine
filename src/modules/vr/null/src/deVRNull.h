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

#ifndef _DEVRNULL_H_
#define _DEVRNULL_H_

#include <dragengine/systems/modules/vr/deBaseVRModule.h>



/**
 * Null VR Module.
 */
class deVRNull : public deBaseVRModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create null VR module. */
	deVRNull(deLoadableModule &loadableModule);
	
	/** Clean up null VR module. */
	~deVRNull() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * Init the module.
	 * \returns true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	bool Init() override;
	
	/** Clean up module. */
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
	
	/**
	 * Start VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	void StartRuntime() override;
	
	/**
	 * Stop VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	void StopRuntime() override;
	
	/**
	 * \brief VR runtime is running.
	 * \version 1.26
	 */
	bool IsRuntimeRunning() override;
	
	/**
	 * Set camera to render on head mounted display.
	 * 
	 * If set to nullptr fades back to safe scene as defined by VR Runtime.
	 */
	void SetCamera(deCamera *camera) override;
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
	
	/** \brief Index of component with identifier on device at index or -1 if absent. */
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
	
	/** VR render projection matrix parameters. */
	void GetProjectionParameters(eEye eye, float &left,
		float &right, float &top, float &bottom) override;
	
	/** VR render matrix transforming from camera space to eye space. */
	decMatrix GetMatrixViewEye(eEye eye) override;
	
	/** VR render hidden area model or nullptr if not supported. */
	deModel *GetHiddenArea(eEye eye) override;
	
	/** VR render distortion image or nullptr if not supported. */
	deImage *GetDistortionMap(eEye eye) override;
	
	/** Start begin frame. */
	void StartBeginFrame() override;
	
	/** Wait for begin frame to be finished. */
	void WaitBeginFrameFinished() override;
	
	/** Submit OpenGL rendered image to the HMD. */
	void SubmitOpenGLTexture2D(eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied) override;
	
	/** End frame. */
	void EndFrame() override;
	/*@}*/
};

#endif
