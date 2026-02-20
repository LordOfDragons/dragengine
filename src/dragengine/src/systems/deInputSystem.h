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

#ifndef _DEINPUTSYSTEM_H_
#define _DEINPUTSYSTEM_H_

#include "deBaseSystem.h"

class deInputEvent;
class deInputEventQueue;
class deBaseInputModule;


/**
 * \brief Input system acquiring input from the user.
 * 
 * Supported input devices are all devices found on the host system which can
 * be used to provide digital or analog input.
 */
class DE_DLL_EXPORT deInputSystem : public deBaseSystem{
private:
	deBaseInputModule *pActiveModule;
	deInputEventQueue *pEventQueue;
	bool pCaptureInputDevices;
	int pDropInputCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input system. */
	deInputSystem(deEngine *engine);
	
	/** \brief Clean up input system. */
	~deInputSystem() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active module or NULL if none is active. */
	inline deBaseInputModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Input queue. */
	inline deInputEventQueue &GetEventQueue() const{ return *pEventQueue; }
	
	/** \brief Clear event queues. */
	void ClearEventQueues();
	
	/** \brief Notify active input module screen size changed. */
	void ScreenSizeChanged();
	
	/** \brief Notify active input module Application activated or deactivated. */
	void AppActivationChanged();
	
	/** \brief Input devices are captured by the active input module if supported. */
	inline bool GetCaptureInputDevices() const{ return pCaptureInputDevices; }
	
	/** \brief Set if input devices are captured by the active input module if supported. */
	void SetCaptureInputDevices(bool captureInputDevices);

	/**
	 * \brief Start drop input.
	 * \version 1.23
	 * 
	 * Increments the drop input counter by 1. If the drop input counter is 1 enables
	 * drop input mode. If drop input mode is enabled input events are discarded. This is
	 * useful for modules displaying overlays required to consume inputs without these
	 * inputs affecting the application. To leave drop input mode call StopDropInput().
	 * This fuction can be called multiple times. Calls to StartDropInput() and
	 * StopDropInput() have to be balanced.
	 */
	void StartDropInput();

	/**
	 * \brief Stop drop input.
	 * \version 1.23
	 * 
	 * Decrements the drop input counter by 1. If the drop input counter is 0 disables
	 * drop input mode. If drop input mode is enabled input events are discarded. This is
	 * useful for modules displaying overlays required to consume inputs without these
	 * inputs affecting the application. To leave drop input mode call StopDropInput().
	 * This fuction can be called multiple times. Calls to StartDropInput() and
	 * StopDropInput() have to be balanced.
	 */
	void StopDropInput();

	/**
	 * \brief Drop input is enabled.
	 * \version 1.23
	 * 
	 * Returns true if the drop input counter is larger than 0.
	 */
	bool GetDropModeEnabled() const;

	/**
	 * \brief Determine if event has to be dropped due to drop input mode.
	 * \version 1.23
	 * 
	 * Returns true if the event must be dropped.
	 */
	bool DropEvent(const deInputEvent &event) const;
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	void SetActiveModule(deLoadableModule *module) override;
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	void ClearPermanents() override;
	
	/** \brief Carry out here actions right after the system started up. */
	void PostStart() override;
	
	/** \brief Carry out here actions right before the system shuts down. */
	void PreStop() override;
	/*@}*/
};

#endif
