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
igdeNullScriptModule::cModule::cModule(deModuleSystem *system) : deInternalModule(system){
	SetName("NullScript");
	SetType(deModuleSystem::emtScript);
	SetDescription("Null Scripting Module");
	SetAuthor("Plüss Roland");
	SetVersion("1.0");
	SetDirectoryName("NullScript");
	SetDefaultLoggingName();
}
igdeNullScriptModule::cModule::~cModule(){}
void igdeNullScriptModule::cModule::CreateModule(){
	SetModule(new igdeNullScriptModule(*this));
	if(!GetModule()) SetErrorCode(eecCreateModuleFailed);
}



// class igdeNullScriptModule
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNullScriptModule::igdeNullScriptModule(deLoadableModule &loadableModule) :
deBaseScriptingModule(loadableModule){
}

igdeNullScriptModule::~igdeNullScriptModule(){
}



// Runtime
////////////

bool igdeNullScriptModule::Init(const char *scriptDirectory, const char *gameObject){
	return true;
}

void igdeNullScriptModule::ShutDown(){
}




// Peer Management
////////////////////

deBaseScriptingCollider *igdeNullScriptModule::CreateCollider(deCollider *collider){
	return new deBaseScriptingCollider;
}

deBaseScriptingConnection *igdeNullScriptModule::CreateConnection(deConnection *connection){
	return new deBaseScriptingConnection;
}

deBaseScriptingNetworkState *igdeNullScriptModule::CreateNetworkState(deNetworkState *state){
	return new deBaseScriptingNetworkState;
}

deBaseScriptingServer *igdeNullScriptModule::CreateServer(deServer *server){
	return new deBaseScriptingServer;
}

deBaseScriptingTouchSensor *igdeNullScriptModule::CreateTouchSensor(deTouchSensor *touchSensor){
	return new deBaseScriptingTouchSensor;
}

deBaseScriptingPropField *igdeNullScriptModule::CreatePropField(dePropField *propField){
	return new deBaseScriptingPropField;
}

deBaseScriptingParticleEmitterInstance *igdeNullScriptModule::CreateParticleEmitterInstance(
deParticleEmitterInstance *instance){
	return NULL;
}

deBaseScriptingSoundLevelMeter *igdeNullScriptModule::CreateSoundLevelMeter(deSoundLevelMeter *meter){
	return NULL;
}

deBaseScriptingSpeaker *igdeNullScriptModule::CreateSpeaker(deSpeaker *speaker){
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

bool igdeNullScriptModule::InitGame(){
	return true;
}

bool igdeNullScriptModule::ExitGame(){
	return true;
}

bool igdeNullScriptModule::OnFrameUpdate(){
	return true;
}

bool igdeNullScriptModule::OnResizeRenderWindow(){
	return true;
}

bool igdeNullScriptModule::SendEvent(deInputEvent*){
	return true;
}

void igdeNullScriptModule::UserRequestQuit(){
}
