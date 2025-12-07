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

#include "deClassDMatrix.h"
#include "deClassDMatrix4.h"
#include "deClassMatrix.h"
#include "deClassMatrix4.h"
#include "deClassVector.h"
#include "deClassQuaternion.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Native structure
/////////////////////

struct sMatNatDat{
	decMatrix matrix;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassMatrix::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsMatrix,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassMatrix::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	matrix.SetIdentity();
}

// public func new( float a11, float a12, float a13, float a14, float a21,
// float a22, float a23, float a24, float a31, float a32, float a33, float a34 )
deClassMatrix::nfNewValues::nfNewValues(const sInitData &init) : dsFunction(init.clsMatrix,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // a11
	p_AddParameter(init.clsFlt); // a12
	p_AddParameter(init.clsFlt); // a13
	p_AddParameter(init.clsFlt); // a14
	p_AddParameter(init.clsFlt); // a21
	p_AddParameter(init.clsFlt); // a22
	p_AddParameter(init.clsFlt); // a23
	p_AddParameter(init.clsFlt); // a24
	p_AddParameter(init.clsFlt); // a31
	p_AddParameter(init.clsFlt); // a32
	p_AddParameter(init.clsFlt); // a33
	p_AddParameter(init.clsFlt); // a34
}
void deClassMatrix::nfNewValues::RunFunction(dsRunTime *rt, dsValue *myself){
	decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	matrix.a11 = rt->GetValue(0)->GetFloat();
	matrix.a12 = rt->GetValue(1)->GetFloat();
	matrix.a13 = rt->GetValue(2)->GetFloat();
	matrix.a14 = rt->GetValue(3)->GetFloat();
	matrix.a21 = rt->GetValue(4)->GetFloat();
	matrix.a22 = rt->GetValue(5)->GetFloat();
	matrix.a23 = rt->GetValue(6)->GetFloat();
	matrix.a24 = rt->GetValue(7)->GetFloat();
	matrix.a31 = rt->GetValue(8)->GetFloat();
	matrix.a32 = rt->GetValue(9)->GetFloat();
	matrix.a33 = rt->GetValue(10)->GetFloat();
	matrix.a34 = rt->GetValue(11)->GetFloat();
	matrix.a41 = 0.0f;
	matrix.a42 = 0.0f;
	matrix.a43 = 0.0f;
	matrix.a44 = 1.0f;
}

// public static func Matrix newTranslation( Vector translation )
deClassMatrix::nfNewTranslation::nfNewTranslation(const sInitData &init) : dsFunction(init.clsMatrix,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // translation
}
void deClassMatrix::nfNewTranslation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objTranslation = rt->GetValue(0)->GetRealObject();
	
	const decVector &translation = clsVec.GetVector(objTranslation);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateTranslation(translation));
}

// public static func Matrix newTranslation( float tx, float ty, float tz )
deClassMatrix::nfNewTranslation2::nfNewTranslation2(const sInitData &init) : dsFunction(init.clsMatrix,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // tx
	p_AddParameter(init.clsFlt); // ty
	p_AddParameter(init.clsFlt); // tz
}
void deClassMatrix::nfNewTranslation2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float tx = rt->GetValue(0)->GetFloat();
	const float ty = rt->GetValue(1)->GetFloat();
	const float tz = rt->GetValue(2)->GetFloat();
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateTranslation(tx, ty, tz));
}

// public static func Matrix newScaling( Vector scaling )
deClassMatrix::nfNewScaling::nfNewScaling(const sInitData &init) : dsFunction(init.clsMatrix,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // scaling
}
void deClassMatrix::nfNewScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue(0)->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector(objScaling);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateScale(scaling));
}

// public static func Matrix newScaling( float sx, float sy, float sz )
deClassMatrix::nfNewScaling2::nfNewScaling2(const sInitData &init) : dsFunction(init.clsMatrix,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // sx
	p_AddParameter(init.clsFlt); // sy
	p_AddParameter(init.clsFlt); // sz
}
void deClassMatrix::nfNewScaling2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float sx = rt->GetValue(0)->GetFloat();
	const float sy = rt->GetValue(1)->GetFloat();
	const float sz = rt->GetValue(2)->GetFloat();
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateScale(sx, sy, sz));
}

