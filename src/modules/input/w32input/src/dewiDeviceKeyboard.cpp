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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "dewiDeviceKeyboard.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceFeedback.h"
#include "dewiDeviceManager.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dewiDeviceKeyboard
/////////////////////////////

// Constructor, destructor
////////////////////////////

struct sKeyTableEntry{
	int virtkey;
	const char *name;
};

static const sKeyTableEntry vKeyTable[] = {
	{VK_ADD,"Numpad +"},
	{VK_ATTN,"Attn"},
	{VK_BACK,"Backspace"},
	{VK_CANCEL,"Break"},
	{VK_CLEAR,"Clear"},
	{VK_CRSEL,"Cr Sel"},
	{VK_DECIMAL,"Numpad ."},
	{VK_DIVIDE,"Numpad /"},
	{VK_EREOF,"Er Eof"},
	{VK_ESCAPE,"Esc"},
	{VK_EXECUTE,"Execute"},
	{VK_EXSEL,"Ex Sel"},
	{VK_ICO_CLEAR,"IcoClr"},
	{VK_ICO_HELP,"IcoHlp"},
	{'0',"0"},
	{'1',"1"},
	{'2',"2"},
	{'3',"3"},
	{'4',"4"},
	{'5',"5"},
	{'6',"6"},
	{'7',"7"},
	{'8',"8"},
	{'9',"9"},
	{'A',"A"},
	{'B',"B"},
	{'C',"C"},
	{'D',"D"},
	{'E',"E"},
	{'F',"F"},
	{'G',"G"},
	{'H',"H"},
	{'I',"I"},
	{'J',"J"},
	{'K',"K"},
	{'L',"L"},
	{'M',"M"},
	{'N',"N"},
	{'O',"O"},
	{'P',"P"},
	{'Q',"Q"},
	{'R',"R"},
	{'S',"S"},
	{'T',"T"},
	{'U',"U"},
	{'V',"V"},
	{'W',"W"},
	{'X',"X"},
	{'Y',"Y"},
	{'Z',"Z"},
	{VK_MULTIPLY,"Numpad *"},
	{VK_NONAME,"NoName"},
	{VK_NUMPAD0,"Numpad 0"},
	{VK_NUMPAD1,"Numpad 1"},
	{VK_NUMPAD2,"Numpad 2"},
	{VK_NUMPAD3,"Numpad 3"},
	{VK_NUMPAD4,"Numpad 4"},
	{VK_NUMPAD5,"Numpad 5"},
	{VK_NUMPAD6,"Numpad 6"},
	{VK_NUMPAD7,"Numpad 7"},
	{VK_NUMPAD8,"Numpad 8"},
	{VK_NUMPAD9,"Numpad 9"},
	{VK_OEM_1,"OEM_1 (: ;)"},
	{VK_OEM_102,"OEM_102 (> <)"},
	{VK_OEM_2,"OEM_2 (? /)"},
	{VK_OEM_3,"OEM_3 (~ `)"},
	{VK_OEM_4,"OEM_4 ({[)"},
	{VK_OEM_5,"OEM_5 (| \\)"},
	{VK_OEM_6,"OEM_6 (}])"},
	{VK_OEM_7,"OEM_7 (\"},\" ')"},
	{VK_OEM_8,"OEM_8 (§ !)"},
	{VK_OEM_ATTN,"Oem Attn"},
	{VK_OEM_AUTO,"Auto"},
	{VK_OEM_AX,"Ax"},
	{VK_OEM_BACKTAB,"Back Tab"},
	{VK_OEM_CLEAR,"OemClr"},
	{VK_OEM_COMMA,"OEM_COMMA (< ,)"},
	{VK_OEM_COPY,"Copy"},
	{VK_OEM_CUSEL,"Cu Sel"},
	{VK_OEM_ENLW,"Enlw"},
	{VK_OEM_FINISH,"Finish"},
	{VK_OEM_FJ_LOYA,"Loya"},
	{VK_OEM_FJ_MASSHOU,"Mashu"},
	{VK_OEM_FJ_ROYA,"Roya"},
	{VK_OEM_FJ_TOUROKU,"Touroku"},
	{VK_OEM_JUMP,"Jump"},
	{VK_OEM_MINUS,"OEM_MINUS (_ -)"},
	{VK_OEM_PA1,"OemPa1"},
	{VK_OEM_PA2,"OemPa2"},
	{VK_OEM_PA3,"OemPa3"},
	{VK_OEM_PERIOD,"OEM_PERIOD (> .)"},
	{VK_OEM_PLUS,"OEM_PLUS (+ =)"},
	{VK_OEM_RESET,"Reset"},
	{VK_OEM_WSCTRL,"WsCtrl"},
	{VK_PA1,"Pa1"},
	{VK_PACKET,"Packet"},
	{VK_PLAY,"Play"},
	{VK_PROCESSKEY,"Process"},
	{VK_RETURN,"Enter"},
	{VK_SELECT,"Select"},
	{VK_SEPARATOR,"Separator"},
	{VK_SPACE,"Space"},
	{VK_SUBTRACT,"Num -"},
	{VK_TAB,"Tab"},
	{VK_ZOOM,"Zoom"},
	{VK_ACCEPT,"Accept"},
	{VK_APPS,"Context Menu"},
	{VK_BROWSER_BACK,"Browser Back"},
	{VK_BROWSER_FAVORITES,"Browser Favorites"},
	{VK_BROWSER_FORWARD,"Browser Forward"},
	{VK_BROWSER_HOME,"Browser Home"},
	{VK_BROWSER_REFRESH,"Browser Refresh"},
	{VK_BROWSER_SEARCH,"Browser Search"},
	{VK_BROWSER_STOP,"Browser Stop"},
	{VK_CAPITAL,"Caps Lock"},
	{VK_CONVERT,"Convert"},
	{VK_DELETE,"Delete"},
	{VK_DOWN,"Arrow Down"},
	{VK_END,"End"},
	{VK_F1,"F1"},
	{VK_F10,"F10"},
	{VK_F11,"F11"},
	{VK_F12,"F12"},
	{VK_F13,"F13"},
	{VK_F14,"F14"},
	{VK_F15,"F15"},
	{VK_F16,"F16"},
	{VK_F17,"F17"},
	{VK_F18,"F18"},
	{VK_F19,"F19"},
	{VK_F2,"F2"},
	{VK_F20,"F20"},
	{VK_F21,"F21"},
	{VK_F22,"F22"},
	{VK_F23,"F23"},
	{VK_F24,"F24"},
	{VK_F3,"F3"},
	{VK_F4,"F4"},
	{VK_F5,"F5"},
	{VK_F6,"F6"},
	{VK_F7,"F7"},
	{VK_F8,"F8"},
	{VK_F9,"F9"},
	{VK_FINAL,"Final"},
	{VK_HELP,"Help"},
	{VK_HOME,"Home"},
	{VK_ICO_00,"Ico00 *"},
	{VK_INSERT,"Insert"},
	{VK_JUNJA,"Junja"},
	{VK_KANA,"Kana"},
	{VK_KANJI,"Kanji"},
	{VK_LAUNCH_APP1,"App1"},
	{VK_LAUNCH_APP2,"App2"},
	{VK_LAUNCH_MAIL,"Mail"},
	{VK_LAUNCH_MEDIA_SELECT,"Media"},
	{VK_LCONTROL,"Left Ctrl"},
	{VK_LEFT,"Arrow Left"},
	{VK_LMENU,"Left Alt"},
	{VK_LSHIFT,"Left Shift"},
	{VK_LWIN,"Left Win"},
	{VK_MEDIA_NEXT_TRACK,"Next Track"},
	{VK_MEDIA_PLAY_PAUSE,"Play / Pause"},
	{VK_MEDIA_PREV_TRACK,"Previous Track"},
	{VK_MEDIA_STOP,"Stop"},
	{VK_MODECHANGE,"Mode Change"},
	{VK_NEXT,"Page Down"},
	{VK_NONCONVERT,"Non Convert"},
	{VK_NUMLOCK,"Num Lock"},
	{VK_OEM_FJ_JISHO,"Jisho"},
	{VK_PAUSE,"Pause"},
	{VK_PRINT,"Print"},
	{VK_PRIOR,"Page Up"},
	{VK_RCONTROL,"Right Ctrl"},
	{VK_RIGHT,"Arrow Right"},
	{VK_RMENU,"Right Alt"},
	{VK_RSHIFT,"Right Shift"},
	{VK_RWIN,"Right Win"},
	{VK_SCROLL,"Scrol Lock"},
	{VK_SLEEP,"Sleep"},
	{VK_SNAPSHOT,"Print Screen"},
	{VK_UP,"Arrow Up"},
	{VK_VOLUME_DOWN,"Volume Down"},
	{VK_VOLUME_MUTE,"Volume Mute"},
	{VK_VOLUME_UP,"Volume Up"},
	{0,NULL}
};

