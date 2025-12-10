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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deClassSmoothVector2.h"
#include "../deClassVector2.h"
#include "../../file/deClassFileReader.h"
#include "../../file/deClassFileWriter.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/smooth/decSmoothVector2.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sSmVecNatDat{
	decSmoothVector2 *smoothVector2;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSmoothVector2::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSmVector2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector2::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmVecNatDat * const nd = new (p_GetNativeData(myself)) sSmVecNatDat;
	
	nd->smoothVector2 = new decSmoothVector2;
}

// public func new( SmoothVector2 smoothFloat )
deClassSmoothVector2::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsSmVector2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSmVector2); // smoothFloat
}
void deClassSmoothVector2::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmVecNatDat * const nd = new (p_GetNativeData(myself)) sSmVecNatDat;
	deClassSmoothVector2 &clsSmoothVector2 = *((deClassSmoothVector2*)GetOwnerClass());
	
	const decSmoothVector2 &copy = clsSmoothVector2.GetSmoothVector2(rt->GetValue(0)->GetRealObject());
	nd->smoothVector2 = new decSmoothVector2(copy);
}


// public func destructor()
deClassSmoothVector2::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSmVector2,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector2::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->~sSmVecNatDat();
}



// Accessors
//////////////

// public func Vector2 getValue()
deClassSmoothVector2::nfGetValue::nfGetValue(const sInitData &init) : dsFunction(init.clsSmVector2,
"getValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassSmoothVector2::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	const deScriptingDragonScript &ds = ((deClassSmoothVector2*)GetOwnerClass())->GetDS();
	
	ds.GetClassVector2()->PushVector2(rt, smoothFloat.GetValue());
}

// public func void setValue( Vector2 value )
deClassSmoothVector2::nfSetValue::nfSetValue(const sInitData &init) : dsFunction(init.clsSmVector2,
"setValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // value
}
void deClassSmoothVector2::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	const deScriptingDragonScript &ds = ((deClassSmoothVector2*)GetOwnerClass())->GetDS();
	
	const decVector2 &value = ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetValue(value);
}

// public func Vector2 getGoal()
deClassSmoothVector2::nfGetGoal::nfGetGoal(const sInitData &init) : dsFunction(init.clsSmVector2,
"getGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassSmoothVector2::nfGetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	const deScriptingDragonScript &ds = ((deClassSmoothVector2*)GetOwnerClass())->GetDS();
	
	ds.GetClassVector2()->PushVector2(rt, smoothFloat.GetGoal());
}

// public func void setGoal( Vector2 goal )
deClassSmoothVector2::nfSetGoal::nfSetGoal(const sInitData &init) : dsFunction(init.clsSmVector2,
"setGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // goal
}
void deClassSmoothVector2::nfSetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	const deScriptingDragonScript &ds = ((deClassSmoothVector2*)GetOwnerClass())->GetDS();
	
	const decVector2 &value = ds.GetClassVector2()->GetVector2(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetGoal(value);
}

// public func float getAdjustTime()
deClassSmoothVector2::nfGetAdjustTime::nfGetAdjustTime(const sInitData &init) : dsFunction(init.clsSmVector2,
"getAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector2::nfGetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	rt->PushFloat(smoothFloat.GetAdjustTime());
}

// public void setAdjustTime( float adjustTime )
deClassSmoothVector2::nfSetAdjustTime::nfSetAdjustTime(const sInitData &init) : dsFunction(init.clsSmVector2,
"setAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothVector2::nfSetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	smoothFloat.SetAdjustTime(rt->GetValue(0)->GetFloat());
}

// public func float getAdjustRange()
deClassSmoothVector2::nfGetAdjustRange::nfGetAdjustRange(const sInitData &init) : dsFunction(init.clsSmVector2,
"getAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector2::nfGetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	rt->PushFloat(smoothFloat.GetAdjustRange());
}

// public func void setAdjustRange( float range )
deClassSmoothVector2::nfSetAdjustRange::nfSetAdjustRange(const sInitData &init) : dsFunction(init.clsSmVector2,
"setAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // range
}
void deClassSmoothVector2::nfSetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	smoothFloat.SetAdjustRange(rt->GetValue(0)->GetFloat());
}

// public func float getChangeSpeed()
deClassSmoothVector2::nfGetChangeSpeed::nfGetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmVector2,
"getChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector2::nfGetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	rt->PushFloat(smoothFloat.GetChangeSpeed());
}

