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

#include <libdscript/libdscript.h>

#include "deClassTouchSensor.h"
#include "deClassTouchSensorListener.h"
#include "../collider/deClassCollider.h"
#include "../collider/deClassColliderListener.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassCollisionFilter.h"
#include "../utils/deClassShapeList.h"
#include "../../peers/dedsCollider.h"
#include "../../peers/dedsTouchSensor.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsColliderListenerClosest.h"
#include "../../utils/dedsColliderListenerAdaptor.h"

#include "dragengine/resources/collider/deCollider.h"
#include "dragengine/resources/sensor/deTouchSensor.h"
#include "dragengine/resources/sensor/deTouchSensorManager.h"
#include "dragengine/deEngine.h"
#include "dragengine/common/shape/decShapeBox.h"
#include "dragengine/common/shape/decShapeSphere.h"
#include "dragengine/common/shape/decShapeCapsule.h"
#include "dragengine/common/shape/decShapeCylinder.h"
#include "dragengine/common/utils/decCollisionFilter.h"
#include <libdscript/exceptions.h>



// native structure
struct sTSNatDat{
	deTouchSensor *touchSensor;
};



// native functions
/////////////////////

// public func new( int type )
deClassTouchSensor::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsTS,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassTouchSensor::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sTSNatDat *nd = ( sTSNatDat* )p_GetNativeData( myself );
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deTouchSensorManager *tsMgr = clsTS->GetGameEngine()->GetTouchSensorManager();
	
	// clear ( important )
	nd->touchSensor = NULL;
	
	// create touchSensor
	nd->touchSensor = tsMgr->CreateTouchSensor();
	if( ! nd->touchSensor ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassTouchSensor::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsTS,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassTouchSensor::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sTSNatDat *nd = ( sTSNatDat* )p_GetNativeData( myself );
	
	if( nd->touchSensor ){
		nd->touchSensor->FreeReference();
		nd->touchSensor = NULL;
	}
}



// management
///////////////

// public func DVector getPosition()
deClassTouchSensor::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsTS,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassTouchSensor::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	clsTS->GetClassDVector()->PushDVector( rt, touchSensor->GetPosition() );
}

// public func setPosition( DVector position )
deClassTouchSensor::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsTS,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassTouchSensor::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	touchSensor->SetPosition( clsTS->GetClassDVector()->GetDVector( obj ) );
}

// public func Quaternion getOrientation()
deClassTouchSensor::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsTS,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassTouchSensor::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	clsTS->GetDS()->PushQuaternion( rt, touchSensor->GetOrientation() );
}

// public func setOrientation( Quaternion orientation )
deClassTouchSensor::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsTS,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassTouchSensor::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	touchSensor->SetOrientation( clsTS->GetClassQuaternion()->GetQuaternion( obj ) );
}

// public func CollisionFilter getCollisionFilter()
deClassTouchSensor::nfGetCollisionFilter::nfGetCollisionFilter( const sInitData &init ) : dsFunction( init.clsTS,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF ){
}
void deClassTouchSensor::nfGetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassCollisionFilter()->PushCollisionFilter( rt, touchSensor.GetCollisionFilter() );
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassTouchSensor::nfSetCollisionFilter::nfSetCollisionFilter( const sInitData &init ) : dsFunction( init.clsTS,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCF ); // collisionFilter
}
void deClassTouchSensor::nfSetCollisionFilter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	const decCollisionFilter &collisionFilter = ds.GetClassCollisionFilter()->
		GetCollisionFilter( rt->GetValue( 0 )->GetRealObject() );
	
	touchSensor.SetCollisionFilter( collisionFilter );
}

// public func bool getTrackEnterLeave()
deClassTouchSensor::nfGetTrackEnterLeave::nfGetTrackEnterLeave( const sInitData &init ) : dsFunction( init.clsTS,
"getTrackEnterLeave", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassTouchSensor::nfGetTrackEnterLeave::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	rt->PushBool( touchSensor.GetTrackEnterLeave() );
}

// public func void setTrackEnterLeave( bool trackEnterLeave )
deClassTouchSensor::nfSetTrackEnterLeave::nfSetTrackEnterLeave( const sInitData &init ) : dsFunction( init.clsTS,
"setTrackEnterLeave", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // trackEnterLeave
}
void deClassTouchSensor::nfSetTrackEnterLeave::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	touchSensor.SetTrackEnterLeave( rt->GetValue( 0 )->GetBool() );
}

