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

#include "deClassARLimit.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARLimitNatDat{
	deAnimator *animator;
	deAnimatorRuleLimit *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARLimit::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARLimit,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARLimit::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleLimit;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARLimit::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARLimit,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARLimit::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	if( nd.animator ){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if( nd.rule ){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}



// public func void setEnablePosMin( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnablePosMin::nfSetEnablePosMin( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnablePosMin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnablePosMin::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePositionXMin( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnablePositionYMin( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnablePositionZMin( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnablePosMax( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnablePosMax::nfSetEnablePosMax( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnablePosMax", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnablePosMax::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePositionXMax( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnablePositionYMax( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnablePositionZMax( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableRotMin( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnableRotMin::nfSetEnableRotMin( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnableRotMin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnableRotMin::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableRotationXMin( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnableRotationYMin( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnableRotationZMin( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableRotMax( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnableRotMax::nfSetEnableRotMax( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnableRotMax", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnableRotMax::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableRotationXMax( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnableRotationYMax( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnableRotationZMax( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableScaleMin( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnableScaleMin::nfSetEnableScaleMin( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnableScaleMin", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnableScaleMin::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableScalingXMin( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnableScalingYMin( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnableScalingZMin( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableScaleMax( bool enabledX, bool enabledY, bool enabledY )
deClassARLimit::nfSetEnableScaleMax::nfSetEnableScaleMax( const sInitData &init ) : dsFunction( init.clsARLimit,
"setEnableScaleMax", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabledX
	p_AddParameter( init.clsBool ); // enabledY
	p_AddParameter( init.clsBool ); // enabledZ
}
void deClassARLimit::nfSetEnableScaleMax::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableScalingXMax( rt->GetValue( 0 )->GetBool() );
	nd.rule->SetEnableScalingYMax( rt->GetValue( 1 )->GetBool() );
	nd.rule->SetEnableScalingZMax( rt->GetValue( 2 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setMinimumPosition( Vector translation )
deClassARLimit::nfSetMinimumPosition::nfSetMinimumPosition( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMinimumPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // translation
}
void deClassARLimit::nfSetMinimumPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumPosition( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumPosition( Vector translation )
deClassARLimit::nfSetMaximumPosition::nfSetMaximumPosition( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMaximumPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // translation
}
void deClassARLimit::nfSetMaximumPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumPosition( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumRotation( Vector rotation )
deClassARLimit::nfSetMinimumRotation::nfSetMinimumRotation( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMinimumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // rotation
}
void deClassARLimit::nfSetMinimumRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumRotation( vector * DEG2RAD );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumRotation( Vector rotation )
deClassARLimit::nfSetMaximumRotation::nfSetMaximumRotation( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMaximumRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // rotation
}
void deClassARLimit::nfSetMaximumRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumRotation( vector * DEG2RAD );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMinimumScaling( Vector scaling )
deClassARLimit::nfSetMinimumScaling::nfSetMinimumScaling( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMinimumScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // scaling
}
void deClassARLimit::nfSetMinimumScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMinimumScaling( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setMaximumScaling( Vector scaling )
deClassARLimit::nfSetMaximumScaling::nfSetMaximumScaling( const sInitData &init ) : dsFunction( init.clsARLimit,
"setMaximumScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // scaling
}
void deClassARLimit::nfSetMaximumScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit &clsARLimit = *( ( deClassARLimit* )GetOwnerClass() );
	const deClassVector &clsVec = *clsARLimit.GetDS().GetClassVector();
	
	const decVector &vector = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	nd.rule->SetMaximumScaling( vector );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setCoordinateFrame( ARLimitCFrame coordinateFrame )
deClassARLimit::nfSetCoordinateFrame::nfSetCoordinateFrame( const sInitData &init ) : dsFunction( init.clsARLimit,
"setCoordinateFrame", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARLimitCFrame ); // coordinateFrame
}
void deClassARLimit::nfSetCoordinateFrame::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetCoordinateFrame( ( deAnimatorRuleLimit::eCoordinateFrames )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setTargetBone( String boneName )
deClassARLimit::nfSetTargetBone::nfSetTargetBone( const sInitData &init ) : dsFunction( init.clsARLimit,
"setTargetBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassARLimit::nfSetTargetBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetTargetBone( rt->GetValue( 0 )->GetString() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}




// public func void targetAddLink( ARLimitTarget target, int link )
deClassARLimit::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARLimit,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARLimitTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARLimit::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	const deClassARLimit::eTargets target = ( deClassARLimit::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARLimit::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARLimitTarget target )
deClassARLimit::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARLimit,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARLimitTarget ); // target
}
void deClassARLimit::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself ) );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const deClassARLimit::eTargets target = ( deClassARLimit::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARLimit::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARLimit
/////////////////////////////

// Constructor
////////////////

deClassARLimit::deClassARLimit( deScriptingDragonScript &ds ) :
dsClass( "ARLimit", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARLimitNatDat ) );
}

deClassARLimit::~deClassARLimit(){
}



// Management
///////////////

void deClassARLimit::CreateClassMembers( dsEngine *engine ){
	pClsARLimitTarget = engine->GetClass( "Dragengine.Scenery.ARLimitTarget" );
	pClsARLimitCFrame = engine->GetClass( "Dragengine.Scenery.ARLimitCFrame" );
	
	sInitData init;
	init.clsARLimit = this;
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
	init.clsARLimitTarget = pClsARLimitTarget;
	init.clsARLimitCFrame = pClsARLimitCFrame;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetEnablePosMin( init ) );
	AddFunction( new nfSetEnablePosMax( init ) );
	AddFunction( new nfSetEnableRotMin( init ) );
	AddFunction( new nfSetEnableRotMax( init ) );
	AddFunction( new nfSetEnableScaleMin( init ) );
	AddFunction( new nfSetEnableScaleMax( init ) );
	
	AddFunction( new nfSetMinimumPosition( init ) );
	AddFunction( new nfSetMaximumPosition( init ) );
	AddFunction( new nfSetMinimumRotation( init ) );
	AddFunction( new nfSetMaximumRotation( init ) );
	AddFunction( new nfSetMinimumScaling( init ) );
	AddFunction( new nfSetMaximumScaling( init ) );
	AddFunction( new nfSetCoordinateFrame( init ) );
	AddFunction( new nfSetTargetBone( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleLimit *deClassARLimit::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARLimitNatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARLimit::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassARLimit::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleLimit *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARLimitNatDat &nd = *( ( sARLimitNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
