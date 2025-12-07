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

#include "deClassColorMatrix.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../graphics/deClassColor.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sCMNatDat{
	decColorMatrix matrix;
};



// Native Functions
/////////////////////

// public func new()
deClassColorMatrix::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsClrMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColorMatrix::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	matrix = decColorMatrix::CreateIdentity();
}

// public func new( ColorMatrix matrix )
deClassColorMatrix::nfNew2::nfNew2(const sInitData &init) : dsFunction(init.clsClrMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsClrMat); // matrix
}
void deClassColorMatrix::nfNew2::RunFunction(dsRunTime *rt, dsValue *myself){
	decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix *clsClrMatrix = (deClassColorMatrix*)GetOwnerClass();
	
	matrix = clsClrMatrix->GetColorMatrix(rt->GetValue(0)->GetRealObject());
}

// public func new( Color red, Color green, Color blue, Color alpha, Color white )
deClassColorMatrix::nfNew3::nfNew3(const sInitData &init) : dsFunction(init.clsClrMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsClr); // red
	p_AddParameter(init.clsClr); // green
	p_AddParameter(init.clsClr); // blue
	p_AddParameter(init.clsClr); // alpha
	p_AddParameter(init.clsClr); // white
}
void deClassColorMatrix::nfNew3::RunFunction(dsRunTime *rt, dsValue *myself){
	decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	const deScriptingDragonScript &ds = ((deClassColorMatrix*)GetOwnerClass())->GetDS();
	deClassColor &clsClr = *ds.GetClassColor();
	
	const decColor &red = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	const decColor &green = clsClr.GetColor(rt->GetValue(1)->GetRealObject());
	const decColor &blue = clsClr.GetColor(rt->GetValue(2)->GetRealObject());
	const decColor &alpha = clsClr.GetColor(rt->GetValue(3)->GetRealObject());
	const decColor &white = clsClr.GetColor(rt->GetValue(4)->GetRealObject());
	
	matrix.SetFrom(red, green, blue, alpha, white);
}

// public static func ColorMatrix newTranslation( Color color )
deClassColorMatrix::nfNewTranslation::nfNewTranslation(const sInitData &init) : dsFunction(init.clsClrMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // color
}
void deClassColorMatrix::nfNewTranslation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &color = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateTranslation(color));
}

// public static func ColorMatrix newTranslation( float red, float green, float blue, float alpha )
deClassColorMatrix::nfNewTranslation2::nfNewTranslation2(const sInitData &init) : dsFunction(init.clsClrMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // red
	p_AddParameter(init.clsFlt); // green
	p_AddParameter(init.clsFlt); // blue
	p_AddParameter(init.clsFlt); // alpha
}
void deClassColorMatrix::nfNewTranslation2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float red = rt->GetValue(0)->GetFloat();
	const float green = rt->GetValue(1)->GetFloat();
	const float blue = rt->GetValue(2)->GetFloat();
	const float alpha = rt->GetValue(3)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateTranslation(red, green, blue, alpha));
}

// public static func ColorMatrix newScaling( Color color )
deClassColorMatrix::nfNewScaling::nfNewScaling(const sInitData &init) : dsFunction(init.clsClrMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // color
}
void deClassColorMatrix::nfNewScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &color = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateScaling(color));
}

// public static func ColorMatrix newScaling( float red, float green, float blue, float alpha )
deClassColorMatrix::nfNewScaling2::nfNewScaling2(const sInitData &init) : dsFunction(init.clsClrMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // red
	p_AddParameter(init.clsFlt); // green
	p_AddParameter(init.clsFlt); // blue
	p_AddParameter(init.clsFlt); // alpha
}
void deClassColorMatrix::nfNewScaling2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float red = rt->GetValue(0)->GetFloat();
	const float green = rt->GetValue(1)->GetFloat();
	const float blue = rt->GetValue(2)->GetFloat();
	const float alpha = rt->GetValue(3)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateScaling(red, green, blue, alpha));
}

