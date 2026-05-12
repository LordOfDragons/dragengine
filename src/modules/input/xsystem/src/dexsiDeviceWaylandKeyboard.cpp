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
#include "deXSystemInput.h"

#include <xkbcommon/xkbcommon.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>


// Helpers
////////////

static deInputEvent::eKeyCodes KeyCodeForXkbKeySym(xkb_keysym_t keysym){
	switch(keysym){
	case XKB_KEY_Shift_L:
	case XKB_KEY_Shift_R:
		return deInputEvent::ekcShift;
		
	case XKB_KEY_Control_L:
	case XKB_KEY_Control_R:
		return deInputEvent::ekcControl;
		
	case XKB_KEY_Meta_L:
	case XKB_KEY_Meta_R:
		return deInputEvent::ekcMeta;
		
	case XKB_KEY_Alt_L:
	case XKB_KEY_Alt_R:
		return deInputEvent::ekcAlt;
		
	case XKB_KEY_Super_L:
	case XKB_KEY_Super_R:
		return deInputEvent::ekcSuper;
		
	case XKB_KEY_Hyper_L:
	case XKB_KEY_Hyper_R:
		return deInputEvent::ekcHyper;
		
	case XKB_KEY_BackSpace:
	case XKB_KEY_Clear:
		return deInputEvent::ekcBackSpace;
		
	case XKB_KEY_Tab:
	case XKB_KEY_KP_Tab:
		return deInputEvent::ekcTab;
		
	case XKB_KEY_Linefeed:
	case XKB_KEY_Return:
	case XKB_KEY_KP_Enter:
		return deInputEvent::ekcReturn;
		
	case XKB_KEY_Pause:
	case XKB_KEY_Sys_Req:
		return deInputEvent::ekcPause;
		
	case XKB_KEY_Escape:
		return deInputEvent::ekcEscape;
		
	case XKB_KEY_space:
		return deInputEvent::ekcSpace;
		
	case XKB_KEY_Delete:
	case XKB_KEY_KP_Delete:
		return deInputEvent::ekcDelete;
		
	case XKB_KEY_Home:
	case XKB_KEY_Begin:
	case XKB_KEY_KP_Home:
	case XKB_KEY_KP_Begin:
		return deInputEvent::ekcHome;
		
	case XKB_KEY_Left:
	case XKB_KEY_KP_Left:
		return deInputEvent::ekcArrowLeft;
		
	case XKB_KEY_Up:
	case XKB_KEY_KP_Up:
		return deInputEvent::ekcArrowUp;
		
	case XKB_KEY_Right:
	case XKB_KEY_KP_Right:
		return deInputEvent::ekcArrowRight;
		
	case XKB_KEY_Down:
	case XKB_KEY_KP_Down:
		return deInputEvent::ekcArrowDown;
		
	case XKB_KEY_Page_Up:
	case XKB_KEY_KP_Page_Up:
		return deInputEvent::ekcPageUp;
		
	case XKB_KEY_Page_Down:
	case XKB_KEY_KP_Page_Down:
		return deInputEvent::ekcPageDown;
		
	case XKB_KEY_End:
	case XKB_KEY_KP_End:
		return deInputEvent::ekcEnd;
		
	case XKB_KEY_Insert:
	case XKB_KEY_KP_Insert:
		return deInputEvent::ekcInsert;
		
	default:
		if(keysym >= XKB_KEY_F1 && keysym <= XKB_KEY_F12){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcF1 + (keysym - XKB_KEY_F1));
			
		}else if(keysym >= XKB_KEY_KP_F1 && keysym <= XKB_KEY_KP_F4){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcF1 + (keysym - XKB_KEY_KP_F1));
			
		}else if(keysym >= XKB_KEY_KP_0 && keysym <= XKB_KEY_KP_9){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (keysym - XKB_KEY_KP_0));
			
		}else if(keysym >= XKB_KEY_0 && keysym <= XKB_KEY_9){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (keysym - XKB_KEY_0));
			
		}else if(keysym >= XKB_KEY_a && keysym <= XKB_KEY_z){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (keysym - XKB_KEY_a));
			
		}else if(keysym >= XKB_KEY_A && keysym <= XKB_KEY_Z){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (keysym - XKB_KEY_A));
		}
	}
	
	return deInputEvent::ekcUndefined;
}

