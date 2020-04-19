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

#include "deClassColliderConstraint.h"
#include "deClassCollider.h"
#include "../math/deClassVector.h"
#include "../math/deClassQuaternion.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/deEngine.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sCConNatDat{
	deColliderConstraint *constraint;
};



// Native Functions
/////////////////////

// public func new()
deClassColliderConstraint::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCCon,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderConstraint::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCConNatDat *nd = ( sCConNatDat* )p_GetNativeData( myself );
	
	// clear ( important )
	nd->constraint = NULL;
	
	// create a constraint
	nd->constraint = new deColliderConstraint();
	if( ! nd->constraint ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassColliderConstraint::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCCon,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderConstraint::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCConNatDat *nd = ( sCConNatDat* )p_GetNativeData( myself );
	
	if( nd->constraint ){
		delete nd->constraint;
		nd->constraint = NULL;
	}
}



// Management
///////////////

// public func Collider getTargetCollider()
deClassColliderConstraint::nfGetTargetCollider::nfGetTargetCollider( const sInitData &init ) : dsFunction( init.clsCCon,
"getTargetCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol ){
}
void deClassColliderConstraint::nfGetTargetCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const deScriptingDragonScript &ds = *( ( deClassColliderConstraint* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassCollider()->PushCollider( rt, constraint.GetTargetCollider() );
}

// public func void setTargetCollider( Collider collider )
deClassColliderConstraint::nfSetTargetCollider::nfSetTargetCollider( const sInitData &init ) : dsFunction( init.clsCCon,
"setTargetCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassColliderConstraint::nfSetTargetCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = *( ( deClassColliderConstraint* )GetOwnerClass() )->GetScriptModule();
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	
	constraint.SetTargetCollider( collider );
}

// public func String getTargetBone()
deClassColliderConstraint::nfGetTargetBone::nfGetTargetBone( const sInitData &init ) : dsFunction( init.clsCCon,
"getTargetBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassColliderConstraint::nfGetTargetBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushString( constraint.GetTargetBone() );
}

// public func void setTargetBone( String bone )
deClassColliderConstraint::nfSetTargetBone::nfSetTargetBone( const sInitData &init ) : dsFunction( init.clsCCon,
"setTargetBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // bone
}
void deClassColliderConstraint::nfSetTargetBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const char *bone = rt->GetValue( 0 )->GetString();
	
	constraint.SetTargetBone( bone );
}




// public func Vector getPosition1()
deClassColliderConstraint::nfGetPosition1::nfGetPosition1( const sInitData &init ) : dsFunction( init.clsCCon,
"getPosition1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetPosition1::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, constraint->GetPosition1() );
}

// public func void setPosition1( Vector position )
deClassColliderConstraint::nfSetPosition1::nfSetPosition1( const sInitData &init ) : dsFunction( init.clsCCon,
"setPosition1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // position
}
void deClassColliderConstraint::nfSetPosition1::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	constraint->SetPosition1( clsCCon->GetClassVector()->GetVector( obj ) );
}

// public func Quaternion getOrientation1()
deClassColliderConstraint::nfGetOrientation1::nfGetOrientation1( const sInitData &init ) : dsFunction( init.clsCCon,
"getOrientation1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassColliderConstraint::nfGetOrientation1::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassQuaternion()->PushQuaternion( rt, constraint->GetOrientation1() );
}

// public func void setOrientation1( Quaternion orientation )
deClassColliderConstraint::nfSetOrientation1::nfSetOrientation1( const sInitData &init ) : dsFunction( init.clsCCon,
"setOrientation1", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassColliderConstraint::nfSetOrientation1::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	constraint->SetOrientation1( clsCCon->GetClassQuaternion()->GetQuaternion( obj ) );
}

// public func Vector getPosition2()
deClassColliderConstraint::nfGetPosition2::nfGetPosition2( const sInitData &init ) : dsFunction( init.clsCCon,
"getPosition2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetPosition2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, constraint->GetPosition2() );
}

