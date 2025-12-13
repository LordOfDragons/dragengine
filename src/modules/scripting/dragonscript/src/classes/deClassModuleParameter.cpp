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

// includes
#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassModuleParameter.h"
#include "../deScriptingDragonScript.h"
#include "../deClassPathes.h"
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sMPNatDat{
	deModuleParameter param;
};

// native functions
/////////////////////

// public destructor Destructor()
deClassModuleParameter::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsMP, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassModuleParameter::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sMPNatDat*>(p_GetNativeData(myself))->~sMPNatDat();
}



// public func String getName()
deClassModuleParameter::nfGetName::nfGetName(const sInitData &init) :
dsFunction(init.clsMP, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModuleParameter::nfGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushString(param.GetName());
}

// public func String getDescription()
deClassModuleParameter::nfGetDescription::nfGetDescription(const sInitData &init) :
dsFunction(init.clsMP, "getDescription", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModuleParameter::nfGetDescription::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushString(param.GetDescription());
}

// public func ModuleParameterType getType()
deClassModuleParameter::nfGetType::nfGetType(const sInitData &init) :
dsFunction(init.clsMP, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModuleParameterType){
}
void deClassModuleParameter::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushValue(static_cast<deClassModuleParameter*>(GetOwnerClass())->GetClassModuleParameterType()
		->GetVariable(param.GetType())->GetStaticValue());
}

// public func float getMinimumValue()
deClassModuleParameter::nfGetMinimumValue::nfGetMinimumValue(const sInitData &init) :
dsFunction(init.clsMP, "getMinimumValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassModuleParameter::nfGetMinimumValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushFloat(param.GetMinimumValue());
}

// public func float getMaximumValue()
deClassModuleParameter::nfGetMaximumValue::nfGetMaximumValue(const sInitData &init) :
dsFunction(init.clsMP, "getMaximumValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassModuleParameter::nfGetMaximumValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushFloat(param.GetMaximumValue());
}

// public func float getValueStepSize()
deClassModuleParameter::nfGetValueStepSize::nfGetValueStepSize(const sInitData &init) :
dsFunction(init.clsMP, "getValueStepSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassModuleParameter::nfGetValueStepSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushFloat(param.GetValueStepSize());
}

// public func int getSelectionEntryCount()
deClassModuleParameter::nfGetSelectionEntryCount::nfGetSelectionEntryCount(const sInitData &init) :
dsFunction(init.clsMP, "getSelectionEntryCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModuleParameter::nfGetSelectionEntryCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushInt(param.GetSelectionEntryCount());
}

// public func String getSelectionEntryValueAt( int index )
deClassModuleParameter::nfGetSelectionEntryValueAt::nfGetSelectionEntryValueAt(const sInitData &init) :
dsFunction(init.clsMP, "getSelectionEntryValueAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryValueAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	int index = rt->GetValue(0)->GetInt();
	rt->PushString(param.GetSelectionEntryAt(index).value);
}

// public func String getSelectionEntryDisplayNameAt( int index )
deClassModuleParameter::nfGetSelectionEntryDisplayNameAt::nfGetSelectionEntryDisplayNameAt(const sInitData &init) :
dsFunction(init.clsMP, "getSelectionEntryDisplayNameAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryDisplayNameAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	int index = rt->GetValue(0)->GetInt();
	rt->PushString(param.GetSelectionEntryAt(index).displayName);
}

// public func String getSelectionEntryDescriptionAt( int index )
deClassModuleParameter::nfGetSelectionEntryDescriptionAt::nfGetSelectionEntryDescriptionAt(const sInitData &init) :
dsFunction(init.clsMP, "getSelectionEntryDescriptionAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassModuleParameter::nfGetSelectionEntryDescriptionAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	int index = rt->GetValue(0)->GetInt();
	rt->PushString(param.GetSelectionEntryAt(index).description);
}

// public func ModuleParameterCategory getCategory()
deClassModuleParameter::nfGetCategory::nfGetCategory(const sInitData &init) :
dsFunction(init.clsMP, "getCategory", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModuleParameterCategory){
}
void deClassModuleParameter::nfGetCategory::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushValue(static_cast<deClassModuleParameter*>(GetOwnerClass())->GetClassModuleParameterCategory()
		->GetVariable(param.GetCategory())->GetStaticValue());
}

// public func String getDisplayName()
deClassModuleParameter::nfGetDisplayName::nfGetDisplayName(const sInitData &init) :
dsFunction(init.clsMP, "getDisplayName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModuleParameter::nfGetDisplayName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushString(param.GetDisplayName());
}

// public func String getDefaultValue()
deClassModuleParameter::nfGetDefaultValue::nfGetDefaultValue(const sInitData &init) :
dsFunction(init.clsMP, "getDefaultValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModuleParameter::nfGetDefaultValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModuleParameter &param = static_cast<sMPNatDat*>(p_GetNativeData(myself))->param;
	rt->PushString(param.GetDefaultValue());
}



// Class deClassModuleParameter
/////////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassModuleParameter::deClassModuleParameter(deEngine *gameEngine, deScriptingDragonScript &ds) :
dsClass("ModuleParameter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pGameEngine(nullptr),
pDS(ds),
pClsModuleParameterType(nullptr),
pClsModuleParameterCategory(nullptr)
{
	if(!gameEngine){
		DSTHROW(dueInvalidParam);
	}
	
	pGameEngine = gameEngine;
	
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMPNatDat));
}

deClassModuleParameter::~deClassModuleParameter(){
}



// Management
///////////////

void deClassModuleParameter::CreateClassMembers(dsEngine *engine){
	// store classes
	pClsModuleParameterType = engine->GetClass("Dragengine.ModuleParameterType");
	pClsModuleParameterCategory = engine->GetClass("Dragengine.ModuleParameterCategory");
	
	sInitData init;
	init.clsMP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsModuleParameterType = pClsModuleParameterType;
	init.clsModuleParameterCategory = pClsModuleParameterCategory;
	
	// add functions
	AddFunction(new nfDestructor(init));
	AddFunction(new nfGetName(init));
	AddFunction(new nfGetDescription(init));
	AddFunction(new nfGetType(init));
	AddFunction(new nfGetMinimumValue(init));
	AddFunction(new nfGetMaximumValue(init));
	AddFunction(new nfGetValueStepSize(init));
	AddFunction(new nfGetSelectionEntryCount(init));
	AddFunction(new nfGetSelectionEntryValueAt(init));
	AddFunction(new nfGetSelectionEntryDisplayNameAt(init));
	AddFunction(new nfGetSelectionEntryDescriptionAt(init));
	AddFunction(new nfGetCategory(init));
	AddFunction(new nfGetDisplayName(init));
	AddFunction(new nfGetDefaultValue(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

void deClassModuleParameter::PushParameter(dsRunTime *rt, const deBaseModule *module, int index){
	if(!rt || !module){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sMPNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sMPNatDat;
	
	try{
		module->GetParameterInfo(index, nd->param);
		
	}catch(const duException &){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
