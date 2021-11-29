/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "deClassAnimation.h"
#include "../resources/deClassResourceListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"
#include "dragengine/resources/animation/deAnimation.h"
#include "dragengine/resources/animation/deAnimationBone.h"
#include "dragengine/resources/animation/deAnimationMove.h"
#include "dragengine/resources/animation/deAnimationManager.h"
#include "dragengine/resources/loader/deResourceLoader.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>


// native structure
struct sAnimNatDat{
	deAnimation *anim;
};

// native functions
/////////////////////

// public func new(String filename)
deClassAnimation::nfLoad::nfLoad(const sInitData &init) : dsFunction(init.clsAnim,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // name
}
void deClassAnimation::nfLoad::RunFunction(dsRunTime *RT, dsValue *This){
	sAnimNatDat *nd = (sAnimNatDat*)p_GetNativeData(This);
	deClassAnimation *clsAnim = (deClassAnimation*)GetOwnerClass();
	deAnimationManager *animMgr = clsAnim->GetGameEngine()->GetAnimationManager();
	// reset all
	nd->anim = NULL;
	// load animation
	const char *filename = RT->GetValue(0)->GetString();
	nd->anim = animMgr->LoadAnimation( filename, "/" );
	if(!nd->anim) DSTHROW(dueInvalidParam);
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassAnimation::nfLoadAsynchron::nfLoadAsynchron( const sInitData &init ) : dsFunction( init.clsAnim,
"loadAsynchron", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
	p_AddParameter( init.clsRN ); // listener
}
void deClassAnimation::nfLoadAsynchron::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimation *clsAnim = ( deClassAnimation* )GetOwnerClass();
	
	const char *filename = rt->GetValue( 0 )->GetString();
	dsRealObject *listener = rt->GetValue( 1 )->GetRealObject();
	
	if( ! listener ) DSTHROW( dueInvalidParam );
	
	clsAnim->GetScriptModule()->GetResourceLoader()->AddRequest( filename,
		deResourceLoader::ertAnimation, listener );
}

// public func void save(String filename)
deClassAnimation::nfSave::nfSave( const sInitData &init ) :
dsFunction( init.clsAnim, "save", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassAnimation::nfSave::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sAnimNatDat &nd = *( ( sAnimNatDat* )p_GetNativeData( myself) );
	deClassAnimation &clsAnim = *( ( deClassAnimation* )GetOwnerClass() );
	
	const char * const filename = rt->GetValue( 0 )->GetString();
	clsAnim.GetGameEngine()->GetAnimationManager()->SaveAnimation( *nd.anim, filename );
}

// public func destructor()
deClassAnimation::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsAnim,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAnimation::nfDestructor::RunFunction(dsRunTime *RT, dsValue *myself){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sAnimNatDat *nd = (sAnimNatDat*)p_GetNativeData( myself );
	
	if(nd->anim){
		nd->anim->FreeReference();
		nd->anim = NULL;
	}
}



// public func String getFilename()
deClassAnimation::nfGetFilename::nfGetFilename(const sInitData &init) : dsFunction(init.clsAnim,
"getFilename", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassAnimation::nfGetFilename::RunFunction(dsRunTime *RT, dsValue *This){
	deAnimation *anim = ((sAnimNatDat*)p_GetNativeData(This))->anim;
	RT->PushString( anim->GetFilename() );
}

// public func float getMovePlaytime(String moveName)
deClassAnimation::nfGetMovePlaytime::nfGetMovePlaytime(const sInitData &init) : dsFunction(init.clsAnim,
"getMovePlaytime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsStr); // moveName
}
void deClassAnimation::nfGetMovePlaytime::RunFunction(dsRunTime *RT, dsValue *This){
	deAnimation *anim = ((sAnimNatDat*)p_GetNativeData(This))->anim;
	const char *moveName = RT->GetValue(0)->GetString();
	int moveIndex = anim->FindMove(moveName);
	if(moveIndex == -1){
		RT->PushFloat( 0.0f );
	}else{
		RT->PushFloat( anim->GetMove(moveIndex)->GetPlaytime() );
	}
}

// public func int getBoneCount()
deClassAnimation::nfGetBoneCount::nfGetBoneCount( const sInitData &init ) :
dsFunction( init.clsAnim, "getBoneCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassAnimation::nfGetBoneCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushInt( animation.GetBoneCount() );
}

// public func int indexOfBoneNamed(String name)
deClassAnimation::nfIndexOfBoneNamed::nfIndexOfBoneNamed( const sInitData &init ) :
dsFunction( init.clsAnim, "indexOfBoneNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassAnimation::nfIndexOfBoneNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushInt( animation.FindBone( rt->GetValue( 0 )->GetString() ) );
}

// public func String getBoneName( int index )
deClassAnimation::nfGetBoneName::nfGetBoneName( const sInitData &init ) :
dsFunction( init.clsAnim, "getBoneName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // index
}
void deClassAnimation::nfGetBoneName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushString( animation.GetBone( rt->GetValue( 0 )->GetInt() )->GetName() );
}

