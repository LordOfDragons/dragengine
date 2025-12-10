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

#include <libdscript/libdscript.h>

#include "deClassVector.h"
#include "deClassDVector.h"
#include "deClassPoint3.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Native structure
struct sDVecNatDat{
	decDVector vector;
};



// Native Functions
/////////////////////

// Constructors, destructor
/////////////////////////////

// public func new()
deClassDVector::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsDVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDVector::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	vector.Set(0.0, 0.0, 0.0);
}

// public func new( float x, float y, float z )
deClassDVector::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsDVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // x
	p_AddParameter(init.clsFlt); // y
	p_AddParameter(init.clsFlt); // z
}
void deClassDVector::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	vector.x = rt->GetValue(0)->GetFloat();
	vector.y = rt->GetValue(1)->GetFloat();
	vector.z = rt->GetValue(2)->GetFloat();
}

// public func new( DVector v )
deClassDVector::nfNew3::nfNew3(const sInitData &init) : dsFunction(init.clsDVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfNew3::RunFunction(dsRunTime *rt, dsValue *myself){
	decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector * const clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	vector = clsDVector->GetDVector(objVec);
}

// public func new( Vector v )
deClassDVector::nfNewFromVector::nfNewFromVector(const sInitData &init) : dsFunction(init.clsDVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // v
}
void deClassDVector::nfNewFromVector::RunFunction(dsRunTime *rt, dsValue *myself){
	decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassVector *clsVector = (static_cast<deClassDVector*>(GetOwnerClass()))->GetClassVector();
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	const decVector &paramVector = clsVector->GetVector(objVec);
	vector.x = paramVector.x;
	vector.y = paramVector.y;
	vector.z = paramVector.z;
}

// public func destructor()
deClassDVector::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsDVec,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDVector::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sDVecNatDat*>(p_GetNativeData(myself))->~sDVecNatDat();
}



// Accessors
//////////////

// public func float getX()
deClassDVector::nfGetX::nfGetX(const sInitData &init) : dsFunction(init.clsDVec,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassDVector::nfGetX::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushFloat((float)vector.x);
}

// public func float getY()
deClassDVector::nfGetY::nfGetY(const sInitData &init) : dsFunction(init.clsDVec,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassDVector::nfGetY::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushFloat((float)vector.y);
}

// public func float getZ()
deClassDVector::nfGetZ::nfGetZ(const sInitData &init) : dsFunction(init.clsDVec,
"getZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassDVector::nfGetZ::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushFloat((float)vector.z);
}

