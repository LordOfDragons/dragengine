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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassDynamicSkin.h"
#include "../canvas/deClassCanvasView.h"
#include "../graphics/deClassCamera.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassImage.h"
#include "../video/deClassVideoPlayer.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderable.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVisitor.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVisitorIdentify.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableValue.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableImage.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCanvas.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableVideoFrame.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableCamera.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sDSkinNatDat{
	deDynamicSkin::Ref dynamicSkin;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new()
deClassDynamicSkin::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsDSkin,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDynamicSkin::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sDSkinNatDat &nd = *((sDSkinNatDat*)p_GetNativeData(myself));
	deClassDynamicSkin &clsDSkin = *((deClassDynamicSkin*)GetOwnerClass());
	deDynamicSkinManager &dskinmgr = *clsDSkin.GetDS()->GetGameEngine()->GetDynamicSkinManager();
	
	// clear (important)
	nd.dynamicSkin = NULL;
	
	// create dynamic skin
	nd.dynamicSkin = dskinmgr.CreateDynamicSkin();
	if(!nd.dynamicSkin) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassDynamicSkin::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsDSkin,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDynamicSkin::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sDSkinNatDat &nd = *((sDSkinNatDat*)p_GetNativeData(myself));
	
	if(nd.dynamicSkin){
		nd.dynamicSkin->FreeReference();
		nd.dynamicSkin = NULL;
	}
}



// public func int getRenderableCount()
deClassDynamicSkin::nfGetRenderableCount::nfGetRenderableCount(const sInitData &init) : dsFunction(init.clsDSkin,
"getRenderableCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassDynamicSkin::nfGetRenderableCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	
	rt->PushInt(dynamicSkin.GetRenderableCount());
}

// public func bool hasRenderable( String name )
deClassDynamicSkin::nfHasRenderable::nfHasRenderable(const sInitData &init) : dsFunction(init.clsDSkin,
"hasRenderable", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsStr); // name
}
void deClassDynamicSkin::nfHasRenderable::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const char * const name = rt->GetValue(0)->GetString();
	
	rt->PushBool(dynamicSkin.HasRenderableNamed(name));
}

// public func int indexOfRenderable( String name )
deClassDynamicSkin::nfIndexOfRenderable::nfIndexOfRenderable(const sInitData &init) : dsFunction(init.clsDSkin,
"indexOfRenderable", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsStr); // name
}
void deClassDynamicSkin::nfIndexOfRenderable::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const char * const name = rt->GetValue(0)->GetString();
	
	rt->PushInt(dynamicSkin.IndexOfRenderableNamed(name));
}

// public func void addRenderable( String name, DynamicSkinRenderableType type )
deClassDynamicSkin::nfAddRenderable::nfAddRenderable(const sInitData &init) : dsFunction(init.clsDSkin,
"addRenderable", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsDynamicSkinRenderableType); // type
}
void deClassDynamicSkin::nfAddRenderable::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const char *name = rt->GetValue(0)->GetString();
	deDSRenderable *renderable = NULL;
	
	if(dynamicSkin.HasRenderableNamed(name)){
		DSTHROW(dueInvalidParam);
	}
	if(!rt->GetValue(1)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	const deDSRenderableVisitorIdentify::eRenderableTypes type = (deDSRenderableVisitorIdentify::eRenderableTypes)
		(((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() ) );
	
	switch(type){
	case deDSRenderableVisitorIdentify::eptValue:
		renderable = new deDSRenderableValue(name);
		break;
		
	case deDSRenderableVisitorIdentify::eptColor:
		renderable = new deDSRenderableColor(name);
		break;
		
	case deDSRenderableVisitorIdentify::eptImage:
		renderable = new deDSRenderableImage(name);
		break;
		
	case deDSRenderableVisitorIdentify::eptCanvas:
		renderable = new deDSRenderableCanvas(name);
		break;
		
	case deDSRenderableVisitorIdentify::eptVideoFrame:
		renderable = new deDSRenderableVideoFrame(name);
		break;
		
	case deDSRenderableVisitorIdentify::eptCamera:
		renderable = new deDSRenderableCamera(name);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	try{
		dynamicSkin.AddRenderable(renderable);
		renderable = NULL;
		
	}catch(...){
		delete renderable;
		throw;
	}
}

// public func void removeRenderable( String name )
deClassDynamicSkin::nfRemoveRenderable::nfRemoveRenderable(const sInitData &init) : dsFunction(init.clsDSkin,
"removeRenderable", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassDynamicSkin::nfRemoveRenderable::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const char * const name = rt->GetValue(0)->GetString();
	
	dynamicSkin.RemoveRenderableNamed(name);
}

// public func void removeAllRenderables()
deClassDynamicSkin::nfRemoveAllRenderables::nfRemoveAllRenderables(const sInitData &init) : dsFunction(init.clsDSkin,
"removeAllRenderables", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassDynamicSkin::nfRemoveAllRenderables::RunFunction(dsRunTime*, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	
	dynamicSkin.RemoveAllRenderables();
}



// public func DynamicSkinRenderableType getTypeAt( int renderable )
deClassDynamicSkin::nfGetTypeAt::nfGetTypeAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getTypeAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDynamicSkinRenderableType){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetTypeAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	int renderable = rt->GetValue(0)->GetInt();
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	
	rt->PushValue(((deClassDynamicSkin*)GetOwnerClass())->GetClassDynamicSkinRenderableType()
		->GetVariable(identify.GetType())->GetStaticValue());
}



// public func float getValueAt(int renderable)
deClassDynamicSkin::nfGetValueAt::nfGetValueAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getValueAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetValueAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsValue()){
		DSTHROW(dueInvalidParam);
	}
	
	rt->PushFloat(identify.CastToValue().GetValue());
}