// public static func Matrix newRotationX( float rotation )
deClassMatrix::nfNewRotationX::nfNewRotationX(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotationX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // rotation
}
void deClassMatrix::nfNewRotationX::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float rotation = rt->GetValue(0)->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotationX(rotation));
}

// public static func Matrix newRotationY( float rotation )
deClassMatrix::nfNewRotationY::nfNewRotationY(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotationY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // rotation
}
void deClassMatrix::nfNewRotationY::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float rotation = rt->GetValue(0)->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotationY(rotation));
}

// public static func Matrix newRotationZ( float rotation )
deClassMatrix::nfNewRotationZ::nfNewRotationZ(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotationZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // rotation
}
void deClassMatrix::nfNewRotationZ::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float rotation = rt->GetValue(0)->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotationZ(rotation));
}

// public static func Matrix newRotation( Vector rotation )
deClassMatrix::nfNewRotation::nfNewRotation(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // rotation
}
void deClassMatrix::nfNewRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objRotation = rt->GetValue(0)->GetRealObject();
	
	const decVector rotation = clsVec.GetVector(objRotation) * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotation(rotation));
}

// public static func Matrix newRotation( float rx, float ry, float rz )
deClassMatrix::nfNewRotation2::nfNewRotation2(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // rx
	p_AddParameter(init.clsFlt); // ry
	p_AddParameter(init.clsFlt); // rz
}
void deClassMatrix::nfNewRotation2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float rx = rt->GetValue(0)->GetFloat() * DEG2RAD;
	const float ry = rt->GetValue(1)->GetFloat() * DEG2RAD;
	const float rz = rt->GetValue(2)->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotation(rx, ry, rz));
}

// public static func Matrix newRotationAxis( Vector axis, float rotation )
deClassMatrix::nfNewRotationAxis::nfNewRotationAxis(const sInitData &init) : dsFunction(init.clsMatrix,
"newRotationAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // axis
	p_AddParameter(init.clsFlt); // rotation
}
void deClassMatrix::nfNewRotationAxis::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	
	const decVector &axis = clsVec.GetVector(rt->GetValue(0)->GetRealObject());
	const float rotation = rt->GetValue(1)->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRotationAxis(axis, rotation));
}

// public static func Matrix newSRT( Vector scaling, Vector rotation, Vector translation )
deClassMatrix::nfNewSRT::nfNewSRT(const sInitData &init) : dsFunction(init.clsMatrix,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // scaling
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsVec); // translation
}
void deClassMatrix::nfNewSRT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objRotation = rt->GetValue(1)->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue(2)->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector(objScaling);
	const decVector rotation = clsVec.GetVector(objRotation) * DEG2RAD;
	const decVector &translation = clsVec.GetVector(objTranslation);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateSRT(scaling, rotation, translation));
}

// public static func Matrix newRT( Vector rotation, Vector translation )
deClassMatrix::nfNewRT::nfNewRT(const sInitData &init) : dsFunction(init.clsMatrix,
"newRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsVec); // translation
}
void deClassMatrix::nfNewRT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objRotation = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue(1)->GetRealObject();
	
	const decVector rotation = clsVec.GetVector(objRotation) * DEG2RAD;
	const decVector &translation = clsVec.GetVector(objTranslation);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateRT(rotation, translation));
}

// public static func Matrix newSVUT( Vector scaling, Vector view, Vector up, Vector translation )
deClassMatrix::nfNewSVUT::nfNewSVUT(const sInitData &init) : dsFunction(init.clsMatrix,
"newSVUT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // rotation
	p_AddParameter(init.clsVec); // view
	p_AddParameter(init.clsVec); // up
	p_AddParameter(init.clsVec); // translation
}
void deClassMatrix::nfNewSVUT::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objView = rt->GetValue(1)->GetRealObject();
	dsRealObject * const objUp = rt->GetValue(2)->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue(3)->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector(objScaling);
	const decVector &view = clsVec.GetVector(objView);
	const decVector &up = clsVec.GetVector(objUp);
	const decVector &translation = clsVec.GetVector(objTranslation);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateSVUT(scaling, view, up, translation));
}

