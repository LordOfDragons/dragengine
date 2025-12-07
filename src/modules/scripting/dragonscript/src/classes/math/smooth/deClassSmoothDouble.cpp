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

#include "deClassSmoothDouble.h"
#include "../../file/deClassFileReader.h"
#include "../../file/deClassFileWriter.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/smooth/decSmoothDouble.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sSmDblNatDat{
	decSmoothDouble *smoothDouble;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSmoothDouble::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSmDouble,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDouble::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmDblNatDat &nd = *((sSmDblNatDat*)p_GetNativeData(myself));
	
	nd.smoothDouble = NULL;
	
	nd.smoothDouble = new decSmoothDouble;
}

// public func new( SmoothDouble smoothDouble )
deClassSmoothDouble::nfNewCopy::nfNewCopy(const sInitData &init) : dsFunction(init.clsSmDouble,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSmDouble); // smoothDouble
}
void deClassSmoothDouble::nfNewCopy::RunFunction(dsRunTime *rt, dsValue *myself){
	sSmDblNatDat &nd = *((sSmDblNatDat*)p_GetNativeData(myself));
	deClassSmoothDouble &clsSmoothDouble = *((deClassSmoothDouble*)GetOwnerClass());
	
	nd.smoothDouble = NULL;
	
	const decSmoothDouble &copy = clsSmoothDouble.GetSmoothDouble(rt->GetValue(0)->GetRealObject());
	nd.smoothDouble = new decSmoothDouble(copy);
}


// public func destructor()
deClassSmoothDouble::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSmDouble,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDouble::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSmDblNatDat &nd = *((sSmDblNatDat*)p_GetNativeData(myself));
	
	if(nd.smoothDouble){
		delete nd.smoothDouble;
		nd.smoothDouble = NULL;
	}
}



// Accessors
//////////////

// public func float getValue()
deClassSmoothDouble::nfGetValue::nfGetValue(const sInitData &init) : dsFunction(init.clsSmDouble,
"getValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDouble::nfGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	rt->PushFloat((float)smoothDouble.GetValue());
}

// public func void setValue( float value )
deClassSmoothDouble::nfSetValue::nfSetValue(const sInitData &init) : dsFunction(init.clsSmDouble,
"setValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDouble::nfSetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.SetValue((double)rt->GetValue(0)->GetFloat());
}

// public func float getGoal()
deClassSmoothDouble::nfGetGoal::nfGetGoal(const sInitData &init) : dsFunction(init.clsSmDouble,
"getGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDouble::nfGetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	rt->PushFloat((float)smoothDouble.GetGoal());
}

// public func void setGoal( float goal )
deClassSmoothDouble::nfSetGoal::nfSetGoal(const sInitData &init) : dsFunction(init.clsSmDouble,
"setGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDouble::nfSetGoal::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.SetGoal((double)rt->GetValue(0)->GetFloat());
}

// public func float getAdjustTime()
deClassSmoothDouble::nfGetAdjustTime::nfGetAdjustTime(const sInitData &init) : dsFunction(init.clsSmDouble,
"getAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDouble::nfGetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	rt->PushFloat((float)smoothDouble.GetAdjustTime());
}

// public void setAdjustTime( float adjustTime )
deClassSmoothDouble::nfSetAdjustTime::nfSetAdjustTime(const sInitData &init) : dsFunction(init.clsSmDouble,
"setAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDouble::nfSetAdjustTime::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.SetAdjustTime((double)rt->GetValue(0)->GetFloat());
}

// public func float getAdjustRange()
deClassSmoothDouble::nfGetAdjustRange::nfGetAdjustRange(const sInitData &init) : dsFunction(init.clsSmDouble,
"getAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDouble::nfGetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	rt->PushFloat((float)smoothDouble.GetAdjustRange());
}

// public func void setAdjustRange( float limit )
deClassSmoothDouble::nfSetAdjustRange::nfSetAdjustRange(const sInitData &init) : dsFunction(init.clsSmDouble,
"setAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDouble::nfSetAdjustRange::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.SetAdjustRange((double)rt->GetValue(0)->GetFloat());
}

