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
#include <libdscript/exceptions.h>

#include "deClassCamera.h"
#include "../effects/deClassEffect.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassPoint.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/deEngine.h>



// native structure
struct sCamNatDat{
	deCamera *camera;
};



// Native functions
/////////////////////

// public func new()
deClassCamera::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCam,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCamNatDat *nd = ( sCamNatDat* )p_GetNativeData( myself );
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	deCameraManager *camMgr = clsCam->GetGameEngine()->GetCameraManager();
	
	// clear (important)
	nd->camera = NULL;
	
	// create camera
	nd->camera = camMgr->CreateCamera();
	if( ! nd->camera ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassCamera::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCam,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCamNatDat *nd = ( sCamNatDat* )p_GetNativeData( myself );
	
	if( nd->camera ){
		nd->camera->FreeReference();
		nd->camera = NULL;
	}
}



// public func DVector getPosition()
deClassCamera::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsCam,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassCamera::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	
	clsCam->GetClassDVector()->PushDVector( rt, camera->GetPosition() );
}

// public func void setPosition( DVector position )
deClassCamera::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsCam,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassCamera::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera->SetPosition( clsCam->GetClassDVector()->GetDVector( obj ) );
}

// public func Quaternion getOrientation()
deClassCamera::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsCam,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassCamera::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	deClassQuaternion *clsQuat = clsCam->GetScriptModule()->GetClassQuaternion();
	
	clsQuat->PushQuaternion( rt, camera->GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassCamera::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsCam,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassCamera::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	deClassQuaternion *clsQuat = clsCam->GetScriptModule()->GetClassQuaternion();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera->SetOrientation( clsQuat->GetQuaternion( obj ) );
}

// public func float getFov()
deClassCamera::nfGetFov::nfGetFov( const sInitData &init ) : dsFunction( init.clsCam,
"getFov", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetFov::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetFov() / DEG2RAD );
}

// public func void setFov( float fov )
deClassCamera::nfSetFov::nfSetFov( const sInitData &init ) : dsFunction( init.clsCam,
"setFov", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // fov
}
void deClassCamera::nfSetFov::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetFov( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
}

// public func float getFovRatio()
deClassCamera::nfGetFovRatio::nfGetFovRatio( const sInitData &init ) : dsFunction( init.clsCam,
"getFovRatio", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetFovRatio::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetFovRatio() );
}

// public func void setFovRatio( float fovRatio )
deClassCamera::nfSetFovRatio::nfSetFovRatio( const sInitData &init ) : dsFunction( init.clsCam,
"setFovRatio", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // fovRatio
}
void deClassCamera::nfSetFovRatio::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetFovRatio( rt->GetValue( 0 )->GetFloat() );
}

// public func float getImageDistance()
deClassCamera::nfGetImageDistance::nfGetImageDistance( const sInitData &init ) : dsFunction( init.clsCam,
"getImageDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetImageDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetImageDistance() );
}

// public func void setImageDistance( float distance )
deClassCamera::nfSetImageDistance::nfSetImageDistance( const sInitData &init ) : dsFunction( init.clsCam,
"setImageDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // distance
}
void deClassCamera::nfSetImageDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetImageDistance( rt->GetValue( 0 )->GetFloat() );
}

// public func float getViewDistance()
deClassCamera::nfGetViewDistance::nfGetViewDistance( const sInitData &init ) : dsFunction( init.clsCam,
"getViewDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetViewDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetViewDistance() );
}

// public func void setViewDistance( float distance )
deClassCamera::nfSetViewDistance::nfSetViewDistance( const sInitData &init ) : dsFunction( init.clsCam,
"setViewDistance", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // distance
}
void deClassCamera::nfSetViewDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetViewDistance( rt->GetValue( 0 )->GetFloat() );
}

// public func float getExposure()
deClassCamera::nfGetExposure::nfGetExposure( const sInitData &init ) : dsFunction( init.clsCam,
"getExposure", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetExposure::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetExposure() );
}

