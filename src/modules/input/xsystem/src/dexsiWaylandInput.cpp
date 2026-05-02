/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <math.h>
#include <sys/mman.h>
#include <linux/input-event-codes.h>

#include "dexsiWaylandInput.h"
#include "dexsiDeviceButton.h"
#include "deXSystemInput.h"

#include <dragengine/app/deOSUnix.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/resources/rendering/deRenderWindow.h>


// Wayland listener structs
/////////////////////////////

static const wl_registry_listener vRegistryListener = {
	dexsiWaylandInput::OnRegistryGlobal,
	dexsiWaylandInput::OnRegistryGlobalRemove
};

static const wl_seat_listener vSeatListener = {
	dexsiWaylandInput::OnSeatCapabilities,
	dexsiWaylandInput::OnSeatName
};

static const wl_pointer_listener vPointerListener = {
	dexsiWaylandInput::OnPointerEnter,
	dexsiWaylandInput::OnPointerLeave,
	dexsiWaylandInput::OnPointerMotion,
	dexsiWaylandInput::OnPointerButton,
	dexsiWaylandInput::OnPointerAxis,
	dexsiWaylandInput::OnPointerFrame,
	dexsiWaylandInput::OnPointerAxisSource,
	dexsiWaylandInput::OnPointerAxisStop,
	dexsiWaylandInput::OnPointerAxisDiscrete
};

static const wl_keyboard_listener vKeyboardListener = {
	dexsiWaylandInput::OnKeyboardKeymap,
	dexsiWaylandInput::OnKeyboardEnter,
	dexsiWaylandInput::OnKeyboardLeave,
	dexsiWaylandInput::OnKeyboardKey,
	dexsiWaylandInput::OnKeyboardModifiers,
	dexsiWaylandInput::OnKeyboardRepeatInfo
};

static const zwp_locked_pointer_v1_listener vLockedPointerListener = {
	dexsiWaylandInput::OnLockedPointerLocked,
	dexsiWaylandInput::OnLockedPointerUnlocked
};

static const zwp_relative_pointer_v1_listener vRelativePointerListener = {
	dexsiWaylandInput::OnRelativeMotion
};


// Class dexsiWaylandInput
////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiWaylandInput::dexsiWaylandInput(deXSystemInput &module) :
pModule(module),
pWlDisplay(nullptr),
pWlRegistry(nullptr),
pInputQueue(nullptr),
pWlSeat(nullptr),
pWlSeatId(0),
pWlSeatVersion(0),
pWlPointer(nullptr),
pWlKeyboard(nullptr),
pXkbContext(nullptr),
pXkbKeymap(nullptr),
pXkbState(nullptr),
pPointerConstraints(nullptr),
pPointerConstraintsId(0),
pRelPtrManager(nullptr),
pRelPtrManagerId(0),
pLockedPointer(nullptr),
pRelativePointer(nullptr),
pPointerLocked(false),
pPointerX(0.0),
pPointerY(0.0),
pPointerDeltaX(0.0),
pPointerDeltaY(0.0),
pPointerInWindow(false),
pPointerSurface(nullptr),
pPointerEnterSerial(0),
pPointerHasMoved(false),
pAccumScrollX(0.0),
pAccumScrollY(0.0),
pPointerHasScroll(false),
pKeyboardHasFocus(false),
pReady(false)
{
	// create device objects first
	pWaylandMouse = dexsiDeviceWaylandMouse::Ref::New(module);
	pWaylandKeyboard = dexsiDeviceWaylandKeyboard::Ref::New(module);
	
	// get Wayland display from OS
	if(!module.GetOSUnix()->GetWaylandDisplay()){
		return;
	}
	
	pWlDisplay = module.GetOSUnix()->GetWaylandDisplay();
	if(!pWlDisplay){
		return;
	}
	
	// create XKB context
	pXkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	if(!pXkbContext){
		module.LogError("dexsiWaylandInput: Failed creating XKB context");
		return;
	}
	
	// create a private event queue so our input events do not interfere with other module queues
	pInputQueue = wl_display_create_queue(pWlDisplay);
	if(!pInputQueue){
		module.LogError("dexsiWaylandInput: Failed creating Wayland event queue");
		return;
	}
	
	// get registry and route it to our input queue
	pWlRegistry = wl_display_get_registry(pWlDisplay);
	if(!pWlRegistry){
		module.LogError("dexsiWaylandInput: Failed getting Wayland registry");
		return;
	}
	
	wl_proxy_set_queue((wl_proxy*)pWlRegistry, pInputQueue);
	wl_proxy_add_listener((wl_proxy*)pWlRegistry, (void(**)(void))&vRegistryListener, this);
	
	// receive all globals and bind seat
	if(wl_display_roundtrip_queue(pWlDisplay, pInputQueue) < 0){
		module.LogError("dexsiWaylandInput: Roundtrip 1 failed");
		return;
	}
	
	if(!pWlSeat){
		module.LogInfo("dexsiWaylandInput: No seat found, Wayland input not available");
		return;
	}
	
	// receive seat capabilities and create pointer/keyboard
	if(wl_display_roundtrip_queue(pWlDisplay, pInputQueue) < 0){
		module.LogError("dexsiWaylandInput: Roundtrip 2 failed");
		return;
	}
	
	// receive initial keyboard keymap and pointer state
	if(wl_display_roundtrip_queue(pWlDisplay, pInputQueue) < 0){
		module.LogError("dexsiWaylandInput: Roundtrip 3 failed");
		return;
	}
	
	if(!pWlPointer && !pWlKeyboard){
		module.LogInfo("dexsiWaylandInput: Seat has neither pointer nor keyboard capability");
		return;
	}
	
	pReady = true;
	module.LogInfo("dexsiWaylandInput: Wayland input initialized");
}

