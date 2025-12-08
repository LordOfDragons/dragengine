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

#include <Application.h>
#include <Point.h>
#include <Rect.h>
#include <Window.h>
#include <WindowScreen.h>

#include "deBeOSInput.h"
#include "debiDevice.h"
#include "debiDeviceAxis.h"
#include "debiDeviceButton.h"
#include "debiDeviceKeyboard.h"
#include "debiDeviceMouse.h"
#include "debiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSBeOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>



#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BeOSInpCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a opengl graphic module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////
deBaseModule *BeOSInpCreateModule(deLoadableModule *loadableModule){
	deBeOSInput *module = NULL;
	
	try{
		module = new deBeOSInput(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Class deBeOSInput
//////////////////////

// Constructor, destructor
////////////////////////////

deBeOSInput::deBeOSInput(deLoadableModule &loadableModule) :
deBaseInputModule(loadableModule),

pOSBeOS(NULL),

pWindowWidth(0),
pWindowHeight(0),

pMouseButtons(0),
pLastMouseX(0),
pLastMouseY(0),

pIsListening(false),

pOldAccelNom(0),
pOldAccelDenom(0),
pOldThreshold(0),

pKeyStates(NULL),
pSystemAutoRepeatEnabled(false),
pAutoRepeatEnabled(false),

pDevices(NULL){
}

deBeOSInput::~deBeOSInput(){
}



// Management
///////////////

bool deBeOSInput::Init(){
	pOSBeOS = GetOS()->CastToOSBeOS();
	pMouseButtons = 0;
	pWindowWidth = 0;
	pWindowHeight = 0;
	
	if(pOSBeOS->GetWindow()){
		const BRect bounds(pOSBeOS->GetWindow()->Bounds());
		pWindowWidth = bounds.IntegerWidth();
		pWindowHeight = bounds.IntegerHeight();
	}
	
	pIsListening = true;
	
	// determine if keyboard auto-repeat is enabled
	pSystemAutoRepeatEnabled = true; // figure this out using X11
	pAutoRepeatEnabled = pSystemAutoRepeatEnabled;
	
	AppActivationChanged();
	
	// scan for devices
	int i;
	
	try{
		pKeyStates = new bool[255];
		for(i=0; i<255; i++){
			pKeyStates[i] = false;
		}
		
		pDevices = new debiDeviceManager(*this);
		pDevices->UpdateDeviceList();
		//pDevices->LogDevices();
		
	}catch(const deException &e){
		LogException(e);
		if(pDevices){
			delete pDevices;
			pDevices = NULL;
		}
		return false;
	}
	
	return true;
}

void deBeOSInput::CleanUp(){
	if(pDevices){
		delete pDevices;
		pDevices = NULL;
	}
	
	if(pKeyStates){
		delete [] pKeyStates;
		pKeyStates = NULL;
	}
	
	pSetAutoRepeatEnabled(pSystemAutoRepeatEnabled);
	pOSBeOS = NULL;
}



// Devices
////////////

int deBeOSInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice *deBeOSInput::GetDeviceAt(int index){
	deInputDevice::Ref device = NULL;
	
	try{
		device.TakeOver(new deInputDevice);
		pDevices->GetAt(index)->GetInfo(*device);
		
	}catch(const deException &){
		throw;
	}
	
	return device;
}

int deBeOSInput::IndexOfDeviceWithID(const char *id){
	return pDevices->IndexOfWithID(id);
}

int deBeOSInput::IndexOfButtonWithID(int device, const char *id){
	return pDevices->GetAt(device)->IndexOfButtonWithID(id);
}

int deBeOSInput::IndexOfAxisWithID(int device, const char *id){
	return pDevices->GetAt(device)->IndexOfAxisWithID(id);
}

int deBeOSInput::IndexOfFeedbackWithID(int device, const char *id){
	return -1;
}

bool deBeOSInput::GetButtonPressed(int device, int button){
	return pDevices->GetAt(device)->GetButtonAt(button)->GetPressed();
}

float deBeOSInput::GetAxisValue(int device, int axis){
	return pDevices->GetAt(device)->GetAxisAt(axis)->GetValue();
}

float deBeOSInput::GetFeedbackValue(int device, int feedback){
	DETHROW(deeInvalidParam);
}

void deBeOSInput::SetFeedbackValue(int device, int feedback, float value){
	DETHROW(deeInvalidParam);
}

int deBeOSInput::ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode){
	if(device != pDevices->GetKeyboard()->GetIndex()){
		return -1;
	}
	
	const debiDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	const int count = rdevice.GetButtonCount();
	int bestPriority = 10;
	int bestButton = -1;
	int i;
	
	for(i=0; i<count; i++){
		const debiDeviceButton &button = *rdevice.GetButtonAt(i);
		if(button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority){
			bestButton = i;
			bestPriority = button.GetMatchPriority();
		}
	}
	
	return bestButton;
}

int deBeOSInput::ButtonMatchingKeyChar(int device, int character){
	if(device != pDevices->GetKeyboard()->GetIndex()){
		return -1;
	}
	
	return pDevices->GetKeyboard()->ButtonMatchingKeyChar(character);
}



// Events
///////////

void deBeOSInput::ProcessEvents(){
	pQueryMousePosition(true);
	
	const int deviceCount = pDevices->GetCount();
	int i;
	for(i=0; i<deviceCount; i++){
		pDevices->GetAt(i)->Update();
	}
}

void deBeOSInput::ClearEvents(){
	pQueryMousePosition(false);
}

void deBeOSInput::EventLoop(const BMessage &message){
	/*const char whatStr[4] = {(char)((message.what>>24)&0xff), (char)((message.what>>16)&0xff),
		(char)((message.what>>8)&0xff), (char)(message.what&0xff)};
	printf("InputBeOS.HandleEvent: what=%.4s\n", whatStr);*/
	
	switch(message.what){
	case B_WINDOW_RESIZED:
		pWindowWidth = message.GetInt32("width", pWindowWidth);
		pWindowHeight = message.FindInt32("height", pWindowHeight);
		break;
		
	case B_WINDOW_ACTIVATED:
		pIsListening = message.GetBool("active", pIsListening);
		AppActivationChanged();
		break;
		
	case B_KEY_DOWN:{
		const int virtualKeyCode = message.GetInt32("key", 0);
		if(virtualKeyCode <= 0 || virtualKeyCode > 255){
			break;
		}
		
		if(pKeyStates[virtualKeyCode]){
			break;
		}
		pKeyStates[virtualKeyCode] = true;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithBICode(virtualKeyCode);
		if(button == -1){
			break;
		}
		
		const char * const keyString = message.GetString("bytes", ""); // UTF-8 encoded
		const int keyChar = strlen(keyString) == 1 ? keyString[0] : 0;
		const int modifiers = message.GetInt32("modifiers", 0);
		
		/*LogInfoFormat( "B_KEY_DOWN: code=%d button=%d keyCode=%d char=%d modifiers=%d",
			virtualKeyCode, button, pDevices->GetKeyboard()->GetButtonAt(button).GetKeyCode(),
			keyChar, modifiers);*/
		
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		debiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt(button);
		deviceButton.SetPressed(true);
		
		pAddKeyPress(pDevices->GetKeyboard()->GetIndex(), button, keyChar,
			deviceButton.GetKeyCode(), modifiers, eventTime);
		} break;
		
	case B_KEY_UP:{
		const int virtualKeyCode = message.GetInt32("key", 0);
		if(virtualKeyCode <= 0 || virtualKeyCode > 255){
			break;
		}
		
		if(!pKeyStates[virtualKeyCode]){
			break;
		}
		pKeyStates[virtualKeyCode] = false;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithBICode(virtualKeyCode);
		if(button == -1){
			break;
		}
		
		const char * const keyString = message.GetString("bytes", "");
		const int keyChar = strlen(keyString) == 1 ? keyString[0] : 0;
		const int modifiers = message.GetInt32("modifiers", 0);
		
		//LogInfoFormat( "B_KEY_UP: code=%d char=%d modifiers=%d", virtualKeyCode, keyChar, modifiers );
		
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		debiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt(button);
		deviceButton.SetPressed(false);
		
		pAddKeyRelease(pDevices->GetKeyboard()->GetIndex(), button, keyChar,
			deviceButton.GetKeyCode(), modifiers, eventTime);
		} break;
		
	case B_UNMAPPED_KEY_DOWN:{
		const int virtualKeyCode = message.GetInt32("key", 0);
		if(virtualKeyCode <= 0 || virtualKeyCode > 255){
			break;
		}
		
		if(pKeyStates[virtualKeyCode]){
			break;
		}
		pKeyStates[virtualKeyCode] = true;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithBICode(virtualKeyCode);
		if(button == -1){
			break;
		}
		
		const int modifiers = message.GetInt32("modifiers", 0);
		
		//LogInfoFormat( "B_UNMAPPED_KEY_DOWN: code=%d modifiers=%d", virtualKeyCode, modifiers );
		
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		debiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt(button);
		deviceButton.SetPressed(true);
		
		pAddKeyPress(pDevices->GetKeyboard()->GetIndex(), button, 0,
			deviceButton.GetKeyCode(), modifiers, eventTime);
		} break;
		
	case B_UNMAPPED_KEY_UP:{
		const int virtualKeyCode = message.GetInt32("key", 0);
		if(virtualKeyCode <= 0 || virtualKeyCode > 255){
			break;
		}
		
		if(!pKeyStates[virtualKeyCode]){
			break;
		}
		pKeyStates[virtualKeyCode] = false;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithBICode(virtualKeyCode);
		if(button == -1){
			break;
		}
		
		const int modifiers = message.GetInt32("modifiers", 0);
		
		//LogInfoFormat( "B_UNMAPPED_KEY_UP: code=%d modifiers=%d", virtualKeyCode, modifiers );
		
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		debiDeviceButton &deviceButton = *pDevices->GetKeyboard()->GetButtonAt(button);
		deviceButton.SetPressed(false);
		
		pAddKeyRelease(pDevices->GetKeyboard()->GetIndex(), button, 0,
			deviceButton.GetKeyCode(), modifiers, eventTime);
		} break;
		
	case B_MODIFIERS_CHANGED:{
		/* can be ignored. sends also B_UNMAPPED* events
		//const int64 eventTime = message.GetInt64( "when", 0 );
		const int modifiers = message.GetInt32("modifiers", 0);
		LogInfoFormat("B_MODIFIERS_CHANGED: modifiers=%d", modifiers);
		*/
		} break;
		
	case B_MOUSE_DOWN:{
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		const BPoint devicePosition(message.GetPoint("where", BPoint()));
		const decPoint position((int)(devicePosition.x + 0.5f), (int)(devicePosition.y + 0.5f));
		const int buttons = message.GetInt32("buttons", B_PRIMARY_MOUSE_BUTTON);
		const int modifiers = pModifiersFromKeyState();
		
		//LogInfoFormat( "B_MOUSE_DOWN: button=%x modifiers=%d pos=(%d,%d)",
		//	buttons, modifiers, position.x, position.y );
		
		const uint32 masks[3] = {B_PRIMARY_MOUSE_BUTTON, B_SECONDARY_MOUSE_BUTTON, B_TERTIARY_MOUSE_BUTTON};
		int i;
		for(i=0; i<3; i++){
			if(!(buttons & masks[i])){
				continue;
			}
			
			const int buttonIndex = pDevices->GetMouse()->IndexOfButtonWithBICode(masks[i]);
			if(buttonIndex == -1){
				continue;
			}
			
			pDevices->GetMouse()->GetButtonAt(buttonIndex)->SetPressed(true);
			pAddMousePress(pDevices->GetMouse()->GetIndex(), buttonIndex, modifiers, eventTime);
		}
		} break;
		
	case B_MOUSE_UP:{
		// BeOS is missing buttons information in B_MOUSE_UP. we have to hack around this.
		// we can use BView::GetMouse but we have no idea how the graphic module creates
		// the render windows. fortunately it is not important which BView we are using
		// as long as it is connected to the app_server. for this reason we take the first
		// BView inside the current application window if present
		if(!pOSBeOS->GetWindow() || pOSBeOS->GetWindow()->CountChildren() == 0){
			break;
		}
		
		BPoint ignoreLocation;
		uint32 buttons = 0;
		BView &view = *pOSBeOS->GetWindow()->ChildAt(0);
		view.LockLooper();
		view.GetMouse(&ignoreLocation, &buttons, false);
		view.UnlockLooper();
		
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		const BPoint devicePosition(message.GetPoint("where", BPoint()));
		const decPoint position((int)(devicePosition.x + 0.5f), (int)(devicePosition.y + 0.5f));
		const int modifiers = pModifiersFromKeyState();
		
		//LogInfoFormat( "B_MOUSE_UP: buttons=%x pos=(%d,%d)", buttons, position.x, position.y );
		
		const uint32 masks[3] = {B_PRIMARY_MOUSE_BUTTON, B_SECONDARY_MOUSE_BUTTON, B_TERTIARY_MOUSE_BUTTON};
		int i;
		for(i=0; i<3; i++){
			if(buttons & masks[i]){
				continue;
			}
			
			const int buttonIndex = pDevices->GetMouse()->IndexOfButtonWithBICode(masks[i]);
			if(buttonIndex == -1){
				continue;
			}
			
			pDevices->GetMouse()->GetButtonAt(buttonIndex)->SetPressed(false);
			pAddMouseRelease(pDevices->GetMouse()->GetIndex(), buttonIndex, modifiers, eventTime);
		}
		} break;
		
	case B_MOUSE_WHEEL_CHANGED:{
		const int x = (int)message.GetFloat("be:wheel_delta_x", 0.0f);
		const int y = (int)message.GetFloat("be:wheel_delta_y", 0.0f);
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		
		//LogInfoFormat( "B_MOUSE_WHEEL_CHANGED: x=%f y=%f", x, y );
		if(y != 0){
			pAddMouseWheel(pDevices->GetMouse()->GetIndex(), 2,
				pModifiersFromKeyState(), 0, -y, eventTime);
		}
		
		if(x != 0){
			pAddMouseWheel(pDevices->GetMouse()->GetIndex(), 3,
				pModifiersFromKeyState(), x, 0, eventTime);
		}
		} break;
		
	case B_MOUSE_MOVED: /*{
		const timeval eventTime = pEventTimeFromBeTime(message.GetInt64("when", 0));
		const BPoint devicePosition(message.GetPoint("where", BPoint()));
		const decPoint position((int)(devicePosition.x + 0.5f), (int)(devicePosition.y + 0.5f));
		const int modifiers = message.GetInt32("modifiers", 0);
		
		//LogInfoFormat( "B_MOUSE_MOVED: modifiers=%d pos=(%d,%d)", modifiers, position.x, position.y );
		
		pAddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers,
			position.x - pCurMousePosition.x, position.y - pCurMousePosition.y, eventTime);
		pCurMousePosition = position;
		}*/ break;
		
	default:
		break;
	}
}

void deBeOSInput::CaptureInputDevicesChanged(){
	if(!pOSBeOS){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition(false);
}

void deBeOSInput::AppActivationChanged(){
	if(!pOSBeOS){
		return; // not inited yet
	}
	
	pUpdateAutoRepeat();
	pQueryMousePosition(false);
}



void deBeOSInput::AddAxisChanged(int device, int axis, float value, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeAxisMove);
	event.SetDevice(device);
	event.SetCode(axis);
	event.SetValue(value);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deBeOSInput::AddButtonPressed(int device, int button, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeButtonPress);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deBeOSInput::AddButtonReleased(int device, int button, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeButtonRelease);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deBeOSInput::AddMouseWheelChanged(int device, int axis, int x, int y, int state,
const timeval& eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeMouseWheel);
	event.SetDevice(device);
	event.SetCode(axis);
	event.SetState(state);
	event.SetX(x);
	event.SetY(y);
	event.SetValue((float)(x + y));
	event.SetTime(eventTime);
	queue.AddEvent(event);
}



// Private functions
//////////////////////

void deBeOSInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(!renderWindow){
		return;
	}
	
	BPoint position;
	position.x = (float)(renderWindow->GetWidth() / 2);
	position.y = (float)(renderWindow->GetHeight() / 2);
	renderWindow->GetWindow()->ConvertToScreen(&position);
	
	set_mouse_position((int)(position.x + 0.5f), (int)(position.y + 0.5f));
}

void deBeOSInput::pQueryMousePosition(bool sendEvents){
	const bool capture = GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	if(capture && !pIsListening){
		return;
	}
	
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(!renderWindow){
		return;
	}
	
	BWindow * const window = renderWindow->GetWindow();
	BView * const view = window->CurrentFocus();
	if(!view){
		return;
	}
	
	BPoint devicePosition;
	uint32 buttons;
	
	window->Lock(); // BLooper needs to be locked for calls to GetMouse to be valid
	view->GetMouse(&devicePosition, &buttons, true);
	window->Unlock();
	
	const int positionX = (int)(devicePosition.x + 0.5f);
	const int positionY = (int)(devicePosition.y + 0.5f);
	
	if(capture){
		if(!sendEvents){
			return;
		}
		
		const int midX = renderWindow->GetWidth() / 2;
		const int midY = renderWindow->GetHeight() / 2;
		if(positionX == midX && positionY == midY){
			return;
		}
		
		timeval eventTime;
		gettimeofday(&eventTime, NULL);
		pAddMouseMove(pDevices->GetMouse()->GetIndex(), pModifiersFromKeyState(),
			positionX - midX, positionY - midY, eventTime);
		pCenterPointer();
		
	}else{
		/*
		if(positionX < 0 || positionX >= renderWindow->GetWidth()){
			return;
		}
		if(positionY < 0 || positionY >= renderWindow->GetHeight()){
			return;
		}
		*/
		
		const int diffX = positionX - pLastMouseX;
		const int diffY = positionY - pLastMouseY;
		
		pLastMouseX = positionX;
		pLastMouseY = positionY;
		
		if(!sendEvents){
			return;
		}
		if(diffX == 0 && diffY == 0){
			return;
		}
		
		timeval eventTime;
		gettimeofday(&eventTime, NULL);
		pAddMouseMove(pDevices->GetMouse()->GetIndex(), pModifiersFromKeyState(),
			pLastMouseX, pLastMouseY, eventTime);
	}
}

timeval deBeOSInput::pEventTimeFromBeTime(int64 betime) const{
	// i have no idea how to convert this time code. it is micro-seconds since 1/1/70
	// but beos seems to be lacking any kind of method to convert a beos time into
	// structure exposing hour/minute/second like data which could in turn be used
	// with posix calls to convert into a proper timeval. so for the time being we
	// just pull the current date and use this one. not correct but also not totally off.
	timeval eventTime;
	//eventTime.tv_sec = ( time_t )( betime / 1000000 );
	//eventTime.tv_usec = ( suseconds_t )( betime % 1000000 );
	(void)betime;
	gettimeofday(&eventTime, NULL);
	return eventTime;
}

void deBeOSInput::pAddKeyPress(int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeKeyPress);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetKeyCode(keyCode);
	event.SetKeyChar(keyChar);
	event.SetState(state);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deBeOSInput::pAddKeyRelease(int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, int state, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeKeyRelease);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetKeyCode(keyCode);
	event.SetKeyChar(keyChar);
	event.SetState(state);
	event.SetTime(eventTime);
	
	queue.AddEvent(event);
}