// public static func ColorMatrix newST( Color scaling, Color translation )
deClassColorMatrix::nfNewST::nfNewST(const sInitData &init) : dsFunction(init.clsClrMat,
"newST", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // scaling
	p_AddParameter(init.clsClr); // translation
}
void deClassColorMatrix::nfNewST::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &scaling = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	const decColor &translation = clsClr.GetColor(rt->GetValue(1)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateST(scaling, translation));
}

// public static func ColorMatrix newBrightness( float brightness )
deClassColorMatrix::nfNewBrightness::nfNewBrightness(const sInitData &init) : dsFunction(init.clsClrMat,
"newBrightness", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // brightness
}
void deClassColorMatrix::nfNewBrightness::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float brightness = rt->GetValue(0)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateBrightness(brightness));
}

// public static func ColorMatrix newBrightness( float red, float green, float blue )
deClassColorMatrix::nfNewBrightness2::nfNewBrightness2(const sInitData &init) : dsFunction(init.clsClrMat,
"newBrightness", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // red
	p_AddParameter(init.clsFlt); // green
	p_AddParameter(init.clsFlt); // blue
}
void deClassColorMatrix::nfNewBrightness2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float red = rt->GetValue(0)->GetFloat();
	const float green = rt->GetValue(1)->GetFloat();
	const float blue = rt->GetValue(2)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateBrightness(red, green, blue));
}

// public static func ColorMatrix newBrightness( Color brightness )
deClassColorMatrix::nfNewBrightness3::nfNewBrightness3(const sInitData &init) : dsFunction(init.clsClrMat,
"newBrightness", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // brightness
}
void deClassColorMatrix::nfNewBrightness3::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &brightness = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateBrightness(brightness));
}

// public static func ColorMatrix newContrast( float contrast )
deClassColorMatrix::nfNewContrast::nfNewContrast(const sInitData &init) : dsFunction(init.clsClrMat,
"newContrast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // contrast
}
void deClassColorMatrix::nfNewContrast::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float contrast = rt->GetValue(0)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateContrast(contrast));
}

// public static func ColorMatrix newContrast( float red, float green, float blue )
deClassColorMatrix::nfNewContrast2::nfNewContrast2(const sInitData &init) : dsFunction(init.clsClrMat,
"newContrast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // red
	p_AddParameter(init.clsFlt); // green
	p_AddParameter(init.clsFlt); // blue
}
void deClassColorMatrix::nfNewContrast2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float red = rt->GetValue(0)->GetFloat();
	const float green = rt->GetValue(1)->GetFloat();
	const float blue = rt->GetValue(2)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateContrast(red, green, blue));
}

// public static func ColorMatrix newContrast( Color contrast )
deClassColorMatrix::nfNewContrast3::nfNewContrast3(const sInitData &init) : dsFunction(init.clsClrMat,
"newContrast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // contrast
}
void deClassColorMatrix::nfNewContrast3::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &contrast = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateContrast(contrast));
}

// public static func ColorMatrix newSaturation( float saturation )
deClassColorMatrix::nfNewSaturation::nfNewSaturation(const sInitData &init) : dsFunction(init.clsClrMat,
"newSaturation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // saturation
}
void deClassColorMatrix::nfNewSaturation::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float saturation = rt->GetValue(0)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateSaturation(saturation));
}

// public static func ColorMatrix newSaturation( float red, float green, float blue )
deClassColorMatrix::nfNewSaturation2::nfNewSaturation2(const sInitData &init) : dsFunction(init.clsClrMat,
"newSaturation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // red
	p_AddParameter(init.clsFlt); // green
	p_AddParameter(init.clsFlt); // blue
}
void deClassColorMatrix::nfNewSaturation2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float red = rt->GetValue(0)->GetFloat();
	const float green = rt->GetValue(1)->GetFloat();
	const float blue = rt->GetValue(2)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateSaturation(red, green, blue));
}

// public static func ColorMatrix newSaturation( Color saturation )
deClassColorMatrix::nfNewSaturation3::nfNewSaturation3(const sInitData &init) : dsFunction(init.clsClrMat,
"newSaturation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClr); // saturation
}
void deClassColorMatrix::nfNewSaturation3::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &saturation = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateSaturation(saturation));
}