// public func void setPosition2( Vector position )
deClassColliderConstraint::nfSetPosition2::nfSetPosition2( const sInitData &init ) : dsFunction( init.clsCCon,
"setPosition2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // position
}
void deClassColliderConstraint::nfSetPosition2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	constraint->SetPosition2( clsCCon->GetClassVector()->GetVector( obj ) );
}

// public func Quaternion getOrientation2()
deClassColliderConstraint::nfGetOrientation2::nfGetOrientation2( const sInitData &init ) : dsFunction( init.clsCCon,
"getOrientation2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassColliderConstraint::nfGetOrientation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassQuaternion()->PushQuaternion( rt, constraint->GetOrientation2() );
}

// public func void setOrientation2( Quaternion orientation )
deClassColliderConstraint::nfSetOrientation2::nfSetOrientation2( const sInitData &init ) : dsFunction( init.clsCCon,
"setOrientation2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassColliderConstraint::nfSetOrientation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	constraint->SetOrientation2( clsCCon->GetClassQuaternion()->GetQuaternion( obj ) );
}



// public func float getLowerLimitFor( ColliderConstraintDof dof )
deClassColliderConstraint::nfGetLowerLimitFor::nfGetLowerLimitFor( const sInitData &init ) : dsFunction( init.clsCCon,
"getLowerLimitFor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // dof
}
void deClassColliderConstraint::nfGetLowerLimitFor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof = ( deColliderConstraint::eDegreesOfFreedom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( dof ){
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		rt->PushFloat( constraint->GetDof( dof ).GetLowerLimit() / DEG2RAD );
		break;
		
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		rt->PushFloat( constraint->GetDof( dof ).GetLowerLimit() );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
}

// public func float getUpperLimitFor( ColliderConstraintDof dof )
deClassColliderConstraint::nfGetUpperLimitFor::nfGetUpperLimitFor( const sInitData &init ) : dsFunction( init.clsCCon,
"getUpperLimitFor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // dof
}
void deClassColliderConstraint::nfGetUpperLimitFor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof = ( deColliderConstraint::eDegreesOfFreedom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( dof ){
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		rt->PushFloat( constraint->GetDof( dof ).GetUpperLimit() / DEG2RAD );
		break;
		
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		rt->PushFloat( constraint->GetDof( dof ).GetUpperLimit() );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
}

// public func void setLimitsFor( ColliderConstraintDof dof, float lowerLimit, float upperLimit )
deClassColliderConstraint::nfSetLimitsFor::nfSetLimitsFor( const sInitData &init ) : dsFunction( init.clsCCon,
"setLimitsFor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // dof
	p_AddParameter( init.clsFlt ); // lowerLimit
	p_AddParameter( init.clsFlt ); // upperLimit
}
void deClassColliderConstraint::nfSetLimitsFor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof = ( deColliderConstraint::eDegreesOfFreedom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	float lowerLimit = rt->GetValue( 1 )->GetFloat();
	float upperLimit = rt->GetValue( 2 )->GetFloat();
	
	switch( dof ){
	case deColliderConstraint::edofAngularX:
	case deColliderConstraint::edofAngularY:
	case deColliderConstraint::edofAngularZ:
		constraint->GetDof( dof ).SetLowerLimit( lowerLimit * DEG2RAD );
		constraint->GetDof( dof ).SetUpperLimit( upperLimit * DEG2RAD );
		break;
		
	case deColliderConstraint::edofLinearX:
	case deColliderConstraint::edofLinearY:
	case deColliderConstraint::edofLinearZ:
		constraint->GetDof( dof ).SetLowerLimit( lowerLimit );
		constraint->GetDof( dof ).SetUpperLimit( upperLimit );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
}

// public func void setLockedFor( ColliderConstraintDof dof )
deClassColliderConstraint::nfSetLockedFor::nfSetLockedFor( const sInitData &init ) : dsFunction( init.clsCCon,
"setLockedFor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // dof
}
void deClassColliderConstraint::nfSetLockedFor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof =
		( deColliderConstraint::eDegreesOfFreedom )rt->GetValue( 0 )->GetInt();
	
	constraint->GetDof( dof ).SetLocked( 0.0f );
}

// public func void setFreeFor( ColliderConstraintDof dof )
deClassColliderConstraint::nfSetFreeFor::nfSetFreeFor( const sInitData &init ) : dsFunction( init.clsCCon,
"setFreeFor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // dof
}
void deClassColliderConstraint::nfSetFreeFor::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof =
		( deColliderConstraint::eDegreesOfFreedom )rt->GetValue( 0 )->GetInt();
	
	constraint->GetDof( dof ).SetFree();
}



// public func Vector getLinearLowerLimits()
deClassColliderConstraint::nfGetLinearLowerLimits::nfGetLinearLowerLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"getLinearLowerLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetLinearLowerLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, decVector(
		constraint->GetDofLinearX().GetLowerLimit(),
		constraint->GetDofLinearY().GetLowerLimit(),
		constraint->GetDofLinearZ().GetLowerLimit() ) );
}

