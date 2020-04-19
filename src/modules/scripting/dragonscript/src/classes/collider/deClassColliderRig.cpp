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

#include "deClassCollider.h"
#include "deClassColliderRig.h"
#include "../math/deClassDMatrix.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../physics/deClassCollisionInfo.h"
#include "../world/deClassRig.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../peers/dedsCollider.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderRig.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>

#include <libdscript/exceptions.h>



// native structure
struct sColRigNatDat{
	deColliderRig *collider;
};



// native functions
/////////////////////

// public func new()
deClassColliderRig::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsColRig,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderRig::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sColRigNatDat &nd = *( ( sColRigNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	deColliderManager &colMgr = *ds.GetGameEngine()->GetColliderManager();
	
	// clear ( important )
	nd.collider = NULL;
	
	// super call
	deClassCollider * const baseClass = ( deClassCollider* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create collider
	nd.collider = colMgr.CreateColliderRig();
	baseClass->AssignCollider( myself->GetRealObject(), nd.collider );
}

// public func destructor()
deClassColliderRig::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsColRig,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColliderRig::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sColRigNatDat &nd = *( ( sColRigNatDat* )p_GetNativeData( myself ) );
	
	if( nd.collider ){
		nd.collider->FreeReference();
		nd.collider = NULL;
	}
}



// management
///////////////

// public func void attachRig( ColliderRig collider )
deClassColliderRig::nfAttachRig::nfAttachRig( const sInitData &init ) : dsFunction( init.clsColRig,
"attachRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsColRig ); // collider
}
void deClassColliderRig::nfAttachRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	deClassColliderRig &clsColRig = *( ( deClassColliderRig* )GetOwnerClass() );
	
	deColliderRig * const attachCollider = clsColRig.GetCollider( rt->GetValue( 0 )->GetRealObject() );
	if( ! attachCollider ){
		DSTHROW( dueNullPointer );
	}
	
	clsColRig.GetDS().GetClassCollider()->AttachRig( collider, attachCollider );
}

// public func void attachBone( Object resource, String targetBone )
deClassColliderRig::nfAttachBone::nfAttachBone( const sInitData &init ) : dsFunction( init.clsColRig,
"attachBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // resource
	p_AddParameter( init.clsStr ); // targetBone
}
void deClassColliderRig::nfAttachBone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	deResource * const resource = ds.GetClassCollider()->GetResource( *rt->GetValue( 0 ) );
	const char * const targetBone = rt->GetValue( 1 )->GetString();
	if( ! resource ){
		DSTHROW( dueNullPointer );
	}
	
	ds.GetClassCollider()->AttachBone( collider, resource, targetBone );
}

// 	public func void attachBone( Object resource, String targetBone, Vector position, Quaternion orientation )
deClassColliderRig::nfAttachBone2::nfAttachBone2( const sInitData &init ) : dsFunction( init.clsColRig,
"attachBone", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // resource
	p_AddParameter( init.clsStr ); // targetBone
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassColliderRig::nfAttachBone2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	deResource * const resource = ds.GetClassCollider()->GetResource( *rt->GetValue( 0 ) );
	const char * const targetBone = rt->GetValue( 1 )->GetString();
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() );
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 3 )->GetRealObject() );
	if( ! resource ){
		DSTHROW( dueNullPointer );
	}
	
	ds.GetClassCollider()->AttachBone( collider, resource, targetBone, position, orientation );
}

// public func void attachWeight( Object resource, CollisionInfo colInfo )
deClassColliderRig::nfAttachWeight::nfAttachWeight( const sInitData &init ) : dsFunction( init.clsColRig,
"attachWeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // resource
	p_AddParameter( init.clsCI ); // colInfo
}
void deClassColliderRig::nfAttachWeight::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	deResource * const resource = ds.GetClassCollider()->GetResource( *rt->GetValue( 0 ) );
	deCollisionInfo * collisionInfo = ds.GetClassCollisionInfo()->GetInfo( rt->GetValue( 1 )->GetRealObject() );
	if( ! resource || ! collisionInfo ){
		DSTHROW( dueNullPointer );
	}
	
	ds.GetClassCollider()->AttachWeight( collider, resource, *collisionInfo );
}



// public func void boneApplyImpuls( int bone, Vector impuls )
deClassColliderRig::nfBoneApplyImpuls::nfBoneApplyImpuls( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyImpuls", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // impuls
}
void deClassColliderRig::nfBoneApplyImpuls::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &impuls = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	collider.ApplyBoneImpuls( bone, impuls );
}

// public func void boneApplyImpulsAt( int bone, Vector impuls, Vector position )
deClassColliderRig::nfBoneApplyImpulsAt::nfBoneApplyImpulsAt( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyImpulsAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // impuls
	p_AddParameter( init.clsVec ); // position
}
void deClassColliderRig::nfBoneApplyImpulsAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &impuls = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() );
	
	collider.ApplyBoneImpulsAt( bone, impuls, position );
}

