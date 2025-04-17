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

#include "deMacOSInput.h"
#include "demoiDevice.h"
#include "demoiDeviceAxis.h"
#include "demoiDeviceButton.h"
#include "demoiDeviceManager.h"
#include "demoiDeviceMouse.h"
#include "demoiDeviceKeyboard.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSMacOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
// #include <HIToolbox/Events.h> // not found... damn i love crappy design U_U


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *MacOSInpCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
#endif

deBaseModule *MacOSInpCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deMacOSInput( *loadableModule );
		
	}catch( const deException &e ){
		return NULL;
	}
	
	return module;
}



// Class deMacOSInput
///////////////////////

// Constructor, destructor
////////////////////////////

deMacOSInput::deMacOSInput( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ),

pOSMacOS( NULL ),

pWindowWidth( 0 ),
pWindowHeight( 0 ),

pMouseButtons( 0 ),
pLastMouseX( 0 ),
pLastMouseY( 0 ),

pIsListening( false ),

pKeyStates( NULL ),
pModifierStates( 0x100 ),
pSystemAutoRepeatEnabled( false ),
pAutoRepeatEnabled( false ),

pDevices( NULL ){
}

deMacOSInput::~deMacOSInput(){
}



// Management
///////////////

bool deMacOSInput::Init(){
	pOSMacOS = GetOS()->CastToOSMacOS();
	pMouseButtons = 0;
	pWindowWidth = 0;
	pWindowHeight = 0;
	
	if( pOSMacOS->GetWindow() ){
		NSWindow *window = pOSMacOS->GetWindow();
		NSRect frameRect = [window frame];
		pWindowWidth = ( int )( frameRect.size.width + 0.5 );
		pWindowHeight = ( int )( frameRect.size.height + 0.5 );
	}
	
	pIsListening = true;
	
	// determine if keyboard auto-repeat is enabled
	pSystemAutoRepeatEnabled = true; // figure this out using MacOS
	pAutoRepeatEnabled = pSystemAutoRepeatEnabled;
	
	AppActivationChanged();
	
	// scan for devices
	int i;
	try{
		pKeyStates = new bool[ 256 ];
		for( i=0; i<256; i++ ){
			pKeyStates[ i ] = false;
		}
		pModifierStates = 0x100;
		
		pDevices = new demoiDeviceManager( *this );
		pDevices->UpdateDeviceList();
		pDevices->LogDevices();
		
	}catch( const deException &e ){
        LogException( e );
		CleanUp();
		return false;
	}
	
	return true;
}

void deMacOSInput::CleanUp(){
	if( pDevices ){
		delete pDevices;
		pDevices = NULL;
	}
	if( pKeyStates ){
		delete [] pKeyStates;
		pKeyStates = NULL;
	}
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
	pOSMacOS = NULL;
}



// Devices
////////////

int deMacOSInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice *deMacOSInput::GetDeviceAt( int index ){
	deInputDevice *device = NULL;
	
	try{
		device = new deInputDevice;
		pDevices->GetAt( index )->GetInfo( *device );
		
	}catch( const deException & ){
		if( device ){
			device->FreeReference();
		}
		throw;
	}
	
	return device;
}

int deMacOSInput::IndexOfDeviceWithID( const char *id ){
	return pDevices->IndexOfWithID( id );
}

int deMacOSInput::IndexOfButtonWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfButtonWithID( id );
}

int deMacOSInput::IndexOfAxisWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfAxisWithID( id );
}

int deMacOSInput::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

bool deMacOSInput::GetButtonPressed( int device, int button ){
	return pDevices->GetAt( device )->GetButtonAt( button ).GetPressed();
}

float deMacOSInput::GetAxisValue( int device, int axis ){
	return pDevices->GetAt( device )->GetAxisAt( axis ).GetValue();
}

float deMacOSInput::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deMacOSInput::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

int deMacOSInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
    if( device != pDevices->GetKeyboard()->GetIndex() ){
        return -1;
    }
    
    const demoiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
    const int count = rdevice.GetButtonCount();
    int bestPriority = 10;
    int bestButton = -1;
    int i;
    
    for( i=0; i<count; i++ ){
        const demoiDeviceButton &button = rdevice.GetButtonAt( i );
        
        if( button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority ){
            bestButton = i;
            bestPriority = button.GetMatchPriority();
        }
    }
    
    return bestButton;
}