// public func float getChangeSpeed()
deClassSmoothDouble::nfGetChangeSpeed::nfGetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmDouble,
"getChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSmoothDouble::nfGetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	rt->PushFloat((float)smoothDouble.GetChangeSpeed());
}

// public func void setChangeSpeed( float changeSpeed )
deClassSmoothDouble::nfSetChangeSpeed::nfSetChangeSpeed(const sInitData &init) : dsFunction(init.clsSmDouble,
"setChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // value
}
void deClassSmoothDouble::nfSetChangeSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.SetChangeSpeed((double)rt->GetValue(0)->GetFloat());
}



// public func void reset()
deClassSmoothDouble::nfReset::nfReset(const sInitData &init) : dsFunction(init.clsSmDouble,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSmoothDouble::nfReset::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.Reset();
}

// public func void update( float elapsed )
deClassSmoothDouble::nfUpdate::nfUpdate(const sInitData &init) : dsFunction(init.clsSmDouble,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // elapsed
}
void deClassSmoothDouble::nfUpdate::RunFunction(dsRunTime *rt, dsValue *myself){
	decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	
	smoothDouble.Update((double)rt->GetValue(0)->GetFloat());
}



// File Handling
//////////////////

// static public func SmoothDouble readFromFile( FileReader reader )
deClassSmoothDouble::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsSmDouble,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSmDouble){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassSmoothDouble::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSmoothDouble &clsSmoothDouble = *((deClassSmoothDouble*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsSmoothDouble.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	decSmoothDouble smoothDouble;
	const int version = reader->ReadByte();
	
	switch(version){
	case 0:
		smoothDouble.SetValue(reader->ReadDouble());
		smoothDouble.SetGoal(reader->ReadDouble());
		smoothDouble.SetAdjustRange(reader->ReadDouble());
		smoothDouble.SetAdjustTime(reader->ReadDouble());
		smoothDouble.SetChangeSpeed(reader->ReadDouble());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	clsSmoothDouble.PushSmoothDouble(rt, smoothDouble);
}

// public func void writeToFile( FileWriter writer )
deClassSmoothDouble::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsSmDouble,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassSmoothDouble::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	const deClassSmoothDouble &clsSmoothDouble = *((deClassSmoothDouble*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsSmoothDouble.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteByte(0); // version 0
	writer->WriteDouble(smoothDouble.GetValue());
	writer->WriteDouble(smoothDouble.GetGoal());
	writer->WriteDouble(smoothDouble.GetAdjustRange());
	writer->WriteDouble(smoothDouble.GetAdjustTime());
	writer->WriteDouble(smoothDouble.GetChangeSpeed());
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassSmoothDouble::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsSmDouble,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassSmoothDouble::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	deClassSmoothDouble * const clsSmoothDouble = (deClassSmoothDouble*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSmoothDouble)){
		rt->PushBool(false);
		
	}else{
		const decSmoothDouble &otherSmoothDouble = clsSmoothDouble->GetSmoothDouble(obj->GetRealObject());
		rt->PushBool(smoothDouble == otherSmoothDouble);
	}
}

// public func String toString()
deClassSmoothDouble::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsSmDouble,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassSmoothDouble::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decSmoothDouble &smoothDouble = *((sSmDblNatDat*)p_GetNativeData(myself))->smoothDouble;
	decString str;
	
	str.Format("%g", smoothDouble.GetValue());
	
	rt->PushString(str);
}



// Class deClassSmoothDouble
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassSmoothDouble::deClassSmoothDouble(deScriptingDragonScript &ds) :
dsClass("SmoothDouble", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSmDblNatDat));
}

deClassSmoothDouble::~deClassSmoothDouble(){
}



// Management
///////////////

void deClassSmoothDouble::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSmDouble = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
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

const decSmoothDouble &deClassSmoothDouble::GetSmoothDouble(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return *((sSmDblNatDat*)p_GetNativeData(myself->GetBuffer()))->smoothDouble;
}

void deClassSmoothDouble::PushSmoothDouble(dsRunTime *rt, const decSmoothDouble &smoothDouble){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	sSmDblNatDat &nd = *((sSmDblNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.smoothDouble = NULL;
	
	try{
		nd.smoothDouble = new decSmoothDouble(smoothDouble);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
