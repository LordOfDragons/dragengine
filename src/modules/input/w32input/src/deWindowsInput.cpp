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

#include "deWindowsInput.h"
#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceButton.h"
#include "dewiDeviceKeyboard.h"
#include "dewiDeviceMouse.h"
#include "dewiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSWindows.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/resources/rendering/deRenderWindow.h>



// Entry Point
////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *W32InputCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *W32InputCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deWindowsInput( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Definitions
////////////////

#define W32_KEY_PRESSED(lparam)		( ( ( lparam ) & ( 1 << 30 ) ) != 0 )
#define W32_GET_SCANCODE(lparam)	( ( ( lparam ) & 0x00ff0000 ) >> 16 )



// Class deWindowsInput
/////////////////////////

// Constructor, destructor
////////////////////////////

deWindowsInput::deWindowsInput( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ),

pOSWindows( NULL ),

pScreenWidth( 0 ),
pScreenHeight( 0 ),

pWindowWidth( 0 ),
pWindowHeight( 0 ),

pMouseButtons( 0 ),
pLastMouseX( 0 ),
pLastMouseY( 0 ),
pLastMouseModifiers( 0 ),

pIsListening( false ),

pOldAccelNom( 0 ),
pOldAccelDenom( 0 ),
pOldThreshold( 0 ),
pKeyStates( NULL ),

pSystemAutoRepeatEnabled( false ),
pAutoRepeatEnabled( false ),

pKeyModifiers( 0 )
{
	try{
		pKeyStates = new BYTE[ 256 ];
		
	}catch( const deException & ){
		throw;
	}
}

deWindowsInput::~deWindowsInput(){
	if( pKeyStates ){
		delete [] pKeyStates;
	}
}



// Management
///////////////

bool deWindowsInput::Init(){
	pOSWindows = GetOS()->CastToOSWindows();
	pMouseButtons = 0;
	pWindowWidth = 0;
	pWindowHeight = 0;
	
	pScreenWidth = GetSystemMetrics( SM_CXFULLSCREEN );
	pScreenHeight = GetSystemMetrics( SM_CYFULLSCREEN );
	
	if( pOSWindows->GetWindow() ){
		RECT clientRect;
		GetClientRect( pOSWindows->GetWindow(), &clientRect );
		pWindowWidth = clientRect.right - clientRect.left;
		pWindowHeight = clientRect.bottom - clientRect.top;
	}
	
	memset( pKeyStates, '\0', sizeof( BYTE ) * 256 );
	pKeyModifiers = 0;

	pIsListening = true;
	
	// determine if keyboard auto-repeat is enabled
	pSystemAutoRepeatEnabled = true; // figure this out somehow
	pAutoRepeatEnabled = pSystemAutoRepeatEnabled;
	
	AppActivationChanged();
	
	pDevices.TakeOver( new dewiDeviceManager( *this ) );
	pDevices->UpdateDeviceList();
	pDevices->LogDevices();
	
	return true;
}

void deWindowsInput::CleanUp(){
	pDevices = nullptr;
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
	pOSWindows = NULL;
}



// Devices
////////////

int deWindowsInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice *deWindowsInput::GetDeviceAt( int index ){
	deInputDevice * const device = new deInputDevice;
	
	try{
		pDevices->GetAt( index )->GetInfo( *device );
		
	}catch( const deException & ){
		if( device ){
			device->FreeReference();
		}
		throw;
	}
	
	return device;
}

int deWindowsInput::IndexOfDeviceWithID( const char *id ){
	return pDevices->IndexOfWithID( id );
}

int deWindowsInput::IndexOfButtonWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfButtonWithID( id );
}

int deWindowsInput::IndexOfAxisWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfAxisWithID( id );
}

int deWindowsInput::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

bool deWindowsInput::GetButtonPressed( int device, int button ){
	return pDevices->GetAt( device )->GetButtonAt( button )->GetPressed();
}

float deWindowsInput::GetAxisValue( int device, int axis ){
	return pDevices->GetAt( device )->GetAxisAt( axis )->GetValue();
}

float deWindowsInput::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deWindowsInput::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

int deWindowsInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
	if( device != pDevices->GetKeyboard()->GetIndex() ){
		return -1;
	}
	
	const dewiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	const int count = rdevice.GetButtonCount();
	int bestPriority = 10;
	int bestButton = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const dewiDeviceButton &button = *rdevice.GetButtonAt( i );
		
		if( button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority ){
			bestButton = i;
			bestPriority = button.GetMatchPriority();
		}
	}
	
	return bestButton;
}

int deWindowsInput::ButtonMatchingKeyChar( int device, int character ){
	if( device != pDevices->GetKeyboard()->GetIndex() ){
		return -1;
	}
	
	return pDevices->GetKeyboard()->ButtonMatchingKeyChar( character );
}

int deWindowsInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
deInputEvent::eKeyLocation location ){
	if( device != pDevices->GetKeyboard()->GetIndex() ){
		return -1;
	}
	
	const dewiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	const int count = rdevice.GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dewiDeviceButton &button = *rdevice.GetButtonAt( i );
		if( button.GetKeyCode() == keyCode && button.GetKeyLocation() == location ){
			return i;
		}
	}
	
	return 1;
}

int deWindowsInput::ButtonMatchingKeyChar( int device, int character, deInputEvent::eKeyLocation location ){
	if( device != pDevices->GetKeyboard()->GetIndex() ){
		return -1;
	}
	
	const dewiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	const int count = rdevice.GetButtonCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const dewiDeviceButton &button = *rdevice.GetButtonAt( i );
		if( button.GetWIChar() == character && button.GetKeyLocation() == location ){
			return i;
		}
	}
	
	return 1;
}



// Events
///////////

void deWindowsInput::ProcessEvents(){
	pQueryMousePosition( true );
	pDevices->Update();
}

void deWindowsInput::ClearEvents(){
	pQueryMousePosition( false );
}

void deWindowsInput::EventLoop( const MSG &message ){
	BYTE keyStates[ 256 ];
	WORD keyChar;
	int result;
	
	switch( message.message ){
	case WM_SIZE:
		pWindowWidth = LOWORD( message.lParam );
		pWindowHeight = HIWORD( message.wParam );
		break;
		
	case WM_ACTIVATEAPP:
		pIsListening = message.wParam == TRUE;
		AppActivationChanged();
		break;
		
// 	case WM_ACTIVATE:
// 		pIsListening = LOWORD( message.wParam ) == TRUE;
// 		AppActivationChanged();
// 		break;
		
	case WM_KEYDOWN:{
		if( W32_KEY_PRESSED( message.lParam ) ){
			break;
		}
		
		const WPARAM wParam = pMapLeftRightKeys( message.wParam, message.lParam );
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithWICode( ( int )wParam );
		if( button == -1 ){
			break;
		}
		
		const int scanCode = W32_GET_SCANCODE( message.lParam );
		if( ! GetKeyboardState( &keyStates[ 0 ] ) ){
			break;
		}

		result = ToAsciiEx( ( UINT )wParam, scanCode, &keyStates[ 0 ],
			&keyChar, 0, GetKeyboardLayout( 0 ) );
		int wichar = 0;
		if( result != 0 ){
			wichar = LOBYTE( keyChar );
		}
		
		//LogInfoFormat("KEY_DOWN %i, '%c', %i", result, LOBYTE( keyChar ), wParam );
		
		dewiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt( button );
		deviceButton.SetPressed( true );
		
		switch( deviceButton.GetKeyCode() ){
		case deInputEvent::ekcShift:
			pKeyModifiers |= deInputEvent::esmShift;
			break;

		case deInputEvent::ekcControl:
			pKeyModifiers |= deInputEvent::esmControl;
			break;

		case deInputEvent::ekcAlt:
			pKeyModifiers |= deInputEvent::esmAlt;
			break;

		default:
			break;
		}

		pAddKeyPress( pDevices->GetKeyboard()->GetIndex(), button, wichar,
			deviceButton.GetKeyCode(), pKeyModifiers, message.time );
		}break;
		
	case WM_KEYUP:{
		const WPARAM wParam = pMapLeftRightKeys( message.wParam, message.lParam );
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithWICode( ( int )wParam );
		if( button == -1 ){
			break;
		}
		
		const int scanCode = W32_GET_SCANCODE( message.lParam );
		if( ! GetKeyboardState( &keyStates[ 0 ] ) ){
			break;
		}

		result = ToAsciiEx( ( UINT )wParam, scanCode, &keyStates[ 0 ],
			&keyChar, 0, GetKeyboardLayout( 0 ) );
		int wichar = 0;
		if( result != 0 ){
			wichar = LOBYTE( keyChar );
		}
		
		dewiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt( button );
		deviceButton.SetPressed( false );
		
		switch( deviceButton.GetKeyCode() ){
		case deInputEvent::ekcShift:
			pKeyModifiers &= ~deInputEvent::esmShift;
			break;

		case deInputEvent::ekcControl:
			pKeyModifiers &= ~deInputEvent::esmControl;
			break;

		case deInputEvent::ekcAlt:
			pKeyModifiers &= ~deInputEvent::esmAlt;
			break;

		default:
			break;
		}

		pAddKeyRelease( pDevices->GetKeyboard()->GetIndex(), button, wichar,
			deviceButton.GetKeyCode(), pKeyModifiers, message.time );
		}break;
		
	case WM_LBUTTONDOWN:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 0 )->SetPressed( true );
		pAddMousePress( pDevices->GetMouse()->GetIndex(), 0,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_MBUTTONDOWN:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 2 )->SetPressed( true );
		pAddMousePress( pDevices->GetMouse()->GetIndex(), 2,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_RBUTTONDOWN:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 1 )->SetPressed( true );
		pAddMousePress( pDevices->GetMouse()->GetIndex(), 1,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_XBUTTONDOWN:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		if( GET_XBUTTON_WPARAM( message.wParam ) == XBUTTON1 ){
			pDevices->GetMouse()->GetButtonAt( 3 )->SetPressed( true );
			pAddMousePress( pDevices->GetMouse()->GetIndex(), 3,
				pLastMouseModifiers, message.time );
			
		}else if( GET_XBUTTON_WPARAM( message.wParam ) == XBUTTON2 ){
			pDevices->GetMouse()->GetButtonAt( 4 )->SetPressed( true );
			pAddMousePress( pDevices->GetMouse()->GetIndex(), 4,
				pLastMouseModifiers, message.time );
		}
		break;
		
	case WM_LBUTTONUP:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 0 )->SetPressed( false );
		pAddMouseRelease( pDevices->GetMouse()->GetIndex(), 0,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_MBUTTONUP:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 2 )->SetPressed( false );
		pAddMouseRelease( pDevices->GetMouse()->GetIndex(), 2,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_RBUTTONUP:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		pDevices->GetMouse()->GetButtonAt( 1 )->SetPressed( false );
		pAddMouseRelease( pDevices->GetMouse()->GetIndex(), 1,
			pLastMouseModifiers, message.time );
		break;
		
	case WM_XBUTTONUP:
		pLastMouseModifiers = pModifiersFromEventState( ( DWORD )message.wParam );
		if( GET_XBUTTON_WPARAM( message.wParam ) == XBUTTON1 ){
			pDevices->GetMouse()->GetButtonAt( 3 )->SetPressed( false );
			pAddMouseRelease( pDevices->GetMouse()->GetIndex(), 3,
				pLastMouseModifiers, message.time );
			
		}else if( GET_XBUTTON_WPARAM( message.wParam ) == XBUTTON2 ){
			pDevices->GetMouse()->GetButtonAt( 4 )->SetPressed( false );
			pAddMouseRelease( pDevices->GetMouse()->GetIndex(), 4,
				pLastMouseModifiers, message.time );
		}
		break;
		
	case WM_MOUSEWHEEL:
		pLastMouseModifiers = pModifiersFromEventState( GET_KEYSTATE_WPARAM( message.wParam ) );
		pDevices->GetMouse()->GetAxisAt( 2 )->IncrementWheelChange(
			GET_WHEEL_DELTA_WPARAM( message.wParam ), pLastMouseModifiers, message.time );
		pDevices->GetMouse()->SetDirtyAxesValues( true );
		break;
	
	/*case WM_MOUSEMOVE:
		inputModule->EventMouseMove( LOWORD( message.lParam ), HIWORD( message.lParam ), eventTime );
		break;*/
	
	default:
		//TranslateMessage( &message );
		//DispatchMessage( &message );
		break;
	}
}

