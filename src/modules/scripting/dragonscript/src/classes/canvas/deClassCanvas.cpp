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

#include "deClassCanvas.h"
#include "deClassCanvasCanvasView.h"
#include "deClassCanvasImage.h"
#include "deClassCanvasPaint.h"
#include "deClassCanvasRenderWorld.h"
#include "deClassCanvasText.h"
#include "deClassCanvasVideoPlayer.h"
#include "deClassCanvasView.h"
#include "../math/deClassTexMatrix2.h"
#include "../math/deClassColorMatrix.h"
#include "../math/deClassPoint.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasVideoPlayer.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasCanvasView.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
struct sCanvasNatDat{
	deCanvas::Ref canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvas::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCanvas,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvas::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	nd.canvas = NULL;
}

// public func destructor()
deClassCanvas::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCanvas,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvas::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	
	if(nd.canvas){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management
///////////////

// public func Point getPosition()
deClassCanvas::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsCanvas,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint){
}
void deClassCanvas::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	
	ds.GetClassPoint()->PushPoint(rt, nd.canvas->GetPosition());
}

// public func void setPosition( Point position )
deClassCanvas::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsCanvas,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint); // position
}
void deClassCanvas::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	const decPoint &position = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	
	nd.canvas->SetPosition(position);
}

// public func Point getSize()
deClassCanvas::nfGetSize::nfGetSize(const sInitData &init) : dsFunction(init.clsCanvas,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint){
}
void deClassCanvas::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	
	ds.GetClassPoint()->PushPoint(rt, nd.canvas->GetSize());
}

// public func void setSize( Point size )
deClassCanvas::nfSetSize::nfSetSize(const sInitData &init) : dsFunction(init.clsCanvas,
"setSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint); // position
}
void deClassCanvas::nfSetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	const decPoint &size = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	
	nd.canvas->SetSize(size);
}

// public func TexMatrix2 getTransform()
deClassCanvas::nfGetTransform::nfGetTransform(const sInitData &init) : dsFunction(init.clsCanvas,
"getTransform", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat2){
}
void deClassCanvas::nfGetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	
	ds.GetClassTexMatrix2()->PushTexMatrix(rt, nd.canvas->GetTransform());
}

// public func void setTransform( TexMatrix2 transform )
deClassCanvas::nfSetTransform::nfSetTransform(const sInitData &init) : dsFunction(init.clsCanvas,
"setTransform", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsTexMat2); // transform
}
void deClassCanvas::nfSetTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	const decTexMatrix2 &transform = ds.GetClassTexMatrix2()->GetTexMatrix(rt->GetValue(0)->GetRealObject());
	
	nd.canvas->SetTransform(transform);
}

// public func ColorMatrix getColorTransform()
deClassCanvas::nfGetColorTransform::nfGetColorTransform(const sInitData &init) : dsFunction(init.clsCanvas,
"getColorTransform", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClrMat){
}
void deClassCanvas::nfGetColorTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	
	ds.GetClassColorMatrix()->PushColorMatrix(rt, nd.canvas->GetColorTransform());
}

// public func void setColorTransform( ColorMatrix transform )
deClassCanvas::nfSetColorTransform::nfSetColorTransform(const sInitData &init) : dsFunction(init.clsCanvas,
"setColorTransform", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsClrMat); // transform
}
void deClassCanvas::nfSetColorTransform::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	const decColorMatrix &transform = ds.GetClassColorMatrix()->GetColorMatrix(rt->GetValue(0)->GetRealObject());
	
	nd.canvas->SetColorTransform(transform);
}

// public func bool getVisible()
deClassCanvas::nfGetVisible::nfGetVisible(const sInitData &init) : dsFunction(init.clsCanvas,
"getVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassCanvas::nfGetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushBool(nd.canvas->GetVisible());
}

// public func void setVisible( bool visible )
deClassCanvas::nfSetVisible::nfSetVisible(const sInitData &init) : dsFunction(init.clsCanvas,
"setVisible", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // visible
}
void deClassCanvas::nfSetVisible::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	nd.canvas->SetVisible(rt->GetValue(0)->GetBool());
}