dewiDeviceKeyboard::dewiDeviceKeyboard(deWindowsInput &module) :
dewiDevice(module, esWindows)
{
	decString string;
	
	string.Format("%s%dkeyboard", WINP_DEVID_PREFIX, esWindows);
	SetID(string);
	SetName("Keyboard");
	SetType(deInputDevice::edtKeyboard);
	SetDisplayImages("keyboard");
	
	// find keys. GetKeyNameText is used with the lParam of messages and thus we have to
	// produce the same bit encoding. documentation has this:
	//   
	//   16-23 : Scan code.
	//   24    : Extended-key flag. Distinguishes some keys on an enhanced keyboard.
	//   25    : "Do not care" bit. The application calling this function sets this
	//           bit to indicate that the function should not distinguish between
	//           left and right CTRL and SHIFT keys, for example.
#if 0
	const int sharedKeyNameValue = (1 << 24) | (0 << 25);
	int i, buttonCount = 0;
	char buffer[100];
	
	for(i=0; i<256; i++){
		if(MapVirtualKey(i, MAPVK_VSC_TO_VK_EX) == 0){
			continue;
		}
		
		if(GetKeyNameText(sharedKeyNameValue | (i << 16),
		(LPTSTR)buffer, sizeof(buffer)) == 0){
			continue;
		}
		
		buttonCount++;
	}
	
	SetButtonCount(buttonCount);
	
	int buttonIndex = 0;
	for(i=0; i<256; i++){
		const int wicode = MapVirtualKey(i, MAPVK_VSC_TO_VK_EX);
		if(wicode == 0){
			continue;
		}
		
		const int len = GetKeyNameText(sharedKeyNameValue | (i << 16),
		(LPTSTR)buffer, sizeof(buffer));
		if(len == 0){
			continue;
		}
		
		dewiDeviceButton &button = GetButtonAt(buttonIndex);
		
		string.Format("sc%d", i);
		button.SetID(string);
		
		button.SetName(buffer);
		
		button.SetWICode(wicode);
		
		const int wichar = MapVirtualKey(wicode, MAPVK_VK_TO_CHAR);
		if(wichar != 0){
			button.SetWIChar(wichar);
		}
		
		button.SetKeyCode(KeyCodeForWICode(wicode));
		button.SetMatchPriority(MatchingPriorityForWICode(wicode));
		
		buttonIndex++;
	}
#endif
	
	dewiDeviceButton::Ref sharedButton(dewiDeviceButton::Ref::NewWith(module));
	sharedButton->SetDisplayImages("key");
	
	int buttonIndex = 0;
	const sKeyTableEntry *iterentry = &vKeyTable[0];
	
	while(iterentry->virtkey){
		const dewiDeviceButton::Ref button(dewiDeviceButton::Ref::NewWith(module));
		AddButton(button);
		
		string.Format("sc%d", buttonIndex);
		button->SetID(string);
		
		button->SetName(iterentry->name);
		button->SetWICode(iterentry->virtkey);
		
		button->SetDisplayImages(sharedButton);
		
		const int wichar = MapVirtualKey(iterentry->virtkey, MAPVK_VK_TO_CHAR);
		if(wichar >= 32){
			button->SetWIChar(wichar);
			button->SetDisplayText(decUnicodeString(wichar).GetUpper().ToUTF8());
			
		}else{
			button->SetDisplayText(button->GetName());
		}
		
		button->SetKeyCode(KeyCodeForWICode(iterentry->virtkey));
		button->SetKeyLocation(KeyLocationForWICode(iterentry->virtkey));
		button->SetMatchPriority(MatchingPriorityForWICode(iterentry->virtkey));
		
		buttonIndex++;
		iterentry++;
	}
}