// public func Vector getLinearUpperLimits()
deClassColliderConstraint::nfGetLinearUpperLimits::nfGetLinearUpperLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"getLinearUpperLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetLinearUpperLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, decVector(
		constraint->GetDofLinearX().GetUpperLimit(),
		constraint->GetDofLinearY().GetUpperLimit(),
		constraint->GetDofLinearZ().GetUpperLimit() ) );
}

// public func void setLinearLimits( Vector lowerLimits, Vector upperLimits )
deClassColliderConstraint::nfSetLinearLimits::nfSetLinearLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"setLinearLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // lowerLimits
	p_AddParameter( init.clsVec ); // upperLimits
}
void deClassColliderConstraint::nfSetLinearLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *objLower = rt->GetValue( 0 )->GetRealObject();
	dsRealObject *objUpper = rt->GetValue( 1 )->GetRealObject();
	if( ! objLower || ! objUpper ) DSTHROW( dueNullPointer );
	
	const decVector &lowerLimits = clsCCon->GetClassVector()->GetVector( objLower );
	const decVector &upperLimits = clsCCon->GetClassVector()->GetVector( objUpper );
	
	constraint->GetDofLinearX().SetLowerLimit( lowerLimits.x );
	constraint->GetDofLinearY().SetLowerLimit( lowerLimits.y );
	constraint->GetDofLinearZ().SetLowerLimit( lowerLimits.z );
	
	constraint->GetDofLinearX().SetUpperLimit( upperLimits.x );
	constraint->GetDofLinearY().SetUpperLimit( upperLimits.y );
	constraint->GetDofLinearZ().SetUpperLimit( upperLimits.z );
}

// public func Vector getAngularLowerLimits()
deClassColliderConstraint::nfGetAngularLowerLimits::nfGetAngularLowerLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"getAngularLowerLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetAngularLowerLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, decVector(
		constraint->GetDofAngularX().GetLowerLimit() / DEG2RAD,
		constraint->GetDofAngularY().GetLowerLimit() / DEG2RAD,
		constraint->GetDofAngularZ().GetLowerLimit() / DEG2RAD ) );
}

// public func Vector getAngularUpperLimits()
deClassColliderConstraint::nfGetAngularUpperLimits::nfGetAngularUpperLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"getAngularUpperLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassColliderConstraint::nfGetAngularUpperLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	clsCCon->GetClassVector()->PushVector( rt, decVector(
		constraint->GetDofAngularX().GetUpperLimit() / DEG2RAD,
		constraint->GetDofAngularY().GetUpperLimit() / DEG2RAD,
		constraint->GetDofAngularZ().GetUpperLimit() / DEG2RAD ) );
}