dexsiWaylandInput::~dexsiWaylandInput(){
	pUnlockPointer();
	pDestroyPointer();
	pDestroyKeyboard();
	
	if(pRelPtrManager){
		zwp_relative_pointer_manager_v1_destroy(pRelPtrManager);
	}
	if(pPointerConstraints){
		zwp_pointer_constraints_v1_destroy(pPointerConstraints);
	}
	
	if(pXkbState){
		xkb_state_unref(pXkbState);
	}
	if(pXkbKeymap){
		xkb_keymap_unref(pXkbKeymap);
	}
	if(pXkbContext){
		xkb_context_unref(pXkbContext);
	}
	
	if(pWlSeat){
		wl_proxy_destroy((wl_proxy*)pWlSeat);
	}
	if(pWlRegistry){
		wl_proxy_destroy((wl_proxy*)pWlRegistry);
	}
	if(pInputQueue){
		wl_event_queue_destroy(pInputQueue);
	}
}


// Management
///////////////

void dexsiWaylandInput::DispatchEvents(){
	if(!pReady || !pWlDisplay || !pInputQueue){
		return;
	}
	
	wl_display_flush(pWlDisplay);
	
	// try to get read access, poll for data, then read or cancel
	if(wl_display_prepare_read_queue(pWlDisplay, pInputQueue) == 0){
		pollfd pfd;
		pfd.fd = wl_display_get_fd(pWlDisplay);
		pfd.events = POLLIN;
		pfd.revents = 0;
		
		if(::poll(&pfd, 1, 0) > 0){
			wl_display_read_events(pWlDisplay);
			
		}else{
			wl_display_cancel_read(pWlDisplay);
		}
	}
	
	wl_display_dispatch_queue_pending(pWlDisplay, pInputQueue);
}

void dexsiWaylandInput::UpdateCapture(){
	if(!pReady){
		return;
	}
	
	const bool capture = pModule.GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	
	if(capture && !pLockedPointer){
		pLockPointer();
		
	}else if(!capture && pLockedPointer){
		pUnlockPointer();
	}
}


// Private functions
//////////////////////