// public func void setValueAt( int renderable, float value )
deClassDynamicSkin::nfSetValueAt::nfSetValueAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setValueAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsFlt); // value
}
void deClassDynamicSkin::nfSetValueAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	int renderable = rt->GetValue(0)->GetInt();
	float value = rt->GetValue(1)->GetFloat();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsValue()) DSTHROW(dueInvalidParam);
	
	identify.CastToValue().SetValue(value);
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func Color getColorAt(int renderable)
deClassDynamicSkin::nfGetColorAt::nfGetColorAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getColorAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetColorAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsColor()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassColor()->PushColor(rt, identify.CastToColor().GetColor());
}

// public func void setColorAt( int renderable, Color color )
deClassDynamicSkin::nfSetColorAt::nfSetColorAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setColorAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsClr); // color
}
void deClassDynamicSkin::nfSetColorAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	deClassDynamicSkin &clsDSkin = *((deClassDynamicSkin*)GetOwnerClass());
	deClassColor &clsClr = *clsDSkin.GetDS()->GetClassColor();
	int renderable = rt->GetValue(0)->GetInt();
	dsRealObject *objColor = rt->GetValue(1)->GetRealObject();
	
	if(!objColor) DSTHROW(dueNullPointer);
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsColor()) DSTHROW(dueInvalidParam);
	
	identify.CastToColor().SetColor(clsClr.GetColor(objColor));
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func Image getImageAt(int renderable)
deClassDynamicSkin::nfGetImageAt::nfGetImageAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getImageAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsImg){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetImageAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsImage()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassImage()->PushImage(rt, identify.CastToImage().GetImage());
}

// public func void setImageAt( int renderable, Image image )
deClassDynamicSkin::nfSetImageAt::nfSetImageAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setImageAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsImg); // image
}
void deClassDynamicSkin::nfSetImageAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	deClassDynamicSkin &clsDSkin = *((deClassDynamicSkin*)GetOwnerClass());
	deClassImage &clsImg = *clsDSkin.GetDS()->GetClassImage();
	int renderable = rt->GetValue(0)->GetInt();
	dsRealObject *objImage = rt->GetValue(1)->GetRealObject();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsImage()) DSTHROW(dueInvalidParam);
	
	identify.CastToImage().SetImage(clsImg.GetImage(objImage));
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func CanvasView getCanvasAt( int renderable )
deClassDynamicSkin::nfGetCanvasAt::nfGetCanvasAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getCanvasAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCView){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetCanvasAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCanvas()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassCanvasView()->PushCanvas(rt, identify.CastToCanvas().GetCanvas());
}