// public func void setExposure( float exposure )
deClassCamera::nfSetExposure::nfSetExposure( const sInitData &init ) : dsFunction( init.clsCam,
"setExposure", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // exposure
}
void deClassCamera::nfSetExposure::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetExposure( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLowestIntensity()
deClassCamera::nfGetLowestIntensity::nfGetLowestIntensity( const sInitData &init ) : dsFunction( init.clsCam,
"getLowestIntensity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetLowestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetLowestIntensity() );
}

// public func void setLowestIntensity( float lowestIntensity )
deClassCamera::nfSetLowestIntensity::nfSetLowestIntensity( const sInitData &init ) : dsFunction( init.clsCam,
"setLowestIntensity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // lowestIntensity
}
void deClassCamera::nfSetLowestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetLowestIntensity( rt->GetValue( 0 )->GetFloat() );
}

// public func float getHighestIntensity()
deClassCamera::nfGetHighestIntensity::nfGetHighestIntensity( const sInitData &init ) : dsFunction( init.clsCam,
"getHighestIntensity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetHighestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetHighestIntensity() );
}

// public func void setHighestIntensity( float highestIntensity )
deClassCamera::nfSetHighestIntensity::nfSetHighestIntensity( const sInitData &init ) : dsFunction( init.clsCam,
"setHighestIntensity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // highestIntensity
}
void deClassCamera::nfSetHighestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetHighestIntensity( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdaptionTime()
deClassCamera::nfGetAdaptionTime::nfGetAdaptionTime( const sInitData &init ) : dsFunction( init.clsCam,
"getAdaptionTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassCamera::nfGetAdaptionTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	rt->PushFloat( camera->GetAdaptionTime() );
}

// public func void setAdaptionTime( float adaptionTime )
deClassCamera::nfSetAdaptionTime::nfSetAdaptionTime( const sInitData &init ) : dsFunction( init.clsCam,
"setAdaptionTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // adaptionTime
}
void deClassCamera::nfSetAdaptionTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->SetAdaptionTime( rt->GetValue( 0 )->GetFloat() );
}



// public func Point project( Point viewportSize, DVector position )
deClassCamera::nfProject::nfProject( const sInitData &init ) : dsFunction( init.clsCam,
"project", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint ){
	p_AddParameter( init.clsPoint ); // viewportSize
	p_AddParameter( init.clsDVec ); // position
}
void deClassCamera::nfProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	const decPoint &viewportSize = ds.GetClassPoint()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decDVector &position = ds.GetClassDVector()->GetDVector( rt->GetValue( 1 )->GetRealObject() );
	
	const double aspectRatio = ( double )viewportSize.x / ( double )viewportSize.y;
	const double halfHeight = ( double )viewportSize.y * 0.5;
	const double halfWidth = ( double )viewportSize.x * 0.5;
	
	decDVector projected( camera.GetOrientation().Conjugate() * ( position - camera.GetPosition() ) );
	if( projected.z < FLOAT_SAFE_EPSILON ){
		ds.GetClassPoint()->PushPoint( rt, decPoint() );
		return;
	}
	
	projected.x = projected.x / tan( ( double )camera.GetFov() * 0.5 ) / projected.z;
	projected.y = projected.y * aspectRatio / tan( ( double )camera.GetFov()
		* ( double )camera.GetFovRatio() * 0.5 ) / projected.z;
	
	projected.x = projected.x * halfWidth + halfWidth;
	projected.y = halfHeight - projected.y * halfWidth;
	
	const decPoint point( ( int )projected.x, ( int )projected.y );
	
	ds.GetClassPoint()->PushPoint( rt, point );
}