void dexsiWaylandInput::pLockPointer(){
	if(pLockedPointer || !pPointerConstraints || !pWlPointer || !pPointerSurface){
		return;
	}
	
	// create locked pointer object with persistent lifetime so the compositor can
	// re-activate it if the pointer temporarily leaves and re-enters the window
	pLockedPointer = zwp_pointer_constraints_v1_lock_pointer(pPointerConstraints,
		pPointerSurface, pWlPointer, nullptr, ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT);
	if(!pLockedPointer){
		pModule.LogWarn("dexsiWaylandInput: Failed to lock pointer");
		return;
	}
	
	wl_proxy_set_queue((wl_proxy*)pLockedPointer, pInputQueue);
	zwp_locked_pointer_v1_add_listener(pLockedPointer, &vLockedPointerListener, this);
	
	// create relative pointer for motion events while locked
	if(pRelPtrManager && !pRelativePointer){
		pRelativePointer = zwp_relative_pointer_manager_v1_get_relative_pointer(pRelPtrManager, pWlPointer);
		if(pRelativePointer){
			wl_proxy_set_queue((wl_proxy*)pRelativePointer, pInputQueue);
			zwp_relative_pointer_v1_add_listener(pRelativePointer, &vRelativePointerListener, this);
		}
	}
	
	wl_surface_commit(pPointerSurface);
	if(pWlDisplay){
		wl_display_flush(pWlDisplay);
		wl_display_roundtrip_queue(pWlDisplay, pInputQueue);
	}
	
	pModule.LogInfo("dexsiWaylandInput: Pointer locked");
}

void dexsiWaylandInput::pUnlockPointer(){
	if(pRelativePointer){
		zwp_relative_pointer_v1_destroy(pRelativePointer);
		pRelativePointer = nullptr;
	}
	
	if(pLockedPointer){
		zwp_locked_pointer_v1_destroy(pLockedPointer);
		pLockedPointer = nullptr;
		pPointerLocked = false;
		
		wl_surface_commit(pPointerSurface);
		if(pWlDisplay){
			wl_display_flush(pWlDisplay);
			wl_display_roundtrip_queue(pWlDisplay, pInputQueue);
		}
		
		pModule.LogInfo("dexsiWaylandInput: Pointer unlocked");
	}
}

void dexsiWaylandInput::pCreatePointer(){
	if(pWlPointer || !pWlSeat){
		return;
	}
	
	pWlPointer = wl_seat_get_pointer(pWlSeat);
	if(!pWlPointer){
		return;
	}
	
	wl_proxy_set_queue((wl_proxy*)pWlPointer, pInputQueue);
	wl_proxy_add_listener((wl_proxy*)pWlPointer, (void(**)(void))&vPointerListener, this);
	
	pModule.LogInfo("dexsiWaylandInput: Wayland pointer created");
}

void dexsiWaylandInput::pDestroyPointer(){
	if(!pWlPointer){
		return;
	}
	wl_proxy_destroy((wl_proxy*)pWlPointer);
	pWlPointer = nullptr;
}

void dexsiWaylandInput::pCreateKeyboard(){
	if(pWlKeyboard || !pWlSeat){
		return;
	}
	
	pWlKeyboard = (wl_keyboard*)wl_seat_get_keyboard(pWlSeat);
	if(!pWlKeyboard){
		return;
	}
	
	wl_proxy_set_queue((wl_proxy*)pWlKeyboard, pInputQueue);
	wl_proxy_add_listener((wl_proxy*)pWlKeyboard, (void(**)(void))&vKeyboardListener, this);
	
	pModule.LogInfo("dexsiWaylandInput: Wayland keyboard created");
}

void dexsiWaylandInput::pDestroyKeyboard(){
	if(!pWlKeyboard){
		return;
	}
	wl_proxy_destroy((wl_proxy*)pWlKeyboard);
	pWlKeyboard = nullptr;
}

