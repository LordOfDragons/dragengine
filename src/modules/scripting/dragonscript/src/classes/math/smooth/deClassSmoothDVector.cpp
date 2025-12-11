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

#include "deClassSmoothDVector.h"
#include "../deClassDVector.h"
#include "../../file/deClassFileReader.h"
#include "../../file/deClassFileWriter.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/smooth/decSmoothDVector.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sSmDVecNatDat{
	decSmoothDVector *smoothDVector = nullptr;
	
	~sSmDVecNatDat(){
		if(smoothDVector){
			delete smoothDVector;
			smoothDVector = nullptr;
		}
	}
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSmoothDVector::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSmDVector,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDVector::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmDVecNatDat * const nd = new (p_GetNativeData(myself)) sSmDVecNatDat;
	
	nd->smoothDVector = new decSmoothDVector;
}

// public func new( SmoothDVector smoothFloat )
deClassSmoothDVector::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsSmDVector,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSmDVector); // smoothFloat
}
void deClassSmoothDVector::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmDVecNatDat * const nd = new (p_GetNativeData(myself)) sSmDVecNatDat;
	const deClassSmoothDVector &clsSmoothDVector = *(static_cast<deClassSmoothDVector*>(GetOwnerClass()));
	
	const decSmoothDVector &copy = clsSmoothDVector.GetSmoothDVector(rt->GetValue(0)->GetRealObject());
	nd->smoothDVector = new decSmoothDVector(copy);
}


// public func destructor()
deClassSmoothDVector::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSmDVector,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDVector::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->~sSmDVecNatDat();
}



// Accessors
//////////////

// public func DVector getValue()
deClassSmoothDVector::nfGetValue::nfGetValue(const sInitData &init) : dsFunction(init.clsSmDVector,
"getValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassSmoothDVector::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothDVector*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassDVector()->PushDVector(rt, smoothFloat.GetValue());
}

// public func void setValue( DVector value )
deClassSmoothDVector::nfSetValue::nfSetValue(const sInitData &init) : dsFunction(init.clsSmDVector,
"setValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // value
}
void deClassSmoothDVector::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothDVector*>(GetOwnerClass()))->GetDS();
	
	const decDVector &value = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetValue(value);
}

// public func DVector getGoal()
deClassSmoothDVector::nfGetGoal::nfGetGoal(const sInitData &init) : dsFunction(init.clsSmDVector,
"getGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassSmoothDVector::nfGetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothDVector*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassDVector()->PushDVector(rt, smoothFloat.GetGoal());
}

// public func void setGoal( DVector goal )
deClassSmoothDVector::nfSetGoal::nfSetGoal(const sInitData &init) : dsFunction(init.clsSmDVector,
"setGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // goal
}
void deClassSmoothDVector::nfSetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	const deScriptingDragonScript &ds = (static_cast<deClassSmoothDVector*>(GetOwnerClass()))->GetDS();
	
	const decDVector &value = ds.GetClassDVector()->GetDVector(rt->GetValue(0)->GetRealObject());
	smoothFloat.SetGoal(value);
}

// public func float getAdjustTime()
deClassSmoothDVector::nfGetAdjustTime::nfGetAdjustTime(const sInitData &init) : dsFunction(init.clsSmDVector,
"getAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDVector::nfGetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	rt->PushFloat((float)smoothFloat.GetAdjustTime());
}

// public void setAdjustTime( float adjustTime )
deClassSmoothDVector::nfSetAdjustTime::nfSetAdjustTime(const sInitData &init) : dsFunction(init.clsSmDVector,
"setAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDVector::nfSetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	smoothFloat.SetAdjustTime((double)rt->GetValue(0)->GetFloat());
}

// public func float getAdjustRange()
deClassSmoothDVector::nfGetAdjustRange::nfGetAdjustRange(const sInitData &init) : dsFunction(init.clsSmDVector,
"getAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDVector::nfGetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	rt->PushFloat((float)smoothFloat.GetAdjustRange());
}

// public func void setAdjustRange( float range )
deClassSmoothDVector::nfSetAdjustRange::nfSetAdjustRange(const sInitData &init) : dsFunction(init.clsSmDVector,
"setAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // range
}
void deClassSmoothDVector::nfSetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	smoothFloat.SetAdjustRange((double)rt->GetValue(0)->GetFloat());
}

