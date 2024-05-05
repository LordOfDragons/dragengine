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
	SetPriority( 1000 );
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
