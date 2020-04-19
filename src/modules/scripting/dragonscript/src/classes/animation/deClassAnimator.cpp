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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassAnimator.h"
#include "deClassAnimation.h"
#include "deClassAnimatorController.h"
#include "deClassAnimatorRule.h"
#include "../world/deClassRig.h"
#include "../curve/deClassCurveBezier.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>



// native structure
struct sArNatDat{
	deAnimator *animator;
};



// native functions
/////////////////////

// public func new()
deClassAnimator::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsAr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassAnimator::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sArNatDat *nd = ( sArNatDat* )p_GetNativeData( myself );
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	deAnimatorManager *aniMgr = clsAr->GetDS()->GetGameEngine()->GetAnimatorManager();
	// clear ( important )
	nd->animator = NULL;
	// create animator
	nd->animator = aniMgr->CreateAnimator();
	if( ! nd->animator ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassAnimator::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsAr,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassAnimator::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sArNatDat *nd = ( sArNatDat* )p_GetNativeData( myself );
	
	if( nd->animator ){
		nd->animator->FreeReference();
		nd->animator = NULL;
	}
}



// public func Rig getRig()
deClassAnimator::nfGetRig::nfGetRig( const sInitData &init ) : dsFunction( init.clsAr,
"getRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsRig ){
}
void deClassAnimator::nfGetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	clsAr->GetDS()->GetClassRig()->PushRig( rt, animator->GetRig() );
}

// public func void setRig( Rig rig )
deClassAnimator::nfSetRig::nfSetRig( const sInitData &init ) : dsFunction( init.clsAr,
"setRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsRig ); // rig
}
void deClassAnimator::nfSetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	dsRealObject *objRig = rt->GetValue( 0 )->GetRealObject();
	
	animator->SetRig( clsAr->GetDS()->GetClassRig()->GetRig( objRig ) );
}

// public func Animation getAnimation()
deClassAnimator::nfGetAnimation::nfGetAnimation( const sInitData &init ) : dsFunction( init.clsAr,
"getAnimation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsAni ){
}
void deClassAnimator::nfGetAnimation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	clsAr->GetDS()->GetClassAnimation()->PushAnimation( rt, animator->GetAnimation() );
}

// public func void setAnimation( Animation animation )
deClassAnimator::nfSetAnimation::nfSetAnimation( const sInitData &init ) : dsFunction( init.clsAr,
"setAnimation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsAni ); // animation
}
void deClassAnimator::nfSetAnimation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	dsRealObject *objAnimation = rt->GetValue( 0 )->GetRealObject();
	
	animator->SetAnimation( clsAr->GetDS()->GetClassAnimation()->GetAnimation( objAnimation ) );
}



// public func int getControllerCount()
deClassAnimator::nfGetControllerCount::nfGetControllerCount( const sInitData &init ) : dsFunction( init.clsAr,
"getControllerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassAnimator::nfGetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	rt->PushInt( animator->GetControllerCount() );
}

// public func void setControllerCount( int count )
deClassAnimator::nfSetControllerCount::nfSetControllerCount( const sInitData &init ) : dsFunction( init.clsAr,
"setControllerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassAnimator::nfSetControllerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	int count = rt->GetValue( 0 )->GetInt();
	deAnimatorController *controller = NULL;
	
	if( count < 0 ) DSTHROW( dueInvalidParam );
	
	while( animator->GetControllerCount() > count ){
		animator->RemoveController( animator->GetControllerAt( animator->GetControllerCount() - 1 ) );
	}
	
	try{
		while( animator->GetControllerCount() < count ){
			controller = new deAnimatorController;
			if( ! controller ) DSTHROW( dueOutOfMemory );
			animator->AddController( controller );
			controller = NULL;
		}
	}catch( ... ){
		if( controller ) delete controller;
		throw;
	}
}

