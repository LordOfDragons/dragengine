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
#include <stdint.h>
#include <string.h>

#include "deClassVideo.h"
#include "../resources/deClassResourceListener.h"
#include "../math/deClassPoint.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <libdscript/exceptions.h>



// native structure
struct sVidNatDat{
	deVideo::Ref video;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassVideo::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsVid,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassVideo::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sVidNatDat * const nd = new (p_GetNativeData(myself)) sVidNatDat;
	deClassVideo &clsVideo = *(static_cast<deClassVideo*>(GetOwnerClass()));
	deVideoManager &vidmgr = *clsVideo.GetDS()->GetGameEngine()->GetVideoManager();
	
	const char *filename = rt->GetValue(0)->GetString();
	
	// prepare
	
	// load sound
	nd->video = vidmgr.LoadVideo(filename, "/", false);
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassVideo::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) :
dsFunction(init.clsVid, "loadAsynchron", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
	p_AddParameter(init.clsResNot); // listener
}
void deClassVideo::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *(static_cast<deClassVideo*>(GetOwnerClass()))->GetDS();
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	
	if(!listener){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetResourceLoader()->AddRequest(filename, deResourceLoader::ertVideo, listener);
}

// public func destructor()
deClassVideo::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsVid,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassVideo::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sVidNatDat*>(p_GetNativeData(myself))->~sVidNatDat();
}



// Management
///////////////

// public func String getFilename()
deClassVideo::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsVid,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassVideo::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushString(video.GetFilename());
}

// public func int getWidth()
deClassVideo::nfGetWidth::nfGetWidth(const sInitData &init) : dsFunction(init.clsVid,
"getWidth", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassVideo::nfGetWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushInt(video.GetWidth());
}

// public func int getHeight()
deClassVideo::nfGetHeight::nfGetHeight(const sInitData &init) : dsFunction(init.clsVid,
"getHeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassVideo::nfGetHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushInt(video.GetHeight());
}

// public func Point getSize()
deClassVideo::nfGetSize::nfGetSize(const sInitData &init) : dsFunction(init.clsVid,
"getSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint){
}
void deClassVideo::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	deScriptingDragonScript &ds = *(static_cast<deClassVideo*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassPoint()->PushPoint(rt, decPoint(video.GetWidth(), video.GetHeight()));
}

// public func int getFrameCount()
deClassVideo::nfGetFrameCount::nfGetFrameCount(const sInitData &init) : dsFunction(init.clsVid,
"getFrameCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassVideo::nfGetFrameCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushInt(video.GetFrameCount());
}

// public func float getFrameRate()
deClassVideo::nfGetFrameRate::nfGetFrameRate(const sInitData &init) : dsFunction(init.clsVid,
"getFrameRate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassVideo::nfGetFrameRate::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushFloat(video.GetFrameRate());
}

// public func float getPlayTime()
deClassVideo::nfGetPlayTime::nfGetPlayTime(const sInitData &init) : dsFunction(init.clsVid,
"getPlayTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassVideo::nfGetPlayTime::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo &video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushFloat(video.GetPlayTime());
}



// Common
///////////

// public func int hashCode()
deClassVideo::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsVid, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassVideo::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo *video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	
	rt->PushInt((int)(intptr_t)video);
}

// public func bool equals( Object object )
deClassVideo::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsVid, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassVideo::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deVideo * const video = static_cast<sVidNatDat*>(p_GetNativeData(myself))->video;
	deClassVideo *clsVideo = static_cast<deClassVideo*>(GetOwnerClass());
	dsValue *object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsVideo)){
		rt->PushBool(false);
		
	}else{
		deVideo * const otherVideo = static_cast<sVidNatDat*>(p_GetNativeData(object))->video;
		
		rt->PushBool(video == otherVideo);
	}
}

// static public func bool equals( Video video1, Video video2 )
deClassVideo::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsVid, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsVid); // video1
	p_AddParameter(init.clsVid); // video2
}
void deClassVideo::nfEquals2::RunFunction(dsRunTime *rt, dsValue*){
	const deClassVideo &clsVideo = *(static_cast<deClassVideo*>(GetOwnerClass()));
	const deVideo * const video1 = clsVideo.GetVideo(rt->GetValue(0)->GetRealObject());
	const deVideo * const video2 = clsVideo.GetVideo(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(video1 == video2);
}



// Class deClassVideo
///////////////////////

// Constructor, Destructor
////////////////////////////

deClassVideo::deClassVideo(deScriptingDragonScript *ds) :
dsClass("Video", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds) DSTHROW(dueInvalidParam);
	
	// prepare
	pDS = ds;
	
	// set parser info
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	
	// do the rest
	p_SetNativeDataSize(sizeof(sVidNatDat));
}

deClassVideo::~deClassVideo(){
}



// Management
///////////////

void deClassVideo::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsVid = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsObj = engine->GetClassObject();
	init.clsRN = pDS->GetClassResourceListener();
	init.clsPoint = pDS->GetClassPoint();
	
	init.clsResNot = pDS->GetClassResourceListener();
	
	// add functions
	AddFunction(new nfNew (init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetWidth(init));
	AddFunction(new nfGetHeight(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfGetFrameCount(init));
	AddFunction(new nfGetFrameRate(init));
	AddFunction(new nfGetPlayTime(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
}

deVideo *deClassVideo::GetVideo(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sVidNatDat*>(p_GetNativeData(myself->GetBuffer()))->video;
}

void deClassVideo::PushVideo(dsRunTime *rt, deVideo *video){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!video){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sVidNatDat)->video = video;
}