// public func float get( int component )
deClassDVector::nfGet::nfGet(const sInitData &init) : dsFunction(init.clsDVec,
"get", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // component
}
void deClassDVector::nfGet::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	int component = rt->GetValue(0)->GetInt();
	
	if(component == 0){
		rt->PushFloat((float)vector.x);
		
	}else if(component == 1){
		rt->PushFloat((float)vector.y);
		
	}else if(component == 2){
		rt->PushFloat((float)vector.z);
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func float getLength()
deClassDVector::nfGetLength::nfGetLength(const sInitData &init) : dsFunction(init.clsDVec,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassDVector::nfGetLength::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushFloat((float)vector.Length());
}

// public func float getLengthSquared()
deClassDVector::nfGetLengthSquared::nfGetLengthSquared(const sInitData &init) : dsFunction(init.clsDVec,
"getLengthSquared", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassDVector::nfGetLengthSquared::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushFloat((float)vector.LengthSquared());
}

// public func DVector normalize()
deClassDVector::nfNormalize::nfNormalize(const sInitData &init) : dsFunction(init.clsDVec,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassDVector::nfNormalize::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	double len = vector.Length();
	if(len == 0.0) DSTHROW(dueDivisionByZero);
	
	clsDVector->PushDVector(rt, vector / len);
}

// public func DVector absolute()
deClassDVector::nfAbsolute::nfAbsolute(const sInitData &init) : dsFunction(init.clsDVec,
"absolute", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassDVector::nfAbsolute::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	clsDVector->PushDVector(rt, vector.Absolute());
}

// public func DVector compMultiply( DVector v )
deClassDVector::nfCompMultiply::nfCompMultiply(const sInitData &init) : dsFunction(init.clsDVec,
"compMultiply", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfCompMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	const decDVector &otherDVector = clsDVector->GetDVector(objVec);
	
	clsDVector->PushDVector(rt, decDVector(vector.x * otherDVector.x, vector.y * otherDVector.y, vector.z * otherDVector.z));
}

// public func DVector compDivide( DVector v )
deClassDVector::nfCompDivide::nfCompDivide(const sInitData &init) : dsFunction(init.clsDVec,
"compDivide", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfCompDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	const decDVector &otherDVector = clsDVector->GetDVector(objVec);
	
	clsDVector->PushDVector(rt, decDVector(vector.x / otherDVector.x, vector.y / otherDVector.y, vector.z / otherDVector.z));
}

// public func DVector compSelect( bool x, bool y, bool z )
deClassDVector::nfCompSelect::nfCompSelect(const sInitData &init) : dsFunction(init.clsDVec,
"compSelect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsBool); // x
	p_AddParameter(init.clsBool); // y
	p_AddParameter(init.clsBool); // z
}
void deClassDVector::nfCompSelect::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	
	decDVector result;
	
	if(rt->GetValue(0)->GetBool()){
		result.x = vector.x;
	}
	if(rt->GetValue(1)->GetBool()){
		result.y = vector.y;
	}
	if(rt->GetValue(2)->GetBool()){
		result.z = vector.z;
	}
	
	clsDVector.PushDVector(rt, result);
}

// public func DVector combine( DVector vector, bool x, bool y, bool z )
deClassDVector::nfCombine::nfCombine(const sInitData &init) : dsFunction(init.clsDVec,
"combine", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // vector
	p_AddParameter(init.clsBool); // x
	p_AddParameter(init.clsBool); // y
	p_AddParameter(init.clsBool); // z
}
void deClassDVector::nfCombine::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	
	const decDVector &otherDVector = clsDVector.GetDVector(rt->GetValue(0)->GetRealObject());
	const bool x = rt->GetValue(1)->GetBool();
	const bool y = rt->GetValue(2)->GetBool();
	const bool z = rt->GetValue(3)->GetBool();
	
	clsDVector.PushDVector(rt, decDVector(
		x ? vector.x : otherDVector.x,
		y ? vector.y : otherDVector.y,
		z ? vector.z : otherDVector.z
));
}

// public func DVector smallest( DVector v )
deClassDVector::nfSmallest::nfSmallest(const sInitData &init) : dsFunction(init.clsDVec,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfSmallest::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const decDVector &otherDVector = clsDVector.GetDVector(rt->GetValue(0)->GetRealObject());
	
	clsDVector.PushDVector(rt, vector.Smallest(otherDVector));
}

// public func DVector largest( DVector v )
deClassDVector::nfLargest::nfLargest(const sInitData &init) : dsFunction(init.clsDVec,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfLargest::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const decDVector &otherDVector = clsDVector.GetDVector(rt->GetValue(0)->GetRealObject());
	
	clsDVector.PushDVector(rt, vector.Largest(otherDVector));
}

// public func DVector clamped( DVector min, DVector max )
deClassDVector::nfClamped::nfClamped(const sInitData &init) : dsFunction(init.clsDVec,
"clamped", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // min
	p_AddParameter(init.clsDVec); // max
}
void deClassDVector::nfClamped::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const decDVector &min = clsDVector.GetDVector(rt->GetValue(0)->GetRealObject());
	const decDVector &max = clsDVector.GetDVector(rt->GetValue(1)->GetRealObject());
	
	clsDVector.PushDVector(rt, vector.Clamped(min, max));
}

