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
#include "deainpDevice.h"
#include "deainpDeviceAxis.h"
#include "deainpDeviceButton.h"
#include "deainpDeviceKeyboard.h"
#include "deainpDeviceMouse.h"
#include "deainpDeviceManager.h"
#include "overlay/deainpOverlaySystem.h"

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



#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *AndroidInpCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif

// #define DEBUG_LOG_INPUT


// Entry point
////////////////

deBaseModule *AndroidInpCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new deAndroidInput(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Class deAndroidInput
//////////////////////////

// Constructor, destructor
////////////////////////////

deAndroidInput::deAndroidInput(deLoadableModule &loadableModule) :
deBaseInputModule(loadableModule),

pOSAndroid(NULL),

pMouseButtons(0),
pIsListening(false),
pKeyStates(NULL),
pPointerMouse(-1),

pOverlaySystem(NULL),

pElapsedTime(0.0f),

pDevices(NULL){
}

deAndroidInput::~deAndroidInput(){
}



// Management
///////////////

bool deAndroidInput::Init(){
	try{
		pOSAndroid = GetGameEngine()->GetOS()->CastToOSAndroid();
		
		pFontDefault = GetGameEngine()->GetFontManager()->LoadFont(
			&GetVFS(), "/share/fonts/nimbus_sans_30.defont", "/");
		//	"/share/fonts/nimbus_sans_30_bold.defont", "/" );
		
		pKeyStates = new bool[256];
		
		pDevices = new deainpDeviceManager(*this);
		pDevices->UpdateDeviceList();
		pDevices->LogDevices();
		
		pCenterPointer();
		pIsListening = true;
		
		pOverlaySystem = new deainpOverlaySystem(*this);
		GetGameEngine()->GetGraphicSystem()->SetInputOverlayCanvas(pOverlaySystem->GetCanvas());
		
		pInputTimer.Reset();
		pElapsedTime = 0.0f;
		
	}catch(const deException &e){
		LogException(e);
		CleanUp();
		if(pDevices){
			delete pDevices;
			pDevices = NULL;
		}
		return false;
	}
	
	return true;
}

void deAndroidInput::CleanUp(){
	if(pOverlaySystem){
		GetGameEngine()->GetGraphicSystem()->SetInputOverlayCanvas(NULL);
		delete pOverlaySystem;
		pOverlaySystem = NULL;
	}
	
	if(pDevices){
		delete pDevices;
		pDevices = NULL;
	}
	
	if(pKeyStates){
		delete [] pKeyStates;
		pKeyStates = NULL;
	}
	
	pOSAndroid = NULL;
}



decPoint deAndroidInput::GetScreenSize() const{
	const deRenderWindow * const window = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(window){
		return decPoint(window->GetWidth(), window->GetHeight());
	}
	
	if(pOSAndroid){
		return pOSAndroid->GetDisplayCurrentResolution(0);
	}
	
	return decPoint(640, 1400);
}



// Devices
////////////

int deAndroidInput::GetDeviceCount(){
	return pDevices->GetCount();
}

deInputDevice::Ref deAndroidInput::GetDeviceAt(int index){
	const deInputDevice::Ref device(deInputDevice::Ref::NewWith());
	pDevices->GetAt(index)->GetInfo(device);
	return device;
}

int deAndroidInput::IndexOfDeviceWithID(const char *id){
	return pDevices->IndexOfWithID(id);
}

int deAndroidInput::IndexOfButtonWithID(int device, const char *id){
	return pDevices->GetAt(device)->IndexOfButtonWithID(id);
}

int deAndroidInput::IndexOfAxisWithID(int device, const char *id){
	return pDevices->GetAt(device)->IndexOfAxisWithID(id);
}

int deAndroidInput::IndexOfFeedbackWithID(int device, const char *id){
	return -1;
}

bool deAndroidInput::GetButtonPressed(int device, int button){
	return pDevices->GetAt(device)->GetButtonAt(button).GetPressed();
}

float deAndroidInput::GetAxisValue(int device, int axis){
	return pDevices->GetAt(device)->GetAxisAt(axis).GetValue();
}

float deAndroidInput::GetFeedbackValue(int device, int feedback){
	DETHROW(deeInvalidParam);
}

void deAndroidInput::SetFeedbackValue(int device, int feedback, float value){
	DETHROW(deeInvalidParam);
}

int deAndroidInput::ButtonMatchingKeyCode(int device, deInputEvent::eKeyCodes keyCode){
	const deainpDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	if(device != rdevice.GetIndex()){
		return -1;
	}
	
	const int count = rdevice.GetButtonCount();
	int bestPriority = 10;
	int bestButton = -1;
	int i;
	
	for(i=0; i<count; i++){
		const deainpDeviceButton &button = rdevice.GetButtonAt(i);
		
		if(button.GetKeyCode() == keyCode && button.GetMatchPriority() < bestPriority){
			bestButton = i;
			bestPriority = button.GetMatchPriority();
		}
	}
	
	return bestButton;
}

int deAndroidInput::ButtonMatchingKeyChar(int device, int character){
	const deainpDeviceKeyboard &rdevice = *pDevices->GetKeyboard();
	if(device != rdevice.GetIndex()){
		return -1;
	}
	
	return rdevice.ButtonMatchingKeyChar(character);
}



// Events
///////////

void deAndroidInput::ProcessEvents(){
	// update elapsed time. we do this independently of the game engine to ensure
	// our own timing in all situations. the elapsed time is clamped in case a
	// stuttering to avoid exploding input
	pElapsedTime = decMath::min(pInputTimer.GetElapsedTime(), 0.1f);
	
	// update content. this can produce input events for the game engine
	if(pOverlaySystem){
		pOverlaySystem->UpdateContent();
	}
}

void deAndroidInput::ClearEvents(){
	pCenterPointer();
	pInputTimer.Reset();
	pElapsedTime = 0.0f;
}

void deAndroidInput::ScreenSizeChanged(){
	if(pOverlaySystem){
		pOverlaySystem->ScreenSizeChanged();
	}
}

void deAndroidInput::EventLoop(const android_input_buffer &inputBuffer){
	uint64_t i;
	
	for(i=0; i<inputBuffer.keyEventsCount; i++){
		pProcessKeyEvent(inputBuffer.keyEvents[i]);
	}
	
	for (i=0; i<inputBuffer.motionEventsCount; ++i){
		pProcessMotionEvent(inputBuffer.motionEvents[i]);
	}
}



timeval deAndroidInput::TimeValNow() const{
	timeval eventTime;
	eventTime.tv_sec = 0; //(time_t)(event.xkey.time / 1000);
	eventTime.tv_usec = 0; //(suseconds_t)((event.xkey.time % 1000) * 1000);
	
	return eventTime;
}

void deAndroidInput::AddKeyPress(int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeKeyPress);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetKeyCode(keyCode);
	event.SetKeyChar(keyChar);
	event.SetState(0);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deAndroidInput::AddKeyRelease(int device, int button, int keyChar,
deInputEvent::eKeyCodes keyCode, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeKeyRelease);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetKeyCode(keyCode);
	event.SetKeyChar(keyChar);
	event.SetState(0);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deAndroidInput::AddMousePress(int device, int button, int state,
const timeval &eventTime){
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

void deAndroidInput::AddMouseRelease(int device, int button, int state,
const timeval &eventTime){
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

void deAndroidInput::AddMouseMove(int device, int state, const decPoint &distance,
const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	pLastMouse = (pLastMouse + distance).
		Largest(decPoint()).
		Smallest(GetScreenSize() - decPoint(1, 1));
	
	event.SetType(deInputEvent::eeMouseMove);
	event.SetDevice(device);
	event.SetCode(pMouseButtons);
	event.SetState(0); // code of x axis. y axis has to be code + 1
	
	if(GetGameEngine()->GetInputSystem()->GetCaptureInputDevices()){
		event.SetX(distance.x);
		event.SetY(distance.y);
		
	}else{
		event.SetX(pLastMouse.x);
		event.SetY(pLastMouse.y);
	}
	
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deAndroidInput::AddAxisChanged(int device, int axis, float value, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeAxisMove);
	event.SetDevice(device);
	event.SetCode(axis);
	event.SetValue(value);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deAndroidInput::AddButtonPressed(int device, int button, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeButtonPress);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void deAndroidInput::AddButtonReleased(int device, int button, const timeval &eventTime){
	deInputEventQueue &queue = GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	
	event.SetType(deInputEvent::eeButtonRelease);
	event.SetDevice(device);
	event.SetCode(button);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}



// Private Functions
//////////////////////

timeval deAndroidInput::pConvertEventTime(int64_t time) const{
	timeval eventTime{};
	eventTime.tv_sec = (time_t)(time / (int64_t)1000000000);
	eventTime.tv_usec = (suseconds_t)((time % (int64_t)1000000000) * (int64_t)1000);
	return eventTime;
}

void deAndroidInput::pProcessKeyEvent(const GameActivityKeyEvent &event){
	bool doController = false;
	
	if (doController) {
		// if(Paddleboat_processGameActivityKeyInputEvent(event, sizeof(GameActivityKeyEvent))){
		//    return;
		// }
	}
	
	const timeval eventTime = pConvertEventTime(event.eventTime);
	const int key = (int)event.keyCode;
	
	switch(event.action){
	case AKEY_EVENT_ACTION_DOWN:{
		if(key < 0 || key > 255){
			break;
		}
		if(pKeyStates[key]){
			break;
		}
		pKeyStates[key] = true;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithAICode(key);
		if(button == -1){
			break;
		}
		
		deainpDeviceButton &ab = pDevices->GetKeyboard()->GetButtonAt(button);
		ab.SetPressed(true);
		
		AddKeyPress(pDevices->GetKeyboard()->GetIndex(), button,
			ab.GetAIChar(), ab.GetKeyCode(), eventTime);
		}break;
		
	case AKEY_EVENT_ACTION_UP:{
		if(key < 0 || key > 255){
			break;
		}
		if(!pKeyStates[key]){
			break;
		}
		pKeyStates[key] = false;
		
		const int button = pDevices->GetKeyboard()->IndexOfButtonWithAICode(key);
		if(button == -1){
			break;
		}
		
		deainpDeviceButton &ab = pDevices->GetKeyboard()->GetButtonAt(button);
		ab.SetPressed(false);
		
		AddKeyRelease(pDevices->GetKeyboard()->GetIndex(), button,
			ab.GetAIChar(), ab.GetKeyCode(), eventTime);
		}break;
		
	case AKEY_EVENT_ACTION_MULTIPLE:
		break;
		
	default:
		break;
	}
}

void deAndroidInput::pProcessMotionEvent(const GameActivityMotionEvent &event){
	if(event.pointerCount == 0){
		return;
	}
	
	switch(event.source){
	case AINPUT_SOURCE_TOUCHSCREEN:
		pProcessMotionEventTouchScreen(event);
		break;
		
	default:
		break;
	}
}

void deAndroidInput::pProcessMotionEventTouchScreen(const GameActivityMotionEvent &event){
	const timeval eventTime = pConvertEventTime(event.eventTime);
	const int action = event.action;
	const int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
	
	switch(actionMasked){
	case AMOTION_EVENT_ACTION_DOWN:{
		const int pointerId = (int)event.pointers[0].id;
		const decPoint position(pPointerPosition(event.pointers[0]));
		#ifdef DEBUG_LOG_INPUT
		LogInfoFormat("DOWN: (%d,%d)", position.x, position.y);
		#endif
		if(pOverlaySystem && pOverlaySystem->OnTouch(pointerId, position)){
			break;
		}
		
		const int modifiers = pModifiersFromMetaState(event.metaState);
		const decPoint distance(position - pLastMouse);
		
		pPointerMouse = pointerId;
		
		if(distance != decPoint()){
			AddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers, distance, eventTime);
		}
		
		//const int buttonstate = AMotionEvent_getButtonState( &event );
		//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
		const int button = 0; // always simulate left button
		deainpDeviceButton &ab = pDevices->GetMouse()->GetButtonAt(button);
		ab.SetPressed(true);
		
		AddMousePress(pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime);
		}break;
		
	case AMOTION_EVENT_ACTION_UP:{
		const int pointerId = (int)event.pointers[0].id;
		if(pOverlaySystem){
			pOverlaySystem->OnRelease(pointerId);
		}
		
		if(pPointerMouse == -1){
			break;
		}
		
		const int modifiers = pModifiersFromMetaState(event.metaState);
		const decPoint position(pPointerPosition(event.pointers[0]));
		#ifdef DEBUG_LOG_INPUT
		LogInfoFormat("UP: (%d,%d)", position.x, position.y);
		#endif
		const decPoint distance(position - pLastMouse);
		
		if(distance != decPoint()){
			AddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers, distance, eventTime);
		}
		
		//const int buttonstate = AMotionEvent_getButtonState( &event );
		//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
		const int button = 0; // always simulate left button
		deainpDeviceButton &ab = pDevices->GetMouse()->GetButtonAt(button);
		ab.SetPressed(false);
		
		AddMouseRelease(pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime);
		pPointerMouse = -1;
		}break;
		
	case AMOTION_EVENT_ACTION_MOVE:{ // movement while touching screen
		int i;
		for(i=0; i<event.pointerCount; i++){
			const decPoint position(pPointerPosition(event.pointers[i]));
			const int pointerId = (int)event.pointers[i].id;
			#ifdef DEBUG_LOG_INPUT
			LogInfoFormat("MOVE[%d:%d]: (%d,%d)", i, pointerId, position.x, position.y);
			#endif
			
			if(pOverlaySystem){
				pOverlaySystem->OnMove(pointerId, position);
			}
			
			if(pointerId != pPointerMouse){
				continue;
			}
			
			const decPoint distance(position - pLastMouse);
			
			if(distance != decPoint()){
				const int modifiers = pModifiersFromMetaState(event.metaState);
				AddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers, distance, eventTime);
			}
		}
		}break;
		
	case AMOTION_EVENT_ACTION_HOVER_MOVE: // movement while not touching screen
		if(event.pointerCount > 0){
			const decPoint position(pPointerPosition(event.pointers[0]));
			#ifdef DEBUG_LOG_INPUT
			LogInfoFormat("HOVER_MOVE: (%d,%d)", position.x, position.y);
			#endif
			
			if(pOverlaySystem){
				pOverlaySystem->OnMove(0, position);
			}
			
			const decPoint distance(position - pLastMouse);
			
			if(distance != decPoint()){
				const int modifiers = pModifiersFromMetaState(event.metaState);
				AddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers, distance, eventTime);
			}
			
		}else{
			#ifdef DEBUG_LOG_INPUT
			LogInfo("HOVER_MOVE");
			#endif
		}
		break;
		
	case AMOTION_EVENT_ACTION_CANCEL:
		// gesture stopped. documentation claims this is the same as AMOTION_EVENT_ACTION_UP
		// but no action should be done like in that case. no idea what this is supposed to mean
		LogInfo("CANCEL");
		break;
		
	case AMOTION_EVENT_ACTION_OUTSIDE:
		// movement outside of the screen. can this be called between up and down?
		LogInfo("OUTSIDE");
		break;
		
	case AMOTION_EVENT_ACTION_POINTER_DOWN:{
		const int pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
			>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		if(pointerIndex < 0 || pointerIndex >= GAMEACTIVITY_MAX_NUM_POINTERS_IN_MOTION_EVENT){
			break;
		}
		
		const int pointerId = (int)event.pointers[pointerIndex].id;
		const decPoint position(pPointerPosition(event.pointers[pointerIndex]));
		#ifdef DEBUG_LOG_INPUT
		LogInfoFormat("POINTER-DOWN[%d:%d]: (%d,%d)", pointerIndex, pointerId, position.x, position.y);
		#endif
		if(pOverlaySystem){
			pOverlaySystem->OnTouch(pointerId, position);
		}
		}break;
		
	case AMOTION_EVENT_ACTION_POINTER_UP:{
		const int pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
			>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		if(pointerIndex < 0 || pointerIndex >= GAMEACTIVITY_MAX_NUM_POINTERS_IN_MOTION_EVENT){
			break;
		}
		
		const int pointerId = (int)event.pointers[pointerIndex].id;
		if(pOverlaySystem){
			pOverlaySystem->OnRelease(pointerId);
		}
		
		if(pointerId != pPointerMouse){
			break;
		}
		
		const int modifiers = pModifiersFromMetaState(event.metaState);
		const decPoint position(pPointerPosition(event.pointers[pointerIndex]));
		#ifdef DEBUG_LOG_INPUT
		LogInfoFormat("POINTER-UP[%d:%d]: (%d,%d)", pointerIndex, pointerId, position.x, position.y);
		#endif
		const decPoint distance(position - pLastMouse);
		
		if(distance != decPoint()){
			AddMouseMove(pDevices->GetMouse()->GetIndex(), modifiers, distance, eventTime);
		}
		
		//const int buttonstate = AMotionEvent_getButtonState( &event );
		//const int button = pDevices->GetMouse()->IndexOfButtonWithAICode( buttonstate );
		const int button = 0; // always simulate left button
		deainpDeviceButton &ab = pDevices->GetMouse()->GetButtonAt(button);
		ab.SetPressed(false);
		
		AddMouseRelease(pDevices->GetMouse()->GetIndex(), button, modifiers, eventTime);
		pPointerMouse = -1;
		}break;
		
	case AMOTION_EVENT_ACTION_SCROLL:
		// The motion event contains relative vertical and/or horizontal scroll offsets.
		// Use getAxisValue to retrieve the information from AMOTION_EVENT_AXIS_VSCROLL
		// and AMOTION_EVENT_AXIS_HSCROLL.
		// The pointer may or may not be down when this event is dispatched.
		// This action is always delivered to the winder under the pointer, which
		// may not be the window currently touched.
		LogInfoFormat("SCROLL: axisX=%f axisY=%f",
			event.pointers[0].axisValues[0], event.pointers[0].axisValues[1]);
		
		// note: on quest this is not called
		break;
		
	case AMOTION_EVENT_ACTION_HOVER_ENTER:
		// not touching but enternig window
		#ifdef DEBUG_LOG_INPUT
		LogInfo("HOVER_ENTER");
		#endif
		break;
		
	case AMOTION_EVENT_ACTION_HOVER_EXIT:
		// not touching but enternig window
		#ifdef DEBUG_LOG_INPUT
		LogInfo("HOVER_EXIT");
		#endif
		break;
		
	default:
		break;
	}
}

