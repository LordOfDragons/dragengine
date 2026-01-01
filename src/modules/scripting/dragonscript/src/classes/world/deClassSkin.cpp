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

#include "deClassSkin.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassImage.h"
#include "../resources/deClassResourceListener.h"
#include "../video/deClassVideo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>

struct sSkinNatDat{
	deSkin::Ref skin;
};



// Native Functions
/////////////////////

// public constructor Load( string name )
deClassSkin::nfLoad::nfLoad(const sInitData &init) :
dsFunction(init.clsSkin, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassSkin::nfLoad::RunFunction(dsRunTime *rt, dsValue *myself){
	sSkinNatDat * const nd = new (p_GetNativeData(myself)) sSkinNatDat();
	
	deScriptingDragonScript &ds = (static_cast<deClassSkin*>(GetOwnerClass()))->GetDS();
	deSkinManager &skinMgr = *ds.GetGameEngine()->GetSkinManager();
	
	nd->skin = skinMgr.LoadSkin(rt->GetValue(0)->GetString(), "/");
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassSkin::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) :
dsFunction(init.clsSkin, "loadAsynchron", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // filename
	p_AddParameter(init.clsResourceListener); // listener
}
void deClassSkin::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = (static_cast<deClassSkin*>(GetOwnerClass()))->GetDS();
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	if(!listener){
		DSTHROW(dueNullPointer);
	}
	
	ds.GetResourceLoader()->AddRequest(filename, deResourceLoader::ertSkin, listener);
}

// public destructor Destructor()
deClassSkin::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkin, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkin::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSkinNatDat*>(p_GetNativeData(myself))->~sSkinNatDat();
}



// public func string GetFilename()
deClassSkin::nfGetFilename::nfGetFilename(const sInitData &init) :
dsFunction(init.clsSkin, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassSkin::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	rt->PushString(skin.GetFilename());
}

// public func int getTextureCount()
deClassSkin::nfGetTextureCount::nfGetTextureCount(const sInitData &init) :
dsFunction(init.clsSkin, "getTextureCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSkin::nfGetTextureCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	rt->PushInt(skin.GetTextureCount());
}

// public func int indexOfTextureNamed( String name )
deClassSkin::nfIndexOfTextureNamed::nfIndexOfTextureNamed(const sInitData &init) :
dsFunction(init.clsSkin, "indexOfTextureNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
	p_AddParameter(init.clsString); // name
}
void deClassSkin::nfIndexOfTextureNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const char * const name = rt->GetValue(0)->GetString();
	
	rt->PushInt(skin.IndexOfTextureNamed(name));
}



// public func String textureGetName( int texture )
deClassSkin::nfTextureGetName::nfTextureGetName(const sInitData &init) :
dsFunction(init.clsSkin, "textureGetName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
	p_AddParameter(init.clsInteger); // texture
}
void deClassSkin::nfTextureGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	
	rt->PushString(texture.GetName());
}

// public func int textureGetPropertyCount( int texture )
deClassSkin::nfTextureGetPropertyCount::nfTextureGetPropertyCount(const sInitData &init) :
dsFunction(init.clsSkin, "textureGetPropertyCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
	p_AddParameter(init.clsInteger); // texture
}
void deClassSkin::nfTextureGetPropertyCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	
	rt->PushInt(texture.GetPropertyCount());
}

// public func int textureIndexOfPropertyNamed( int texture, String property )
deClassSkin::nfTextureIndexOfPropertyNamed::nfTextureIndexOfPropertyNamed(const sInitData &init) :
dsFunction(init.clsSkin, "textureIndexOfPropertyNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // property
}
void deClassSkin::nfTextureIndexOfPropertyNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	
	const char * const name = rt->GetValue(1)->GetString();
	const int count = texture.GetPropertyCount();
	int i;
	
	for(i=0; i<count; i++){
		if(texture.GetPropertyAt(i)->GetType() == name){
			rt->PushInt(i);
			return;
		}
	}
	
	rt->PushInt(-1);
}



// public func String propertyGetName( int texture, int property )
deClassSkin::nfPropertyGetName::nfPropertyGetName(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetName::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	const deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	
	rt->PushString(property.GetType());
}

// public func String propertyGetRenderable( int texture, int property )
deClassSkin::nfPropertyGetRenderable::nfPropertyGetRenderable(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetRenderable", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetRenderable::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	const deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	
	rt->PushString(property.GetRenderable());
}

// public func SkinPropertyType propertyGetType( int texture, int property )
deClassSkin::nfPropertyGetType::nfPropertyGetType(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkinPropertyType){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit(visitor);
	rt->PushValue((static_cast<deClassSkin*>(GetOwnerClass()))->GetClassSkinPropertyType()
		->GetVariable(visitor.GetType())->GetStaticValue());
}