// public func Point3 round()
deClassDVector::nfRound::nfRound(const sInitData &init) :
dsFunction(init.clsDVec, "round", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint3){
}
void deClassDVector::nfRound::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	(static_cast<deClassDVector*>(GetOwnerClass()))->GetScriptModule()->
		GetClassPoint3()->PushPoint(rt, vector.Round());
}

// public func DVector round(float unit)
deClassDVector::nfRound2::nfRound2(const sInitData &init) :
dsFunction(init.clsDVec, "round", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsFlt); // unit
}
void deClassDVector::nfRound2::RunFunction(dsRunTime *rt, dsValue *myself){
	decDVector vector(static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector);
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const double unit = (double)rt->GetValue(0)->GetFloat();
	
	vector /= unit;
	vector.x = floor(vector.x + 0.5);
	vector.y = floor(vector.y + 0.5);
	vector.z = floor(vector.z + 0.5);
	vector *= unit;
	clsDVector.PushDVector(rt, vector);
}

// public func DVector mix(DVector vector, float factor)
deClassDVector::nfMix::nfMix(const sInitData &init) :
dsFunction(init.clsDVec, "mix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // vector
	p_AddParameter(init.clsFlt); // factor
}
void deClassDVector::nfMix::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const decDVector &other = clsDVector.GetDVector(rt->GetValue(0)->GetRealObject());
	const float factor = rt->GetValue(1)->GetFloat();
	
	clsDVector.PushDVector(rt, vector.Mix(other, factor));
}

// public func Vector toVector()
deClassDVector::nfToVector::nfToVector(const sInitData &init) : dsFunction(init.clsDVec,
"toVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassDVector::nfToVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassVector *clsVector = (static_cast<deClassDVector*>(GetOwnerClass()))->GetClassVector();
	
	clsVector->PushVector(rt, vector.ToVector());
}



// Testing
////////////

// public func bool isEqualTo( DVector v, float delta )
deClassDVector::nfIsEqualTo::nfIsEqualTo(const sInitData &init) : dsFunction(init.clsDVec,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // v
	p_AddParameter(init.clsFlt); // delta
}
void deClassDVector::nfIsEqualTo::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	double delta = rt->GetValue(1)->GetFloat();
	rt->PushBool(vector.IsEqualTo(clsDVector->GetDVector(objVec), delta));
}

// public func bool isAtLeast( float value )
deClassDVector::nfIsAtLeast::nfIsAtLeast(const sInitData &init) : dsFunction(init.clsDVec,
"isAtLeast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsFlt); // value
}
void deClassDVector::nfIsAtLeast::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	double value = rt->GetValue(0)->GetFloat();
	
	rt->PushBool(vector.x >= value && vector.y >= value && vector.z >= value);
}

// public func bool isAtMost( float value )
deClassDVector::nfIsAtMost::nfIsAtMost(const sInitData &init) : dsFunction(init.clsDVec,
"isAtMost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsFlt); // value
}
void deClassDVector::nfIsAtMost::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	double value = rt->GetValue(0)->GetFloat();
	
	rt->PushBool(vector.x <= value && vector.y <= value && vector.z <= value);
}

// public func bool isZero()
deClassDVector::nfIsZero::nfIsZero(const sInitData &init) :
dsFunction(init.clsDVec, "isZero", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassDVector::nfIsZero::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	rt->PushBool(vector.IsZero());
}



// File Handling
//////////////////

// static public func DVector readFromFile( FileReader reader )
deClassDVector::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsDVec,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsDVec){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassDVector::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const deClassFileReader &clsFileReader = *clsDVector.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	decDVector vector;
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	clsDVector.PushDVector(rt, reader->ReadDVector());
}

// public func void writeToFile( FileWriter writer )
deClassDVector::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsDVec,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassDVector::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	const deClassDVector &clsDVector = *(static_cast<deClassDVector*>(GetOwnerClass()));
	const deClassFileWriter &clsFileWriter = *clsDVector.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	writer->WriteDVector(vector);
}



