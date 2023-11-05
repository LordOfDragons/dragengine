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

#include "dexsiDevice.h"
#include "dexsiDeviceAxis.h"
#include "dexsiDeviceButton.h"
#include "dexsiDeviceFeedback.h"
#include "dexsiDeviceManager.h"
#include "dexsiDeviceCoreMouse.h"
#include "dexsiDeviceCoreKeyboard.h"
#include "dexsiDeviceLibEvent.h"
#include "deXSystemInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOSUnix.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deInputSystem.h>



// Class dexsiDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiDeviceManager::dexsiDeviceManager( deXSystemInput &module ) :
pModule( module ),
pX11CoreMouse( NULL ),
pX11CoreKeyboard( NULL ),
pPrimaryMouse( NULL ),
pPrimaryKeyboard( NULL )
{
	try{
		pCreateDevices();
		pFindPrimaryDevices();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dexsiDeviceManager::~dexsiDeviceManager(){
	pCleanUp();
}



// Management
///////////////

void dexsiDeviceManager::UpdateDeviceList(){
}



int dexsiDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

dexsiDevice *dexsiDeviceManager::GetAt( int index ) const{
	return ( dexsiDevice* )pDevices.GetAt( index );
}

dexsiDevice *dexsiDeviceManager::GetWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dexsiDevice * const device = ( dexsiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return NULL;
}

int dexsiDeviceManager::IndexOfWithID( const char *id ){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dexsiDevice * const device = ( dexsiDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}



void dexsiDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pModule.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const dexsiDevice &device = *( ( dexsiDevice* )pDevices.GetAt( i ) );
		pModule.LogInfoFormat( "- '%s' (%s) [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetType() );
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pModule.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const dexsiDeviceAxis &axis = *device.GetAxisAt( j );
				pModule.LogInfoFormat( "    - '%s' (%s) %d .. %d [%d %d]",
					axis.GetName().GetString(), axis.GetID().GetString(), axis.GetMinimum(),
					axis.GetMaximum(), axis.GetFuzz(), axis.GetFlat() );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			if( device.GetType() == deInputDevice::edtKeyboard ){
				pModule.LogInfoFormat( "  Keys: %d", buttonCount );
				
			}else{
				pModule.LogInfo( "  Buttons:" );
				for( j=0; j<buttonCount; j++ ){
					const dexsiDeviceButton &button = *device.GetButtonAt( j );
					pModule.LogInfoFormat( "    - '%s' (%s) %d => %d",
						button.GetName().GetString(), button.GetID().GetString(),
						button.GetEvdevCode(), j );
				}
			}
		}
	}
}



decString dexsiDeviceManager::NormalizeID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = strlen( id );
	if( len == 0 ){
		return decString();
	}
	
	decString nid;
	nid.Set( ' ', len );
	
	int i;
	for( i=0; i<len; i++ ){
		if( ( id[ i ]  >= 'A' && id[ i ] <= 'Z' )
		|| ( id[ i ] >= 'a' && id[ i ] <= 'z' )
		|| ( id[ i ] >= '0' && id[ i ] <= '9' )
		|| id[ i ] == '_' ){
			nid[ i ] = id[ i ];
			
		}else{
			nid[ i ] = '_';
		}
	}
	
	return nid;
}



// Private functions
//////////////////////

void dexsiDeviceManager::pCleanUp(){
	pDevices.RemoveAll();
	if( pX11CoreKeyboard ){
		pX11CoreKeyboard->FreeReference();
	}
	if( pX11CoreMouse ){
		pX11CoreMouse->FreeReference();
	}
}



// general extension information: // https://www.x.org/wiki/guide/extensions