// public func void setAngularLimits( Vector lowerLimits, Vector upperLimits )
deClassColliderConstraint::nfSetAngularLimits::nfSetAngularLimits( const sInitData &init ) : dsFunction( init.clsCCon,
"setAngularLimits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // lowerLimits
	p_AddParameter( init.clsVec ); // upperLimits
}
void deClassColliderConstraint::nfSetAngularLimits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	
	dsRealObject *objLower = rt->GetValue( 0 )->GetRealObject();
	dsRealObject *objUpper = rt->GetValue( 1 )->GetRealObject();
	if( ! objLower || ! objUpper ) DSTHROW( dueNullPointer );
	
	const decVector &lowerLimits = clsCCon->GetClassVector()->GetVector( objLower );
	const decVector &upperLimits = clsCCon->GetClassVector()->GetVector( objUpper );
	
	constraint->GetDofAngularX().SetLowerLimit( lowerLimits.x * DEG2RAD );
	constraint->GetDofAngularY().SetLowerLimit( lowerLimits.y * DEG2RAD );
	constraint->GetDofAngularZ().SetLowerLimit( lowerLimits.z * DEG2RAD );
	
	constraint->GetDofAngularX().SetUpperLimit( upperLimits.x * DEG2RAD );
	constraint->GetDofAngularY().SetUpperLimit( upperLimits.y * DEG2RAD );
	constraint->GetDofAngularZ().SetUpperLimit( upperLimits.z * DEG2RAD );
}



// public func float getLinearDamping()
deClassColliderConstraint::nfGetLinearDamping::nfGetLinearDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"getLinearDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColliderConstraint::nfGetLinearDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushFloat( constraint.GetLinearDamping() );
}

// public func void setLinearDamping( float damping )
deClassColliderConstraint::nfSetLinearDamping::nfSetLinearDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"setLinearDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // damping
}
void deClassColliderConstraint::nfSetLinearDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const float damping = rt->GetValue( 0 )->GetFloat();
	
	constraint.SetLinearDamping( damping );
}

// public func float getAngularDamping()
deClassColliderConstraint::nfGetAngularDamping::nfGetAngularDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"getAngularDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColliderConstraint::nfGetAngularDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushFloat( constraint.GetAngularDamping() );
}

// public func void setAngularDamping( float damping )
deClassColliderConstraint::nfSetAngularDamping::nfSetAngularDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"setAngularDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // damping
}
void deClassColliderConstraint::nfSetAngularDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const float damping = rt->GetValue( 0 )->GetFloat();
	
	constraint.SetAngularDamping( damping );
}

// public func float getSpringDamping()
deClassColliderConstraint::nfGetSpringDamping::nfGetSpringDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"getSpringDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColliderConstraint::nfGetSpringDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushFloat( constraint.GetSpringDamping() );
}

// public func void setSpringDamping( float damping )
deClassColliderConstraint::nfSetSpringDamping::nfSetSpringDamping( const sInitData &init ) : dsFunction( init.clsCCon,
"setSpringDamping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // damping
}
void deClassColliderConstraint::nfSetSpringDamping::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const float damping = rt->GetValue( 0 )->GetFloat();
	
	constraint.SetSpringDamping( damping );
}



// public func void lockAll()
deClassColliderConstraint::nfLockAll::nfLockAll( const sInitData &init ) : dsFunction( init.clsCCon,
"lockAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderConstraint::nfLockAll::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	
	constraint->LockAll();
}

// public func void setToBallJoint()
deClassColliderConstraint::nfSetToBallJoint::nfSetToBallJoint( const sInitData &init ) : dsFunction( init.clsCCon,
"setToBallJoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderConstraint::nfSetToBallJoint::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	
	constraint->SetToBallJoint();
}

