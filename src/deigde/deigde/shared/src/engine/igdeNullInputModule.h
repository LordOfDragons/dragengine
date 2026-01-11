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

#ifndef _IGDENULLINPUTMODULE_H_
#define _IGDENULLINPUTMODULE_H_


#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>



/**
 * \brief nullptr Input Module.
 * 
 * Provides an input module which does nothing as the input is handled by the map editor already.
 */
class DE_DLL_EXPORT igdeNullInputModule : public deBaseInputModule{
public:
	class DE_DLL_EXPORT cModule : public deInternalModule{
	public:
		typedef deTObjectReference<cModule> Ref;
		cModule(deModuleSystem *system);
		
	protected:
		~cModule() override;
		
	public:
		void CreateModule() override;
	};
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	igdeNullInputModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~igdeNullInputModule() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initialize module. */
	bool Init() override;
	
	/** \brief Clean up module. */
	void CleanUp() override;
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	int GetDeviceCount() override;
	
	/** \brief Information for input device at index. */
	deInputDevice::Ref GetDeviceAt(int index) override;
	
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
	
	/** \brief Index of button best matching key code or -1 if not found. */
	int ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode) override;
	
	/** \brief Index of button best matching character or -1 if not found. */
	int ButtonMatchingKeyChar(int device, int character) override;
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief Process events. */
	void ProcessEvents() override;
	
	/** \brief Get event. */
	virtual bool GetEvent(deInputEvent *event);
	
	/** \brief Clear events. */
	void ClearEvents() override;
	/*@}*/
};

#endif
