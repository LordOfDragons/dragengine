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

#ifndef _DEMOIDEVICEKEYBOARD_H_
#define _DEMOIDEVICEKEYBOARD_H_

#include "demoiDevice.h"

#include <dragengine/input/deInputEvent.h>



/**
 * \brief MacOS input device Keyboard.
 */
class demoiDeviceKeyboard : public demoiDevice{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	demoiDeviceKeyboard(deMacOSInput &module);
	
protected:
	/** \brief Clean up device. */
	~demoiDeviceKeyboard() override;
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
    /** \brief Get key code for MO key code. */
    static deInputEvent::eKeyCodes KeyCodeForMOCode(int code);
    
    /** \brief Get key code for MO key code or empty if unknown. */
    static decString NameForMOCode(int code);
    
    /** \brief Get matching priority for MO key code. */
    int MatchingPriorityForMOCode(int code) const;
    
    /** \brief Button matching key char. */
    int ButtonMatchingKeyChar(int keyChar) const;
	/*@}*/
};

#endif
