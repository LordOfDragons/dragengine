/* 
 * Drag[en]gine MacOS Input Module
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

#include "demoiDevice.h"
#include "demoiDeviceKeyboard.h"
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "deMacOSInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
// #include <HIToolbox/Events.h> // not found... damn i love crappy design U_U


// Class demoiDeviceKeyboard
///////////////////////////

// Constructor, destructor
////////////////////////////

demoiDeviceKeyboard::demoiDeviceKeyboard( deMacOSInput &module ) :
demoiDevice( module, esMO )
{
    decString string;
    
    string.Format( "%s%dk", MOINP_DEVID_PREFIX, demoiDevice::esMO );
    SetID( string );
    SetName( "Keyboard" );
    SetType( deInputDevice::edtKeyboard );
    
    // this is a mess. Apple documentation on all this is so bad (if existing at
    // all) it just plain out hurts. if somebody knows a better method he is
    // welcome to tell me
    
    // get input source. according to sources on the internet this can fail for
    // non-ASCII keyboards like asien keyboards. No idea what kinds of keyboard
    // are affected by this could be affected so I just try a fallback
    //
    // WARNING! do not delete kbDataRef or segfault happens
    TISInputSourceRef kbInpSource = TISCopyCurrentKeyboardInputSource();
    
    CFDataRef kbDataRef = ( CFDataRef )TISGetInputSourceProperty(
        kbInpSource, kTISPropertyUnicodeKeyLayoutData );
    if( ! kbDataRef ){
        CFRelease( kbInpSource );
        kbInpSource = TISCopyCurrentASCIICapableKeyboardLayoutInputSource();
        kbDataRef = ( CFDataRef )TISGetInputSourceProperty(
            kbInpSource, kTISPropertyUnicodeKeyLayoutData );
    }
    
    if( ! kbDataRef ){
        CFRelease( kbInpSource );
        DETHROW( deeInvalidParam );
    }
    
    // this is nebulous. documentation claims UCKeyboardLayout contains tables but
    // not where or how. there exists a translation method which we use to probe
    // all possible key codes for a hit. according to documentation only 0 - 0x7f
    // are valid key codes so we try those. not far away from other OS so it seems
    // a reasonable value.
    const UCKeyboardLayout * const kbLayout = ( const UCKeyboardLayout *)
        CFDataGetBytePtr( kbDataRef );
    if( ! kbLayout ){
        CFRelease( kbInpSource );
        DETHROW( deeInvalidParam );
    }
    
    UniCharCount unicodeLen = 0;
    UniChar unicode[ 4 ];
    int i;
    
    try{
        const UInt kbType = LMGetKbdType();
        
        // count the number of mapped key codes
        int validCount = 0;
        
        for( i=0; i<128; i++ ){
            UInt32 dummyDeadKeyState = 0; // init to zero is mandatory
            if( UCKeyTranslate( kbLayout, i, kUCKeyActionDown, 0, kbType, 0,
            &dummyDeadKeyState, 4, &unicodeLen, unicode) == noErr ){
                validCount++;
            }
        }
        
        // add buttons for assigned keys
        SetButtonCount( validCount );
        
        int buttonIndex = 0;
        for( i=0; i<128; i++ ){
            UInt32 dummyDeadKeyState = 0; // init to zero is mandatory
            if( UCKeyTranslate( kbLayout, i, kUCKeyActionDown, 0, kbType, 0,
            &dummyDeadKeyState, 4, &unicodeLen, unicode) != noErr ){
                break;
            }
            
            demoiDeviceButton &button = GetButtonAt( buttonIndex );
            
            string.Format( "k%d", i );
            button.SetID( string );
            
            string = NameForMOCode( i );
            GetModule().LogInfoFormat("NameForMOCode(%d)=%s", i, string.GetString());
            if( string.IsEmpty() ){
                NSString * const utf8 = [NSString stringWithCharacters:unicode
                    length:(NSUInteger)unicodeLen];
                
                if( utf8 && unicodeLen > 0 ){
                    string = [utf8 UTF8String];
                    
                    if( ! string.IsEmpty() && isprint( string.GetAt( 0 ) ) ){
                        string.SetAt( 0, toupper( string.GetAt( 0 ) ) );
                        
                    }else{
                        if( ! string.IsEmpty() ){
                            GetModule().LogInfoFormat( "Non-printable character %d for keycode %d",
                                string.GetAt( 0 ), i );
                        }
                        string.Format( "<KeyCode %d>", i );
                    }
                    
                }else{
                    string.Format( "<KeyCode %d>", i );
                }
            }
            button.SetName( string );

            button.SetMOCode( i );
            button.SetKeyCode( KeyCodeForMOCode( i ) );
            button.SetMatchPriority( MatchingPriorityForMOCode( i ) );
            
            buttonIndex++;
        }
        
        CFRelease( kbInpSource );
        
    }catch( const deException & ){
        CFRelease( kbInpSource );
        throw;
    }
}

demoiDeviceKeyboard::~demoiDeviceKeyboard(){
}



// Management
///////////////

deInputEvent::eKeyCodes demoiDeviceKeyboard::KeyCodeForMOCode( int code ){
    switch( code ){
    case kVK_Shift:
    case kVK_RightShift:
        return deInputEvent::ekcShift;
        
    case kVK_Control:
    case kVK_RightControl:
        return deInputEvent::ekcControl;
        
    case kVK_Option:
    case kVK_RightOption:
        return deInputEvent::ekcMeta;
        
    case kVK_Command:
    case kVK_Function:
        return deInputEvent::ekcAlt;
        
    case kVK_Delete:
        return deInputEvent::ekcBackSpace;
        
    case kVK_Tab:
        return deInputEvent::ekcTab;
        
    case kVK_Return:
    case kVK_ANSI_KeypadEnter:
        return deInputEvent::ekcReturn;
        
    case kVK_F15:
        return deInputEvent::ekcPause;
        
    case kVK_Escape:
        return deInputEvent::ekcEscape;
        
    case kVK_Space:
        return deInputEvent::ekcSpace;
        
    case kVK_ForwardDelete:
        return deInputEvent::ekcDelete;
        
    case kVK_Home:
        return deInputEvent::ekcHome;
        
    case kVK_LeftArrow:
        return deInputEvent::ekcArrowLeft;
        
    case kVK_UpArrow:
        return deInputEvent::ekcArrowUp;
        
    case kVK_RightArrow:
        return deInputEvent::ekcArrowRight;
        
    case kVK_DownArrow:
        return deInputEvent::ekcArrowDown;
        
    case kVK_PageUp:
        return deInputEvent::ekcPageUp;
        
    case kVK_PageDown:
        return deInputEvent::ekcPageDown;
        
    case kVK_End:
        return deInputEvent::ekcEnd;
        
    case kVK_Help:
        return deInputEvent::ekcInsert;
        
    default:
        if( code >= kVK_F1 && code <= kVK_F12 ){
            return ( deInputEvent::eKeyCodes )( deInputEvent::ekcF1 + ( code - kVK_F1 ) );
            
        }else if( code >= kVK_ANSI_0 && code <= kVK_ANSI_9 ){
            return ( deInputEvent::eKeyCodes )( deInputEvent::ekc0 + ( code - kVK_ANSI_0 ) );
            
        }else if( code >= kVK_ANSI_A && code <= kVK_ANSI_Z ){
            return ( deInputEvent::eKeyCodes )( deInputEvent::ekcA + ( code - kVK_ANSI_A ) );
            
        }else if( code >= kVK_ANSI_Keypad0 && code <= kVK_ANSI_Keypad9 ){
            return ( deInputEvent::eKeyCodes )( deInputEvent::ekc0 + ( code - kVK_ANSI_Keypad0 ) );
        }
    }
    
    return deInputEvent::ekcUndefined;
}

decString demoiDeviceKeyboard::NameForMOCode( int code ){
    switch( code ){
    case kVK_Shift:
        return "Shift Left";
        
    case kVK_RightShift:
        return "Shift Right";
        
    case kVK_Control:
        return "Control Left";
        
    case kVK_RightControl:
        return "Control Right";
        
    case kVK_Option:
        return "Option Left";
        
    case kVK_RightOption:
        return "Option Right";
        
    case kVK_Command:
        return "Command";
        
    case kVK_Function:
        return "Function";
        
    case kVK_Delete:
        return "Delete";
        
    case kVK_Tab:
        return "Tab";
        
    case kVK_Return:
        return "Return";
        
    case kVK_ANSI_KeypadEnter:
        return "Keypad Return";
        
    case kVK_F15:
        return "Pause";
        
    case kVK_Escape:
        return "Escape";
        
    case kVK_Space:
        return "Space";
        
    case kVK_ForwardDelete:
        return "Forward Delete";
        
    case kVK_Home:
        return "Home";
        
    case kVK_LeftArrow:
        return "Arrow Left";
        
    case kVK_UpArrow:
        return "Arrow Up";
        
    case kVK_RightArrow:
        return "Arrow Right";
        
    case kVK_DownArrow:
        return "Arrow Down";
        
    case kVK_PageUp:
        return "Page Up";
        
    case kVK_PageDown:
        return "Page Down";
        
    case kVK_End:
        return "End";
        
    case kVK_Help:
        return "Help";
        
    default:
        if( code >= kVK_F1 && code <= kVK_F12 ){
            decString string;
            string.Format( "F%d", code - kVK_F1 );
            return string;
            
        }else if( code >= kVK_ANSI_0 && code <= kVK_ANSI_9 ){
            decString string;
            string.Format( "%d", code - kVK_ANSI_0 );
            return string;
            
        }else if( code >= kVK_ANSI_A && code <= kVK_ANSI_Z ){
            decString string;
            string.Format( "%c", 'A' + ( code - kVK_ANSI_A ) );
            return string;
            
        }else if( code >= kVK_ANSI_Keypad0 && code <= kVK_ANSI_Keypad9 ){
            decString string;
            string.Format( "Keypad %d", code - kVK_ANSI_Keypad0 );
            return string;
        }
    }
    
    return decString();
}

int demoiDeviceKeyboard::MatchingPriorityForMOCode( int code ) const{
    // lower value is higher priority
    
    switch( code ){
        case kVK_RightShift:
        case kVK_RightControl:
        case kVK_RightOption:
        case kVK_Function:
        case kVK_Delete:
        case kVK_Tab:
        case kVK_Return:
        case kVK_F15:
        case kVK_Escape:
        case kVK_Space:
        case kVK_ForwardDelete:
        case kVK_Home:
        case kVK_LeftArrow:
        case kVK_UpArrow:
        case kVK_RightArrow:
        case kVK_DownArrow:
        case kVK_PageUp:
        case kVK_PageDown:
        case kVK_End:
        case kVK_Help:
            return 0;
            
        case kVK_Shift:
        case kVK_Control:
        case kVK_Option:
        case kVK_Command:
        case kVK_ANSI_KeypadEnter:
            return 1;
            
        default:
            if( code >= kVK_F1 && code <= kVK_F12 ){
                return 0;
                
            }else if( code >= kVK_ANSI_0 && code <= kVK_ANSI_9 ){
                return 0;
                
            }else if( code >= kVK_ANSI_A && code <= kVK_ANSI_Z ){
                return 1;

            }else if( code >= kVK_ANSI_Keypad0 && code <= kVK_ANSI_Keypad9 ){
                return 1;
            }
    }
    
    return 10;
}

int demoiDeviceKeyboard::ButtonMatchingKeyChar( int keyChar ) const{
    const int count = GetButtonCount();
    int i;
    
    for( i=0; i<count; i++ ){
        if( GetButtonAt( i ).GetMOChar() == keyChar ){
            return i;
        }
    }
    
    return -1;
}
