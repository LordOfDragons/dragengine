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

#ifndef _DEWIDEVICEKEYBOARD_H_
#define _DEWIDEVICEKEYBOARD_H_

#include "dewiDevice.h"

#include <dragengine/input/deInputEvent.h>



/**
 * Windows keyboard input device.
 */
class dewiDeviceKeyboard : public dewiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dewiDeviceKeyboard> Ref;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	dewiDeviceKeyboard( deWindowsInput &module );
	
protected:
	/** Clean up device. */
	virtual ~dewiDeviceKeyboard();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Get key code for windows virtual key code. */
	static deInputEvent::eKeyCodes KeyCodeForWICode( int code );
	
	/** Get key location for windows virtual key code. */
	static deInputEvent::eKeyLocation KeyLocationForWICode( int code );
	
	/** Get matching priority for virtual key code. */
	static int MatchingPriorityForWICode( int code );
	
	/** Get button best matching a character. */
	int ButtonMatchingKeyChar( int keyChar ) const;
	/*@}*/
	
	
	
private:
	void pSetButtonAt( int index, const char *id, const char *name, int wiCode,
		deInputEvent::eKeyCodes keyCode, int wiChar, int matchPriority );
};

#endif
