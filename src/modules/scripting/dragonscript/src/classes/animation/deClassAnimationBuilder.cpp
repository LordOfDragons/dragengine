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

#include "deClassAnimation.h"
#include "deClassAnimationBuilder.h"
#include "../dedsHelpers.h"
#include "../math/deClassVector.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationBone.h>
#include <dragengine/resources/animation/deAnimationBuilder.h>
#include <dragengine/resources/animation/deAnimationKeyframe.h>

#include <dragengine/resources/animation/deAnimationKeyframeVertexPositionSet.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimationMove.h>


class deClassAnimationBuilder_Builder : public deAnimationBuilder{
	dsRunTime *pRT;
	dsValue *pMyself;
	deAnimation *pAnimation;
	
public:
	deClassAnimationBuilder_Builder(dsRunTime *rt, dsValue *myself) :
	pRT(rt), pMyself(myself), pAnimation(nullptr){
	}
	
	void BuildAnimation(deAnimation *animation) override{
		pAnimation = animation;
		
		try{
			pRT->RunFunction(pMyself, "buildAnimation", 0);
			
		}catch(const duException &e){
			pAnimation = nullptr;
			pRT->PrintExceptionTrace();
			e.PrintError();
			DETHROW(deeInvalidParam);
			
		}catch(...){
			pAnimation = nullptr;
			throw;
		}
		
		pAnimation = nullptr;
	}
	
	inline deAnimation *GetAnimation() const{ return pAnimation; }
};


struct sAnimBldNatDat{
	deClassAnimationBuilder_Builder *builder;
};



// Native Functions
/////////////////////

// public constructor new()
deClassAnimationBuilder::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimationBuilder::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder = nullptr;
}

// public destructor Destructor()
deClassAnimationBuilder::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimationBuilder::nfDestructor::RunFunction(dsRunTime*, dsValue*){
}



// public func Animation build( String filename )
deClassAnimationBuilder::nfBuild::nfBuild(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "build", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimation){
	p_AddParameter(init.clsString); // filename
}
void deClassAnimationBuilder::nfBuild::RunFunction(dsRunTime *rt, dsValue *myself){
	sAnimBldNatDat &nd = dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself));
	if(nd.builder){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = static_cast<deClassAnimationBuilder*>(GetOwnerClass())->GetDS();
	const char * const filename = rt->GetValue(0)->GetString();
	deClassAnimationBuilder_Builder builder(rt, myself);
	deAnimation::Ref animation;
	
	nd.builder = &builder;
	
	try{
		animation = ds.GetGameEngine()->GetAnimationManager()->CreateAnimation(filename, builder);
		
	}catch(...){
		nd.builder = nullptr;
		throw;
	}
	
	nd.builder = nullptr;
	ds.GetClassAnimation()->PushAnimation(rt, animation);
}



// abstract protected func void buildAnimation()
deClassAnimationBuilder::nfBuildAnimation::nfBuildAnimation(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "buildAnimation", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE | DSTM_ABSTRACT, init.clsVoid){
}
void deClassAnimationBuilder::nfBuildAnimation::RunFunction(dsRunTime*, dsValue*){
}



// protected func void addBone( String name )
deClassAnimationBuilder::nfAddBone::nfAddBone(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addBone", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassAnimationBuilder::nfAddBone::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimationBuilder_Builder * const builder = 
		dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const char * const name = rt->GetValue(0)->GetString();
	
	deTUniqueReference<deAnimationBone> bone(deTUniqueReference<deAnimationBone>::New());
	bone->SetName(name);
	builder->GetAnimation()->AddBone(std::move(bone));
}

// protected func void addVertexPositionSet(String name)
deClassAnimationBuilder::nfAddVertexPositionSet::nfAddVertexPositionSet(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addVertexPositionSet", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
}
void deClassAnimationBuilder::nfAddVertexPositionSet::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimationBuilder_Builder * const builder =
		dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const char * const name = rt->GetValue(0)->GetString();
	if(builder->GetAnimation()->GetVertexPositionSets().Has(name)){
		DSTHROW(dueInvalidParam);
	}
	
	builder->GetAnimation()->GetVertexPositionSets().Add(name);
}

// protected func void addMove( String name, float playTime )
deClassAnimationBuilder::nfAddMove::nfAddMove(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addMove", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsFloat); // playTime
}
void deClassAnimationBuilder::nfAddMove::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimationBuilder_Builder * const builder =
		dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const char * const name = rt->GetValue(0)->GetString();
	const float playTime = rt->GetValue(1)->GetFloat();
	
	deTUniqueReference<deAnimationMove> move(deTUniqueReference<deAnimationMove>::New());
	move->SetName(name);
	move->SetPlaytime(playTime);
	builder->GetAnimation()->AddMove(std::move(move));
}

// protected func void addMove( String name, float playTime, float fps )
deClassAnimationBuilder::nfAddMove2::nfAddMove2(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addMove", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsFloat); // playTime
	p_AddParameter(init.clsFloat); // fps
}
void deClassAnimationBuilder::nfAddMove2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimationBuilder_Builder * const builder =
		dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const char * const name = rt->GetValue(0)->GetString();
	const float playTime = rt->GetValue(1)->GetFloat();
	const float fps = rt->GetValue(2)->GetFloat();
	
	deTUniqueReference<deAnimationMove> move(deTUniqueReference<deAnimationMove>::New());
	move->SetName(name);
	move->SetPlaytime(playTime);
	move->SetFPS(fps);
	builder->GetAnimation()->AddMove(std::move(move));
}

