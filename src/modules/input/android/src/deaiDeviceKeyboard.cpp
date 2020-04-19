/* 
 * Drag[en]gine Android Input Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "deaiDeviceKeyboard.h"
#include "deaiDeviceAxis.h"
#include "deaiDeviceButton.h"
#include "deaiDeviceFeedback.h"
#include "deaiDeviceManager.h"
#include "deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class deaiDeviceKeyboard
/////////////////////////////

// Constructor, destructor
////////////////////////////

deaiDeviceKeyboard::deaiDeviceKeyboard( deAndroidInput &module ) :
deaiDevice( module, esAndroid )
{
	decString string;
	
	string.Format( "%s%dkeyboard", AINP_DEVID_PREFIX, deaiDevice::esAndroid );
	SetID( string );
	SetName( "Keyboard" );
	SetType( deInputDevice::edtKeyboard );
	
	// static mapping so far
	SetButtonCount( 112 );
	
	pSetButtonAt( 0, "softLeft", "Soft Left", AKEYCODE_SOFT_LEFT,
		deInputEvent::ekcArrowLeft, 0, 0 );
	pSetButtonAt( 1, "softRight", "Soft Right", AKEYCODE_SOFT_RIGHT,
		deInputEvent::ekcArrowRight, 0, 1 );
	
	pSetButtonAt( 2, "home", "Home", AKEYCODE_HOME, deInputEvent::ekcHome, 0, 0 );
	pSetButtonAt( 3, "back", "Back", AKEYCODE_BACK, deInputEvent::ekcEscape, 0, 0 );
	
	pSetButtonAt( 4, "0", "0", AKEYCODE_0, deInputEvent::ekc0, '0', 0 );
	pSetButtonAt( 5, "1", "1", AKEYCODE_1, deInputEvent::ekc1, '1', 0 );
	pSetButtonAt( 6, "2", "2", AKEYCODE_2, deInputEvent::ekc2, '2', 0 );
	pSetButtonAt( 7, "3", "3", AKEYCODE_3, deInputEvent::ekc3, '3', 0 );
	pSetButtonAt( 8, "4", "4", AKEYCODE_4, deInputEvent::ekc4, '4', 0 );
	pSetButtonAt( 9, "5", "5", AKEYCODE_5, deInputEvent::ekc5, '5', 0 );
	pSetButtonAt( 10, "6", "6", AKEYCODE_6, deInputEvent::ekc6, '6', 0 );
	pSetButtonAt( 11, "7", "7", AKEYCODE_7, deInputEvent::ekc7, '7', 0 );
	pSetButtonAt( 12, "8", "8", AKEYCODE_8, deInputEvent::ekc8, '8', 0 );
	pSetButtonAt( 13, "9", "9", AKEYCODE_9, deInputEvent::ekc9, '9', 0 );
	
	pSetButtonAt( 14, "dpadUp", "D-Pad Up", AKEYCODE_DPAD_UP,
		deInputEvent::ekcArrowUp, 0, 1 );
	pSetButtonAt( 15, "dpadDown", "D-Pad Down", AKEYCODE_DPAD_DOWN,
		deInputEvent::ekcArrowDown, 0, 1 );
	pSetButtonAt( 16, "dpadLeft", "D-Pad Left", AKEYCODE_DPAD_LEFT,
		deInputEvent::ekcArrowLeft, 0, 1 );
	pSetButtonAt( 17, "dpadRight", "D-Pad Right", AKEYCODE_DPAD_RIGHT,
		deInputEvent::ekcArrowRight, 0, 1 );
	
	pSetButtonAt( 18, "clear", "Clear", AKEYCODE_CLEAR, deInputEvent::ekcBackSpace, 0, 1 );
	
	pSetButtonAt( 19, "a", "A", AKEYCODE_A, deInputEvent::ekcA, 'a', 0 );
	pSetButtonAt( 20, "b", "B", AKEYCODE_B, deInputEvent::ekcB, 'b', 0 );
	pSetButtonAt( 21, "c", "C", AKEYCODE_C, deInputEvent::ekcC, 'c', 0 );
	pSetButtonAt( 22, "d", "D", AKEYCODE_D, deInputEvent::ekcD, 'd', 0 );
	pSetButtonAt( 23, "e", "E", AKEYCODE_E, deInputEvent::ekcE, 'e', 0 );
	pSetButtonAt( 24, "f", "F", AKEYCODE_F, deInputEvent::ekcF, 'f', 0 );
	pSetButtonAt( 25, "g", "G", AKEYCODE_G, deInputEvent::ekcG, 'g', 0 );
	pSetButtonAt( 26, "h", "H", AKEYCODE_H, deInputEvent::ekcH, 'h', 0 );
	pSetButtonAt( 27, "i", "I", AKEYCODE_I, deInputEvent::ekcI, 'i', 0 );
	pSetButtonAt( 28, "j", "J", AKEYCODE_J, deInputEvent::ekcJ, 'j', 0 );
	pSetButtonAt( 29, "k", "K", AKEYCODE_K, deInputEvent::ekcK, 'k', 0 );
	pSetButtonAt( 30, "l", "L", AKEYCODE_L, deInputEvent::ekcL, 'l', 0 );
	pSetButtonAt( 31, "m", "M", AKEYCODE_M, deInputEvent::ekcM, 'm', 0 );
	pSetButtonAt( 32, "n", "N", AKEYCODE_N, deInputEvent::ekcN, 'n', 0 );
	pSetButtonAt( 33, "o", "O", AKEYCODE_O, deInputEvent::ekcO, 'o', 0 );
	pSetButtonAt( 34, "p", "P", AKEYCODE_P, deInputEvent::ekcP, 'p', 0 );
	pSetButtonAt( 35, "q", "Q", AKEYCODE_Q, deInputEvent::ekcQ, 'q', 0 );
	pSetButtonAt( 36, "r", "R", AKEYCODE_R, deInputEvent::ekcR, 'r', 0 );
	pSetButtonAt( 37, "s", "S", AKEYCODE_S, deInputEvent::ekcS, 's', 0 );
	pSetButtonAt( 38, "t", "T", AKEYCODE_T, deInputEvent::ekcT, 't', 0 );
	pSetButtonAt( 39, "u", "U", AKEYCODE_U, deInputEvent::ekcU, 'u', 0 );
	pSetButtonAt( 40, "v", "V", AKEYCODE_V, deInputEvent::ekcV, 'v', 0 );
	pSetButtonAt( 41, "w", "W", AKEYCODE_W, deInputEvent::ekcW, 'w', 0 );
	pSetButtonAt( 42, "x", "X", AKEYCODE_X, deInputEvent::ekcX, 'x', 0 );
	pSetButtonAt( 43, "y", "Y", AKEYCODE_Y, deInputEvent::ekcY, 'y', 0 );
	pSetButtonAt( 44, "z", "Z", AKEYCODE_Z, deInputEvent::ekcZ, 'z', 0 );
	
	pSetButtonAt( 45, "altLeft", "Alt Left", AKEYCODE_ALT_LEFT,
		deInputEvent::ekcAlt, 0, 0 );
	pSetButtonAt( 46, "altRight", "Alt Right", AKEYCODE_ALT_RIGHT,
		deInputEvent::ekcAlt, 0, 1 );
	pSetButtonAt( 47, "shiftLeft", "Shift Left", AKEYCODE_SHIFT_LEFT,
		deInputEvent::ekcShift, 0, 0 );
	pSetButtonAt( 48, "shiftRight", "Shift Right", AKEYCODE_SHIFT_RIGHT,
		deInputEvent::ekcShift, 0, 1 );
	
	pSetButtonAt( 49, "tab", "Tab", AKEYCODE_TAB, deInputEvent::ekcTab, '\t', 0 );
	pSetButtonAt( 50, "space", "Space", AKEYCODE_SPACE, deInputEvent::ekcSpace, ' ', 0 );
	pSetButtonAt( 51, "enter", "Enter", AKEYCODE_ENTER, deInputEvent::ekcReturn, '\n', 0 );
	pSetButtonAt( 52, "del", "Delete", AKEYCODE_DEL, deInputEvent::ekcBackSpace, 0, 0 );
	pSetButtonAt( 53, "pageUp", "Page Up", AKEYCODE_PAGE_UP,
		deInputEvent::ekcPageUp, 0, 0 );
	pSetButtonAt( 54, "pageDown", "Page Down", AKEYCODE_PAGE_DOWN,
		deInputEvent::ekcPageDown, 0, 0 );
	pSetButtonAt( 55, "escape", "Escape", AKEYCODE_ESCAPE, deInputEvent::ekcEscape, 0, 0 );
	pSetButtonAt( 56, "forwardDel", "Forward Delete", AKEYCODE_FORWARD_DEL,
		deInputEvent::ekcDelete, 0, 0 );
	pSetButtonAt( 57, "ctrlLeft", "Control Left", AKEYCODE_CTRL_LEFT,
		deInputEvent::ekcControl, 0, 0 );
	pSetButtonAt( 58, "ctrlRight", "Control Right", AKEYCODE_CTRL_RIGHT,
		deInputEvent::ekcControl, 0, 1 );
	pSetButtonAt( 59, "metaLeft", "Meta Left", AKEYCODE_META_LEFT,
		deInputEvent::ekcMeta, 0, 0 );
	pSetButtonAt( 60, "metaRight", "Meta Right", AKEYCODE_META_RIGHT,
		deInputEvent::ekcMeta, 0, 1 );
	pSetButtonAt( 61, "sysRq", "SysRQ", AKEYCODE_SYSRQ, deInputEvent::ekcPause, 0, 2 );
	pSetButtonAt( 62, "break", "Break", AKEYCODE_BREAK, deInputEvent::ekcPause, 0, 1 );
	pSetButtonAt( 63, "moveHome", "Move Home", AKEYCODE_MOVE_HOME, deInputEvent::ekcHome, 0, 1 );
	pSetButtonAt( 64, "moveEnd", "Move End", AKEYCODE_MOVE_END, deInputEvent::ekcEnd, 0, 1 );
	pSetButtonAt( 65, "insert", "Insert", AKEYCODE_INSERT, deInputEvent::ekcInsert, 0, 0 );
	pSetButtonAt( 66, "f1", "F1", AKEYCODE_F1, deInputEvent::ekcF1, 0, 0 );
	pSetButtonAt( 67, "f2", "F2", AKEYCODE_F2, deInputEvent::ekcF2, 0, 0 );
	pSetButtonAt( 68, "f3", "F3", AKEYCODE_F3, deInputEvent::ekcF3, 0, 0 );
	pSetButtonAt( 69, "f4", "F4", AKEYCODE_F4, deInputEvent::ekcF4, 0, 0 );
	pSetButtonAt( 70, "f5", "F5", AKEYCODE_F5, deInputEvent::ekcF5, 0, 0 );
	pSetButtonAt( 71, "f6", "F6", AKEYCODE_F6, deInputEvent::ekcF6, 0, 0 );
	pSetButtonAt( 72, "f7", "F7", AKEYCODE_F7, deInputEvent::ekcF7, 0, 0 );
	pSetButtonAt( 73, "f8", "F8", AKEYCODE_F8, deInputEvent::ekcF8, 0, 0 );
	pSetButtonAt( 74, "f9", "F9", AKEYCODE_F9, deInputEvent::ekcF9, 0, 0 );
	pSetButtonAt( 75, "f10", "F10", AKEYCODE_F10, deInputEvent::ekcF10, 0, 0 );
	pSetButtonAt( 76, "f11", "F11", AKEYCODE_F11, deInputEvent::ekcF11, 0, 0 );
	pSetButtonAt( 77, "f12", "F12", AKEYCODE_F12, deInputEvent::ekcF12, 0, 0 );
	pSetButtonAt( 78, "numpad0", "Num-Pad 0", AKEYCODE_NUMPAD_0, deInputEvent::ekc0, '0', 1 );
	pSetButtonAt( 79, "numpad1", "Num-Pad 1", AKEYCODE_NUMPAD_1, deInputEvent::ekc1, '1', 1 );
	pSetButtonAt( 80, "numpad2", "Num-Pad 2", AKEYCODE_NUMPAD_2, deInputEvent::ekc2, '2', 1 );
	pSetButtonAt( 81, "numpad3", "Num-Pad 3", AKEYCODE_NUMPAD_3, deInputEvent::ekc3, '3', 1 );
	pSetButtonAt( 82, "numpad4", "Num-Pad 4", AKEYCODE_NUMPAD_4, deInputEvent::ekc4, '4', 1 );
	pSetButtonAt( 83, "numpad5", "Num-Pad 5", AKEYCODE_NUMPAD_5, deInputEvent::ekc5, '5', 1 );
	pSetButtonAt( 84, "numpad6", "Num-Pad 6", AKEYCODE_NUMPAD_6, deInputEvent::ekc6, '6', 1 );
	pSetButtonAt( 85, "numpad7", "Num-Pad 7", AKEYCODE_NUMPAD_7, deInputEvent::ekc7, '7', 1 );
	pSetButtonAt( 86, "numpad8", "Num-Pad 8", AKEYCODE_NUMPAD_8, deInputEvent::ekc8, '8', 1 );
	pSetButtonAt( 87, "numpad9", "Num-Pad 9", AKEYCODE_NUMPAD_9, deInputEvent::ekc9, '9', 1 );
	pSetButtonAt( 88, "numpadEnter", "Num-Pad Enter", AKEYCODE_NUMPAD_ENTER,
		deInputEvent::ekcReturn, '\n', 1 );
	
	// keys without key code
	pSetButtonAt( 89, "star", "*", AKEYCODE_STAR, deInputEvent::ekcUndefined, '*', 0 );
	pSetButtonAt( 90, "pound", "£", AKEYCODE_POUND, deInputEvent::ekcUndefined, 156/*'£'*/, 0 );
	pSetButtonAt( 91, "comma", ",", AKEYCODE_COMMA, deInputEvent::ekcUndefined, ',', 0 );
	pSetButtonAt( 92, "period", ".", AKEYCODE_PERIOD, deInputEvent::ekcUndefined, '.', 0 );
	pSetButtonAt( 93, "minus", "-", AKEYCODE_MINUS, deInputEvent::ekcUndefined, '-', 0 );
	pSetButtonAt( 94, "equals", "=", AKEYCODE_EQUALS, deInputEvent::ekcUndefined, '=', 0 );
	pSetButtonAt( 95, "bracketLeft", "[", AKEYCODE_LEFT_BRACKET,
		deInputEvent::ekcUndefined, '[', 0 );
	pSetButtonAt( 96, "bracketRight", "]", AKEYCODE_RIGHT_BRACKET,
		deInputEvent::ekcUndefined, ']', 0 );
	pSetButtonAt( 97, "backslash", "\\", AKEYCODE_BACKSLASH,
		deInputEvent::ekcUndefined, '\\', 0 );
	pSetButtonAt( 98, "semicolon", ";", AKEYCODE_SEMICOLON,
		deInputEvent::ekcUndefined, ';', 0 );
	pSetButtonAt( 99, "apostrophe", "'", AKEYCODE_APOSTROPHE,
		deInputEvent::ekcUndefined, '\'', 0 );
	pSetButtonAt( 100, "slash", "/", AKEYCODE_SLASH, deInputEvent::ekcUndefined, '/', 0 );
	pSetButtonAt( 101, "at", "@", AKEYCODE_AT, deInputEvent::ekcUndefined, '@', 0 );
	pSetButtonAt( 102, "plus", "+", AKEYCODE_PLUS, deInputEvent::ekcUndefined, '+', 0 );
	
	pSetButtonAt( 103, "numpadDivide", "Num-Pad /", AKEYCODE_NUMPAD_DIVIDE,
		deInputEvent::ekcUndefined, '/', 1 );
	pSetButtonAt( 104, "numpadMultiply", "Num-Pad *", AKEYCODE_NUMPAD_MULTIPLY,
		deInputEvent::ekcUndefined, '*', 1 );
	pSetButtonAt( 105, "numpadSubtract", "Num-Pad -", AKEYCODE_NUMPAD_SUBTRACT,
		deInputEvent::ekcUndefined, '-', 1 );
	pSetButtonAt( 106, "numpadAdd", "Num-Pad +", AKEYCODE_NUMPAD_ADD,
		deInputEvent::ekcUndefined, '+', 1 );
	pSetButtonAt( 107, "numpadDot", "Num-Pad .", AKEYCODE_NUMPAD_DOT,
		deInputEvent::ekcUndefined, '.', 1 );
	pSetButtonAt( 108, "numpadComma", "Num-Pad ,", AKEYCODE_NUMPAD_COMMA,
		deInputEvent::ekcUndefined, ',', 1 );
	pSetButtonAt( 109, "numpadEquals", "Num-Pad =", AKEYCODE_NUMPAD_EQUALS,
		deInputEvent::ekcUndefined, '=', 1 );
	pSetButtonAt( 110, "numpadParenLeft", "Num-Pad (", AKEYCODE_NUMPAD_LEFT_PAREN,
		deInputEvent::ekcUndefined, '(', 1 );
	pSetButtonAt( 111, "numpadParenRight", "Num-Pad )", AKEYCODE_NUMPAD_RIGHT_PAREN,
		deInputEvent::ekcUndefined, ')', 1 );
	
	
	
	//pKeyCodeMappings[ AKEYCODE_UNKNOWN ] = 
	//pKeyCodeMappings[ AKEYCODE_CALL ] =
	//pKeyCodeMappings[ AKEYCODE_ENDCALL ] = 
	//pKeyCodeMappings[ AKEYCODE_DPAD_CENTER ] = 
	//pKeyCodeMappings[ AKEYCODE_VOLUME_UP ] = 
	//pKeyCodeMappings[ AKEYCODE_VOLUME_DOWN ] = 
	//pKeyCodeMappings[ AKEYCODE_POWER ] = 
	//pKeyCodeMappings[ AKEYCODE_CAMERA ] = 
	//pKeyCodeMappings[ AKEYCODE_SYM ] = 
	//pKeyCodeMappings[ AKEYCODE_EXPLORER ] = 
	//pKeyCodeMappings[ AKEYCODE_ENVELOPE ] = 
	//pKeyCodeMappings[ AKEYCODE_GRAVE ] = 
	//pKeyCodeMappings[ AKEYCODE_NUM ] = 
	//pKeyCodeMappings[ AKEYCODE_HEADSETHOOK ] = 
	//pKeyCodeMappings[ AKEYCODE_FOCUS ] = // *Camera* focus
	//pKeyCodeMappings[ AKEYCODE_MENU ] = 
	//pKeyCodeMappings[ AKEYCODE_NOTIFICATION ] = 
	//pKeyCodeMappings[ AKEYCODE_SEARCH ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_PLAY_PAUSE ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_STOP ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_NEXT ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_PREVIOUS ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_REWIND ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_FAST_FORWARD ] = 
	//pKeyCodeMappings[ AKEYCODE_MUTE ] = 
	//pKeyCodeMappings[ AKEYCODE_PICTSYMBOLS ] = 
	//pKeyCodeMappings[ AKEYCODE_SWITCH_CHARSET ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_A ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_B ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_C ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_X ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_Y ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_Z ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_L1 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_R1 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_L2 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_R2 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_THUMBL ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_THUMBR ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_START ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_SELECT ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_MODE ] = 
	//pKeyCodeMappings[ AKEYCODE_CAPS_LOCK ] = 
	//pKeyCodeMappings[ AKEYCODE_SCROLL_LOCK ] = 
	//pKeyCodeMappings[ AKEYCODE_FUNCTION ] = 
	//pKeyCodeMappings[ AKEYCODE_FORWARD ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_PLAY ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_PAUSE ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_CLOSE ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_EJECT ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_RECORD ] = 
	//pKeyCodeMappings[ AKEYCODE_NUM_LOCK ] = 
	//pKeyCodeMappings[ AKEYCODE_VOLUME_MUTE ] = 
	//pKeyCodeMappings[ AKEYCODE_INFO ] = 
	//pKeyCodeMappings[ AKEYCODE_CHANNEL_UP ] = 
	//pKeyCodeMappings[ AKEYCODE_CHANNEL_DOWN ] = 
	//pKeyCodeMappings[ AKEYCODE_ZOOM_IN ] = 
	//pKeyCodeMappings[ AKEYCODE_ZOOM_OUT ] = 
	//pKeyCodeMappings[ AKEYCODE_TV ] = 
	//pKeyCodeMappings[ AKEYCODE_WINDOW ] = 
	//pKeyCodeMappings[ AKEYCODE_GUIDE ] = 
	//pKeyCodeMappings[ AKEYCODE_DVR ] = 
	//pKeyCodeMappings[ AKEYCODE_BOOKMARK ] = 
	//pKeyCodeMappings[ AKEYCODE_CAPTIONS ] = 
	//pKeyCodeMappings[ AKEYCODE_SETTINGS ] = 
	//pKeyCodeMappings[ AKEYCODE_TV_POWER ] = 
	//pKeyCodeMappings[ AKEYCODE_TV_INPUT ] = 
	//pKeyCodeMappings[ AKEYCODE_STB_POWER ] = 
	//pKeyCodeMappings[ AKEYCODE_STB_INPUT ] = 
	//pKeyCodeMappings[ AKEYCODE_AVR_POWER ] = 
	//pKeyCodeMappings[ AKEYCODE_AVR_INPUT ] = 
	//pKeyCodeMappings[ AKEYCODE_PROG_RED ] = 
	//pKeyCodeMappings[ AKEYCODE_PROG_GREEN ] = 
	//pKeyCodeMappings[ AKEYCODE_PROG_YELLOW ] = 
	//pKeyCodeMappings[ AKEYCODE_PROG_BLUE ] = 
	//pKeyCodeMappings[ AKEYCODE_APP_SWITCH ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_1 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_2 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_3 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_4 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_5 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_6 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_7 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_8 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_9 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_10 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_11 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_12 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_13 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_14 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_15 ] = 
	//pKeyCodeMappings[ AKEYCODE_BUTTON_16 ] = 
	//pKeyCodeMappings[ AKEYCODE_LANGUAGE_SWITCH ] = 
	//pKeyCodeMappings[ AKEYCODE_MANNER_MODE ] = 
	//pKeyCodeMappings[ AKEYCODE_3D_MODE ] = 
	//pKeyCodeMappings[ AKEYCODE_CONTACTS ] = 
	//pKeyCodeMappings[ AKEYCODE_CALENDAR ] = 
	//pKeyCodeMappings[ AKEYCODE_MUSIC ] = 
	//pKeyCodeMappings[ AKEYCODE_CALCULATOR ] = 
	//pKeyCodeMappings[ AKEYCODE_ZENKAKU_HANKAKU ] = 
	//pKeyCodeMappings[ AKEYCODE_EISU ] = 
	//pKeyCodeMappings[ AKEYCODE_MUHENKAN ] = 
	//pKeyCodeMappings[ AKEYCODE_HENKAN ] = 
	//pKeyCodeMappings[ AKEYCODE_KATAKANA_HIRAGANA ] = 
	//pKeyCodeMappings[ AKEYCODE_YEN ] = 
	//pKeyCodeMappings[ AKEYCODE_RO ] = 
	//pKeyCodeMappings[ AKEYCODE_KANA ] = 
	//pKeyCodeMappings[ AKEYCODE_ASSIST ] = 
	//pKeyCodeMappings[ AKEYCODE_BRIGHTNESS_DOWN ] = 
	//pKeyCodeMappings[ AKEYCODE_BRIGHTNESS_UP ] = 
	//pKeyCodeMappings[ AKEYCODE_MEDIA_AUDIO_TRACK ] = 
}

deaiDeviceKeyboard::~deaiDeviceKeyboard(){
}



// Management
///////////////

int deaiDeviceKeyboard::ButtonMatchingKeyChar( int keyChar ) const{
	// this task is just a guess so missing a solution is fine
	const int count = GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( GetButtonAt( i ).GetAIChar() == keyChar ){
			return i;
		}
	}
	
	return -1;
}



// Private Functions
//////////////////////

void deaiDeviceKeyboard::pSetButtonAt( int index, const char *id, const char *name,
int aiCode, deInputEvent::eKeyCodes keyCode, int aiChar, int matchPriority ){
	deaiDeviceButton &button = GetButtonAt( index );
	
	button.SetID( id );
	button.SetName( name );
	button.SetAICode( aiCode );
	button.SetAIChar( aiChar );
	button.SetKeyCode( keyCode );
	button.SetMatchPriority( matchPriority );
}
