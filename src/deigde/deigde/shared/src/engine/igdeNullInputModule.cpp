/* 
 * Drag[en]gine IGDE
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
