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

#include <new>

#include <stdint.h>

#include "deClassService.h"
#include "deClassServiceObject.h"
#include "../animation/deClassAnimation.h"
#include "../file/deClassMemoryFile.h"
#include "../graphics/deClassImage.h"
#include "../graphics/deClassOcclusionMesh.h"
#include "../particle/deClassParticleEmitter.h"
#include "../sound/deClassSound.h"
#include "../translation/deClassLanguagePack.h"
#include "../world/deClassModel.h"
#include "../world/deClassRig.h"
#include "../world/deClassSkin.h"
#include "../video/deClassVideo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/service/deService.h>
#include <dragengine/resources/service/deServiceObject.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/video/deVideo.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sServiceObjectNatDat{
	deServiceObject::Ref object;
};



// Native functions
/////////////////////

// func new()
deClassServiceObject::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsServiceObject, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}

void deClassServiceObject::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sServiceObjectNatDat * const nd = new (p_GetNativeData(myself)) sServiceObjectNatDat;
	nd->object = deServiceObject::Ref::New();
}


// static func ServiceObject newBool(bool value)
deClassServiceObject::nfNewBool::nfNewBool(const sInitData &init) :
dsFunction(init.clsServiceObject, "newBool", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsBool); // value
}

void deClassServiceObject::nfNewBool::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	const bool value = rt->GetValue(0)->GetBool();
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewBool(value));
}


// static func ServiceObject newInt(int value)
deClassServiceObject::nfNewInteger::nfNewInteger(const sInitData &init) :
dsFunction(init.clsServiceObject, "newInt", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsInteger); // value
}

void deClassServiceObject::nfNewInteger::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	const int value = rt->GetValue(0)->GetInt();
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewInt(value));
}


// static func ServiceObject newFloat(float value)
deClassServiceObject::nfNewFloat::nfNewFloat(const sInitData &init) :
dsFunction(init.clsServiceObject, "newFloat", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsFloat); // value
}

void deClassServiceObject::nfNewFloat::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	const float value = rt->GetValue(0)->GetFloat();
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewFloat(value));
}


// static func ServiceObject newString(String value)
deClassServiceObject::nfNewString::nfNewString(const sInitData &init) :
dsFunction(init.clsServiceObject, "nfNewString", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsString); // value
}

void deClassServiceObject::nfNewString::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	const char * const value = rt->GetValue(0)->GetString();
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewString(value));
}


// static func ServiceObject newResource(Object value)
deClassServiceObject::nfNewResource::nfNewResource(const sInitData &init) :
dsFunction(init.clsServiceObject, "newResource", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsObject); // value
}

void deClassServiceObject::nfNewResource::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	deResource * const value = clsServiceObject.GetResource(*rt->GetValue(0));
	if(!value){
		DSTHROW(dueNullPointer);
	}
	
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewResource(deResource::Ref(value)));
}


// static func ServiceObject newData(MemoryFile value)
deClassServiceObject::nfNewData::nfNewData(const sInitData &init) :
dsFunction(init.clsServiceObject, "newData", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsMemoryFile); // value
}

void deClassServiceObject::nfNewData::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	deScriptingDragonScript &ds = clsServiceObject.GetDS();
	
	decMemoryFile * const value = ds.GetClassMemoryFile()->
		GetMemoryFile(rt->GetValue(0)->GetRealObject());
	if(!value){
		DSTHROW(dueNullPointer);
	}
	
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewData(decMemoryFile::Ref(value)));
}


// static func ServiceObject newList()
deClassServiceObject::nfNewList::nfNewList(const sInitData &init) :
dsFunction(init.clsServiceObject, "newList", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
}

void deClassServiceObject::nfNewList::RunFunction(dsRunTime *rt, dsValue*){
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	clsServiceObject.PushServiceObject(rt, deServiceObject::NewList());
}


// new(ServiceObject copy, bool deep)
deClassServiceObject::nfNewCopy::nfNewCopy(const sInitData &init) :
dsFunction(init.clsServiceObject, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsServiceObject); // copy
	p_AddParameter(init.clsBool); // deep
}