// public func AnimatorController getControllerAt( int index )
deClassAnimator::nfGetControllerAt::nfGetControllerAt( const sInitData &init ) : dsFunction( init.clsAr,
"getControllerAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorCtrl ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassAnimator::nfGetControllerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator * const animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	const deScriptingDragonScript &ds = *( ( ( deClassAnimator* )GetOwnerClass() )->GetDS() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 ){
		ds.GetClassAnimatorController()->PushController( rt, animator,
			animator->GetControllerCount() + index );
		
	}else{
		ds.GetClassAnimatorController()->PushController( rt, animator, index );
	}
}

// public func AnimatorController getControllerNamed( String name )
deClassAnimator::nfGetControllerNamed::nfGetControllerNamed( const sInitData &init ) : dsFunction( init.clsAr,
"getControllerNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsAnimatorCtrl ){
	p_AddParameter( init.clsStr ); // name
}
void deClassAnimator::nfGetControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator * const animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	const deScriptingDragonScript &ds = *( ( ( deClassAnimator* )GetOwnerClass() )->GetDS() );
	const int index = animator->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() );
	
	if( index == -1 ){
		rt->PushObject( NULL, ds.GetClassAnimatorController() );
		
	}else{
		ds.GetClassAnimatorController()->PushController( rt, animator, index );
	}
}

// public func int indexOfControllerNamed( String name )
deClassAnimator::nfIndexOfControllerNamed::nfIndexOfControllerNamed( const sInitData &init ) : dsFunction( init.clsAr,
"indexOfControllerNamed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsStr ); // name
}
void deClassAnimator::nfIndexOfControllerNamed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator * const animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	rt->PushInt( animator->IndexOfControllerNamed( rt->GetValue( 0 )->GetString() ) );
}



// public func int getLinkCount()
deClassAnimator::nfGetLinkCount::nfGetLinkCount( const sInitData &init ) : dsFunction( init.clsAr,
"getLinkCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassAnimator::nfGetLinkCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	rt->PushInt( animator->GetLinkCount() );
}

// public func int addLink( int controller )
deClassAnimator::nfAddLink::nfAddLink( const sInitData &init ) : dsFunction( init.clsAr,
"addLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // controller
}
void deClassAnimator::nfAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	deAnimatorLink *link = NULL;
	
	try{
		link = new deAnimatorLink;
		if( ! link ) DSTHROW( dueOutOfMemory );
		
		link->SetController( rt->GetValue( 0 )->GetInt() );
		
		animator->AddLink( link );
		
	}catch( ... ){
		if( link ) delete link;
		throw;
	}
	
	rt->PushInt( animator->GetLinkCount() - 1 );
}

// public func void removeAllLinks()
deClassAnimator::nfRemoveAllLinks::nfRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsAr,
"removeAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassAnimator::nfRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	animator->RemoveAllLinks();
}

// public func void setLinkController( int link, int controller );
deClassAnimator::nfSetLinkController::nfSetLinkController( const sInitData &init ) : dsFunction( init.clsAr,
"setLinkController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // link
	p_AddParameter( init.clsInt ); // controller
}
void deClassAnimator::nfSetLinkController::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	int index = rt->GetValue( 0 )->GetInt();
	deAnimatorLink *link = animator->GetLinkAt( index );
	
	link->SetController( rt->GetValue( 1 )->GetInt() );
	
	animator->NotifyLinkChangedAt( index );
}

// public func void setLinkCurve( int link, CurveBezier curve );
deClassAnimator::nfSetLinkCurve::nfSetLinkCurve( const sInitData &init ) :
dsFunction( init.clsAr, "setLinkCurve", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // link
	p_AddParameter( init.clsCurveBezier ); // curve
}
void deClassAnimator::nfSetLinkCurve::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator &animator = *( ( ( sArNatDat* )p_GetNativeData( myself ) )->animator );
	deScriptingDragonScript &ds = *( ( ( deClassAnimator* )GetOwnerClass() )->GetDS() );
	
	const int index = rt->GetValue( 0 )->GetInt();
	const decCurveBezier &curve = ds.GetClassCurveBezier()->GetCurve( rt->GetValue( 1 )->GetRealObject() );
	
	animator.GetLinkAt( index )->SetCurve( curve );
	
	animator.NotifyLinkChangedAt( index );
}

