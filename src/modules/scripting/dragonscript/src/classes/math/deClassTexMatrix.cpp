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

#include "deClassVector2.h"
#include "deClassTexMatrix.h"
#include "deClassTexMatrix2.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Native Structure
/////////////////////

struct sTMatNatDat{
	decTexMatrix matrix;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassTexMatrix::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsTexMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassTexMatrix::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	new (p_GetNativeData(myself)) sTMatNatDat;
}

// public func new( TexMatrix matrix )
deClassTexMatrix::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsTexMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTexMat); // matrix
}
void deClassTexMatrix::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	decTexMatrix &matrix = (new (p_GetNativeData(myself)) sTMatNatDat)->matrix;
	const deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	dsRealObject * const objMat = rt->GetValue(0)->GetRealObject();
	if(!objMat){
		DSTHROW(dueNullPointer);
	}
	
	matrix = clsTexMatrix.GetTexMatrix(objMat);
}

// public static func TexMatrix newTranslation( Vector2 translation )
deClassTexMatrix::nfNewTranslation::nfNewTranslation(const sInitData &init) : dsFunction(init.clsTexMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // translation
}
void deClassTexMatrix::nfNewTranslation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVector2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	dsRealObject * const objTranslation = rt->GetValue(0)->GetRealObject();
	if(!objTranslation){
		DSTHROW(dueNullPointer);
	}
	
	const decVector2 &translation = clsVector2.GetVector2(objTranslation);
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateTranslation(translation));
}

// public static func TexMatrix newTranslation( float u, float v )
deClassTexMatrix::nfNewTranslation2::nfNewTranslation2(const sInitData &init) : dsFunction(init.clsTexMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // u
	p_AddParameter(init.clsFlt); // v
}
void deClassTexMatrix::nfNewTranslation2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float u = rt->GetValue(0)->GetFloat();
	const float v = rt->GetValue(1)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateTranslation(u, v));
}

// public static func TexMatrix newScaling( Vector2 scaling )
deClassTexMatrix::nfNewScaling::nfNewScaling(const sInitData &init) : dsFunction(init.clsTexMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // scaling
}
void deClassTexMatrix::nfNewScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVec2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	dsRealObject * const objScaling = rt->GetValue(0)->GetRealObject();
	if(!objScaling){
		DSTHROW(dueNullPointer);
	}
	
	const decVector2 &scaling = clsVec2.GetVector2(objScaling);
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateScale(scaling));
}

// public static func TexMatrix newScaling( float u, float v )
deClassTexMatrix::nfNewScaling2::nfNewScaling2(const sInitData &init) : dsFunction(init.clsTexMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // u
	p_AddParameter(init.clsFlt); // v
}
void deClassTexMatrix::nfNewScaling2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float u = rt->GetValue(0)->GetFloat();
	const float v = rt->GetValue(1)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateScale(u, v));
}

// public static func TexMatrix newRotation( float rotation )
deClassTexMatrix::nfNewRotation::nfNewRotation(const sInitData &init) : dsFunction(init.clsTexMat,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // rotation
}
void deClassTexMatrix::nfNewRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float rotation = rt->GetValue(0)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateRotation(rotation * DEG2RAD));
}

// static func TexMatrix newShear(Vector2 shear)
deClassTexMatrix::nfNewShear::nfNewShear(const sInitData &init) :
dsFunction(init.clsTexMat, "newShear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // shear
}
void deClassTexMatrix::nfNewShear::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVec2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	const decVector2 &shear = clsVec2.GetVector2(rt->GetValue(0)->GetRealObject());
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateShear(shear));
}

// static func TexMatrix newShear(float u, float v)
deClassTexMatrix::nfNewShear2::nfNewShear2(const sInitData &init) :
dsFunction(init.clsTexMat, "newShear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // u
	p_AddParameter(init.clsFlt); // v
}
void deClassTexMatrix::nfNewShear2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float u = rt->GetValue(0)->GetFloat();
	const float v = rt->GetValue(1)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateShear(u, v));
}

// static func TexMatrix newST(Vector2 scaling, Vector2 translation)
deClassTexMatrix::nfNewST::nfNewST(const sInitData &init) :
dsFunction(init.clsTexMat, "newST", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // scaling
	p_AddParameter(init.clsVec2); // translation
}
void deClassTexMatrix::nfNewST::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVector2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2(rt->GetValue(0)->GetRealObject());
	const decVector2 &translation = clsVector2.GetVector2(rt->GetValue(1)->GetRealObject());
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateST(scaling, translation));
}