void deClassServiceObject::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sServiceObjectNatDat * const nd = new (p_GetNativeData(myself)) sServiceObjectNatDat;
	
	const deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	const deServiceObject * const copy = clsServiceObject.GetServiceObject(rt->GetValue(0)->GetRealObject());
	const bool deep = rt->GetValue(1)->GetBool();
	if(!copy){
		DSTHROW(dueNullPointer);
	}
	
	nd->object = deServiceObject::Ref::New(*copy, deep);
}


// public func destructor()
deClassServiceObject::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsServiceObject, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}

void deClassServiceObject::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself))->~sServiceObjectNatDat();
}



// Management
///////////////

// func ServiceObjectType getValueType()
deClassServiceObject::nfGetValueType::nfGetValueType(const sInitData &init) :
dsFunction(init.clsServiceObject, "getValueType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObjectType){
}

void deClassServiceObject::nfGetValueType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushValue((static_cast<deClassServiceObject*>(GetOwnerClass()))->GetClassServiceObjectType()
		->GetVariable(nd.object->GetValueType())->GetStaticValue());
}



// func bool isBoolean()
deClassServiceObject::nfIsBoolean::nfIsBoolean(const sInitData &init) :
dsFunction(init.clsServiceObject, "isBoolean", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsBoolean::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsBoolean());
}


// func bool getBoolean()
deClassServiceObject::nfGetBoolean::nfGetBoolean(const sInitData &init) :
dsFunction(init.clsServiceObject, "getBoolean", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfGetBoolean::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->GetBoolean());
}



// func bool isInteger()
deClassServiceObject::nfIsInteger::nfIsInteger(const sInitData &init) :
dsFunction(init.clsServiceObject, "isInteger", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsInteger::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsInteger());
}


// func int getInteger()
deClassServiceObject::nfGetInteger::nfGetInteger(const sInitData &init) :
dsFunction(init.clsServiceObject, "getInteger", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassServiceObject::nfGetInteger::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt(nd.object->GetInteger());
}



// func bool isFloat()
deClassServiceObject::nfIsFloat::nfIsFloat(const sInitData &init) :
dsFunction(init.clsServiceObject, "isFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsFloat());
}


// func float getFloat()
deClassServiceObject::nfGetFloat::nfGetFloat(const sInitData &init) :
dsFunction(init.clsServiceObject, "getFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}

void deClassServiceObject::nfGetFloat::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.object->GetFloat());
}



// func bool isString()
deClassServiceObject::nfIsString::nfIsString(const sInitData &init) :
dsFunction(init.clsServiceObject, "isString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsString::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsString());
}


// func String getString()
deClassServiceObject::nfGetString::nfGetString(const sInitData &init) :
dsFunction(init.clsServiceObject, "getString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}

void deClassServiceObject::nfGetString::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushString(nd.object->GetString());
}



// func bool isResource()
deClassServiceObject::nfIsResource::nfIsResource(const sInitData &init) :
dsFunction(init.clsServiceObject, "isResource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsResource::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsResource());
}


// func Object getResource()
deClassServiceObject::nfGetResource::nfGetResource(const sInitData &init) :
dsFunction(init.clsServiceObject, "getResource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsObject){
}

void deClassServiceObject::nfGetResource::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	clsServiceObject.PushResource(*rt, nd.object->GetResource());
}



// func bool isData()
deClassServiceObject::nfIsData::nfIsData(const sInitData &init) :
dsFunction(init.clsServiceObject, "isData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsData::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsData());
}


// func MemoryFile getData()
deClassServiceObject::nfGetData::nfGetData(const sInitData &init) :
dsFunction(init.clsServiceObject, "getData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMemoryFile){
}

void deClassServiceObject::nfGetData::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = (static_cast<deClassServiceObject*>(GetOwnerClass()))->GetDS();
	ds.GetClassMemoryFile()->PushMemoryFile(rt, nd.object->GetData());
}



// func bool isList()
deClassServiceObject::nfIsList::nfIsList(const sInitData &init) :
dsFunction(init.clsServiceObject, "isList", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsList::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsList());
}