// public func void setCanvasAt( int renderable, CanvasView canvas )
deClassDynamicSkin::nfSetCanvasAt::nfSetCanvasAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setCanvasAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsCView); // canvas
}
void deClassDynamicSkin::nfSetCanvasAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	deCanvasView * const canvas = ds.GetClassCanvasView()->GetCanvas(rt->GetValue(1)->GetRealObject());
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCanvas()){
		DSTHROW(dueInvalidParam);
	}
	
	identify.CastToCanvas().SetCanvas(canvas);
	
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func void setCanvasAt(int renderable, CanvasView canvas, int componentCount)
deClassDynamicSkin::nfSetCanvasAt2::nfSetCanvasAt2(const sInitData &init) :
dsFunction(init.clsDSkin, "setCanvasAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsCView); // canvas
	p_AddParameter(init.clsInt); // componentCount
}
void deClassDynamicSkin::nfSetCanvasAt2::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	deCanvasView * const canvas = ds.GetClassCanvasView()->GetCanvas(rt->GetValue(1)->GetRealObject());
	const int componentCount = rt->GetValue(2)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCanvas()){
		DSTHROW(dueInvalidParam);
	}
	
	deDSRenderableCanvas &renderableCanvas = identify.CastToCanvas();
	renderableCanvas.SetCanvas(canvas);
	renderableCanvas.SetComponentCount(componentCount);
	
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func void setCanvasAt(int renderable, CanvasView canvas, int componentCount, int bitCount)
deClassDynamicSkin::nfSetCanvasAt3::nfSetCanvasAt3(const sInitData &init) :
dsFunction(init.clsDSkin, "setCanvasAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsCView); // canvas
	p_AddParameter(init.clsInt); // componentCount
	p_AddParameter(init.clsInt); // bitCount
}
void deClassDynamicSkin::nfSetCanvasAt3::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	deCanvasView * const canvas = ds.GetClassCanvasView()->GetCanvas(rt->GetValue(1)->GetRealObject());
	const int componentCount = rt->GetValue(2)->GetInt();
	const int bitCount = rt->GetValue(3)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCanvas()){
		DSTHROW(dueInvalidParam);
	}
	
	deDSRenderableCanvas &renderableCanvas = identify.CastToCanvas();
	renderableCanvas.SetCanvas(canvas);
	renderableCanvas.SetComponentCount(componentCount);
	renderableCanvas.SetBitCount(bitCount);
	
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func Camera getCameraAt(int renderable)
deClassDynamicSkin::nfGetCameraAt::nfGetCameraAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getCameraAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCam){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetCameraAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCamera()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassCamera()->PushCamera(rt, identify.CastToCamera().GetCamera());
}

// public func void setCameraAt( int renderable, Camera camera )
deClassDynamicSkin::nfSetCameraAt::nfSetCameraAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setCameraAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsCam); // camera
}
void deClassDynamicSkin::nfSetCameraAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	deClassDynamicSkin &clsDSkin = *((deClassDynamicSkin*)GetOwnerClass());
	deClassCamera &clsCam = *clsDSkin.GetDS()->GetClassCamera();
	int renderable = rt->GetValue(0)->GetInt();
	dsRealObject *objCamera = rt->GetValue(1)->GetRealObject();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsCamera()) DSTHROW(dueInvalidParam);
	
	identify.CastToCamera().SetCamera(clsCam.GetCamera(objCamera));
	dynamicSkin.NotifyRenderableChanged(renderable);
}

// public func VideoPlayer getVideoPlayerAt(int renderable)
deClassDynamicSkin::nfGetVideoPlayerAt::nfGetVideoPlayerAt(const sInitData &init) : dsFunction(init.clsDSkin,
"getVideoPlayerAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVP){
	p_AddParameter(init.clsInt); // renderable
}
void deClassDynamicSkin::nfGetVideoPlayerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	const deScriptingDragonScript &ds = *(((deClassDynamicSkin*)GetOwnerClass())->GetDS());
	const int renderable = rt->GetValue(0)->GetInt();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsVideoFrame()){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetClassVideoPlayer()->PushVideoPlayer(rt, identify.CastToVideoFrame().GetVideoPlayer());
}