// public func float propertyGetValue( int texture, int property )
deClassSkin::nfPropertyGetValue::nfPropertyGetValue(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetValue::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit(visitor);
	rt->PushFloat(visitor.CastToValue().GetValue());
}

// public func Color propertyGetColor( int texture, int property )
deClassSkin::nfPropertyGetColor::nfPropertyGetColor(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetColor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	deScriptingDragonScript &ds = (static_cast<deClassSkin*>(GetOwnerClass()))->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit(visitor);
	ds.GetClassColor()->PushColor(rt, visitor.CastToColor().GetColor());
}

// public func Image propertyGetImage( int texture, int property )
deClassSkin::nfPropertyGetImage::nfPropertyGetImage(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	deScriptingDragonScript &ds = (static_cast<deClassSkin*>(GetOwnerClass()))->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit(visitor);
	ds.GetClassImage()->PushImage(rt, visitor.CastToImage().GetImage());
}

// public func Video propertyGetVideo( int texture, int property )
deClassSkin::nfPropertyGetVideo::nfPropertyGetVideo(const sInitData &init) :
dsFunction(init.clsSkin, "propertyGetVideo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVideo){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
}
void deClassSkin::nfPropertyGetVideo::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSkin &skin = *static_cast<sSkinNatDat*>(p_GetNativeData(myself))->skin;
	const deSkinTexture &texture = *skin.GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	deScriptingDragonScript &ds = (static_cast<deClassSkin*>(GetOwnerClass()))->GetDS();
	
	deSkinPropertyVisitorIdentify visitor;
	property.Visit(visitor);
	ds.GetClassVideo()->PushVideo(rt, visitor.CastToVideo().GetVideo());
}



// public func int hashCode()
deClassSkin::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSkin, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassSkin::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkinNatDat &nd = *static_cast<sSkinNatDat*>(p_GetNativeData(myself));
	
	// hash code = memory location
	rt->PushInt((int)(intptr_t)nd.skin);
}

// public func bool equals( Object obj )
deClassSkin::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSkin, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassSkin::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSkinNatDat &nd = *static_cast<sSkinNatDat*>(p_GetNativeData(myself));
	const deClassSkin * const clsSkin = static_cast<deClassSkin*>(GetOwnerClass());
	dsValue * const object = rt->GetValue(0);
	
	if(object->GetType()->GetPrimitiveType() != DSPT_OBJECT || !object->GetRealObject()
	|| object->GetRealObject()->GetType() != clsSkin){
		rt->PushBool(false);
		
	}else{
		const sSkinNatDat &other = *static_cast<sSkinNatDat*>(p_GetNativeData(object));
		rt->PushBool(nd.skin == other.skin);
	}
}



// Class deClassSkin
///////////////////////

// Constructor, destructor
////////////////////////////

deClassSkin::deClassSkin(deScriptingDragonScript &ds) :
dsClass("Skin", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSkinNatDat));
}

deClassSkin::~deClassSkin(){
}



// Management
///////////////

void deClassSkin::CreateClassMembers(dsEngine *engine){
	pClsSkinPropertyType = engine->GetClass("Dragengine.Scenery.SkinPropertyType");
	
	sInitData init;
	init.clsSkin = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsResourceListener = pDS.GetClassResourceListener();
	init.clsColor = pDS.GetClassColor();
	init.clsImage = pDS.GetClassImage();
	init.clsVideo = pDS.GetClassVideo();
	init.clsSkinPropertyType = pClsSkinPropertyType;
	
	// functions
	AddFunction(new nfLoad(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	
	AddFunction(new nfGetTextureCount(init));
	AddFunction(new nfIndexOfTextureNamed(init));
	
	AddFunction(new nfTextureGetName(init));
	AddFunction(new nfTextureGetPropertyCount(init));
	AddFunction(new nfTextureIndexOfPropertyNamed(init));
	
	AddFunction(new nfPropertyGetName(init));
	AddFunction(new nfPropertyGetRenderable(init));
	AddFunction(new nfPropertyGetType(init));
	AddFunction(new nfPropertyGetValue(init));
	AddFunction(new nfPropertyGetColor(init));
	AddFunction(new nfPropertyGetImage(init));
	AddFunction(new nfPropertyGetVideo(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	CalcMemberOffsets();
}

deSkin *deClassSkin::GetSkin(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	return static_cast<sSkinNatDat*>(p_GetNativeData(myself->GetBuffer()))->skin;
}

void deClassSkin::PushSkin(dsRunTime *rt, deSkin *skin){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!skin){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	(new (p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer())) sSkinNatDat)->skin = skin;
}