// public static func Matrix newVU( Vector view, Vector up )
deClassMatrix::nfNewVU::nfNewVU(const sInitData &init) : dsFunction(init.clsMatrix,
"newVU", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // view
	p_AddParameter(init.clsVec); // up
}
void deClassMatrix::nfNewVU::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objView = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objUp = rt->GetValue(1)->GetRealObject();
	
	const decVector &view = clsVec.GetVector(objView);
	const decVector &up = clsVec.GetVector(objUp);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateVU(view, up));
}

// public static func Matrix newCamera( Vector position, Vector view, Vector up )
deClassMatrix::nfNewCamera::nfNewCamera(const sInitData &init) : dsFunction(init.clsMatrix,
"newCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsVec); // view
	p_AddParameter(init.clsVec); // up
}
void deClassMatrix::nfNewCamera::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objPosition = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objView = rt->GetValue(1)->GetRealObject();
	dsRealObject * const objUp = rt->GetValue(2)->GetRealObject();
	
	const decVector &position = clsVec.GetVector(objPosition);
	const decVector &view = clsVec.GetVector(objView);
	const decVector &up = clsVec.GetVector(objUp);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateCamera(position, view, up));
}

// public static func Matrix newWorld( Vector position, Vector view, Vector up )
deClassMatrix::nfNewWorld::nfNewWorld(const sInitData &init) : dsFunction(init.clsMatrix,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsVec); // view
	p_AddParameter(init.clsVec); // up
}
void deClassMatrix::nfNewWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objPosition = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objView = rt->GetValue(1)->GetRealObject();
	dsRealObject * const objUp = rt->GetValue(2)->GetRealObject();
	
	const decVector &position = clsVec.GetVector(objPosition);
	const decVector &view = clsVec.GetVector(objView);
	const decVector &up = clsVec.GetVector(objUp);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateWorld(position, view, up));
}

// public static func Matrix newWorld( Vector position, Quaternion orientation )
deClassMatrix::nfNewWorld2::nfNewWorld2(const sInitData &init) : dsFunction(init.clsMatrix,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsVec); // position
	p_AddParameter(init.clsQuat); // orientation
}
void deClassMatrix::nfNewWorld2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objPosition = rt->GetValue(0)->GetRealObject();
	dsRealObject * const objOrientation = rt->GetValue(1)->GetRealObject();
	
	const decVector &position = clsVec.GetVector(objPosition);
	const decQuaternion &orientation = clsQuat.GetQuaternion(objOrientation);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateWorld(position, orientation));
}

// public static func Matrix newFromQuaternion( Quaternion quaternion )
deClassMatrix::nfNewFromQuaternion::nfNewFromQuaternion(const sInitData &init) : dsFunction(init.clsMatrix,
"newFromQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsQuat); // quaternion
}
void deClassMatrix::nfNewFromQuaternion::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objQuaternion = rt->GetValue(0)->GetRealObject();
	
	const decQuaternion &quaternion = clsQuat.GetQuaternion(objQuaternion);
	
	clsMatrix.PushMatrix(rt, decMatrix::CreateFromQuaternion(quaternion));
}

// public func destructor()
deClassMatrix::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsMatrix,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassMatrix::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
}



// Accessors
//////////////

// public func float getAt( int row, int col )
deClassMatrix::nfGetAt::nfGetAt(const sInitData &init) : dsFunction(init.clsMatrix,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // row
	p_AddParameter(init.clsInt); // col
}
void deClassMatrix::nfGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const int row = rt->GetValue(0)->GetInt();
	const int col = rt->GetValue(1)->GetInt();
	
	if(row == 0){
		if(col == 0){
			rt->PushFloat(matrix.a11);
			
		}else if(col == 1){
			rt->PushFloat(matrix.a12);
			
		}else if(col == 2){
			rt->PushFloat(matrix.a13);
			
		}else if(col == 3){
			rt->PushFloat(matrix.a14);
			
		}else{
			DSTHROW(dueInvalidParam);
		}
		
	}else if(row == 1){
		if(col == 0){
			rt->PushFloat(matrix.a21);
			
		}else if(col == 1){
			rt->PushFloat(matrix.a22);
			
		}else if(col == 2){
			rt->PushFloat(matrix.a23);
			
		}else if(col == 3){
			rt->PushFloat(matrix.a24);
			
		}else{
			DSTHROW(dueInvalidParam);
		}
		
	}else if(row == 2){
		if(col == 0){
			rt->PushFloat(matrix.a31);
			
		}else if(col == 1){
			rt->PushFloat(matrix.a32);
			
		}else if(col == 2){
			rt->PushFloat(matrix.a33);
			
		}else if(col == 3){
			rt->PushFloat(matrix.a34);
			
		}else{
			DSTHROW(dueInvalidParam);
		}
		
	}else{
		DSTHROW(dueInvalidParam);
	}
}