// static func TexMatrix newST(float scalingU, float scalingV, float translationU, float translationV)
deClassTexMatrix::nfNewST2::nfNewST2(const sInitData &init) :
dsFunction(init.clsTexMat, "newST", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // scalingU
	p_AddParameter(init.clsFlt); // scalingV
	p_AddParameter(init.clsFlt); // translationU
	p_AddParameter(init.clsFlt); // translationV
}
void deClassTexMatrix::nfNewST2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float scalingU = rt->GetValue(0)->GetFloat();
	const float scalingV = rt->GetValue(1)->GetFloat();
	const float translationU = rt->GetValue(2)->GetFloat();
	const float translationV = rt->GetValue(3)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateST(scalingU, scalingV, translationU, translationV));
}

// static func TexMatrix newSRT(Vector2 scaling, float rotation, Vector2 translation)
deClassTexMatrix::nfNewSRT::nfNewSRT(const sInitData &init) :
dsFunction(init.clsTexMat, "newSRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // scaling
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsVec2); // translation
}
void deClassTexMatrix::nfNewSRT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVector2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2(rt->GetValue(0)->GetRealObject());
	const float rotation = rt->GetValue(1)->GetFloat();
	const decVector2 &translation = clsVector2.GetVector2(rt->GetValue(2)->GetRealObject());
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateSRT(scaling, rotation * DEG2RAD, translation));
}

// static func TexMatrix newSRT(float scalingU, float scalingV, float rotation, float translationU, float translationV)
deClassTexMatrix::nfNewSRT2::nfNewSRT2(const sInitData &init) :
dsFunction(init.clsTexMat, "newSRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // scalingU
	p_AddParameter(init.clsFlt); // scalingV
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsFlt); // translationU
	p_AddParameter(init.clsFlt); // translationV
}
void deClassTexMatrix::nfNewSRT2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float scalingU = rt->GetValue(0)->GetFloat();
	const float scalingV = rt->GetValue(1)->GetFloat();
	const float rotation = rt->GetValue(2)->GetFloat();
	const float translationU = rt->GetValue(3)->GetFloat();
	const float translationV = rt->GetValue(4)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateSRT(
		scalingU, scalingV, rotation * DEG2RAD, translationU, translationV));
}

// static func TexMatrix newRT(float rotation, Vector2 translation)
deClassTexMatrix::nfNewRT::nfNewRT(const sInitData &init) :
dsFunction(init.clsTexMat, "newRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsVec2); // translation
}
void deClassTexMatrix::nfNewRT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVector2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	const float rotation = rt->GetValue(0)->GetFloat();
	const decVector2 &translation = clsVector2.GetVector2(rt->GetValue(1)->GetRealObject());
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateRT(rotation * DEG2RAD, translation));
}

// static func TexMatrix newRT(float rotation, float translationU, float translationV)
deClassTexMatrix::nfNewRT2::nfNewRT2(const sInitData &init) :
dsFunction(init.clsTexMat, "newRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsFlt); // translationU
	p_AddParameter(init.clsFlt); // translationV
}
void deClassTexMatrix::nfNewRT2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float rotation = rt->GetValue(0)->GetFloat();
	const float translationU = rt->GetValue(1)->GetFloat();
	const float translationV = rt->GetValue(2)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateRT(
		rotation * DEG2RAD, translationU, translationV));
}

// static func TexMatrix newCenterSRT(Vector2 scaling, float rotation, Vector2 translation)
deClassTexMatrix::nfNewCenterSRT::nfNewCenterSRT(const sInitData &init) :
dsFunction(init.clsTexMat, "newCenterSRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsVec2); // scaling
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsVec2); // translation
}
void deClassTexMatrix::nfNewCenterSRT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassVector2 &clsVector2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2(rt->GetValue(0)->GetRealObject());
	const float rotation = rt->GetValue(1)->GetFloat();
	const decVector2 &translation = clsVector2.GetVector2(rt->GetValue(2)->GetRealObject());
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateCenterSRT(scaling, rotation * DEG2RAD, translation));
}