// public func void boneApplyTorqueImpuls( int bone, Vector torqueImpuls )
deClassColliderRig::nfBoneApplyTorqueImpuls::nfBoneApplyTorqueImpuls( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyTorqueImpuls", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // torqueImpuls
}
void deClassColliderRig::nfBoneApplyTorqueImpuls::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &torqueImpuls = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	collider.ApplyBoneTorqueImpuls( bone, torqueImpuls );
}

// public func void boneApplyForce( int bone, Vector force )
deClassColliderRig::nfBoneApplyForce::nfBoneApplyForce( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyForce", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // force
}
void deClassColliderRig::nfBoneApplyForce::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &force = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	collider.ApplyBoneForce( bone, force );
}

// public func void boneApplyForceAt( int bone, Vector force, Vector position )
deClassColliderRig::nfBoneApplyForceAt::nfBoneApplyForceAt( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyForceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // force
	p_AddParameter( init.clsVec ); // position
}
void deClassColliderRig::nfBoneApplyForceAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &force = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const decVector &position = ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() );
	
	collider.ApplyBoneForceAt( bone, force, position );
}

// public func void boneApplyTorque( int bone, Vector torque )
deClassColliderRig::nfBoneApplyTorque::nfBoneApplyTorque( const sInitData &init ) : dsFunction( init.clsColRig,
"boneApplyTorque", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // torque
}
void deClassColliderRig::nfBoneApplyTorque::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	const int bone = rt->GetValue( 0 )->GetInt();
	const decVector &torque = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	collider.ApplyBoneTorque( bone, torque );
}



// public func Rig getRig()
deClassColliderRig::nfGetRig::nfGetRig( const sInitData &init ) : dsFunction( init.clsColRig,
"getRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsRig ){
}
void deClassColliderRig::nfGetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	ds.GetClassRig()->PushRig( rt, collider.GetRig() );
}

// public func void setRig( Rig rig )
deClassColliderRig::nfSetRig::nfSetRig( const sInitData &init ) : dsFunction( init.clsColRig,
"setRig", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsRig ); // rig
}
void deClassColliderRig::nfSetRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	
	deRig * const rig = ds.GetClassRig()->GetRig( rt->GetValue( 0 )->GetRealObject() );
	collider.SetRig( rig );
}

// public func void copyStatesFromCollider( ColliderRig collider )
deClassColliderRig::nfCopyStatesFromCollider::nfCopyStatesFromCollider( const sInitData &init ) : dsFunction( init.clsColRig,
"copyStatesFromCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsColRig ); // collider
}
void deClassColliderRig::nfCopyStatesFromCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deClassColliderRig &clsColRig = *( ( deClassColliderRig* )GetOwnerClass() );
	deColliderRig * const collider2 = clsColRig.GetCollider( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! collider2 ){
		DSTHROW( dueNullPointer );
	}
	
	collider.CopyStatesFromColliderRig( *collider2 );
}

// public func void copyStateFromCollider( int bone, ColliderRig collider )
deClassColliderRig::nfCopyStateFromCollider::nfCopyStateFromCollider( const sInitData &init ) : dsFunction( init.clsColRig,
"copyStateFromCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsColRig ); // collider
}
void deClassColliderRig::nfCopyStateFromCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deClassColliderRig &clsColRig = *( ( deClassColliderRig* )GetOwnerClass() );
	deColliderRig * const collider2 = clsColRig.GetCollider( rt->GetValue( 1 )->GetRealObject() );
	const int bone = rt->GetValue( 0 )->GetInt();
	
	if( ! collider2 ){
		DSTHROW( dueNullPointer );
	}
	
	collider.CopyStateFromColliderRig( bone, *collider2 );
}

// public func void copyStateFromCollider( int boneFrom, int boneTo, ColliderRig collider )
deClassColliderRig::nfCopyStateFromCollider2::nfCopyStateFromCollider2( const sInitData &init ) : dsFunction( init.clsColRig,
"copyStateFromCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // boneFrom
	p_AddParameter( init.clsInt ); // boneTo
	p_AddParameter( init.clsColRig ); // collider
}
void deClassColliderRig::nfCopyStateFromCollider2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deClassColliderRig &clsColRig = *( ( deClassColliderRig* )GetOwnerClass() );
	deColliderRig * const collider2 = clsColRig.GetCollider( rt->GetValue( 2 )->GetRealObject() );
	const int boneFrom = rt->GetValue( 0 )->GetInt();
	const int boneTo = rt->GetValue( 1 )->GetInt();
	
	if( ! collider2 ){
		DSTHROW( dueNullPointer );
	}
	
	collider.CopyStateFromColliderRig( boneFrom, boneTo, *collider2 );
}