// func bool isDictionary()
deClassServiceObject::nfIsDictionary::nfIsDictionary(const sInitData &init) :
dsFunction(init.clsServiceObject, "isDictionary", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}

void deClassServiceObject::nfIsDictionary::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.object->IsDictionary());
}


// func int getChildCount()
deClassServiceObject::nfGetChildCount::nfGetChildCount(const sInitData &init) :
dsFunction(init.clsServiceObject, "getChildCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassServiceObject::nfGetChildCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt(nd.object->GetChildCount());
}


// func Array getChildrenKeys()
deClassServiceObject::nfGetChildrenKeys::nfGetChildrenKeys(const sInitData &init) :
dsFunction(init.clsServiceObject, "getChildrenKeys", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsArray){
}

void deClassServiceObject::nfGetChildrenKeys::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = (static_cast<deClassServiceObject*>(GetOwnerClass()))->GetDS();
	const decStringList keys(nd.object->GetChildrenKeys());
	const dsEngine &sengine = *ds.GetScriptEngine();
	const int count = keys.GetCount();
	
	dsValue * const valueKeys = rt->CreateValue(sengine.GetClassArray());
	
	try{
		rt->CreateObject(valueKeys, sengine.GetClassArray(), 0);
		
		int i;
		for(i=0; i<count; i++){
			rt->PushString(keys.GetAt(i));
			rt->RunFunction(valueKeys, "add", 1);
		}
		
		rt->PushValue(valueKeys);
		rt->FreeValue(valueKeys);
		
	}catch(...){
		if(valueKeys){
			rt->FreeValue(valueKeys);
		}
		throw;
	}
}


// func ServiceObject getChildAt(int index)
deClassServiceObject::nfGetChildAt::nfGetChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "getChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsInteger); // index
}

void deClassServiceObject::nfGetChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	const int index = rt->GetValue(0)->GetInt();
	
	clsServiceObject.PushServiceObject(rt, nd.object->GetChildAt(index));
}


// func ServiceObject getChildAt(String key)
deClassServiceObject::nfGetChildAt2::nfGetChildAt2(const sInitData &init) :
dsFunction(init.clsServiceObject, "getChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject){
	p_AddParameter(init.clsString); // key
}

void deClassServiceObject::nfGetChildAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	const char * const key = rt->GetValue(0)->GetString();
	
	clsServiceObject.PushServiceObject(rt, nd.object->GetChildAt(key));
}


// func void addChild(ServiceObject value)
deClassServiceObject::nfAddChild::nfAddChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsServiceObject); // value
}

void deClassServiceObject::nfAddChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	nd.object->AddChild(deServiceObject::Ref(clsServiceObject.GetServiceObject(
		rt->GetValue(0)->GetRealObject())));
}


// func void setChildAt(String key, ServiceObject value)
deClassServiceObject::nfSetChildAt::nfSetChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsServiceObject); // value
}

void deClassServiceObject::nfSetChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	const char * const key = rt->GetValue(0)->GetString();
	deServiceObject * const value = clsServiceObject.GetServiceObject(rt->GetValue(1)->GetRealObject());
	
	nd.object->SetChildAt(key, deServiceObject::Ref(value));
}


// func void addBoolChild(bool value)
deClassServiceObject::nfAddBoolChild::nfAddBoolChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addBoolChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // value
}

void deClassServiceObject::nfAddBoolChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->AddBoolChild(rt->GetValue(0)->GetBool());
}


// func void setBoolChildAt(String key, bool value)
deClassServiceObject::nfSetBoolChildAt::nfSetBoolChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setBoolChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsBool); // value
}

void deClassServiceObject::nfSetBoolChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	const bool value = rt->GetValue(1)->GetBool();
	
	nd.object->SetBoolChildAt(key, value);
}


// func void addIntChild(int value)
deClassServiceObject::nfAddIntChild::nfAddIntChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addIntChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // value
}

void deClassServiceObject::nfAddIntChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->AddIntChild(rt->GetValue(0)->GetInt());
}


// func void setIntChildAt(String key, int value)
deClassServiceObject::nfSetIntChildAt::nfSetIntChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setIntChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsInteger); // value
}