void dexsiWaylandInput::pFlushPointerEvents(const timeval &eventTime){
	if(!pWaylandMouse){
		return;
	}
	
	const int mouseDevice = pWaylandMouse->GetIndex();
	const int mods = pGetModifiers();
	
	const bool captureRequested = pModule.GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	if(pPointerHasMoved && (pPointerInWindow || captureRequested)){
		int x, y;
		if(pModule.GetGameEngine()->GetInputSystem()->GetCaptureInputDevices()){
			x = (int)round(pPointerDeltaX);
			y = (int)round(pPointerDeltaY);
			
		}else{
			x = (int)round(pPointerX);
			y = (int)round(pPointerY);
		}
		
		pEnqueueMouseMove(x, y, mods, eventTime);
		pPointerHasMoved = false;
		pPointerDeltaX = 0.0;
		pPointerDeltaY = 0.0;
	}
	
	if(pPointerHasScroll){
		deInputEventQueue &queue = pModule.GetGameEngine()->GetInputSystem()->GetEventQueue();
		
		if(pAccumScrollY != 0.0){
			// axis index 2 = wheelY (vertical scroll)
			deInputEvent event;
			event.SetType(deInputEvent::eeMouseWheel);
			event.SetDevice(mouseDevice);
			event.SetCode(2); // axis 2 = wheelY
			event.SetState(mods);
			event.SetY((int)round(-pAccumScrollY)); // negative: scroll up = positive delta
			event.SetValue((float)(-pAccumScrollY));
			event.SetTime(eventTime);
			queue.AddEvent(event);
			pAccumScrollY = 0.0;
		}
		
		if(pAccumScrollX != 0.0){
			// axis index 3 = wheelX (horizontal scroll)
			deInputEvent event;
			event.SetType(deInputEvent::eeMouseWheel);
			event.SetDevice(mouseDevice);
			event.SetCode(3); // axis 3 = wheelX
			event.SetState(mods);
			event.SetX((int)round(pAccumScrollX));
			event.SetValue((float)(pAccumScrollX));
			event.SetTime(eventTime);
			queue.AddEvent(event);
			pAccumScrollX = 0.0;
		}
		
		pPointerHasScroll = false;
	}
}

int dexsiWaylandInput::pGetModifiers() const{
	if(!pXkbState){
		return deInputEvent::esmNone;
	}
	
	int mods = deInputEvent::esmNone;
	
	if(pXkbKeymap){
		const xkb_mod_index_t shiftMod = xkb_keymap_mod_get_index(pXkbKeymap, XKB_MOD_NAME_SHIFT);
		if(shiftMod != XKB_MOD_INVALID
		&& xkb_state_mod_index_is_active(pXkbState, shiftMod, XKB_STATE_MODS_EFFECTIVE) > 0){
			mods |= deInputEvent::esmShift;
		}
		
		const xkb_mod_index_t ctrlMod = xkb_keymap_mod_get_index(pXkbKeymap, XKB_MOD_NAME_CTRL);
		if(ctrlMod != XKB_MOD_INVALID
		&& xkb_state_mod_index_is_active(pXkbState, ctrlMod, XKB_STATE_MODS_EFFECTIVE) > 0){
			mods |= deInputEvent::esmControl;
		}
		
		const xkb_mod_index_t altMod = xkb_keymap_mod_get_index(pXkbKeymap, XKB_MOD_NAME_ALT);
		if(altMod != XKB_MOD_INVALID
		&& xkb_state_mod_index_is_active(pXkbState, altMod, XKB_STATE_MODS_EFFECTIVE) > 0){
			mods |= deInputEvent::esmAlt;
		}
	}
	
	return mods;
}

timeval dexsiWaylandInput::pMsToTimeval(uint32_t ms) const{
	timeval tv;
	tv.tv_sec = (time_t)(ms / 1000);
	tv.tv_usec = (suseconds_t)((ms % 1000) * 1000);
	return tv;
}

