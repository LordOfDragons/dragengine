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
#include "deClassSkinBuilder.h"
#include "../math/deClassVector2.h"
#include "../math/deClassPoint.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassImage.h"
#include "../graphics/deClassFont.h"
#include "../video/deClassVideo.h"
#include "../curve/deClassCurveBezier.h"
#include "../math/deClassPoint3.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/deSkinMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitorIdentify.h>

#include <libdscript/packages/default/dsClassEnumeration.h>


class deClassSkinBuilder_Builder : public deSkinBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deSkin *pSkin;
	deSkinPropertyConstructed *pOpenConstructed;
	decPointerList pOpenNodes;
	bool pNextNodeAsMask;
	
public:
	deClassSkinBuilder_Builder(dsRunTime *rt, dsValue *myself) :
	pRT(rt), pMyself(myself), pSkin(NULL), pOpenConstructed(NULL), pNextNodeAsMask(false){
	}
	
	virtual void BuildSkin(deSkin *skin){
		pSkin = skin;
		pOpenConstructed = NULL;
		pOpenNodes.RemoveAll();
		pNextNodeAsMask = false;
		
		try{
			pRT->RunFunction(pMyself, "buildSkin", 0);
			
		}catch(const duException &e){
			pSkin = NULL;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW(deeInvalidParam);
			
		}catch(...){
			pNextNodeAsMask = false;
			pOpenConstructed = NULL;
			pOpenNodes.RemoveAll();
			pSkin = NULL;
			throw;
		}
		
		pNextNodeAsMask = false;
		pOpenConstructed = NULL;
		pOpenNodes.RemoveAll();
		pSkin = NULL;
	}
	
	inline deSkin *GetSkin() const{ return pSkin; }
	
	inline deSkinPropertyConstructed *GetOpenConstructed() const{ return pOpenConstructed; }
	
	deSkinPropertyNodeGroup *OpenConstructed(int indexTexture, int indexProperty){
		if(!pSkin || pOpenConstructed){
			DSTHROW(dueInvalidAction);
		}
		deSkinPropertyVisitorIdentify identify;
		pSkin->GetTextureAt(indexTexture)->GetPropertyAt(indexProperty)->Visit(identify);
		pOpenConstructed = &identify.CastToConstructed();
		pOpenNodes.Add(&pOpenConstructed->GetContent());
		pNextNodeAsMask = false;
		return &pOpenConstructed->GetContent();
	}
	
	deSkinPropertyNodeGroup *GetOpenNode(){
		return (deSkinPropertyNodeGroup*)pOpenNodes.GetAt(pOpenNodes.GetCount() - 1);
	}
	
	void OpenGroup(deSkinPropertyNodeGroup *group){
		if(!pSkin || pOpenConstructed || !group){
			DSTHROW(dueInvalidAction);
		}
		pOpenNodes.Add(group);
		pNextNodeAsMask = false;
	}
	
	inline bool GetNextNodeAsMask() const{ return pNextNodeAsMask; }
	
	void SetNextNodeAsMask(bool nextNodeAsMask){
		pNextNodeAsMask = nextNodeAsMask;
	}
	
	void CloseNode(){
		if(!pSkin || !pOpenConstructed || pOpenNodes.GetCount() == 0){
			DSTHROW(dueInvalidAction);
		}
		pOpenNodes.RemoveFrom(pOpenNodes.GetCount() - 1);
		pNextNodeAsMask = false;
		if(pOpenNodes.GetCount() == 0){
			pOpenConstructed = NULL;
		}
	}
};


struct sMdlBldNatDat{
	deClassSkinBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassSkinBuilder::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSkinBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkinBuilder::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder = NULL;
}

// public destructor Destructor()
deClassSkinBuilder::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSkinBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSkinBuilder::nfDestructor::RunFunction(dsRunTime*, dsValue*){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->~sMdlBldNatDat();
}



