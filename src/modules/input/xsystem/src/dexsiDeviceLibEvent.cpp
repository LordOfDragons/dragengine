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
#include <errno.h>

#include "dexsiDeviceLibEvent.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Table
//////////

struct sTableEntryAxis{
	int code;
	deInputDeviceAxis::eAxisTypes type;
	const char *id;
	const char *displayName;
	const char *displayText;
	int renameCode;
	const char *renameDisplayText;
};

struct sTableEntryButton{
	int code;
	deInputDeviceButton::eButtonTypes type;
	const char *id;
	const char *displayName;
	const char *displayText;
	int renameCode;
	const char *renameDisplayText;
};

static const int vTableRelativeAxisCount = 4;
static const sTableEntryAxis vTableRelativeAxes[ vTableRelativeAxisCount ]{
	{ REL_X, deInputDeviceAxis::eatTouchPad, "tpx1", "touchpadX", "", REL_RX, "L" },
	{ REL_Y, deInputDeviceAxis::eatTouchPad, "tpy1", "touchpadY", "", REL_RY, "L" },
	{ REL_RX, deInputDeviceAxis::eatTouchPad, "tpx2", "touchpadX", "R"},
	{ REL_RY, deInputDeviceAxis::eatTouchPad, "tpy2", "touchpadY", "R", 0, nullptr }
	/*
	REL_Z:
	REL_RZ:
	REL_HWHEEL:
	REL_DIAL:
	REL_WHEEL:
	REL_MISC:
	REL_WHEEL_HI_RES:
	REL_HWHEEL_HI_RES:
	*/
};

static const int vTableTouchpadAbsAxisCount = 4;
static const sTableEntryAxis vTableTouchpadAbsAxis[vTableTouchpadAbsAxisCount]{
	{ABS_X, deInputDeviceAxis::eatTouchPad, "x", "touchpadX", ""},
	{ABS_Y, deInputDeviceAxis::eatTouchPad, "y", "touchpadY", ""},
	{ABS_MT_POSITION_X, deInputDeviceAxis::eatGeneric, nullptr},
	{ABS_MT_POSITION_Y, deInputDeviceAxis::eatGeneric, nullptr}
};

static const int vTableAbsAxisCount = 27;
static const sTableEntryAxis vTableAbsAxis[vTableAbsAxisCount]{
	{ ABS_X, deInputDeviceAxis::eatStick, "sx0", "stickX", "", ABS_RX, "L" },
	{ ABS_Y, deInputDeviceAxis::eatStick, "sy0", "stickY", "", ABS_RY, "L" },
	{ ABS_Z, deInputDeviceAxis::eatTrigger, "tr0", "trigger", "", ABS_RZ, "L" },
	{ ABS_RX, deInputDeviceAxis::eatStick, "sx1", "stickX", "R"},
	{ ABS_RY, deInputDeviceAxis::eatStick, "sy1", "stickY", "R"},
	{ ABS_RZ, deInputDeviceAxis::eatTrigger, "tr1", "trigger", "R"},
	{ ABS_THROTTLE, deInputDeviceAxis::eatThrottle, "thr", "trigger", "Thr"},
	{ ABS_RUDDER, deInputDeviceAxis::eatThrottle, "rud", "trigger", "Rud"},
	{ ABS_WHEEL, deInputDeviceAxis::eatSteeringWheel, "wh", "trigger", "Wh"},
	{ ABS_GAS, deInputDeviceAxis::eatTrigger, "gas", "trigger", "Gas"},
	{ ABS_BRAKE, deInputDeviceAxis::eatTrigger, "brk", "trigger", "Brk"},
	{ ABS_HAT0X, deInputDeviceAxis::eatHat, "hx0", "stickX", "", ABS_HAT1X, "1" },
	{ ABS_HAT0Y, deInputDeviceAxis::eatHat, "hy0", "stickY", "", ABS_HAT1Y, "1" },
	{ ABS_HAT1X, deInputDeviceAxis::eatHat, "hx1", "stickX", "2"},
	{ ABS_HAT1Y, deInputDeviceAxis::eatHat, "hy1", "stickY", "2"},
	{ ABS_HAT2X, deInputDeviceAxis::eatHat, "hx2", "stickX", "3"},
	{ ABS_HAT2Y, deInputDeviceAxis::eatHat, "hy2", "stickY", "3"},
	{ ABS_HAT3X, deInputDeviceAxis::eatHat, "hx3", "stickX", "4"},
	{ ABS_HAT3Y, deInputDeviceAxis::eatHat, "hy3", "stickY", "4"},
	{ ABS_PRESSURE, deInputDeviceAxis::eatGeneric, "pre", "trigger", "Pre"},
	{ ABS_DISTANCE, deInputDeviceAxis::eatGeneric, "dis", "trigger", "Dis"},
	{ ABS_TILT_X, deInputDeviceAxis::eatStick, "tltx", "stickX", "Tlt"},
	{ ABS_TILT_Y, deInputDeviceAxis::eatStick, "tlty", "stickY", "Tlt"},
	{ ABS_TOOL_WIDTH, deInputDeviceAxis::eatGeneric, "twi", "trigger", "Twi"},
	{ ABS_VOLUME, deInputDeviceAxis::eatGeneric, "vol", "trigger", "Vol"},
	{ ABS_MISC, deInputDeviceAxis::eatGeneric, "misc", "trigger", "Misc"},
#ifdef ABS_PROFILE
	{ ABS_PROFILE, deInputDeviceAxis::eatGeneric, "pro", "trigger", "Pro", 0, nullptr }
#else
	{ -1 } // missing on github
#endif
};

