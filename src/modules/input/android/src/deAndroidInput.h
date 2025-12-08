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

#ifndef _DEANDROIDINPUT_H_
#define _DEANDROIDINPUT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include <android/configuration.h>
#include <android/native_activity.h>
#include <android/input.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

class deOSAndroid;
class deainpOverlaySystem;
class deainpDeviceManager;
#include <dragengine/resources/font/deFont.h>



/**
 * \brief input module device identifier prefix.
 */
#define AINP_DEVID_PREFIX "AInp_"


/**
 * \brief Android input module.
 */
class deAndroidInput : public deBaseInputModule{
private:
	deOSAndroid *pOSAndroid;
	
	int pMouseButtons;
	bool pIsListening;
	int pOldThreshold;
	decPoint pLastMouse;
	bool *pKeyStates;
	int pPointerMouse;
	
	deainpOverlaySystem *pOverlaySystem;
	
	deFont::Ref pFontDefault;
	
	decTimer pInputTimer;
	float pElapsedTime;
	
	deainpDeviceManager *pDevices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input module. */
	deAndroidInput(deLoadableModule &loadableModule);
	
	/** \brief Clean up input module. */
	~deAndroidInput() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize module. */
	bool Init() override;
	
	/** \brief Clean up module. */
	void CleanUp() override;
	
	
	
	/** \brief Screen size. */
	decPoint GetScreenSize() const;
	
	/** \brief Overlay system. */
	inline deainpOverlaySystem *GetOverlaySystem() const{ return pOverlaySystem; }
	
	/** \brief Default font. */
	inline const deFont::Ref &GetDefaultFont() const{ return pFontDefault; }
	
	/** \brief Elapsed time since the last event processing. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** \brief Devices manager. */
	inline deainpDeviceManager &GetDevices() const{ return *pDevices; }
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	int GetDeviceCount() override;
	
	/** \brief Information for input device at index. */
	deInputDevice *GetDeviceAt(int index) override;
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfDeviceWithID(const char *id) override;
	
	/** \brief Index of button with identifier on device at index or -1 if absent. */
	int IndexOfButtonWithID(int device, const char *id) override;
	
	/** \brief Index of axis with identifier on device at index or -1 if absent. */
	int IndexOfAxisWithID(int device, const char *id) override;
	
	/** \brief Index of feedback with identifier on device at index or -1 if absent. */
	int IndexOfFeedbackWithID(int device, const char *id) override;
	
	/** \brief Button at index on device at index is pressed down. */
	bool GetButtonPressed(int device, int button) override;
	
	/** \brief Value of axis at index on device at index. */
	float GetAxisValue(int device, int axis) override;
	
	/** \brief Value of feedback at index on device at index. */
	float GetFeedbackValue(int device, int feedback) override;
	
	/** \brief Set value of feedback at index on device at index. */
	void SetFeedbackValue(int device, int feedback, float value) override;
	
	/**
	 * \brief Index of button best matching key code or -1 if not found.
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
	 * \brief Index of button best matching character or -1 if not found.
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
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/**
	 * \brief Check state of input devices.
	 * \details This function is called before any other frame related tasks are carried out.
	 *          Record changes in devices states have to be recored into a game event reported
	 *          as engine input events You are expected to check message and event queues to
	 *          deliver system notification (like quitting the game) to the game engine.
	 */
	void ProcessEvents() override;
	
	/** \brief Clear event queues in case any are used. */
	void ClearEvents() override;
	
	/** \brief Notify module screen size changed in case it uses an overlay canvas. */
	void ScreenSizeChanged() override;
	
	/** \brief An event processed by the application event loop. */
	void EventLoop(const android_input_buffer &inputBuffer) override;
	
	
	
	/** \brief Timeval of now. */
	timeval TimeValNow() const;
	
	/** \brief Add key press event to game input queue. */
	void AddKeyPress(int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, const timeval &eventTime);
	
	/** \brief Add key release event to game input queue. */
	void AddKeyRelease(int device, int button, int keyChar,
		deInputEvent::eKeyCodes keyCode, const timeval &eventTime);
	
	/** \brief Add mouse press event to game input queue. */
	void AddMousePress(int device, int button, int state, const timeval &eventTime);
	
	/** \brief Add mouse release event to game input queue. */
	void AddMouseRelease(int device, int button, int state, const timeval &eventTime);
	
	/** \brief Add mouse move event to game input queue. */
	void AddMouseMove(int device, int state, const decPoint &distance, const timeval &eventTime);
	
	/** \brief Add axis changed event. */
	void AddAxisChanged(int device, int axis, float value, const timeval &eventTime);
	
	/** \brief Add button pressed. */
	void AddButtonPressed(int device, int button, const timeval &eventTime);
	
	/** \brief Add button released. */
	void AddButtonReleased(int device, int button, const timeval &eventTime);
	/*@}*/
	
	
	
private:
	timeval pConvertEventTime(int64_t time) const;
	void pProcessKeyEvent(const GameActivityKeyEvent &event);
	void pProcessMotionEvent(const GameActivityMotionEvent &event);
	void pProcessMotionEventTouchScreen(const GameActivityMotionEvent &event);
	void pCenterPointer();
	decPoint pPointerPosition(const GameActivityPointerAxes &pointer) const;
	int pModifiersFromMetaState(int32_t metaState) const;
};

#endif