void deBeOSInput::pAddMousePress(int device, int button, int state, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeMousePress);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetState(state);
	event.SetX(0);
	event.SetY(0);
	event.SetTime(eventTime);
	
	queue.AddEvent(event);
	
	pMouseButtons |= (1 << button);
}

void deBeOSInput::pAddMouseRelease(int device, int button, int state, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeMouseRelease);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetState(state);
	event.SetX(0);
	event.SetY(0);
	event.SetTime(eventTime);
	
	queue.AddEvent(event);
	
	pMouseButtons &= ~(1 << button);
}

void deBeOSInput::pAddMouseMove(int device, int state, int x, int y, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeMouseMove);
	event.SetDevice(device);
	event.SetCode(0); // code of x axis. y axis has to be code + 1
	event.SetState(state);
	event.SetX(x);
	event.SetY(y);
	event.SetTime(eventTime);
	
	queue.AddEvent(event);
}

void deBeOSInput::pAddMouseWheel(int device, int axis, int state, int x, int y,
const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeMouseWheel);
	event.SetDevice(device);
	event.SetCode(axis);
	event.SetState(state);
	event.SetX(x);
	event.SetY(y);
	event.SetTime(eventTime);
	
	queue.AddEvent(event);
}

/*
int deBeOSInput::pModifiersFromEvent(int modifiers) const {
	int modifiers = deInputEvent::esmNone;
	// 257 shift
	// 4100 control
	// 1026 alt
	return modifiers;
}
*/