static const int vTableButtonCount = 64;
static const sTableEntryButton vTableButton[ vTableButtonCount ]{
	// misc
	{ BTN_0, deInputDeviceButton::ebtAction, "ba0", "button", "0"},
	{ BTN_1, deInputDeviceButton::ebtAction, "ba1", "button", "1"},
	{ BTN_2, deInputDeviceButton::ebtAction, "ba2", "button", "2"},
	{ BTN_3, deInputDeviceButton::ebtAction, "ba3", "button", "3"},
	{ BTN_4, deInputDeviceButton::ebtAction, "ba4", "button", "4"},
	{ BTN_5, deInputDeviceButton::ebtAction, "ba5", "button", "5"},
	{ BTN_6, deInputDeviceButton::ebtAction, "ba6", "button", "6"},
	{ BTN_7, deInputDeviceButton::ebtAction, "ba7", "button", "7"},
	{ BTN_8, deInputDeviceButton::ebtAction, "ba8", "button", "8"},
	{ BTN_9, deInputDeviceButton::ebtAction, "ba9", "button", "9"},
	
	// mouse
	{ BTN_LEFT, deInputDeviceButton::ebtAction, "bml", "button", "Left"},
	{ BTN_RIGHT, deInputDeviceButton::ebtAction, "bmr", "button", "Right"},
	{ BTN_MIDDLE, deInputDeviceButton::ebtAction, "bmm", "button", "Middle"},
	{ BTN_SIDE, deInputDeviceButton::ebtAction, "bms", "button", "Side"},
	{ BTN_EXTRA, deInputDeviceButton::ebtAction, "bme", "button", "Extra"},
	{ BTN_FORWARD, deInputDeviceButton::ebtAction, "bmf", "button", "Forward"},
	{ BTN_BACK, deInputDeviceButton::ebtAction, "bmb", "button", "Back"},
	{ BTN_TASK, deInputDeviceButton::ebtAction, "bmt", "button", "Task"},
	
	// joystick
	{ BTN_TRIGGER, deInputDeviceButton::ebtAction, "bjtr", "button", "Trigger"},
	{ BTN_THUMB, deInputDeviceButton::ebtAction, "bjtb1", "button", "Thumb"},
	{ BTN_THUMB2, deInputDeviceButton::ebtAction, "bjtb2", "button", "Thumb2"},
	{ BTN_TOP, deInputDeviceButton::ebtAction, "bjtop1", "button", "Top"},
	{ BTN_TOP2, deInputDeviceButton::ebtAction, "bjtop2", "button", "Top2"},
	{ BTN_PINKIE, deInputDeviceButton::ebtAction, "bjpin", "button", "Pinkie"},
	{ BTN_BASE, deInputDeviceButton::ebtAction, "bjba1", "button", "Base"},
	{ BTN_BASE2, deInputDeviceButton::ebtAction, "bjba2", "button", "Base2"},
	{ BTN_BASE3, deInputDeviceButton::ebtAction, "bjba3", "button", "Base3"},
	{ BTN_BASE4, deInputDeviceButton::ebtAction, "bjba4", "button", "Base4"},
	{ BTN_BASE5, deInputDeviceButton::ebtAction, "bjba5", "button", "Base5"},
	{ BTN_BASE6, deInputDeviceButton::ebtAction, "bjba6", "button", "Base6"},
	{ BTN_DEAD, deInputDeviceButton::ebtAction, "bjdead", "button", "Dead"},
	
	// gamepad
	{ BTN_A, deInputDeviceButton::ebtAction, "baa", "button", "A"}, // BTN_SOUTH
	{ BTN_B, deInputDeviceButton::ebtAction, "bab", "button", "B"}, // BTN_EAST
	{ BTN_C, deInputDeviceButton::ebtAction, "bac", "button", "C"},
	{ BTN_X, deInputDeviceButton::ebtAction, "bax", "button", "X"}, // BTN_NORTH
	{ BTN_Y, deInputDeviceButton::ebtAction, "bay", "button", "Y"}, // BTN_WEST
	{ BTN_Z, deInputDeviceButton::ebtAction, "baz", "button", "Z"},
	{ BTN_TL, deInputDeviceButton::ebtShoulderLeft, "basl1", "button", "SL", BTN_TL2, "SL1" },
	{ BTN_TR, deInputDeviceButton::ebtShoulderRight, "basr1", "button", "SR", BTN_TR2, "SL1" },
	{ BTN_TL2, deInputDeviceButton::ebtShoulderLeft, "basl2", "button", "SL2"},
	{ BTN_TR2, deInputDeviceButton::ebtShoulderRight, "basr2", "button", "SR2"},
	{ BTN_SELECT, deInputDeviceButton::ebtSelect, "basel", "button", "Select"},
	{ BTN_START, deInputDeviceButton::ebtHome, "basta", "button", "Start"},
	{ BTN_MODE, deInputDeviceButton::ebtAction, "bamod", "button", "Mode"},
	{ BTN_THUMBL, deInputDeviceButton::ebtStick, "batl", "button", "TL"},
	{ BTN_THUMBR, deInputDeviceButton::ebtStick, "batr", "button", "TR"},
	
	// digi
	{ BTN_TOOL_PEN, deInputDeviceButton::ebtAction, "badtp", "button", "Pen"},
	{ BTN_TOOL_RUBBER, deInputDeviceButton::ebtAction, "badtr", "button", "Rubber"},
	{ BTN_TOOL_BRUSH, deInputDeviceButton::ebtAction, "badtb", "button", "Brush"},
	{ BTN_TOOL_PENCIL, deInputDeviceButton::ebtAction, "badtpc", "button", "Pencil"},
	{ BTN_TOOL_AIRBRUSH, deInputDeviceButton::ebtAction, "badta", "button", "Airbrush"},
	{ BTN_TOOL_FINGER, deInputDeviceButton::ebtAction, "badtf", "button", "Finger"},
	{ BTN_TOOL_MOUSE, deInputDeviceButton::ebtAction, "badtm", "button", "Mouse"},
	{ BTN_TOOL_LENS, deInputDeviceButton::ebtAction, "badtl", "button", "Lens"},
	{ BTN_TOOL_DOUBLETAP, deInputDeviceButton::ebtAction, "badtt2", "button", "Tap2"},
	{ BTN_TOOL_TRIPLETAP, deInputDeviceButton::ebtAction, "badtt3", "button", "Tap3"},
	{ BTN_TOOL_QUADTAP, deInputDeviceButton::ebtAction, "badtt4", "button", "Tap4"},
	{ BTN_TOOL_QUINTTAP, deInputDeviceButton::ebtAction, "badtt5", "button", "Tap5"},
	{ BTN_STYLUS, deInputDeviceButton::ebtAction, "bads1", "button", "Stylus" },
	{ BTN_STYLUS2, deInputDeviceButton::ebtAction, "bads2", "button", "Stylus2" },
	{ BTN_STYLUS3, deInputDeviceButton::ebtAction, "bads3", "button", "Stylus3" },
	{ BTN_TOUCH, deInputDeviceButton::ebtAction, "badth", "button", "Touch" },
	
	// wheel
	{ BTN_GEAR_DOWN, deInputDeviceButton::ebtAction, "bawgd", "button", "GearDown" },
	{ BTN_GEAR_UP, deInputDeviceButton::ebtAction, "bawgu", "button", "GearUp" }
};