// public static func ColorMatrix newColorInversion()
deClassColorMatrix::nfNewColorInversion::nfNewColorInversion(const sInitData &init) : dsFunction(init.clsClrMat,
"newColorInversion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsClrMat){
}
void deClassColorMatrix::nfNewColorInversion::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	clsClrMatrix.PushColorMatrix(rt, decColorMatrix::CreateColorInversion());
}

// public func destructor()
deClassColorMatrix::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsClrMat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassColorMatrix::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
}



// Accessors
//////////////

// public func float getAt( int row, int col )
deClassColorMatrix::nfGetAt::nfGetAt(const sInitData &init) : dsFunction(init.clsClrMat,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // row
	p_AddParameter(init.clsInt); // col
}
void deClassColorMatrix::nfGetAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	const int row = rt->GetValue(0)->GetInt();
	const int col = rt->GetValue(1)->GetInt();
	
	if(row < 0 || row > 3 || col < 0 || col > 4){
		DSTHROW(dueInvalidParam);
	}
	
	const float * const values = &matrix.a11;
	
	rt->PushFloat(values[row * 5 + col]);
}

// public func float get11()
deClassColorMatrix::nfGet11::nfGet11(const sInitData &init) : dsFunction(init.clsClrMat,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet11::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a11);
}

// public func float get12()
deClassColorMatrix::nfGet12::nfGet12(const sInitData &init) : dsFunction(init.clsClrMat,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet12::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a12);
}

// public func float get13()
deClassColorMatrix::nfGet13::nfGet13(const sInitData &init) : dsFunction(init.clsClrMat,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet13::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a13);
}

// public func float get14()
deClassColorMatrix::nfGet14::nfGet14(const sInitData &init) : dsFunction(init.clsClrMat,
"get14", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet14::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a14);
}

// public func float get15()
deClassColorMatrix::nfGet15::nfGet15(const sInitData &init) : dsFunction(init.clsClrMat,
"get15", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet15::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a15);
}

// public func float get21()
deClassColorMatrix::nfGet21::nfGet21(const sInitData &init) : dsFunction(init.clsClrMat,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet21::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a21);
}

// public func float get22()
deClassColorMatrix::nfGet22::nfGet22(const sInitData &init) : dsFunction(init.clsClrMat,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet22::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a22);
}

// public func float get23()
deClassColorMatrix::nfGet23::nfGet23(const sInitData &init) : dsFunction(init.clsClrMat,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet23::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a23);
}

// public func float get24()
deClassColorMatrix::nfGet24::nfGet24(const sInitData &init) : dsFunction(init.clsClrMat,
"get24", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet24::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a24);
}

// public func float get25()
deClassColorMatrix::nfGet25::nfGet25(const sInitData &init) : dsFunction(init.clsClrMat,
"get25", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet25::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a25);
}

// public func float get31()
deClassColorMatrix::nfGet31::nfGet31(const sInitData &init) : dsFunction(init.clsClrMat,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet31::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a31);
}

// public func float get32()
deClassColorMatrix::nfGet32::nfGet32(const sInitData &init) : dsFunction(init.clsClrMat,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet32::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a32);
}

// public func float get33()
deClassColorMatrix::nfGet33::nfGet33(const sInitData &init) : dsFunction(init.clsClrMat,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet33::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a33);
}

// public func float get34()
deClassColorMatrix::nfGet34::nfGet34(const sInitData &init) : dsFunction(init.clsClrMat,
"get34", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet34::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a34);
}

// public func float get35()
deClassColorMatrix::nfGet35::nfGet35(const sInitData &init) : dsFunction(init.clsClrMat,
"get35", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet35::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a35);
}

// public func float get41()
deClassColorMatrix::nfGet41::nfGet41(const sInitData &init) : dsFunction(init.clsClrMat,
"get41", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet41::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a41);
}

// public func float get42()
deClassColorMatrix::nfGet42::nfGet42(const sInitData &init) : dsFunction(init.clsClrMat,
"get42", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet42::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a42);
}

// public func float get43()
deClassColorMatrix::nfGet43::nfGet43(const sInitData &init) : dsFunction(init.clsClrMat,
"get43", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet43::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a43);
}

