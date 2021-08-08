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
#include "igdeNullScriptModule.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/deModuleSystem.h"
#include "dragengine/systems/modules/deBaseModule.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingCollider.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingConnection.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingServer.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingPropField.h"
#include "dragengine/common/exceptions.h"


// cModule
igdeNullScriptModule::cModule::cModule( deModuleSystem *system ) : deInternalModule( system ){
	SetName( "NullScript" );
	SetType( deModuleSystem::emtScript );
	SetDescription( "Null Scripting Module" );
	SetAuthor( "Plüss Roland" );
	SetVersion( "1.0" );
	SetDirectoryName( "NullScript" );
}
igdeNullScriptModule::cModule::~cModule(){ }
void igdeNullScriptModule::cModule::CreateModule(){
	SetModule( new igdeNullScriptModule( *this ) );
	if( ! GetModule() ) SetErrorCode( eecCreateModuleFailed );
}



// class igdeNullScriptModule
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNullScriptModule::igdeNullScriptModule( deLoadableModule &loadableModule ) :
deBaseScriptingModule( loadableModule ){
}

igdeNullScriptModule::~igdeNullScriptModule(){
}



// Runtime
////////////

bool igdeNullScriptModule::Init( const char *scriptDirectory, const char *gameObject ){
	return true;
}

void igdeNullScriptModule::ShutDown( ){
}




// Peer Management
////////////////////

deBaseScriptingCollider *igdeNullScriptModule::CreateCollider( deCollider *collider ){
	return new deBaseScriptingCollider;
}

deBaseScriptingConnection *igdeNullScriptModule::CreateConnection( deConnection *connection ){
	return new deBaseScriptingConnection;
}

deBaseScriptingNetworkState *igdeNullScriptModule::CreateNetworkState( deNetworkState *state ){
	return new deBaseScriptingNetworkState;
}

deBaseScriptingServer *igdeNullScriptModule::CreateServer( deServer *server ){
	return new deBaseScriptingServer;
}

deBaseScriptingTouchSensor *igdeNullScriptModule::CreateTouchSensor( deTouchSensor *touchSensor ){
	return new deBaseScriptingTouchSensor;
}

deBaseScriptingPropField *igdeNullScriptModule::CreatePropField( dePropField *propField ){
	return new deBaseScriptingPropField;
}

deBaseScriptingParticleEmitterInstance *igdeNullScriptModule::CreateParticleEmitterInstance(
deParticleEmitterInstance *instance ){
	return NULL;
}

deBaseScriptingSoundLevelMeter *igdeNullScriptModule::CreateSoundLevelMeter( deSoundLevelMeter *meter ){
	return NULL;
}

deBaseScriptingSpeaker *igdeNullScriptModule::CreateSpeaker( deSpeaker *speaker ){
	return NULL;
}



// Events
///////////

const char *igdeNullScriptModule::GetSharedDataDir() const{
	return NULL;
}

const char *igdeNullScriptModule::GetVFSSharedDataDir() const{
	return NULL;
}

bool igdeNullScriptModule::InitGame( ){
	return true;
}

bool igdeNullScriptModule::ExitGame( ){
	return true;
}

bool igdeNullScriptModule::OnFrameUpdate( ){
	return true;
}

bool igdeNullScriptModule::OnResizeRenderWindow( ){
	return true;
}

bool igdeNullScriptModule::SendEvent( deInputEvent* ){
	return true;
}

void igdeNullScriptModule::UserRequestQuit(){
}
