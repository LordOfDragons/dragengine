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

#include "deClassSmoothVector.h"
#include "../deClassVector.h"
#include "../../file/deClassFileReader.h"
#include "../../file/deClassFileWriter.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/smooth/decSmoothVector.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sSmVecNatDat{
	decSmoothVector *smoothVector;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSmoothVector::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSmVector,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmVecNatDat * const nd = new (p_GetNativeData(myself)) sSmVecNatDat;
	
	nd->smoothVector = new decSmoothVector;
}

// public func new( SmoothVector smoothFloat )
deClassSmoothVector::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsSmVector,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSmVector); // smoothFloat
}
void deClassSmoothVector::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmVecNatDat * const nd = new (p_GetNativeData(myself)) sSmVecNatDat;
	const deClassSmoothVector &clsSmoothVector = *(static_cast<deClassSmoothVector*>(GetOwnerClass()));
	
	const decSmoothVector &copy = clsSmoothVector.GetSmoothVector(rt->GetValue(0)->GetRealObject());
	nd->smoothVector = new decSmoothVector(copy);
}


// public func destructor()
deClassSmoothVector::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSmVector,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->~sSmVecNatDat();
}



// Accessors
//////////////

// public func Vector getValue()
deClassSmoothVector::nfGetValue::nfGetValue(const sInitData &init) : dsFunction(init.clsSmVector,
"getValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassSmoothVector::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothVector*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassVector()->PushVector(rt, smoothFloat.GetValue());
}

// public func void setValue( Vector value )
deClassSmoothVector::nfSetValue::nfSetValue(const sInitData &init) : dsFunction(init.clsSmVector,
"setValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // value
}
void deClassSmoothVector::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothVector*>(GetOwnerClass()))->GetDS();
	
	const decVector &value = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetValue(value);
}

// public func Vector getGoal()
deClassSmoothVector::nfGetGoal::nfGetGoal(const sInitData &init) : dsFunction(init.clsSmVector,
"getGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassSmoothVector::nfGetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothVector*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassVector()->PushVector(rt, smoothFloat.GetGoal());
}

// public func void setGoal( Vector goal )
deClassSmoothVector::nfSetGoal::nfSetGoal(const sInitData &init) : dsFunction(init.clsSmVector,
"setGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // goal
}
void deClassSmoothVector::nfSetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothVector*>(GetOwnerClass()))->GetDS();
	
	const decVector &value = ds.GetClassVector()->GetVector(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetGoal(value);
}

// public func float getAdjustTime()
deClassSmoothVector::nfGetAdjustTime::nfGetAdjustTime(const sInitData &init) : dsFunction(init.clsSmVector,
"getAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector::nfGetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	rt->PushFloat(smoothFloat.GetAdjustTime());
}

// public void setAdjustTime( float adjustTime )
deClassSmoothVector::nfSetAdjustTime::nfSetAdjustTime(const sInitData &init) : dsFunction(init.clsSmVector,
"setAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothVector::nfSetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	smoothFloat.SetAdjustTime(rt->GetValue(0)->GetFloat());
}

// public func float getAdjustRange()
deClassSmoothVector::nfGetAdjustRange::nfGetAdjustRange(const sInitData &init) : dsFunction(init.clsSmVector,
"getAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector::nfGetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	rt->PushFloat(smoothFloat.GetAdjustRange());
}

// public func void setAdjustRange( float range )
deClassSmoothVector::nfSetAdjustRange::nfSetAdjustRange(const sInitData &init) : dsFunction(init.clsSmVector,
"setAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // range
}
void deClassSmoothVector::nfSetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	smoothFloat.SetAdjustRange(rt->GetValue(0)->GetFloat());
}

// public func float getChangeSpeed()
deClassSmoothVector::nfGetChangeSpeed::nfGetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmVector,
"getChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothVector::nfGetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	rt->PushFloat(smoothFloat.GetChangeSpeed());
}