int deBeOSInput::pModifiersFromKeyState() const{
	int modifiers = deInputEvent::esmNone;
	
	if(pKeyStates[75] || pKeyStates[86]){
		modifiers |= deInputEvent::esmShift;
	}
	if(pKeyStates[92] || pKeyStates[95]){
		modifiers |= deInputEvent::esmControl;
	}
	if(pKeyStates[93] || pKeyStates[96]){
		modifiers |= deInputEvent::esmAlt;
	}
	if(pKeyStates[102] || pKeyStates[103]){
		modifiers |= deInputEvent::esmMeta;
	}
	
	return modifiers;
}

void deBeOSInput::pUpdateAutoRepeat(){
	if(GetGameEngine()->GetInputSystem()->GetCaptureInputDevices()){
		if(pOSBeOS->GetAppActive()){
			if(pIsListening){
				pSetAutoRepeatEnabled(false);
				return;
			}
		}
	}
	
	pSetAutoRepeatEnabled(pSystemAutoRepeatEnabled);
}

void deBeOSInput::pSetAutoRepeatEnabled(bool enabled){
	if(enabled == pAutoRepeatEnabled){
		return;
	}
	
	pAutoRepeatEnabled = enabled;
	
	if(enabled){
		//XAutoRepeatOn( display );
		
	}else{
		//XAutoRepeatOff( display );
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class debiModuleInternal : public deInternalModule{
public:
	debiModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("BeOSInput");
		SetDescription("Processes input on BeOS type operating systems.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtInput);
		SetDirectoryName("beos");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(BeOSInpCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *debiRegisterInternalModule(deModuleSystem *system){
	return new debiModuleInternal(system);
}
#endif
