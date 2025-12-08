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

#include "deClassCanvasView.h"
#include "deClassCanvas.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCViewNatDat{
	deCanvasView::Ref canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasView::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCView,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasView::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasView*)GetOwnerClass())->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = (deClassCanvas*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasView();
	baseClass->AssignCanvas(myself->GetRealObject(), nd.canvas);
}

// public func destructor()
deClassCanvasView::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCView,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasView::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	
	if(nd.canvas){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func int getCanvasCount()
deClassCanvasView::nfGetCanvasCount::nfGetCanvasCount(const sInitData &init) : dsFunction(init.clsCView,
"getCanvasCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassCanvasView::nfGetCanvasCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	rt->PushInt(nd.canvas->GetCanvasCount());
}

// public func Canvas getCanvasAt( int index )
deClassCanvasView::nfGetCanvasAt::nfGetCanvasAt(const sInitData &init) :
dsFunction(init.clsCView, "getCanvasAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsCanvas){
	p_AddParameter(init.clsInt); // index
}
void deClassCanvasView::nfGetCanvasAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasView*)GetOwnerClass())->GetDS();
	
	int index = rt->GetValue(0)->GetInt();
	deCanvas *canvas = nd.canvas->GetRootCanvas();
	while(index-- > 0){
		if(!canvas){
			DSTHROW(dueInvalidParam);
		}
		canvas = canvas->GetLLViewNext();
	}
	
	ds.GetClassCanvas()->PushCanvas(rt, canvas);
}

// public func void addCanvas( Canvas canvas )
deClassCanvasView::nfAddCanvas::nfAddCanvas(const sInitData &init) : dsFunction(init.clsCView,
"addCanvas", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvas); // canvas
}
void deClassCanvasView::nfAddCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasView*)GetOwnerClass())->GetDS();
	
	deCanvas * const child = ds.GetClassCanvas()->GetCanvas(rt->GetValue(0)->GetRealObject());
	nd.canvas->AddCanvas(child);
}

// public func bool hasCanvas(Canvas canvas)
deClassCanvasView::nfHasCanvas::nfHasCanvas(const sInitData &init) :
dsFunction(init.clsCView, "hasCanvas", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsCanvas); // canvas
}
void deClassCanvasView::nfHasCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasView*)GetOwnerClass())->GetDS();
	
	deCanvas * const child = ds.GetClassCanvas()->GetCanvas(rt->GetValue(0)->GetRealObject());
	if(!child){
		DSTHROW_INFO(dueNullPointer, "canvas");
	}
	rt->PushBool(child->GetParentView() == nd.canvas);
}

// public func void removeCanvas( Canvas canvas )
deClassCanvasView::nfRemoveCanvas::nfRemoveCanvas(const sInitData &init) : dsFunction(init.clsCView,
"removeCanvas", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCanvas); // canvas
}
void deClassCanvasView::nfRemoveCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasView*)GetOwnerClass())->GetDS();
	
	deCanvas * const child = ds.GetClassCanvas()->GetCanvas(rt->GetValue(0)->GetRealObject());
	nd.canvas->RemoveCanvas(child);
}

// public func void removeAllCanvas()
deClassCanvasView::nfRemoveAllCanvas::nfRemoveAllCanvas(const sInitData &init) : dsFunction(init.clsCView,
"removeAllCanvas", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasView::nfRemoveAllCanvas::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(myself));
	nd.canvas->RemoveAllCanvas();
}



// public func int hashCode()
deClassCanvasView::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCView, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCanvasView::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasView * const canvas = ((sCViewNatDat*)p_GetNativeData(myself))->canvas;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)canvas);
}

// public func bool equals( Object obj )
deClassCanvasView::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCView, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCanvasView::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasView * const canvas = ((sCViewNatDat*)p_GetNativeData(myself))->canvas;
	deClassCanvasView * const clsCView = (deClassCanvasView*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCView)){
		rt->PushBool(false);
		
	}else{
		deCanvasView * const otherCanvas = ((sCViewNatDat*)p_GetNativeData(obj))->canvas;
		rt->PushBool(canvas == otherCanvas);
	}
}



// Class deClassCanvasView
////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasView::deClassCanvasView(deScriptingDragonScript &ds) :
dsClass("CanvasView", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Canvas");
	
	p_SetNativeDataSize(sizeof(sCViewNatDat));
}

deClassCanvasView::~deClassCanvasView(){
}



// Management
///////////////

void deClassCanvasView::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsCView = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsCanvas = pDS.GetClassCanvas();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetCanvasCount(init));
	AddFunction(new nfGetCanvasAt(init));
	AddFunction(new nfHasCanvas(init));
	AddFunction(new nfAddCanvas(init));
	AddFunction(new nfRemoveCanvas(init));
	AddFunction(new nfRemoveAllCanvas(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasView *deClassCanvasView::GetCanvas(dsRealObject *myself) const {
	if(!myself){
		return NULL;
	}
	
	return ((sCViewNatDat*)p_GetNativeData(myself->GetBuffer()))->canvas;
}

void deClassCanvasView::PushCanvas(dsRunTime *rt, deCanvasView *canvas){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!canvas){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassCanvas * const baseClass = (deClassCanvas*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sCViewNatDat &nd = *((sCViewNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.canvas = NULL;
		
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.canvas = canvas;
		baseClass->AssignCanvas(rt->GetValue(0)->GetRealObject(), canvas);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