void dexsiDeviceManager::pCreateXInputDevices(){
	// from XInput extension add what is not core
	// 
	// NOTE core mouse and keyboard parameters can only be queried using XInput
	// 
	// PROBLEM for some reason this doesn't work and causes segfaults. documentation is
	//         complicated and does not give correct information on how this is all
	//         supposed to work.
	// 
	// possible solution is XInput2 which might be better designed than the previous version
	// https://www.x.org/releases/X11R7.7/doc/inputproto/XI2proto.txt
	return;
	
	// check if XInput in the required version is present
	Display * const display = pModule.GetOSUnix()->GetDisplay();
	
	int extOpcode = -1;
	int extEvent;
	int extError;
	if( ! XQueryExtension(display, "XInputExtension", &extOpcode, &extEvent, &extError) ){
		DETHROW( deeInvalidParam );
	}
	
	int extOpcodeMajor = 2;
	int extOpcodeMinor = 2;
	if( XIQueryVersion( display, &extOpcodeMajor, &extOpcodeMinor ) != Success ){
		DETHROW( deeInvalidParam );
	}
	
	// what the?! using XIQueryDevice causes XCloseDisplay to segfault. happens on both
	// XInput and XInput2. is there some hidden magic required to not segfault?
	int countDevices;
	XIDeviceInfo * const devices = XIQueryDevice( display, XIAllDevices, &countDevices ); // XIAllMasterDevices
	
	
	XIFreeDeviceInfo( devices );
	
/*
typedef struct{
	int                 deviceid;
	char                *name;
	int                 use;
	int                 attachment;
	Bool                enabled;
	int                 num_classes;
	XIAnyClassInfo      **classes;
} XIDeviceInfo;

The use and attachment fields specify the type of the device
and the current attachment or pairing.
- If use is XIMasterPointer, the device is a master pointer and
  attachment specifies the deviceid of the paired master
  keyboard.
- If use is XIMasterKeyboard, the device is a master keyboard,
  and the attachment field specifies the paired master pointer.
- If use is XISlavePointer, the device is a slave device and
  currently attached to the master pointer specified in
  attachement.
- If use is XISlaveKeyboard, the device is a slave device an
  currently attached to the master keyboard specified in
  attachment.
- If use is XIFloatingSlave, the device is a slave device
  currently not attached to any master device. The value of the
  attachment field for floating slave devices is undefined.

typedef struct{
	int         type;
	int         sourceid;
} XIAnyClassInfo;



A device may have zero or one XIButtonClass, denoting the
device's capability to send button events.

typedef struct{
    int           mask_len;
    unsigned char *mask;
} XIButtonState;

typedef struct{
    int         type;
    int         sourceid;
    int         num_buttons;
    Atom        *labels;
    XIButtonState state;
} XIButtonClassInfo;

labels is a list of num_buttons Atoms specifying the button
labels for this device. If the label is not None, then the
label specifies the type of button in physical device order
(i.e. as the buttons are numbered on the physical input
device).



A device may have zero or one XIKeyClass, denoting the device's
capability to send key events.

typedef struct{
    int         type;
    int         sourceid;
    int         num_keycodes;
    int         *keycodes;
} XIKeyClassInfo;

// RPTD: should be possible to convert KeyCode to KeySym then use XKeysymToString



A device may have zero or more XIValuatorClass, denoting the
device's capability to send coordinates.

typedef struct{
    int         type;
    int         sourceid;
    int         number;
    Atom        label;
    double      min;
    double      max;
    double      value;
    int         resolution;
    int         mode;
} XIValuatorInfo;

If the label field is not None, the value of label is an Atom
describing the axis.

min and max are the minimum and maximum values allowed on this
axis. If both are zero, no minumum or maximum values are set on
this device. value is the current value of this axis.

The resolution field specifies the resolution of the device in
units/m.

The mode specifies the mode of this axis. If the mode is
XIModeAbsolute this axis sends absolute coordinates. If the
mode is XIModeRelative, this device sends relative coordinates.
*/
	
	
	
	
#if 0
	Display * const display = pModule.GetOSUnix()->GetDisplay();
	XDeviceInfo *xdevices = NULL;
	dexsiDevice *device = NULL;
	int i, countDevices;
	
	try{
		xdevices = XListInputDevices( display, &countDevices );
		if( ! xdevices ){
			return;
		}
		
		for( i=0; i<countDevices; i++ ){
			if( xdevices[ i ].use == IsXPointer ){
				pModule.LogInfo( "  -> Core Pointer" );
				
			}else if( xdevices[ i ].use == IsXKeyboard ){
				pModule.LogInfo( "  -> Core Keyboard" );
				
			}else if( xdevices[ i ].use == IsXExtensionDevice ){
				pModule.LogInfo( "  -> Extension Device" );
			}
			// device can only be added if it is core or extension device. all other uses
			// are not useful since we can not open the device
			
			device = new dexsiDevice( pModule, xdevices[ i ] );
			device->FreeReference();
		}
		
	}catch( const deException & ){
		if( device ){
			device->FreeReference();
		}
		if( xdevices ){
			XFreeDeviceList( xdevices );
		}
		throw;
	}
	
	XFreeDeviceList( xdevices );
#endif
	
	// old interface: joystick interface
	// /dev/input/jsX => include linux/joystick,h
	// https://www.kernel.org/doc/Documentation/input/joystick-api.txt
}

void dexsiDeviceManager::pCreateEvdevDevices(){
	// from libevdev add gamepads and joysticks
	// https://www.freedesktop.org/software/libevdev/doc/latest/index.html
	// SDL type mapping: https://meghprkh.github.io/blog/2016/06/03/Handling-joysticks-and-gamepads-in-linux/
	const char * const basePath[] = { "/dev/input/event%d", "/dev/event%d" };
	deObjectReference refDevice;
	decString pathDevice;
	int i, bp;
	
	for( bp=0; bp<2; bp++ ){
		for( i=0; i<24; i++ ){
			pathDevice.Format( basePath[ bp ], i );
			
			try{
				refDevice.TakeOver( new dexsiDeviceLibEvent( pModule, pathDevice ) );
				dexsiDeviceLibEvent * const device = ( dexsiDeviceLibEvent* )( deObject* )refDevice;
				
				switch( device->GetType() ){
				case deInputDevice::edtGamepad:
					device->SetIndex( pDevices.GetCount() );
					pDevices.Add( device );
					break;
					
				default:
					break;
				}
				
			}catch( const deException & ){
				// ignore
			}
		}
	}
}



void dexsiDeviceManager::pCreateDevices(){
	pX11CoreMouse = new dexsiDeviceCoreMouse( pModule );
	pX11CoreMouse->SetIndex( pDevices.GetCount() );
	pDevices.Add( pX11CoreMouse );
	
	pX11CoreKeyboard = new dexsiDeviceCoreKeyboard( pModule );
	pX11CoreKeyboard->SetIndex( pDevices.GetCount() );
	pDevices.Add( pX11CoreKeyboard );
	
	pCreateXInputDevices();
	pCreateEvdevDevices();
}

void dexsiDeviceManager::pFindPrimaryDevices(){
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dexsiDevice * const device = ( dexsiDevice* )pDevices.GetAt( i );
		
		switch( device->GetType() ){
		case deInputDevice::edtMouse:
			if( ! pPrimaryMouse ){
				pPrimaryMouse = device;
			}
			break;
			
		case deInputDevice::edtKeyboard:
			if( ! pPrimaryKeyboard ){
				pPrimaryKeyboard = device;
			}
			break;
			
		default:
			break;
		}
	}
	
	if( ! pPrimaryMouse ){
		pModule.LogError( "No mouse device found" );
		DETHROW( deeInvalidParam );
	}
	if( ! pPrimaryKeyboard ){
		pModule.LogError( "No keyboard device found" );
		DETHROW( deeInvalidParam );
	}
}
