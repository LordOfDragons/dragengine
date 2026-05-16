/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deClassScene.h"
#include "deClassModel.h"
#include "deClassRig.h"
#include "deClassSkin.h"
#include "../dedsHelpers.h"
#include "../resources/deClassResourceListener.h"
#include "../animation/deClassAnimation.h"
#include "../file/deClassMemoryFile.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/scene/deScene.h>
#include <dragengine/resources/scene/deSceneManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>

#include <libdscript/exceptions.h>


// Native structure
struct sSceneNatDat{
	deScene::Ref scene;
};


// Native Functions
/////////////////////

// func new(String filename)
deClassScene::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsScene,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
}
void deClassScene::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSceneNatDat &nd = dedsNewNativeData<sSceneNatDat>(p_GetNativeData(myself));
	
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	deSceneManager &sceneMgr = *clsScene.GetDS().GetGameEngine()->GetSceneManager();
	
	nd.scene = sceneMgr.LoadScene(rt->GetValue(0)->GetString(), "/");
}

// static func void loadAsynchron(String filename, ResourceListener listener)
deClassScene::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) : dsFunction(init.clsScene,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // filename
	p_AddParameter(init.clsRN); // listener
}
void deClassScene::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue*){
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	if(!listener){
		DSTHROW(dueNullPointer);
	}
	
	clsScene.GetDS().GetResourceLoader()->AddRequest(filename, deResourceLoader::ertScene, listener);
}

// func destructor()
deClassScene::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsScene,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassScene::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).~sSceneNatDat();
}



// func String getFilename()
deClassScene::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsScene,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassScene::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushString(scene.GetFilename());
}



// func int getResourceCount()
deClassScene::nfGetResourceCount::nfGetResourceCount(const sInitData &init) :
dsFunction(init.clsScene, "getResourceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsInt){
}
void deClassScene::nfGetResourceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushInt(scene.GetResources().GetCount());
}

// func Array getResourceKeys()
deClassScene::nfGetResourceKeys::nfGetResourceKeys(const sInitData &init) :
dsFunction(init.clsScene, "getResourceKeys", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsArray){
}
void deClassScene::nfGetResourceKeys::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	
	dsClass * const clsArray = rt->GetEngine()->GetClassArray();
	dsValue * const arr = rt->CreateValue(clsArray);
	const auto keys = scene.GetResources().GetKeys();
	
	try{
		rt->CreateObject(arr, clsArray, 0);
		
		keys.Visit([&](const decString &key){
			rt->PushString(key);
			rt->RunFunction(arr, "add", 1);
		});
		
		rt->PushValue(arr);
		rt->FreeValue(arr);
		
	}catch(...){
		rt->FreeValue(arr);
		throw;
	}
}

// func Object getResourceAt(String name)
deClassScene::nfGetResourceAt::nfGetResourceAt(const sInitData &init) :
dsFunction(init.clsScene, "getResourceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsObj){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfGetResourceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	const deScriptingDragonScript &ds = clsScene.GetDS();
	
	const deResource::Ref *refRes;
	if(!scene.GetResources().GetAt(rt->GetValue(0)->GetString(), refRes)){
		rt->PushObject(nullptr, clsScene.GetDS().GetScriptEngine()->GetClassObject());
		return;
	}
	
	auto resource = refRes->Pointer();
	switch(resource->GetResourceManager()->GetResourceType()){
	case deResourceManager::ertModel:
		ds.GetClassModel()->PushModel(rt, (deModel*)resource);
		break;
		
	case deResourceManager::ertSkin:
		ds.GetClassSkin()->PushSkin(rt, (deSkin*)resource);
		break;
		
	case deResourceManager::ertRig:
		ds.GetClassRig()->PushRig(rt, (deRig*)resource);
		break;
		
	case deResourceManager::ertAnimation:
		ds.GetClassAnimation()->PushAnimation(rt, (deAnimation*)resource);
		break;
		
	default:
		rt->PushObject(nullptr, clsScene.GetDS().GetScriptEngine()->GetClassObject());
	}
}

