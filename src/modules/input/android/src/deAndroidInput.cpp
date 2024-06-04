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

#include "deAndroidInput.h"
#include "deaiDevice.h"
#include "deaiDeviceAxis.h"
#include "deaiDeviceButton.h"
#include "deaiDeviceKeyboard.h"
#include "deaiDeviceMouse.h"
#include "deaiDeviceManager.h"
#include "overlay/deaiOverlaySystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSAndroid.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *AndroidInpCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry point
////////////////

deBaseModule *AndroidInpCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deAndroidInput( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deAndroidInput
//////////////////////////

// Constructor, destructor
////////////////////////////

deAndroidInput::deAndroidInput( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ),

pOSAndroid( NULL ),

pMouseButtons( 0 ),
pIsListening( false ),
pKeyStates( NULL ),
pPointerMouse( -1 ),

pOverlaySystem( NULL ),

pFontDefault( NULL ),

pElapsedTime( 0.0f ),

pDevices( NULL ){
}

deAndroidInput::~deAndroidInput(){
}



// Management
///////////////

bool deAndroidInput::Init(){
	try{
		pOSAndroid = GetGameEngine()->GetOS()->CastToOSAndroid();
		
		pFontDefault = GetGameEngine()->GetFontManager()->LoadFont(
			&GetVFS(), "/share/fonts/nimbus_sans_30.defont", "/" );
		//	"/share/fonts/nimbus_sans_30_bold.defont", "/" );
		
		pKeyStates = new bool[ 256 ];
		
		pDevices = new deaiDeviceManager( *this );
		pDevices->UpdateDeviceList();
		pDevices->LogDevices();
		
		pCenterPointer();
		pIsListening = true;
		
		pOverlaySystem = new deaiOverlaySystem( *this );
		GetGameEngine()->GetGraphicSystem()->SetInputOverlayCanvas( pOverlaySystem->GetCanvas() );
		
		pInputTimer.Reset();
		pElapsedTime = 0.0f;
		
	}catch( const deException & ){
		CleanUp();
		if( pDevices ){
			delete pDevices;
			pDevices = NULL;
		}
		return false;
	}
	
	return true;
}

void deAndroidInput::CleanUp(){
	if( pOverlaySystem ){
		GetGameEngine()->GetGraphicSystem()->SetInputOverlayCanvas( NULL );
		delete pOverlaySystem;
		pOverlaySystem = NULL;
	}
	
	if( pFontDefault ){
		pFontDefault->FreeReference();
		pFontDefault = NULL;
	}
	
	if( pDevices ){
		delete pDevices;
		pDevices = NULL;
	}
	
	if( pKeyStates ){
		delete [] pKeyStates;
		pKeyStates = NULL;
	}
	
	pOSAndroid = NULL;
}



decPoint deAndroidInput::GetScreenSize() const{
	if( ! pOSAndroid ){
		return decPoint( 1024, 768 );
	}
	
	const decPoint &res = pOSAndroid->GetDisplayCurrentResolution( 0 );
	return decPoint( res.x, res.y );
}



// Devices
////////////

int deAndroidInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice *deAndroidInput::GetDeviceAt( int index ){
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

int deAndroidInput::IndexOfDeviceWithID( const char *id ){
	return pDevices->IndexOfWithID( id );
}

int deAndroidInput::IndexOfButtonWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfButtonWithID( id );
}

int deAndroidInput::IndexOfAxisWithID( int device, const char *id ){
	return pDevices->GetAt( device )->IndexOfAxisWithID( id );
}

int deAndroidInput::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

bool deAndroidInput::GetButtonPressed( int device, int button ){
	return pDevices->GetAt( device )->GetButtonAt( button ).GetPressed();
}

float deAndroidInput::GetAxisValue( int device, int axis ){
	return pDevices->GetAt( device )->GetAxisAt( axis ).GetValue();
}

float deAndroidInput::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deAndroidInput::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

int deAndroidInput::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
	const deaiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	if( device != rdevice.GetIndex() ){
		return -1;
	}
	
	const int count = rdevice.GetButtonCount();
	int bestPriority = 10;
	int bestButton = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const deaiDeviceButton &button = rdevice.GetButtonAt( i );
		
		if( button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority ){
			bestButton = i;
			bestPriority = button.GetMatchPriority();
		}
	}
	
	return bestButton;
}