void dexsiWaylandInput::pEnqueueMouseMove(int x, int y, int state, const timeval &time){
	if(!pWaylandMouse){
		return;
	}
	
	deInputEventQueue &queue = pModule.GetGameEngine()->GetInputSystem()->GetEventQueue();
	const int device = pWaylandMouse->GetIndex();
	const bool capture = pModule.GetGameEngine()->GetInputSystem()->GetCaptureInputDevices();
	
	// collapse multiple mouse move events per frame into one
	const int count = queue.GetEventCount();
	for(int i=0; i<count; i++){
		deInputEvent &ev = queue.GetEventAt(i);
		if(ev.GetType() == deInputEvent::eeMouseMove && ev.GetDevice() == device){
			if(capture){
				ev.SetX(ev.GetX() + x);
				ev.SetY(ev.GetY() + y);
				
			}else{
				ev.SetX(x);
				ev.SetY(y);
			}
			ev.SetState(state);
			return;
		}
	}
	
	deInputEvent event;
	event.SetType(deInputEvent::eeMouseMove);
	event.SetDevice(device);
	event.SetCode(0); // x axis
	event.SetState(state);
	event.SetX(x);
	event.SetY(y);
	event.SetTime(time);
	queue.AddEvent(event);
}


// Registry callbacks
///////////////////////

void dexsiWaylandInput::OnRegistryGlobal(void *data, wl_registry *registry, uint32_t name,
const char *interface, uint32_t version){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(strcmp(interface, wl_seat_interface.name) == 0 && !self.pWlSeat){
		// bind seat at version 2+ to get seat name, cap at 5 for safety
		const uint32_t bindVersion = version < 2u ? 1u : (version < 5u ? version : 5u);
		self.pWlSeat = (wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, bindVersion);
		self.pWlSeatId = name;
		self.pWlSeatVersion = bindVersion;
		
		if(self.pWlSeat){
			wl_proxy_set_queue((wl_proxy*)self.pWlSeat, self.pInputQueue);
			wl_proxy_add_listener((wl_proxy*)self.pWlSeat, (void(**)(void))&vSeatListener, data);
			self.pModule.LogInfo("dexsiWaylandInput: Found seat interface");
		}
		
	}else if(strcmp(interface, zwp_pointer_constraints_v1_interface.name) == 0
	&& !self.pPointerConstraints){
		self.pPointerConstraints = (zwp_pointer_constraints_v1*)wl_registry_bind(
			registry, name, &zwp_pointer_constraints_v1_interface, 1);
		self.pPointerConstraintsId = name;
		if(self.pPointerConstraints){
			wl_proxy_set_queue((wl_proxy*)self.pPointerConstraints, self.pInputQueue);
			self.pModule.LogInfo("dexsiWaylandInput: Found interface zwp_pointer_constraints_v1");
		}
		
	}else if(strcmp(interface, zwp_relative_pointer_manager_v1_interface.name) == 0
	&& !self.pRelPtrManager){
		self.pRelPtrManager = (zwp_relative_pointer_manager_v1*)wl_registry_bind(
			registry, name, &zwp_relative_pointer_manager_v1_interface, 1);
		self.pRelPtrManagerId = name;
		if(self.pRelPtrManager){
			wl_proxy_set_queue((wl_proxy*)self.pRelPtrManager, self.pInputQueue);
			self.pModule.LogInfo("dexsiWaylandInput: Found interface zwp_relative_pointer_manager_v1");
		}
	}
}

void dexsiWaylandInput::OnRegistryGlobalRemove(void *data, wl_registry *registry, uint32_t name){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(name == self.pWlSeatId && self.pWlSeat){
		// seat gone. destroy pointer and keyboard
		self.pUnlockPointer();
		self.pDestroyPointer();
		self.pDestroyKeyboard();
		wl_proxy_destroy((wl_proxy*)self.pWlSeat);
		self.pWlSeat = nullptr;
		self.pWlSeatId = 0;
		self.pReady = false;
		
	}else if(name == self.pPointerConstraintsId && self.pPointerConstraints){
		zwp_pointer_constraints_v1_destroy(self.pPointerConstraints);
		self.pPointerConstraints = nullptr;
		self.pPointerConstraintsId = 0;
		
	}else if(name == self.pRelPtrManagerId && self.pRelPtrManager){
		zwp_relative_pointer_manager_v1_destroy(self.pRelPtrManager);
		self.pRelPtrManager = nullptr;
		self.pRelPtrManagerId = 0;
	}
}


