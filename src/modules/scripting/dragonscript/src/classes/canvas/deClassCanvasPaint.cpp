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
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCanvasPaint.h"
#include "deClassCanvas.h"
#include "../graphics/deClassColor.h"
#include "../math/deClassPoint.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasManager.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
struct sCPaintNatDat{
	deCanvasPaint *canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasPaint::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCPaint,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasPaint::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = (deClassCanvas*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasPaint();
	baseClass->AssignCanvas(myself->GetRealObject(), nd.canvas);
}

// public func destructor()
deClassCanvasPaint::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCPaint,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasPaint::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	
	if(nd.canvas){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func CanvasPaintShape getShapeType()
deClassCanvasPaint::nfGetShapeType::nfGetShapeType(const sInitData &init) : dsFunction(init.clsCPaint,
"getShapeType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvasPaintShape){
}
void deClassCanvasPaint::nfGetShapeType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushValue(((deClassCanvasPaint*)GetOwnerClass())->GetClassCanvasPaintShape()
		->GetVariable(nd.canvas->GetShapeType())->GetStaticValue());
}

// public func void setShapeType( CanvasPaintShape shapeType )
deClassCanvasPaint::nfSetShapeType::nfSetShapeType(const sInitData &init) : dsFunction(init.clsCPaint,
"setShapeType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvasPaintShape); // shapeType
}
void deClassCanvasPaint::nfSetShapeType::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.canvas->SetShapeType((deCanvasPaint::eShapeTypes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Color getLineColor()
deClassCanvasPaint::nfGetLineColor::nfGetLineColor(const sInitData &init) : dsFunction(init.clsCPaint,
"getLineColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassCanvasPaint::nfGetLineColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	ds.GetClassColor()->PushColor(rt, nd.canvas->GetLineColor());
}

// public func void setLineColor( Color color )
deClassCanvasPaint::nfSetLineColor::nfSetLineColor(const sInitData &init) : dsFunction(init.clsCPaint,
"setLineColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassCanvasPaint::nfSetLineColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject());
	nd.canvas->SetLineColor(color);
}

// public func Color getFillColor()
deClassCanvasPaint::nfGetFillColor::nfGetFillColor(const sInitData &init) : dsFunction(init.clsCPaint,
"getFillColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassCanvasPaint::nfGetFillColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	ds.GetClassColor()->PushColor(rt, nd.canvas->GetFillColor());
}

// public func void setFillColor( Color color )
deClassCanvasPaint::nfSetFillColor::nfSetFillColor(const sInitData &init) : dsFunction(init.clsCPaint,
"setFillColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassCanvasPaint::nfSetFillColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject());
	nd.canvas->SetFillColor(color);
}

// public func float getThickness()
deClassCanvasPaint::nfGetThickness::nfGetThickness(const sInitData &init) : dsFunction(init.clsCPaint,
"getThickness", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvasPaint::nfGetThickness::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushFloat(nd.canvas->GetThickness());
}

// public func void setThickness( float thickness )
deClassCanvasPaint::nfSetThickness::nfSetThickness(const sInitData &init) : dsFunction(init.clsCPaint,
"setThickness", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // thickness
}
void deClassCanvasPaint::nfSetThickness::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->SetThickness(rt->GetValue(0)->GetFloat());
}

// public func float getRoundCornerX()
deClassCanvasPaint::nfGetRoundCornerX::nfGetRoundCornerX(const sInitData &init) : dsFunction(init.clsCPaint,
"getRoundCornerX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvasPaint::nfGetRoundCornerX::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushFloat(nd.canvas->GetRoundCornerX());
}

// public func void setRoundCornerX( float roundCorner )
deClassCanvasPaint::nfSetRoundCornerX::nfSetRoundCornerX(const sInitData &init) : dsFunction(init.clsCPaint,
"setRoundCornerX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // roundCorner
}
void deClassCanvasPaint::nfSetRoundCornerX::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->SetRoundCornerX(rt->GetValue(0)->GetFloat());
}

// public func float getRoundCornerY()
deClassCanvasPaint::nfGetRoundCornerY::nfGetRoundCornerY(const sInitData &init) : dsFunction(init.clsCPaint,
"getRoundCornerY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvasPaint::nfGetRoundCornerY::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushFloat(nd.canvas->GetRoundCornerY());
}

// public func void setRoundCornerY( float roundCorner )
deClassCanvasPaint::nfSetRoundCornerY::nfSetRoundCornerY(const sInitData &init) : dsFunction(init.clsCPaint,
"setRoundCornerY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // roundCorner
}
void deClassCanvasPaint::nfSetRoundCornerY::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->SetRoundCornerY(rt->GetValue(0)->GetFloat());
}

// public func float getStartAngle()
deClassCanvasPaint::nfGetStartAngle::nfGetStartAngle(const sInitData &init) : dsFunction(init.clsCPaint,
"getStartAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvasPaint::nfGetStartAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushFloat(RAD2DEG * nd.canvas->GetStartAngle());
}

// public func void setStartAngle( float angle )
deClassCanvasPaint::nfSetStartAngle::nfSetStartAngle(const sInitData &init) : dsFunction(init.clsCPaint,
"setStartAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassCanvasPaint::nfSetStartAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->SetStartAngle(DEG2RAD * rt->GetValue(0)->GetFloat());
}