// public func float getOrder()
deClassCanvas::nfGetOrder::nfGetOrder(const sInitData &init) : dsFunction(init.clsCanvas,
"getOrder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvas::nfGetOrder::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.canvas->GetOrder());
}

// public func void setOrder( float order )
deClassCanvas::nfSetOrder::nfSetOrder(const sInitData &init) : dsFunction(init.clsCanvas,
"setOrder", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // order
}
void deClassCanvas::nfSetOrder::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	nd.canvas->SetOrder(rt->GetValue(0)->GetFloat());
}

// public func float getTransparency()
deClassCanvas::nfGetTransparency::nfGetTransparency(const sInitData &init) : dsFunction(init.clsCanvas,
"getTransparency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassCanvas::nfGetTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushFloat(nd.canvas->GetTransparency());
}

// public func void setTransparency( float transparency )
deClassCanvas::nfSetTransparency::nfSetTransparency(const sInitData &init) : dsFunction(init.clsCanvas,
"setTransparency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // transparency
}
void deClassCanvas::nfSetTransparency::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	nd.canvas->SetTransparency(rt->GetValue(0)->GetFloat());
}

// public func CanvasBlendMode getBlendMode()
deClassCanvas::nfGetBlendMode::nfGetBlendMode(const sInitData &init) : dsFunction(init.clsCanvas,
"getBlendMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvasBlendMode){
}
void deClassCanvas::nfGetBlendMode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	rt->PushValue(((deClassCanvas*)GetOwnerClass())->GetClassCanvasBlendMode()
		->GetVariable(nd.canvas->GetBlendMode())->GetStaticValue());
}

// public func void setBlendMode( CanvasBlendMode blendMoe )
deClassCanvas::nfSetBlendMode::nfSetBlendMode(const sInitData &init) : dsFunction(init.clsCanvas,
"setBlendMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvasBlendMode); // blendMode
}
void deClassCanvas::nfSetBlendMode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas || !rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.canvas->SetBlendMode((deCanvas::eBlendModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Canvas getMask()
deClassCanvas::nfGetMask::nfGetMask(const sInitData &init) : dsFunction(init.clsCanvas,
"getMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvas){
}
void deClassCanvas::nfGetMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	deClassCanvas &clsCanvas = *((deClassCanvas*)GetOwnerClass());
	clsCanvas.PushCanvas(rt, nd.canvas->GetMask());
}

// public func void setMask( Canvas mask )
deClassCanvas::nfSetMask::nfSetMask(const sInitData &init) : dsFunction(init.clsCanvas,
"setMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvas); // mask
}
void deClassCanvas::nfSetMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas || !rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deClassCanvas &clsCanvas = *((deClassCanvas*)GetOwnerClass());
	nd.canvas->SetMask(clsCanvas.GetCanvas(rt->GetValue(0)->GetRealObject()));
}

// public func CanvasView getParentView()
deClassCanvas::nfGetParentView::nfGetParentView(const sInitData &init) :
dsFunction(init.clsCanvas, "getParentView", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvasView){
}
void deClassCanvas::nfGetParentView::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	const deScriptingDragonScript &ds = ((deClassCanvas*)GetOwnerClass())->GetDS();
	ds.GetClassCanvasView()->PushCanvas(rt, nd.canvas->GetParentView());
}

// public func Canvas getParentMask()
deClassCanvas::nfGetParentMask::nfGetParentMask(const sInitData &init) :
dsFunction(init.clsCanvas, "getParentMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvas){
}
void deClassCanvas::nfGetParentMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself));
	if(!nd.canvas){
		DSTHROW(dueNullPointer);
	}
	
	((deClassCanvas*)GetOwnerClass())->PushCanvas(rt, nd.canvas->GetParentMask());
}



// public func int hashCode()
deClassCanvas::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCanvas, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCanvas::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvas * const canvas = ((sCanvasNatDat*)p_GetNativeData(myself))->canvas;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)canvas);
}