// static func TexMatrix newCenterSRT(float scalingU, float scalingV, float rotation, float translationU, float translationV)
deClassTexMatrix::nfNewCenterSRT2::nfNewCenterSRT2(const sInitData &init) :
dsFunction(init.clsTexMat, "newCenterSRT", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // scalingU
	p_AddParameter(init.clsFlt); // scalingV
	p_AddParameter(init.clsFlt); // rotation
	p_AddParameter(init.clsFlt); // translationU
	p_AddParameter(init.clsFlt); // translationV
}
void deClassTexMatrix::nfNewCenterSRT2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float scalingU = rt->GetValue(0)->GetFloat();
	const float scalingV = rt->GetValue(1)->GetFloat();
	const float rotation = rt->GetValue(2)->GetFloat();
	const float translationU = rt->GetValue(3)->GetFloat();
	const float translationV = rt->GetValue(4)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateCenterSRT(
		scalingU, scalingV, rotation * DEG2RAD, translationU, translationV));
}

// static func TexMatrix newCenterRotation(float rotation)
deClassTexMatrix::nfNewCenterRotation::nfNewCenterRotation(const sInitData &init) :
dsFunction(init.clsTexMat, "newCenterRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // rotation
}
void deClassTexMatrix::nfNewCenterRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float rotation = rt->GetValue(0)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, decTexMatrix::CreateCenterRotation(rotation * DEG2RAD));
}

// public func destructor()
deClassTexMatrix::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsTexMat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassTexMatrix::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sTMatNatDat*>(p_GetNativeData(myself))->~sTMatNatDat();
}



// Management
///////////////

// public func float getAt( int row, int col )
deClassTexMatrix::nfGetAt::nfGetAt(const sInitData &init) : dsFunction(init.clsTexMat,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // row
	p_AddParameter(init.clsInt); // col
}
void deClassTexMatrix::nfGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	const int row = rt->GetValue(0)->GetInt();
	const int col = rt->GetValue(1)->GetInt();
	const float * const values = &matrix.a11;
	
	if(row < 0 || row > 2 || col < 0 || col > 2){
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushFloat(values[row * 3 + col]);
}

// public func float get11()
deClassTexMatrix::nfGet11::nfGet11(const sInitData &init) : dsFunction(init.clsTexMat,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet11::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a11);
}

// public func float get12()
deClassTexMatrix::nfGet12::nfGet12(const sInitData &init) : dsFunction(init.clsTexMat,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet12::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a12);
}

// public func float get13()
deClassTexMatrix::nfGet13::nfGet13(const sInitData &init) : dsFunction(init.clsTexMat,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet13::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a13);
}

// public func float get21()
deClassTexMatrix::nfGet21::nfGet21(const sInitData &init) : dsFunction(init.clsTexMat,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet21::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a21);
}

// public func float get22()
deClassTexMatrix::nfGet22::nfGet22(const sInitData &init) : dsFunction(init.clsTexMat,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet22::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a22);
}

// public func float get23()
deClassTexMatrix::nfGet23::nfGet23(const sInitData &init) : dsFunction(init.clsTexMat,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet23::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a23);
}

// public func float get31()
deClassTexMatrix::nfGet31::nfGet31(const sInitData &init) : dsFunction(init.clsTexMat,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet31::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a31);
}

// public func float get32()
deClassTexMatrix::nfGet32::nfGet32(const sInitData &init) : dsFunction(init.clsTexMat,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet32::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a32);
}

// public func float get33()
deClassTexMatrix::nfGet33::nfGet33(const sInitData &init) : dsFunction(init.clsTexMat,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGet33::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a33);
}

// func Vector2 transformNormal(Vector2 normal)
deClassTexMatrix::nfTransformNormal::nfTransformNormal(const sInitData &init) :
dsFunction(init.clsTexMat, "transformNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsVec2); // normal
}
void deClassTexMatrix::nfTransformNormal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = *clsTexMatrix.GetDS();
	deClassVector2 &clsVec2 = *ds.GetClassVector2();
	dsRealObject * const objNormal = rt->GetValue(0)->GetRealObject();
	
	const decVector2 &normal = clsVec2.GetVector2(objNormal);
	
	clsVec2.PushVector2(rt, matrix.TransformNormal(normal));
}

// func Vector2 getRightVector()
deClassTexMatrix::nfGetRightVector::nfGetRightVector(const sInitData &init) :
dsFunction(init.clsTexMat, "getRightVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
}
void deClassTexMatrix::nfGetRightVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = *clsTexMatrix.GetDS();
	deClassVector2 &clsVec2 = *ds.GetClassVector2();
	
	clsVec2.PushVector2(rt, matrix.TransformRight());
}