int deMacOSInput::ButtonMatchingKeyChar( int device, int character ){
    if( device != pDevices->GetKeyboard()->GetIndex() ){
        return -1;
    }
    
    return pDevices->GetKeyboard()->ButtonMatchingKeyChar( character );
}



// Events
///////////

void deMacOSInput::ProcessEvents(){
	pQueryMousePosition( true );
}

void deMacOSInput::ClearEvents(){
	pQueryMousePosition( false );
}

void deMacOSInput::EventLoop( const NSEvent &event ){
    const int eventType = [&event type];
	switch( eventType ){
	/*
	// handled by query mouse position
	case NSEventTypeMouseMoved:
	case NSEventTypeLeftMouseDragged:
	case NSEventTypeRightMouseDragged:
	case NSEventTypeOtherMouseDragged:
		break;
	*/
	
	case NSEventTypeLeftMouseDown:
	case NSEventTypeRightMouseDown:
	case NSEventTypeOtherMouseDown:{
        const int button = [&event buttonNumber];
        timeval eventTime;
        int code;
        
		gettimeofday( &eventTime, NULL );
        if( eventType == NSEventTypeLeftMouseDown ){
            code = deInputEvent::embcLeft;
            
        }else if( eventType == NSEventTypeRightMouseDown ){
            code = deInputEvent::embcRight;
            
        }else if( button < 3 ){
            code = deInputEvent::embcMiddle;
            
        }else{
            code = button;
        }
        
        pDevices->GetMouse()->GetButtonAt( code ).SetPressed( true );
		pAddMousePress( pDevices->GetMouse()->GetIndex(), code,
            pModifiersFromEvent( [&event modifierFlags] ), eventTime );
		} break;
		
	case NSEventTypeLeftMouseUp:
	case NSEventTypeRightMouseUp:
	case NSEventTypeOtherMouseUp:{
        const int button = [&event buttonNumber];
        timeval eventTime;
        int code;
        
        gettimeofday( &eventTime, NULL );
        if( eventType == NSEventTypeLeftMouseUp ){
            code = deInputEvent::embcLeft;
            
        }else if( eventType == NSEventTypeRightMouseUp ){
            code = deInputEvent::embcRight;
            
        }else if( button < 3 ){
            code = deInputEvent::embcMiddle;
            
        }else{
            code = button;
        }
        
        pDevices->GetMouse()->GetButtonAt( code ).SetPressed( false );
		pAddMouseRelease( pDevices->GetMouse()->GetIndex(), button,
            pModifiersFromEvent( [&event modifierFlags] ), eventTime );
		} break;
		
	case NSEventTypeKeyDown:{
		if( [&event isARepeat] ){
			break;
		}
		
		const int virtualKeyCode = [&event keyCode];
		if( virtualKeyCode < 0 || virtualKeyCode > 255 ){
			break;
		}
		if( pKeyStates[ virtualKeyCode ] ){
			break;
		}
		pKeyStates[ virtualKeyCode ] = true;
		
        const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( virtualKeyCode );
        if( button == -1 ){
            break;
        }
        
		const NSString * const nschars = [&event characters];
		const char *characters = NULL;
		if( nschars ){
			characters = [nschars UTF8String];
		}
		
		//LogInfoFormat( "Received keyDown: keyCode=%d modflags=%d chars=%s", virtualKeyCode, [&event modifierFlags], characters );
		
		if( ! characters || ! characters[ 0 ] ){
			break;
		}
		
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
        
        demoiDeviceButton &deviceButton = pDevices->GetKeyboard()->GetButtonAt( button );
        deviceButton.SetPressed( true );
        
		pAddKeyPress( pDevices->GetKeyboard()->GetIndex(), button, characters[ 0 ],
            deviceButton.GetKeyCode(), pModifiersFromEvent( [&event modifierFlags] ), eventTime );
		}break;
		
	case NSEventTypeKeyUp:{
		if( [&event isARepeat] ){
			break;
		}
		
		const int virtualKeyCode = [&event keyCode];
		if( virtualKeyCode < 0 || virtualKeyCode > 255 ){
			break;
		}
		if( ! pKeyStates[ virtualKeyCode ] ){
			break;
		}
		pKeyStates[ virtualKeyCode ] = false;
		
        const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( virtualKeyCode );
        if( button == -1 ){
            break;
        }
        
		const NSString * const nschars = [&event characters];
		const char *characters = NULL;
		if( nschars ){
			characters = [nschars UTF8String];
		}
		
		//LogInfoFormat( "Received keyDown: keyCode=%d modflags=%d chars=%s", virtualKeyCode, [&event modifierFlags], characters );
		
		if( ! characters || ! characters[ 0 ] ){
			break;
		}
		
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
        
        demoiDeviceButton &deviceButton = pDevices->GetKeyboard()->GetButtonAt( button );
        deviceButton.SetPressed( false );
        
        pAddKeyRelease( pDevices->GetKeyboard()->GetIndex(), button, characters[ 0 ],
            deviceButton.GetKeyCode(), pModifiersFromEvent( [&event modifierFlags] ), eventTime );
		}break;
		
	case NSEventTypeFlagsChanged:{
		const int modifierFlags = [&event modifierFlags];
		if( modifierFlags == pModifierStates ){
			break;
		}
		
		// //LogInfoFormat( "Received flagsChanged: modifier=%x states=%x", [&event modifierFlags], pModifierStates );
		
        const int device = pDevices->GetKeyboard()->GetIndex();
        
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
		const int switchedFlags = pModifierStates ^ modifierFlags;
		pModifierStates = modifierFlags;
		
		if( ( switchedFlags & NSEventModifierFlagShift ) == NSEventModifierFlagShift ){
			if( ( switchedFlags & ( 1 << 1 ) ) == ( 1 << 1 ) ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_Shift );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & ( 1 << 1 ) ) == ( 1 << 1 ) ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcShift, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcShift, 0, eventTime );
				}
				
			}else if ( ( switchedFlags & ( 1 << 2 ) ) == ( 1 << 2 ) ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_RightShift );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & ( 1 << 2 ) ) == ( 1 << 2 ) ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcShift, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcShift, 0, eventTime );
				}
			}
			
		}else if( ( switchedFlags & NSEventModifierFlagControl ) == NSEventModifierFlagControl ){
			if( ( switchedFlags & 1 ) == 1 ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_Control );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & 1 ) == 1 ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcControl, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcControl, 0, eventTime );
				}
				
			}else if ( ( switchedFlags & ( 1 << 13 ) ) == ( 1 << 13 ) ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_RightControl );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & ( 1 << 13 ) ) == ( 1 << 13 ) ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcControl, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcControl, 0, eventTime );
				}
			}
			
		}else if( ( switchedFlags & NSEventModifierFlagOption ) == NSEventModifierFlagOption ){
			if( ( switchedFlags & ( 1 << 5 ) ) == ( 1 << 5 ) ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_Option );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & ( 1 << 5 ) ) == ( 1 << 5 ) ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcAlt, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcAlt, 0, eventTime );
				}
				
			}else if ( ( switchedFlags & ( 1 << 6 ) ) == ( 1 << 6 ) ){
                const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_RightOption );
                if( button == -1 ){
                    break;
                }
                
				if( ( modifierFlags & ( 1 << 6 ) ) == ( 1 << 6 ) ){
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
					pAddKeyPress( device, button, 0, deInputEvent::ekcAlt, 0, eventTime );
					
				}else{
                    pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
					pAddKeyRelease( device, button, 0, deInputEvent::ekcAlt, 0, eventTime );
				}
			}
			
		}else if( ( switchedFlags & NSEventModifierFlagCommand ) == NSEventModifierFlagCommand ){
            const int button = pDevices->GetKeyboard()->IndexOfButtonWithMOCode( kVK_Command );
            if( button == -1 ){
                break;
            }
            
			if( ( modifierFlags & NSEventModifierFlagCommand ) == NSEventModifierFlagCommand ){
                pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( true );
				pAddKeyPress( device, button, 0, deInputEvent::ekcMeta, 0, eventTime );
				
			}else{
                pDevices->GetKeyboard()->GetButtonAt( button ).SetPressed( false );
				pAddKeyRelease( device, button, 0, deInputEvent::ekcMeta, 0, eventTime );
			}
		}
		} break;
		
	case NSEventTypeScrollWheel:{
        const int deviceIndex = pDevices->GetMouse()->GetIndex();
        const int modifiers = pModifiersFromEvent( [&event modifierFlags] );
        timeval eventTime;
        gettimeofday( &eventTime, NULL );
        
		const int deltaY = ( int )[&event scrollingDeltaY];
		if( deltaY != 0 ){
			pAddMouseWheel( deviceIndex, 3, 0, deltaY, modifiers, eventTime );
		}
        
        const int deltaX = ( int )[&event scrollingDeltaX];
        if( deltaX != 0 ){
            pAddMouseWheel( deviceIndex, 4, deltaX, 0, modifiers, eventTime );
        }
		} break;

	}
