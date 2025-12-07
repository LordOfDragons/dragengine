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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "deClassImage.h"
#include "../resources/deClassResourceListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"
#include "dragengine/resources/image/deImage.h"
#include "dragengine/resources/image/deImageManager.h"
#include "dragengine/resources/loader/deResourceLoader.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sImgNatDat{
	deImage *image;
};

// native functions
/////////////////////

// public func new(String filename)
deClassImage::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsImg,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassImage::nfNew::RunFunction(dsRunTime *RT, dsValue *This){
	sImgNatDat *nd = (sImgNatDat*)p_GetNativeData(This);
	deClassImage *clsImg = (deClassImage*)GetOwnerClass();
	deImageManager *imgMgr = clsImg->GetGameEngine()->GetImageManager();
	// reset all
	nd->image = NULL;
	// load image
	const char *filename = RT->GetValue(0)->GetString();
	nd->image = imgMgr->LoadImage(filename, "/");
	if(!nd->image) DSTHROW(dueInvalidParam);
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassImage::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) : dsFunction(init.clsImg,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
	p_AddParameter(init.clsRN); // listener
}
void deClassImage::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassImage *clsImg = (deClassImage*)GetOwnerClass();
	
	const char *filename = rt->GetValue(0)->GetString();
	dsRealObject *listener = rt->GetValue(1)->GetRealObject();
	
	if(! listener) DSTHROW(dueInvalidParam);
	
	clsImg->GetScriptModule()->GetResourceLoader()->AddRequest(filename,
		deResourceLoader::ertImage, listener);
}

// public func destructor()
deClassImage::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsImg,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassImage::nfDestructor::RunFunction(dsRunTime *RT, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sImgNatDat *nd = (sImgNatDat*)p_GetNativeData(myself);
	
	if(nd->image){
		nd->image->FreeReference();
		nd->image = NULL;
	}
}



// public func String getFilename()
deClassImage::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsImg,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassImage::nfGetFilename::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	RT->PushString(image->GetFilename());
}

// public func int getWidth()
deClassImage::nfGetWidth::nfGetWidth(const sInitData &init) : dsFunction(init.clsImg,
"getWidth", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassImage::nfGetWidth::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	RT->PushInt(image->GetWidth());
}

// public func int getHeight()
deClassImage::nfGetHeight::nfGetHeight(const sInitData &init) : dsFunction(init.clsImg,
"getHeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassImage::nfGetHeight::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	RT->PushInt(image->GetHeight());
}

// public func int getDepth()
deClassImage::nfGetDepth::nfGetDepth(const sInitData &init) : dsFunction(init.clsImg,
"getDepth", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassImage::nfGetDepth::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*) p_GetNativeData(This))->image;
	RT->PushInt(image->GetDepth());
}

// public func int getComponentCount()
deClassImage::nfGetComponentCount::nfGetComponentCount(const sInitData &init) : dsFunction(init.clsImg,
"getComponentCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassImage::nfGetComponentCount::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	RT->PushInt(image->GetComponentCount());
}

// public func int getBitCount()
deClassImage::nfGetBitCount::nfGetBitCount(const sInitData &init) : dsFunction(init.clsImg,
"getBitCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassImage::nfGetBitCount::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	RT->PushInt(image->GetBitCount());
}

// public func Point getSize()
deClassImage::nfGetSize::nfGetSize(const sInitData &init) : dsFunction(init.clsImg,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt){
}
void deClassImage::nfGetSize::RunFunction(dsRunTime *RT, dsValue *This){
	deImage *image = ((sImgNatDat*)p_GetNativeData(This))->image;
	deClassImage *clsImg = (deClassImage*)GetOwnerClass();
	clsImg->GetScriptModule()->PushPoint(RT, 
		decPoint(image->GetWidth(), image->GetHeight()));
}

// public func void saveToFile(String filename)
deClassImage::nfSaveToFile::nfSaveToFile(const sInitData &init) : dsFunction(init.clsImg,
"saveToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassImage::nfSaveToFile::RunFunction(dsRunTime *RT, dsValue *This){
	sImgNatDat *nd = (sImgNatDat*)p_GetNativeData(This);
	deClassImage *clsImg = (deClassImage*)GetOwnerClass();
	deImageManager *imgMgr = clsImg->GetGameEngine()->GetImageManager();
	deScriptingDragonScript &ds = *clsImg->GetScriptModule();
	
	// save image
	const char *filename = RT->GetValue(0)->GetString();
	//ds.LogInfoFormat( "Image: saving %s", filename );
	try{
		imgMgr->SaveImage(nd->image, filename);
		
	}catch(const duException &){
		ds.LogInfoFormat("Image: saving %s failed.", filename);
		throw;
	}
}



// public func int hashCode()
deClassImage::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsImg, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassImage::nfHashCode::RunFunction(dsRunTime *RT, dsValue *This){
	sImgNatDat *nd = (sImgNatDat*)p_GetNativeData(This);
	// hash code = memory location
	RT->PushInt((int)(intptr_t)nd->image);
}

// public func bool equals(Object obj)
deClassImage::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsImg, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassImage::nfEquals::RunFunction(dsRunTime *RT, dsValue *This){
	sImgNatDat *nd = (sImgNatDat*)p_GetNativeData(This);
	deClassImage *clsImg = (deClassImage*)GetOwnerClass();
	dsValue *obj = RT->GetValue(0);
	// check if the object is a map and not null
	if(!p_IsObjOfType(obj, clsImg)){
		RT->PushBool(false);
	// compare pointers
	}else{
		sImgNatDat *other = (sImgNatDat*)p_GetNativeData(obj);
		RT->PushBool(nd->image == other->image);
	}
}

// static public func bool equals( Image image1, Image image2 )
deClassImage::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsImg, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsImg); // image1
	p_AddParameter(init.clsImg); // image2
}
void deClassImage::nfEquals2::RunFunction(dsRunTime *rt, dsValue*){
	const deClassImage &clsImage = *((deClassImage*)GetOwnerClass());
	const deImage * const image1 = clsImage.GetImage(rt->GetValue(0)->GetRealObject());
	const deImage * const image2 = clsImage.GetImage(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(image1 == image2);
}



// class deClassImage
///////////////////////
// constructor
deClassImage::deClassImage(deEngine *GameEngine, deScriptingDragonScript *ScrMgr) :
dsClass("Image", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!GameEngine || !ScrMgr) DSTHROW(dueInvalidParam);
	// prepare
	p_gameEngine = GameEngine;
	pScrMgr = ScrMgr;
	// set parser info
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	// do the rest
	p_SetNativeDataSize(sizeof(sImgNatDat));
}
deClassImage::~deClassImage(){
}
// management
void deClassImage::CreateClassMembers(dsEngine *engine){
	sInitData init;
	// store classes
	pClsRN = pScrMgr->GetClassResourceListener();
	init.clsImg = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsPt = engine->GetClass(DECN_POINT);
	init.clsRN = pClsRN;
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetWidth(init));
	AddFunction(new nfGetHeight(init));
	AddFunction(new nfGetDepth(init));
	AddFunction(new nfGetComponentCount(init));
	AddFunction(new nfGetBitCount(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfSaveToFile(init));
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
	// calculate member offsets
	CalcMemberOffsets();
}

deImage *deClassImage::GetImage(dsRealObject *myself) const{
	if(! myself){
		return NULL;
	}
	
	return ((sImgNatDat*)p_GetNativeData(myself->GetBuffer()))->image;
}

void deClassImage::PushImage(dsRunTime *rt, deImage *image){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(! image){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sImgNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->image = image;
	image->AddReference();
}