// public func Skin build( String filename )
deClassSkinBuilder::nfBuild::nfBuild(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
	p_AddParameter(init.clsString); // filename
}
void deClassSkinBuilder::nfBuild::RunFunction(dsRunTime *rt, dsValue *myself){
	sMdlBldNatDat &nd = *static_cast<sMdlBldNatDat*>(p_GetNativeData(myself));
	if(nd.builder){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	const char * const filename = rt->GetValue(0)->GetString();
	deClassSkinBuilder_Builder builder(rt, myself);
	deSkin::Ref skin;
	
	nd.builder = &builder;
	
	try{
		skin = ds.GetGameEngine()->GetSkinManager()->CreateSkin(filename, builder);
		
	}catch(...){
		nd.builder = NULL;
		throw;
	}
	
	nd.builder = NULL;
	ds.GetClassSkin()->PushSkin(rt, skin);
}



// abstract protected func void buildSkin()
deClassSkinBuilder::nfBuildSkin::nfBuildSkin(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "buildSkin", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid){
}
void deClassSkinBuilder::nfBuildSkin::RunFunction(dsRunTime*, dsValue*){
}



// protected func void addMapped(String name, CurveBezier curve, SkinMappedInputType inputType,
// Vector2 inputRange, bool inputClamped, Vector2 outputRange, String bone, String renderable,
// SkinMappedRenderableComponent renderableComponent)
deClassSkinBuilder::nfAddMapped::nfAddMapped(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addMapped", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsCurveBezier); // curve
	p_AddParameter(init.clsSkinMappedInputType); // inputType
	p_AddParameter(init.clsVector2); // inputRange
	p_AddParameter(init.clsBoolean); // inputClamped
	p_AddParameter(init.clsVector2); // outputRange
	p_AddParameter(init.clsString); // bone
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsSkinMappedRenderableComponent); // renderableComponent
}
void deClassSkinBuilder::nfAddMapped::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deClassCurveBezier &clsCurveBezier = *ds.GetClassCurveBezier();
	deClassVector2 &clsVector2 = *ds.GetClassVector2();
	dsClassEnumeration &clsEnum = *((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration());
	
	const deSkinMapped::Ref mapped(deSkinMapped::Ref::NewWith(rt->GetValue(0)->GetString()));
	mapped->GetCurve() = clsCurveBezier.GetCurve(rt->GetValue(1)->GetRealObject());
	mapped->SetInputType((deSkinMapped::eInputTypes)
		clsEnum.GetConstantOrder(*rt->GetValue(2)->GetRealObject()));
	
	const decVector2 &inputRange = clsVector2.GetVector2(rt->GetValue(3)->GetRealObject());
	mapped->SetInputLower(inputRange.x);
	mapped->SetInputUpper(inputRange.y);
	
	mapped->SetInputClamped(rt->GetValue(4)->GetBool());
	
	const decVector2 &outputRange = clsVector2.GetVector2(rt->GetValue(5)->GetRealObject());
	mapped->SetOutputLower(outputRange.x);
	mapped->SetOutputUpper(outputRange.y);
	
	mapped->SetBone(rt->GetValue(6)->GetString());
	
	mapped->SetRenderable(rt->GetValue(7)->GetString());
	mapped->SetRenderableComponent((deSkinMapped::eRenderableComponent)
		clsEnum.GetConstantOrder(*rt->GetValue(8)->GetRealObject()));
	
	builder->GetSkin()->AddMapped(mapped);
}

// protected func void addTexture( String name )
deClassSkinBuilder::nfAddTexture::nfAddTexture(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addTexture", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassSkinBuilder::nfAddTexture::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const char * const name = rt->GetValue(0)->GetString();
	
	deSkinTexture * const texture = new deSkinTexture(name);
	try{
		builder->GetSkin()->AddTexture(texture);
		
	}catch(...){
		delete texture;
		throw;
	}
}

