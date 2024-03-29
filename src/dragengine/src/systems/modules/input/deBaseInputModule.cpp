/* 
 * Drag[en]gine Game Engine
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

#include "deBaseInputModule.h"
#include "../../../common/exceptions.h"
#include "../../../input/deInputDevicePose.h"


// Class deBaseInputModule
////////////////////////////

deBaseInputModule::deBaseInputModule( deLoadableModule &loadableModule ) :
deBaseModule( loadableModule ){
}

deBaseInputModule::~deBaseInputModule(){
}



// Notifications
//////////////////

bool deBaseInputModule::GetButtonTouched( int, int ){
	return false;
}

int deBaseInputModule::IndexOfComponentWithID( int, const char* ){
	return -1;
}

void deBaseInputModule::GetDevicePose( int, deInputDevicePose &pose ){
	pose = deInputDevicePose();
}

void deBaseInputModule::GetDeviceBonePose( int, int, bool, deInputDevicePose &pose ){
	pose = deInputDevicePose();
}

float deBaseInputModule::GetDeviceFaceExpression( int device, int expression ){
	return 0.0f;
}

int deBaseInputModule::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode,
deInputEvent::eKeyLocation ){
	return ButtonMatchingKeyCode( device, keyCode );
}

int deBaseInputModule::ButtonMatchingKeyChar( int device, int character, deInputEvent::eKeyLocation ){
	return ButtonMatchingKeyChar( device, character );
}

void deBaseInputModule::ProcessEvents(){
}

void deBaseInputModule::ClearEvents(){
}

void deBaseInputModule::ScreenSizeChanged(){
}

void deBaseInputModule::CaptureInputDevicesChanged(){
}

void deBaseInputModule::AppActivationChanged(){
}

#if defined OS_UNIX && defined HAS_LIB_X11
void deBaseInputModule::EventLoop( XEvent& ){
}
#endif

#ifdef OS_W32
void deBaseInputModule::EventLoop( const MSG& ){
}
#endif

#ifdef OS_ANDROID
void deBaseInputModule::EventLoop( const AInputEvent& ){
}
#endif
	
#ifdef OS_BEOS
void deBaseInputModule::EventLoop( const BMessage& ){
}
#endif
	
#ifdef OS_MACOS
void deBaseInputModule::EventLoop( const NSEvent& ){
}
#endif
