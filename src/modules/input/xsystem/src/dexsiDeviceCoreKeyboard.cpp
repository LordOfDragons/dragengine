/* 
 * Drag[en]gine X System Input Module
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

#include "dexsiDeviceCoreKeyboard.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dexsiDeviceCoreKeyboard
//////////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceCoreKeyboard::dexsiDeviceCoreKeyboard( deXSystemInput &module ) :
dexsiDevice( module, esX11 )
{
	decString string;
	
	string.Format( "%s%dck", XINP_DEVID_PREFIX, dexsiDevice::esX11 );
	SetID( string );
	SetName( "Core Keyboard" );
	SetType( deInputDevice::edtKeyboard );
	SetDisplayImages( "keyboard" );
	
	// get key information
	Display * const display = GetModule().GetOSUnix()->GetDisplay();
	int i, minKeyCode, maxKeyCode, keySymPerKeyCode;
	KeySym *keysyms = NULL;
	
	XDisplayKeycodes( display, &minKeyCode, &maxKeyCode );
	
	deObjectReference refSharedButton;
	refSharedButton.TakeOver( new dexsiDeviceButton( module ) );
	dexsiDeviceButton &sharedButton = ( dexsiDeviceButton& )( deObject& )refSharedButton;
	sharedButton.SetDisplayImages( "key" );
	
	XKeyEvent fakeKeyEvent;
	memset( &fakeKeyEvent, 0, sizeof( fakeKeyEvent ) );
	fakeKeyEvent.display = display;
	fakeKeyEvent.type = ButtonPress;
	fakeKeyEvent.root = XDefaultRootWindow( display );
	fakeKeyEvent.window = fakeKeyEvent.root;
	
	const unsigned int fakeKeyEventStates[ 8 ] = {
		0,
		ShiftMask,
		LockMask,
		Mod1Mask,
		Mod2Mask,
		Mod3Mask,
		Mod4Mask,
		Mod5Mask
	};
	
	try{
		// up to keySymPerKeyCode key syms can be defined for each entry. the array is
		// indexed like this:
		//   (keyCode - minKeyCode) * keySymPerKeyCode + keySymIndex
		// 
		// keySymIndex is in the range 0..<keySymPerKeyCode . we only look at the first
		// defined key sym
		const int keyCodeCount = maxKeyCode - minKeyCode + 1;
		
		keysyms = XGetKeyboardMapping( display, minKeyCode, keyCodeCount, &keySymPerKeyCode );
		if( ! keysyms ){
			DETHROW( deeInvalidParam );
		}
		
		int validCount = 0;
		for( i=0; i<keyCodeCount; i++ ){
			if( keysyms[ i * keySymPerKeyCode ] != NoSymbol ){
				validCount++;
			}
		}
		
		ResetX11KeyCodeMap( minKeyCode, keyCodeCount );
		
		int buttonIndex = 0;
		deObjectReference refButton;
		for( i=0; i<keyCodeCount; i++ ){
			const KeySym keysym = keysyms[ i * keySymPerKeyCode ];
			
			if( keysym == NoSymbol ){
				continue;
			}
			
			refButton.TakeOver( new dexsiDeviceButton( module ) );
			dexsiDeviceButton &button = ( dexsiDeviceButton& )( deObject& )refButton;
			AddButton( &button );
			
			string.Format( "k%d", minKeyCode + i );
			button.SetID( string );
			
			const char * const keysymString = XKeysymToString( keysym );
			if( keysymString ){
				string = keysymString;
				if( ! string.IsEmpty() ){
					string.SetAt( 0, toupper( string.GetAt( 0 ) ) );
				}
				button.SetName( string );
				
			}else{
				string.Format( "<KeyCode %d>", minKeyCode + i );
				button.SetName( string );
			}
			
			button.SetX11Code( minKeyCode + i );
			button.SetKeyCode( KeyCodeForKeySym( keysym ) );
			button.SetKeyLocation( KeyLocationForKeySym( keysym ) );
			button.SetMatchPriority( MatchingPriorityForKeySym( keysym ) );
			
			button.SetDisplayImages( sharedButton );
			
			// try to find a representative character for this key. X11 has no useful
			// function for this so we have to do this on our own. furthermore various
			// characters can result from pressing a key. we can easily end up with a
			// non-printable one so we try a few combinations picking the first one
			// which is printable
			fakeKeyEvent.keycode = XKeysymToKeycode( display, keysym );
			int displayCharacter = 0;
			int j;
			
			for( j=0; j<8; j++ ){
				fakeKeyEvent.state = fakeKeyEventStates[ j ];
				
				char character = 0;
				KeySym charKeySym = 0;
				if( ! XLookupString( &fakeKeyEvent, &character, 1, &charKeySym, NULL )
				|| ! charKeySym || ! character ){
					continue;
				}
				
				const int ordinal = ( unsigned char )character;
				if( ordinal < 32 ){
					continue; // only printable
				}
				
				//module.LogInfoFormat( "Check '%s': %d => %d", button.GetName().GetString(), j, character );
				displayCharacter = ordinal;
				break;
			}
			
			if( displayCharacter ){
				button.SetDisplayText( decUnicodeString( displayCharacter ).GetUpper().ToUTF8() );
				
			}else{
				button.SetDisplayText( button.GetName() );
			}
			
			SetLookupX11KeyCode( button.GetX11Code(), buttonIndex++ );
		}
		
		XFree( keysyms );
		
	}catch( const deException & ){
		if( keysyms ){
			XFree( keysyms );
		}
		throw;
	}
	
}

dexsiDeviceCoreKeyboard::~dexsiDeviceCoreKeyboard(){
}



// Management
///////////////

deInputEvent::eKeyCodes dexsiDeviceCoreKeyboard::KeyCodeForKeySym( KeySym keysym ){
	switch( keysym ){
	case XK_Shift_L:
	case XK_Shift_R:
		return deInputEvent::ekcShift;
		
	case XK_Control_L:
	case XK_Control_R:
		return deInputEvent::ekcControl;
		
	case XK_Meta_L:
	case XK_Meta_R:
		return deInputEvent::ekcMeta;
		
	case XK_Alt_L:
	case XK_Alt_R:
		return deInputEvent::ekcAlt;
		
	case XK_Super_L:
	case XK_Super_R:
		return deInputEvent::ekcSuper;
		
	case XK_Hyper_L:
	case XK_Hyper_R:
		return deInputEvent::ekcHyper;
		
	case XK_BackSpace:
	case XK_Clear:
		return deInputEvent::ekcBackSpace;
		
	case XK_Tab:
	case XK_KP_Tab:
		return deInputEvent::ekcTab;
		
	case XK_Linefeed:
	case XK_Return:
	case XK_KP_Enter:
		return deInputEvent::ekcReturn;
		
	case XK_Pause:
	case XK_Sys_Req:
		return deInputEvent::ekcPause;
		
	case XK_Escape:
		return deInputEvent::ekcEscape;
		
	case XK_space:
		return deInputEvent::ekcSpace;
		
	case XK_Delete:
	case XK_KP_Delete:
		return deInputEvent::ekcDelete;
		
	case XK_Home:
	case XK_Begin:
	case XK_KP_Home:
	case XK_KP_Begin:
		return deInputEvent::ekcHome;
		
	case XK_Left:
	case XK_KP_Left:
		return deInputEvent::ekcArrowLeft;
		
	case XK_Up:
	case XK_KP_Up:
		return deInputEvent::ekcArrowUp;
		
	case XK_Right:
	case XK_KP_Right:
		return deInputEvent::ekcArrowRight;
		
	case XK_Down:
	case XK_KP_Down:
		return deInputEvent::ekcArrowDown;
		
	case XK_Page_Up:
	case XK_KP_Page_Up:
		return deInputEvent::ekcPageUp;
		
	case XK_Page_Down:
	case XK_KP_Page_Down:
		return deInputEvent::ekcPageDown;
		
	case XK_End:
	case XK_KP_End:
		return deInputEvent::ekcEnd;
		
	case XK_Insert:
	case XK_KP_Insert:
		return deInputEvent::ekcInsert;
		
	default:
		if( keysym >= XK_F1 && keysym <= XK_F12 ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekcF1 + ( keysym - XK_F1 ) );
			
		}else if( keysym >= XK_KP_F1 && keysym <= XK_KP_F4 ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekcF1 + ( keysym - XK_KP_F1 ) );
			
		}else if( keysym >= XK_KP_0 && keysym <= XK_KP_9 ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekc0 + ( keysym - XK_KP_0 ) );
			
		}else if( keysym >= XK_0 && keysym <= XK_9 ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekc0 + ( keysym - XK_0 ) );
			
		}else if( keysym >= XK_a && keysym <= XK_z ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekcA + ( keysym - XK_a ) );
			
		}else if( keysym >= XK_A && keysym <= XK_Z ){
			return ( deInputEvent::eKeyCodes )( deInputEvent::ekcA + ( keysym - XK_A ) );
		}
	}
	
	return deInputEvent::ekcUndefined;
}

deInputEvent::eKeyLocation dexsiDeviceCoreKeyboard::KeyLocationForKeySym( KeySym keysym ){
	switch( keysym ){
	case XK_Shift_L:
	case XK_Control_L:
	case XK_Meta_L:
	case XK_Alt_L:
	case XK_Super_L:
	case XK_Hyper_L:
		return deInputEvent::eklLeft;
		
	case XK_Shift_R:
	case XK_Control_R:
	case XK_Meta_R:
	case XK_Alt_R:
	case XK_Super_R:
	case XK_Hyper_R:
		return deInputEvent::eklRight;
		
	case XK_KP_Tab:
	case XK_KP_Enter:
	case XK_KP_Delete:
	case XK_KP_Home:
	case XK_KP_Begin:
	case XK_KP_Left:
	case XK_KP_Up:
	case XK_KP_Right:
	case XK_KP_Down:
	case XK_KP_Page_Up:
	case XK_KP_Page_Down:
	case XK_KP_End:
	case XK_KP_Insert:
		return deInputEvent::eklNumberPad;
		
	default:
		if( keysym >= XK_KP_F1 && keysym <= XK_KP_F4 ){
			return deInputEvent::eklNumberPad;
			
		}else if( keysym >= XK_KP_0 && keysym <= XK_KP_9 ){
			return deInputEvent::eklNumberPad;
		}
	}
	
	return deInputEvent::eklNone;
}

int dexsiDeviceCoreKeyboard::MatchingPriorityForKeySym( KeySym keysym ){
	// lower value is higher priority
	
	switch( keysym ){
	case XK_Shift_R:
	case XK_Control_R:
	case XK_Meta_R:
	case XK_Alt_R:
	case XK_Super_R:
	case XK_Hyper_R:
	case XK_BackSpace:
	case XK_Tab:
	case XK_Return:
	case XK_Pause:
	case XK_Escape:
	case XK_space:
	case XK_Delete:
	case XK_Home:
	case XK_Left:
	case XK_Up:
	case XK_Right:
	case XK_Down:
	case XK_Page_Up:
	case XK_Page_Down:
	case XK_End:
	case XK_Insert:
		return 0;
		
	case XK_Shift_L:
	case XK_Control_L:
	case XK_Meta_L:
	case XK_Alt_L:
	case XK_Super_L:
	case XK_Hyper_L:
	case XK_Clear:
	case XK_KP_Tab:
	case XK_KP_Enter:
	case XK_Sys_Req:
	case XK_KP_Delete:
	case XK_KP_Home:
	case XK_KP_Left:
	case XK_KP_Up:
	case XK_KP_Right:
	case XK_KP_Down:
	case XK_KP_Page_Up:
	case XK_KP_Page_Down:
	case XK_KP_End:
	case XK_KP_Insert:
		return 1;
		
	case XK_Linefeed:
	case XK_Begin:
	case XK_KP_Begin:
		return 2;
		
	default:
		if( keysym >= XK_F1 && keysym <= XK_F12 ){
			return 0;
			
		}else if( keysym >= XK_KP_F1 && keysym <= XK_KP_F4 ){
			return 1;
			
		}else if( keysym >= XK_KP_0 && keysym <= XK_KP_9 ){
			return 1;
			
		}else if( keysym >= XK_0 && keysym <= XK_9 ){
			return 0;
			
		}else if( keysym >= XK_a && keysym <= XK_z ){
			return 0;
			
		}else if( keysym >= XK_A && keysym <= XK_Z ){
			return 1;
		}
	}
	
	return 10;
}

int dexsiDeviceCoreKeyboard::ButtonMatchingKeyChar( int keyChar ) const{
	// this task is just a guess so missing a solution is fine. it so happens that for
	// x11 the key syms in the range 0x20 - 0xff match their correspondig key character.
	// other characters would have to be manually matched. this effort is not worth the
	// time since this method is most often used to figure out a reasonable binding
	// configuration than doing precise key scanning
	Display * const display = GetModule().GetOSUnix()->GetDisplay();
	
	if( keyChar >= 0x20 && keyChar <= 0xff ){
		const KeyCode x11code = XKeysymToKeycode( display, ( KeySym )keyChar );
		if( x11code == 0 ){
			return -1;
		}
		
		return LookupX11KeyCode( x11code );
	}
	
	return -1;
}
