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

#ifndef _DEXSYSTEMINPUT_H_
#define _DEXSYSTEMINPUT_H_

#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include "dexsiXInclude.h"
#include "dexsiDeviceManager.h"
#include "parameters/dexsiParameterList.h"

class deOSUnix;


/**
 * input module device identifier prefix.
 */
#define XINP_DEVID_PREFIX "XSys_"


/**
 * X-System input module.
 */
class deXSystemInput : public deBaseInputModule{
public:
	enum class LogLevel{
		error,
		warning,
		info,
		debug
	};
	
	
private:
	struct sKey{
		int button;
		int virtualKeyCode;
		KeySym keySym;
		int character;
	};
	
	deOSUnix *pOSUnix;
	
	int pWindowWidth;
	int pWindowHeight;
	
	int pMouseButtons;
	int pLastMouseX;
	int pLastMouseY;
	
	bool pIsListening;
	
	bool pSystemAutoRepeatEnabled;
	bool pAutoRepeatEnabled;
	
	dexsiDeviceManager::Ref pDevices;
	
	bool *pKeyStates;
	
	LogLevel pLogLevel;
	bool pEnableRawMouseInput, pRawMouseInputActive;
	double pAccumRawMouseInputX, pAccumRawMouseInputY;
	double pRawMouseInputSensitivity;
	
	dexsiParameterList pParameters;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deXSystemInput(deLoadableModule &loadableModule);
	
	/** Clean up module. */
	~deXSystemInput() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** OS. */
	inline deOSUnix *GetOSUnix() const{ return pOSUnix; }
	
	/** Device manager. */
	inline const dexsiDeviceManager::Ref &GetDevices() const{ return pDevices; }
	
	/** Log level. */
	inline LogLevel GetLogLevel() const{ return pLogLevel; }
	void SetLogLevel(LogLevel level){pLogLevel = level;}
	
	/** Raw mouse input. */
	inline bool GetEnableRawMouseInput() const{ return pEnableRawMouseInput; }
	void SetEnableRawMouseInput(bool enable);
	
	/** Raw mouse input sensitivity. */
	inline double GetRawMouseInputSensitivity() const{ return pRawMouseInputSensitivity; }
	void SetRawMouseInputSensitivity(double sensitivity);
	
	
	/**
	 * Init the module.
	 * \returns \em true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	bool Init() override;
	
	/** Clean up module. */
	void CleanUp() override;
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
	
	/** Button at index on device at index is pressed down. */
	bool GetButtonPressed(int device, int button) override;
	
	/** Value of axis at index on device at index. */
	float GetAxisValue(int device, int axis) override;
	
	/** Value of feedback at index on device at index. */
	float GetFeedbackValue(int device, int feedback) override;
	
	/** Set value of feedback at index on device at index. */
	void SetFeedbackValue(int device, int feedback, float value) override;
	
	/**
	 * Index of button best matching key code or -1 if not found.
	 * 
	 * If more than one button matches the key code the input module decides which
	 * button is the more likely choice. Once decided the input module is required
	 * to return the same button for the same key code in subsequent queries unless
	 * a deInputEvent::eeDeviceParamsChanged event is issued.
	 * 
	 * Can be used for example to locate keyboard keys to create default binding
	 * layouts without the user pressing input keys.
	 */
	int ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode) override;
	
	/**
	 * Index of button best matching character or -1 if not found.
	 * 
	 * If more than one button matches the character the input module decides which
	 * button is the more likely choice. Once decided the input module is required
	 * to return the same button for the same character in subsequent queries unless
	 * a deInputEvent::eeDeviceParamsChanged event is issued.
	 * 
	 * Keys usually have different characters depending on the state of modifier keys.
	 * The input module matches character case insensitive to increase the chance to
	 * find a match. If mutliple keys match the character but under different modifier
	 * states the input module has to first select the key matching case sensitive
	 * then the key requiring less modifiers.
	 * 
	 * Can be used for example to locate keyboard keys to create default binding
	 * layouts without the user pressing input keys.
	 */
	int ButtonMatchingKeyChar(int device, int character) override;
	
	/**
	 * Index of button best matching key code or -1 if not found.
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	int ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyLocation location) override;
	
	/**
	 * Index of button best matching character or -1 if not found.
	 * 
	 * Same as ButtonMatchingKeyChar(int,int) but allows to distinguish between multiple
	 * keys of the same type, for example left and right shift key.
	 */
	int ButtonMatchingKeyChar(int device, int character,
		deInputEvent::eKeyLocation location) override;
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/**
	 * Check state of input devices.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	void ProcessEvents() override;
	
	/** Clear event queues in case any are used. */
	void ClearEvents() override;
	
	/** Capture input devices changed. */
	void CaptureInputDevicesChanged() override;
	
	/** Application activated or deactivated. */
	void AppActivationChanged() override;
	
	/** An event processed by the application event loop. */
	void EventLoop(XEvent &event) override;
	
	
	
	/** Add axis changed event. */
	void AddAxisChanged(int device, int axis, float value, const timeval &eventTime);
	
	/** Add button pressed. */
	void AddButtonPressed(int device, int button, const timeval &eventTime);
	
	/** Add button released. */
	void AddButtonReleased(int device, int button, const timeval &eventTime);
	
	/** Add mouse wheel changed event. */
	void AddMouseWheelChanged(int device, int axis, int x, int y, int state,
		const timeval &eventTime);
	
	/** Add device attached/detached event. */
	void AddDeviceAttachedDetached(const timeval &eventTime);
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
	void pCenterPointer();
	
	void pAddKeyPress(int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime);
	
	void pAddKeyRelease(int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime);
	
	void pAddMousePress(int device, int button, int state, const timeval &eventTime);
	void pAddMouseRelease(int device, int button, int state, const timeval &eventTime);
	void pAddMouseMove(int device, int state, int x, int y, const timeval &eventTime);
	void pQueryMousePosition(bool sendEvents);
	int pModifiersFromXState(int xstate) const;
	bool pLookUpKey(XKeyEvent &event, sKey &key);
	//int pModifiersFromKeyState() const;
	void pUpdateAutoRepeat();
	void pSetAutoRepeatEnabled(bool enabled);
	void pUpdateRawMouseInput();
	void pCreateParameters();
};

#endif