// public func bool getEnabled()
deClassTouchSensor::nfGetEnabled::nfGetEnabled( const sInitData &init ) : dsFunction( init.clsTS,
"getEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassTouchSensor::nfGetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	rt->PushBool( touchSensor.GetEnabled() );
}

// public func void setEnabled( bool enabled )
deClassTouchSensor::nfSetEnabled::nfSetEnabled( const sInitData &init ) : dsFunction( init.clsTS,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassTouchSensor::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	touchSensor.SetEnabled( rt->GetValue( 0 )->GetBool() );
}

// public func ShapeList getShape()
deClassTouchSensor::nfGetShape::nfGetShape( const sInitData &init ) : dsFunction( init.clsTS,
"getShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList ){
}
void deClassTouchSensor::nfGetShape::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	ds.GetClassShapeList()->PushShapeList( rt, touchSensor.GetShape() );
}

// public func void setShape( ShapeList shape )
deClassTouchSensor::nfSetShape::nfSetShape( const sInitData &init ) : dsFunction( init.clsTS,
"setShape", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsShapeList ); // shape
}
void deClassTouchSensor::nfSetShape::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	touchSensor.SetShape( ds.GetClassShapeList()->GetShapeList(
		rt->GetValue( 0 )->GetRealObject() ) );
}



// public func bool isEmpty()
deClassTouchSensor::nfIsEmpty::nfIsEmpty( const sInitData &init ) : dsFunction( init.clsTS,
"isEmpty", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassTouchSensor::nfIsEmpty::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	rt->PushBool( touchSensor->IsEmpty() );
}

// public func int getColliderCount()
deClassTouchSensor::nfGetColliderCount::nfGetColliderCount( const sInitData &init ) : dsFunction( init.clsTS,
"getColliderCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassTouchSensor::nfGetColliderCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	rt->PushInt( touchSensor->GetColliderCount() );
}

// public func Collider getColliderAt( int index )
deClassTouchSensor::nfGetColliderAt::nfGetColliderAt( const sInitData &init ) : dsFunction( init.clsTS,
"getColliderAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol ){
	p_AddParameter( init.clsInt ); // collider
}
void deClassTouchSensor::nfGetColliderAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	clsTS->GetClassCollider()->PushCollider( rt, touchSensor->GetColliderAt(
		rt->GetValue( 0 )->GetInt() ) );
}



// Ignore colliders
/////////////////////

// public func int getIgnoreColliderCount()
deClassTouchSensor::nfGetIgnoreColliderCount::nfGetIgnoreColliderCount( const sInitData &init ) : dsFunction( init.clsTS,
"getIgnoreColliderCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassTouchSensor::nfGetIgnoreColliderCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	
	rt->PushInt( touchSensor.GetIgnoreColliderCount() );
}

// public func Collider getIgnoreColliderAt( int index )
deClassTouchSensor::nfGetIgnoreColliderAt::nfGetIgnoreColliderAt( const sInitData &init ) : dsFunction( init.clsTS,
"getIgnoreColliderAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCol ){
	p_AddParameter( init.clsInt ); // index
}
void deClassTouchSensor::nfGetIgnoreColliderAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	const int index = rt->GetValue( 0 )->GetInt();
	ds.GetClassCollider()->PushCollider( rt, touchSensor.GetIgnoreColliderAt( index ) );
}

// public func bool hasIgnoreCollider( Collider collider )
deClassTouchSensor::nfHasIgnoreCollider::nfHasIgnoreCollider( const sInitData &init ) : dsFunction( init.clsTS,
"hasIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassTouchSensor::nfHasIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	rt->PushBool( touchSensor.HasIgnoreCollider( collider ) );
}

// public func void addIgnoreCollider( Collider collider )
deClassTouchSensor::nfAddIgnoreCollider::nfAddIgnoreCollider( const sInitData &init ) : dsFunction( init.clsTS,
"addIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassTouchSensor::nfAddIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	touchSensor.AddIgnoreCollider( collider );
}

// public func void removeIgnoreCollider( Collider collider )
deClassTouchSensor::nfRemoveIgnoreCollider::nfRemoveIgnoreCollider( const sInitData &init ) : dsFunction( init.clsTS,
"removeIgnoreCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassTouchSensor::nfRemoveIgnoreCollider::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider * const collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	touchSensor.RemoveIgnoreCollider( collider );
}