// Operators
//////////////

// public func DVector -()
deClassDVector::nfOpMinus::nfOpMinus(const sInitData &init) : dsFunction(init.clsDVec,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassDVector::nfOpMinus::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	clsDVector->PushDVector(rt, -vector);
}

// public func DVector +( DVector v )
deClassDVector::nfOpAdd::nfOpAdd(const sInitData &init) : dsFunction(init.clsDVec,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpAdd::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	clsDVector->PushDVector(rt, vector + clsDVector->GetDVector(objVec));
}

// public func DVector -( DVector v )
deClassDVector::nfOpSubtract::nfOpSubtract(const sInitData &init) : dsFunction(init.clsDVec,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpSubtract::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	clsDVector->PushDVector(rt, vector - clsDVector->GetDVector(objVec));
}

// public func DVector *( float k )
deClassDVector::nfOpScale::nfOpScale(const sInitData &init) : dsFunction(init.clsDVec,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsFlt); // k
}
void deClassDVector::nfOpScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	clsDVector->PushDVector(rt, vector * rt->GetValue(0)->GetFloat());
}

// public func DVector /( float k )
deClassDVector::nfOpDivide::nfOpDivide(const sInitData &init) : dsFunction(init.clsDVec,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsFlt); // k
}
void deClassDVector::nfOpDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	clsDVector->PushDVector(rt, vector / rt->GetValue(0)->GetFloat());
}

// public func float *( DVector v )
deClassDVector::nfOpDot::nfOpDot(const sInitData &init) : dsFunction(init.clsDVec,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpDot::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	rt->PushFloat((float)(vector * clsDVector->GetDVector(objVec)));
}

// public func DVector %( DVector v )
deClassDVector::nfOpCross::nfOpCross(const sInitData &init) : dsFunction(init.clsDVec,
"%", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpCross::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	clsDVector->PushDVector(rt, vector % clsDVector->GetDVector(objVec));
}

// public func bool <( DVector v )
deClassDVector::nfOpLess::nfOpLess(const sInitData &init) : dsFunction(init.clsDVec,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpLess::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector * const clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	rt->PushBool(vector < clsDVector->GetDVector(objVec));
}

// public func bool <=( DVector v )
deClassDVector::nfOpLessEqual::nfOpLessEqual(const sInitData &init) : dsFunction(init.clsDVec,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpLessEqual::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector * const clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	rt->PushBool(vector <= clsDVector->GetDVector(objVec));
}

// public func bool >( DVector v )
deClassDVector::nfOpGreater::nfOpGreater(const sInitData &init) : dsFunction(init.clsDVec,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpGreater::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector * const clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	rt->PushBool(vector > clsDVector->GetDVector(objVec));
}

// public func bool >=( DVector v )
deClassDVector::nfOpGreaterEqual::nfOpGreaterEqual(const sInitData &init) : dsFunction(init.clsDVec,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsDVec); // v
}
void deClassDVector::nfOpGreaterEqual::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector * const clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	
	dsRealObject *objVec = rt->GetValue(0)->GetRealObject();
	if(!objVec) DSTHROW(dueNullPointer);
	
	rt->PushBool(vector >= clsDVector->GetDVector(objVec));
}



// Specials
/////////////

// public func bool equals( Object other )
deClassDVector::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsDVec,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassDVector::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	deClassDVector *clsDVector = static_cast<deClassDVector*>(GetOwnerClass());
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsDVector)){
		rt->PushBool(false);
		
	}else{
		const decDVector &otherDVector = static_cast<sDVecNatDat*>(p_GetNativeData(obj))->vector;
		rt->PushBool(vector.IsEqualTo(otherDVector));
	}
}

// public func int hashCode()
deClassDVector::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsDVec,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassDVector::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	
	rt->PushInt((int)(vector.x * 1000000.0) + (int)(vector.y * 10000.0) + (int)(vector.z * 100.0));
}

