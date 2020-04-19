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

#include "deClassARSubAnimator.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARSubANatDat{
	deAnimator *animator;
	deAnimatorRuleSubAnimator *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARSubAnimator::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARSubA,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARSubAnimator::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleSubAnimator;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARSubAnimator::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARSubA,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARSubAnimator::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	if( nd.animator ){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if( nd.rule ){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}




// public func void setEnablePosition( bool enabled )
deClassARSubAnimator::nfSetEnablePosition::nfSetEnablePosition( const sInitData &init ) : dsFunction( init.clsARSubA,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARSubAnimator::nfSetEnablePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePosition( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARSubAnimator::nfSetEnableOrientation::nfSetEnableOrientation( const sInitData &init ) : dsFunction( init.clsARSubA,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARSubAnimator::nfSetEnableOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableOrientation( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARSubAnimator::nfSetEnableSize::nfSetEnableSize( const sInitData &init ) : dsFunction( init.clsARSubA,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARSubAnimator::nfSetEnableSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableSize( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARSubAnimatorTarget target, int link )
deClassARSubAnimator::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARSubA,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARSubAnimatorTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARSubAnimator::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	const deClassARSubAnimator::eTargets target = ( deClassARSubAnimator::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARSubAnimator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARSubAnimatorTarget target )
deClassARSubAnimator::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARSubA,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARSubAnimatorTarget ); // target
}
void deClassARSubAnimator::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	const deClassARSubAnimator::eTargets target = ( deClassARSubAnimator::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARSubAnimator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setSubAnimator( Animator animator )
deClassARSubAnimator::nfSetSubAnimator::nfSetSubAnimator( const sInitData &init ) : dsFunction( init.clsARSubA,
"setSubAnimator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsAr ); // animator
}
void deClassARSubAnimator::nfSetSubAnimator::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	deClassARSubAnimator *clsARSubA = ( deClassARSubAnimator* )GetOwnerClass();
	deClassAnimator *clsAr = clsARSubA->GetDS().GetClassAnimator();
	
	dsRealObject *objAnimator = rt->GetValue( 0 )->GetRealObject();
	
	nd.rule->SetSubAnimator( clsAr->GetAnimator( objAnimator ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setConnectionAt( int target, int controller )
deClassARSubAnimator::nfSetConnectionAt::nfSetConnectionAt( const sInitData &init ) : dsFunction( init.clsARSubA,
"setConnectionAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // target
	p_AddParameter( init.clsInt ); // controller
}
void deClassARSubAnimator::nfSetConnectionAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	const int target = rt->GetValue( 0 )->GetInt();
	const int controller = rt->GetValue( 1 )->GetInt();
	
	if( target > 0 || target < nd.rule->GetConnectionCount() ){
		nd.rule->SetConnectionAt( target, controller );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMatchingConnections( Animator animator )
deClassARSubAnimator::nfSetMatchingConnections::nfSetMatchingConnections( const sInitData &init ) :
dsFunction( init.clsARSubA, "setMatchingConnections", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsAr );
}
void deClassARSubAnimator::nfSetMatchingConnections::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassARSubAnimator* )GetOwnerClass() )->GetDS();
	
	deAnimator * const animator = ds.GetClassAnimator()->GetAnimator( rt->GetValue( 0 )->GetRealObject() );
	if( ! animator ){
		DSTHROW( dueNullPointer );
	}
	
	nd.rule->SetMatchingConnections( *animator );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void clearConnections()
deClassARSubAnimator::nfClearConnections::nfClearConnections( const sInitData &init ) :
dsFunction( init.clsARSubA, "clearConnections", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARSubAnimator::nfClearConnections::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself ) );
	
	nd.rule->ClearConnections();
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARSubAnimator
/////////////////////////////

// Constructor
////////////////

deClassARSubAnimator::deClassARSubAnimator( deScriptingDragonScript &ds ) :
dsClass( "ARSubAnimator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARSubANatDat ) );
}

deClassARSubAnimator::~deClassARSubAnimator(){
}



// Management
///////////////

void deClassARSubAnimator::CreateClassMembers( dsEngine *engine ){
	pClsARSubAnimatorTarget = engine->GetClass( "Dragengine.Scenery.ARSubAnimatorTarget" );
	
	sInitData init;
	init.clsARSubA = this;
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
	init.clsARSubAnimatorTarget = pClsARSubAnimatorTarget;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetEnablePosition( init ) );
	AddFunction( new nfSetEnableOrientation( init ) );
	AddFunction( new nfSetEnableSize( init ) );
	
	AddFunction( new nfSetSubAnimator( init ) );
	AddFunction( new nfSetConnectionAt( init ) );
	AddFunction( new nfSetMatchingConnections( init ) );
	AddFunction( new nfClearConnections( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleSubAnimator *deClassARSubAnimator::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARSubANatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARSubAnimator::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassARSubAnimator::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleSubAnimator *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARSubANatDat &nd = *( ( sARSubANatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
