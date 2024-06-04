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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "igdeNullInputModule.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/deModuleSystem.h"
#include "dragengine/systems/modules/deBaseModule.h"
#include "dragengine/common/exceptions.h"



// igdeNullInputModule::cModule
///////////////////////////////

igdeNullInputModule::cModule::cModule( deModuleSystem *system ) : deInternalModule( system ){
	SetName( "NullInput" );
	SetType( deModuleSystem::emtInput );
	SetDescription( "Null Input Module" );
	SetAuthor( "Plüss Roland" );
	SetVersion( "1.0" );
	SetDirectoryName( "NullInput" );
}

igdeNullInputModule::cModule::~cModule(){
}

void igdeNullInputModule::cModule::CreateModule(){
	SetModule( new igdeNullInputModule( *this ) );
	if( ! GetModule() ) SetErrorCode( eecCreateModuleFailed );
}



// class igdeNullInputModule
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNullInputModule::igdeNullInputModule( deLoadableModule &loadableModule ) :
deBaseInputModule( loadableModule ){
}

igdeNullInputModule::~igdeNullInputModule( ){
}



// Runtime
////////////

bool igdeNullInputModule::Init(){
	return true;
}

void igdeNullInputModule::CleanUp(){
}



// Devices
////////////

int igdeNullInputModule::GetDeviceCount(){
	return 0;
}

deInputDevice *igdeNullInputModule::GetDeviceAt( int index ){
	DETHROW( deeInvalidParam );
}

int igdeNullInputModule::IndexOfDeviceWithID( const char *id ){
	return -1;
}

int igdeNullInputModule::IndexOfButtonWithID( int device, const char *id ){
	return -1;
}

int igdeNullInputModule::IndexOfAxisWithID( int device, const char *id ){
	return -1;
}

int igdeNullInputModule::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

bool igdeNullInputModule::GetButtonPressed( int device, int button ){
	DETHROW( deeInvalidParam );
}

float igdeNullInputModule::GetAxisValue( int device, int axis ){
	DETHROW( deeInvalidParam );
}

float igdeNullInputModule::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void igdeNullInputModule::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

int igdeNullInputModule::ButtonMatchingKeyChar( int device, int character ){
	DETHROW( deeInvalidParam );
}

int igdeNullInputModule::ButtonMatchingKeyCode( int device, deInputEvent::eKeyCodes keyCode ){
	DETHROW( deeInvalidParam );
}



// Events
///////////

void igdeNullInputModule::ProcessEvents(){
}

bool igdeNullInputModule::GetEvent( deInputEvent *event ){
	return false;
}

void igdeNullInputModule::ClearEvents(){
}