// public func void removeAllIgnoreColliders()
deClassTouchSensor::nfRemoveAllIgnoreColliders::nfRemoveAllIgnoreColliders( const sInitData &init ) : dsFunction( init.clsTS,
"removeAllIgnoreColliders", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassTouchSensor::nfRemoveAllIgnoreColliders::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	
	touchSensor.RemoveAllIgnoreColliders();
}



// Collision Detection
////////////////////////

// public func bool pointInside( DVector point )
deClassTouchSensor::nfPointInside::nfPointInside( const sInitData &init ) : dsFunction( init.clsTS,
"pointInside", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsDVec ); // point
}
void deClassTouchSensor::nfPointInside::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deClassTouchSensor * const clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deScriptingDragonScript &ds = *clsTS->GetDS();
	
	const decDVector &point = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	
	rt->PushBool( touchSensor.PointInside( point ) );
}

// public func void allHits( ColliderListener listener )
deClassTouchSensor::nfAllHits::nfAllHits( const sInitData &init ) : dsFunction( init.clsTS,
"allHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfAllHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deClassTouchSensor * const clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deScriptingDragonScript &ds = *clsTS->GetDS();
	
	dsRealObject * const objListener = rt->GetValue( 0 )->GetRealObject();
	if( ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	touchSensor.AllHits( &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void rayHits( DVector rayOrigin, Vector rayDirection, ColliderListener listener )
deClassTouchSensor::nfRayHits::nfRayHits( const sInitData &init ) : dsFunction( init.clsTS,
"rayHits", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // rayOrigin
	p_AddParameter( init.clsVec ); // rayDirection
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfRayHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deClassTouchSensor * const clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deScriptingDragonScript &ds = *clsTS->GetDS();
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const decVector &rayDirection = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	touchSensor.RayHits( rayOrigin, rayDirection, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void rayHitsClosest( DVector rayOrigin, Vector rayDirection, ColliderListener listener )
deClassTouchSensor::nfRayHitsClosest::nfRayHitsClosest( const sInitData &init ) : dsFunction( init.clsTS,
"rayHitsClosest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // rayOrigin
	p_AddParameter( init.clsVec ); // rayDirection
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfRayHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	deClassTouchSensor * const clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deScriptingDragonScript &ds = *clsTS->GetDS();
	
	const decDVector &rayOrigin = ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const decVector &rayDirection = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener( objListener );
	hitsClosest.Reset();
	
	touchSensor.RayHits( rayOrigin, rayDirection, &hitsClosest );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}

// public func void colliderHits( Collider collider, ColliderListener listener )
deClassTouchSensor::nfColliderHits::nfColliderHits( const sInitData &init ) :
dsFunction( init.clsTS, "colliderHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	deScriptingDragonScript &ds = *clsTS->GetDS();
	
	dsRealObject *objCollider = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objListener = rt->GetValue( 1 )->GetRealObject();
	if( ! objCollider || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	deCollider * const collider = clsTS->GetClassCollider()->GetCollider( objCollider );
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	touchSensor->ColliderHits( collider, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderMoveHits( Collider collider, Vector displacement, ColliderListener listener )
deClassTouchSensor::nfColliderMoveHits::nfColliderMoveHits( const sInitData &init ) :
dsFunction( init.clsTS, "colliderMoveHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderMoveHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	dsRealObject * const objCollider = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objDisplacement = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	if( ! objCollider || ! objDisplacement || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	deCollider *collider = clsTS->GetClassCollider()->GetCollider( objCollider );
	decVector displacement = clsTS->GetDS()->GetVector( objDisplacement );
	
	dedsColliderListenerAdaptor &adaptor = clsTS->GetDS()->GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	touchSensor->ColliderMoveHits( collider, displacement, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderMoveHitsClosest( Collider collider, Vector displacement, ColliderListener listener )
deClassTouchSensor::nfColliderMoveHitsClosest::nfColliderMoveHitsClosest( const sInitData &init ) :
dsFunction( init.clsTS, "colliderMoveHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderMoveHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	
	dsRealObject * const objCollider = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objDisplacement = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	if( ! objCollider || ! objDisplacement || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	deCollider *collider = clsTS->GetClassCollider()->GetCollider( objCollider );
	decVector displacement = clsTS->GetDS()->GetVector( objDisplacement );
	
	dedsColliderListenerClosest &hitsClosest = clsTS->GetDS()->GetColliderListenerClosest();
	hitsClosest.SetListener( objListener );
	hitsClosest.Reset();
	
	touchSensor->ColliderMoveHits( collider, displacement, &hitsClosest );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}

// public func void colliderRotateHits( Collider collider, ColliderListener listener )
deClassTouchSensor::nfColliderRotateHits::nfColliderRotateHits( const sInitData &init ) :
dsFunction( init.clsTS, "colliderRotateHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderRotateHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider *collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector rotation( ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() ) * DEG2RAD );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	
	if( ! collider || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( ( ( dedsCollider* )collider->GetPeerScripting() )->GetCallback() );
	adaptor.Reset();
	
	touchSensor.ColliderRotateHits( collider, rotation, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderRotateHitsClosest( Collider collider, Vector rotation, ColliderListener listener )
deClassTouchSensor::nfColliderRotateHitsClosest::nfColliderRotateHitsClosest( const sInitData &init ) :
dsFunction( init.clsTS, "colliderRotateHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderRotateHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider *collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector rotation( ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() ) * DEG2RAD );
	dsRealObject * const objListener = rt->GetValue( 2 )->GetRealObject();
	
	if( ! collider || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener( objListener );
	hitsClosest.Reset();
	
	touchSensor.ColliderRotateHits( collider, rotation, &hitsClosest );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}

// public func void colliderMoveRotateHits( Collider collider, Vector displacement, Vector rotation, ColliderListener listener )
deClassTouchSensor::nfColliderMoveRotateHits::nfColliderMoveRotateHits( const sInitData &init ) :
dsFunction( init.clsTS, "colliderMoveRotateHits", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderMoveRotateHits::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider *collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector &displacement = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const decVector rotation( ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() ) * DEG2RAD );
	dsRealObject * const objListener = rt->GetValue( 3 )->GetRealObject();
	
	if( ! collider || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerAdaptor &adaptor = ds.GetColliderListenerAdaptor();
	adaptor.SetListener( objListener );
	adaptor.Reset();
	
	touchSensor.ColliderMoveRotateHits( collider, displacement, rotation, &adaptor );
	
	adaptor.SetListener( NULL );
	adaptor.Reset();
}

// public func void colliderMoveRotateHitsClosest( Collider collider, Vector displacement, Vector rotation, ColliderListener listener )
deClassTouchSensor::nfColliderMoveRotateHitsClosest::nfColliderMoveRotateHitsClosest( const sInitData &init ) :
dsFunction( init.clsTS, "colliderMoveRotateHitsClosest", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
	p_AddParameter( init.clsVec ); // displacement
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsColLis ); // listener
}
void deClassTouchSensor::nfColliderMoveRotateHitsClosest::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor &touchSensor = *( ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor );
	const deScriptingDragonScript &ds = *( ( ( deClassTouchSensor* )GetOwnerClass() )->GetDS() );
	
	deCollider *collider = ds.GetClassCollider()->GetCollider( rt->GetValue( 0 )->GetRealObject() );
	const decVector &displacement = ds.GetClassVector()->GetVector( rt->GetValue( 1 )->GetRealObject() );
	const decVector rotation( ds.GetClassVector()->GetVector( rt->GetValue( 2 )->GetRealObject() ) * DEG2RAD );
	dsRealObject * const objListener = rt->GetValue( 3 )->GetRealObject();
	
	if( ! collider || ! objListener ){
		DSTHROW( dueNullPointer );
	}
	
	dedsColliderListenerClosest &hitsClosest = ds.GetColliderListenerClosest();
	hitsClosest.SetListener( objListener );
	hitsClosest.Reset();
	
	touchSensor.ColliderMoveRotateHits( collider, displacement, rotation, &hitsClosest );
	
	hitsClosest.NotifyListener();
	hitsClosest.SetListener( NULL );
	hitsClosest.Reset();
}



// Notifications
//////////////////

// public func TouchSensorListener getListener()
deClassTouchSensor::nfGetListener::nfGetListener( const sInitData &init ) : dsFunction( init.clsTS,
"getListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsTSL ){
}
void deClassTouchSensor::nfGetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	dedsTouchSensor *scrTS = ( dedsTouchSensor* )touchSensor->GetPeerScripting();
	rt->PushObject( scrTS->GetCallback(), clsTS->GetClassTouchSensorListener() );
}

// public func void setListener( TouchSensorListener listener )
deClassTouchSensor::nfSetListener::nfSetListener( const sInitData &init ) : dsFunction( init.clsTS,
"setListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsTSL ); // listener
}
void deClassTouchSensor::nfSetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	dedsTouchSensor *scrTS = ( dedsTouchSensor* )touchSensor->GetPeerScripting();
	
	scrTS->SetCallback( rt->GetValue( 0 )->GetRealObject() );
}



// public func int hashCode()
deClassTouchSensor::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsTS, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassTouchSensor::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	// hash code = memory location
	rt->PushInt( ( intptr_t )touchSensor );
}

// public func bool equals( Object obj )
deClassTouchSensor::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsTS, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassTouchSensor::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deTouchSensor *touchSensor = ( ( sTSNatDat* )p_GetNativeData( myself ) )->touchSensor;
	deClassTouchSensor *clsTS = ( deClassTouchSensor* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsTS ) ){
		rt->PushBool( false );
	}else{
		deTouchSensor *otherCol = ( ( sTSNatDat* )p_GetNativeData( obj ) )->touchSensor;
		rt->PushBool( touchSensor == otherCol );
	}
}



// class deClassTouchSensor
///////////////////////////

// constructor
deClassTouchSensor::deClassTouchSensor( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "TouchSensor", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pDS = scrMgr;
	// store informations into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sTSNatDat ) );
}
deClassTouchSensor::~deClassTouchSensor(){
}

// management
void deClassTouchSensor::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsTSL = pDS->GetClassTouchSensorListener();
	pClsVec = pDS->GetClassVector();
	pClsDVec = pDS->GetClassDVector();
	pClsQuat = pDS->GetClassQuaternion();
	pClsCol = pDS->GetClassCollider();
	
	init.clsTS = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsQuat = pClsQuat;
	init.clsTSL = pClsTSL;
	init.clsCol = pClsCol;
	init.clsCF = pDS->GetClassCollisionFilter();
	init.clsColLis = pDS->GetClassColliderListener();
	init.clsShapeList = pDS->GetClassShapeList();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	
	AddFunction( new nfGetCollisionFilter( init ) );
	AddFunction( new nfSetCollisionFilter( init ) );
	AddFunction( new nfGetTrackEnterLeave( init ) );
	AddFunction( new nfSetTrackEnterLeave( init ) );
	AddFunction( new nfGetEnabled( init ) );
	AddFunction( new nfSetEnabled( init ) );
	AddFunction( new nfGetShape( init ) );
	AddFunction( new nfSetShape( init ) );
	
	AddFunction( new nfIsEmpty( init ) );
	AddFunction( new nfGetColliderCount( init ) );
	AddFunction( new nfGetColliderAt( init ) );
	
	AddFunction( new nfGetIgnoreColliderCount( init ) );
	AddFunction( new nfGetIgnoreColliderAt( init ) );
	AddFunction( new nfHasIgnoreCollider( init ) );
	AddFunction( new nfAddIgnoreCollider( init ) );
	AddFunction( new nfRemoveIgnoreCollider( init ) );
	AddFunction( new nfRemoveAllIgnoreColliders( init ) );
	
	AddFunction( new nfPointInside( init ) );
	AddFunction( new nfAllHits( init ) );
	AddFunction( new nfRayHits( init ) );
	AddFunction( new nfRayHitsClosest( init ) );
	AddFunction( new nfColliderHits( init ) );
	AddFunction( new nfColliderMoveHits( init ) );
	AddFunction( new nfColliderMoveHitsClosest( init ) );
	AddFunction( new nfColliderRotateHits( init ) );
	AddFunction( new nfColliderRotateHitsClosest( init ) );
	AddFunction( new nfColliderMoveRotateHits( init ) );
	AddFunction( new nfColliderMoveRotateHitsClosest( init ) );
	
	AddFunction( new nfGetListener( init ) );
	AddFunction( new nfSetListener( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deTouchSensor *deClassTouchSensor::GetTouchSensor( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sTSNatDat* )p_GetNativeData( myself->GetBuffer() ) )->touchSensor;
}

void deClassTouchSensor::PushTouchSensor( dsRunTime *rt, deTouchSensor *touchSensor ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! touchSensor ){
		rt->PushObject( NULL, this );
		return;
	}
	
	// create new value
	rt->CreateObjectNakedOnStack( this );
	( ( sTSNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->touchSensor = touchSensor;
	touchSensor->AddReference();
}
