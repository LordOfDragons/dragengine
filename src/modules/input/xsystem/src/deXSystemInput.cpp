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

#include "deXSystemInput.h"
#include "dexsiDevice.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "dexsiDeviceCoreMouse.h"
#include "dexsiDeviceCoreKeyboard.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *XSysInpCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry point
////////////////

deBaseModule *XSysInpCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deXSystemInput( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class deXSystemInput
/////////////////////////

// Constructor, destructor
////////////////////////////

deXSystemInput::deXSystemInput( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ),

pOSUnix( NULL ),

pWindowWidth( 0 ),
pWindowHeight( 0 ),

pMouseButtons( 0 ),
pLastMouseX( 0 ),
pLastMouseY( 0 ),

pIsListening( false ),

pOldAccelNom( 0 ),
pOldAccelDenom( 0 ),
pOldThreshold( 0 ),

pSystemAutoRepeatEnabled( false ),
pAutoRepeatEnabled( false ),

pDevices( NULL ),

pKeyStates( NULL ){
}

deXSystemInput::~deXSystemInput(){
}



// Management
///////////////

bool deXSystemInput::Init(){
	XWindowAttributes xwa;
	int i;
	
	pOSUnix = GetOS()->CastToOSUnix();
	pMouseButtons = 0;
	pWindowWidth = 0;
	pWindowHeight = 0;
	pOSUnix->SetEventMask( StructureNotifyMask | ExposureMask
		| EnterWindowMask | LeaveWindowMask | FocusChangeMask
		| ButtonPressMask | ButtonReleaseMask
		| KeyPressMask | KeyReleaseMask
		/* | ButtonMotionMask | MotionNotify | PointerMotionMask | PointerMotionHintMask */
	);
	
	if( pOSUnix->GetWindow() > 255 ){
		XGetWindowAttributes( pOSUnix->GetDisplay(), pOSUnix->GetWindow(), &xwa );
		pWindowWidth = xwa.width;
		pWindowHeight = xwa.height;
	}
	
	pIsListening = true;
	
	// determine if keyboard auto-repeat is enabled
	pSystemAutoRepeatEnabled = true; // figure this out using X11
	pAutoRepeatEnabled = pSystemAutoRepeatEnabled;
	
	AppActivationChanged();
	
	// scan for devices
	try{
		pKeyStates = new bool[ 256 ];
		for( i=0; i<256; i++ ){
			pKeyStates[ i ] = false;
		}
		
		pDevices = new dexsiDeviceManager( *this );
		pDevices->UpdateDeviceList();
		//pDevices->LogDevices();
		
	}catch( const deException & ){
		CleanUp();
		return false;
	}
	
	return true;
}

void deXSystemInput::CleanUp(){
	if( pDevices ){
		delete pDevices;
		pDevices = NULL;
	}
	if( pKeyStates ){
		delete [] pKeyStates;
		pKeyStates = NULL;
	}
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
	pOSUnix = NULL;
}



// Devices
////////////

int deXSystemInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice *deXSystemInput::GetDeviceAt( int index ){
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

int deXSystemInput::IndexOfDeviceWithID( const char *id ){
	return pDevices->IndexOfWithID( id );
}

int deXSystemInput::IndexOfButtonWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfButtonWithID( id );
}

int deXSystemInput::IndexOfAxisWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfAxisWithID( id );
}

int deXSystemInput::IndexOfFeedbackWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfFeedbackWithID( id );
}

bool deXSystemInput::GetButtonPressed( int device, int button ){
	return pDevices->GetAt( device )->GetButtonAt( button )->GetPressed();
}

float deXSystemInput::GetAxisValue( int device, int axis ){
	return pDevices->GetAt( device )->GetAxisAt( axis )->GetValue();
}

float deXSystemInput::GetFeedbackValue( int device, int feedback ){
	return pDevices->GetAt( device )->GetFeedbackAt( feedback )->GetValue();
}

void deXSystemInput::SetFeedbackValue( int device, int feedback, float value ){
	pDevices->GetAt( device )->GetFeedbackAt( feedback )->SetValue( value );
}

int deXSystemInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
	if( device != pDevices->GetX11CoreKeyboard()->GetIndex() ){
		return -1;
	}
	
	const dexsiDeviceCoreKeyboard &rdevice = *pDevices->GetX11CoreKeyboard();
	const int count = rdevice.GetButtonCount();
	int bestPriority = 10;
	int bestButton = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const dexsiDeviceButton &button = *rdevice.GetButtonAt( i );
		
		if( button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority ){
			bestButton = i;
			bestPriority = button.GetMatchPriority();
		}
	}
	
	return bestButton;
}

int deXSystemInput::ButtonMatchingKeyChar( int device, int character ){
	if( device != pDevices->GetX11CoreKeyboard()->GetIndex() ){
		return -1;
	}
	
	return pDevices->GetX11CoreKeyboard()->ButtonMatchingKeyChar( character );
}

int deXSystemInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
deInputEvent::eKeyLocation location ){
	if( device != pDevices->GetX11CoreKeyboard()->GetIndex() ){
		return -1;
	}
	
	const dexsiDeviceCoreKeyboard &rdevice = *pDevices->GetX11CoreKeyboard();
	const int count = rdevice.GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dexsiDeviceButton &button = *rdevice.GetButtonAt( i );
		if( button.GetKeyCode() == keyCode && button.GetKeyLocation() == location ){
			return i;
		}
	}
	
	return -1;
}

int deXSystemInput::ButtonMatchingKeyChar( int device, int character,
deInputEvent::eKeyLocation location ){
	if( device != pDevices->GetX11CoreKeyboard()->GetIndex() ){
		return -1;
	}
	if( character < 0x20 || character > 0xff ){
		return -1;
	}
	
	Display * const display = GetOSUnix()->GetDisplay();
	const KeyCode x11code = XKeysymToKeycode( display, ( KeySym )character );
	if( x11code == 0 ){
		return -1;
	}
	
	const dexsiDeviceCoreKeyboard &rdevice = *pDevices->GetX11CoreKeyboard();
	const int count = rdevice.GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dexsiDeviceButton &button = *rdevice.GetButtonAt( i );
		if( button.GetX11Code() == x11code && button.GetKeyLocation() == location ){
			return i;
		}
	}
	
	return -1;
}



// events
///////////

void deXSystemInput::ProcessEvents(){
	pQueryMousePosition( true );
	
	const int deviceCount = pDevices->GetCount();
	int i;
	for( i=0; i<deviceCount; i++ ){
		pDevices->GetAt( i )->Update();
	}
}

void deXSystemInput::ClearEvents(){
	pQueryMousePosition( false );
}