// public func Vector backProject( Point viewportSize, Point position )
deClassCamera::nfBackProject::nfBackProject( const sInitData &init ) : dsFunction( init.clsCam,
"backProject", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsPoint ); // viewportSize
	p_AddParameter( init.clsPoint ); // position
}
void deClassCamera::nfBackProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	const decPoint &viewportSize = ds.GetClassPoint()->GetPoint( rt->GetValue( 0 )->GetRealObject() );
	const decPoint &position = ds.GetClassPoint()->GetPoint( rt->GetValue( 1 )->GetRealObject() );
	
	const double aspectRatio = ( double )viewportSize.x / ( double )viewportSize.y;
	const int halfHeight = viewportSize.y / 2;
	const int halfWidth = viewportSize.x / 2;
	decDVector direction;
	
	direction.x = tan( ( double )camera.GetFov() * 0.5 )
		* ( ( double )( position.x - halfWidth ) / ( double )halfWidth );
	direction.y = tan( ( double )camera.GetFov() * ( double )camera.GetFovRatio() * 0.5 )
		* ( ( double )( halfHeight - position.y ) / ( double )halfHeight ) / aspectRatio;
	direction.z = 1.0;
	
	ds.GetClassVector()->PushVector( rt, ( camera.GetOrientation() * direction.Normalized() ).ToVector() );
}

// public func Vector backProject( int width, int height, int x, int y )
deClassCamera::nfBackProject2::nfBackProject2( const sInitData &init ) : dsFunction( init.clsCam,
"backProject", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsInt ); // width
	p_AddParameter( init.clsInt ); // height
	p_AddParameter( init.clsInt ); // x
	p_AddParameter( init.clsInt ); // y
}
void deClassCamera::nfBackProject2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	const int width = rt->GetValue( 0 )->GetInt();
	const int height = rt->GetValue( 1 )->GetInt();
	const int x = rt->GetValue( 2 )->GetInt();
	const int y = rt->GetValue( 3 )->GetInt();
	
	const double aspectRatio = ( double )width / ( double )height;
	const int halfHeight = height / 2;
	const int halfWidth = width / 2;
	decDVector direction;
	
	direction.x = tan( ( double )camera.GetFov() * 0.5 )
		* ( ( double )( x - halfWidth ) / ( double )halfWidth );
	direction.y = tan( ( double )camera.GetFov() * ( double )camera.GetFovRatio() * 0.5 )
		* ( ( double )( halfHeight - y ) / ( double )halfHeight ) / aspectRatio;
	direction.z = 1.0;
	
	ds.GetClassVector()->PushVector( rt, ( camera.GetOrientation() * direction.Normalized() ).ToVector() );
}



// public func LayerMask getLayerMask()
deClassCamera::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) : dsFunction( init.clsCam,
"getLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassCamera::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassLayerMask()->PushLayerMask( rt, camera.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassCamera::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) : dsFunction( init.clsCam,
"setLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassCamera::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	
	camera.GetLayerMask() = ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() );
	camera.NotifyLayerMaskChanged();
}

// public func World getParentWorld()
deClassCamera::nfGetParentWorld::nfGetParentWorld( const sInitData &init ) : dsFunction( init.clsCam,
"getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld ){
}
void deClassCamera::nfGetParentWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = *( ( deClassCamera* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassWorld()->PushWorld( rt, camera.GetParentWorld() );
}



// Efects
///////////

// public func int getEffectCount()
deClassCamera::nfGetEffectCount::nfGetEffectCount( const sInitData &init ) : dsFunction( init.clsCam,
"getEffectCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCamera::nfGetEffectCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	rt->PushInt( camera->GetEffectCount() );
}

// public func Effect getEffectAt( int index )
deClassCamera::nfGetEffectAt::nfGetEffectAt( const sInitData &init ) : dsFunction( init.clsCam,
"getEffect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsEff ){
	p_AddParameter( init.clsInt ); // index
}
void deClassCamera::nfGetEffectAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	int index = rt->GetValue( 0 )->GetInt();
	
	clsCam->GetClassEffect()->PushEffect( rt, camera->GetEffectAt( index ) );
}