// public func void setChangeSpeed( float changeSpeed )
deClassSmoothVector::nfSetChangeSpeed::nfSetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmVector,
"setChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothVector::nfSetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	smoothFloat.SetChangeSpeed(rt->GetValue(0)->GetFloat());
}



// public func void reset()
deClassSmoothVector::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsSmVector,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothVector::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	smoothFloat.Reset();
}

// public func void update( float elapsed )
deClassSmoothVector::nfUpdate::nfUpdate(const sInitData &init) : dsFunction(init.clsSmVector,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // elapsed
}
void deClassSmoothVector::nfUpdate::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	
	smoothFloat.Update(rt->GetValue(0)->GetFloat());
}



// File Handling
//////////////////

// static public func SmoothVector readFromFile( FileReader reader )
deClassSmoothVector::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsSmVector,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSmVector){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassSmoothVector::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSmoothVector &clsSmoothVector = *(static_cast<deClassSmoothVector*>(GetOwnerClass()));
	const deClassFileReader &clsFileReader = *clsSmoothVector.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	decSmoothVector smoothVector;
	const int version = reader->ReadByte();
	
	switch(version){
	case 0:
		smoothVector.SetValue(reader->ReadVector());
		smoothVector.SetGoal(reader->ReadVector());
		smoothVector.SetAdjustRange(reader->ReadFloat());
		smoothVector.SetAdjustTime(reader->ReadFloat());
		smoothVector.SetChangeSpeed(reader->ReadFloat());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsSmoothVector.PushSmoothVector(rt, smoothVector);
}

// public func void writeToFile( FileWriter writer )
deClassSmoothVector::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsSmVector,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassSmoothVector::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothVector = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	const deClassSmoothVector &clsSmoothVector = *(static_cast<deClassSmoothVector*>(GetOwnerClass()));
	const deClassFileWriter &clsFileWriter = *clsSmoothVector.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteByte(0); // version 0
	writer->WriteVector(smoothVector.GetValue());
	writer->WriteVector(smoothVector.GetGoal());
	writer->WriteFloat(smoothVector.GetAdjustRange());
	writer->WriteFloat(smoothVector.GetAdjustTime());
	writer->WriteFloat(smoothVector.GetChangeSpeed());
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassSmoothVector::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsSmVector,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassSmoothVector::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	deClassSmoothVector * const clsSmoothVector = static_cast<deClassSmoothVector*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSmoothVector)){
		rt->PushBool(false);
		
	}else{
		const decSmoothVector &otherSmoothVector = clsSmoothVector->GetSmoothVector(obj->GetRealObject());
		rt->PushBool(smoothFloat == otherSmoothVector);
	}
}

// public func String toString()
deClassSmoothVector::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsSmVector,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassSmoothVector::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothVector &smoothFloat = *static_cast<sSmVecNatDat*>(p_GetNativeData(myself))->smoothVector;
	decString str;
	
	str.Format("[%g,%g,%g]", smoothFloat.GetValue().x, smoothFloat.GetValue().y, smoothFloat.GetValue().z);
	
	rt->PushString(str);
}



// Class deClassSmoothVector
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassSmoothVector::deClassSmoothVector(deScriptingDragonScript &ds) :
dsClass("SmoothVector", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSmVecNatDat));
}

deClassSmoothVector::~deClassSmoothVector(){
}



// Management
///////////////

void deClassSmoothVector::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSmVector = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec = pDS.GetClassVector();
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

const decSmoothVector &deClassSmoothVector::GetSmoothVector(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *static_cast<sSmVecNatDat*>(p_GetNativeData(myself->GetBuffer()))->smoothVector;
}

void deClassSmoothVector::PushSmoothVector(dsRunTime *rt, const decSmoothVector &smoothFloat){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSmVecNatDat &nd = *static_cast<sSmVecNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.smoothVector = nullptr;
	
	try{
		nd.smoothVector = new decSmoothVector(smoothFloat);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
