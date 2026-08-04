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
#include "deClassRig.h"
#include "../dedsHelpers.h"
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
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/systems/modules/physics/deBasePhysicsModel.h>

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
	sMdlNatDat &nd = dedsNewNativeData<sMdlNatDat>(p_GetNativeData(myself));
	
	deScriptingDragonScript &ds = *static_cast<deClassModel*>(GetOwnerClass())->GetDS();
	deModelManager &mdlMgr = *ds.GetGameEngine()->GetModelManager();
	
	nd.model = mdlMgr.LoadModel(rt->GetValue(0)->GetString(), "/");
}

// static func void loadAsynchron( String filename, ResourceListener listener )
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
	
	dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).~sMdlNatDat();
}



// func string GetFilename()
deClassModel::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsMdl,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassModel::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushString(model.GetFilename());
}



// func int getLodCount()
deClassModel::nfGetLodCount::nfGetLodCount(const sInitData &init) : dsFunction(init.clsMdl,
"getLodCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModel::nfGetLodCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushInt(model.GetLODCount());
}

// func int getTextureCount()
deClassModel::nfGetTextureCount::nfGetTextureCount(const sInitData &init) : dsFunction(init.clsMdl,
"getTextureCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModel::nfGetTextureCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushInt(model.GetTextureCount());
}

// func String getTextureNameAt( int texture )
deClassModel::nfGetTextureNameAt::nfGetTextureNameAt(const sInitData &init) : dsFunction(init.clsMdl,
"getTextureNameAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // texture
}
void deClassModel::nfGetTextureNameAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const int texture = rt->GetValue(0)->GetInt();
	
	rt->PushString(model.GetTextureAt(texture)->GetName());
}

// func int getFaceCount( int lod )
deClassModel::nfGetFaceCount::nfGetFaceCount(const sInitData &init) : dsFunction(init.clsMdl,
"getFaceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // lod
}
void deClassModel::nfGetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const int lod = rt->GetValue(0)->GetInt();
	
	rt->PushInt(model.GetLODAt(lod)->GetFaces().GetCount());
}

// func int getVertexCount( int lod )
deClassModel::nfGetVertexCount::nfGetVertexCount(const sInitData &init) : dsFunction(init.clsMdl,
"getVertexCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // lod
}
void deClassModel::nfGetVertexCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const int lod = rt->GetValue(0)->GetInt();
	
	rt->PushInt(model.GetLODAt(lod)->GetVertices().GetCount());
}

// func Vector getMinimumExtend()
deClassModel::nfGetMinimumExtend::nfGetMinimumExtend(const sInitData &init) :
dsFunction(init.clsMdl, "getMinimumExtend", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassModel::nfGetMinimumExtend::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const deScriptingDragonScript &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	const deModelLOD &lod = model.GetLODAt(0);
	
	decVector extend;
	if(lod.GetVertices().IsNotEmpty()){
		extend = lod.GetVertices().First().GetPosition();
		lod.GetVertices().Visit(1, lod.GetVertices().GetCount() - 1, [&](const deModelVertex &vertex){
			extend.SetSmallest(vertex.GetPosition());
		});
	}
	
	ds.GetClassVector()->PushVector(rt, extend);
}

// func Vector getMaximumExtend()
deClassModel::nfGetMaximumExtend::nfGetMaximumExtend(const sInitData &init) :
dsFunction(init.clsMdl, "getMaximumExtend", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassModel::nfGetMaximumExtend::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const deScriptingDragonScript &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	const deModelLOD &lod = model.GetLODAt(0);
	
	decVector extend;
	if(lod.GetVertices().IsNotEmpty()){
		extend = lod.GetVertices().First().GetPosition();
		lod.GetVertices().Visit(1, lod.GetVertices().GetCount() - 1, [&](const deModelVertex &vertex){
			extend.SetLargest(vertex.GetPosition());
		});
	}
	
	ds.GetClassVector()->PushVector(rt, extend);
}



// func int getVertexPositionSetCount()
deClassModel::nfGetVertexPositionSetCount::nfGetVertexPositionSetCount(const sInitData &init) :
dsFunction(init.clsMdl, "getVertexPositionSetCount",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassModel::nfGetVertexPositionSetCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushInt(model.GetVertexPositionSetCount());
}