// Class dexsiDeviceLibEvent
//////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceLibEvent::dexsiDeviceLibEvent( deXSystemInput &module, const char *pathDevice ) :
dexsiDevice( module, esLibevdev ),

pEvdevPath( pathDevice ),
pEvdevFile( -1 ),
pEvdevDevice( NULL ),
pEvdevMapRelAxis( NULL ),
pEvdevMapAbsAxis( NULL ),
pEvdevMapKeys( NULL )
{
	// libevdev version
	// https://www.freedesktop.org/software/libevdev/doc/latest/index.html
	// SDL type mapping: https://meghprkh.github.io/blog/2016/06/03/Handling-joysticks-and-gamepads-in-linux/
	
	decString string;
	int i, j;
	
	// create libevdev device for device at pathDevice
	pEvdevFile = open( pathDevice, O_RDONLY | O_NONBLOCK );
	if( pEvdevFile == -1 ){
		DETHROW_INFO( deeOpenFile, pathDevice );
	}
	
	if( libevdev_new_from_fd( pEvdevFile, &pEvdevDevice ) < 0 ){
		DETHROW_INFO( deeOpenFile, pathDevice );
	}
	
	// retrieve device parameters
	SetName( libevdev_get_name( pEvdevDevice ) );
	
	const char * const unique = libevdev_get_uniq( pEvdevDevice );
	if( unique ){
		string.Format( "%s%d%s", XINP_DEVID_PREFIX, esLibevdev,
			dexsiDeviceManager::NormalizeID( unique ).GetString() );
		
	}else{
		const int bustype = libevdev_get_id_bustype( pEvdevDevice );
		const int vendor = libevdev_get_id_vendor( pEvdevDevice );
		const int product = libevdev_get_id_product( pEvdevDevice );
		const int version = libevdev_get_id_version( pEvdevDevice );
		
		string.Format( "%s%d%04x%04x%04x%04x", XINP_DEVID_PREFIX, esLibevdev,
			bustype, vendor, product, version );
	}
	
	SetID( string );
	
	// try to identify what kind of device this is

	/*
	Touchpads are problematic. Example report of an Elantech touchpad:

	'ELAN1200:00 04F3:307A Touchpad' (XSys_2) [2]
	Axes:
	- 'ABS_X' (sx0)[2] 0 .. 3234 [0 323]
	- 'ABS_Y' (sy0)[2] 0 .. 1661 [0 166]
	- 'ABS_MT_SLOT' (aa0)[7] 0 .. 4 [0 0]
	- 'ABS_MT_POSITION_X' (aa1)[7] 0 .. 3234 [0 0]
	- 'ABS_MT_POSITION_Y' (aa2)[7] 0 .. 1661 [0 0]
	- 'ABS_MT_TOOL_TYPE' (aa3)[7] 0 .. 2 [0 0]
	- 'ABS_MT_TRACKING_ID' (aa4)[7] 0 .. 65535 [0 0]

	Buttons:
	- 'BTN_LEFT' (bml)[1] 272 => 0
	- 'BTN_RIGHT' (bmr)[1] 273 => 1
	- 'BTN_TOOL_FINGER' (badtf)[1] 325 => 2
	- 'BTN_TOOL_QUINTTAP' (badtt5)[1] 328 => 3
	- 'BTN_TOUCH' (badth)[1] 330 => 4
	- 'BTN_TOOL_DOUBLETAP' (badtt2)[1] 333 => 5
	- 'BTN_TOOL_TRIPLETAP' (badtt3)[1] 334 => 6
	- 'BTN_TOOL_QUADTAP' (badtt4)[1] 335 => 7
	*/

	const bool hasRelativeAxes = libevdev_has_event_type(pEvdevDevice, EV_REL);
	const bool hasAbsoluteAxes = libevdev_has_event_type(pEvdevDevice, EV_ABS);
	const bool hasKeys = libevdev_has_event_type(pEvdevDevice, EV_KEY);
	const bool isKeyboard = libevdev_has_event_code(pEvdevDevice, EV_KEY, KEY_ENTER)
		|| libevdev_has_event_code(pEvdevDevice, EV_KEY, KEY_ESC);
	const bool isMouse = libevdev_has_event_code(pEvdevDevice, EV_KEY, BTN_LEFT);
	const bool isTouchpad = libevdev_has_event_code(pEvdevDevice, EV_ABS, ABS_MT_POSITION_X)
		|| libevdev_has_event_code(pEvdevDevice, EV_ABS, ABS_MT_POSITION_Y);
	
	const decString lcname(GetName().GetLower());
	if(lcname.FindString("mouse") != -1 || lcname.FindString("keyboard") != -1){
		// drop mouse and keyboard. they are covered by core input already and mess things up
		return;
	}
	
	if(hasRelativeAxes && isMouse){
		SetType(deInputDevice::edtMouse);
		SetDisplayImages("mouse");
		
	}else if(hasKeys && isKeyboard){
		SetType(deInputDevice::edtKeyboard);
		SetDisplayImages("keyboard");

	}else if(hasAbsoluteAxes && isTouchpad){
		SetType(deInputDevice::edtTouchpad);
		SetDisplayImages("touchpad");
		
	}else if(hasAbsoluteAxes && hasKeys){
		SetType(deInputDevice::edtGamepad);
		SetDisplayImages("gamepad");

	}else{
		// we do not know what this is. ignore it to avoid problems
		return;
	}
	
	// relative and absolute axes
	int countAxes = 0;
	
	if( hasRelativeAxes ){
		for( i=0; i<REL_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_REL, i ) ){
				countAxes++;
			}
		}
	}
	
	if( hasAbsoluteAxes ){
		for( i=0; i<ABS_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_ABS, i ) ){
				countAxes++;
			}
		}
	}
	
	if( hasRelativeAxes ){
		pEvdevMapRelAxis = new short[ REL_MAX ];
		for( i=0; i<REL_MAX; i++ ){
			pEvdevMapRelAxis[ i ] = -1;
		}
	}
	if( hasAbsoluteAxes ){
		pEvdevMapAbsAxis = new short[ ABS_MAX ];
		for( i=0; i<ABS_MAX; i++ ){
			pEvdevMapAbsAxis[ i ] = -1;
		}
	}
	
	int indexAxis = 0;
	
	if( hasRelativeAxes ){
		for( i=0; i<REL_MAX; i++ ){
			if( ! libevdev_has_event_code( pEvdevDevice, EV_REL, i ) ){
				continue;
			}
			
			const dexsiDeviceAxis::Ref axis( dexsiDeviceAxis::Ref::NewWith(module) );
			AddAxis( axis );
			axis->SetIndex( indexAxis );
			axis->SetName( libevdev_event_code_get_name( EV_REL, i ) );
			axis->SetEvdevCode( i );
			axis->SetAbsolute( false );
			
			for( j=0; j<vTableRelativeAxisCount; j++ ){
				const sTableEntryAxis &t = vTableRelativeAxes[ j ];
				if( t.code != i ){
					continue;
				}
				
				axis->SetType( t.type );
				axis->SetDisplayImages( t.displayName );
				axis->SetID( t.id );
				axis->SetDisplayText( t.displayText );
				if( t.renameDisplayText && libevdev_has_event_code( pEvdevDevice, EV_REL, t.renameCode ) ){
					axis->SetDisplayText( t.renameDisplayText );
				}
				break;
			}
			
			if( j == vTableRelativeAxisCount ){
				axis->SetType( deInputDeviceAxis::eatGeneric );
				
				string.Format( "ra%d", i );
				axis->SetID( string );
				
				string.Format( "%d", indexAxis + 1 );
				axis->SetDisplayText( string );
			}
			
			pEvdevMapRelAxis[ i ] = indexAxis++;
		}
	}
	
	if( hasAbsoluteAxes ){
		int nextGeneric = 0;
		
		for(i=0; i<ABS_MAX; i++){
			if(!libevdev_has_event_code(pEvdevDevice, EV_ABS, i)){
				continue;
			}
			
			const sTableEntryAxis *entry = nullptr;

			if(GetType() == deInputDevice::edtTouchpad){
				for(j=0; j<vTableTouchpadAbsAxisCount; j++){
					const sTableEntryAxis &t = vTableTouchpadAbsAxis[j];
					if(t.code == i){
						entry = &t;
						break;
					}
				}
			}

			if(!entry){
				for(j=0; j<vTableAbsAxisCount; j++){
					const sTableEntryAxis &t = vTableAbsAxis[j];
					if(t.code == i){
						entry = &t;
						break;
					}
				}
			}
			
			if(entry && !entry->id){
				continue;
			}
			
			const dexsiDeviceAxis::Ref axis(dexsiDeviceAxis::Ref::NewWith(module));
			AddAxis(axis);
			axis->SetIndex(indexAxis);
			axis->SetName(libevdev_event_code_get_name(EV_ABS, i));
			axis->SetEvdevCode(i);
			axis->SetAbsolute(true);
			axis->SetMinimum(libevdev_get_abs_minimum(pEvdevDevice, i));
			axis->SetMaximum(libevdev_get_abs_maximum(pEvdevDevice, i));
			axis->SetFuzz(libevdev_get_abs_fuzz(pEvdevDevice, i));
			axis->SetFlat(libevdev_get_abs_flat(pEvdevDevice, i));

			if(entry){
				axis->SetType(entry->type);
				axis->SetDisplayImages(entry->displayName);
				axis->SetID(entry->id);
				axis->SetDisplayText(entry->displayText);
				if(entry->renameDisplayText && libevdev_has_event_code(pEvdevDevice, EV_ABS, entry->renameCode)){
					axis->SetDisplayText(entry->renameDisplayText);
				}

			}else{
				axis->SetType(deInputDeviceAxis::eatGeneric);
				axis->SetDisplayImages("stick");
				
				string.Format("aa%d", nextGeneric++);
				axis->SetID(string);
				
				string.Format("%d", nextGeneric);
				axis->SetDisplayText(string);
			}
			
			if(axis->GetType() == deInputDeviceAxis::eatStick){
				// libevdev likes to lie about the deadzone of input devices. ensure the deadzone
				// is not smaller than a specific percentage of the total range. typical deadzone
				// ranges are 0.2 - 0.25 of half-range
				axis->LimitFlat(0.1f);
			}
			
			pEvdevMapAbsAxis[i] = indexAxis++;
		}
	}
	
	// keys and buttons
	if( hasKeys ){
		int countButtons = 0;
		
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			if( libevdev_has_event_code( pEvdevDevice, EV_KEY, i ) ){
				countButtons++;
			}
		}
		
		pEvdevMapKeys = new short[ KEY_MAX - BTN_MISC ];
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			pEvdevMapKeys[ i - BTN_MISC ] = -1;
		}
		
		int indexButton = 0;
		int nextGeneric = 0;
		
		for( i=BTN_MISC; i<KEY_MAX; i++ ){
			if( ! libevdev_has_event_code( pEvdevDevice, EV_KEY, i ) ){
				continue;
			}
			
			const dexsiDeviceButton::Ref button( dexsiDeviceButton::Ref::NewWith(module) );
			AddButton( button );
			button->SetName( libevdev_event_code_get_name( EV_KEY, i ) );
			button->SetEvdevCode( i );
			
			for( j=0; j<vTableButtonCount; j++ ){
				const sTableEntryButton &t = vTableButton[ j ];
				if( t.code != i ){
					continue;
				}
				
				button->SetType( t.type );
				button->SetDisplayImages( t.displayName );
				button->SetID( t.id );
				button->SetDisplayText( t.displayText );
				if( t.renameDisplayText && libevdev_has_event_code( pEvdevDevice, EV_KEY, t.renameCode ) ){
					button->SetDisplayText( t.renameDisplayText );
				}
				break;
			}
			
			if( j == vTableButtonCount ){
				button->SetType( deInputDeviceButton::ebtGeneric );
				button->SetDisplayImages( "button" );
				
				string.Format( "bg%d", nextGeneric++ );
				button->SetID( string );
				
				string.Format( "%d", nextGeneric );
				button->SetDisplayText( string );
			}
			
			pEvdevMapKeys[ i - BTN_MISC ] = indexButton++;
		}
	}
	
	/*
	if( libevdev_has_event_type( evdev, EV_SW ) ){
		pModule.LogInfo( "  switches:" );
		for( j=0; j<SW_MAX; j++ ){
			if( libevdev_has_event_code( evdev, EV_SW, j ) ){
				pModule.LogInfoFormat( "  - 0x%x", j );
			}
		}
	}
	
	if( libevdev_has_event_type( evdev, EV_LED ) ){
		pModule.LogInfo( "  led:" );
		for( j=0; j<LED_MAX; j++ ){
			if( libevdev_has_event_code( evdev, EV_LED, j ) ){
				pModule.LogInfoFormat( "  - 0x%x", j );
			}
		}
	}
	*/
	
	/*
	const int slotCount = libevdev_get_num_slots( evdev );
	pModule.LogInfoFormat( "  slotcount=%d", slotCount );
	//INPUT_PROP_BUTTONPAD;
	for( j=0; j<slotCount; j++ ){
	}
	*/
}

