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

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dexsiDeviceWaylandKeyboard.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceCoreKeyboard.h"
#include "deXSystemInput.h"

#include <xkbcommon/xkbcommon.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>


// Class dexsiDeviceWaylandKeyboard
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceWaylandKeyboard::dexsiDeviceWaylandKeyboard(deXSystemInput &module) :
dexsiDevice(module, esWayland)
{
	decString string;
	string.Format("%s%dwk", XINP_DEVID_PREFIX, dexsiDevice::esWayland);
	SetID(string);
	SetName("Wayland Keyboard");
	SetType(deInputDevice::edtKeyboard);
	SetDisplayImages("keyboard");
}

dexsiDeviceWaylandKeyboard::~dexsiDeviceWaylandKeyboard() = default;


// Management
///////////////

void dexsiDeviceWaylandKeyboard::RebuildFromKeymap(xkb_keymap *keymap){
	if(!keymap){
		return;
	}
	
	// clear existing buttons and lookup table
	ClearButtons();
	pLinuxKeyCodeMap.RemoveAll();
	
	const xkb_keycode_t minKey = xkb_keymap_min_keycode(keymap);
	const xkb_keycode_t maxKey = xkb_keymap_max_keycode(keymap);
	
	// pre-allocate lookup table
	const int maxLinuxKeyCode = (int)(maxKey - 8 + 1);
	pLinuxKeyCodeMap.SetCount(maxLinuxKeyCode, -1);
	
	auto sharedButton = dexsiDeviceButton::Ref::New(GetModule());
	sharedButton->SetDisplayImages("key");
	
	char keysymName[128];
	int buttonIndex = 0;
	decString string;
	
	for(xkb_keycode_t xkbCode=minKey; xkbCode<=maxKey; xkbCode++){
		// get primary keysym (group 0, level 0 = unshifted)
		const xkb_keysym_t *syms = nullptr;
		const int numSyms = xkb_keymap_key_get_syms_by_level(keymap, xkbCode, 0, 0, &syms);
		if(numSyms < 1 || !syms || syms[0] == XKB_KEY_NoSymbol){
			continue;
		}
		
		const xkb_keysym_t keysym = syms[0];
		const uint32_t linuxCode = (uint32_t)(xkbCode - 8);
		
		auto button = dexsiDeviceButton::Ref::New(GetModule());
		
		string.Format("k%d", (int)linuxCode);
		button->SetID(string);
		
		// get human-readable name from XKB
		xkb_keysym_get_name(keysym, keysymName, sizeof(keysymName));
		string = keysymName;
		if(!string.IsEmpty()){
			string.SetAt(0, (char)toupper(string.GetAt(0)));
		}
		button->SetName(string);
		
		// store keysym as X11Code (reused field, compatible values)
		button->SetX11Code((int)keysym);
		button->SetKeyCode(dexsiDeviceCoreKeyboard::KeyCodeForKeySym((KeySym)keysym));
		button->SetKeyLocation(dexsiDeviceCoreKeyboard::KeyLocationForKeySym((KeySym)keysym));
		button->SetMatchPriority(dexsiDeviceCoreKeyboard::MatchingPriorityForKeySym((KeySym)keysym));
		
		button->SetDisplayImages(sharedButton);
		
		// display text. use unicode character for printable keys
		const uint32_t ucs32 = xkb_keysym_to_utf32(keysym);
		if(ucs32 >= 32 && ucs32 != 127){
			button->SetDisplayText(decUnicodeString((int)ucs32).GetUpper().ToUTF8());
			
		}else{
			button->SetDisplayText(button->GetName());
		}
		
		AddButton(button);
		
		if((int)linuxCode < maxLinuxKeyCode){
			pLinuxKeyCodeMap.SetAt((int)linuxCode, buttonIndex);
		}
		buttonIndex++;
	}
}

int dexsiDeviceWaylandKeyboard::LookupLinuxKeyCode(uint32_t linuxKeyCode) const{
	const int index = (int)linuxKeyCode;
	if(index < 0 || index >= pLinuxKeyCodeMap.GetCount()){
		return -1;
	}
	return pLinuxKeyCodeMap.GetAt(index);
}

int dexsiDeviceWaylandKeyboard::ButtonMatchingKeyChar(uint32_t character) const{
	if(character < 32 || character == 127){
		return -1;
	}
	
	int bestPriority = 10;
	int bestButton = -1;
	
	GetButtons().VisitIndexed([&](int i, const dexsiDeviceButton &button){
		// GetX11Code() stores the keysym value; for basic characters, keysym == unicode
		const uint32_t keysym = (uint32_t)button.GetX11Code();
		const uint32_t ucs = xkb_keysym_to_utf32(keysym);
		if(ucs == 0){
			return;
		}
		
		// case-insensitive comparison
		const char a = (char)tolower((int)character);
		const char b = (char)tolower((int)ucs);
		if(a != b){
			return;
		}
		
		// prefer exact match over case-folded match
		const int priority = (ucs == character) ? 0 : 1;
		if(priority < bestPriority || (priority == bestPriority && button.GetMatchPriority() < bestPriority)){
			bestButton = i;
			bestPriority = priority;
		}
	});
	
	return bestButton;
}

void dexsiDeviceWaylandKeyboard::Update(){
}

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
