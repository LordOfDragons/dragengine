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

#include "deClassModel.h"
#include "../resources/deClassResourceListener.h"
#include "../math/deClassVector.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelVertex.h>

#include <libdscript/exceptions.h>


struct sMdlNatDat{
	deModel::Ref model;
};



// Native Functions
/////////////////////

// public constructor Load(string filename)
deClassModel::nfLoad::nfLoad(const sInitData &init) : dsFunction(init.clsMdl,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassModel::nfLoad::RunFunction(dsRunTime *rt, dsValue *myself){
	sMdlNatDat &nd = *static_cast<sMdlNatDat*>(p_GetNativeData(myself));
	deScriptingDragonScript &ds = *static_cast<deClassModel*>(GetOwnerClass())->GetDS();
	deModelManager &mdlMgr = *ds.GetGameEngine()->GetModelManager();
	
	// prepare
	nd.model = nullptr;
	
	// load model
	const char * const filename = rt->GetValue(0)->GetString();
	nd.model = mdlMgr.LoadModel(filename, "/");
	if(!nd.model){
		DSTHROW(dueInvalidParam);
	}
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassModel::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) : dsFunction(init.clsMdl,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
	p_AddParameter(init.clsRN); // listener
}
void deClassModel::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	
	if(!listener){
		DSTHROW(dueNullPointer);
	}
	
	ds.GetResourceLoader()->AddRequest(filename, deResourceLoader::ertModel, listener);
}

// public destructor Destructor()
deClassModel::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsMdl,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassModel::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sMdlNatDat*>(p_GetNativeData(myself))->~sMdlNatDat();
}



// public func string GetFilename()
deClassModel::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsMdl,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModel::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	
	rt->PushString(model.GetFilename());
}



// public func int getLodCount()
deClassModel::nfGetLodCount::nfGetLodCount(const sInitData &init) : dsFunction(init.clsMdl,
"getLodCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModel::nfGetLodCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	
	rt->PushInt(model.GetLODCount());
}

// public func int getTextureCount()
deClassModel::nfGetTextureCount::nfGetTextureCount(const sInitData &init) : dsFunction(init.clsMdl,
"getTextureCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModel::nfGetTextureCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	
	rt->PushInt(model.GetTextureCount());
}

// public func String getTextureNameAt( int texture )
deClassModel::nfGetTextureNameAt::nfGetTextureNameAt(const sInitData &init) : dsFunction(init.clsMdl,
"getTextureNameAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // texture
}
void deClassModel::nfGetTextureNameAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	const int texture = rt->GetValue(0)->GetInt();
	
	rt->PushString(model.GetTextureAt(texture)->GetName());
}

// public func int getFaceCount( int lod )
deClassModel::nfGetFaceCount::nfGetFaceCount(const sInitData &init) : dsFunction(init.clsMdl,
"getFaceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // lod
}
void deClassModel::nfGetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	const int lod = rt->GetValue(0)->GetInt();
	
	rt->PushInt(model.GetLODAt(lod)->GetFaceCount());
}

// public func int getVertexCount( int lod )
deClassModel::nfGetVertexCount::nfGetVertexCount(const sInitData &init) : dsFunction(init.clsMdl,
"getVertexCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // lod
}
void deClassModel::nfGetVertexCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	const int lod = rt->GetValue(0)->GetInt();
	
	rt->PushInt(model.GetLODAt(lod)->GetVertexCount());
}

// public func Vector getMinimumExtend()
deClassModel::nfGetMinimumExtend::nfGetMinimumExtend(const sInitData &init) :
dsFunction(init.clsMdl, "getMinimumExtend", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassModel::nfGetMinimumExtend::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	const deScriptingDragonScript &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	const deModelLOD &lod = *model.GetLODAt(0);
	
	const int count = lod.GetVertexCount();
	decVector extend;
	
	if(count > 0){
		const deModelVertex * const vertices = lod.GetVertices();
		int i;
		
		extend = vertices[0].GetPosition();
		
		for(i=1; i<count; i++){
			extend.SetSmallest(vertices[i].GetPosition());
		}
	}
	
	ds.GetClassVector()->PushVector(rt, extend);
}

// public func Vector getMaximumExtend()
deClassModel::nfGetMaximumExtend::nfGetMaximumExtend(const sInitData &init) :
dsFunction(init.clsMdl, "getMaximumExtend", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassModel::nfGetMaximumExtend::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *static_cast<sMdlNatDat*>(p_GetNativeData(myself))->model;
	const deScriptingDragonScript &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	const deModelLOD &lod = *model.GetLODAt(0);
	
	const int count = lod.GetVertexCount();
	decVector extend;
	
	if(count > 0){
		const deModelVertex * const vertices = lod.GetVertices();
		int i;
		
		extend = vertices[0].GetPosition();
		
		for(i=1; i<count; i++){
			extend.SetLargest(vertices[i].GetPosition());
		}
	}
	
	ds.GetClassVector()->PushVector(rt, extend);
}



// public func int hashCode()
deClassModel::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsMdl, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassModel::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	sMdlNatDat &nd = *static_cast<sMdlNatDat*>(p_GetNativeData(myself));
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)nd.model);
}

// public func bool equals( Object obj )
deClassModel::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsMdl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassModel::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sMdlNatDat &nd = *static_cast<sMdlNatDat*>(p_GetNativeData(myself));
	deClassModel * const clsModel = static_cast<deClassModel*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(obj->GetType()->GetPrimitiveType() != DSPT_OBJECT || !obj->GetRealObject()
	|| obj->GetRealObject()->GetType() != clsModel){
		rt->PushBool(false);
		
	}else{
		const sMdlNatDat &other = *static_cast<sMdlNatDat*>(p_GetNativeData(obj));
		rt->PushBool(nd.model == other.model);
	}
}




// Class deClassModel
///////////////////////

// Constructor, destructor
////////////////////////////

deClassModel::deClassModel(deScriptingDragonScript *ds) : dsClass("Model",
DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds){
		DSTHROW(dueInvalidParam);
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMdlNatDat));
}

deClassModel::~deClassModel(){
}



// Management
///////////////

void deClassModel::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsMdl = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsRN = pDS->GetClassResourceListener();
	
	// add functions
	AddFunction(new nfLoad(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfGetLodCount(init));
	AddFunction(new nfGetTextureCount(init));
	AddFunction(new nfGetTextureNameAt(init));
	AddFunction(new nfGetFaceCount(init));
	AddFunction(new nfGetVertexCount(init));
	AddFunction(new nfGetMinimumExtend(init));
	AddFunction(new nfGetMaximumExtend(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
}

deModel *deClassModel::GetModel(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return static_cast<sMdlNatDat*>(p_GetNativeData(myself->GetBuffer()))->model;
}

void deClassModel::PushModel(dsRunTime *rt, deModel *model){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!model){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sMdlNatDat)->model = model;
}