void deXSystemInput::EventLoop( XEvent &event ){
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
		
	case KeyPress:{
		const int keyCode = event.xkey.keycode;
		if( keyCode > 255 ){
			break; // should never happen
		}
		if( pKeyStates[ keyCode ] ){
			break;
		}
		pKeyStates[ keyCode ] = true;
		
		sKey key;
		if( ! pLookUpKey( event.xkey, key ) ){
			break;
		}
		
		timeval eventTime;
		eventTime.tv_sec = ( time_t )( event.xkey.time / 1000 );
		eventTime.tv_usec = ( suseconds_t )( ( event.xkey.time % 1000 ) * 1000 );
		
		dexsiDeviceButton &deviceButton = *pDevices->GetPrimaryKeyboard()->GetButtonAt( key.button );
		deviceButton.SetPressed( true );
		
		pAddKeyPress( pDevices->GetPrimaryKeyboard()->GetIndex(), key.button, key.character,
			deviceButton.GetKeyCode(), pModifiersFromXState( event.xkey.state ), eventTime );
		}break;
		
	case KeyRelease:{
		const int keyCode = event.xkey.keycode;
		if( keyCode > 255 ){
			break; // should never happen
		}
		if( ! pKeyStates[ keyCode ] ){
			break;
		}
		pKeyStates[ keyCode ] = false;
		
		sKey key;
		if( ! pLookUpKey( event.xkey, key ) ){
			break;
		}
		
		timeval eventTime;
		eventTime.tv_sec = ( time_t )( event.xkey.time / 1000 );
		eventTime.tv_usec = ( suseconds_t )( ( event.xkey.time % 1000 ) * 1000 );
		
		dexsiDeviceButton &deviceButton = *pDevices->GetPrimaryKeyboard()->GetButtonAt( key.button );
		deviceButton.SetPressed( false );
		
		pAddKeyRelease( pDevices->GetPrimaryKeyboard()->GetIndex(), key.button, key.character,
			deviceButton.GetKeyCode(), pModifiersFromXState( event.xkey.state ), eventTime );
		}break;
		
	case ButtonPress:{
		timeval eventTime;
		eventTime.tv_sec = ( time_t )( event.xbutton.time / 1000 );
		eventTime.tv_usec = ( suseconds_t )( ( event.xbutton.time % 1000 ) * 1000 );
		const int device = pDevices->GetPrimaryMouse()->GetIndex();
		const int modifiers = pModifiersFromXState( event.xbutton.state );
		
		switch( event.xbutton.button ){
		case 1:  // left mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcLeft )->SetPressed( true );
			pAddMousePress( device, deInputEvent::embcLeft, modifiers, eventTime );
			break;
			
		case 2:  // middle mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcMiddle )->SetPressed( true );
			pAddMousePress( device, deInputEvent::embcMiddle, modifiers, eventTime );
			break;
			
		case 3:  // right mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcRight )->SetPressed( true );
			pAddMousePress( device, deInputEvent::embcRight, modifiers, eventTime );
			break;
			
		case 4:  // wheel up. X11 sends one event for each raster turn
			pDevices->GetPrimaryMouse()->GetAxisAt( 2 )->IncrementWheelChange( 1, modifiers, eventTime );
			pDevices->GetPrimaryMouse()->SetDirtyAxesValues( true );
			break;
			
		case 5:  // wheel down. X11 sends one event for each raster turn
			pDevices->GetPrimaryMouse()->GetAxisAt( 2 )->IncrementWheelChange( -1, modifiers, eventTime );
			pDevices->GetPrimaryMouse()->SetDirtyAxesValues( true );
			break;
			
		case 6:  // wheel right. X11 sends one event for each raster turn
			pDevices->GetPrimaryMouse()->GetAxisAt( 3 )->IncrementWheelChange( 1, modifiers, eventTime );
			pDevices->GetPrimaryMouse()->SetDirtyAxesValues( true );
			break;
			
		case 7:  // wheel left. X11 sends one event for each raster turn
			pDevices->GetPrimaryMouse()->GetAxisAt( 3 )->IncrementWheelChange( -1, modifiers, eventTime );
			pDevices->GetPrimaryMouse()->SetDirtyAxesValues( true );
			break;
			
		case 8:  // browse backward
		case 9:  // browse forward
			pDevices->GetPrimaryMouse()->GetButtonAt( event.xbutton.button - 5 )->SetPressed( true );
			pAddMousePress( device, event.xbutton.button - 5, modifiers, eventTime );
			break;
		}
		}break;
		
	case ButtonRelease:{
		timeval eventTime;
		eventTime.tv_sec = ( time_t )( event.xbutton.time / 1000 );
		eventTime.tv_usec = ( suseconds_t )( ( event.xbutton.time % 1000 ) * 1000 );
		const int device = pDevices->GetPrimaryMouse()->GetIndex();
		const int modifiers = pModifiersFromXState( event.xbutton.state );
		
		switch( event.xbutton.button ){
		case 1:  // left mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcLeft )->SetPressed( false );
			pAddMouseRelease( device, deInputEvent::embcLeft, modifiers, eventTime );
			break;
			
		case 2:  // middle mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcMiddle )->SetPressed( false );
			pAddMouseRelease( device, deInputEvent::embcMiddle, modifiers, eventTime );
			break;
			
		case 3:  // right mouse button
			pDevices->GetPrimaryMouse()->GetButtonAt( deInputEvent::embcRight )->SetPressed( false );
			pAddMouseRelease( device, deInputEvent::embcRight, modifiers, eventTime );
			break;
			
		case 4:  // wheel up. X11 sends one event for each raster turn
		case 5:  // wheel down. X11 sends one event for each raster turn
		case 6:  // wheel right. X11 sends one event for each raster turn
		case 7:  // wheel left. X11 sends one event for each raster turn
			break;
			
		case 8:  // browse backward
		case 9:  // browse forward
			pDevices->GetPrimaryMouse()->GetButtonAt( event.xbutton.button - 5 )->SetPressed( false );
			pAddMouseRelease( device, event.xbutton.button - 5, modifiers, eventTime );
			break;
		}
		}break;
	}
}