// public func float get11()
deClassMatrix::nfGet11::nfGet11(const sInitData &init) : dsFunction(init.clsMatrix,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet11::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a11);
}

// public func float get12()
deClassMatrix::nfGet12::nfGet12(const sInitData &init) : dsFunction(init.clsMatrix,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet12::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a12);
}

// public func float get13()
deClassMatrix::nfGet13::nfGet13(const sInitData &init) : dsFunction(init.clsMatrix,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet13::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a13);
}

// public func float get14()
deClassMatrix::nfGet14::nfGet14(const sInitData &init) : dsFunction(init.clsMatrix,
"get14", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet14::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a14);
}

// public func float get21()
deClassMatrix::nfGet21::nfGet21(const sInitData &init) : dsFunction(init.clsMatrix,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet21::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a21);
}

// public func float get22()
deClassMatrix::nfGet22::nfGet22(const sInitData &init) : dsFunction(init.clsMatrix,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet22::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a22);
}

// public func float get23()
deClassMatrix::nfGet23::nfGet23(const sInitData &init) : dsFunction(init.clsMatrix,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet23::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a23);
}

// public func float get24()
deClassMatrix::nfGet24::nfGet24(const sInitData &init) : dsFunction(init.clsMatrix,
"get24", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet24::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a24);
}

// public func float get31()
deClassMatrix::nfGet31::nfGet31(const sInitData &init) : dsFunction(init.clsMatrix,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet31::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a31);
}

// public func float get32()
deClassMatrix::nfGet32::nfGet32(const sInitData &init) : dsFunction(init.clsMatrix,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet32::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a32);
}

// public func float get33()
deClassMatrix::nfGet33::nfGet33(const sInitData &init) : dsFunction(init.clsMatrix,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet33::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a33);
}

// public func float get34()
deClassMatrix::nfGet34::nfGet34(const sInitData &init) : dsFunction(init.clsMatrix,
"get34", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassMatrix::nfGet34::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a34);
}

// public func Vector getViewVector()
deClassMatrix::nfGetViewVector::nfGetViewVector(const sInitData &init) : dsFunction(init.clsMatrix,
"getViewVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetViewVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, decVector(matrix.a13, matrix.a23, matrix.a33));
}

// public func Vector getUpVector()
deClassMatrix::nfGetUpVector::nfGetUpVector(const sInitData &init) : dsFunction(init.clsMatrix,
"getUpVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetUpVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, decVector(matrix.a12, matrix.a22, matrix.a32));
}

// public func Vector getRightVector()
deClassMatrix::nfGetRightVector::nfGetRightVector(const sInitData &init) : dsFunction(init.clsMatrix,
"getRightVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetRightVector::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, decVector(matrix.a11, matrix.a21, matrix.a31));
}

// public func Vector getPosition()
deClassMatrix::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsMatrix,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, decVector(matrix.a14, matrix.a24, matrix.a34));
}

// public func Vector transformNormal( Vector normal )
deClassMatrix::nfTransformNormal::nfTransformNormal(const sInitData &init) : dsFunction(init.clsMatrix,
"transformNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsVec); // normal
}
void deClassMatrix::nfTransformNormal::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objNormal = rt->GetValue(0)->GetRealObject();
	
	const decVector &normal = clsVec.GetVector(objNormal);
	
	clsVec.PushVector(rt, matrix.TransformNormal(normal));
}

// public func Vector getEulerAngles()
deClassMatrix::nfGetEulerAngles::nfGetEulerAngles(const sInitData &init) : dsFunction(init.clsMatrix,
"getEulerAngles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetEulerAngles::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, matrix.GetEulerAngles() / DEG2RAD);
}

