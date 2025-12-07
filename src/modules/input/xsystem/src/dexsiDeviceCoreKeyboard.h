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

#ifndef _DEXSIDEVICECOREKEYBOARD_H_
#define _DEXSIDEVICECOREKEYBOARD_H_

#include "dexsiDevice.h"

#include <dragengine/input/deInputEvent.h>



/**
 * X-System core keyboard input device.
 */
class dexsiDeviceCoreKeyboard : public dexsiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dexsiDeviceCoreKeyboard> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dexsiDeviceCoreKeyboard(deXSystemInput &module);
	
protected:
	/** Clean up device. */
	~dexsiDeviceCoreKeyboard() override;
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Get key code for X11 key sym. */
	static deInputEvent::eKeyCodes KeyCodeForKeySym(KeySym keysym);
	
	/** Get key location for X11 key sym. */
	static deInputEvent::eKeyLocation KeyLocationForKeySym(KeySym keysym);
	
	/** Get matching priority for X11 key sym. */
	static int MatchingPriorityForKeySym(KeySym keysym);
	
	/** Get button best matching a character. */
	int ButtonMatchingKeyChar(int keyChar) const;
	/*@}*/
};

#endif