// public func void setLinkRepeat( int link, int repeat );
deClassAnimator::nfSetLinkRepeat::nfSetLinkRepeat( const sInitData &init ) : dsFunction( init.clsAr,
"setLinkRepeat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // link
	p_AddParameter( init.clsInt ); // repeat
}
void deClassAnimator::nfSetLinkRepeat::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator &animator = *( ( ( sArNatDat* )p_GetNativeData( myself ) )->animator );
	
	const int index = rt->GetValue( 0 )->GetInt();
	deAnimatorLink &link = *animator.GetLinkAt( index );
	
	link.SetRepeat( rt->GetValue( 1 )->GetInt() );
	
	animator.NotifyLinkChangedAt( index );
}



// public func int getRuleCount()
deClassAnimator::nfGetRuleCount::nfGetRuleCount( const sInitData &init ) : dsFunction( init.clsAr,
"getRuleCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassAnimator::nfGetRuleCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	rt->PushInt( animator->GetRuleCount() );
}

// public func void addRule( AnimatorRule rule )
deClassAnimator::nfAddRule::nfAddRule( const sInitData &init ) : dsFunction( init.clsAr,
"addRule", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsArR ); // rule
}
void deClassAnimator::nfAddRule::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator &clsAr = *( ( deClassAnimator* )GetOwnerClass() );
	deClassAnimatorRule &clsArR = *clsAr.GetDS()->GetClassAnimatorRule();
	
	dsRealObject *objRule = rt->GetValue( 0 )->GetRealObject();
	if( ! objRule ) DSTHROW( dueNullPointer );
	
	deAnimatorRule *rule = clsArR.GetRule( objRule );
	
	animator->AddRule( rule );
	
	clsArR.AssignAnimator( objRule, animator );
}

// public func AnimatorRule getRuleAt( int position )
deClassAnimator::nfGetRuleAt::nfGetRuleAt( const sInitData &init ) : dsFunction( init.clsAr,
"getRuleAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsArR ){
	p_AddParameter( init.clsInt ); // position
}
void deClassAnimator::nfGetRuleAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator &clsAr = *( ( deClassAnimator* )GetOwnerClass() );
	deClassAnimatorRule &clsArR = *clsAr.GetDS()->GetClassAnimatorRule();
	
	const int position = rt->GetValue( 0 )->GetInt();
	deAnimatorRule * const rule = animator->GetRuleAt( position );
	
	clsArR.PushRule( rt, animator, rule );
}

// DEPRECATED Remove this function call since it is not useful
// public func void removeRuleAt( int index )
deClassAnimator::nfRemoveRuleAt::nfRemoveRuleAt( const sInitData &init ) : dsFunction( init.clsAr,
"removeRuleAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassAnimator::nfRemoveRuleAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	animator->RemoveRule( animator->GetRuleAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func void removeAllRules()
deClassAnimator::nfRemoveAllRules::nfRemoveAllRules( const sInitData &init ) : dsFunction( init.clsAr,
"removeAllRules", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassAnimator::nfRemoveAllRules::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	
	animator->RemoveAllRules();
}



// public func void addBone( String boneName )
deClassAnimator::nfAddBone::nfAddBone( const sInitData &init ) : dsFunction( init.clsArR,
"addBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassAnimator::nfAddBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator &animator = *( ( ( sArNatDat* )p_GetNativeData( myself ) )->animator );
	
	animator.GetListBones().Add( rt->GetValue( 0 )->GetString() );
	animator.NotifyBonesChanged();
}

// public func void removeAllBones()
deClassAnimator::nfRemoveAllBones::nfRemoveAllBones( const sInitData &init ) : dsFunction( init.clsArR,
"removeAllBones", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassAnimator::nfRemoveAllBones::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator &animator = *( ( ( sArNatDat* )p_GetNativeData( myself ) )->animator );
	
	animator.GetListBones().RemoveAll();
	animator.NotifyBonesChanged();
}



// public func int hashCode()
deClassAnimator::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsAr,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassAnimator::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	// hash code = memory location
	rt->PushInt( ( intptr_t )animator );
}