static deInputEvent::eKeyLocation KeyLocationForXkbKeySym(xkb_keysym_t keysym){
	switch(keysym){
	case XKB_KEY_Shift_L:
	case XKB_KEY_Control_L:
	case XKB_KEY_Meta_L:
	case XKB_KEY_Alt_L:
	case XKB_KEY_Super_L:
	case XKB_KEY_Hyper_L:
		return deInputEvent::eklLeft;
		
	case XKB_KEY_Shift_R:
	case XKB_KEY_Control_R:
	case XKB_KEY_Meta_R:
	case XKB_KEY_Alt_R:
	case XKB_KEY_Super_R:
	case XKB_KEY_Hyper_R:
		return deInputEvent::eklRight;
		
	case XKB_KEY_KP_Tab:
	case XKB_KEY_KP_Enter:
	case XKB_KEY_KP_Delete:
	case XKB_KEY_KP_Home:
	case XKB_KEY_KP_Begin:
	case XKB_KEY_KP_Left:
	case XKB_KEY_KP_Up:
	case XKB_KEY_KP_Right:
	case XKB_KEY_KP_Down:
	case XKB_KEY_KP_Page_Up:
	case XKB_KEY_KP_Page_Down:
	case XKB_KEY_KP_End:
	case XKB_KEY_KP_Insert:
		return deInputEvent::eklNumberPad;
		
	default:
		if(keysym >= XKB_KEY_KP_F1 && keysym <= XKB_KEY_KP_F4){
			return deInputEvent::eklNumberPad;
			
		}else if(keysym >= XKB_KEY_KP_0 && keysym <= XKB_KEY_KP_9){
			return deInputEvent::eklNumberPad;
		}
	}
	
	return deInputEvent::eklNone;
}

static int MatchingPriorityForXkbKeySym(xkb_keysym_t keysym){
	// lower value is higher priority
	
	switch(keysym){
	case XKB_KEY_Shift_R:
	case XKB_KEY_Control_R:
	case XKB_KEY_Meta_R:
	case XKB_KEY_Alt_R:
	case XKB_KEY_Super_R:
	case XKB_KEY_Hyper_R:
	case XKB_KEY_BackSpace:
	case XKB_KEY_Tab:
	case XKB_KEY_Return:
	case XKB_KEY_Pause:
	case XKB_KEY_Escape:
	case XKB_KEY_space:
	case XKB_KEY_Delete:
	case XKB_KEY_Home:
	case XKB_KEY_Left:
	case XKB_KEY_Up:
	case XKB_KEY_Right:
	case XKB_KEY_Down:
	case XKB_KEY_Page_Up:
	case XKB_KEY_Page_Down:
	case XKB_KEY_End:
	case XKB_KEY_Insert:
		return 0;
		
	case XKB_KEY_Shift_L:
	case XKB_KEY_Control_L:
	case XKB_KEY_Meta_L:
	case XKB_KEY_Alt_L:
	case XKB_KEY_Super_L:
	case XKB_KEY_Hyper_L:
	case XKB_KEY_Clear:
	case XKB_KEY_KP_Tab:
	case XKB_KEY_KP_Enter:
	case XKB_KEY_Sys_Req:
	case XKB_KEY_KP_Delete:
	case XKB_KEY_KP_Home:
	case XKB_KEY_KP_Left:
	case XKB_KEY_KP_Up:
	case XKB_KEY_KP_Right:
	case XKB_KEY_KP_Down:
	case XKB_KEY_KP_Page_Up:
	case XKB_KEY_KP_Page_Down:
	case XKB_KEY_KP_End:
	case XKB_KEY_KP_Insert:
		return 1;
		
	case XKB_KEY_Linefeed:
	case XKB_KEY_Begin:
	case XKB_KEY_KP_Begin:
		return 2;
		
	default:
		if(keysym >= XKB_KEY_F1 && keysym <= XKB_KEY_F12){
			return 0;
			
		}else if(keysym >= XKB_KEY_KP_F1 && keysym <= XKB_KEY_KP_F4){
			return 1;
			
		}else if(keysym >= XKB_KEY_KP_0 && keysym <= XKB_KEY_KP_9){
			return 1;
			
		}else if(keysym >= XKB_KEY_0 && keysym <= XKB_KEY_9){
			return 0;
			
		}else if(keysym >= XKB_KEY_a && keysym <= XKB_KEY_z){
			return 0;
			
		}else if(keysym >= XKB_KEY_A && keysym <= XKB_KEY_Z){
			return 1;
		}
	}
	
	return 10;
}


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
	const int maxLinuxKeyCode = (int)(maxKey - minKey + 1);
	pLinuxKeyCodeMap.SetAll(maxLinuxKeyCode, -1);
	
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
		button->SetKeyCode(KeyCodeForXkbKeySym(keysym));
		button->SetKeyLocation(KeyLocationForXkbKeySym(keysym));
		button->SetMatchPriority(MatchingPriorityForXkbKeySym(keysym));
		
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