#if 0
	unsigned char theChar = 0;
	KeySym keySym = 0;
	timeval eventTime;
	int keyStateByte;
	int keyStateBit;
	int virtKey;
	
	switch( event.type ){
	case ConfigureNotify:
		pWindowWidth = event.xconfigure.width;
		pWindowHeight = event.xconfigure.height;
		break;
		
	case FocusIn:
		pIsListening = true;
		AppActivationChanged();
		break;
		
	case FocusOut:
		pIsListening = false;
		AppActivationChanged();
		break;
		
	}
#endif
}

void deMacOSInput::CaptureInputDevicesChanged(){
	if( ! pOSMacOS ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}

void deMacOSInput::AppActivationChanged(){
	if( ! pOSMacOS ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}



void deMacOSInput::AddAxisChanged( int device, int axis, float value, const timeval &eventTime ){
    deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
    deInputEvent event;
    
    event.SetType( deInputEvent::eeAxisMove );
    event.SetDevice( device );
    event.SetCode( axis );
    event.SetValue( value );
    event.SetTime( eventTime );
    queue.AddEvent( event );
}



// Private functions
//////////////////////

void deMacOSInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	NSWindow * const window = renderWindow->GetWindow();
	NSPoint mousePointer( NSMakePoint( renderWindow->GetWidth() / 2, renderWindow->GetHeight() / 2 ) );
	mousePointer = [window convertRectToScreen:(CGRect){.origin=mousePointer}].origin;
	const float screenHeight = [[NSScreen screens][0] frame].size.height;
	mousePointer.y = screenHeight - mousePointer.y;
	CGEventErr error;
	
	/*
	error = CGAssociateMouseAndMouseCursorPosition( false ); // prevent mouse events
	if( error != CGEventNoErr ){
		LogWarnFormat( "CGAssociateMouseAndMouseCursorPosition failed: %d\n", error );
	}
	*/
	
	error = CGWarpMouseCursorPosition( mousePointer );
	if( error != CGEventNoErr ){
		LogWarnFormat( "CGWarpMouseCursorPosition failed: %d\n", error );
	}
	
	/*
	error = CGAssociateMouseAndMouseCursorPosition( true ); // enable mouse events
	if( error != CGEventNoErr ){
		LogWarnFormat( "CGAssociateMouseAndMouseCursorPosition failed: %d\n", error );
	}
	*/
}

void deMacOSInput::pQueryMousePosition( bool sendEvents ){
	const bool capture = GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	
	if( capture && ! pIsListening ){
		return;
	}
	
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	if( capture ){
		if( ! sendEvents ){
			return;
		}
		
		int deltaX, deltaY;
		CGGetLastMouseDelta( &deltaX, &deltaY );
		if( deltaX == 0 && deltaY == 0 ){
			return;
		}
		
		pCenterPointer();
		
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
		//printf( "deltaX=%d deltaY=%d\n", deltaX, deltaY );
		pAddMouseMove( pDevices->GetMouse()->GetIndex(),
            pModifiersFromEvent( 0 ), deltaX, deltaY, eventTime );
		
	}else{
		NSWindow * const window = renderWindow->GetWindow();
		NSPoint mousePointer = [window mouseLocationOutsideOfEventStream];
		const int childX = ( int )( mousePointer.x + 0.5 );
		const int childY = renderWindow->GetHeight() - ( int )( mousePointer.y + 0.5 );
		
		/*
		if( childX < 0 || childX >= renderWindow->GetWidth() ){
			return;
		}
		if( childY < 0 || childY >= renderWindow->GetHeight() ){
			return;
		}
		*/
		
		const int diffX = childX - pLastMouseX;
		const int diffY = childY - pLastMouseY;
		
		pLastMouseX = childX;
		pLastMouseY = childY;
		
		if( ! sendEvents ){
			return;
		}
		if( diffX == 0 && diffY == 0 ){
			return;
		}
		
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
		pAddMouseMove( pDevices->GetMouse()->GetIndex(),
            pModifiersFromEvent( 0 ), diffX, diffY, eventTime );
	}
}

void deMacOSInput::pAddKeyPress( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( state );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deMacOSInput::pAddKeyRelease( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( state );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deMacOSInput::pAddMousePress( int device, int button, int state, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMousePress );
    event.SetDevice( device );
	event.SetCode( button );
	event.SetState( state );
	event.SetX( 0 );
	event.SetY( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
	
	pMouseButtons |= ( 1 << button );
}

void deMacOSInput::pAddMouseRelease( int device, int button, int state, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseRelease );
    event.SetDevice( device );
	event.SetCode( button );
	event.SetState( state );
	event.SetX( 0 );
	event.SetY( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
	
	pMouseButtons &= ~( 1 << button );
}

void deMacOSInput::pAddMouseMove( int device, int state, int x, int y, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseMove );
    event.SetDevice( device );
	event.SetCode( 0 ); // code of x axis. y axis has to be code + 1
	event.SetState( state );
	event.SetX( x );
	event.SetY( y );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deMacOSInput::pAddMouseWheel( int device, int axis, int x, int y, int state,
const timeval &eventTime ){
    deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
    deInputEvent event;
    
    event.SetType( deInputEvent::eeMouseWheel );
    event.SetDevice( device );
    event.SetCode( axis );
    event.SetState( state );
    event.SetX( x );
    event.SetY( y );
    event.SetValue( ( float )( x + y ) );
    event.SetTime( eventTime );
    queue.AddEvent( event );
}

int deMacOSInput::pModifiersFromEvent( int modifierFlags ) const{
	int modifiers = deInputEvent::esmNone;
	
	if( ( modifierFlags & NSEventModifierFlagShift ) == NSEventModifierFlagShift ){
		modifiers |= deInputEvent::esmShift;
	}
	if( ( modifierFlags & NSEventModifierFlagControl ) == NSEventModifierFlagControl ){
		modifiers |= deInputEvent::esmControl;
	}
	if( ( modifierFlags & NSEventModifierFlagOption ) == NSEventModifierFlagOption ){
		modifiers |= deInputEvent::esmAlt;
	}
	
	return modifiers;
}

int deMacOSInput::pModifiersFromKeyState() const{
	int modifiers = deInputEvent::esmNone;
	
	#if 0
	if( ( pKeyStates[ ( XK_Shift_L & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Shift_L & 7 ] ) != 0
	||  ( pKeyStates[ ( XK_Shift_R & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Shift_R & 7 ] ) != 0 ){
		modifiers |= deInputEvent::esmShift;
	}
	
	if( ( pKeyStates[ ( XK_Control_L & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Control_L & 7 ] ) != 0
	||  ( pKeyStates[ ( XK_Control_R & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Control_R & 7 ] ) != 0 ){
		modifiers |= deInputEvent::esmControl;
	}
	
	if( ( pKeyStates[ ( XK_Alt_L & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Alt_L & 7 ] ) != 0
	||  ( pKeyStates[ ( XK_Alt_R & 0xff ) >> 3 ] & KEYSTATE_MASK_SET[ XK_Alt_R & 7 ] ) != 0 ){
		modifiers |= deInputEvent::esmAlt;
	}
	#endif
	
	return modifiers;
}

void deMacOSInput::pUpdateAutoRepeat(){
	if( GetGameEngine()->GetInputSystem()->GetCaptureInputDevices() ){
		if( pOSMacOS->GetAppActive() ){
			if( pIsListening ){
				pSetAutoRepeatEnabled( false );
				return;
			}
		}
	}
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
}

void deMacOSInput::pSetAutoRepeatEnabled( bool enabled ){
	if( enabled == pAutoRepeatEnabled ){
		return;
	}
	
	pAutoRepeatEnabled = enabled;
	
	#if 0
	Display * const display = pOSMacOS->GetDisplay();
	if( ! display ){
		return;
	}
	
	if( enabled ){
		XAutoRepeatOn( display );
		
	}else{
		XAutoRepeatOff( display );
	}
	#endif
}