dexsiDeviceLibEvent::~dexsiDeviceLibEvent(){
	if( pEvdevMapKeys ){
		delete [] pEvdevMapKeys;
	}
	if( pEvdevMapAbsAxis ){
		delete [] pEvdevMapAbsAxis;
	}
	if( pEvdevMapRelAxis ){
		delete [] pEvdevMapRelAxis;
	}
	if( pEvdevDevice ){
		libevdev_free( pEvdevDevice );
	}
	if( pEvdevFile != -1 ){
		close( pEvdevFile );
	}
}



// Management
///////////////

void dexsiDeviceLibEvent::Update(){
	deXSystemInput &module = GetModule();
	const int deviceIndex = GetIndex();
	int flag = LIBEVDEV_READ_FLAG_NORMAL;
	input_event ev;
	
	while( true ){
		const int result = libevdev_next_event( pEvdevDevice, flag, &ev );
		
		if( result == -EAGAIN ){
			break; // no more events
			
		}else if( result == LIBEVDEV_READ_STATUS_SYNC ){
			flag = LIBEVDEV_READ_FLAG_SYNC;
			continue;
			
		}else{
			switch( ev.type ){
			case EV_REL:
				if( pEvdevMapRelAxis && ev.code >= 0 && ev.code < REL_MAX
				&& pEvdevMapRelAxis[ ev.code ] != -1 ){
					GetAxisAt( pEvdevMapRelAxis[ ev.code ] )->EvdevProcessEvent( *this, ev );
				}
				break;
				
			case EV_ABS:
				if( pEvdevMapAbsAxis && ev.code >= 0 && ev.code < ABS_MAX
				&& pEvdevMapAbsAxis[ ev.code ] != -1 ){
					GetAxisAt( pEvdevMapAbsAxis[ ev.code ] )->EvdevProcessEvent( *this, ev );
				}
				break;
				
			case EV_KEY:
				if( pEvdevMapKeys && ev.code >= BTN_MISC && ev.code < KEY_MAX
				&& pEvdevMapKeys[ ev.code - BTN_MISC ] != -1 ){
					const int code = pEvdevMapKeys[ ev.code - BTN_MISC ];
					
					switch( ev.value ){
					case 0: // release
						GetButtonAt( code )->SetPressed( false );
						module.AddButtonReleased( deviceIndex, code, ev.time );
						break;
						
					case 1: // press
						GetButtonAt( code )->SetPressed( true );
						module.AddButtonPressed( deviceIndex, code, ev.time );
						break;
						
					case 2: // auto-repeat (or auto-fire)
						GetButtonAt( code )->SetPressed( true );
						module.AddButtonReleased( deviceIndex, code, ev.time );
						module.AddButtonPressed( deviceIndex, code, ev.time );
						break;
					}
				}
				break;
				
			default:
				break;
			}
		}
	}
	
	SendDirtyAxisEvents();
}


// Private Functions
//////////////////////