dewiDeviceKeyboard::~dewiDeviceKeyboard(){
}



// Management
///////////////

deInputEvent::eKeyCodes dewiDeviceKeyboard::KeyCodeForWICode(int code){
	switch(code){
	case VK_BACK:
	case VK_CLEAR:
		return deInputEvent::ekcBackSpace;
		
	case VK_TAB:
		return deInputEvent::ekcTab;
		
	case VK_RETURN:
		return deInputEvent::ekcReturn;
		
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		return deInputEvent::ekcShift;
		
	case VK_CONTROL:
	case VK_LCONTROL:
	case VK_RCONTROL:
		return deInputEvent::ekcControl;
		
	case VK_MENU:
	case VK_LMENU:
	case VK_RMENU:
		return deInputEvent::ekcAlt;
		
	case VK_PAUSE:
		return deInputEvent::ekcPause;
		
	case VK_ESCAPE:
		return deInputEvent::ekcEscape;
		
	case VK_SPACE:
		return deInputEvent::ekcSpace;
		
	case VK_PRIOR:
		return deInputEvent::ekcPageUp;
		
	case VK_NEXT:
		return deInputEvent::ekcPageDown;
		
	case VK_END:
		return deInputEvent::ekcEnd;
		
	case VK_HOME:
		return deInputEvent::ekcHome;
		
	case VK_LEFT:
		return deInputEvent::ekcArrowLeft;
		
	case VK_RIGHT:
		return deInputEvent::ekcArrowRight;
		
	case VK_DOWN:
		return deInputEvent::ekcArrowDown;
		
	case VK_INSERT:
		return deInputEvent::ekcInsert;
		
	case VK_DELETE:
		return deInputEvent::ekcDelete;
		
	case VK_LWIN:
	case VK_RWIN:
		return deInputEvent::ekcMeta;
		
	case VK_APPS:
		return deInputEvent::ekcSuper;
		
	default:
		if(code >= VK_F1 && code <= VK_F12){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcF1 + (code - VK_F1));
			
		}else if(code >= 0x30 && code <= 0x39){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (code - 0x30));
			
		}else if(code >= VK_NUMPAD0 && code <= VK_NUMPAD9){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekc0 + (code - VK_NUMPAD0));
			
		}else if(code >= 0x41 && code <= 0x5A){
			return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (code - 0x41));
		}
	}
	
	return deInputEvent::ekcUndefined;
}