// public func Vector getScaling()
deClassMatrix::nfGetScaling::nfGetScaling(const sInitData &init) : dsFunction(
init.clsMatrix, "getScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassMatrix::nfGetScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector(rt, matrix.GetScale());
}

// public func Matrix getInverse()
deClassMatrix::nfGetInverse::nfGetInverse(const sInitData &init) : dsFunction(init.clsMatrix,
"getInverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
}
void deClassMatrix::nfGetInverse::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	
	clsMatrix.PushMatrix(rt, matrix.QuickInvert());
}

// public func Matrix getRotation()
deClassMatrix::nfGetRotation::nfGetRotation(const sInitData &init) :
dsFunction(init.clsMatrix, "getRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
}
void deClassMatrix::nfGetRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	
	clsMatrix.PushMatrix(rt, matrix.GetRotationMatrix());
}

// public func Matrix normalize()
deClassMatrix::nfNormalize::nfNormalize(const sInitData &init) :
dsFunction(init.clsMatrix, "normalize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
}
void deClassMatrix::nfNormalize::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	clsMatrix.PushMatrix(rt, matrix.Normalized());
}

// public func Quaternion toQuaternion()
deClassMatrix::nfToQuaternion::nfToQuaternion(const sInitData &init) : dsFunction(init.clsMatrix,
"toQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassMatrix::nfToQuaternion::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	
	clsQuat.PushQuaternion(rt, matrix.ToQuaternion());
}

// public func DMatrix4 toDMatrix4()
deClassMatrix::nfToDMatrix4::nfToDMatrix4(const sInitData &init) : dsFunction(init.clsMatrix,
"toDMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4){
}
void deClassMatrix::nfToDMatrix4::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const deScriptingDragonScript &ds = *((deClassMatrix*)GetOwnerClass())->GetDS();
	deClassDMatrix4 &clsDMatrix4 = *ds.GetClassDMatrix4();
	
	clsDMatrix4.PushDMatrix4(rt, decDMatrix(matrix));
}

// public func DMatrix toDMatrix()
deClassMatrix::nfToDMatrix::nfToDMatrix(const sInitData &init) : dsFunction(init.clsMatrix,
"toDMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix){
}
void deClassMatrix::nfToDMatrix::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const deScriptingDragonScript &ds = *((deClassMatrix*)GetOwnerClass())->GetDS();
	deClassDMatrix &clsDMatrix = *ds.GetClassDMatrix();
	
	clsDMatrix.PushDMatrix(rt, decDMatrix(matrix));
}

// public func Matrix4 toMatrix4()
deClassMatrix::nfToMatrix4::nfToMatrix4(const sInitData &init) : dsFunction(init.clsMatrix,
"toMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4){
}
void deClassMatrix::nfToMatrix4::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const deScriptingDragonScript &ds = *((deClassMatrix*)GetOwnerClass())->GetDS();
	deClassMatrix4 &clsMatrix4 = *ds.GetClassMatrix4();
	
	clsMatrix4.PushMatrix4(rt, matrix);
}



// File Handling
//////////////////

// static public func Matrix readFromFile( FileReader reader )
deClassMatrix::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsMatrix,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsMatrix){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassMatrix::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsMatrix.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	decMatrix matrix;
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	float * const values = &matrix.a11;
	int i;
	for(i=0; i<16; i++){
		values[i] = reader->ReadFloat();
	}
	
	clsMatrix.PushMatrix(rt, matrix);
}

// public func void writeToFile( FileWriter writer )
deClassMatrix::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsMatrix,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassMatrix::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsMatrix.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	const float * const values = &matrix.a11;
	int i;
	for(i=0; i<16; i++){
		writer->WriteFloat(values[i]);
	}
}



// Operators
//////////////

// public func Matrix +( Matrix marix )
deClassMatrix::nfOpAdd::nfOpAdd(const sInitData &init) : dsFunction(init.clsMatrix,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsMatrix); // matrix
}
void deClassMatrix::nfOpAdd::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	dsRealObject * const objMatrix = rt->GetValue(0)->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix(objMatrix);
	
	clsMatrix.PushMatrix(rt, matrix + otherMatrix);
}

