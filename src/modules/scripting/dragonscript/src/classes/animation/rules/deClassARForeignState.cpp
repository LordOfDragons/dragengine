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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassARForeignState.h"
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
#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sARFStaNatDat{
	deAnimator *animator;
	deAnimatorRuleForeignState *rule;
};



// Native Functions
/////////////////////

// public func new()
deClassARForeignState::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsARFSta,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARForeignState::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.animator = NULL;
	nd.rule = NULL;
	
	// super call
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create animator rule
	nd.rule = new deAnimatorRuleForeignState;
	baseClass->AssignRule( myself->GetRealObject(), nd.rule );
}

// public func destructor()
deClassARForeignState::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsARFSta,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassARForeignState::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	if( nd.animator ){
		nd.animator->FreeReference();
		nd.animator = NULL;
	}
	
	if( nd.rule ){
		nd.rule->FreeReference();
		nd.rule = NULL;
	}
}

// public func void setModifyX( bool modify )
deClassARForeignState::nfSetModifyX::nfSetModifyX( const sInitData &init ) : dsFunction( init.clsARFSta,
"setModifyX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // modify
}
void deClassARForeignState::nfSetModifyX::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetModifyX( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setModifyY( bool modify )
deClassARForeignState::nfSetModifyY::nfSetModifyY( const sInitData &init ) : dsFunction( init.clsARFSta,
"setModifyY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // modify
}
void deClassARForeignState::nfSetModifyY::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetModifyY( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setModifyZ( bool modify )
deClassARForeignState::nfSetModifyZ::nfSetModifyZ( const sInitData &init ) : dsFunction( init.clsARFSta,
"setModifyZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // modify
}
void deClassARForeignState::nfSetModifyZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetModifyZ( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}




// public func void setEnablePosition( bool enabled )
deClassARForeignState::nfSetEnablePosition::nfSetEnablePosition( const sInitData &init ) : dsFunction( init.clsARFSta,
"setEnablePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARForeignState::nfSetEnablePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnablePosition( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableOrientation( bool enabled )
deClassARForeignState::nfSetEnableOrientation::nfSetEnableOrientation( const sInitData &init ) : dsFunction( init.clsARFSta,
"setEnableOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARForeignState::nfSetEnableOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableOrientation( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableSize( bool enabled )
deClassARForeignState::nfSetEnableSize::nfSetEnableSize( const sInitData &init ) : dsFunction( init.clsARFSta,
"setEnableSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARForeignState::nfSetEnableSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableSize( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setEnableVertexPositionSet( bool enabled )
deClassARForeignState::nfSetEnableVertexPositionSet::nfSetEnableVertexPositionSet( const sInitData &init ) :
dsFunction( init.clsARFSta, "setEnableVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassARForeignState::nfSetEnableVertexPositionSet::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetEnableVertexPositionSet( rt->GetValue( 0 )->GetBool() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void targetAddLink( ARForeignStateTarget target, int link )
deClassARForeignState::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsARFSta,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARForeignStateTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassARForeignState::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	const deClassARForeignState::eTargets target = ( deClassARForeignState::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassARForeignState::etBlendFactor:
		nd.rule->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassARForeignState::etPosition:
		nd.rule->GetTargetPosition().AddLink( link );
		break;
		
	case deClassARForeignState::etOrientation:
		nd.rule->GetTargetOrientation().AddLink( link );
		break;
		
	case deClassARForeignState::etSize:
		nd.rule->GetTargetSize().AddLink( link );
		break;
		
	case deClassARForeignState::etVertexPositionSet:
		nd.rule->GetTargetVertexPositionSet().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void targetRemoveAllLinks( ARForeignStateTarget target )
deClassARForeignState::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsARFSta,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARForeignStateTarget ); // target
}
void deClassARForeignState::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	const deClassARForeignState::eTargets target = ( deClassARForeignState::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassARForeignState::etBlendFactor:
		nd.rule->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassARForeignState::etPosition:
		nd.rule->GetTargetPosition().RemoveAllLinks();
		break;
		
	case deClassARForeignState::etOrientation:
		nd.rule->GetTargetOrientation().RemoveAllLinks();
		break;
		
	case deClassARForeignState::etSize:
		nd.rule->GetTargetSize().RemoveAllLinks();
		break;
		
	case deClassARForeignState::etVertexPositionSet:
		nd.rule->GetTargetVertexPositionSet().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// public func void setScalePosition( float scalePosition )
deClassARForeignState::nfSetScalePosition::nfSetScalePosition( const sInitData &init ) : dsFunction( init.clsARFSta,
"setScalePosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // scalePosition
}
void deClassARForeignState::nfSetScalePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetScalePosition( rt->GetValue( 0 )->GetFloat() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setScaleOrientation( float scaleOrientation )
deClassARForeignState::nfSetScaleOrientation::nfSetScaleOrientation( const sInitData &init ) : dsFunction( init.clsARFSta,
"setScaleOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // scaleOrientation
}
void deClassARForeignState::nfSetScaleOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetScaleOrientation( rt->GetValue( 0 )->GetFloat() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setScaleSize( float scaleSize )
deClassARForeignState::nfSetScaleSize::nfSetScaleSize( const sInitData &init ) : dsFunction( init.clsARFSta,
"setScaleSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // scaleSize
}
void deClassARForeignState::nfSetScaleSize::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetScaleSize( rt->GetValue( 0 )->GetFloat() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setScaleVertexPositionSet( float scaleVertexPositionSet )
deClassARForeignState::nfSetScaleVertexPositionSet::nfSetScaleVertexPositionSet( const sInitData &init ) :
dsFunction( init.clsARFSta, "setScaleVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // scaleVertexPositionSet
}
void deClassARForeignState::nfSetScaleVertexPositionSet::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetScaleVertexPositionSet( rt->GetValue( 0 )->GetFloat() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setForeignBone( String boneName )
deClassARForeignState::nfSetForeignBone::nfSetForeignBone( const sInitData &init ) : dsFunction( init.clsARFSta,
"setForeignBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // boneName
}
void deClassARForeignState::nfSetForeignBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	const char * const boneName = rt->GetValue( 0 )->GetString();
	
	nd.rule->SetForeignBone( boneName );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setForeignVertexPositionSet(String name)
deClassARForeignState::nfSetForeignVertexPositionSet::nfSetForeignVertexPositionSet( const sInitData &init ) :
dsFunction( init.clsARFSta, "setForeignVertexPositionSet", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // name
}
void deClassARForeignState::nfSetForeignVertexPositionSet::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	
	nd.rule->SetForeignVertexPositionSet( rt->GetValue( 0 )->GetString() );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setSourceCoordinateFrame( ARForeignStateCFrame coordinateFrame )
deClassARForeignState::nfSetSourceCoordinateFrame::nfSetSourceCoordinateFrame( const sInitData &init ) : dsFunction( init.clsARFSta,
"setSourceCoordinateFrame", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARForeignStateCFrame ); // coordinateFrame
}
void deClassARForeignState::nfSetSourceCoordinateFrame::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	nd.rule->SetSourceCoordinateFrame( ( deAnimatorRuleForeignState::eCoordinateFrames )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}

// public func void setDestinationCoordinateFrame( ARForeignStateCFrame coordinateFrame )
deClassARForeignState::nfSetDestinationCoordinateFrame::nfSetDestinationCoordinateFrame( const sInitData &init ) : dsFunction( init.clsARFSta,
"setDestinationCoordinateFrame", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsARForeignStateCFrame ); // coordinateFrame
}
void deClassARForeignState::nfSetDestinationCoordinateFrame::RunFunction( dsRunTime *rt, dsValue *myself ){
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself ) );
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	nd.rule->SetDestCoordinateFrame( ( deAnimatorRuleForeignState::eCoordinateFrames )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if( nd.animator ){
		nd.animator->NotifyRulesChanged();
	}
}



// Class deClassARForeignState
////////////////////////////////

// Constructor
////////////////

deClassARForeignState::deClassARForeignState( deScriptingDragonScript &ds ) :
dsClass( "ARForeignState", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "AnimatorRule" );
	
	p_SetNativeDataSize( sizeof( sARFStaNatDat ) );
}

deClassARForeignState::~deClassARForeignState(){
}



// Management
///////////////

void deClassARForeignState::CreateClassMembers( dsEngine *engine ){
	pClsARForeignStateTarget = engine->GetClass( "Dragengine.Scenery.ARForeignStateTarget" );
	pClsARForeignStateCFrame = engine->GetClass( "Dragengine.Scenery.ARForeignStateCFrame" );
	
	sInitData init;
	init.clsARFSta = this;
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
	init.clsARForeignStateTarget = pClsARForeignStateTarget;
	init.clsARForeignStateCFrame = pClsARForeignStateCFrame;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfSetScalePosition( init ) );
	AddFunction( new nfSetScaleOrientation( init ) );
	AddFunction( new nfSetScaleSize( init ) );
	AddFunction( new nfSetScaleVertexPositionSet( init ) );
	AddFunction( new nfSetForeignBone( init ) );
	AddFunction( new nfSetForeignVertexPositionSet( init ) );
	AddFunction( new nfSetSourceCoordinateFrame( init ) );
	AddFunction( new nfSetDestinationCoordinateFrame( init ) );
	AddFunction( new nfSetModifyX( init ) );
	AddFunction( new nfSetModifyY( init ) );
	AddFunction( new nfSetModifyZ( init ) );
	
	AddFunction( new nfSetEnablePosition( init ) );
	AddFunction( new nfSetEnableOrientation( init ) );
	AddFunction( new nfSetEnableSize( init ) );
	AddFunction( new nfSetEnableVertexPositionSet( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAnimatorRuleForeignState *deClassARForeignState::GetRule( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sARFStaNatDat* )p_GetNativeData( myself->GetBuffer() ) )->rule;
}

void deClassARForeignState::AssignAnimator( dsRealObject *myself, deAnimator *animator ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassAnimatorRule()->AssignAnimator( myself, animator );
	
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
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

void deClassARForeignState::PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleForeignState *rule ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! rule ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassAnimatorRule * const baseClass = ( deClassAnimatorRule* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sARFStaNatDat &nd = *( ( sARFStaNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
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