// func int indexOfVertexPositionSetNamed(String name)
deClassModel::nfIndexOfVertexPositionSetNamed::nfIndexOfVertexPositionSetNamed(const sInitData &init) :
dsFunction(init.clsMdl, "indexOfVertexPositionSetNamed",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsStr); // name
}
void deClassModel::nfIndexOfVertexPositionSetNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushInt(model.IndexOfVertexPositionSetNamed(rt->GetValue(0)->GetString()));
}

// func String vertexPositionSetGetNameAt(int index)
deClassModel::nfVertexPositionSetGetNameAt::nfVertexPositionSetGetNameAt(const sInitData &init) :
dsFunction(init.clsMdl, "vertexPositionSetGetNameAt",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
	p_AddParameter(init.clsInt); // index
}
void deClassModel::nfVertexPositionSetGetNameAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deModel &model = *dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	
	rt->PushString(model.GetVertexPositionSetAt(rt->GetValue(0)->GetInt())->GetName());
}


// func Rig generateCollisionShapes()
deClassModel::nfGenerateCollisionShapes::nfGenerateCollisionShapes(const sInitData &init) :
dsFunction(init.clsMdl, "generateCollisionShapes", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsRig){
}

void deClassModel::nfGenerateCollisionShapes::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &model = dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const auto &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	
	deRig::Ref rig;
	
	auto peer = model->GetPeerPhysics();
	if(peer){
		rig = peer->GenerateCollisionShapes(1.0f, 0.45f);
	}
	
	ds.GetClassRig()->PushRig(rt, rig);
}

// func Rig generateCollisionShapes(float convexHullThreshold, float weightThreshold)
deClassModel::nfGenerateCollisionShapes2::nfGenerateCollisionShapes2(const sInitData &init) :
dsFunction(init.clsMdl, "generateCollisionShapes", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsRig){
	p_AddParameter(init.clsFloat); // convexHullThreshold
	p_AddParameter(init.clsFloat); // weightThreshold
}

void deClassModel::nfGenerateCollisionShapes2::RunFunction(dsRunTime *rt, dsValue *myself){
	const auto &model = dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself)).model;
	const auto &ds = *(static_cast<deClassModel*>(GetOwnerClass()))->GetDS();
	
	const float convexHullThreshold = rt->GetValue(0)->GetFloat();
	const float weightThreshold = rt->GetValue(1)->GetFloat();
	deRig::Ref rig;
	
	auto peer = model->GetPeerPhysics();
	if(peer){
		rig = peer->GenerateCollisionShapes(convexHullThreshold, weightThreshold);
	}
	
	ds.GetClassRig()->PushRig(rt, rig);
}


// func int hashCode()
deClassModel::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsMdl, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassModel::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sMdlNatDat &nd = dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself));
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)nd.model);
}

// func bool equals( Object obj )
deClassModel::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsMdl, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassModel::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sMdlNatDat &nd = dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself));
	const deClassModel * const clsModel = static_cast<deClassModel*>(GetOwnerClass());
	dsValue * const obj = rt->GetValue(0);
	
	if(obj->GetType()->GetPrimitiveType() != DSPT_OBJECT || !obj->GetRealObject()
	|| obj->GetRealObject()->GetType() != clsModel){
		rt->PushBool(false);
		
	}else{
		const sMdlNatDat &other = dedsGetNativeData<sMdlNatDat>(p_GetNativeData(obj));
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
	
	p_SetNativeDataSize(dedsNativeDataSize<sMdlNatDat>());
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
	init.clsFloat = engine->GetClassFloat();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS->GetClassVector();
	init.clsRN = pDS->GetClassResourceListener();
	init.clsRig = pDS->GetClassRig();
	
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
	
	AddFunction(new nfGetVertexPositionSetCount(init));
	AddFunction(new nfIndexOfVertexPositionSetNamed(init));
	AddFunction(new nfVertexPositionSetGetNameAt(init));
	
	AddFunction(new nfGenerateCollisionShapes(init));
	AddFunction(new nfGenerateCollisionShapes2(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
}

deModel *deClassModel::GetModel(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	
	return dedsGetNativeData<sMdlNatDat>(p_GetNativeData(myself->GetBuffer())).model;
}

void deClassModel::PushModel(dsRunTime *rt, deModel *model){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!model){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	dedsNewNativeData<sMdlNatDat>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())).model = model;
}