void deAndroidInput::pCenterPointer(){
	const deRenderWindow * const renderWindow = GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if(!renderWindow){
		return;
	}
	
	pLastMouse.x = renderWindow->GetWidth() >> 1;
	pLastMouse.y = renderWindow->GetHeight() >> 1;
}

decPoint deAndroidInput::pPointerPosition(const GameActivityPointerAxes &pointer) const{
	// rawX and rawY seems to miss system bar offsets. the axis values seem to include them
	//return decPoint((int)(pointer.rawX + 0.5f), (int)(pointer.rawY + 0.5f));
	return decPoint((int)(pointer.axisValues[0] + 0.5f), (int)(pointer.axisValues[1] + 0.5f));
}

int deAndroidInput::pModifiersFromMetaState(int32_t metaState) const{
	int modifiers = deInputEvent::esmNone;
	
	if((metaState & AMETA_SHIFT_ON) == AMETA_SHIFT_ON){
		modifiers |= deInputEvent::esmShift;
	}
	if((metaState & AMETA_CTRL_ON) == AMETA_CTRL_ON){
		modifiers |= deInputEvent::esmControl;
	}
	if((metaState & AMETA_ALT_ON) == AMETA_ALT_ON){
		modifiers |= deInputEvent::esmAlt;
	}
	
	return modifiers;
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deainpModuleInternal : public deInternalModule{
public:
	deainpModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("AndroidInput");
		SetDescription("Processes input of Android Operating systems.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtInput);
		SetDirectoryName("androidinput");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(AndroidInpCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deainpRegisterInternalModule(deModuleSystem *system){
	return new deainpModuleInternal(system);
}
#endif