// func bool hasResourceAt(String name)
deClassScene::nfHasResourceAt::nfHasResourceAt(const sInitData &init) :
dsFunction(init.clsScene, "hasResourceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsBool){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfHasResourceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushBool(scene.GetResources().Has(rt->GetValue(0)->GetString()));
}

// func void addResource(String name, Object resource)
deClassScene::nfAddResource::nfAddResource(const sInitData &init) :
dsFunction(init.clsScene, "addResource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsObj); // resource
}
void deClassScene::nfAddResource::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	const deScriptingDragonScript &ds = clsScene.GetDS();
	
	const char * const name = rt->GetValue(0)->GetString();
	dsRealObject * const resObj = rt->GetValue(1)->GetRealObject();
	if(!resObj){
		DSTHROW(dueNullPointer);
	}
	
	// determine resource type from the object's class
	deResource *resource = nullptr;
	if(resObj->GetType() == ds.GetClassModel()){
		resource = ds.GetClassModel()->GetModel(resObj);
		
	}else if(resObj->GetType() == ds.GetClassSkin()){
		resource = ds.GetClassSkin()->GetSkin(resObj);
		
	}else if(resObj->GetType() == ds.GetClassRig()){
		resource = ds.GetClassRig()->GetRig(resObj);
		
	}else if(resObj->GetType() == ds.GetClassAnimation()){
		resource = ds.GetClassAnimation()->GetAnimation(resObj);
	}
	
	if(!resource){
		DSTHROW(dueInvalidParam);
	}
	
	scene.AddResource(name, resource);
}

// func void removeResource(String name)
deClassScene::nfRemoveResource::nfRemoveResource(const sInitData &init) :
dsFunction(init.clsScene, "removeResource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfRemoveResource::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	scene.RemoveResource(rt->GetValue(0)->GetString());
}

// func void removeAllResources()
deClassScene::nfRemoveAllResources::nfRemoveAllResources(const sInitData &init) :
dsFunction(init.clsScene, "removeAllResources", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
}
void deClassScene::nfRemoveAllResources::RunFunction(dsRunTime*, dsValue *myself){
	dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene->RemoveAllResources();
}



// func int getFileCount()
deClassScene::nfGetFileCount::nfGetFileCount(const sInitData &init) :
dsFunction(init.clsScene, "getFileCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsInt){
}
void deClassScene::nfGetFileCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushInt(scene.GetFiles().GetCount());
}

// func Array getFileKeys()
deClassScene::nfGetFileKeys::nfGetFileKeys(const sInitData &init) :
dsFunction(init.clsScene, "getFileKeys", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsArray){
}
void deClassScene::nfGetFileKeys::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	
	dsClass * const clsArray = rt->GetEngine()->GetClassArray();
	dsValue * const arr = rt->CreateValue(clsArray);
	const auto keys = scene.GetFiles().GetKeys();
	
	try{
		rt->CreateObject(arr, clsArray, 0);
		keys.Visit([&](const decString &key){
			rt->PushString(key);
			rt->RunFunction(arr, "add", 1);
		});
		
		rt->PushValue(arr);
		rt->FreeValue(arr);
		
	}catch(...){
		rt->FreeValue(arr);
		throw;
	}
}

// func MemoryFile getFileAt(String name)
deClassScene::nfGetFileAt::nfGetFileAt(const sInitData &init) :
dsFunction(init.clsScene, "getFileAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsMemFile){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfGetFileAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	
	clsScene.GetDS().GetClassMemoryFile()->PushMemoryFile(rt,
		scene.GetFiles().GetAt(rt->GetValue(0)->GetString()));
}

// func bool hasFileAt(String name)
deClassScene::nfHasFileAt::nfHasFileAt(const sInitData &init) :
dsFunction(init.clsScene, "hasFileAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsBool){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfHasFileAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushBool(scene.GetFiles().Has(rt->GetValue(0)->GetString()));
}

