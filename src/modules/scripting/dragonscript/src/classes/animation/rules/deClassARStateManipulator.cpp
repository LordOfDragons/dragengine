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

#include "deClassARStateManipulator.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleStateManipulator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARStaMNatDat{
	deAnimator *animator;
	deAnimatorRuleStateManipulator *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARStateManipulator::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARStaM,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARStateManipulator::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleStateManipulator;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARStateManipulator::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARStaM,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARStateManipulator::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	
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
deClassARStateManipulator::nfSetEnablePosition::nfSetEnablePosition( const sInitData &init ) : dsFunction( init.clsARStaM,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateManipulator::nfSetEnablePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePosition( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARStateManipulator::nfSetEnableOrientation::nfSetEnableOrientation( const sInitData &init ) : dsFunction( init.clsARStaM,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateManipulator::nfSetEnableOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableRotation( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARStateManipulator::nfSetEnableSize::nfSetEnableSize( const sInitData &init ) : dsFunction( init.clsARStaM,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARStateManipulator::nfSetEnableSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableSize( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setMinimumPosition( Vector position )
deClassARStateManipulator::nfSetMinimumPosition::nfSetMinimumPosition( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMinimumPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // position
}
void deClassARStateManipulator::nfSetMinimumPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumPosition( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumPosition( Vector position )
deClassARStateManipulator::nfSetMaximumPosition::nfSetMaximumPosition( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMaximumPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // position
}
void deClassARStateManipulator::nfSetMaximumPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumPosition( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumRotation( Vector rotation )
deClassARStateManipulator::nfSetMinimumRotation::nfSetMinimumRotation( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMinimumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // rotation
}
void deClassARStateManipulator::nfSetMinimumRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumRotation( vector * DEG2RAD );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumRotation( Vector rotation )
deClassARStateManipulator::nfSetMaximumRotation::nfSetMaximumRotation( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMaximumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // rotation
}
void deClassARStateManipulator::nfSetMaximumRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumRotation( vector * DEG2RAD );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumSize( Vector size )
deClassARStateManipulator::nfSetMinimumSize::nfSetMinimumSize( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMinimumSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // size
}
void deClassARStateManipulator::nfSetMinimumSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumSize( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumSize( Vector size )
deClassARStateManipulator::nfSetMaximumSize::nfSetMaximumSize( const sInitData &init ) : dsFunction( init.clsARStaM,
"setMaximumSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // size
}
void deClassARStateManipulator::nfSetMaximumSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator &clsARStaM = *( ( deClassARStateManipulator* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARStaM.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumSize( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARStateManipulatorTarget target, int link )
deClassARStateManipulator::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARStaM,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARStateManipulatorTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARStateManipulator::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator::eTargets target = ( deClassARStateManipulator::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARStateManipulator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassARStateManipulator::etPosition:
		nd.rule->GetTargetPosition().AddLink( link );
		break;
		
	case deClassARStateManipulator::etOrientation:
		nd.rule->GetTargetRotation().AddLink( link );
		break;
		
	case deClassARStateManipulator::etSize:
		nd.rule->GetTargetSize().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARStateManipulatorTarget target )
deClassARStateManipulator::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARStaM,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARStateManipulatorTarget ); // target
}
void deClassARStateManipulator::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself ) );
	const deClassARStateManipulator::eTargets target = ( deClassARStateManipulator::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARStateManipulator::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARStateManipulator::etPosition:
		nd.rule->GetTargetPosition().RemoveAllLinks();
		break;
		
	case deClassARStateManipulator::etOrientation:
		nd.rule->GetTargetRotation().RemoveAllLinks();
		break;
		
	case deClassARStateManipulator::etSize:
		nd.rule->GetTargetSize().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARStateManipulator
////////////////////////////////////

// Constructor
////////////////

deClassARStateManipulator::deClassARStateManipulator( deScriptingDragonScript &ds ) :
dsClass( "ARStateManipulator", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARStaMNatDat ) );
}

deClassARStateManipulator::~deClassARStateManipulator(){
}



// Management
///////////////

void deClassARStateManipulator::CreateClassMembers( dsEngine *engine ){
	pClsARStateManipulatorTarget = engine->GetClass( "Dragengine.Scenery.ARStateManipulatorTarget" );
	
	sInitData init;
	init.clsARStaM = this;
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
	init.clsARStateManipulatorTarget = pClsARStateManipulatorTarget;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetEnablePosition( init ) );
	AddFunction( new nfSetEnableOrientation( init ) );
	AddFunction( new nfSetEnableSize( init ) );
	
	AddFunction( new nfSetMinimumPosition( init ) );
	AddFunction( new nfSetMaximumPosition( init ) );
	AddFunction( new nfSetMinimumRotation( init ) );
	AddFunction( new nfSetMaximumRotation( init ) );
	AddFunction( new nfSetMinimumSize( init ) );
	AddFunction( new nfSetMaximumSize( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleStateManipulator *deClassARStateManipulator::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARStaMNatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARStateManipulator::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassARStateManipulator::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleStateManipulator *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARStaMNatDat &nd = *( ( sARStaMNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