// public func float getChangeSpeed()
deClassSmoothDVector::nfGetChangeSpeed::nfGetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmDVector,
"getChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDVector::nfGetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	rt->PushFloat((float)smoothFloat.GetChangeSpeed());
}

// public func void setChangeSpeed( float changeSpeed )
deClassSmoothDVector::nfSetChangeSpeed::nfSetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmDVector,
"setChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDVector::nfSetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	smoothFloat.SetChangeSpeed((double)rt->GetValue(0)->GetFloat());
}



// public func void reset()
deClassSmoothDVector::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsSmDVector,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDVector::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	smoothFloat.Reset();
}

// public func void update( float elapsed )
deClassSmoothDVector::nfUpdate::nfUpdate(const sInitData &init) : dsFunction(init.clsSmDVector,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // elapsed
}
void deClassSmoothDVector::nfUpdate::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	
	smoothFloat.Update((double)rt->GetValue(0)->GetFloat());
}



// File Handling
//////////////////

// static public func SmoothDVector readFromFile( FileReader reader )
deClassSmoothDVector::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsSmDVector,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSmDVector){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassSmoothDVector::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSmoothDVector &clsSmoothDVector = *(static_cast<deClassSmoothDVector*>(GetOwnerClass()));
	const deClassFileReader &clsFileReader = *clsSmoothDVector.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	decSmoothDVector smoothDVector;
	const int version = reader->ReadByte();
	
	switch(version){
	case 0:
		smoothDVector.SetValue(reader->ReadDVector());
		smoothDVector.SetGoal(reader->ReadDVector());
		smoothDVector.SetAdjustRange(reader->ReadDouble());
		smoothDVector.SetAdjustTime(reader->ReadDouble());
		smoothDVector.SetChangeSpeed(reader->ReadDouble());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsSmoothDVector.PushSmoothDVector(rt, smoothDVector);
}

// public func void writeToFile( FileWriter writer )
deClassSmoothDVector::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsSmDVector,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassSmoothDVector::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothDVector = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	const deClassSmoothDVector &clsSmoothDVector = *(static_cast<deClassSmoothDVector*>(GetOwnerClass()));
	const deClassFileWriter &clsFileWriter = *clsSmoothDVector.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteByte(0); // version 0
	writer->WriteDVector(smoothDVector.GetValue());
	writer->WriteDVector(smoothDVector.GetGoal());
	writer->WriteDouble(smoothDVector.GetAdjustRange());
	writer->WriteDouble(smoothDVector.GetAdjustTime());
	writer->WriteDouble(smoothDVector.GetChangeSpeed());
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassSmoothDVector::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsSmDVector,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassSmoothDVector::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	deClassSmoothDVector * const clsSmoothDVector = static_cast<deClassSmoothDVector*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSmoothDVector)){
		rt->PushBool(false);
		
	}else{
		const decSmoothDVector &otherSmoothDVector = clsSmoothDVector->GetSmoothDVector(obj->GetRealObject());
		rt->PushBool(smoothFloat == otherSmoothDVector);
	}
}

// public func String toString()
deClassSmoothDVector::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsSmDVector,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassSmoothDVector::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDVector &smoothFloat = *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself))->smoothDVector;
	decString str;
	
	str.Format("[%g,%g,%g]", smoothFloat.GetValue().x, smoothFloat.GetValue().y, smoothFloat.GetValue().z);
	
	rt->PushString(str);
}



// Class deClassSmoothDVector
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassSmoothDVector::deClassSmoothDVector(deScriptingDragonScript &ds) :
dsClass("SmoothDVector", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSmDVecNatDat));
}

deClassSmoothDVector::~deClassSmoothDVector(){
}



// Management
///////////////

void deClassSmoothDVector::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSmDVector = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsDVec = pDS.GetClassDVector();
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

const decSmoothDVector &deClassSmoothDVector::GetSmoothDVector(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *static_cast<sSmDVecNatDat*>(p_GetNativeData(myself->GetBuffer()))->smoothDVector;
}

void deClassSmoothDVector::PushSmoothDVector(dsRunTime *rt, const decSmoothDVector &smoothFloat){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSmDVecNatDat * const nd = new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sSmDVecNatDat;
	
	try{
		nd->smoothDVector = new decSmoothDVector(smoothFloat);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
