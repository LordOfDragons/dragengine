/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEXSIDEVICEWAYLANDKEYBOARD_H_
#define _DEXSIDEVICEWAYLANDKEYBOARD_H_

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include "dexsiDevice.h"

struct xkb_keymap;


/**
 * Wayland keyboard input device.
 */
class dexsiDeviceWaylandKeyboard : public dexsiDevice{
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<dexsiDeviceWaylandKeyboard>;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	explicit dexsiDeviceWaylandKeyboard(deXSystemInput &module);
	
protected:
	/** Clean up device. */
	~dexsiDeviceWaylandKeyboard() override;
	/*@}*/
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Rebuild button list from XKB keymap. */
	void RebuildFromKeymap(xkb_keymap *keymap);
	
	/** Look up button index by Linux keycode (0-based) or -1 if not found. */
	int LookupLinuxKeyCode(uint32_t linuxKeyCode) const;
	
	/** Index of button best matching key character or -1 if not found. */
	int ButtonMatchingKeyChar(uint32_t character) const;
	
	/** Update device state. */
	void Update() override;
	/*@}*/
	
	
private:
	/** Lookup table: Linux keycode -> button index (-1 = not mapped). */
	decTList<int> pLinuxKeyCodeMap;
};

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
#endif // _DEXSIDEVICEWAYLANDKEYBOARD_H_