// public func bool equals( Object obj )
deClassAnimator::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsAr,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassAnimator::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deAnimator *animator = ( ( sArNatDat* )p_GetNativeData( myself ) )->animator;
	deClassAnimator *clsAr = ( deClassAnimator* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsAr ) ){
		rt->PushBool( false );
	}else{
		deAnimator *otherAnimator = ( ( sArNatDat* )p_GetNativeData( obj ) )->animator;
		rt->PushBool( animator == otherAnimator );
	}
}

// public static func bool equals( Animator animator1, Animator animator2 )
deClassAnimator::nfEquals2::nfEquals2( const sInitData &init ) :
dsFunction( init.clsAr, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsAr ); // animator1
	p_AddParameter( init.clsAr ); // animator2
}
void deClassAnimator::nfEquals2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimator &clsAr = *( ( deClassAnimator* )GetOwnerClass() );
	deAnimator *animator1 = clsAr.GetAnimator( rt->GetValue( 0 )->GetRealObject() );
	deAnimator *animator2 = clsAr.GetAnimator( rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushBool( animator1 == animator2 );
}



// class deClassAnimator
//////////////////////////

// Constructor
////////////////

deClassAnimator::deClassAnimator( deScriptingDragonScript *ds ) :
dsClass( "Animator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	// prepare
	pDS = ds;
	
	// store informations into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sArNatDat ) );
}

deClassAnimator::~deClassAnimator(){
}



// Management
///////////////

void deClassAnimator::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsAr = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsArR = pDS->GetClassAnimatorRule();
	init.clsRig = pDS->GetClassRig();
	init.clsAni = pDS->GetClassAnimation();
	init.clsCurveBezier = pDS->GetClassCurveBezier();
	init.clsAnimatorCtrl = pDS->GetClassAnimatorController();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetRig( init ) );
	AddFunction( new nfSetRig( init ) );
	AddFunction( new nfGetAnimation( init ) );
	AddFunction( new nfSetAnimation( init ) );
	
	AddFunction( new nfGetControllerCount( init ) );
	AddFunction( new nfSetControllerCount( init ) );
	AddFunction( new nfGetControllerAt( init ) );
	AddFunction( new nfGetControllerNamed( init ) );
	AddFunction( new nfIndexOfControllerNamed( init ) );
	
	AddFunction( new nfGetLinkCount( init ) );
	AddFunction( new nfAddLink( init ) );
	AddFunction( new nfRemoveAllLinks( init ) );
	AddFunction( new nfSetLinkController( init ) );
	AddFunction( new nfSetLinkCurve( init ) );
	AddFunction( new nfSetLinkRepeat( init ) );
	
	AddFunction( new nfGetRuleCount( init ) );
	AddFunction( new nfAddRule( init ) );
	AddFunction( new nfGetRuleAt( init ) );
	AddFunction( new nfRemoveRuleAt( init ) );
	AddFunction( new nfRemoveAllRules( init ) );
	
	AddFunction( new nfAddBone( init ) );
	AddFunction( new nfRemoveAllBones( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfEquals2( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimator *deClassAnimator::GetAnimator( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sArNatDat* )p_GetNativeData( myself->GetBuffer() ) )->animator;
}

void deClassAnimator::PushAnimator( dsRunTime *rt, deAnimator *animator ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! animator ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sArNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->animator = animator;
	animator->AddReference();
}