// public func float get44()
deClassColorMatrix::nfGet44::nfGet44(const sInitData &init) : dsFunction(init.clsClrMat,
"get44", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet44::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a44);
}

// public func float get45()
deClassColorMatrix::nfGet45::nfGet45(const sInitData &init) : dsFunction(init.clsClrMat,
"get45", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassColorMatrix::nfGet45::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	rt->PushFloat(matrix.a45);
}

// public func Color getRedBase()
deClassColorMatrix::nfGetRedBase::nfGetRedBase(const sInitData &init) : dsFunction(init.clsClrMat,
"getRedBase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassColorMatrix::nfGetRedBase::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	clsClr.PushColor(rt, matrix.GetRedBase());
}

// public func Color getGreenBase()
deClassColorMatrix::nfGetGreenBase::nfGetGreenBase(const sInitData &init) : dsFunction(init.clsClrMat,
"getGreenBase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassColorMatrix::nfGetGreenBase::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	clsClr.PushColor(rt, matrix.GetGreenBase());
}

// public func Color getBlueBase()
deClassColorMatrix::nfGetBlueBase::nfGetBlueBase(const sInitData &init) : dsFunction(init.clsClrMat,
"getBlueBase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassColorMatrix::nfGetBlueBase::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	clsClr.PushColor(rt, matrix.GetBlueBase());
}

// public func Color getAlphaBase()
deClassColorMatrix::nfGetAlphaBase::nfGetAlphaBase(const sInitData &init) : dsFunction(init.clsClrMat,
"getAlphaBase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassColorMatrix::nfGetAlphaBase::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	clsClr.PushColor(rt, matrix.GetAlphaBase());
}

// public func Color getWhiteBase()
deClassColorMatrix::nfGetWhiteBase::nfGetWhiteBase(const sInitData &init) : dsFunction(init.clsClrMat,
"getWhiteBase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
}
void deClassColorMatrix::nfGetWhiteBase::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	clsClr.PushColor(rt, matrix.GetWhiteBase());
}



// File Handling
//////////////////

// static public func ColorMatrix readFromFile( FileReader reader )
deClassColorMatrix::nfReadFromFile::nfReadFromFile(const sInitData &init) : dsFunction(init.clsClrMat,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClrMat){
	p_AddParameter(init.clsFileReader); // reader
}
void deClassColorMatrix::nfReadFromFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassColorMatrix &clsColorMatrix = *((deClassColorMatrix*)GetOwnerClass());
	const deClassFileReader &clsFileReader = *clsColorMatrix.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader(rt->GetValue(0)->GetRealObject());
	decColorMatrix matrix;
	
	if(!reader){
		DSTHROW(dueNullPointer);
	}
	
	float * const values = &matrix.a11;
	int i;
	for(i=0; i<20; i++){
		values[i] = reader->ReadFloat();
	}
	
	clsColorMatrix.PushColorMatrix(rt, matrix);
}

// public func void writeToFile( FileWriter writer )
deClassColorMatrix::nfWriteToFile::nfWriteToFile(const sInitData &init) : dsFunction(init.clsClrMat,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFileWriter); // writer
}
void deClassColorMatrix::nfWriteToFile::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	const deClassColorMatrix &clsColorMatrix = *((deClassColorMatrix*)GetOwnerClass());
	const deClassFileWriter &clsFileWriter = *clsColorMatrix.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(rt->GetValue(0)->GetRealObject());
	
	if(!writer){
		DSTHROW(dueNullPointer);
	}
	
	const float * const values = &matrix.a11;
	int i;
	for(i=0; i<20; i++){
		writer->WriteFloat(values[i]);
	}
}



// operators
//////////////

// public func ColorMatrix +( ColorMatrix matrix )
deClassColorMatrix::nfOpAdd::nfOpAdd(const sInitData &init) : dsFunction(init.clsClrMat,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClrMat); // matrix
}
void deClassColorMatrix::nfOpAdd::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const decColorMatrix &otherMatrix = clsClrMatrix.GetColorMatrix(rt->GetValue(0)->GetRealObject());
	clsClrMatrix.PushColorMatrix(rt, matrix + otherMatrix);
}