// public func Matrix -( Matrix matrix )
deClassMatrix::nfOpSubtract::nfOpSubtract(const sInitData &init) : dsFunction(init.clsMatrix,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsMatrix); // matrix
}
void deClassMatrix::nfOpSubtract::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	dsRealObject * const objMatrix = rt->GetValue(0)->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix(objMatrix);
	
	clsMatrix.PushMatrix(rt, matrix - otherMatrix);
}

// public func Matrix *( float k )
deClassMatrix::nfOpScale::nfOpScale(const sInitData &init) : dsFunction(init.clsMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // k
}
void deClassMatrix::nfOpScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float k = rt->GetValue(0)->GetFloat();
	
	clsMatrix.PushMatrix(rt, matrix.QuickMultiply(k));
}

// public func Matrix /( float k )
deClassMatrix::nfOpDivide::nfOpDivide(const sInitData &init) : dsFunction(init.clsMatrix,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsFlt); // k
}
void deClassMatrix::nfOpDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const float k = rt->GetValue(0)->GetFloat();
	
	clsMatrix.PushMatrix(rt, matrix / k);
}

// public func Matrix *( Matrix matrix )
deClassMatrix::nfOpMultiply::nfOpMultiply(const sInitData &init) : dsFunction(init.clsMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix){
	p_AddParameter(init.clsMatrix); // matrix
}
void deClassMatrix::nfOpMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	dsRealObject * const objMatrix = rt->GetValue(0)->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix(objMatrix);
	
	clsMatrix.PushMatrix(rt, matrix.QuickMultiply(otherMatrix));
}

// public func Matrix *( Vector vector )
deClassMatrix::nfOpMultiply2::nfOpMultiply2(const sInitData &init) : dsFunction(init.clsMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
	p_AddParameter(init.clsVec); // vector
}
void deClassMatrix::nfOpMultiply2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix &clsMatrix = *((deClassMatrix*)GetOwnerClass());
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objVector = rt->GetValue(0)->GetRealObject();
	
	const decVector &vector = clsVec.GetVector(objVector);
	
	clsVec.PushVector(rt, matrix * vector);
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassMatrix::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsMatrix,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassMatrix::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	deClassMatrix * const clsMatrix = (deClassMatrix*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsMatrix)){
		rt->PushBool(false);
		
	}else{
		const decMatrix &otherMatrix = ((sMatNatDat*)p_GetNativeData(obj))->matrix;
		const bool equal = matrix.a11 == otherMatrix.a11
			&& matrix.a12 == otherMatrix.a12
			&& matrix.a13 == otherMatrix.a13
			&& matrix.a14 == otherMatrix.a14
			&& matrix.a21 == otherMatrix.a21
			&& matrix.a22 == otherMatrix.a22
			&& matrix.a23 == otherMatrix.a23
			&& matrix.a24 == otherMatrix.a24
			&& matrix.a31 == otherMatrix.a31
			&& matrix.a32 == otherMatrix.a32
			&& matrix.a33 == otherMatrix.a33
			&& matrix.a34 == otherMatrix.a34;
		rt->PushBool(equal);
	}
}

// public func int hashCode()
deClassMatrix::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsMatrix,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassMatrix::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	const int hash = (int)(matrix.a11 * 0x0001)
		+ (int)(matrix.a12 * 0x0002)
		+ (int)(matrix.a13 * 0x0004)
		+ (int)(matrix.a14 * 0x0008)
		+ (int)(matrix.a21 * 0x0010)
		+ (int)(matrix.a22 * 0x0020)
		+ (int)(matrix.a23 * 0x0040)
		+ (int)(matrix.a24 * 0x0080)
		+ (int)(matrix.a31 * 0x0100)
		+ (int)(matrix.a32 * 0x0200)
		+ (int)(matrix.a33 * 0x0400)
		+ (int)(matrix.a34 * 0x0800);
	rt->PushInt(hash);
}

// public func String toString()
deClassMatrix::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsMatrix,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassMatrix::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format("[[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13, matrix.a14,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34,
		matrix.a41, matrix.a42, matrix.a43, matrix.a44);
	
	rt->PushString(str);
}

