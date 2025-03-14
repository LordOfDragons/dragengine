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

#ifdef OS_ANDROID
void deBaseInputModule::EventLoop(const android_input_buffer &inputBuffer){
}
#elif defined OS_BEOS
void deBaseInputModule::EventLoop(const BMessage&){
}
#elif defined OS_MACOS
void deBaseInputModule::EventLoop( const NSEvent& ){
}
#elif defined OS_UNIX && defined HAS_LIB_X11
void deBaseInputModule::EventLoop( XEvent& ){
}
#elif defined OS_W32
void deBaseInputModule::EventLoop( const MSG& ){
}
#endif