// Seat callbacks
///////////////////

void dexsiWaylandInput::OnSeatCapabilities(void *data, wl_seat *seat, uint32_t capabilities){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	const bool hasPointer = (capabilities & WL_SEAT_CAPABILITY_POINTER) != 0;
	const bool hasKeyboard = (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) != 0;
	
	if(hasPointer && !self.pWlPointer){
		self.pCreatePointer();
		
	}else if(!hasPointer && self.pWlPointer){
		self.pDestroyPointer();
	}
	
	if(hasKeyboard && !self.pWlKeyboard){
		self.pCreateKeyboard();
		
	}else if(!hasKeyboard && self.pWlKeyboard){
		self.pDestroyKeyboard();
	}
}

void dexsiWaylandInput::OnSeatName(void*, wl_seat*, const char*){
}


// Pointer callbacks
//////////////////////

void dexsiWaylandInput::OnPointerEnter(void *data, wl_pointer*, uint32_t serial,
wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	self.pPointerInWindow = true;
	self.pPointerSurface = surface;
	self.pPointerEnterSerial = serial;
	self.pPointerX = wl_fixed_to_double(sx);
	self.pPointerY = wl_fixed_to_double(sy);
	self.pPointerDeltaX = 0.0;
	self.pPointerDeltaY = 0.0;
	
	if(self.pModule.GetGameEngine()->GetInputSystem()->GetCaptureInputDevices()){
		self.pLockPointer();
	}
}

void dexsiWaylandInput::OnPointerLeave(void *data, wl_pointer*, uint32_t, wl_surface*){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	self.pUnlockPointer();
	self.pPointerInWindow = false;
	self.pPointerSurface = nullptr;
	self.pPointerEnterSerial = 0;
}

void dexsiWaylandInput::OnPointerMotion(void *data, wl_pointer*, uint32_t,
wl_fixed_t sx, wl_fixed_t sy){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	const double newX = wl_fixed_to_double(sx);
	const double newY = wl_fixed_to_double(sy);
	
	// when pointer is locked, skip absolute motion (relative pointer handles delta)
	if(!self.pPointerLocked && self.pPointerInWindow){
		self.pPointerDeltaX += newX - self.pPointerX;
		self.pPointerDeltaY += newY - self.pPointerY;
		self.pPointerHasMoved = true;
	}
	
	self.pPointerX = newX;
	self.pPointerY = newY;
}

void dexsiWaylandInput::OnPointerButton(void *data, wl_pointer*, uint32_t, uint32_t time,
uint32_t button, uint32_t state){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(!self.pWaylandMouse || !self.pPointerInWindow){
		return;
	}
	
	// map Linux button codes to mouse button indices:
	// BTN_LEFT=272, BTN_RIGHT=273, BTN_MIDDLE=274, BTN_SIDE=275(back), BTN_EXTRA=276(fwd)
	int buttonIndex;
	switch(button){
	case BTN_LEFT:
		buttonIndex = 0;
		break;
		
	case BTN_RIGHT:
		buttonIndex = 1;
		break;
		
	case BTN_MIDDLE:
		buttonIndex = 2;
		break;
		
	case BTN_SIDE:
		buttonIndex = 3;
		break;
		
	case BTN_EXTRA:
		buttonIndex = 4;
		break;
		
	default:
		return;
	}
	
	const int mouseDevice = self.pWaylandMouse->GetIndex();
	const int mods = self.pGetModifiers();
	const timeval eventTime = self.pMsToTimeval(time);
	
	const bool pressed = (state == WL_POINTER_BUTTON_STATE_PRESSED);
	
	if(self.pWaylandMouse->GetButtons().GetCount() > buttonIndex){
		self.pWaylandMouse->GetButtons().GetAt(buttonIndex)->SetPressed(pressed);
	}
	
	deInputEventQueue &queue = self.pModule.GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	event.SetType(pressed ? deInputEvent::eeMousePress : deInputEvent::eeMouseRelease);
	event.SetDevice(mouseDevice);
	event.SetCode(buttonIndex);
	event.SetState(mods);
	event.SetTime(eventTime);
	queue.AddEvent(event);
}

