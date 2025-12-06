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
#include <unistd.h>
#include <stdlib.h>

#include "debiDeviceKeyboard.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "deBeOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class debiDeviceKeyboard
//////////////////////////

// Constructor, destructor
////////////////////////////

debiDeviceKeyboard::debiDeviceKeyboard( deBeOSInput &module ) :
debiDevice( module )
{
	decString string;
	
	string.Format( "%s%dk", BEINP_DEVID_PREFIX, debiDevice::esBeOS );
	SetID( string );
	SetName( "Keyboard" );
	SetType( deInputDevice::edtKeyboard );
	
	// get key infomation
	key_map *keyMap = NULL;
	char *keyBuffer = NULL;
	get_key_map( &keyMap, &keyBuffer );
	
	try{
		// https://www.haiku-os.org/docs/api/keyboard.html
		// /boot/system/develop/headers/os/interface/InterfaceDefs.h
	
		// determine the number of usable keys
		int i, keyCount = 0;
		
		for( i=0; i<128; i++ ){
			switch( i ){
			case B_F1_KEY:
			case B_F2_KEY:
			case B_F3_KEY:
			case B_F4_KEY:
			case B_F5_KEY:
			case B_F6_KEY:
			case B_F7_KEY:
			case B_F8_KEY:
			case B_F9_KEY:
			case B_F10_KEY:
			case B_F11_KEY:
			case B_F12_KEY:
			case B_PRINT_KEY:
			case B_SCROLL_KEY:
			case B_PAUSE_KEY:
				keyCount++;
				break;
				
			default:
				if( keyMap->normal_map[ i ] < 0 ){
					continue;
				}
				keyCount++;
			}
		}
		
		// create buttons
		debiDeviceButton::Ref sharedButton(debiDeviceButton::Ref::NewWith(module));
		sharedButton->SetDisplayImages( "key" );
		
		deObjectReference refButton;
		int buttonIndex = 0;
		const char *keyCharBegin;
		char keyCharUtf8[ 5 ];
		int keyCharLen;
		
		for( i=0; i<128; i++ ){
			string.Empty();
			deInputEvent::eKeyCodes engKeyCode = deInputEvent::ekcUndefined;
			int matchPriority = 10;
			int reprChar = 0;
			
			switch( i ){
 			case B_F1_KEY:
			case B_F2_KEY:
			case B_F3_KEY:
			case B_F4_KEY:
			case B_F5_KEY:
			case B_F6_KEY:
			case B_F7_KEY:
			case B_F8_KEY:
			case B_F9_KEY:
			case B_F10_KEY:
			case B_F11_KEY:
			case B_F12_KEY:
				string.Format( "F%d", i - B_F1_KEY + 1 );
				engKeyCode = ( deInputEvent::eKeyCodes )( deInputEvent::ekcF1 + ( i - B_F1_KEY ) );
				matchPriority = 0;
				break;
				
			case B_PRINT_KEY:
				string = "Print";
				matchPriority = 0;
				break;
				
			case B_SCROLL_KEY:
				string = "Scroll";
				matchPriority = 0;
				break;
				
			case B_PAUSE_KEY:
				string = "Pause";
				engKeyCode = deInputEvent::ekcPause;
				matchPriority = 0;
				break;
				
			default:
				if( keyMap->normal_map[ i ] < 0 ){
					continue;
				}
				
				keyCharBegin = keyBuffer + keyMap->normal_map[ i ];
				keyCharLen = *keyCharBegin;
				
				if( ( uint32 )i == keyMap->caps_key ){
					string = "CapsLock";
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->scroll_key ){
					string = "ScrollLock";
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->num_key ){
					string = "NumLock";
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->left_shift_key ){
					string = "Shift Left";
					engKeyCode = deInputEvent::ekcShift;
					matchPriority = 1;
					
				}else if( ( uint32 )i == keyMap->right_shift_key ){
					string = "Shift Right";
					engKeyCode = deInputEvent::ekcShift;
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->left_command_key ){
					string = "Command Left";
					engKeyCode = deInputEvent::ekcMeta;
					matchPriority = 1;
					
				}else if( ( uint32 )i == keyMap->right_command_key ){
					string = "Command Right";
					engKeyCode = deInputEvent::ekcMeta;
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->left_control_key ){
					string = "Control Left";
					engKeyCode = deInputEvent::ekcControl;
					matchPriority = 1;
					
				}else if( ( uint32 )i == keyMap->right_control_key ){
					string = "Control Right";
					engKeyCode = deInputEvent::ekcControl;
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->left_option_key ){
					string = "Option Left";
					engKeyCode = deInputEvent::ekcAlt;
					matchPriority = 1;
					
				}else if( ( uint32 )i == keyMap->right_option_key ){
					string = "Option Right";
					engKeyCode = deInputEvent::ekcAlt;
					matchPriority = 0;
					
				}else if( ( uint32 )i == keyMap->menu_key ){
					string = "Menu";
					matchPriority = 0;
					
				}else if( keyCharLen == 1 ){
					reprChar = ( uint8 )keyCharBegin[ 1 ];
					
					switch( ( uint8 )keyCharBegin[ 1 ] ){
					case B_BACKSPACE:
						string = "Backspace";
						engKeyCode = deInputEvent::ekcBackSpace;
						matchPriority = 0;
						break;
						
					case B_TAB:
						string = "Tab";
						engKeyCode = deInputEvent::ekcTab;
						matchPriority = 0;
						break;
						
					case B_ENTER:
						string = "Enter";
						engKeyCode = deInputEvent::ekcReturn;
						matchPriority = 0;
						break;
						
					case B_SPACE:
						string = "Space";
						engKeyCode = deInputEvent::ekcSpace;
						matchPriority = 0;
						break;
						
					case B_ESCAPE:
						string = "Escape";
						engKeyCode = deInputEvent::ekcEscape;
						matchPriority = 0;
						break;
						
					case 0xc8:
						string = "SysReq";
						matchPriority = 0;
						break;
						
					case 0xca:
						string = "Break";
						matchPriority = 0;
						break;
					
					case B_INSERT:
						string = "Insert";
						engKeyCode = deInputEvent::ekcInsert;
						matchPriority = 0;
						break;
						
					case B_HOME:
						string = "Home";
						engKeyCode = deInputEvent::ekcHome;
						matchPriority = 0;
						break;
						
					case B_PAGE_UP:
						string = "PageUp";
						engKeyCode = deInputEvent::ekcPageUp;
						matchPriority = 0;
						break;
						
					case B_DELETE:
						string = "Delete";
						engKeyCode = deInputEvent::ekcDelete;
						matchPriority = 0;
						break;
						
					case B_END:
						string = "End";
						engKeyCode = deInputEvent::ekcEnd;
						matchPriority = 0;
						break;
						
					case B_PAGE_DOWN:
						string = "PageDown";
						engKeyCode = deInputEvent::ekcPageDown;
						matchPriority = 0;
						break;
						
					case B_UP_ARROW:
						string = "Up";
						engKeyCode = deInputEvent::ekcArrowUp;
						matchPriority = 0;
						break;
						
					case B_LEFT_ARROW:
						string = "Left";
						engKeyCode = deInputEvent::ekcArrowLeft;
						matchPriority = 0;
						break;
						
					case B_DOWN_ARROW:
						string = "Down";
						engKeyCode = deInputEvent::ekcArrowDown;
						matchPriority = 0;
						break;
						
					case B_RIGHT_ARROW:
						string = "Right";
						engKeyCode = deInputEvent::ekcArrowRight;
						matchPriority = 0;
						break;
						
					case B_SUBSTITUTE:
						string = "Substitute";
						matchPriority = 1;
						break;
					}
					
					if( keyCharBegin[ 1 ] >= 'a' && keyCharBegin[ 1 ] <= 'z' ){
						engKeyCode = ( deInputEvent::eKeyCodes )(
							deInputEvent::ekcA + ( keyCharBegin[ 1 ] - 'a' ) );
						matchPriority = 0;
							
					}else if( keyCharBegin[ 1 ] >= '0' && keyCharBegin[ 1 ] <= '9' ){
						engKeyCode = ( deInputEvent::eKeyCodes )(
							deInputEvent::ekc0 + ( keyCharBegin[ 1 ] - '0' ) );
							matchPriority = 0;
					}
					
				}else if( keyCharLen > 1 ){
					// this is a unicode character. recode it into an integer since
					// matching asks for an integer. we only look at the correct
					// cases here since beos should not mess up
					if( ( keyCharBegin[ 1 ] & 0xf1 ) == 0xf0 ){ // 11110xxx
						if( keyCharLen == 4 ){
							reprChar = ( keyCharBegin[ 1 ] & 0x07 );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 2 ] & 0x3f );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 3 ] & 0x3f );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 4 ] & 0x3f );
						}
						
					}else if( ( keyCharBegin[ 1 ] & 0xf0 ) == 0xe0 ){ // 1110xxxx
						if( keyCharLen == 3 ){
							reprChar = ( keyCharBegin[ 1 ] & 0x0f );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 2 ] & 0x3f );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 3 ] & 0x3f );
						}
						
					}else if( ( keyCharBegin[ 1 ] & 0xe0 ) == 0xc0 ){ // 110xxxxx
						if( keyCharLen == 2 ){
							reprChar = ( keyCharBegin[ 1 ] & 0x1f );
							reprChar = ( reprChar << 6 ) | ( keyCharBegin[ 2 ] & 0x3f );
						}
					}
					
				}else if( keyCharLen == 0 ){
					string.Format( "Key %d", i );
				}
				
				if( string.IsEmpty() ){
					strncpy( keyCharUtf8, keyCharBegin + 1, keyCharLen );
					keyCharUtf8[ keyCharLen ] = '\0';
					string.Format( "%s", keyCharUtf8 );
				}
			}
			
			if( ! string.IsEmpty() ){
				string.SetAt( 0, toupper( string.GetAt( 0 ) ) );
			}
			
			refButton.TakeOver( new debiDeviceButton( module ) );
			debiDeviceButton &button = ( debiDeviceButton& )( deObject& )refButton;
			AddButton( &button );
			
			button.SetName( string );
			string.Format( "k%d", i );
			button.SetID( string );
			button.SetBICode( i );
			button.SetBIChar( reprChar );
			button.SetKeyCode( engKeyCode );
			button.SetMatchPriority( matchPriority );
			
			button.SetDisplayImages( sharedButton );
			button.SetDisplayText( button.GetName() );
			
			buttonIndex++;
		}
		
		// clean up
		free( keyBuffer );
		free( keyMap );
		
	}catch( const deException & ){
		if( keyBuffer ){
			free( keyBuffer );
		}
		if( keyMap ){
			free( keyMap );
		}
		throw;
	}
}

debiDeviceKeyboard::~debiDeviceKeyboard(){
}



// Management
///////////////

int debiDeviceKeyboard::ButtonMatchingKeyChar( int keyChar ) const{
	// this task is just a guess so missing a solution is fine
	const int count = GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( GetButtonAt( i )->GetBIChar() == keyChar ){
			return i;
		}
	}
	
	return -1;
}