// func Vector2 getUpVector()
deClassTexMatrix::nfGetUpVector::nfGetUpVector(const sInitData &init) :
dsFunction(init.clsTexMat, "getUpVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
}
void deClassTexMatrix::nfGetUpVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = *clsTexMatrix.GetDS();
	deClassVector2 &clsVec2 = *ds.GetClassVector2();
	
	clsVec2.PushVector2(rt, matrix.TransformUp());
}

// func Vector2 getPosition()
deClassTexMatrix::nfGetPosition::nfGetPosition(const sInitData &init) :
dsFunction(init.clsTexMat, "getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
}
void deClassTexMatrix::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = *clsTexMatrix.GetDS();
	deClassVector2 &clsVec2 = *ds.GetClassVector2();
	
	clsVec2.PushVector2(rt, decVector2(matrix.a13, matrix.a23));
}

// func float getRotationAngle()
deClassTexMatrix::nfGetRotationAngle::nfGetRotationAngle(const sInitData &init) :
dsFunction(init.clsTexMat, "getRotationAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassTexMatrix::nfGetRotationAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.GetRotationAngle() * RAD2DEG);
}

// func Vector2 getScaling()
deClassTexMatrix::nfGetScaling::nfGetScaling(const sInitData &init) :
dsFunction(init.clsTexMat, "getScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
}
void deClassTexMatrix::nfGetScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deScriptingDragonScript &ds = *clsTexMatrix.GetDS();
	deClassVector2 &clsVec2 = *ds.GetClassVector2();
	
	clsVec2.PushVector2(rt, matrix.GetScale());
}

// func TexMatrix getInverse()
deClassTexMatrix::nfGetInverse::nfGetInverse(const sInitData &init) :
dsFunction(init.clsTexMat, "getInverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
}
void deClassTexMatrix::nfGetInverse::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	clsTexMatrix.PushTexMatrix(rt, matrix.Invert());
}

// func TexMatrix getRotation()
deClassTexMatrix::nfGetRotation::nfGetRotation(const sInitData &init) :
dsFunction(init.clsTexMat, "getRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
}
void deClassTexMatrix::nfGetRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	clsTexMatrix.PushTexMatrix(rt, matrix.GetRotationMatrix());
}

// func TexMatrix normalize()
deClassTexMatrix::nfNormalize::nfNormalize(const sInitData &init) :
dsFunction(init.clsTexMat, "normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
}
void deClassTexMatrix::nfNormalize::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	clsTexMatrix.PushTexMatrix(rt, matrix.Normalized());
}

// func TexMatrix2 toTexMatrix2()
deClassTexMatrix::nfToTexMatrix2::nfToTexMatrix2(const sInitData &init) :
dsFunction(init.clsTexMat, "toTexMatrix2", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMatrix2){
}
void deClassTexMatrix::nfToTexMatrix2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	const deScriptingDragonScript &ds = *(static_cast<deClassTexMatrix*>(GetOwnerClass()))->GetDS();
	deClassTexMatrix2 &clsTexMatrix2 = *ds.GetClassTexMatrix2();
	
	clsTexMatrix2.PushTexMatrix(rt, matrix.ToTexMatrix2());
}



// static public func TexMatrix readFromFile( FileReader reader )
deClassTexMatrix::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsTexMat,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsTexMat){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassTexMatrix::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassFileReader &clsFileReader = *clsTexMatrix.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	decTexMatrix matrix;
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	float * const values = &matrix.a11;
	int i;
	for(i=0; i<9; i++){
		values[i] = reader->ReadFloat();
	}
	
	clsTexMatrix.PushTexMatrix(rt, matrix);
}

// public func void writeToFile( FileWriter writer )
deClassTexMatrix::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsTexMat,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassTexMatrix::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	const deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const deClassFileWriter &clsFileWriter = *clsTexMatrix.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	const float * const values = &matrix.a11;
	int i;
	for(i=0; i<9; i++){
		writer->WriteFloat(values[i]);
	}
}



// Operators
//////////////

// public func TexMatrix +( TexMatrix matrix )
deClassTexMatrix::nfOpAdd::nfOpAdd(const sInitData &init) : dsFunction(init.clsTexMat,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsTexMat); // matrix
}
void deClassTexMatrix::nfOpAdd::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	dsRealObject * const objMat = rt->GetValue(0)->GetRealObject();
	if(!objMat){
		DSTHROW(dueNullPointer);
	}
	
	const decTexMatrix &otherMatrix = clsTexMatrix.GetTexMatrix(objMat);
	
	clsTexMatrix.PushTexMatrix(rt, matrix + otherMatrix);
}

