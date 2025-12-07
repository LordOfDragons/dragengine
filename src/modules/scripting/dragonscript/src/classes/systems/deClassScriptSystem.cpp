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
#include <libdscript/libdscript.h>

#include "deClassScriptSystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////



// Parameters
///////////////

// public static func int getParameterCount()
deClassScriptSystem::nfGetParameterCount::nfGetParameterCount(const sInitData &init) :
dsFunction(init.clsScrSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassScriptSystem::nfGetParameterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassScriptSystem *clsScrSys = (deClassScriptSystem*)GetOwnerClass();
	deScriptingSystem *audSys = clsScrSys->GetDS().GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule *module = audSys->GetActiveModule();
	
	rt->PushInt(module->GetParameterCount());
}

// public static func ModuleParameter getParameterInfo( int index )
deClassScriptSystem::nfGetParameterInfo::nfGetParameterInfo(const sInitData &init) :
dsFunction(init.clsScrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsInteger); // index
}
void deClassScriptSystem::nfGetParameterInfo::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassScriptSystem *clsScrSys = (deClassScriptSystem*)GetOwnerClass();
	deScriptingSystem *audSys = clsScrSys->GetDS().GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule *module = audSys->GetActiveModule();
	int index = rt->GetValue(0)->GetInt();
	
	if(index < 0 || index >= module->GetParameterCount()) DSTHROW(dueInvalidParam);
	
	clsScrSys->GetDS().GetClassModuleParameter()->PushParameter(rt, module, index);
}

// public static func ModuleParameter getParameterInfo( String name )
deClassScriptSystem::nfGetParameterInfo2::nfGetParameterInfo2(const sInitData &init) :
dsFunction(init.clsScrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsString); // name
}
void deClassScriptSystem::nfGetParameterInfo2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassScriptSystem*)GetOwnerClass())->GetDS();
	const deScriptingSystem &audSys = *ds.GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule * const module = audSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter(rt, module,
		module->IndexOfParameterNamed(rt->GetValue(0)->GetString()));
}

// public static func String getParameterValue( String name )
deClassScriptSystem::nfGetParameterValue::nfGetParameterValue(const sInitData &init) :
dsFunction(init.clsScrSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // name
}
void deClassScriptSystem::nfGetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassScriptSystem *clsScrSys = (deClassScriptSystem*)GetOwnerClass();
	deScriptingSystem *audSys = clsScrSys->GetDS().GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule *module = audSys->GetActiveModule();
	rt->PushString(module->GetParameterValue(rt->GetValue(0)->GetString()));
}

// public static func void setParameterValue( String name, String value )
deClassScriptSystem::nfSetParameterValue::nfSetParameterValue(const sInitData &init) :
dsFunction(init.clsScrSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsString); // value
}
void deClassScriptSystem::nfSetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassScriptSystem *clsScrSys = (deClassScriptSystem*)GetOwnerClass();
	deScriptingSystem *audSys = clsScrSys->GetDS().GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule *module = audSys->GetActiveModule();
	
	module->SetParameterValue(rt->GetValue(0)->GetString(), rt->GetValue(1)->GetString());
}



// public static func String sendCommand( String command )
deClassScriptSystem::nfSendCommand::nfSendCommand(const sInitData &init) : dsFunction(init.clsScrSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // command
}
void deClassScriptSystem::nfSendCommand::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassScriptSystem*)GetOwnerClass())->GetDS();
	const deScriptingSystem &scriptSystem = *ds.GetGameEngine()->GetScriptingSystem();
	deBaseScriptingModule &module = *scriptSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8(rt->GetValue(0)->GetString());
	argList.ParseCommand(command);
	module.SendCommand(argList, answer);
	rt->PushString(answer.ToUTF8());
}

// public static func int getFPSRate()
deClassScriptSystem::nfGetFPSRate::nfGetFPSRate(const sInitData &init) :
dsFunction(init.clsScrSys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassScriptSystem::nfGetFPSRate::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = ((deClassScriptSystem*)GetOwnerClass())->GetDS();
	rt->PushInt(ds.GetGameEngine()->GetFPSRate());
}



// Class deClassScriptSystem
//////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassScriptSystem::deClassScriptSystem(deScriptingDragonScript &ds) :
dsClass("ScriptSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(0);
}

deClassScriptSystem::~deClassScriptSystem(){
}



// Management
///////////////

void deClassScriptSystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsScrSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	
	// add functions
	AddFunction(new nfGetParameterCount(init));
	AddFunction(new nfGetParameterInfo(init));
	AddFunction(new nfGetParameterInfo2(init));
	AddFunction(new nfGetParameterValue(init));
	AddFunction(new nfSetParameterValue(init));
	
	AddFunction(new nfSendCommand(init));
	AddFunction(new nfGetFPSRate(init));
}