void deClassServiceObject::nfSetIntChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	const int value = rt->GetValue(1)->GetInt();
	
	nd.object->SetIntChildAt(key, value);
}


// func void addFloatChild(float value)
deClassServiceObject::nfAddFloatChild::nfAddFloatChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addFloatChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // value
}

void deClassServiceObject::nfAddFloatChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->AddFloatChild(rt->GetValue(0)->GetFloat());
}


// func void setFloatChildAt(String key, float value)
deClassServiceObject::nfSetFloatChildAt::nfSetFloatChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setFloatChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsFloat); // value
}

void deClassServiceObject::nfSetFloatChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	const float value = rt->GetValue(1)->GetFloat();
	
	nd.object->SetFloatChildAt(key, value);
}


// func void addStringChild(String value)
deClassServiceObject::nfAddStringChild::nfAddStringChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addStringChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // value
}

void deClassServiceObject::nfAddStringChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->AddStringChild(rt->GetValue(0)->GetString());
}


// func void setStringChildAt(String key, String value)
deClassServiceObject::nfSetStringChildAt::nfSetStringChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setStringChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsString); // value
}

void deClassServiceObject::nfSetStringChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	const char * const value = rt->GetValue(1)->GetString();
	
	nd.object->SetStringChildAt(key, value);
}


// func void addResourceChild(Object value)
deClassServiceObject::nfAddResourceChild::nfAddResourceChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addResourceChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObject); // value
}

void deClassServiceObject::nfAddResourceChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	
	nd.object->AddResourceChild(clsServiceObject.GetResource(*rt->GetValue(0)));
}


// func void setResourceChildAt(String key, Object value)
deClassServiceObject::nfSetResourceChildAt::nfSetResourceChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setResourceChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsObject); // value
}

void deClassServiceObject::nfSetResourceChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	
	const deClassServiceObject &clsServiceObject = *(static_cast<deClassServiceObject*>(GetOwnerClass()));
	deResource * const value = clsServiceObject.GetResource(*rt->GetValue(1));
	
	nd.object->SetResourceChildAt(key, value);
}


// func void addDataChild(MemoryFile value)
deClassServiceObject::nfAddDataChild::nfAddDataChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "addDataChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsMemoryFile); // value
}

void deClassServiceObject::nfAddDataChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = (static_cast<deClassServiceObject*>(GetOwnerClass()))->GetDS();
	
	nd.object->AddDataChild(decMemoryFile::Ref(ds.GetClassMemoryFile()->GetMemoryFile(
		rt->GetValue(0)->GetRealObject())));
}


// func void setDataChildAt(String key, MemoryFile value)
deClassServiceObject::nfSetDataChildAt::nfSetDataChildAt(const sInitData &init) :
dsFunction(init.clsServiceObject, "setDataChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
	p_AddParameter(init.clsMemoryFile); // value
}

void deClassServiceObject::nfSetDataChildAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	const char * const key = rt->GetValue(0)->GetString();
	
	const deScriptingDragonScript &ds = (static_cast<deClassServiceObject*>(GetOwnerClass()))->GetDS();
	decMemoryFile * const value = ds.GetClassMemoryFile()->GetMemoryFile(
		rt->GetValue(1)->GetRealObject());
	
	nd.object->SetDataChildAt(key, decMemoryFile::Ref(value));
}


// func void removeChild(int index)
deClassServiceObject::nfRemoveChild::nfRemoveChild(const sInitData &init) :
dsFunction(init.clsServiceObject, "removeChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // index
}

void deClassServiceObject::nfRemoveChild::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->RemoveChild(rt->GetValue(0)->GetInt());
}


// func void removeChild(String key)
deClassServiceObject::nfRemoveChild2::nfRemoveChild2(const sInitData &init) :
dsFunction(init.clsServiceObject, "removeChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // key
}

void deClassServiceObject::nfRemoveChild2::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->RemoveChild(rt->GetValue(0)->GetString());
}