// public func float getEndAngle()
deClassCanvasPaint::nfGetEndAngle::nfGetEndAngle(const sInitData &init) : dsFunction(init.clsCPaint,
"getEndAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvasPaint::nfGetEndAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushFloat(RAD2DEG * nd.canvas->GetEndAngle());
}

// public func void setEndAngle( float angle )
deClassCanvasPaint::nfSetEndAngle::nfSetEndAngle(const sInitData &init) : dsFunction(init.clsCPaint,
"setEndAngle", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // angle
}
void deClassCanvasPaint::nfSetEndAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->SetEndAngle(DEG2RAD * rt->GetValue(0)->GetFloat());
}



// public func int getPointCount()
deClassCanvasPaint::nfGetPointCount::nfGetPointCount(const sInitData &init) : dsFunction(init.clsCPaint,
"getPointCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCanvasPaint::nfGetPointCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	rt->PushInt(nd.canvas->GetPointCount());
}

// public func Point getPointAt( int index )
deClassCanvasPaint::nfGetPointAt::nfGetPointAt(const sInitData &init) : dsFunction(init.clsCPaint,
"getPointAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint){
	p_AddParameter(init.clsInt); // index
}
void deClassCanvasPaint::nfGetPointAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	ds.GetClassPoint()->PushPoint(rt, nd.canvas->GetPointAt(index));
}

// public func void addPoint( Point point )
deClassCanvasPaint::nfAddPoint::nfAddPoint(const sInitData &init) : dsFunction(init.clsCPaint,
"addPoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint); // point
}
void deClassCanvasPaint::nfAddPoint::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasPaint*)GetOwnerClass())->GetDS();
	
	const decPoint &point = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	nd.canvas->AddPoint(point);
}

// public func void removeAllPoints()
deClassCanvasPaint::nfRemoveAllPoints::nfRemoveAllPoints(const sInitData &init) : dsFunction(init.clsCPaint,
"removeAllPoints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasPaint::nfRemoveAllPoints::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(myself));
	nd.canvas->RemoveAllPoints();
}



// public func int hashCode()
deClassCanvasPaint::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCPaint, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCanvasPaint::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasPaint * const canvas = ((sCPaintNatDat*)p_GetNativeData(myself))->canvas;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)canvas);
}

// public func bool equals( Object obj )
deClassCanvasPaint::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCPaint, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCanvasPaint::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasPaint * const canvas = ((sCPaintNatDat*)p_GetNativeData(myself))->canvas;
	deClassCanvasPaint * const clsCPaint = (deClassCanvasPaint*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCPaint)){
		rt->PushBool(false);
		
	}else{
		deCanvasPaint * const otherCanvas = ((sCPaintNatDat*)p_GetNativeData(obj))->canvas;
		rt->PushBool(canvas == otherCanvas);
	}
}



// Class deClassCanvasPaint
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasPaint::deClassCanvasPaint(deScriptingDragonScript &ds) :
dsClass("CanvasPaint", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Canvas");
	
	p_SetNativeDataSize(sizeof(sCPaintNatDat));
}

deClassCanvasPaint::~deClassCanvasPaint(){
}



// Management
///////////////

void deClassCanvasPaint::CreateClassMembers(dsEngine *engine){
	pClsCanvasPaintShape = engine->GetClass("Dragengine.Gui.CanvasPaintShape");
	
	sInitData init;
	init.clsCPaint = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsPoint = pDS.GetClassPoint();
	init.clsColor = pDS.GetClassColor();
	init.clsCanvasPaintShape = pClsCanvasPaintShape;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetShapeType(init));
	AddFunction(new nfSetShapeType(init));
	AddFunction(new nfGetLineColor(init));
	AddFunction(new nfSetLineColor(init));
	AddFunction(new nfGetFillColor(init));
	AddFunction(new nfSetFillColor(init));
	AddFunction(new nfGetThickness(init));
	AddFunction(new nfSetThickness(init));
	AddFunction(new nfGetRoundCornerX(init));
	AddFunction(new nfSetRoundCornerX(init));
	AddFunction(new nfGetRoundCornerY(init));
	AddFunction(new nfSetRoundCornerY(init));
	AddFunction(new nfGetStartAngle(init));
	AddFunction(new nfSetStartAngle(init));
	AddFunction(new nfGetEndAngle(init));
	AddFunction(new nfSetEndAngle(init));
	
	AddFunction(new nfGetPointCount(init));
	AddFunction(new nfGetPointAt(init));
	AddFunction(new nfAddPoint(init));
	AddFunction(new nfRemoveAllPoints(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasPaint *deClassCanvasPaint::GetCanvas(dsRealObject *myself) const {
	if(!myself){
		return NULL;
	}
	
	return ((sCPaintNatDat*)p_GetNativeData(myself->GetBuffer()))->canvas;
}

void deClassCanvasPaint::PushCanvas(dsRunTime *rt, deCanvasPaint *canvas){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!canvas){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassCanvas * const baseClass = (deClassCanvas*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sCPaintNatDat &nd = *((sCPaintNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.canvas = NULL;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.canvas = canvas;
		canvas->AddReference();
		
		baseClass->AssignCanvas(rt->GetValue(0)->GetRealObject(), canvas);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