// public func void addEffect( Effect effect )
deClassCamera::nfAddEffect::nfAddEffect( const sInitData &init ) : dsFunction( init.clsCam,
"addEffect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsEff ); // effect
}
void deClassCamera::nfAddEffect::RunFunction(dsRunTime *rt, dsValue *myself){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera->AddEffect( clsCam->GetClassEffect()->GetEffect( obj ) );
}

// public func void removeEffect( Effect effect )
deClassCamera::nfRemoveEffect::nfRemoveEffect( const sInitData &init ) : dsFunction( init.clsCam,
"removeEffect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsEff ); // effect
}
void deClassCamera::nfRemoveEffect::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera->RemoveEffect( clsCam->GetClassEffect()->GetEffect( obj ) );
}

// public func void removeAllEffects()
deClassCamera::nfRemoveAllEffects::nfRemoveAllEffects( const sInitData &init ) : dsFunction( init.clsCam,
"removeAllEffects", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfRemoveAllEffects::RunFunction(dsRunTime *rt, dsValue *myself){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	camera->RemoveAllEffects();
}



// Misc
/////////

// public func int hashCode()
deClassCamera::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCam, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCamera::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )camera );
}

// public func bool equals( Object object )
deClassCamera::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCam, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassCamera::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera *camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera *clsCam = ( deClassCamera* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCam ) ){
		rt->PushBool( false );
	}else{
		deCamera *otherCamera = ( ( sCamNatDat* )p_GetNativeData( obj ) )->camera;
		rt->PushBool( camera == otherCamera );
	}
}



// Class deClassCamera
////////////////////////

// Constructor
////////////////

deClassCamera::deClassCamera( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "Camera", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store information into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sCamNatDat ) );
}

deClassCamera::~deClassCamera(){
}



// Management
///////////////

void deClassCamera::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsDVec = pScrMgr->GetClassDVector();
	pClsEff = pScrMgr->GetClassEffect();
	pClsVec = pScrMgr->GetClassVector();
	
	init.clsCam = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsQuat = pScrMgr->GetClassQuaternion();
	init.clsEff = pClsEff;
	init.clsPoint = pScrMgr->GetClassPoint();
	init.clsLayerMask = pScrMgr->GetClassLayerMask();
	init.clsWorld = pScrMgr->GetClassWorld();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetFov( init ) );
	AddFunction( new nfSetFov( init ) );
	AddFunction( new nfGetFovRatio( init ) );
	AddFunction( new nfSetFovRatio( init ) );
	AddFunction( new nfGetImageDistance( init ) );
	AddFunction( new nfSetImageDistance( init ) );
	AddFunction( new nfGetViewDistance( init ) );
	AddFunction( new nfSetViewDistance( init ) );
	AddFunction( new nfGetExposure( init ) );
	AddFunction( new nfSetExposure( init ) );
	AddFunction( new nfGetLowestIntensity( init ) );
	AddFunction( new nfSetLowestIntensity( init ) );
	AddFunction( new nfGetHighestIntensity( init ) );
	AddFunction( new nfSetHighestIntensity( init ) );
	AddFunction( new nfGetAdaptionTime( init ) );
	AddFunction( new nfSetAdaptionTime( init ) );
	AddFunction( new nfProject( init ) );
	AddFunction( new nfBackProject( init ) );
	AddFunction( new nfBackProject2( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	AddFunction( new nfGetParentWorld( init ) );
	
	AddFunction( new nfGetEffectCount( init ) );
	AddFunction( new nfGetEffectAt( init ) );
	AddFunction( new nfAddEffect( init ) );
	AddFunction( new nfRemoveEffect( init ) );
	AddFunction( new nfRemoveAllEffects( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deCamera *deClassCamera::GetCamera( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCamNatDat* )p_GetNativeData( myself->GetBuffer() ) )->camera;
}

void deClassCamera::PushCamera( dsRunTime *rt, deCamera *camera ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! camera ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sCamNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->camera = camera;
	camera->AddReference();
}