void deWindowsInput::CaptureInputDevicesChanged(){
	if( ! pOSWindows ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}

void deWindowsInput::AppActivationChanged(){
	if( ! pOSWindows ){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition( false );
}



void deWindowsInput::AddAxisChanged( int device, int axis, float value, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeAxisMove );
	event.SetDevice( device );
	event.SetCode( axis );
	event.SetValue( value );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}

void deWindowsInput::AddButtonPressed( int device, int button, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}

void deWindowsInput::AddButtonReleased( int device, int button, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}

void deWindowsInput::AddMouseWheelChanged( int device, int axis, int x, int y,
int state, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseWheel );
	event.SetDevice( device );
	event.SetCode( axis );
	event.SetState( state );
	event.SetX( x );
	event.SetY( y );
	event.SetValue( ( float )( x + y ) );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}

void deWindowsInput::AddDevicesAttachedDetached( DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeDevicesAttachedDetached );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}




// Private Functions
//////////////////////

void deWindowsInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	POINT position;
	position.x = renderWindow->GetWidth() / 2;
	position.y = renderWindow->GetHeight() / 2;
	ClientToScreen( renderWindow->GetWindow(), &position );
	
	SetCursorPos( position.x, position.y );
}

void deWindowsInput::pQueryMousePosition( bool sendEvents ){
	const bool capture = GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	if( capture && ! pIsListening ){
		return;
	}
	
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	POINT position;
	GetCursorPos( &position );
	ScreenToClient( renderWindow->GetWindow(), &position );
	
	if( capture ){
		if( ! sendEvents ){
			return;
		}
		
		const int midX = renderWindow->GetWidth() / 2;
		const int midY = renderWindow->GetHeight() / 2;
		if( position.x == midX && position.y == midY ){
			return;
		}
		
		pAddMouseMove( pDevices->GetMouse()->GetIndex(), pLastMouseModifiers,
			( int )( position.x - midX ), ( int )( position.y - midY ), timeGetTime() );
		pCenterPointer();
		
	}else{
		/*
		if( position.x < 0 || position.x >= renderWindow->GetWidth() ){
			return;
		}
		if( position.y < 0 || position.y >= renderWindow->GetHeight() ){
			return;
		}
		*/
		
		const int diffX = position.x - pLastMouseX;
		const int diffY = position.y - pLastMouseY;
		
		pLastMouseX = position.x;
		pLastMouseY = position.y;
		
		if( ! sendEvents ){
			return;
		}
		if( diffX == 0 && diffY == 0 ){
			return;
		}
		
		pAddMouseMove( pDevices->GetMouse()->GetIndex(), pLastMouseModifiers,
			pLastMouseX, pLastMouseY, timeGetTime() );
	}
}