int deAndroidInput::ButtonMatchingKeyChar( int device, int character ){
	const deaiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	if( device != rdevice.GetIndex() ){
		return -1;
	}
	
	return rdevice.ButtonMatchingKeyChar( character );
}



// Events
///////////

void deAndroidInput::ProcessEvents(){
	// update elapsed time. we do this independently of the game engine to ensure
	// our own timing in all situations. the elapsed time is clamped in case a
	// stuttering to avoid exploding input
	pElapsedTime = decMath::min( pInputTimer.GetElapsedTime(), 0.1f );
	
	// update content. this can produce input events for the game engine
	pOverlaySystem->UpdateContent();
}

void deAndroidInput::ClearEvents(){
	pCenterPointer();
	pInputTimer.Reset();
	pElapsedTime = 0.0f;
}

void deAndroidInput::ScreenSizeChanged(){
	pOverlaySystem->ScreenSizeChanged();
}

void deAndroidInput::EventLoop( const AInputEvent &event ){
	timeval eventTime;
	eventTime.tv_sec = 0; //( time_t )( event.xkey.time / 1000 );
	eventTime.tv_usec = 0; //( suseconds_t )( ( event.xkey.time % 1000 ) * 1000 );
	
	switch( AInputEvent_getType( &event ) ){
	case AINPUT_EVENT_TYPE_KEY:{
		const int key = AKeyEvent_getKeyCode( &event );
		
		switch( AKeyEvent_getAction( &event ) ){
		case AKEY_EVENT_ACTION_DOWN:{
			if( key < 0 || key > 255 ){
				break;
			}
			if( pKeyStates[ key ] ){
				break;
			}
			pKeyStates[ key ] = true;
			
			const int button = pDevices->GetKeyboard()->IndexOfButtonWithAICode( key );
			if( button == -1 ){
				break;
			}
			
			deaiDeviceButton &ab = pDevices->GetKeyboard()->GetButtonAt( button );
			ab.SetPressed( true );
			
			AddKeyPress( pDevices->GetKeyboard()->GetIndex(), button,
				ab.GetAIChar(), ab.GetKeyCode(), eventTime );
			}break;
			
		case AKEY_EVENT_ACTION_UP:{
			if( key < 0 || key > 255 ){
				break;
			}
			if( ! pKeyStates[ key ] ){
				break;
			}
			pKeyStates[ key ] = false;
			
			const int button = pDevices->GetKeyboard()->IndexOfButtonWithAICode( key );
			if( button == -1 ){
				break;
			}
			
			deaiDeviceButton &ab = pDevices->GetKeyboard()->GetButtonAt( button );
			ab.SetPressed( false );
			
			AddKeyRelease( pDevices->GetKeyboard()->GetIndex(), button,
				ab.GetAIChar(), ab.GetKeyCode(), eventTime );
			}break;
			
		case AKEY_EVENT_ACTION_MULTIPLE:
			 // Multiple duplicate key events have occurred in a row, or a complex string is
			 // being delivered.  The repeat_count property of the key event contains the number
			 // of times the given key code should be executed.
			break;
			
		default:
			break;
		}
		}break;
		
	case AINPUT_EVENT_TYPE_MOTION:
		switch( AMotionEvent_getAction( &event ) & AMOTION_EVENT_ACTION_MASK ){
		case AMOTION_EVENT_ACTION_DOWN:{
			const int pointerId = AMotionEvent_getPointerId( &event, 0 );
			const int x = AMotionEvent_getX( &event, 0 );
			const int y = AMotionEvent_getY( &event, 0 );
			
			if( pOverlaySystem->OnTouch( pointerId, decPoint( x, y ) ) ){
				break;
			}
			
			const int modifiers = pModifiersFromEvent( event );
			const int dx = x - pLastMouseX;
			const int dy = y - pLastMouseY;
			
			pPointerMouse = pointerId;
			
			if( dx != 0 || dy != 0 ){
				AddMouseMove( pDevices->GetMouse()->GetIndex(), modifiers, dx, dy, eventTime );
			}
			
			//const int buttonstate = AMotionEvent_getButtonState( &event );
			//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
			const int button = 0;  // always simulate left button
			deaiDeviceButton &ab = pDevices->GetMouse()->GetButtonAt( button );
			ab.SetPressed( true );
			
			AddMousePress( pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime );
			
			}break;
			
		case AMOTION_EVENT_ACTION_UP:{
			const int pointerId = AMotionEvent_getPointerId( &event, 0 );
			
			pOverlaySystem->OnRelease( pointerId );
			
			if( pPointerMouse == -1 ){
				break;
			}
			
			const int modifiers = pModifiersFromEvent( event );
			const int x = AMotionEvent_getX( &event, 0 );
			const int y = AMotionEvent_getY( &event, 0 );
			const int dx = x - pLastMouseX;
			const int dy = y - pLastMouseY;
			
			if( dx != 0 || dy != 0 ){
				AddMouseMove( pDevices->GetMouse()->GetIndex(), modifiers, dx, dy, eventTime );
			}
			
			//const int buttonstate = AMotionEvent_getButtonState( &event );
			//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
			const int button = 0;  // always simulate left button
			deaiDeviceButton &ab = pDevices->GetMouse()->GetButtonAt( button );
			ab.SetPressed( false );
			
			AddMouseRelease( pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime );
			
			pPointerMouse = -1;
			
			}break;
			
		case AMOTION_EVENT_ACTION_MOVE:{ // movement while touching screen
			const int pointerCount = AMotionEvent_getPointerCount( &event );
			int i;
			
			for( i=0; i<pointerCount; i++ ){
				const int x = AMotionEvent_getX( &event, i );
				const int y = AMotionEvent_getY( &event, i );
				const int pointerId = AMotionEvent_getPointerId( &event, i );
				
				pOverlaySystem->OnMove( pointerId, decPoint( x, y ) );
				
				if( pointerId != pPointerMouse ){
					continue;
				}
				
				const int dx = x - pLastMouseX;
				const int dy = y - pLastMouseY;
				
				if( dx != 0 || dy != 0 ){
					AddMouseMove( pDevices->GetMouse()->GetIndex(),
						pModifiersFromEvent( event ), dx, dy, eventTime );
				}
			}
			
			}break;
			
		case AMOTION_EVENT_ACTION_HOVER_MOVE: // movement while not touching screen
			LogInfo( "AMOTION_EVENT_ACTION_HOVER_MOVE" );
			// these are absolute positions. need to track difference to previous values
			// most probably has to use AMotionEvent_getHistoricalX, AMotionEvent_getHistoricalY
			// and AMotionEvent_getHistorySize, whereas history exists only for MOVE not HOVER_MOVE
			//engine->state.x = AMotionEvent_getX(event, 0);
			//engine->state.y = AMotionEvent_getY(event, 0);
			break;
			
		case AMOTION_EVENT_ACTION_CANCEL:
			// gesture stopped. documentation claims this is the same as AMOTION_EVENT_ACTION_UP
			// but no action should be done like in that case. no idea what this is supposed to mean
			LogInfo( "AMOTION_EVENT_ACTION_CANCEL" );
			break;
			
		case AMOTION_EVENT_ACTION_OUTSIDE:
			// movement outside of the screen. can this be called between up and down?
			LogInfo( "AMOTION_EVENT_ACTION_OUTSIDE" );
			break;
			
		case AMOTION_EVENT_ACTION_POINTER_DOWN:{
			const int pointerIndex = ( AMotionEvent_getAction( &event ) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK )
				>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			const int pointerId = AMotionEvent_getPointerId( &event, pointerIndex );
			const int x = AMotionEvent_getX( &event, pointerIndex );
			const int y = AMotionEvent_getY( &event, pointerIndex );
			
			pOverlaySystem->OnTouch( pointerId, decPoint( x, y ) );
			
			}break;
			
		case AMOTION_EVENT_ACTION_POINTER_UP:{
			const int pointerIndex = ( AMotionEvent_getAction( &event ) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK )
				>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			const int pointerId = AMotionEvent_getPointerId( &event, pointerIndex );
			
			pOverlaySystem->OnRelease( pointerId );
			
			if( pointerId != pPointerMouse ){
				break;
			}
			
			const int modifiers = pModifiersFromEvent( event );
			const int x = AMotionEvent_getX( &event, pointerIndex );
			const int y = AMotionEvent_getY( &event, pointerIndex );
			const int dx = x - pLastMouseX;
			const int dy = y - pLastMouseY;
			
			if( dx != 0 || dy != 0 ){
				AddMouseMove( pDevices->GetMouse()->GetIndex(), modifiers, dx, dy, eventTime );
			}
			
			//const int buttonstate = AMotionEvent_getButtonState( &event );
			//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
			const int button = 0;  // always simulate left button
			deaiDeviceButton &ab = pDevices->GetMouse()->GetButtonAt( button );
			ab.SetPressed( false );
			
			AddMouseRelease( pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime );
			
			pPointerMouse = -1;
			
			}break;
			
		case AMOTION_EVENT_ACTION_SCROLL:
			// The motion event contains relative vertical and/or horizontal scroll offsets.
			// Use getAxisValue to retrieve the information from AMOTION_EVENT_AXIS_VSCROLL
			// and AMOTION_EVENT_AXIS_HSCROLL.
			// The pointer may or may not be down when this event is dispatched.
			// This action is always delivered to the winder under the pointer, which
			// may not be the window currently touched.
			LogInfoFormat( "AMOTION_EVENT_ACTION_SCROLL axisX=%f axisY=%f",
				AMotionEvent_getAxisValue( &event, AMOTION_EVENT_AXIS_HSCROLL, 0 ),
				AMotionEvent_getAxisValue( &event, AMOTION_EVENT_AXIS_VSCROLL, 0 ) );
			break;
			
		case AMOTION_EVENT_ACTION_HOVER_ENTER:
			// not touching but enternig window
			LogInfo( "AMOTION_EVENT_ACTION_HOVER_ENTER" );
			break;
			
		case AMOTION_EVENT_ACTION_HOVER_EXIT:
			// not touching but enternig window
			LogInfo( "AMOTION_EVENT_ACTION_HOVER_EXIT" );
			break;
			
		default:
			break;
		}
		break;
		
	default:
		break;
	}
}