void dexsiWaylandInput::OnPointerAxis(void *data, wl_pointer*, uint32_t,
uint32_t axis, wl_fixed_t value){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	const double scrollValue = wl_fixed_to_double(value);
	
	// WL_POINTER_AXIS_VERTICAL_SCROLL=0 => vertical
	// WL_POINTER_AXIS_HORIZONTAL_SCROLL=1 => horizontal
	if(axis == WL_POINTER_AXIS_VERTICAL_SCROLL){
		self.pAccumScrollY += scrollValue;
		
	}else if(axis == WL_POINTER_AXIS_HORIZONTAL_SCROLL){
		self.pAccumScrollX += scrollValue;
	}
	self.pPointerHasScroll = true;
}

void dexsiWaylandInput::OnPointerFrame(void *data, wl_pointer*){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	timeval eventTime;
	gettimeofday(&eventTime, nullptr);
	self.pFlushPointerEvents(eventTime);
}

void dexsiWaylandInput::OnPointerAxisSource(void*, wl_pointer*, uint32_t){
}

void dexsiWaylandInput::OnPointerAxisStop(void*, wl_pointer*, uint32_t, uint32_t){
	// could be used to detect end of kinetic scroll
}

void dexsiWaylandInput::OnPointerAxisDiscrete(void*, wl_pointer*, uint32_t, int32_t){
	// discrete scroll steps for mice with click wheels.
	// already handled by OnPointerAxis accumulation
}


// Locked pointer callbacks
/////////////////////////////

void dexsiWaylandInput::OnLockedPointerLocked(void *data, zwp_locked_pointer_v1*){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	self.pPointerLocked = true;
	self.pPointerDeltaX = 0.0;
	self.pPointerDeltaY = 0.0;
	self.pPointerHasMoved = false;
}

void dexsiWaylandInput::OnLockedPointerUnlocked(void *data, zwp_locked_pointer_v1*){
	((dexsiWaylandInput*)data)->pPointerLocked = false;
}


// Relative pointer callbacks
///////////////////////////////

void dexsiWaylandInput::OnRelativeMotion(void *data, zwp_relative_pointer_v1*,
uint32_t utime_hi, uint32_t utime_lo,
wl_fixed_t dx, wl_fixed_t dy, wl_fixed_t, wl_fixed_t){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	if(!self.pPointerLocked){
		return;
	}
	
	self.pPointerDeltaX += wl_fixed_to_double(dx);
	self.pPointerDeltaY += wl_fixed_to_double(dy);
	self.pPointerHasMoved = true;
	
	const uint64_t us = ((uint64_t)utime_hi << 32) | (uint64_t)utime_lo;
	timeval eventTime;
	eventTime.tv_sec = (time_t)(us / 1000000u);
	eventTime.tv_usec = (suseconds_t)(us % 1000000u);
	
	self.pFlushPointerEvents(eventTime);
}


// Keyboard callbacks
///////////////////////