// public func ColorMatrix -( ColorMatrix matrix )
deClassColorMatrix::nfOpSubtract::nfOpSubtract(const sInitData &init) : dsFunction(init.clsClrMat,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClrMat); // matrix
}
void deClassColorMatrix::nfOpSubtract::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const decColorMatrix &otherMatrix = clsClrMatrix.GetColorMatrix(rt->GetValue(0)->GetRealObject());
	clsClrMatrix.PushColorMatrix(rt, matrix - otherMatrix);
}

// public func ColorMatrix *( float scalar )
deClassColorMatrix::nfOpScale::nfOpScale(const sInitData &init) : dsFunction(init.clsClrMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // scalar
}
void deClassColorMatrix::nfOpScale::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const float scalar = rt->GetValue(0)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, matrix * scalar);
}

// public func ColorMatrix /( float scalar )
deClassColorMatrix::nfOpDivide::nfOpDivide(const sInitData &init) : dsFunction(init.clsClrMat,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsFlt); // k
}
void deClassColorMatrix::nfOpDivide::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	float scalar = rt->GetValue(0)->GetFloat();
	
	clsClrMatrix.PushColorMatrix(rt, matrix / scalar);
}

// public func ColorMatrix *( ColorMatrix matrix )
deClassColorMatrix::nfOpMultiply::nfOpMultiply(const sInitData &init) : dsFunction(init.clsClrMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
	p_AddParameter(init.clsClrMat); // matrix
}
void deClassColorMatrix::nfOpMultiply::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	
	const decColorMatrix &otherMatrix = clsClrMatrix.GetColorMatrix(rt->GetValue(0)->GetRealObject());
	
	clsClrMatrix.PushColorMatrix(rt, matrix * otherMatrix);
}

// public func ColorMatrix *( Color color )
deClassColorMatrix::nfOpMultiply2::nfOpMultiply2(const sInitData &init) : dsFunction(init.clsClrMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
	p_AddParameter(init.clsClr); // color
}
void deClassColorMatrix::nfOpMultiply2::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix &clsClrMatrix = *((deClassColorMatrix*)GetOwnerClass());
	deClassColor &clsClr = *clsClrMatrix.GetDS().GetClassColor();
	
	const decColor &color = clsClr.GetColor(rt->GetValue(0)->GetRealObject());
	
	clsClr.PushColor(rt, matrix * color);
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassColorMatrix::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsClrMat,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassColorMatrix::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	deClassColorMatrix *clsClrMatrix = (deClassColorMatrix*)GetOwnerClass();
	
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsClrMatrix)){
		rt->PushBool(false);
		
	}else{
		const decColorMatrix &otherMatrix = ((sCMNatDat*)p_GetNativeData(obj))->matrix;
		rt->PushBool(matrix.IsEqualTo(otherMatrix));
	}
}

// public func int hashCode()
deClassColorMatrix::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsClrMat,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassColorMatrix::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	
	int hash = (int)(matrix.a11 * 0x0001)
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
		+ (int)(matrix.a34 * 0x0800)
		+ (int)(matrix.a41 * 0x1000)
		+ (int)(matrix.a42 * 0x2000)
		+ (int)(matrix.a43 * 0x4000)
		+ (int)(matrix.a44 * 0x8000);
	
	rt->PushInt(hash);
}