// public func void setToHingeJoint( ColliderConstraintDof dof, float lowerLimit, float upperLimit )
deClassColliderConstraint::nfSetToHingeJoint::nfSetToHingeJoint( const sInitData &init ) : dsFunction( init.clsCCon,
"setToHingeJoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // dof
	p_AddParameter( init.clsFlt ); // lowerLimit
	p_AddParameter( init.clsFlt ); // upperLimit
}
void deClassColliderConstraint::nfSetToHingeJoint::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof = ( deColliderConstraint::eDegreesOfFreedom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	float lowerLimit = rt->GetValue( 1 )->GetFloat();
	float upperLimit = rt->GetValue( 2 )->GetFloat();
	
	constraint->SetToHingeJoint( dof, lowerLimit * DEG2RAD, upperLimit * DEG2RAD );
}

// public func void setToPistonJoint( ColliderConstraintDof dof, float lowerLimit, float upperLimit )
deClassColliderConstraint::nfSetToPistonJoint::nfSetToPistonJoint( const sInitData &init ) : dsFunction( init.clsCCon,
"setToPistonJoint", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // dof
	p_AddParameter( init.clsFlt ); // lowerLimit
	p_AddParameter( init.clsFlt ); // upperLimit
}
void deClassColliderConstraint::nfSetToPistonJoint::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	const deColliderConstraint::eDegreesOfFreedom dof = ( deColliderConstraint::eDegreesOfFreedom )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	float lowerLimit = rt->GetValue( 1 )->GetFloat();
	float upperLimit = rt->GetValue( 2 )->GetFloat();
	
	constraint->SetToPistonJoint( dof, lowerLimit, upperLimit );
}



// public func bool getIsRope()
deClassColliderConstraint::nfGetIsRope::nfGetIsRope( const sInitData &init ) : dsFunction( init.clsCCon,
"getIsRope", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassColliderConstraint::nfGetIsRope::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushBool( constraint.GetIsRope() );
}

// public func void setIsRope( bool isRope )
deClassColliderConstraint::nfSetIsRope::nfSetIsRope( const sInitData &init ) : dsFunction( init.clsCCon,
"setIsRope", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // isRope
}
void deClassColliderConstraint::nfSetIsRope::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const bool isRope = rt->GetValue( 0 )->GetBool();
	
	constraint.SetIsRope( isRope );
}



// public func float getBreakingThreshold()
deClassColliderConstraint::nfGetBreakingThreshold::nfGetBreakingThreshold( const sInitData &init ) : dsFunction( init.clsCCon,
"getBreakingThreshold", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColliderConstraint::nfGetBreakingThreshold::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	rt->PushFloat( constraint.GetBreakingThreshold() );
}

// public func void setBreakingThreshold( float impuls )
deClassColliderConstraint::nfSetBreakingThreshold::nfSetBreakingThreshold( const sInitData &init ) : dsFunction( init.clsCCon,
"setBreakingThreshold", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // impuls
}
void deClassColliderConstraint::nfSetBreakingThreshold::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint &constraint = *( ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint );
	const float impuls = rt->GetValue( 0 )->GetFloat();
	
	constraint.SetBreakingThreshold( impuls );
}



// public func int getBone()
deClassColliderConstraint::nfGetBone::nfGetBone( const sInitData &init ) : dsFunction( init.clsCCon,
"getBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassColliderConstraint::nfGetBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	
	rt->PushInt( constraint->GetBone() );
}

// public func void setBone( int bone )
deClassColliderConstraint::nfSetBone::nfSetBone( const sInitData &init ) : dsFunction( init.clsCCon,
"setBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderConstraint::nfSetBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	int bone = rt->GetValue( 0 )->GetInt();
	
	constraint->SetBone( bone );
}



// Misc
/////////

// public func int hashCode()
deClassColliderConstraint::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCCon, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassColliderConstraint::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )constraint );
}

// public func bool equals( Object obj )
deClassColliderConstraint::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCCon, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassColliderConstraint::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderConstraint *constraint = ( ( sCConNatDat* )p_GetNativeData( myself ) )->constraint;
	deClassColliderConstraint *clsCCon = ( deClassColliderConstraint* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCCon ) ){
		rt->PushBool( false );
		
	}else{
		deColliderConstraint *otherConstraint = ( ( sCConNatDat* )p_GetNativeData( obj ) )->constraint;
		rt->PushBool( constraint == otherConstraint );
	}
}