// protected func void addPropertyValue( int texture, String type, String texCoordSet,
// String renderable, float value )
deClassSkinBuilder::nfAddPropertyValue::nfAddPropertyValue(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyValue", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsFloat); // value
}
void deClassSkinBuilder::nfAddPropertyValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	const float value = rt->GetValue(4)->GetFloat();
	
	deSkinPropertyValue * const property = new deSkinPropertyValue(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetValue(value);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyColor( int texture, String type, String texCoordSet,
// String renderable, Color color )
deClassSkinBuilder::nfAddPropertyColor::nfAddPropertyColor(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyColor", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsColor); // color
}
void deClassSkinBuilder::nfAddPropertyColor::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(4)->GetRealObject());
	
	deSkinPropertyColor * const property = new deSkinPropertyColor(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetColor(color);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyImage( int texture, String type, String texCoordSet,
// String renderable, String pathImage, Image image )
deClassSkinBuilder::nfAddPropertyImage::nfAddPropertyImage(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyImage", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsString); // pathImage
	p_AddParameter(init.clsImage); // image
}
void deClassSkinBuilder::nfAddPropertyImage::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	const char * const pathImage = rt->GetValue(4)->GetString();
	deImage * const image = ds.GetClassImage()->GetImage(rt->GetValue(5)->GetRealObject());
	
	deSkinPropertyImage * const property = new deSkinPropertyImage(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetPath(pathImage);
		property->SetImage(image);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyVideo( int texture, String type, String texCoordSet,
// String renderable, String pathVideo, Video video, bool sharedTime )
deClassSkinBuilder::nfAddPropertyVideo::nfAddPropertyVideo(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyVideo", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsString); // pathVideo
	p_AddParameter(init.clsVideo); // video
	p_AddParameter(init.clsBoolean); // sharedTime
}
void deClassSkinBuilder::nfAddPropertyVideo::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	const char * const pathVideo = rt->GetValue(4)->GetString();
	deVideo * const video = ds.GetClassVideo()->GetVideo(rt->GetValue(5)->GetRealObject());
	const bool sharedTime = rt->GetValue(6)->GetBool();
	
	deSkinPropertyVideo * const property = new deSkinPropertyVideo(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetPath(pathVideo);
		property->SetVideo(video);
		property->SetSharedTime(sharedTime);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyMapped( int texture, String type, String texCoordSet, String renderable,
// CurveBezier redCurve, SkinPropertyMappedInputType redInputType, Vector2 redInputRange, bool redInputClamped, String redBone,
// CurveBezier greenCurve, SkinPropertyMappedInputType greenInputType, Vector2 greenInputRange, bool greenInputClamped, String greenBone,
// CurveBezier blueCurve, SkinPropertyMappedInputType blueInputType, Vector2 blueInputRange, bool blueInputClamped, String blueBone,
// CurveBezier alphaCurve, SkinPropertyMappedInputType alphaInputType, Vector2 alphaInputRange, bool alphaInputClamped, String alphaBone,
// )
deClassSkinBuilder::nfAddPropertyMapped::nfAddPropertyMapped(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyMapped", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsCurveBezier); // redCurve
	p_AddParameter(init.clsSkinPropertyMappedInputType); // redInputType
	p_AddParameter(init.clsVector2); // redInputRange
	p_AddParameter(init.clsBoolean); // redInputClamped
	p_AddParameter(init.clsString); // redBone
	p_AddParameter(init.clsCurveBezier); // greenCurve
	p_AddParameter(init.clsSkinPropertyMappedInputType); // greenInputType
	p_AddParameter(init.clsVector2); // greenInputRange
	p_AddParameter(init.clsBoolean); // greenInputClamped
	p_AddParameter(init.clsString); // greenBone
	p_AddParameter(init.clsCurveBezier); // blueCurve
	p_AddParameter(init.clsSkinPropertyMappedInputType); // blueInputType
	p_AddParameter(init.clsVector2); // blueInputRange
	p_AddParameter(init.clsBoolean); // blueInputClamped
	p_AddParameter(init.clsString); // blueBone
	p_AddParameter(init.clsCurveBezier); // alphaCurve
	p_AddParameter(init.clsSkinPropertyMappedInputType); // alphaInputType
	p_AddParameter(init.clsVector2); // alphaInputRange
	p_AddParameter(init.clsBoolean); // alphaInputClamped
	p_AddParameter(init.clsString); // alphaBone
}
void deClassSkinBuilder::nfAddPropertyMapped::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deClassCurveBezier &clsCurveBezier = *ds.GetClassCurveBezier();
	deClassVector2 &clsVector2 = *ds.GetClassVector2();
	dsClassEnumeration &clsEnum = *((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration());
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	
	const decCurveBezier &redCurve = clsCurveBezier.GetCurve(rt->GetValue(4)->GetRealObject());
	const deSkinMapped::eInputTypes redInputType = (deSkinMapped::eInputTypes)
		clsEnum.GetConstantOrder(*rt->GetValue(5)->GetRealObject());
	const decVector2 &redInputRange = clsVector2.GetVector2(rt->GetValue(6)->GetRealObject());
	const bool redInputClamped = rt->GetValue(7)->GetBool();
	const char * const redBone = rt->GetValue(8)->GetString();
	
	const decCurveBezier &greenCurve = clsCurveBezier.GetCurve(rt->GetValue(9)->GetRealObject());
	const deSkinMapped::eInputTypes greenInputType = (deSkinMapped::eInputTypes)
		clsEnum.GetConstantOrder(*rt->GetValue(10)->GetRealObject());
	const decVector2 &greenInputRange = clsVector2.GetVector2(rt->GetValue(11)->GetRealObject());
	const bool greenInputClamped = rt->GetValue(12)->GetBool();
	const char * const greenBone = rt->GetValue(13)->GetString();
	
	const decCurveBezier &blueCurve = clsCurveBezier.GetCurve(rt->GetValue(14)->GetRealObject());
	const deSkinMapped::eInputTypes blueInputType = (deSkinMapped::eInputTypes)
		clsEnum.GetConstantOrder(*rt->GetValue(15)->GetRealObject());
	const decVector2 &blueInputRange = clsVector2.GetVector2(rt->GetValue(16)->GetRealObject());
	const bool blueInputClamped = rt->GetValue(17)->GetBool();
	const char * const blueBone = rt->GetValue(18)->GetString();
	
	const decCurveBezier &alphaCurve = clsCurveBezier.GetCurve(rt->GetValue(19)->GetRealObject());
	const deSkinMapped::eInputTypes alphaInputType = (deSkinMapped::eInputTypes)
		clsEnum.GetConstantOrder(*rt->GetValue(20)->GetRealObject());
	const decVector2 &alphaInputRange = clsVector2.GetVector2(rt->GetValue(21)->GetRealObject());
	const bool alphaInputClamped = rt->GetValue(22)->GetBool();
	const char * const alphaBone = rt->GetValue(23)->GetString();
	
	
	deSkinPropertyMapped * const property = new deSkinPropertyMapped(type);
	deSkinMapped::Ref mapped;
	decString name;
	int index;
	
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		
		if(redCurve.GetPointCount() > 0){
			index = builder->GetSkin()->GetMappedCount();
			name.Format("#generated%d", index);
			mapped.TakeOver(new deSkinMapped(name));
			mapped->GetCurve() = redCurve;
			mapped->SetInputType(redInputType);
			mapped->SetInputLower(redInputRange.x);
			mapped->SetInputUpper(redInputRange.y);
			mapped->SetInputClamped(redInputClamped);
			mapped->SetBone(redBone);
			property->SetRed(index);
		}
		
		if(greenCurve.GetPointCount() > 0){
			index = builder->GetSkin()->GetMappedCount();
			name.Format("#generated%d", index);
			mapped.TakeOver(new deSkinMapped(name));
			mapped->GetCurve() = greenCurve;
			mapped->SetInputType(greenInputType);
			mapped->SetInputLower(greenInputRange.x);
			mapped->SetInputUpper(greenInputRange.y);
			mapped->SetInputClamped(greenInputClamped);
			mapped->SetBone(greenBone);
			property->SetGreen(index);
		}
		
		if(blueCurve.GetPointCount() > 0){
			index = builder->GetSkin()->GetMappedCount();
			name.Format("#generated%d", index);
			mapped.TakeOver(new deSkinMapped(name));
			mapped->GetCurve() = blueCurve;
			mapped->SetInputType(blueInputType);
			mapped->SetInputLower(blueInputRange.x);
			mapped->SetInputUpper(blueInputRange.y);
			mapped->SetInputClamped(blueInputClamped);
			mapped->SetBone(blueBone);
			property->SetBlue(index);
		}
		
		if(alphaCurve.GetPointCount() > 0){
			index = builder->GetSkin()->GetMappedCount();
			name.Format("#generated%d", index);
			mapped.TakeOver(new deSkinMapped(name));
			mapped->GetCurve() = alphaCurve;
			mapped->SetInputType(alphaInputType);
			mapped->SetInputLower(alphaInputRange.x);
			mapped->SetInputUpper(alphaInputRange.y);
			mapped->SetInputClamped(alphaInputClamped);
			mapped->SetBone(alphaBone);
			property->SetAlpha(index);
		}
		
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyMapped(int texture, String type, String texCoordSet,
// String renderable, int red, int green, int blue, int alpha)
deClassSkinBuilder::nfAddPropertyMapped2::nfAddPropertyMapped2(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyMapped", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsInteger); // red
	p_AddParameter(init.clsInteger); // green
	p_AddParameter(init.clsInteger); // blue
	p_AddParameter(init.clsInteger); // alpha
}
void deClassSkinBuilder::nfAddPropertyMapped2::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const int mappedCount = builder->GetSkin()->GetMappedCount();
	
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	
	const int red = rt->GetValue(4)->GetInt();
	if(red < -1 || red >= mappedCount){
		DETHROW(deeInvalidParam);
	}
	
	const int green = rt->GetValue(5)->GetInt();
	if(green < -1 || green >= mappedCount){
		DETHROW(deeInvalidParam);
	}
	
	const int blue = rt->GetValue(6)->GetInt();
	if(blue < -1 || blue >= mappedCount){
		DETHROW(deeInvalidParam);
	}
	
	const int alpha = rt->GetValue(7)->GetInt();
	if(alpha < -1 || alpha >= mappedCount){
		DETHROW(deeInvalidParam);
	}
	
	deSkinPropertyMapped * const property = new deSkinPropertyMapped(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetRed(red);
		property->SetGreen(green);
		property->SetBlue(blue);
		property->SetAlpha(alpha);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void addPropertyConstructed( int texture, String type, String texCoordSet,
// String renderable, Color color, bool tileX, bool tileY, Point3 size )
deClassSkinBuilder::nfAddPropertyConstructed::nfAddPropertyConstructed(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addPropertyConstructed", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsString); // type
	p_AddParameter(init.clsString); // texCoordSet
	p_AddParameter(init.clsString); // renderable
	p_AddParameter(init.clsColor); // color
	p_AddParameter(init.clsBoolean); // tileX
	p_AddParameter(init.clsBoolean); // tileY
	p_AddParameter(init.clsPoint3); // size
}
void deClassSkinBuilder::nfAddPropertyConstructed::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	const char * const type = rt->GetValue(1)->GetString();
	const char * const texCoordSet = rt->GetValue(2)->GetString();
	const char * const renderable = rt->GetValue(3)->GetString();
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(4)->GetRealObject());
	const bool tileX = rt->GetValue(5)->GetBool();
	const bool tileY = rt->GetValue(6)->GetBool();
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint(rt->GetValue(7)->GetRealObject());
	
	deSkinPropertyConstructed * const property = new deSkinPropertyConstructed(type);
	try{
		property->SetTexCoordSet(texCoordSet);
		property->SetRenderable(renderable);
		property->SetColor(color);
		property->SetTileX(tileX);
		property->SetTileY(tileY);
		property->GetContent().SetSize(size);
		texture.AddProperty(property);
		
	}catch(...){
		delete property;
		throw;
	}
}

// protected func void setPropertyBone(int texture, int property, String bone)
deClassSkinBuilder::nfSetPropertyBone::nfSetPropertyBone(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "setPropertyBone",
DSFT_FUNCTION, DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
	p_AddParameter(init.clsString); // bone
}
void deClassSkinBuilder::nfSetPropertyBone::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	deSkinTexture &texture = *builder->GetSkin()->GetTextureAt(rt->GetValue(0)->GetInt());
	deSkinProperty &property = *texture.GetPropertyAt(rt->GetValue(1)->GetInt());
	property.SetBone(rt->GetValue(2)->GetString());
}

// protected func void constructedOpenContent( int texture, int property )
deClassSkinBuilder::nfConstructedOpenContent::nfConstructedOpenContent(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "constructedOpenContent", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // texture
	p_AddParameter(init.clsInteger); // property
	p_AddParameter(init.clsPoint3); // size
	p_AddParameter(init.clsColor); // backgroundColor
}
void deClassSkinBuilder::nfConstructedOpenContent::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	builder->OpenConstructed(rt->GetValue(0)->GetInt(), rt->GetValue(1)->GetInt());
}

// protected func void addNodeImage( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float contrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// String path, Image image, Point repeat )
deClassSkinBuilder::nfAddNodeImage::nfAddNodeImage(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addNodeImage", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint3); // position
	p_AddParameter(init.clsPoint3); // size
	p_AddParameter(init.clsFloat); // rotation
	p_AddParameter(init.clsFloat); // shear
	p_AddParameter(init.clsFloat); // brightness
	p_AddParameter(init.clsFloat); // contrast
	p_AddParameter(init.clsFloat); // gamma
	p_AddParameter(init.clsColor); // colorize
	p_AddParameter(init.clsFloat); // transparency
	p_AddParameter(init.clsSkinPropertyNodeCombineMode); // combineMode
	p_AddParameter(init.clsString); // path
	p_AddParameter(init.clsImage); // image
	p_AddParameter(init.clsPoint); // repeat
}
void deClassSkinBuilder::nfAddNodeImage::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint(rt->GetValue(0)->GetRealObject());
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint(rt->GetValue(1)->GetRealObject());
	const float rotation = rt->GetValue(2)->GetFloat();
	const float shear = rt->GetValue(3)->GetFloat();
	const float brightness = rt->GetValue(4)->GetFloat();
	const float contrast = rt->GetValue(5)->GetFloat();
	const float gamma = rt->GetValue(6)->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor(rt->GetValue(7)->GetRealObject());
	const float transparency = rt->GetValue(8)->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = (deSkinPropertyNode::eCombineModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const char * const path = rt->GetValue(10)->GetString();
	deImage * const image = ds.GetClassImage()->GetImage(rt->GetValue(11)->GetRealObject());
	const decPoint &repeat = ds.GetClassPoint()->GetPoint(rt->GetValue(12)->GetRealObject());
	
	deSkinPropertyNodeImage * const node = new deSkinPropertyNodeImage;
	try{
		node->SetPosition(position);
		node->SetSize(size);
		node->SetRotation(rotation);
		node->SetShear(shear);
		node->SetBrightness(brightness);
		node->SetContrast(contrast);
		node->SetGamma(gamma);
		node->SetColorize(colorize);
		node->SetTransparency(transparency);
		node->SetPath(path);
		node->SetImage(image);
		node->SetRepeat(repeat);
		node->SetCombineMode(combineMode);
		
		if(builder->GetNextNodeAsMask()){
			group->SetMask(node);
			
		}else{
			group->AddNode(node);
		}
		
	}catch(...){
		delete node;
		throw;
	}
}

// protected func void addNodeText( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float contrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// String path, Font font, float fontSize, String text, Color color )
deClassSkinBuilder::nfAddNodeText::nfAddNodeText(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addNodeText", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint3); // position
	p_AddParameter(init.clsPoint3); // size
	p_AddParameter(init.clsFloat); // rotation
	p_AddParameter(init.clsFloat); // shear
	p_AddParameter(init.clsFloat); // brightness
	p_AddParameter(init.clsFloat); // contrast
	p_AddParameter(init.clsFloat); // gamma
	p_AddParameter(init.clsColor); // colorize
	p_AddParameter(init.clsFloat); // transparency
	p_AddParameter(init.clsSkinPropertyNodeCombineMode); // combineMode
	p_AddParameter(init.clsString); // path
	p_AddParameter(init.clsFont); // font
	p_AddParameter(init.clsFloat); // fontSize
	p_AddParameter(init.clsString); // text
	p_AddParameter(init.clsColor); // color
}
void deClassSkinBuilder::nfAddNodeText::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint(rt->GetValue(0)->GetRealObject());
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint(rt->GetValue(1)->GetRealObject());
	const float rotation = rt->GetValue(2)->GetFloat();
	const float shear = rt->GetValue(3)->GetFloat();
	const float brightness = rt->GetValue(4)->GetFloat();
	const float contrast = rt->GetValue(5)->GetFloat();
	const float gamma = rt->GetValue(6)->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor(rt->GetValue(7)->GetRealObject());
	const float transparency = rt->GetValue(8)->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = (deSkinPropertyNode::eCombineModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const char * const path = rt->GetValue(10)->GetString();
	deFont * const font = ds.GetClassFont()->GetFont(rt->GetValue(11)->GetRealObject());
	const float fontSize = rt->GetValue(12)->GetFloat();
	const char * const text = rt->GetValue(13)->GetString();
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(14)->GetRealObject());
	
	deSkinPropertyNodeText * const node = new deSkinPropertyNodeText;
	try{
		node->SetPosition(position);
		node->SetSize(size);
		node->SetRotation(rotation);
		node->SetShear(shear);
		node->SetBrightness(brightness);
		node->SetContrast(contrast);
		node->SetGamma(gamma);
		node->SetColorize(colorize);
		node->SetTransparency(transparency);
		node->SetCombineMode(combineMode);
		node->SetPath(path);
		node->SetFont(font);
		node->SetFontSize(fontSize);
		node->SetText(text);
		node->SetColor(color);
		
		if(builder->GetNextNodeAsMask()){
			group->SetMask(node);
			
		}else{
			group->AddNode(node);
		}
		
	}catch(...){
		delete node;
		throw;
	}
}

// protected func void addNodeShape( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float contrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode,
// int shapeType, Color fillColor, Color lineColor, float thickness )
deClassSkinBuilder::nfAddNodeShape::nfAddNodeShape(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addNodeShape", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint3); // position
	p_AddParameter(init.clsPoint3); // size
	p_AddParameter(init.clsFloat); // rotation
	p_AddParameter(init.clsFloat); // shear
	p_AddParameter(init.clsFloat); // brightness
	p_AddParameter(init.clsFloat); // contrast
	p_AddParameter(init.clsFloat); // gamma
	p_AddParameter(init.clsColor); // colorize
	p_AddParameter(init.clsFloat); // transparency
	p_AddParameter(init.clsSkinPropertyNodeCombineMode); // combineMode
	p_AddParameter(init.clsInteger); // shapeType
	p_AddParameter(init.clsColor); // fillColor
	p_AddParameter(init.clsColor); // lineColor
	p_AddParameter(init.clsFloat); // thickness
}
void deClassSkinBuilder::nfAddNodeShape::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint(rt->GetValue(0)->GetRealObject());
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint(rt->GetValue(1)->GetRealObject());
	const float rotation = rt->GetValue(2)->GetFloat();
	const float shear = rt->GetValue(3)->GetFloat();
	const float brightness = rt->GetValue(4)->GetFloat();
	const float contrast = rt->GetValue(5)->GetFloat();
	const float gamma = rt->GetValue(6)->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor(rt->GetValue(7)->GetRealObject());
	const float transparency = rt->GetValue(8)->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = (deSkinPropertyNode::eCombineModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	const deSkinPropertyNodeShape::eShapeTypes shapeType =
		(deSkinPropertyNodeShape::eShapeTypes)rt->GetValue(10)->GetInt();
	const decColor &fillColor = ds.GetClassColor()->GetColor(rt->GetValue(11)->GetRealObject());
	const decColor &lineColor = ds.GetClassColor()->GetColor(rt->GetValue(12)->GetRealObject());
	const float thickness = rt->GetValue(13)->GetFloat();
	
	deSkinPropertyNodeShape * const node = new deSkinPropertyNodeShape;
	try{
		node->SetPosition(position);
		node->SetSize(size);
		node->SetRotation(rotation);
		node->SetShear(shear);
		node->SetBrightness(brightness);
		node->SetContrast(contrast);
		node->SetGamma(gamma);
		node->SetColorize(colorize);
		node->SetTransparency(transparency);
		node->SetCombineMode(combineMode);
		node->SetShapeType(shapeType);
		node->SetFillColor(fillColor);
		node->SetLineColor(lineColor);
		node->SetThickness(thickness);
		
		if(builder->GetNextNodeAsMask()){
			group->SetMask(node);
			
		}else{
			group->AddNode(node);
		}
		
	}catch(...){
		delete node;
		throw;
	}
}

// protected func void addNodeGroup( Point3 position, Point3 size, float rotation, float shear,
// float brightness, float contrast, float gamma, Color colorize, float transparency,
// SkinPropertyNodeCombineMode combineMode )
deClassSkinBuilder::nfAddNodeGroup::nfAddNodeGroup(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "addNodeGroup", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPoint3); // position
	p_AddParameter(init.clsPoint3); // size
	p_AddParameter(init.clsFloat); // rotation
	p_AddParameter(init.clsFloat); // shear
	p_AddParameter(init.clsFloat); // brightness
	p_AddParameter(init.clsFloat); // contrast
	p_AddParameter(init.clsFloat); // gamma
	p_AddParameter(init.clsColor); // colorize
	p_AddParameter(init.clsFloat); // transparency
	p_AddParameter(init.clsSkinPropertyNodeCombineMode); // combineMode
}
void deClassSkinBuilder::nfAddNodeGroup::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = ((deClassSkinBuilder*)GetOwnerClass())->GetDS();
	deSkinPropertyNodeGroup * const group = builder->GetOpenNode();
	const decPoint3 &position = ds.GetClassPoint3()->GetPoint(rt->GetValue(0)->GetRealObject());
	const decPoint3 &size = ds.GetClassPoint3()->GetPoint(rt->GetValue(1)->GetRealObject());
	const float rotation = rt->GetValue(2)->GetFloat();
	const float shear = rt->GetValue(3)->GetFloat();
	const float brightness = rt->GetValue(4)->GetFloat();
	const float contrast = rt->GetValue(5)->GetFloat();
	const float gamma = rt->GetValue(6)->GetFloat();
	const decColor &colorize = ds.GetClassColor()->GetColor(rt->GetValue(7)->GetRealObject());
	const float transparency = rt->GetValue(8)->GetFloat();
	const deSkinPropertyNode::eCombineModes &combineMode = (deSkinPropertyNode::eCombineModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 9 )->GetRealObject() );
	
	deSkinPropertyNodeGroup * const node = new deSkinPropertyNodeGroup;
	try{
		node->SetPosition(position);
		node->SetSize(size);
		node->SetRotation(rotation);
		node->SetShear(shear);
		node->SetBrightness(brightness);
		node->SetContrast(contrast);
		node->SetGamma(gamma);
		node->SetColorize(colorize);
		node->SetTransparency(transparency);
		node->SetCombineMode(combineMode);
		
		if(builder->GetNextNodeAsMask()){
			group->SetMask(node);
			
		}else{
			group->AddNode(node);
		}
		
	}catch(...){
		delete node;
		throw;
	}
	
	builder->OpenGroup(node);
}

// protected func void nextNodeAsMask()
deClassSkinBuilder::nfNextNodeAsMask::nfNextNodeAsMask(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "nextNodeAsMask", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
}
void deClassSkinBuilder::nfNextNodeAsMask::RunFunction(dsRunTime*, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	builder->SetNextNodeAsMask(true);
}

// protected func void closeNode()
deClassSkinBuilder::nfCloseNode::nfCloseNode(const sInitData &init) :
dsFunction(init.clsSkinBuilder, "closeNode", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
}
void deClassSkinBuilder::nfCloseNode::RunFunction(dsRunTime*, dsValue *myself){
	deClassSkinBuilder_Builder * const builder = static_cast<sMdlBldNatDat*>(p_GetNativeData(myself))->builder;
	if(!builder || !builder->GetSkin()){
		DSTHROW(dueInvalidAction);
	}
	
	builder->CloseNode();
}



// Class deClassSkinBuilder
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassSkinBuilder::deClassSkinBuilder(deScriptingDragonScript &ds) :
dsClass("SkinBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sMdlBldNatDat));
}

deClassSkinBuilder::~deClassSkinBuilder(){
}



// Management
///////////////

void deClassSkinBuilder::CreateClassMembers(dsEngine *engine){
	pClsSkinPropertyNodeCombineMode = engine->GetClass("Dragengine.Scenery.SkinPropertyNodeCombineMode");
	pClsSkinPropertyMappedInputType = engine->GetClass("Dragengine.Scenery.SkinPropertyMappedInputType");
	pClsSkinMappedInputType = engine->GetClass("Dragengine.Scenery.SkinMappedInputType");
	pClsSkinMappedRenderableComponent = engine->GetClass("Dragengine.Scenery.SkinMappedRenderableComponent");
	
	sInitData init;
	init.clsSkinBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsSkin = pDS.GetClassSkin();
	init.clsColor = pDS.GetClassColor();
	init.clsImage = pDS.GetClassImage();
	init.clsVideo = pDS.GetClassVideo();
	init.clsPoint = pDS.GetClassPoint();
	init.clsPoint3 = pDS.GetClassPoint3();
	init.clsVector2 = pDS.GetClassVector2();
	init.clsFont = pDS.GetClassFont();
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	init.clsSkinPropertyNodeCombineMode = pClsSkinPropertyNodeCombineMode;
	init.clsSkinPropertyMappedInputType = pClsSkinPropertyMappedInputType;
	init.clsSkinMappedInputType = pClsSkinMappedInputType;
	init.clsSkinMappedRenderableComponent = pClsSkinMappedRenderableComponent;
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfBuild(init));
	AddFunction(new nfBuildSkin(init));
	AddFunction(new nfAddMapped(init));
	AddFunction(new nfAddTexture(init));
	AddFunction(new nfAddPropertyValue(init));
	AddFunction(new nfAddPropertyColor(init));
	AddFunction(new nfAddPropertyImage(init));
	AddFunction(new nfAddPropertyVideo(init));
	AddFunction(new nfAddPropertyConstructed(init));
	AddFunction(new nfAddPropertyMapped(init));
	AddFunction(new nfAddPropertyMapped2(init));
	AddFunction(new nfSetPropertyBone(init));
	AddFunction(new nfConstructedOpenContent(init));
	AddFunction(new nfAddNodeImage(init));
	AddFunction(new nfAddNodeText(init));
	AddFunction(new nfAddNodeShape(init));
	AddFunction(new nfAddNodeGroup(init));
	AddFunction(new nfNextNodeAsMask(init));
	AddFunction(new nfCloseNode(init));
}
