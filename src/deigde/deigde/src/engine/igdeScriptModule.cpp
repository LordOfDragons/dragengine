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

#include "igdeScriptModule.h"
#include "igdeSMCollider.h"
#include "igdeSMTouchSensor.h"
#include "igdeSMPropField.h"
#include "../gui/igdeWindowMain.h"

#include <dragengine/deEngine.h>
#include "dragengine/common/exceptions.h"
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingServer.h>



// TODO: Make these proper classes

// cModule
igdeScriptModule::cModule::cModule( deModuleSystem *system, igdeWindowMain &windowMain ) :
deInternalModule( system ),
pWindowMain( windowMain ){
	SetName( "IGDEScript" );
	SetType( deModuleSystem::emtScript );
	SetDescription( "IGDE Scripting Module" );
	SetAuthor( "Plüss Roland" );
	SetVersion( "1.0" );
	SetDirectoryName( "IGDEScript" );
}
igdeScriptModule::cModule::~cModule(){ }
void igdeScriptModule::cModule::CreateModule(){
	SetModule( new igdeScriptModule( *this, pWindowMain ) );
	if( ! GetModule() ){
		SetErrorCode( eecCreateModuleFailed );
	}
}



// Class igdeScriptModule
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeScriptModule::igdeScriptModule( deLoadableModule &loadableModule, igdeWindowMain &windowMain ) :
deBaseScriptingModule( loadableModule ),
pWindowMain( windowMain ){
}

igdeScriptModule::~igdeScriptModule(){
}



// Management
///////////////

bool igdeScriptModule::Init( const char *scriptDirectory, const char *gameObject ){
	return true;
}

void igdeScriptModule::ShutDown( ){
}

const char *igdeScriptModule::GetSharedDataDir() const{
	return NULL;
}

const char *igdeScriptModule::GetVFSSharedDataDir() const{
	return NULL;
}



deBaseScriptingCollider *igdeScriptModule::CreateCollider( deCollider *collider ){
	return new igdeSMCollider;
}

deBaseScriptingConnection *igdeScriptModule::CreateConnection( deConnection *connection ){
	return new deBaseScriptingConnection;
}

deBaseScriptingNetworkState *igdeScriptModule::CreateNetworkState( deNetworkState *state ){
	return new deBaseScriptingNetworkState;
}

deBaseScriptingServer *igdeScriptModule::CreateServer( deServer *server ){
	return new deBaseScriptingServer;
}

deBaseScriptingTouchSensor *igdeScriptModule::CreateTouchSensor( deTouchSensor *touchSensor ){
	return new igdeSMTouchSensor;
}

deBaseScriptingPropField *igdeScriptModule::CreatePropField( dePropField *propField ){
	return new igdeSMPropField;
}

deBaseScriptingParticleEmitterInstance *igdeScriptModule::CreateParticleEmitterInstance(
deParticleEmitterInstance *instance ){
	return NULL;
}

deBaseScriptingSoundLevelMeter *igdeScriptModule::CreateSoundLevelMeter( deSoundLevelMeter *meter ){
	return NULL;
}

deBaseScriptingSpeaker *igdeScriptModule::CreateSpeaker( deSpeaker *speaker ){
	return NULL;
}



// Events
///////////

bool igdeScriptModule::InitGame(){
	return true;
}

bool igdeScriptModule::ExitGame(){
	return true;
}

bool igdeScriptModule::OnFrameUpdate(){
	pWindowMain.UpdateFrame();
	return true;
}

bool igdeScriptModule::OnResizeRenderWindow(){
	return true;
}

bool igdeScriptModule::SendEvent( deInputEvent* ){
	return true;
}

void igdeScriptModule::UserRequestQuit(){
}

bool igdeScriptModule::SendVREvent( deInputEvent* ){
	return true;
}