// public func String toString()
deClassDVector::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsDVec,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassDVector::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	decString str;
	str.Format("(%f,%f,%f)", vector.x, vector.y, vector.z);
	rt->PushString(str);
}

// public func String toString( int precision )
deClassDVector::nfToStringPrecision::nfToStringPrecision(const sInitData &init) :
dsFunction(init.clsDVec, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // precision
}
void deClassDVector::nfToStringPrecision::RunFunction(dsRunTime *rt, dsValue *myself){
	const int precision = rt->GetValue(0)->GetInt();
	if(precision < 0){
		DSTHROW_INFO(dueInvalidParam, "precision < 0");
	}
	if(precision > 17){
		DSTHROW_INFO(dueInvalidParam, "precision > 17");
	}
	
	const unsigned short p = (unsigned short)precision;
	char format[21];
	snprintf(format, sizeof(format), "(%%.%huf,%%.%huf,%%.%huf)", p, p, p);
	
	const decDVector &vector = static_cast<sDVecNatDat*>(p_GetNativeData(myself))->vector;
	decString str;
	str.Format(str, vector.x, vector.y, vector.z);
	rt->PushString(str);
}



// Class deClassDVector
/////////////////////////

// Constructor, destructor
////////////////////////////

deClassDVector::deClassDVector(deEngine *gameEngine, deScriptingDragonScript *scriptManager) :
dsClass("DVector", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!gameEngine || !scriptManager) DSTHROW(dueInvalidParam);
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	
	// set parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	// do the rest
	p_SetNativeDataSize(sizeof(sDVecNatDat));
}

deClassDVector::~deClassDVector(){
}



// Management
///////////////

void deClassDVector::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	pClsVec = pScrMgr->GetClassVector();
	
	init.clsDVec = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec = pClsVec;
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	init.clsPoint3 = pScrMgr->GetClassPoint3();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfNew3(init));
	AddFunction(new nfNewFromVector(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetX(init));
	AddFunction(new nfGetY(init));
	AddFunction(new nfGetZ(init));
	AddFunction(new nfGet(init));
	AddFunction(new nfGetLength(init));
	AddFunction(new nfGetLengthSquared(init));
	AddFunction(new nfNormalize(init));
	AddFunction(new nfAbsolute(init));
	AddFunction(new nfCompMultiply(init));
	AddFunction(new nfCompDivide(init));
	AddFunction(new nfCompSelect(init));
	AddFunction(new nfCombine(init));
	AddFunction(new nfSmallest(init));
	AddFunction(new nfLargest(init));
	AddFunction(new nfClamped(init));
	AddFunction(new nfRound(init));
	AddFunction(new nfRound2(init));
	AddFunction(new nfMix(init));
	AddFunction(new nfToVector(init));
	
	AddFunction(new nfIsEqualTo(init));
	AddFunction(new nfIsAtLeast(init));
	AddFunction(new nfIsAtMost(init));
	AddFunction(new nfIsZero(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfOpMinus(init));
	AddFunction(new nfOpAdd(init));
	AddFunction(new nfOpSubtract(init));
	AddFunction(new nfOpScale(init));
	AddFunction(new nfOpDivide(init));
	AddFunction(new nfOpDot(init));
	AddFunction(new nfOpCross(init));
	AddFunction(new nfOpLess(init));
	AddFunction(new nfOpLessEqual(init));
	AddFunction(new nfOpGreater(init));
	AddFunction(new nfOpGreaterEqual(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	AddFunction(new nfToString(init));
	AddFunction(new nfToStringPrecision(init));
}

const decDVector &deClassDVector::GetDVector(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return (const decDVector &)static_cast<sDVecNatDat*>(p_GetNativeData(myself->GetBuffer()))->vector;
}

void deClassDVector::PushDVector(dsRunTime *rt, const decDVector &vector){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sDVecNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->vector = vector;
}