void dexsiWaylandInput::OnKeyboardKeymap(void *data, wl_keyboard*,
uint32_t format, int fd, uint32_t size){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1){
		close(fd);
		return;
	}
	
	// map the fd to get the keymap string
	void * const map = mmap(nullptr, (size_t)size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	
	if(map == MAP_FAILED){
		self.pModule.LogError("dexsiWaylandInput: Failed mapping keymap fd");
		return;
	}
	
	// create new XKB keymap from string
	xkb_keymap * const keymap = xkb_keymap_new_from_string(self.pXkbContext,
		(const char*)map, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	munmap(map, (size_t)size);
	
	if(!keymap){
		self.pModule.LogError("dexsiWaylandInput: Failed parsing XKB keymap");
		return;
	}
	
	// create new state from keymap
	xkb_state * const state = xkb_state_new(keymap);
	if(!state){
		xkb_keymap_unref(keymap);
		self.pModule.LogError("dexsiWaylandInput: Failed creating XKB state");
		return;
	}
	
	// replace old keymap/state
	if(self.pXkbState){
		xkb_state_unref(self.pXkbState);
	}
	if(self.pXkbKeymap){
		xkb_keymap_unref(self.pXkbKeymap);
	}
	
	self.pXkbKeymap = keymap;
	self.pXkbState = state;
	
	// rebuild keyboard buttons from new keymap
	self.pWaylandKeyboard->RebuildFromKeymap(keymap);
	self.pModule.LogInfoFormat("dexsiWaylandInput: Keymap loaded, %d keys",
		self.pWaylandKeyboard->GetButtons().GetCount());
}

void dexsiWaylandInput::OnKeyboardEnter(void *data, wl_keyboard*, uint32_t, wl_surface*, wl_array*){
	((dexsiWaylandInput*)data)->pKeyboardHasFocus = true;
}

void dexsiWaylandInput::OnKeyboardLeave(void *data, wl_keyboard*, uint32_t, wl_surface*){
	((dexsiWaylandInput*)data)->pKeyboardHasFocus = false;
}

void dexsiWaylandInput::OnKeyboardKey(void *data, wl_keyboard*, uint32_t,
uint32_t time, uint32_t key, uint32_t state){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(!self.pWaylandKeyboard || !self.pXkbState){
		return;
	}
	
	// linux keycode => XKB keycode (+8)
	const xkb_keycode_t xkbCode = key + 8;
	const int buttonIndex = self.pWaylandKeyboard->LookupLinuxKeyCode(key);
	if(buttonIndex < 0){
		return;
	}
	
	const bool pressed = state == WL_KEYBOARD_KEY_STATE_PRESSED;
	
	// get unicode character from current XKB state
	const uint32_t ucs32 = xkb_state_key_get_utf32(self.pXkbState, xkbCode);
	const int keyChar = (ucs32 >= 32 && ucs32 != 127) ? (int)ucs32 : 0;
	
	// update button pressed state
	dexsiDeviceButton * const deviceButton = self.pWaylandKeyboard->GetButtons().GetAt(buttonIndex);
	deviceButton->SetPressed(pressed);
	
	const deInputEvent::eKeyCodes keyCode = deviceButton->GetKeyCode();
	const int mods = self.pGetModifiers();
	const timeval eventTime = self.pMsToTimeval(time);
	const int kbDevice = self.pWaylandKeyboard->GetIndex();
	
	deInputEventQueue &queue = self.pModule.GetGameEngine()->GetInputSystem()->GetEventQueue();
	deInputEvent event;
	event.SetType(pressed ? deInputEvent::eeKeyPress : deInputEvent::eeKeyRelease);
	event.SetDevice(kbDevice);
	event.SetCode(buttonIndex);
	event.SetKeyCode(keyCode);
	event.SetKeyChar(keyChar);
	event.SetState(mods);
	event.SetTime(eventTime);
	queue.AddEvent(event);
	
	// update XKB state for modifier tracking
	xkb_state_update_key(self.pXkbState, xkbCode, pressed ? XKB_KEY_DOWN : XKB_KEY_UP);
}

void dexsiWaylandInput::OnKeyboardModifiers(void *data, wl_keyboard*, uint32_t,
uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group){
	dexsiWaylandInput &self = *(dexsiWaylandInput*)data;
	
	if(self.pXkbState){
		xkb_state_update_mask(self.pXkbState, mods_depressed, mods_latched, mods_locked, group, 0, 0);
	}
}

void dexsiWaylandInput::OnKeyboardRepeatInfo(void*, wl_keyboard*, int32_t, int32_t){
	// compositor manages key repeat
}

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