int deWindowsInput::pModifiersFromEventState( DWORD wParam ) const{
	int modifiers = deInputEvent::esmNone;
	
	if( ( wParam & MK_SHIFT ) == MK_SHIFT ){
		modifiers |= deInputEvent::esmShift;
	}
	if( ( wParam & MK_CONTROL ) == MK_CONTROL ){
		modifiers |= deInputEvent::esmControl;
	}
	
	return modifiers;
}

timeval deWindowsInput::pConvertEventTime( DWORD eventTime ) const{
	// this is wrong but better than nothing
	timeval converted;
	converted.tv_sec = eventTime;
	converted.tv_usec = 0;
	return converted;
}



void deWindowsInput::pAddKeyPress( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( state );
	event.SetTime( pConvertEventTime( eventTime ) );
	queue.AddEvent( event );
}

void deWindowsInput::pAddKeyRelease( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( state );
	event.SetTime( pConvertEventTime( eventTime ) );
	
	queue.AddEvent( event );
}

void deWindowsInput::pAddMousePress( int device, int button, int state, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMousePress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetState( state );
	event.SetX( 0 );
	event.SetY( 0 );
	event.SetTime( pConvertEventTime( eventTime ) );
	
	queue.AddEvent( event );
	
	pMouseButtons |= ( 1 << button );
}

void deWindowsInput::pAddMouseRelease( int device, int button, int state, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetState( state );
	event.SetX( 0 );
	event.SetY( 0 );
	event.SetTime( pConvertEventTime( eventTime ) );
	
	queue.AddEvent( event );
	
	pMouseButtons &= ~( 1 << button );
}

void deWindowsInput::pAddMouseMove( int device, int state, int x, int y, DWORD eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseMove );
	event.SetDevice( device );
	event.SetCode( 0 ); // code of x axis. y axis has to be code + 1
	event.SetState( pMouseButtons ); // event.state
	event.SetX( x );
	event.SetY( y );
	event.SetTime( pConvertEventTime( eventTime ) );
	
	queue.AddEvent( event );
}



void deWindowsInput::pUpdateAutoRepeat(){
	if( GetGameEngine()->GetInputSystem()->GetCaptureInputDevices() ){
		if( pOSWindows->GetAppActive() ){
			if( pIsListening ){
				pSetAutoRepeatEnabled( false );
				return;
			}
		}
	}
	
	pSetAutoRepeatEnabled( pSystemAutoRepeatEnabled );
}

void deWindowsInput::pSetAutoRepeatEnabled( bool enabled ){
	if( enabled == pAutoRepeatEnabled ){
		return;
	}
	
	pAutoRepeatEnabled = enabled;
}

WPARAM deWindowsInput::pMapLeftRightKeys( WPARAM virtKey, LPARAM lParam ) const{
	// this is one of the reasons I hate windows. everything sucks from one end to the other.
	// workaround from https://stackoverflow.com/questions/15966642/how-do-you-tell-lshift-apart-from-rshift-in-wm-keydown-events
	
	const UINT scancode = ( lParam & 0x00ff0000 ) >> 16;
	const int extended  = ( lParam & 0x01000000 ) != 0;
	
	switch( virtKey ){
	case VK_SHIFT:
		return MapVirtualKey( scancode, MAPVK_VSC_TO_VK_EX );
		
	case VK_CONTROL:
		return extended ? VK_RCONTROL : VK_LCONTROL;
		
	case VK_MENU:
		return extended ? VK_RMENU : VK_LMENU;
		
	default:
		// not a key we map from generic to left/right specialized just return it
		return virtKey;
	}
}