// Class deClassColliderConstraint
////////////////////////////////////

// Constructor and Destructor
///////////////////////////////

deClassColliderConstraint::deClassColliderConstraint( deEngine *gameEngine, deScriptingDragonScript *scrDS ) :
dsClass( "ColliderConstraint", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scrDS ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrDS = scrDS;
	
	// store informations into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sCConNatDat ) );
}

deClassColliderConstraint::~deClassColliderConstraint(){
}



// Management
///////////////

void deClassColliderConstraint::CreateClassMembers( dsEngine *engine ){
	pClsVec = pScrDS->GetClassVector();
	pClsQuat = pScrDS->GetClassQuaternion();
	pClsColliderConstraintDof = engine->GetClass( "Dragengine.Scenery.ColliderConstraintDof" );
	
	sInitData init;
	init.clsCCon = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsQuat = pClsQuat;
	init.clsCol = pScrDS->GetClassCollider();
	init.clsColliderConstraintDof = pClsColliderConstraintDof;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetTargetCollider( init ) );
	AddFunction( new nfSetTargetCollider( init ) );
	AddFunction( new nfGetTargetBone( init ) );
	AddFunction( new nfSetTargetBone( init ) );
	
	AddFunction( new nfGetPosition1( init ) );
	AddFunction( new nfSetPosition1( init ) );
	AddFunction( new nfGetOrientation1( init ) );
	AddFunction( new nfSetOrientation1( init ) );
	AddFunction( new nfGetPosition2( init ) );
	AddFunction( new nfSetPosition2( init ) );
	AddFunction( new nfGetOrientation2( init ) );
	AddFunction( new nfSetOrientation2( init ) );
	
	AddFunction( new nfGetLowerLimitFor( init ) );
	AddFunction( new nfGetUpperLimitFor( init ) );
	AddFunction( new nfSetLimitsFor( init ) );
	AddFunction( new nfSetLockedFor( init ) );
	AddFunction( new nfSetFreeFor( init ) );
	
	AddFunction( new nfGetLinearLowerLimits( init ) );
	AddFunction( new nfGetLinearUpperLimits( init ) );
	AddFunction( new nfSetLinearLimits( init ) );
	AddFunction( new nfGetAngularLowerLimits( init ) );
	AddFunction( new nfGetAngularUpperLimits( init ) );
	AddFunction( new nfSetAngularLimits( init ) );
	
	AddFunction( new nfGetLinearDamping( init ) );
	AddFunction( new nfSetLinearDamping( init ) );
	AddFunction( new nfGetAngularDamping( init ) );
	AddFunction( new nfSetAngularDamping( init ) );
	AddFunction( new nfGetSpringDamping( init ) );
	AddFunction( new nfSetSpringDamping( init ) );
	
	AddFunction( new nfLockAll( init ) );
	AddFunction( new nfSetToBallJoint( init ) );
	AddFunction( new nfSetToHingeJoint( init ) );
	AddFunction( new nfSetToPistonJoint( init ) );
	
	AddFunction( new nfGetIsRope( init ) );
	AddFunction( new nfSetIsRope( init ) );
	
	AddFunction( new nfGetBreakingThreshold( init ) );
	AddFunction( new nfSetBreakingThreshold( init ) );
	
	AddFunction( new nfGetBone( init ) );
	AddFunction( new nfSetBone( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deColliderConstraint *deClassColliderConstraint::GetConstraint( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCConNatDat* )p_GetNativeData( myself->GetBuffer() ) )->constraint;
}

void deClassColliderConstraint::PushConstraint( dsRunTime *rt, deColliderConstraint *constraint ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! constraint ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	sCConNatDat &nd = *( ( sCConNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.constraint = NULL;
	
	try{
		nd.constraint = new deColliderConstraint( *constraint );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