// public func void setVideoPlayerAt( int renderable, VideoPlayer videoPlayer )
deClassDynamicSkin::nfSetVideoPlayerAt::nfSetVideoPlayerAt(const sInitData &init) : dsFunction(init.clsDSkin,
"setVideoPlayerAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // renderable
	p_AddParameter(init.clsVP); // videoPlayer
}
void deClassDynamicSkin::nfSetVideoPlayerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin &dynamicSkin = *(((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin);
	deClassDynamicSkin &clsDSkin = *((deClassDynamicSkin*)GetOwnerClass());
	deClassVideoPlayer &clsVP = *clsDSkin.GetDS()->GetClassVideoPlayer();
	int renderable = rt->GetValue(0)->GetInt();
	dsRealObject *objVideoPlayer = rt->GetValue(1)->GetRealObject();
	
	deDSRenderableVisitorIdentify identify;
	
	dynamicSkin.GetRenderableAt(renderable)->Visit(identify);
	if(!identify.IsVideoFrame()) DSTHROW(dueInvalidParam);
	
	identify.CastToVideoFrame().SetVideoPlayer(clsVP.GetVideoPlayer(objVideoPlayer));
	dynamicSkin.NotifyRenderableChanged(renderable);
}



// public func int hashCode()
deClassDynamicSkin::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsDSkin,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassDynamicSkin::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin::Ref dynamicSkin = ((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin;
	
	rt->PushInt((int)(intptr_t)dynamicSkin);
}

// public func bool equals( Object obj )
deClassDynamicSkin::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsDSkin,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassDynamicSkin::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deDynamicSkin::Ref dynamicSkin = ((sDSkinNatDat*)p_GetNativeData(myself))->dynamicSkin;
	deClassDynamicSkin *clsDSkin = (deClassDynamicSkin*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsDSkin)){
		rt->PushBool(false);
		
	}else{
		deDynamicSkin *otherDSkin = ((sDSkinNatDat*)p_GetNativeData(obj))->dynamicSkin;
		
		rt->PushBool(dynamicSkin == otherDSkin);
	}
}



// Class deClassDynamicSkin
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassDynamicSkin::deClassDynamicSkin(deScriptingDragonScript *ds) :
dsClass("DynamicSkin", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sDSkinNatDat));
}

deClassDynamicSkin::~deClassDynamicSkin(){
}



// Management
///////////////

void deClassDynamicSkin::CreateClassMembers(dsEngine *engine){
	pClsDynamicSkinRenderableType = engine->GetClass("Dragengine.Scenery.DynamicSkinRenderableType");
	
	sInitData init;
	init.clsDSkin = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsClr = pDS->GetClassColor();
	init.clsImg = pDS->GetClassImage();
	init.clsCam = pDS->GetClassCamera();
	init.clsVP = pDS->GetClassVideoPlayer();
	init.clsCView = pDS->GetClassCanvasView();
	init.clsDynamicSkinRenderableType = pClsDynamicSkinRenderableType;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetRenderableCount(init));
	AddFunction(new nfIndexOfRenderable(init));
	AddFunction(new nfHasRenderable(init));
	AddFunction(new nfAddRenderable(init));
	AddFunction(new nfRemoveRenderable(init));
	AddFunction(new nfRemoveAllRenderables(init));
	
	AddFunction(new nfGetTypeAt(init));
	
	AddFunction(new nfGetValueAt(init));
	AddFunction(new nfSetValueAt(init));
	
	AddFunction(new nfGetColorAt(init));
	AddFunction(new nfSetColorAt(init));
	
	AddFunction(new nfGetImageAt(init));
	AddFunction(new nfSetImageAt(init));
	
	AddFunction(new nfGetCanvasAt(init));
	AddFunction(new nfSetCanvasAt(init));
	AddFunction(new nfSetCanvasAt2(init));
	AddFunction(new nfSetCanvasAt3(init));
	
	AddFunction(new nfGetCameraAt(init));
	AddFunction(new nfSetCameraAt(init));
	
	AddFunction(new nfGetVideoPlayerAt(init));
	AddFunction(new nfSetVideoPlayerAt(init));
	
	AddFunction(new nfHashCode(init));
	AddFunction(new nfEquals(init));
}

deDynamicSkin *deClassDynamicSkin::GetDynamicSkin(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sDSkinNatDat*)p_GetNativeData(myself->GetBuffer()))->dynamicSkin;
}

void deClassDynamicSkin::PushDynamicSkin(dsRunTime *rt, deDynamicSkin::Ref dynamicSkin){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!dynamicSkin){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sDSkinNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->dynamicSkin = dynamicSkin;
}