// public func DVector boneGetPosition( int bone )
deClassColliderRig::nfBoneGetPosition::nfBoneGetPosition( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassDVector()->PushDVector( rt, collider.GetBoneAt( bone ).GetPosition() );
}

// public func boneSetPosition( int bone, DVector position )
deClassColliderRig::nfBoneSetPosition::nfBoneSetPosition( const sInitData &init ) : dsFunction( init.clsColRig,
"boneSetPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsDVec ); // position
}
void deClassColliderRig::nfBoneSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 1 )->GetRealObject() );
	const int bone = rt->GetValue( 0 )->GetInt();
	
	deColliderBone &colbone = collider.GetBoneAt( bone );
	colbone.SetPosition( position );
	colbone.UpdateMatrix();
}

// public func Quaternion boneGetOrientation( int bone )
deClassColliderRig::nfBoneGetOrientation::nfBoneGetOrientation( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, collider.GetBoneAt( bone ).GetOrientation() );
}

// public func boneSetOrientation( int bone, Quaternion orientation )
deClassColliderRig::nfBoneSetOrientation::nfBoneSetOrientation( const sInitData &init ) : dsFunction( init.clsColRig,
"boneSetOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassColliderRig::nfBoneSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const decQuaternion &orientation = ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 1 )->GetRealObject() );
	const int bone = rt->GetValue( 0 )->GetInt();
	
	deColliderBone &colbone = collider.GetBoneAt( bone );
	colbone.SetOrientation( orientation );
	colbone.UpdateMatrix();
}

// public func Vector boneGetLinearVelocity( int bone )
deClassColliderRig::nfBoneGetLinearVelocity::nfBoneGetLinearVelocity( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassVector()->PushVector( rt, collider.GetBoneAt( bone ).GetLinearVelocity() );
}

// public func boneSetLinearVelocity( int bone, Vector velocity )
deClassColliderRig::nfBoneSetLinearVelocity::nfBoneSetLinearVelocity( const sInitData &init ) : dsFunction( init.clsColRig,
"boneSetLinearVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // velocity
}
void deClassColliderRig::nfBoneSetLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const decVector &velocity = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const int bone = rt->GetValue( 0 )->GetInt();
	
	collider.GetBoneAt( bone ).SetLinearVelocity( velocity );
}

// public func Vector boneGetAngularVelocity( int bone )
deClassColliderRig::nfBoneGetAngularVelocity::nfBoneGetAngularVelocity( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassVector()->PushVector( rt, collider.GetBoneAt( bone ).GetAngularVelocity() );
}

// public func boneSetAngularVelocity( int bone, Vector velocity )
deClassColliderRig::nfBoneSetAngularVelocity::nfBoneSetAngularVelocity( const sInitData &init ) : dsFunction( init.clsColRig,
"boneSetAngularVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsVec ); // velocity
}
void deClassColliderRig::nfBoneSetAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const decVector &velocity = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const int bone = rt->GetValue( 0 )->GetInt();
	
	collider.GetBoneAt( bone ).SetAngularVelocity( velocity );
}

// public func DMatrix boneGetMatrix( int bone )
deClassColliderRig::nfBoneGetMatrix::nfBoneGetMatrix( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassDMatrix()->PushDMatrix( rt, collider.GetBoneAt( bone ).GetMatrix() );
}

// public func DMatrix boneGetInverseMatrix( int bone )
deClassColliderRig::nfBoneGetInverseMatrix::nfBoneGetInverseMatrix( const sInitData &init ) : dsFunction( init.clsColRig,
"boneGetInverseMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMat ){
	p_AddParameter( init.clsInt ); // bone
}
void deClassColliderRig::nfBoneGetInverseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const deScriptingDragonScript &ds = ( ( deClassColliderRig* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassDMatrix()->PushDMatrix( rt, collider.GetBoneAt( bone ).GetInverseMatrix() );
}



// public func void enableRigBoneConstraints( int bone, bool enable )
deClassColliderRig::nfEnableRigBoneConstraints::nfEnableRigBoneConstraints( const sInitData &init ) : dsFunction( init.clsColRig,
"enableRigBoneConstraints", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsBool ); // enable
}
void deClassColliderRig::nfEnableRigBoneConstraints::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig &collider = *( ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider );
	const int bone = rt->GetValue( 0 )->GetInt();
	const bool enable = rt->GetValue( 1 )->GetBool();
	
	const deRig * const rig = collider.GetRig();
	
	if( rig && bone >= 0 && bone < rig->GetBoneCount() ){
		const deRigBone &rigBone = rig->GetBoneAt( bone );
		const int count = rigBone.GetConstraintCount();
		int i;
		
		for( i=0; i<count; i++ ){
			collider.EnableBoneConstraint( bone, i, enable );
		}
	}
}