// func void removeAllChildren()
deClassServiceObject::nfRemoveAllChildren::nfRemoveAllChildren(const sInitData &init) :
dsFunction(init.clsServiceObject, "removeAllChildren", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}

void deClassServiceObject::nfRemoveAllChildren::RunFunction(dsRunTime*, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	nd.object->RemoveAllChildren();
}



// func int hashCode()
deClassServiceObject::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsServiceObject, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassServiceObject::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushInt((int)(intptr_t)nd.object);
}


// func bool equals( Object obj )
deClassServiceObject::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsServiceObject, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}

void deClassServiceObject::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sServiceObjectNatDat &nd = *static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself));
	if(!nd.object){
		DSTHROW(dueNullPointer);
	}
	
	deClassServiceObject * const clsServiceObject = static_cast<deClassServiceObject*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsServiceObject)){
		rt->PushBool(false);
		
	}else{
		const deServiceObject * const other = static_cast<sServiceObjectNatDat*>(p_GetNativeData(obj))->object;
		rt->PushBool(nd.object == other);
	}
}


// Class deClassServiceObject
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassServiceObject::deClassServiceObject(deScriptingDragonScript &ds) :
dsClass("ServiceObject", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sServiceObjectNatDat));
}

deClassServiceObject::~deClassServiceObject(){
}



// Management
///////////////