// public func String toString()
deClassColorMatrix::nfToString::nfToString(const sInitData &init) : dsFunction(init.clsClrMat,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassColorMatrix::nfToString::RunFunction(dsRunTime *rt, dsValue *myself){
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format("[[%g,%g,%g,%g,%g],[%g,%g,%g,%g,%g],[%g,%g,%g,%g,%g],[%g,%g,%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a15,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24, matrix.a25,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a35,
		matrix.a41, matrix.a42, matrix.a43, matrix.a44, matrix.a45);
	
	rt->PushString(str);
}

// public func String toString( int precision )
deClassColorMatrix::nfToStringPrecision::nfToStringPrecision(const sInitData &init) :
dsFunction(init.clsClrMat, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // precision
}
void deClassColorMatrix::nfToStringPrecision::RunFunction(dsRunTime *rt, dsValue *myself){
	const int precision = rt->GetValue(0)->GetInt();
	if(precision < 0){
		DSTHROW_INFO(dueInvalidParam, "precision < 0");
	}
	if(precision > 9){
		DSTHROW_INFO(dueInvalidParam, "precision > 9");
	}
	
	const unsigned short p = (unsigned short)precision;
	char format[110];
	snprintf(format, sizeof(format), "[[%%.%huf,%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf,%%.%huf]]",
		p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p);
	
	const decColorMatrix &matrix = ((sCMNatDat*)p_GetNativeData(myself))->matrix;
	decString str;
	
	str.Format(format,
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a15,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24, matrix.a25,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a35,
		matrix.a41, matrix.a42, matrix.a43, matrix.a44, matrix.a45);
	
	rt->PushString(str);
}



// Class deClassColorMatrix
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassColorMatrix::deClassColorMatrix(deScriptingDragonScript &ds) :
dsClass("ColorMatrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCMNatDat));
}

deClassColorMatrix::~deClassColorMatrix(){
}



// Management
///////////////

void deClassColorMatrix::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	memset(&init, '\0', sizeof(init));
	
	init.clsClrMat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsClr = pDS.GetClassColor();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNew2(init));
	AddFunction(new nfNew3(init));
	AddFunction(new nfNewTranslation(init));
	AddFunction(new nfNewTranslation2(init));
	AddFunction(new nfNewScaling(init));
	AddFunction(new nfNewScaling2(init));
	AddFunction(new nfNewST(init));
	AddFunction(new nfNewBrightness(init));
	AddFunction(new nfNewBrightness2(init));
	AddFunction(new nfNewBrightness3(init));
	AddFunction(new nfNewContrast(init));
	AddFunction(new nfNewContrast2(init));
	AddFunction(new nfNewContrast3(init));
	AddFunction(new nfNewSaturation(init));
	AddFunction(new nfNewSaturation2(init));
	AddFunction(new nfNewSaturation3(init));
	AddFunction(new nfNewColorInversion(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetAt(init));
	AddFunction(new nfGet11(init));
	AddFunction(new nfGet12(init));
	AddFunction(new nfGet13(init));
	AddFunction(new nfGet14(init));
	AddFunction(new nfGet15(init));
	AddFunction(new nfGet21(init));
	AddFunction(new nfGet22(init));
	AddFunction(new nfGet23(init));
	AddFunction(new nfGet24(init));
	AddFunction(new nfGet25(init));
	AddFunction(new nfGet31(init));
	AddFunction(new nfGet32(init));
	AddFunction(new nfGet33(init));
	AddFunction(new nfGet34(init));
	AddFunction(new nfGet35(init));
	AddFunction(new nfGet41(init));
	AddFunction(new nfGet42(init));
	AddFunction(new nfGet43(init));
	AddFunction(new nfGet44(init));
	AddFunction(new nfGet45(init));
	AddFunction(new nfGetRedBase(init));
	AddFunction(new nfGetBlueBase(init));
	AddFunction(new nfGetGreenBase(init));
	AddFunction(new nfGetAlphaBase(init));
	AddFunction(new nfGetWhiteBase(init));
	
	AddFunction(new nfReadFromFile(init));
	AddFunction(new nfWriteToFile(init));
	
	AddFunction(new nfOpAdd(init));
	AddFunction(new nfOpSubtract(init));
	AddFunction(new nfOpScale(init));
	AddFunction(new nfOpDivide(init));
	AddFunction(new nfOpMultiply(init));
	AddFunction(new nfOpMultiply2(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
	AddFunction(new nfToString(init));
	AddFunction(new nfToStringPrecision(init));
}

const decColorMatrix &deClassColorMatrix::GetColorMatrix(dsRealObject *myself) const{
	if(!myself){
		DSTHROW(dueNullPointer);
	}
	
	return (const decColorMatrix &)((sCMNatDat*)p_GetNativeData(myself->GetBuffer()))->matrix;
}

void deClassColorMatrix::PushColorMatrix(dsRunTime *rt, const decColorMatrix &matrix){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sCMNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->matrix = matrix;
}
