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

#include "deClassARTrackTo.h"
#include "../deClassAnimator.h"
#include "../deClassAnimatorRule.h"
#include "../../math/deClassVector.h"
#include "../../math/deClassVector2.h"
#include "../../math/deClassQuaternion.h"
#include "../../collider/deClassCollider.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARTrackNatDat{
	deAnimator *animator;
	deAnimatorRuleTrackTo *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARTrackTo::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARTrack,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARTrackTo::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleTrackTo;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARTrackTo::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARTrack,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARTrackTo::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	if( nd.animator ){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if( nd.rule ){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}



// public func void setTrackBone( String bone )
deClassARTrackTo::nfSetTrackBone::nfSetTrackBone( const sInitData &init ) : dsFunction( init.clsARTrack,
"setTrackBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // bone
}
void deClassARTrackTo::nfSetTrackBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetTrackBone( rt->GetValue( 0 )->GetString() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setTrackAxis( ARTrackToAxis axis )
deClassARTrackTo::nfSetTrackAxis::nfSetTrackAxis( const sInitData &init ) : dsFunction( init.clsARTrack,
"setTrackAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToAxis ); // axis
}
void deClassARTrackTo::nfSetTrackAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetTrackAxis( ( deAnimatorRuleTrackTo::eTrackAxis )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUpAxis( ARTrackToAxis axis )
deClassARTrackTo::nfSetUpAxis::nfSetUpAxis( const sInitData &init ) : dsFunction( init.clsARTrack,
"setUpAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToAxis ); // axis
}
void deClassARTrackTo::nfSetUpAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetUpAxis( ( deAnimatorRuleTrackTo::eTrackAxis )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setUpTarget( ARTrackToUp target )
deClassARTrackTo::nfSetUpTarget::nfSetUpTarget( const sInitData &init ) : dsFunction( init.clsARTrack,
"setUpTarget", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToUp ); // target
}
void deClassARTrackTo::nfSetUpTarget::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetUpTarget( ( deAnimatorRuleTrackTo::eUpTarget )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setLockedAxis( ARTrackToLocked axis )
deClassARTrackTo::nfSetLockedAxis::nfSetLockedAxis( const sInitData &init ) : dsFunction( init.clsARTrack,
"setLockedAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToLocked ); // axis
}
void deClassARTrackTo::nfSetLockedAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	
	nd.rule->SetLockedAxis( ( deAnimatorRuleTrackTo::eLockedAxis )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARTrackToTarget target, int link )
deClassARTrackTo::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARTrack,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARTrackTo::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	const deClassARTrackTo::eTargets target = ( deClassARTrackTo::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARTrackTo::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassARTrackTo::etPosition:
		nd.rule->GetTargetPosition().AddLink( link );
		break;
		
	case deClassARTrackTo::etUp:
		nd.rule->GetTargetUp().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARTrackToTarget target )
deClassARTrackTo::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARTrack,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARTrackToTarget ); // target
}
void deClassARTrackTo::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself ) );
	const deClassARTrackTo::eTargets target = ( deClassARTrackTo::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARTrackTo::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARTrackTo::etPosition:
		nd.rule->GetTargetPosition().RemoveAllLinks();
		break;
		
	case deClassARTrackTo::etUp:
		nd.rule->GetTargetUp().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARTrackTo
/////////////////////////////

// Constructor
////////////////

deClassARTrackTo::deClassARTrackTo( deScriptingDragonScript &ds ) :
dsClass( "ARTrackTo", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARTrackNatDat ) );
}

deClassARTrackTo::~deClassARTrackTo(){
}



// Management
///////////////

void deClassARTrackTo::CreateClassMembers( dsEngine *engine ){
	pClsARTrackToTarget = engine->GetClass( "Dragengine.Scenery.ARTrackToTarget" );
	pClsARTrackToAxis = engine->GetClass( "Dragengine.Scenery.ARTrackToAxis" );
	pClsARTrackToUp = engine->GetClass( "Dragengine.Scenery.ARTrackToUp" );
	pClsARTrackToLocked = engine->GetClass( "Dragengine.Scenery.ARTrackToLocked" );
	
	sInitData init;
	init.clsARTrack = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsVec2 = pDS.GetClassVector2();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsAr = pDS.GetClassAnimator();
	init.clsCol = pDS.GetClassCollider();
	init.clsARTrackToTarget = pClsARTrackToTarget;
	init.clsARTrackToAxis = pClsARTrackToAxis;
	init.clsARTrackToUp = pClsARTrackToUp;
	init.clsARTrackToLocked = pClsARTrackToLocked;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetTrackBone( init ) );
	AddFunction( new nfSetTrackAxis( init ) );
	AddFunction( new nfSetUpAxis( init ) );
	AddFunction( new nfSetUpTarget( init ) );
	AddFunction( new nfSetLockedAxis( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleTrackTo *deClassARTrackTo::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARTrackNatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARTrackTo::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( animator == nd.animator ){
		return;
	}
	
	if( nd.animator ){
		nd.animator->FreeReference();
	}
	
	nd.animator = animator;
	
	if( animator ){
		animator->AddReference();
	}
}

void deClassARTrackTo::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleTrackTo *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARTrackNatDat &nd = *( ( sARTrackNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.animator = NULL;
	nd.rule = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.animator = animator;
		if( animator ){
			animator->AddReference();
		}
		
		nd.rule = rule;
		rule->AddReference();
		
		baseClass->AssignRule( rt->GetValue( 0 )->GetRealObject(), rule );
		baseClass->AssignAnimator( rt->GetValue( 0 )->GetRealObject(), animator );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
