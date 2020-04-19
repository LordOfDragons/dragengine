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

#include "deClassARStateSnapshot.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARSnapNatDat{
	deAnimator *animator;
	deAnimatorRuleStateSnapshot *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARStateSnapshot::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARSnap,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARStateSnapshot::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleStateSnapshot;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARStateSnapshot::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARSnap,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARStateSnapshot::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
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
deClassARStateSnapshot::nfSetEnablePosition::nfSetEnablePosition( const sInitData &init ) : dsFunction( init.clsARSnap,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateSnapshot::nfSetEnablePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePosition( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARStateSnapshot::nfSetEnableOrientation::nfSetEnableOrientation( const sInitData &init ) : dsFunction( init.clsARSnap,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateSnapshot::nfSetEnableOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableOrientation( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARStateSnapshot::nfSetEnableSize::nfSetEnableSize( const sInitData &init ) : dsFunction( init.clsARSnap,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateSnapshot::nfSetEnableSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableSize( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setUseLastState( bool useLastState )
deClassARStateSnapshot::nfSetUseLastState::nfSetUseLastState( const sInitData &init ) : dsFunction( init.clsARSnap,
"setUseLastState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // useLastState
}
void deClassARStateSnapshot::nfSetUseLastState::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetUseLastState( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setID( int identifier )
deClassARStateSnapshot::nfSetID::nfSetID( const sInitData &init ) : dsFunction( init.clsARSnap,
"setID", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // identifier
}
void deClassARStateSnapshot::nfSetID::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetID( rt->GetValue( 0 )->GetInt() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARStateSnapshotTarget target, int link )
deClassARStateSnapshot::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARSnap,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARStateSnapshotTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARStateSnapshot::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	const deClassARStateSnapshot::eTargets target = ( deClassARStateSnapshot::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARStateSnapshot::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARStateSnapshotTarget target )
deClassARStateSnapshot::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARSnap,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARStateSnapshotTarget ); // target
}
void deClassARStateSnapshot::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself ) );
	const deClassARStateSnapshot::eTargets target = ( deClassARStateSnapshot::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARStateSnapshot::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARStateSnapshot
/////////////////////////////

// Constructor
////////////////

deClassARStateSnapshot::deClassARStateSnapshot( deScriptingDragonScript &ds ) :
dsClass( "ARStateSnapshot", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARSnapNatDat ) );
}

deClassARStateSnapshot::~deClassARStateSnapshot(){
}



// Management
///////////////

void deClassARStateSnapshot::CreateClassMembers( dsEngine *engine ){
	pClsARStateSnapshotTarget = engine->GetClass( "Dragengine.Scenery.ARStateSnapshotTarget" );
	
	sInitData init;
	init.clsARSnap = this;
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
	init.clsARStateSnapshotTarget = pClsARStateSnapshotTarget;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetEnablePosition( init ) );
	AddFunction( new nfSetEnableOrientation( init ) );
	AddFunction( new nfSetEnableSize( init ) );
	
	AddFunction( new nfSetUseLastState( init ) );
	AddFunction( new nfSetID( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleStateSnapshot *deClassARStateSnapshot::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARSnapNatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARStateSnapshot::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassARStateSnapshot::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleStateSnapshot *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARSnapNatDat &nd = *( ( sARSnapNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