// public func String toString()
deClassMatrix::nfToStringPrecision::nfToStringPrecision(const sInitData &init) :
dsFunction(init.clsMatrix, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // precision
}
void deClassMatrix::nfToStringPrecision::RunFunction(dsRunTime *rt, dsValue *myself){
	const int precision = rt->GetValue(0)->GetInt();
	if(precision < 0){
		DSTHROW_INFO(dueInvalidParam, "precision < 0");
	}
	if(precision > 9){
		DSTHROW_INFO(dueInvalidParam, "precision > 9");
	}
	
	const unsigned short p = (unsigned short)precision;
	char format[90];
	snprintf(format, sizeof(format), "[[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf]]",
		p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p);
	
	const decMatrix &matrix = ((sMatNatDat*)p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format(format,
		matrix.a11, matrix.a12, matrix.a13, matrix.a14,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34,
		matrix.a41, matrix.a42, matrix.a43, matrix.a44);
	
	rt->PushString(str);
}



// Class deClassMatrix
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassMatrix::deClassMatrix(deScriptingDragonScript *ds) :
dsClass("Matrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMatNatDat));
}

deClassMatrix::~deClassMatrix(){
}



// Management
///////////////

void deClassMatrix::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsMatrix = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsStr = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	init.clsDMatrix4 = pDS->GetClassDMatrix4();
	init.clsDMatrix = pDS->GetClassDMatrix();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	init.clsMatrix4 = pDS->GetClassMatrix4();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsVec = pDS->GetClassVector();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewValues(init));
	AddFunction(new nfNewTranslation(init));
	AddFunction(new nfNewTranslation2(init));
	AddFunction(new nfNewScaling(init));
	AddFunction(new nfNewScaling2(init));
	AddFunction(new nfNewRotationX(init));
	AddFunction(new nfNewRotationY(init));
	AddFunction(new nfNewRotationZ(init));
	AddFunction(new nfNewRotation(init));
	AddFunction(new nfNewRotation2(init));
	AddFunction(new nfNewRotationAxis(init));
	AddFunction(new nfNewSRT(init));
	AddFunction(new nfNewRT(init));
	AddFunction(new nfNewSVUT(init));
	AddFunction(new nfNewVU(init));
	AddFunction(new nfNewCamera(init));
	AddFunction(new nfNewWorld(init));
	AddFunction(new nfNewWorld2(init));
	AddFunction(new nfNewFromQuaternion(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAt(init));
	AddFunction(new nfGet11(init));
	AddFunction(new nfGet12(init));
	AddFunction(new nfGet13(init));
	AddFunction(new nfGet14(init));
	AddFunction(new nfGet21(init));
	AddFunction(new nfGet22(init));
	AddFunction(new nfGet23(init));
	AddFunction(new nfGet24(init));
	AddFunction(new nfGet31(init));
	AddFunction(new nfGet32(init));
	AddFunction(new nfGet33(init));
	AddFunction(new nfGet34(init));
	AddFunction(new nfGetViewVector(init));
	AddFunction(new nfGetUpVector(init));
	AddFunction(new nfGetRightVector(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfTransformNormal(init));
	AddFunction(new nfGetEulerAngles(init));
	AddFunction(new nfGetScaling(init));
	AddFunction(new nfGetInverse(init));
	AddFunction(new nfGetRotation(init));
	AddFunction(new nfNormalize(init));
	AddFunction(new nfToQuaternion(init));
	AddFunction(new nfToDMatrix4(init));
	AddFunction(new nfToDMatrix(init));
	AddFunction(new nfToMatrix4(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfOpAdd(init));
	AddFunction(new nfOpSubtract(init));
	AddFunction(new nfOpScale(init));
	AddFunction(new nfOpDivide(init));
	AddFunction(new nfOpMultiply(init));
	AddFunction(new nfOpMultiply2(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	AddFunction(new nfToString(init));
	AddFunction(new nfToStringPrecision(init));
}

const decMatrix &deClassMatrix::GetMatrix(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return (const decMatrix &)((sMatNatDat*)p_GetNativeData(myself->GetBuffer()))->matrix;
}

void deClassMatrix::PushMatrix(dsRunTime *rt, const decMatrix &matrix){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sMatNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->matrix = matrix;
}