// public func TexMatrix -( TexMatrix matrix )
deClassTexMatrix::nfOpSubtract::nfOpSubtract(const sInitData &init) : dsFunction(init.clsTexMat,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsTexMat); // matrix
}
void deClassTexMatrix::nfOpSubtract::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	dsRealObject * const objMat = rt->GetValue(0)->GetRealObject();
	if(!objMat){
		DSTHROW(dueNullPointer);
	}
	
	const decTexMatrix &otherMatrix = clsTexMatrix.GetTexMatrix(objMat);
	
	clsTexMatrix.PushTexMatrix(rt, matrix - otherMatrix);
}

// public func TexMatrix *( float k )
deClassTexMatrix::nfOpScale::nfOpScale(const sInitData &init) : dsFunction(init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // k
}
void deClassTexMatrix::nfOpScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float k = rt->GetValue(0)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, matrix * k);
}

// public func TexMatrix /( float k )
deClassTexMatrix::nfOpDivide::nfOpDivide(const sInitData &init) : dsFunction(init.clsTexMat,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsFlt); // k
}
void deClassTexMatrix::nfOpDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	const float k = rt->GetValue(0)->GetFloat();
	
	clsTexMatrix.PushTexMatrix(rt, matrix / k);
}

// public func TexMatrix *( TexMatrix matrix )
deClassTexMatrix::nfOpMultiply::nfOpMultiply(const sInitData &init) : dsFunction(init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat){
	p_AddParameter(init.clsTexMat); // matrix
}
void deClassTexMatrix::nfOpMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	
	dsRealObject * const objMat = rt->GetValue(0)->GetRealObject();
	if(!objMat){
		DSTHROW(dueNullPointer);
	}
	
	const decTexMatrix &otherMatrix = clsTexMatrix.GetTexMatrix(objMat);
	
	clsTexMatrix.PushTexMatrix(rt, matrix * otherMatrix);
}

// public func TexMatrix *( Vector2 texcoord )
deClassTexMatrix::nfOpMultiply2::nfOpMultiply2(const sInitData &init) : dsFunction(init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsVec2); // texcoord
}
void deClassTexMatrix::nfOpMultiply2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	const deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	deClassVector2 &clsVec2 = *clsTexMatrix.GetDS()->GetClassVector2();
	
	dsRealObject * const objTexCoord = rt->GetValue(0)->GetRealObject();
	if(!objTexCoord){
		DSTHROW(dueNullPointer);
	}
	
	const decVector2 &texcoord = clsVec2.GetVector2(objTexCoord);
	
	clsVec2.PushVector2(rt, matrix * texcoord);
}

// public func TexMatrix *( float u, float v )
deClassTexMatrix::nfOpMultiply3::nfOpMultiply3(const sInitData &init) : dsFunction(init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsFlt); // u
	p_AddParameter(init.clsFlt); // v
}
void deClassTexMatrix::nfOpMultiply3::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	const deClassTexMatrix &clsTexMatrix = *(static_cast<deClassTexMatrix*>(GetOwnerClass()));
	deClassVector2 &clsVec2 = *clsTexMatrix.GetDS()->GetClassVector2();
	const float u = rt->GetValue(0)->GetFloat();
	const float v = rt->GetValue(0)->GetFloat();
	
	clsVec2.PushVector2(rt, matrix * decVector2(u, v));
}



// public func bool equals( Object other )
deClassTexMatrix::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsTexMat,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassTexMatrix::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	deClassTexMatrix * const clsTexMatrix = static_cast<deClassTexMatrix*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsTexMatrix)){
		rt->PushBool(false);
		
	}else{
		const decTexMatrix &otherMatrix = static_cast<sTMatNatDat*>(p_GetNativeData(obj))->matrix;
		rt->PushBool(matrix.IsEqualTo(otherMatrix));
	}
}

// public func int hashCode()
deClassTexMatrix::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsTexMat,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassTexMatrix::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	
	const int hash = (int)(matrix.a11 * 0x0001)
		+ (int)(matrix.a12 * 0x0002)
		+ (int)(matrix.a13 * 0x0004)
		+ (int)(matrix.a21 * 0x0010)
		+ (int)(matrix.a22 * 0x0020)
		+ (int)(matrix.a23 * 0x0040)
		+ (int)(matrix.a31 * 0x0100)
		+ (int)(matrix.a32 * 0x0200)
		+ (int)(matrix.a33 * 0x0400);
	
	rt->PushInt(hash);
}