// public func int hashCode()
deClassColliderRig::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsColRig, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassColliderRig::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig *collider = ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider;
	// hash code = memory location
	rt->PushInt( ( intptr_t )collider );
}

// public func bool equals( Object obj )
deClassColliderRig::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsColRig, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassColliderRig::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deColliderRig * const collider = ( ( sColRigNatDat* )p_GetNativeData( myself ) )->collider;
	deClassColliderRig * const clsColRig = ( deClassColliderRig* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsColRig ) ){
		rt->PushBool( false );
		
	}else{
		deColliderRig * const otherCol = ( ( sColRigNatDat* )p_GetNativeData( obj ) )->collider;
		rt->PushBool( collider == otherCol );
	}
}

// public static func bool equals( Collider collider1, Collider collider2 )
deClassColliderRig::nfEquals2::nfEquals2( const sInitData &init ) :
dsFunction( init.clsColRig, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsColRig ); // collider1
	p_AddParameter( init.clsColRig ); // collider2
}
void deClassColliderRig::nfEquals2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassColliderRig &clsColRig = *( ( deClassColliderRig* )GetOwnerClass() );
	deColliderRig * const collider1 = clsColRig.GetCollider( rt->GetValue( 0 )->GetRealObject() );
	deColliderRig * const collider2 = clsColRig.GetCollider( rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushBool( collider1 == collider2 );
}



// Class deClassColliderRig
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassColliderRig::deClassColliderRig( deScriptingDragonScript &ds ) :
dsClass( "ColliderRig", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Collider" );
	
	p_SetNativeDataSize( sizeof( sColRigNatDat ) );
}

deClassColliderRig::~deClassColliderRig(){
}



// Management
///////////////

void deClassColliderRig::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsColRig = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pDS.GetClassVector();
	init.clsDVec = pDS.GetClassDVector();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsRig = pDS.GetClassRig();
	init.clsDMat = pDS.GetClassDMatrix();
	init.clsCI = pDS.GetClassCollisionInfo();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfAttachRig( init ) );
	AddFunction( new nfAttachBone( init ) );
	AddFunction( new nfAttachBone2( init ) );
	AddFunction( new nfAttachWeight( init ) );
	
	AddFunction( new nfBoneApplyImpuls( init ) );
	AddFunction( new nfBoneApplyImpulsAt( init ) );
	AddFunction( new nfBoneApplyTorqueImpuls( init ) );
	AddFunction( new nfBoneApplyForce( init ) );
	AddFunction( new nfBoneApplyForceAt( init ) );
	AddFunction( new nfBoneApplyTorque( init ) );
	
	AddFunction( new nfGetRig( init ) );
	AddFunction( new nfSetRig( init ) );
	
	AddFunction( new nfCopyStatesFromCollider( init ) );
	AddFunction( new nfCopyStateFromCollider( init ) );
	AddFunction( new nfCopyStateFromCollider2( init ) );
	AddFunction( new nfBoneGetPosition( init ) );
	AddFunction( new nfBoneSetPosition( init ) );
	AddFunction( new nfBoneGetOrientation( init ) );
	AddFunction( new nfBoneSetOrientation( init ) );
	AddFunction( new nfBoneGetLinearVelocity( init ) );
	AddFunction( new nfBoneSetLinearVelocity( init ) );
	AddFunction( new nfBoneGetAngularVelocity( init ) );
	AddFunction( new nfBoneSetAngularVelocity( init ) );
	AddFunction( new nfBoneGetMatrix( init ) );
	AddFunction( new nfBoneGetInverseMatrix( init ) );
	
	AddFunction( new nfEnableRigBoneConstraints( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfEquals2( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deColliderRig *deClassColliderRig::GetCollider( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sColRigNatDat* )p_GetNativeData( myself->GetBuffer() ) )->collider;
}

void deClassColliderRig::PushCollider( dsRunTime *rt, deColliderRig *collider ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! collider ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassCollider * const baseClass = ( deClassCollider* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sColRigNatDat &nd = *( ( sColRigNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.collider = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.collider = collider;
		collider->AddReference();
		
		baseClass->AssignCollider( rt->GetValue( 0 )->GetRealObject(), collider );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}

void deClassColliderRig::AssignCollider( dsRealObject *myself, deColliderRig *collider ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	sColRigNatDat &nd = *( ( sColRigNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( collider == nd.collider ){
		return;
	}
	
	if( nd.collider ){
		nd.collider->FreeReference();
	}
	
	nd.collider = collider;
	
	if( collider ){
		collider->AddReference();
	}
	
	( ( deClassCollider* )GetBaseClass() )->AssignCollider( myself, collider );
}