// protected func void setKeyframeListCount( int move, int count )
deClassAnimationBuilder::nfSetKeyframeListCount::nfSetKeyframeListCount(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "setKeyframeListCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // move
	p_AddParameter(init.clsInteger); // count
}
void deClassAnimationBuilder::nfSetKeyframeListCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deClassAnimationBuilder_Builder * const builder =
		dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	deAnimationMove &move = builder->GetAnimation()->GetMove(rt->GetValue(0)->GetInt());
	const int count = rt->GetValue(1)->GetInt();
	
	while(move.GetKeyframeListCount() < count){
		move.AddKeyframeList(deAnimationMove::KeyframeListRef::New());
	}
}

// protected func void addKeyframe( int move, int keyFrameList, float time, Vector position, Vector rotation, Vector scale )
deClassAnimationBuilder::nfAddKeyframe::nfAddKeyframe(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addKeyframe", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // move
	p_AddParameter(init.clsInteger); // keyFrameList
	p_AddParameter(init.clsFloat); // time
	p_AddParameter(init.clsVector); // position
	p_AddParameter(init.clsVector); // rotation
	p_AddParameter(init.clsVector); // scale
}
void deClassAnimationBuilder::nfAddKeyframe::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimationBuilder_Builder * const builder = dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const deScriptingDragonScript &ds = static_cast<deClassAnimationBuilder*>(GetOwnerClass())->GetDS();
	deAnimationMove &move = builder->GetAnimation()->GetMove(rt->GetValue(0)->GetInt());
	
	deAnimationKeyframe::List &kflist = move.GetKeyframeList(rt->GetValue(1)->GetInt());
	deAnimationKeyframe keyframe;
	keyframe.SetTime(rt->GetValue(2)->GetFloat());
	keyframe.SetPosition(ds.GetClassVector()->GetVector(rt->GetValue(3)->GetRealObject()));
	keyframe.SetRotation(ds.GetClassVector()->GetVector(rt->GetValue(4)->GetRealObject()) * DEG2RAD);
	keyframe.SetScale(ds.GetClassVector()->GetVector(rt->GetValue(5)->GetRealObject()));
	kflist.Add(keyframe);
}

// protected func void setVertexPositionSetKeyframeListCount(int move, int count)
deClassAnimationBuilder::nfSetVertexPositionSetKeyframeListCount::nfSetVertexPositionSetKeyframeListCount(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "setVertexPositionSetKeyframeListCount", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // move
	p_AddParameter(init.clsInteger); // count
}
void deClassAnimationBuilder::nfSetVertexPositionSetKeyframeListCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimationBuilder_Builder * const builder = dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	const int count = rt->GetValue(1)->GetInt();
	deAnimationMove &move = builder->GetAnimation()->GetMove(rt->GetValue(0)->GetInt());
	
	while(move.GetVertexPositionSetKeyframeListCount() < count){
		move.AddVertexPositionSetKeyframeList(deAnimationMove::VertexPositionSetKeyframeListRef::New());
	}
}

// protected func void addVertexPositionSetKeyframe(int move, int keyFrameList, float time, float weight)
deClassAnimationBuilder::nfAddVertexPositionSetKeyframe::nfAddVertexPositionSetKeyframe(const sInitData &init) :
dsFunction(init.clsAnimationBuilder, "addVertexPositionSetKeyframe", DSFT_FUNCTION,
DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // move
	p_AddParameter(init.clsInteger); // keyFrameList
	p_AddParameter(init.clsFloat); // time
	p_AddParameter(init.clsFloat); // weight
}
void deClassAnimationBuilder::nfAddVertexPositionSetKeyframe::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAnimationBuilder_Builder * const builder = dedsGetNativeData<sAnimBldNatDat>(p_GetNativeData(myself)).builder;
	if(!builder || !builder->GetAnimation()){
		DSTHROW(dueInvalidAction);
	}
	
	deAnimationMove &move = builder->GetAnimation()->GetMove(rt->GetValue(0)->GetInt());
	
	deAnimationKeyframeVertexPositionSet::List &kflist =
		move.GetVertexPositionSetKeyframeList( rt->GetValue( 1 )->GetInt() );
	
	deAnimationKeyframeVertexPositionSet keyframe;
	keyframe.SetTime(rt->GetValue(2)->GetFloat());
	keyframe.SetWeight(rt->GetValue(3)->GetFloat());
	kflist.Add(keyframe);
}



// Class deClassAnimationBuilder
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassAnimationBuilder::deClassAnimationBuilder(deScriptingDragonScript &ds) :
dsClass("AnimationBuilder", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(dedsNativeDataSize<sAnimBldNatDat>());
}

deClassAnimationBuilder::~deClassAnimationBuilder(){
}



// Management
///////////////

void deClassAnimationBuilder::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsAnimationBuilder = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	init.clsAnimation = pDS.GetClassAnimation();
	init.clsVector = pDS.GetClassVector();
	
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfBuild(init));
	AddFunction(new nfBuildAnimation(init));
	AddFunction(new nfAddBone(init));
	AddFunction(new nfAddVertexPositionSet(init));
	AddFunction(new nfAddMove(init));
	AddFunction(new nfAddMove2(init));
	AddFunction(new nfSetKeyframeListCount(init));
	AddFunction(new nfAddKeyframe(init));
	AddFunction(new nfSetVertexPositionSetKeyframeListCount(init));
	AddFunction(new nfAddVertexPositionSetKeyframe(init));
}
