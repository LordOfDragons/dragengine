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

#include "deClassAnimatorSystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////



// Parameters
///////////////

// public static func int getParameterCount()
deClassAnimatorSystem::nfGetParameterCount::nfGetParameterCount(const sInitData &init) :
dsFunction(init.clsAmrSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassAnimatorSystem::nfGetParameterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimatorSystem *clsAmrSys = (deClassAnimatorSystem*)GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	
	rt->PushInt(module->GetParameterCount());
}

// public static func ModuleParameter getParameterInfo( int index )
deClassAnimatorSystem::nfGetParameterInfo::nfGetParameterInfo(const sInitData &init) :
dsFunction(init.clsAmrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsInteger); // index
}
void deClassAnimatorSystem::nfGetParameterInfo::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimatorSystem *clsAmrSys = (deClassAnimatorSystem*)GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	int index = rt->GetValue(0)->GetInt();
	
	if(index < 0 || index >= module->GetParameterCount()) DSTHROW(dueInvalidParam);
	
	clsAmrSys->GetDS().GetClassModuleParameter()->PushParameter(rt, module, index);
}

// public static func ModuleParameter getParameterInfo( String name )
deClassAnimatorSystem::nfGetParameterInfo2::nfGetParameterInfo2(const sInitData &init) :
dsFunction(init.clsAmrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsString); // name
}
void deClassAnimatorSystem::nfGetParameterInfo2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAnimatorSystem*)GetOwnerClass())->GetDS();
	const deAnimatorSystem &amrSys = *ds.GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule * const module = amrSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter(rt, module,
		module->IndexOfParameterNamed(rt->GetValue(0)->GetString()));
}

// public static func String getParameterValue( String name )
deClassAnimatorSystem::nfGetParameterValue::nfGetParameterValue(const sInitData &init) :
dsFunction(init.clsAmrSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // name
}
void deClassAnimatorSystem::nfGetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimatorSystem *clsAmrSys = (deClassAnimatorSystem*)GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	rt->PushString(module->GetParameterValue(rt->GetValue(0)->GetString()));
}

// public static func void setParameterValue( String name, String value )
deClassAnimatorSystem::nfSetParameterValue::nfSetParameterValue(const sInitData &init) :
dsFunction(init.clsAmrSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsString); // value
}
void deClassAnimatorSystem::nfSetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimatorSystem *clsAmrSys = (deClassAnimatorSystem*)GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	
	module->SetParameterValue(rt->GetValue(0)->GetString(), rt->GetValue(1)->GetString());
}



// public static func String sendCommand( String command )
deClassAnimatorSystem::nfSendCommand::nfSendCommand(const sInitData &init) : dsFunction(init.clsAmrSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // command
}
void deClassAnimatorSystem::nfSendCommand::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAnimatorSystem*)GetOwnerClass())->GetDS();
	const deAnimatorSystem &animatorSystem = *ds.GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule &module = *animatorSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8(rt->GetValue(0)->GetString());
	argList.ParseCommand(command);
	module.SendCommand(argList, answer);
	rt->PushString(answer.ToUTF8());
}



// Class deClassAnimatorSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassAnimatorSystem::deClassAnimatorSystem(deScriptingDragonScript &ds) :
dsClass("AnimatorSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(0);
}

deClassAnimatorSystem::~deClassAnimatorSystem(){
}



// Management
///////////////

void deClassAnimatorSystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsAmrSys = this;
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
}