deInputEvent::eKeyLocation dewiDeviceKeyboard::KeyLocationForWICode(int code){
	switch(code){
	case VK_LSHIFT:
	case VK_LCONTROL:
	case VK_LMENU:
	case VK_LWIN:
		return deInputEvent::eklLeft;
		
	case VK_RSHIFT:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_RWIN:
		return deInputEvent::eklRight;
		
	default:
		if(code >= VK_NUMPAD0 && code <= VK_NUMPAD9){
			return deInputEvent::eklNumberPad;
		}
	}
	
	return deInputEvent::eklNone;
}

int dewiDeviceKeyboard::MatchingPriorityForWICode(int code){
	// lower value is higher priority
	
	switch(code){
	case VK_BACK:
	case VK_TAB:
	case VK_RETURN:
	case VK_SHIFT:
	case VK_RSHIFT:
	case VK_CONTROL:
	case VK_RCONTROL:
	case VK_MENU:
	case VK_RMENU:
	case VK_PAUSE:
	case VK_ESCAPE:
	case VK_SPACE:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_DOWN:
	case VK_INSERT:
	case VK_DELETE:
	case VK_RWIN:
	case VK_APPS:
		return 0;
		
	case VK_LSHIFT:
	case VK_LCONTROL:
	case VK_LMENU:
	case VK_LWIN:
	case VK_CLEAR:
		return 1;
		
	default:
		if(code >= VK_F1 && code <= VK_F12){
			return 0;
			
		}else if(code >= 0x30 && code <= 0x39){
			return 0;
			
		}else if(code >= VK_NUMPAD0 && code <= VK_NUMPAD9){
			return 1;
			
		}else if(code >= 0x41 && code <= 0x5A){
			return 0;
		}
	}
	
	return 10;
}

int dewiDeviceKeyboard::ButtonMatchingKeyChar(int keyChar) const{
	// this task is just a guess so missing a solution is fine
	const int count = GetButtonCount();
	int i;
	for(i=0; i<count; i++){
		if(GetButtonAt(i)->GetWIChar() == keyChar){
			return i;
		}
	}
	
	return -1;
}



// Private Functions
//////////////////////

void dewiDeviceKeyboard::pSetButtonAt(int index, const char *id, const char *name,
int wiCode, deInputEvent::eKeyCodes keyCode, int wiChar, int matchPriority){
	dewiDeviceButton &button = *GetButtonAt(index);
	
	button.SetID(id);
	button.SetName(name);
	button.SetWICode(wiCode);
	button.SetWIChar(wiChar);
	button.SetKeyCode(keyCode);
	button.SetMatchPriority(matchPriority);
}