// public func void setChangeSpeed( float changeSpeed )
deClassSmoothVector2::nfSetChangeSpeed::nfSetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmVector2,
"setChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothVector2::nfSetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	smoothFloat.SetChangeSpeed(rt->GetValue(0)->GetFloat());
}



// public func void reset()
deClassSmoothVector2::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsSmVector2,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector2::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	smoothFloat.Reset();
}

// public func void update( float elapsed )
deClassSmoothVector2::nfUpdate::nfUpdate(const sInitData &init) : dsFunction(init.clsSmVector2,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // elapsed
}
void deClassSmoothVector2::nfUpdate::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	
	smoothFloat.Update(rt->GetValue(0)->GetFloat());
}



// File Handling
//////////////////

// static public func SmoothVector2 readFromFile( FileReader reader )
deClassSmoothVector2::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsSmVector2,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSmVector2){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassSmoothVector2::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSmoothVector2 &clsSmoothVector2 = *((deClassSmoothVector2*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsSmoothVector2.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	decSmoothVector2 smoothVector2;
	const int version = reader->ReadByte();
	
	switch(version){
	case 0:
		smoothVector2.SetValue(reader->ReadVector2());
		smoothVector2.SetGoal(reader->ReadVector2());
		smoothVector2.SetAdjustRange(reader->ReadFloat());
		smoothVector2.SetAdjustTime(reader->ReadFloat());
		smoothVector2.SetChangeSpeed(reader->ReadFloat());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsSmoothVector2.PushSmoothVector2(rt, smoothVector2);
}

// public func void writeToFile( FileWriter writer )
deClassSmoothVector2::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsSmVector2,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassSmoothVector2::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothVector2 = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	const deClassSmoothVector2 &clsSmoothVector2 = *((deClassSmoothVector2*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsSmoothVector2.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteByte(0); // version 0
	writer->WriteVector2(smoothVector2.GetValue());
	writer->WriteVector2(smoothVector2.GetGoal());
	writer->WriteFloat(smoothVector2.GetAdjustRange());
	writer->WriteFloat(smoothVector2.GetAdjustTime());
	writer->WriteFloat(smoothVector2.GetChangeSpeed());
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassSmoothVector2::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsSmVector2,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassSmoothVector2::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	deClassSmoothVector2 * const clsSmoothVector2 = (deClassSmoothVector2*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSmoothVector2)){
		rt->PushBool(false);
		
	}else{
		const decSmoothVector2 &otherSmoothVector2 = clsSmoothVector2->GetSmoothVector2(obj->GetRealObject());
		rt->PushBool(smoothFloat == otherSmoothVector2);
	}
}

// public func String toString()
deClassSmoothVector2::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsSmVector2,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassSmoothVector2::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector2 &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector2;
	decString str;
	
	str.Format("[%g,%g]", smoothFloat.GetValue().x, smoothFloat.GetValue().y);
	
	rt->PushString(str);
}



// Class deClassSmoothVector2
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassSmoothVector2::deClassSmoothVector2(deScriptingDragonScript &ds) :
dsClass("SmoothVector2", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSmVecNatDat));
}

deClassSmoothVector2::~deClassSmoothVector2(){
}



// Management
///////////////

void deClassSmoothVector2::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSmVector2 = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec = pDS.GetClassVector2();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewCopy(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetValue(init));
	AddFunction(new nfSetValue(init));
	AddFunction(new nfGetGoal(init));
	AddFunction(new nfSetGoal(init));
	AddFunction(new nfGetAdjustTime(init));
	AddFunction(new nfSetAdjustTime(init));
	AddFunction(new nfGetAdjustRange(init));
	AddFunction(new nfSetAdjustRange(init));
	AddFunction(new nfGetChangeSpeed(init));
	AddFunction(new nfSetChangeSpeed(init));
	
	AddFunction(new nfReset(init));
	AddFunction(new nfUpdate(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfToString(init));
}

const decSmoothVector2 &deClassSmoothVector2::GetSmoothVector2(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *static_cast<sSmVecNatDat*>(p_GetNativeData(myself->GetBuffer()))->smoothVector2;
}

void deClassSmoothVector2::PushSmoothVector2(dsRunTime *rt, const decSmoothVector2 &smoothFloat){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSmVecNatDat &nd = *static_cast<sSmVecNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.smoothVector2 = NULL;
	
	try{
		nd.smoothVector2 = new decSmoothVector2(smoothFloat);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
