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

#include "deClassAISystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/modules/ai/deBaseAIModule.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public static func int getParameterCount()
deClassAISystem::nfGetParameterCount::nfGetParameterCount(const sInitData &init) : dsFunction(init.clsAISys,
"getParameterCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt){
}
void deClassAISystem::nfGetParameterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAISystem &aiSys = *(((deClassAISystem*)GetOwnerClass())->GetDS().GetGameEngine()->GetAISystem());
	rt->PushInt(aiSys.GetActiveModule()->GetParameterCount());
}

// public static func ModuleParameter getParameterInfo(int index)
deClassAISystem::nfGetParameterInfo::nfGetParameterInfo(const sInitData &init) : dsFunction(init.clsAISys,
"getParameterInfo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsInt); // index
}
void deClassAISystem::nfGetParameterInfo::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAISystem*)GetOwnerClass())->GetDS();
	const deAISystem &aiSys = *(ds.GetGameEngine()->GetAISystem());
	deBaseAIModule * const module = aiSys.GetActiveModule();
	
	const int index = rt->GetValue(0)->GetInt();
	if(index < 0 || index >= module->GetParameterCount()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassModuleParameter()->PushParameter(rt, module, index);
}

// public static func ModuleParameter getParameterInfo( String name )
deClassAISystem::nfGetParameterInfo2::nfGetParameterInfo2(const sInitData &init) :
dsFunction(init.clsAISys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsStr); // name
}
void deClassAISystem::nfGetParameterInfo2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAISystem*)GetOwnerClass())->GetDS();
	const deAISystem &aiSys = *ds.GetGameEngine()->GetAISystem();
	deBaseAIModule * const module = aiSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter(rt, module,
		module->IndexOfParameterNamed(rt->GetValue(0)->GetString()));
}

// public static func String getParameterValue( String name )
deClassAISystem::nfGetParameterValue::nfGetParameterValue(const sInitData &init) : dsFunction(init.clsAISys,
"getParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr){
	p_AddParameter(init.clsStr); // name
}
void deClassAISystem::nfGetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAISystem &aiSys = *(((deClassAISystem*)GetOwnerClass())->GetDS().GetGameEngine()->GetAISystem());
	const deBaseAIModule &module = *aiSys.GetActiveModule();
	
	rt->PushString(module.GetParameterValue(rt->GetValue(0)->GetString()));
}

// public static func void setParameterValue( String name, String value )
deClassAISystem::nfSetParameterValue::nfSetParameterValue(const sInitData &init) : dsFunction(init.clsAISys,
"setParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsStr); // value
}
void deClassAISystem::nfSetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAISystem &aiSys = *(((deClassAISystem*)GetOwnerClass())->GetDS().GetGameEngine()->GetAISystem());
	deBaseAIModule &module = *aiSys.GetActiveModule();
	
	module.SetParameterValue(rt->GetValue(0)->GetString(), rt->GetValue(1)->GetString());
}



// public static func String sendCommand( String command )
deClassAISystem::nfSendCommand::nfSendCommand(const sInitData &init) : dsFunction(init.clsAISys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr){
	p_AddParameter(init.clsStr); // command
}
void deClassAISystem::nfSendCommand::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAISystem*)GetOwnerClass())->GetDS();
	const deAISystem &aiSys = *ds.GetGameEngine()->GetAISystem();
	deBaseAIModule &module = *aiSys.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8(rt->GetValue(0)->GetString());
	argList.ParseCommand(command);
	module.SendCommand(argList, answer);
	rt->PushString(answer.ToUTF8().GetString());
}



// Class deClassAISystem
//////////////////////////

// Constructor, Destructor
////////////////////////////

deClassAISystem::deClassAISystem(deScriptingDragonScript &ds) :
dsClass("AISystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(0);
}

deClassAISystem::~deClassAISystem(){
}



// Management
///////////////

void deClassAISystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsAISys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	
	// add functions
	AddFunction(new nfGetParameterCount(init));
	AddFunction(new nfGetParameterInfo(init));
	AddFunction(new nfGetParameterInfo2(init));
	AddFunction(new nfGetParameterValue(init));
	AddFunction(new nfSetParameterValue(init));
	
	AddFunction(new nfSendCommand(init));
}