// public func bool equals( Object obj )
deClassCanvas::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCanvas, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCanvas::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvas * const canvas = ((sCanvasNatDat*)p_GetNativeData(myself))->canvas;
	deClassCanvas * const clsCanvas = (deClassCanvas*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCanvas)){
		rt->PushBool(false);
		
	}else{
		deCanvas * const otherCanvas = ((sCanvasNatDat*)p_GetNativeData(obj))->canvas;
		rt->PushBool(canvas == otherCanvas);
	}
}



// Class deClassCanvas
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvas::deClassCanvas(deScriptingDragonScript &ds) :
dsClass("Canvas", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCanvasNatDat));
}

deClassCanvas::~deClassCanvas(){
}



// Management
///////////////

void deClassCanvas::CreateClassMembers(dsEngine *engine){
	pClsCanvasBlendMode = engine->GetClass("Dragengine.Gui.CanvasBlendMode");
	
	sInitData init;
	init.clsCanvas = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsPoint = pDS.GetClassPoint();
	init.clsTexMat2 = pDS.GetClassTexMatrix2();
	init.clsClrMat = pDS.GetClassColorMatrix();
	init.clsCanvasBlendMode = pClsCanvasBlendMode;
	init.clsCanvasView = pDS.GetClassCanvasView();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSetSize(init));
	AddFunction(new nfGetTransform(init));
	AddFunction(new nfSetTransform(init));
	AddFunction(new nfGetColorTransform(init));
	AddFunction(new nfSetColorTransform(init));
	AddFunction(new nfGetVisible(init));
	AddFunction(new nfSetVisible(init));
	AddFunction(new nfGetOrder(init));
	AddFunction(new nfSetOrder(init));
	AddFunction(new nfGetTransparency(init));
	AddFunction(new nfSetTransparency(init));
	AddFunction(new nfGetBlendMode(init));
	AddFunction(new nfSetBlendMode(init));
	AddFunction(new nfGetMask(init));
	AddFunction(new nfSetMask(init));
	AddFunction(new nfGetParentView(init));
	AddFunction(new nfGetParentMask(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



void deClassCanvas::AssignCanvas(dsRealObject *myself, deCanvas::Ref canvas){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	sCanvasNatDat &nd = *((sCanvasNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(canvas == nd.canvas){
		return;
	}
	
	if(nd.canvas){
		nd.canvas->FreeReference();
	}
	
	nd.canvas = canvas;
}



deCanvas *deClassCanvas::GetCanvas(dsRealObject *myself) const {
	if(!myself){
		return NULL;
	}
	
	return ((sCanvasNatDat*)p_GetNativeData(myself->GetBuffer()))->canvas;
}

void deClassCanvas::PushCanvas(dsRunTime *rt, deCanvas::Ref canvas){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!canvas){
		rt->PushObject(NULL, this);
		return;
	}
	
	deCanvasVisitorIdentify identify;
	canvas->Visit(identify);
	
	switch(identify.GetType()){
	case deCanvasVisitorIdentify::ectCanvasView:
		pDS.GetClassCanvasCanvasView()->PushCanvas(rt, &identify.CastToCanvasView());
		break;
		
	case deCanvasVisitorIdentify::ectImage:
		pDS.GetClassCanvasImage()->PushCanvas(rt, &identify.CastToImage());
		break;
		
	case deCanvasVisitorIdentify::ectPaint:
		pDS.GetClassCanvasPaint()->PushCanvas(rt, &identify.CastToPaint());
		break;
		
	case deCanvasVisitorIdentify::ectRenderWorld:
		pDS.GetClassCanvasRenderWorld()->PushCanvas(rt, &identify.CastToRenderWorld());
		break;
		
	case deCanvasVisitorIdentify::ectText:
		pDS.GetClassCanvasText()->PushCanvas(rt, &identify.CastToText());
		break;
		
	case deCanvasVisitorIdentify::ectVideoPlayer:
		pDS.GetClassCanvasVideoPlayer()->PushCanvas(rt, &identify.CastToVideoPlayer());
		break;
		
	case deCanvasVisitorIdentify::ectView:
		pDS.GetClassCanvasView()->PushCanvas(rt, &identify.CastToView());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}
