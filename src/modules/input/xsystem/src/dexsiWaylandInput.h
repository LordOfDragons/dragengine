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

#ifndef _DEXSIWAYLANDINPUT_H_
#define _DEXSIWAYLANDINPUT_H_

#include <dragengine/dragengine_configuration.h>
#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include <sys/time.h>
#include <xkbcommon/xkbcommon.h>
#include <dragengine/deObject.h>
#include <dragengine/input/deInputEvent.h>

#include "dexsiDeviceWaylandMouse.h"
#include "dexsiDeviceWaylandKeyboard.h"
#include "wayland/wayland-defs.h"

class deXSystemInput;


/**
 * Wayland input manager.
 *
 * Manages wl_seat, wl_pointer and wl_keyboard using a dedicated wl_event_queue for thread safety.
 */
class dexsiWaylandInput : public deObject{
public:
	using Ref = deTObjectReference<dexsiWaylandInput>;
	
	
private:
	deXSystemInput &pModule;
	
	wl_display *pWlDisplay;
	wl_registry *pWlRegistry;
	wl_event_queue *pInputQueue;
	wl_seat *pWlSeat;
	uint32_t pWlSeatId;
	uint32_t pWlSeatVersion;
	wl_pointer *pWlPointer;
	wl_keyboard *pWlKeyboard;
	
	xkb_context *pXkbContext;
	xkb_keymap *pXkbKeymap;
	xkb_state *pXkbState;
	
	dexsiDeviceWaylandMouse::Ref pWaylandMouse;
	dexsiDeviceWaylandKeyboard::Ref pWaylandKeyboard;
	
	/** Pointer constraints and relative pointer. */
	zwp_pointer_constraints_v1 *pPointerConstraints;
	uint32_t pPointerConstraintsId;
	zwp_relative_pointer_manager_v1 *pRelPtrManager;
	uint32_t pRelPtrManagerId;
	zwp_locked_pointer_v1 *pLockedPointer;
	zwp_relative_pointer_v1 *pRelativePointer;
	bool pPointerLocked;
	
	/** Pointer state. */
	double pPointerX, pPointerY;
	double pPointerDeltaX, pPointerDeltaY;
	bool pPointerInWindow;
	wl_surface *pPointerSurface;
	uint32_t pPointerEnterSerial;
	bool pPointerHasMoved;
	double pAccumScrollX, pAccumScrollY;
	bool pPointerHasScroll;
	
	/** Keyboard focus. */
	bool pKeyboardHasFocus;
	
	bool pReady;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create Wayland input. */
	explicit dexsiWaylandInput(deXSystemInput &module);
	
protected:
	/** Clean up Wayland input. */
	~dexsiWaylandInput() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Wayland input is ready to use. */
	inline bool IsReady() const{ return pReady; }
	
	/** Wayland mouse device. */
	inline const dexsiDeviceWaylandMouse::Ref &GetWaylandMouse() const{ return pWaylandMouse; }
	
	/** Wayland keyboard device. */
	inline const dexsiDeviceWaylandKeyboard::Ref &GetWaylandKeyboard() const{ return pWaylandKeyboard; }
	
	/** Dispatch pending Wayland input events (non-blocking). */
	void DispatchEvents();
	
	/** Update pointer capture state to match current input-capture setting. */
	void UpdateCapture();
	
	
	/** \name Listener callbacks */
	/*@{*/
	/** Registry listener callbacks. */
	static void OnRegistryGlobal(void *data, wl_registry *registry,
		uint32_t name, const char *interface, uint32_t version);
	
	static void OnRegistryGlobalRemove(void *data, wl_registry *registry, uint32_t name);
	
	/** Seat listener callbacks. */
	static void OnSeatCapabilities(void *data, wl_seat *seat, uint32_t capabilities);
	static void OnSeatName(void *data, wl_seat *seat, const char *name);
	
	/** Pointer listener callbacks. */
	static void OnPointerEnter(void *data, wl_pointer *pointer, uint32_t serial,
		wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy);
	
	static void OnPointerLeave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface);
	
	static void OnPointerMotion(void *data, wl_pointer *pointer, uint32_t time,
		wl_fixed_t sx, wl_fixed_t sy);
	
	static void OnPointerButton(void *data, wl_pointer *pointer, uint32_t serial,
		uint32_t time, uint32_t button, uint32_t state);
	
	static void OnPointerAxis(void *data, wl_pointer *pointer, uint32_t time,
		uint32_t axis, wl_fixed_t value);
	
	static void OnPointerFrame(void *data, wl_pointer *pointer);
	
	static void OnPointerAxisSource(void *data, wl_pointer *pointer, uint32_t axis_source);
	
	static void OnPointerAxisStop(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis);
	
	static void OnPointerAxisDiscrete(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete);
	
	/** Locked pointer listener callbacks. */
	static void OnLockedPointerLocked(void *data, zwp_locked_pointer_v1 *lockedPointer);
	static void OnLockedPointerUnlocked(void *data, zwp_locked_pointer_v1 *lockedPointer);
	
	/** Relative pointer listener callback. */
	static void OnRelativeMotion(void *data, zwp_relative_pointer_v1 *relPointer,
		uint32_t utime_hi, uint32_t utime_lo, wl_fixed_t dx, wl_fixed_t dy,
		wl_fixed_t dx_unaccel, wl_fixed_t dy_unaccel);
	
	/** Keyboard listener callbacks. */
	static void OnKeyboardKeymap(void *data, wl_keyboard *keyboard,
		uint32_t format, int fd, uint32_t size);
	
	static void OnKeyboardEnter(void *data, wl_keyboard *keyboard,
		uint32_t serial, wl_surface *surface, wl_array *keys);
	
	static void OnKeyboardLeave(void *data, wl_keyboard *keyboard,
		uint32_t serial, wl_surface *surface);
	
	static void OnKeyboardKey(void *data, wl_keyboard *keyboard, uint32_t serial,
		uint32_t time, uint32_t key, uint32_t state);
	
	static void OnKeyboardModifiers(void *data, wl_keyboard *keyboard, uint32_t serial,
		uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
	
	static void OnKeyboardRepeatInfo(void *data, wl_keyboard *keyboard, int32_t rate, int32_t delay);
	/*@}*/
	
	
private:
	void pCreatePointer();
	void pDestroyPointer();
	void pCreateKeyboard();
	void pDestroyKeyboard();
	void pLockPointer();
	void pUnlockPointer();
	void pFlushPointerEvents(const timeval &eventTime);
	
	int pGetModifiers() const;
	timeval pMsToTimeval(uint32_t ms) const;
	void pEnqueueMouseMove(int x, int y, int state, const timeval &time);
};

#endif // OS_UNIX_X11 && OS_UNIX_WAYLAND
#endif // _DEXSIWAYLANDINPUT_H_