// public func String toString()
deClassTexMatrix::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsTexMat,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassTexMatrix::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format("[[%g,%g,%g],[%g,%g,%g],[%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13,
		matrix.a21, matrix.a22, matrix.a23,
		matrix.a31, matrix.a32, matrix.a33);
	
	rt->PushString(str);
}

// public func String toString( int precision )
deClassTexMatrix::nfToStringPrecision::nfToStringPrecision(const sInitData &init) :
dsFunction(init.clsTexMat, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // precision
}
void deClassTexMatrix::nfToStringPrecision::RunFunction(dsRunTime *rt, dsValue *myself){
	const int precision = rt->GetValue(0)->GetInt();
	if(precision < 0){
		DSTHROW_INFO(dueInvalidParam, "precision < 0");
	}
	if(precision > 9){
		DSTHROW_INFO(dueInvalidParam, "precision > 9");
	}
	
	const unsigned short p = (unsigned short)precision;
	char format[53];
	snprintf(format, sizeof(format), "[[%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf]]",
		p, p, p, p, p, p, p, p, p);
	
	const decTexMatrix &matrix = static_cast<sTMatNatDat*>(p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format(format,
		matrix.a11, matrix.a12, matrix.a13,
		matrix.a21, matrix.a22, matrix.a23,
		matrix.a31, matrix.a32, matrix.a33);
	
	rt->PushString(str);
}



// Class deClassTexMatrix
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassTexMatrix::deClassTexMatrix(deScriptingDragonScript *ds) :
dsClass("TexMatrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sTMatNatDat));
}

deClassTexMatrix::~deClassTexMatrix(){
}



// Management
///////////////

void deClassTexMatrix::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	memset(&init, '\0', sizeof(init));
	
	init.clsTexMat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec2 = pDS->GetClassVector2();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	init.clsTexMatrix2 = pDS->GetClassTexMatrix2();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfNewTranslation(init));
	AddFunction(new nfNewTranslation2(init));
	AddFunction(new nfNewScaling(init));
	AddFunction(new nfNewScaling2(init));
	AddFunction(new nfNewRotation(init));
	AddFunction(new nfNewShear(init));
	AddFunction(new nfNewShear2(init));
	AddFunction(new nfNewST(init));
	AddFunction(new nfNewST2(init));
	AddFunction(new nfNewSRT(init));
	AddFunction(new nfNewSRT2(init));
	AddFunction(new nfNewRT(init));
	AddFunction(new nfNewRT2(init));
	AddFunction(new nfNewCenterSRT(init));
	AddFunction(new nfNewCenterSRT2(init));
	AddFunction(new nfNewCenterRotation(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAt(init));
	AddFunction(new nfGet11(init));
	AddFunction(new nfGet12(init));
	AddFunction(new nfGet13(init));
	AddFunction(new nfGet21(init));
	AddFunction(new nfGet22(init));
	AddFunction(new nfGet23(init));
	AddFunction(new nfGet31(init));
	AddFunction(new nfGet32(init));
	AddFunction(new nfGet33(init));
	AddFunction(new nfTransformNormal(init));
	AddFunction(new nfGetRightVector(init));
	AddFunction(new nfGetUpVector(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfGetRotationAngle(init));
	AddFunction(new nfGetScaling(init));
	AddFunction(new nfGetInverse(init));
	AddFunction(new nfGetRotation(init));
	AddFunction(new nfNormalize(init));
	AddFunction(new nfToTexMatrix2(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfOpAdd(init));
	AddFunction(new nfOpSubtract(init));
	AddFunction(new nfOpScale(init));
	AddFunction(new nfOpDivide(init));
	AddFunction(new nfOpMultiply(init));
	AddFunction(new nfOpMultiply2(init));
	AddFunction(new nfOpMultiply3(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	AddFunction(new nfToString(init));
	AddFunction(new nfToStringPrecision(init));
}

const decTexMatrix &deClassTexMatrix::GetTexMatrix(dsRealObject *myself) const{
	return (const decTexMatrix &)static_cast<sTMatNatDat*>(p_GetNativeData(myself->GetBuffer()))->matrix;
}

void deClassTexMatrix::PushTexMatrix(dsRunTime *rt, const decTexMatrix &matrix){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sTMatNatDat)->matrix = matrix;
}