// func void addFile(String name, MemoryFile file)
deClassScene::nfAddFile::nfAddFile(const sInitData &init) :
dsFunction(init.clsScene, "addFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
	p_AddParameter(init.clsStr); // name
	p_AddParameter(init.clsMemFile); // file
}
void deClassScene::nfAddFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	const deClassScene &clsScene = *static_cast<deClassScene*>(GetOwnerClass());
	
	const char * const name = rt->GetValue(0)->GetString();
	decMemoryFile * const file = clsScene.GetDS().GetClassMemoryFile()->GetMemoryFile(
		rt->GetValue(1)->GetRealObject());
	if(!file){
		DSTHROW(dueNullPointer);
	}
	
	scene.AddFile(name, file);
}

// func void removeFile(String name)
deClassScene::nfRemoveFile::nfRemoveFile(const sInitData &init) :
dsFunction(init.clsScene, "removeFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassScene::nfRemoveFile::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene &scene = *dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	scene.RemoveFile(rt->GetValue(0)->GetString());
}

// func void removeAllFiles()
deClassScene::nfRemoveAllFiles::nfRemoveAllFiles(const sInitData &init) :
dsFunction(init.clsScene, "removeAllFiles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE,
init.clsVoid){
}
void deClassScene::nfRemoveAllFiles::RunFunction(dsRunTime*, dsValue *myself){
	dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene->RemoveAllFiles();
}



// func bool equals(Object other)
deClassScene::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsScene,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // other
}
void deClassScene::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene * const scene = dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	auto clsScene = static_cast<deClassScene*>(GetOwnerClass());
	auto obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsScene)){
		rt->PushBool(false);
		
	}else{
		const deScene * const otherScene = dedsGetNativeData<sSceneNatDat>(p_GetNativeData(obj)).scene;
		rt->PushBool(scene == otherScene);
	}
}

// func int hashCode()
deClassScene::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsScene,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassScene::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deScene * const scene = dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself)).scene;
	rt->PushInt((int)(intptr_t)scene);
}



// Class deClassScene
///////////////////////

deClassScene::deClassScene(deScriptingDragonScript &ds) :
dsClass("Scene", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
}

deClassScene::~deClassScene(){
}

void deClassScene::CreateClassMembers(dsEngine *engine){
	sInitData init;
	init.clsScene = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsArray = engine->GetClassArray();
	init.clsRN = pDS.GetClassResourceListener();
	init.clsMemFile = pDS.GetClassMemoryFile();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfGetResourceCount(init));
	AddFunction(new nfGetResourceKeys(init));
	AddFunction(new nfGetResourceAt(init));
	AddFunction(new nfHasResourceAt(init));
	AddFunction(new nfAddResource(init));
	AddFunction(new nfRemoveResource(init));
	AddFunction(new nfRemoveAllResources(init));
	
	AddFunction(new nfGetFileCount(init));
	AddFunction(new nfGetFileKeys(init));
	AddFunction(new nfGetFileAt(init));
	AddFunction(new nfHasFileAt(init));
	AddFunction(new nfAddFile(init));
	AddFunction(new nfRemoveFile(init));
	AddFunction(new nfRemoveAllFiles(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	CalcMemberOffsets();
}

deScene *deClassScene::GetScene(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	return dedsGetNativeData<sSceneNatDat>(p_GetNativeData(myself->GetBuffer())).scene;
}

void deClassScene::PushScene(dsRunTime *rt, deScene *scene){
	if(!scene){
		rt->PushObject(nullptr, this);
		return;
	}
	
	dsValue * const value = rt->CreateValue(this);
	try{
		rt->CreateObject(value, this, 0);
		dedsNewNativeData<sSceneNatDat>(p_GetNativeData(value->GetRealObject()->GetBuffer())).scene = scene;
		rt->PushValue(value);
		rt->FreeValue(value);
	}catch(...){
		rt->FreeValue(value);
		throw;
	}
}