timeval deAndroidInput::TimeValNow() const{
	timeval eventTime;
	eventTime.tv_sec = 0; //( time_t )( event.xkey.time / 1000 );
	eventTime.tv_usec = 0; //( suseconds_t )( ( event.xkey.time % 1000 ) * 1000 );
	
	return eventTime;
}

void deAndroidInput::AddKeyPress( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deAndroidInput::AddKeyRelease( int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeKeyRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetKeyCode( keyCode );
	event.SetKeyChar( keyChar );
	event.SetState( 0 );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deAndroidInput::AddMousePress( int device, int button, int state,
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

void deAndroidInput::AddMouseRelease( int device, int button, int state,
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

void deAndroidInput::AddMouseMove( int device, int state, int x, int y, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeMouseMove );
	event.SetDevice( device );
	event.SetCode( pMouseButtons );
	event.SetState( 0 ); // code of x axis. y axis has to be code + 1
	event.SetX( x );
	event.SetY( y );
	event.SetTime( eventTime );
	queue.AddEvent( event );
	
	const decPoint &res = pOSAndroid->GetDisplayCurrentResolution( 0 );
	pLastMouseX = decMath::clamp( pLastMouseX + x, 0, res.x - 1 );
	pLastMouseY = decMath::clamp( pLastMouseY + y, 0, res.y - 1 );
}

void deAndroidInput::AddAxisChanged( int device, int axis, float value, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeAxisMove );
	event.SetDevice( device );
	event.SetCode( axis );
	event.SetValue( value );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deAndroidInput::AddButtonPressed( int device, int button, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonPress );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}

void deAndroidInput::AddButtonReleased( int device, int button, const timeval &eventTime ){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType( deInputEvent::eeButtonRelease );
	event.SetDevice( device );
	event.SetCode( button );
	event.SetTime( eventTime );
	queue.AddEvent( event );
}



// Private Functions
//////////////////////

void deAndroidInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		return;
	}
	
	pLastMouseX = renderWindow->GetWidth() >> 1;
	pLastMouseY = renderWindow->GetHeight() >> 1;
}

int deAndroidInput::pModifiersFromEvent( const AInputEvent &event ) const{
	const int metastate = AMotionEvent_getMetaState( &event );
	int modifiers = deInputEvent::esmNone;
	
	if( ( metastate & AMETA_SHIFT_ON ) ==  AMETA_SHIFT_ON ){
		modifiers |= deInputEvent::esmShift;
	}
	if( ( metastate & AMETA_CTRL_ON ) ==  AMETA_CTRL_ON ){
		modifiers |= deInputEvent::esmControl;
	}
	if( ( metastate & AMETA_ALT_ON ) ==  AMETA_ALT_ON ){
		modifiers |= deInputEvent::esmAlt;
	}
	
	return modifiers;
}