void deXSystemInput::CaptureInputDevicesChanged(){
	if( ! pOSUnix ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}

void deXSystemInput::AppActivationChanged(){
	if( ! pOSUnix ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}



void deXSystemInput::AddAxisChanged( int device, int axis, float value, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeAxisMove );
	event.SetDevice( device );
	event.SetCode( axis );
	event.SetValue( value );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deXSystemInput::AddButtonPressed( int device, int button, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deXSystemInput::AddButtonReleased( int device, int button, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deXSystemInput::AddMouseWheelChanged( int device, int axis, int x, int y, int state,
const timeval& eventTime ){
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



// Private functions
//////////////////////

void deXSystemInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	// NOTE XWarpPointer is a multi-tool. The signature is this:
	//      XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y)
	//      The following cases are supported
	//      dest_w == None
	//        Move cursor by (dest_x,dest_y) relative to the current cursor position
	//      dest_w != None, src_w == None
	//        Set cursor to (dest_x,dest_y) relative to window dest_w
	//      dest_w != None, src_w != None
	//        Set cursor to (dest_x,dest_y) relative to window dest_w but only if all is true:
	//        - cursor is inside window src_w
	//        - cursor is inside box (src_x,src_y)-(src_width,src_height)
	Display * const display = pOSUnix->GetDisplay();
	const int x = renderWindow->GetWidth() / 2;
	const int y = renderWindow->GetHeight() / 2;
	
	XWarpPointer( display, None, renderWindow->GetWindow(), 0, 0, 0, 0, x, y );
	XFlush( display ); // required or mouse movement is missed the next time XQueryPointer is called
}

void deXSystemInput::pQueryMousePosition( bool sendEvents ){
	const bool capture = GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	if( capture && ! pIsListening ){
		return;
	}
	
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	Display * const display = pOSUnix->GetDisplay();
	Window window = renderWindow->GetWindow();
	int rootX, rootY, childX, childY;
	Window dummyRoot, dummyChild;
	unsigned int state;
	
	if( ! XQueryPointer( display, window, &dummyRoot, &dummyChild, &rootX, &rootY,
	&childX, &childY, &state ) ){
		return;
	}
	
	if( capture ){
		if( ! sendEvents ){
			return;
		}
		
		const int midX = renderWindow->GetWidth() / 2;
		const int midY = renderWindow->GetHeight() / 2;
		if( childX == midX && childY == midY ){
			return;
		}
		
		timeval eventTime;
		gettimeofday( &eventTime, NULL );
		pAddMouseMove( pDevices->GetPrimaryMouse()->GetIndex(),
			pModifiersFromXState( state ), childX - midX, childY - midY, eventTime );
		pCenterPointer();
		
	}else{
		if( childX < 0 || childX >= renderWindow->GetWidth() ){
			return;
		}
		if( childY < 0 || childY >= renderWindow->GetHeight() ){
			return;
		}
		
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
		pAddMouseMove( pDevices->GetPrimaryMouse()->GetIndex(),
			pModifiersFromXState( state ), pLastMouseX, pLastMouseY, eventTime );
	}
}

void deXSystemInput::pAddKeyPress( int device, int button, int keyChar,
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

void deXSystemInput::pAddKeyRelease( int device, int button, int keyChar,
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

void deXSystemInput::pAddMousePress( int device, int button, int state,
const timeval &eventTime ){
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

void deXSystemInput::pAddMouseRelease( int device, int button, int state,
const timeval &eventTime ){
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

void deXSystemInput::pAddMouseMove( int device, int state, int x, int y,
const timeval &eventTime ){
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

int deXSystemInput::pModifiersFromXState( int xstate ) const{
	int modifiers = deInputEvent::esmNone;
	
	if( ( xstate & ShiftMask ) == ShiftMask ){
		modifiers |= deInputEvent::esmShift;
	}
	if( ( xstate & ControlMask ) == ControlMask ){
		modifiers |= deInputEvent::esmControl;
	}
	if( ( xstate & LockMask ) == LockMask ){
		modifiers |= deInputEvent::esmAlt;
	}
	
	/*
	// these seem to be used with button groups. not important for us.
	if( ( xstate & Mod1Mask ) == Mod1Mask ){
		modifiers |= deInputEvent::esmMeta;
	}
	if( ( xstate & Mod2Mask ) == Mod2Mask ){
		modifiers |= deInputEvent::esmSuper;
	}
	if( ( xstate & Mod3Mask ) == Mod3Mask ){
		modifiers |= deInputEvent::esmHyper;
	}
	*/
	
	return modifiers;
}

bool deXSystemInput::pLookUpKey( XKeyEvent &event, deXSystemInput::sKey &key ){
	key.button = pDevices->GetPrimaryKeyboard()->LookupX11KeyCode( event.keycode );
	if( key.button == -1 ){
		return false;
	}
	
	key.virtualKeyCode = XLookupKeysym( &event, 0 );
	if( key.virtualKeyCode == NoSymbol ){
		return false;
	}
	
	key.keySym = 0;
	key.character = 0;
	
	char utf8[ 4 ];
	const int count = XLookupString( &event, ( char* )&utf8, 4, &key.keySym, nullptr );
// 	LogInfoFormat("lookUpKey: %d %d %d %d %d\n", count, utf8[0], utf8[1], utf8[2], utf8[3]);
	
	switch( count ){
	case 1:
		// this is a huge hack here. on some systems XLookupString returns for characters
		// larger than 127 a UTF-8 character composed of 2 bytes. on other systems
		// XLookupString returns an ASCII-8 character composed of 1 byte. for the UTF-8
		// case we would have to do "utf8[ 0 ] & 0x7f" to get a correctly encoded character.
		// due to other systems returning a single ASCII-8 this would break. if we do not
		// apply the mask and simply use the byte as-is we can get both systems working.
		// the conversation to unsigned char is required or it breaks.
// 		key.character = utf8[ 0 ] & 0x7f;
		key.character = ( unsigned char )utf8[ 0 ];
		break;
		
	case 2:
		key.character = ( ( utf8[ 0 ] & 0x1f ) << 6 ) + ( utf8[ 1 ] & 0x3f );
		break;
		
	case 3:
		key.character = ( ( utf8[ 0 ] & 0xf ) << 12 ) + ( ( utf8[ 1 ] & 0x3f ) << 6 )
			+ ( utf8[ 2 ] & 0x3f );
		break;
		
	case 4:
		key.character = ( ( utf8[ 0 ] & 0x7 ) << 18 ) + ( ( utf8[ 1 ] & 0x3f ) << 12 )
			+ ( ( utf8[ 2 ] & 0x3f ) << 6 ) + ( utf8[ 3 ] & 0x3f );
		break;
		
	default:
		key.character = 0;
	}
	
	return true;
}

void deXSystemInput::pUpdateAutoRepeat(){
	if( GetGameEngine()->GetInputSystem()->GetCaptureInputDevices() ){
		if( pOSUnix->GetAppActive() ){
			if( pIsListening ){
				pSetAutoRepeatEnabled( false );
				return;
			}
		}
	}
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
}

void deXSystemInput::pSetAutoRepeatEnabled( bool enabled ){
	if( enabled == pAutoRepeatEnabled ){
		return;
	}
	
	pAutoRepeatEnabled = enabled;
	
	Display * const display = pOSUnix->GetDisplay();
	if( ! display ){
		return;
	}
	
	if( enabled ){
		XAutoRepeatOn( display );
		
	}else{
		XAutoRepeatOff( display );
	}
	
	//XSync( display, False ); // to make sure it is applied
}