void deClassServiceObject::CreateClassMembers(dsEngine *engine){
	pClsServiceObjectType = engine->GetClass("Dragengine.ServiceObjectType");
	
	sInitData init;
	init.clsServiceObject = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsArray = engine->GetClassArray();
	init.clsMemoryFile = pDS.GetClassMemoryFile();
	init.clsServiceObjectType = pClsServiceObjectType;
	
	AddFunction(new nfNew(init));
	AddFunction(new nfNewBool(init));
	AddFunction(new nfNewInteger(init));
	AddFunction(new nfNewFloat(init));
	AddFunction(new nfNewString(init));
	AddFunction(new nfNewResource(init));
	AddFunction(new nfNewData(init));
	AddFunction(new nfNewList(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetValueType(init));
	
	AddFunction(new nfIsBoolean(init));
	AddFunction(new nfGetBoolean(init));
	
	AddFunction(new nfIsInteger(init));
	AddFunction(new nfGetInteger(init));
	
	AddFunction(new nfIsFloat(init));
	AddFunction(new nfGetFloat(init));
	
	AddFunction(new nfIsString(init));
	AddFunction(new nfGetString(init));
	
	AddFunction(new nfIsResource(init));
	AddFunction(new nfGetResource(init));
	
	AddFunction(new nfIsData(init));
	AddFunction(new nfGetData(init));
	
	AddFunction(new nfIsList(init));
	AddFunction(new nfIsDictionary(init));
	AddFunction(new nfGetChildCount(init));
	AddFunction(new nfGetChildrenKeys(init));
	AddFunction(new nfGetChildAt(init));
	AddFunction(new nfGetChildAt2(init));
	AddFunction(new nfAddChild(init));
	AddFunction(new nfSetChildAt(init));
	AddFunction(new nfAddBoolChild(init));
	AddFunction(new nfSetBoolChildAt(init));
	AddFunction(new nfAddIntChild(init));
	AddFunction(new nfSetIntChildAt(init));
	AddFunction(new nfAddFloatChild(init));
	AddFunction(new nfSetFloatChildAt(init));
	AddFunction(new nfAddStringChild(init));
	AddFunction(new nfSetStringChildAt(init));
	AddFunction(new nfAddResourceChild(init));
	AddFunction(new nfSetResourceChildAt(init));
	AddFunction(new nfAddDataChild(init));
	AddFunction(new nfSetDataChildAt(init));
	AddFunction(new nfRemoveChild(init));
	AddFunction(new nfRemoveChild2(init));
	AddFunction(new nfRemoveAllChildren(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	
	CalcMemberOffsets();
}

deServiceObject *deClassServiceObject::GetServiceObject(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sServiceObjectNatDat*>(p_GetNativeData(myself->GetBuffer()))->object;
}

void deClassServiceObject::PushServiceObject(dsRunTime *rt, const deServiceObject::Ref &object){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!object){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sServiceObjectNatDat)->object = object;
}

deResource *deClassServiceObject::GetResource(dsValue &myself) const{
	if(myself.GetType()->GetPrimitiveType() != DSPT_OBJECT){
		return nullptr;
	}
	
	dsRealObject * const realObject = myself.GetRealObject();
	if(!realObject){
		return nullptr;
	}
	
	const dsClass * const sclass = realObject->GetType();
	
	if(sclass == pDS.GetClassAnimation()){
		return pDS.GetClassAnimation()->GetAnimation(realObject);
		
	}else if(sclass == pDS.GetClassImage()){
		return pDS.GetClassImage()->GetImage(realObject);
		
	}else if(sclass == pDS.GetClassLanguagePack()){
		return pDS.GetClassLanguagePack()->GetLanguagePack(realObject);
		
	}else if(sclass == pDS.GetClassModel()){
		return pDS.GetClassModel()->GetModel(realObject);
		
	}else if(sclass == pDS.GetClassOcclusionMesh()){
		return pDS.GetClassOcclusionMesh()->GetOcclusionMesh(realObject);
		
	}else if(sclass == pDS.GetClassParticleEmitter()){
		return pDS.GetClassParticleEmitter()->GetParticleEmitter(realObject);
		
	}else if(sclass == pDS.GetClassRig()){
		return pDS.GetClassRig()->GetRig(realObject);
		
	}else if(sclass == pDS.GetClassSkin()){
		return pDS.GetClassSkin()->GetSkin(realObject);
		
	}else if(sclass == pDS.GetClassSound()){
		return pDS.GetClassSound()->GetSound(realObject);
		
	}else if(sclass == pDS.GetClassVideo()){
		return pDS.GetClassVideo()->GetVideo(realObject);
		
	}else if(sclass == pDS.GetClassService()){
		return pDS.GetClassService()->GetService(realObject);
	}
	
	return nullptr;
}

void deClassServiceObject::PushResource(dsRunTime &rt, deResource *resource){
	if(!resource){
		rt.PushObject(NULL, pDS.GetScriptEngine()->GetClassObject());
		return;
	}
	
	switch(resource->GetResourceManager()->GetResourceType()){
	case deResourceManager::ertAnimation:
		pDS.GetClassAnimation()->PushAnimation(&rt, static_cast<deAnimation*>(resource));
		break;
		
	case deResourceManager::ertImage:
		pDS.GetClassImage()->PushImage(&rt, static_cast<deImage*>(resource));
		break;
		
	case deResourceManager::ertLanguagePack:
		pDS.GetClassLanguagePack()->PushLanguagePack(&rt, static_cast<deLanguagePack*>(resource));
		break;
		
	case deResourceManager::ertModel:
		pDS.GetClassModel()->PushModel(&rt, static_cast<deModel*>(resource));
		break;
		
	case deResourceManager::ertOcclusionMesh:
		pDS.GetClassOcclusionMesh()->PushOcclusionMesh(&rt, static_cast<deOcclusionMesh*>(resource));
		break;
		
	case deResourceManager::ertParticleEmitter:
		pDS.GetClassParticleEmitter()->PushParticleEmitter(&rt, static_cast<deParticleEmitter*>(resource));
		break;
		
	case deResourceManager::ertRig:
		pDS.GetClassRig()->PushRig(&rt, static_cast<deRig*>(resource));
		break;
		
	case deResourceManager::ertSkin:
		pDS.GetClassSkin()->PushSkin(&rt, static_cast<deSkin*>(resource));
		break;
		
	case deResourceManager::ertSound:
		pDS.GetClassSound()->PushSound(&rt, static_cast<deSound*>(resource));
		break;
		
	case deResourceManager::ertVideo:
		pDS.GetClassVideo()->PushVideo(&rt, static_cast<deVideo*>(resource));
		break;
		
	case deResourceManager::ertService:
		pDS.GetClassService()->PushService(&rt, static_cast<deService*>(resource));
		break;
		
	default:
		rt.PushObject(nullptr, pDS.GetScriptEngine()->GetClassObject());
	}
}