// public func float getMoveFPS( String moveName )
deClassAnimation::nfGetMoveFPS::nfGetMoveFPS( const sInitData &init ) :
dsFunction( init.clsAnim, "getMoveFPS", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsStr ); // moveName
}
void deClassAnimation::nfGetMoveFPS::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &anim = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	const char * const moveName = rt->GetValue( 0 )->GetString();
	const int moveIndex = anim.FindMove( moveName );
	
	if( moveIndex == -1 ){
		rt->PushFloat( 25.0f );
		
	}else{
		rt->PushFloat( anim.GetMove( moveIndex )->GetPlaytime() );
	}
}

// public func int getMoveCount()
deClassAnimation::nfGetMoveCount::nfGetMoveCount( const sInitData &init ) :
dsFunction( init.clsAnim, "getMoveCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassAnimation::nfGetMoveCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushInt( animation.GetMoveCount() );
}

// public func String getMoveName( int index )
deClassAnimation::nfGetMoveName::nfGetMoveName( const sInitData &init ) :
dsFunction( init.clsAnim, "getMoveName", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // index
}
void deClassAnimation::nfGetMoveName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushString( animation.GetMove( rt->GetValue( 0 )->GetInt() )->GetName() );
}

// public func int indexOfMoveNamed(String name)
deClassAnimation::nfIndexOfMoveNamed::nfIndexOfMoveNamed( const sInitData &init ) :
dsFunction( init.clsAnim, "indexOfMoveNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassAnimation::nfIndexOfMoveNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deAnimation &animation = *( ( ( sAnimNatDat* )p_GetNativeData( myself ) )->anim );
	rt->PushInt( animation.FindMove( rt->GetValue( 0 )->GetString() ) );
}



// public func int hashCode()
deClassAnimation::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsAnim, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassAnimation::nfHashCode::RunFunction(dsRunTime *RT, dsValue *This){
	sAnimNatDat *nd = (sAnimNatDat*)p_GetNativeData(This);
	// hash code = memory location
	RT->PushInt( ( intptr_t )nd->anim );
}

// public func bool equals(Object obj)
deClassAnimation::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsAnim, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassAnimation::nfEquals::RunFunction(dsRunTime *RT, dsValue *This){
	sAnimNatDat *nd = (sAnimNatDat*)p_GetNativeData(This);
	deClassAnimation *clsAnim = (deClassAnimation*)GetOwnerClass();
	dsValue *obj = RT->GetValue(0);
	if( !p_IsObjOfType(obj, clsAnim) ){
		RT->PushBool(false);
	}else{
		sAnimNatDat *other = (sAnimNatDat*)p_GetNativeData(obj);
		RT->PushBool( nd->anim == other->anim );
	}
}



// class deClassAnimation
///////////////////////
// constructor
deClassAnimation::deClassAnimation(deEngine *GameEngine, deScriptingDragonScript *ScrMgr) :
dsClass("Animation", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!GameEngine || !ScrMgr) DSTHROW(dueInvalidParam);
	// prepare
	p_gameEngine = GameEngine;
	pScrMgr = ScrMgr;
	// set parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	// do the rest
	p_SetNativeDataSize(sizeof(sAnimNatDat));
}
deClassAnimation::~deClassAnimation(){
}
// management
void deClassAnimation::CreateClassMembers(dsEngine *engine){
	sInitData init;
	// store classes
	pClsRN = pScrMgr->GetClassResourceListener();
	init.clsAnim = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsObj = engine->GetClassObject();
	init.clsRN = pClsRN;
	// add functions
	AddFunction(new nfLoad(init));
	AddFunction( new nfLoadAsynchron( init ) );
	AddFunction( new nfSave( init ) );
	AddFunction(new nfDestructor(init));
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetMovePlaytime(init));
	AddFunction( new nfGetBoneCount( init ) );
	AddFunction( new nfIndexOfBoneNamed( init ) );
	AddFunction( new nfGetBoneName( init ) );
	AddFunction( new nfGetMoveFPS( init ) );
	AddFunction( new nfGetMoveCount( init ) );
	AddFunction( new nfGetMoveName( init ) );
	AddFunction( new nfIndexOfMoveNamed( init ) );
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	// calculate member offsets
	CalcMemberOffsets();
}
deAnimation *deClassAnimation::GetAnimation( dsRealObject *object ) const{
	if( ! object ){
		return NULL;
	}
	
	return ( ( sAnimNatDat* )p_GetNativeData( object->GetBuffer() ) )->anim;
}

void deClassAnimation::PushAnimation( dsRunTime *rt, deAnimation *anim ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! anim ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sAnimNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->anim = anim;
	anim->AddReference();
}
