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

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>

#include "deClassCamera.h"
#include "../deClassEngine.h"
#include "../effects/deClassEffect.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassPoint.h"
#include "../math/deClassDMatrix.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>



// Native structure
/////////////////////

struct sCamNatDat{
	deCamera *camera;
};



// Native functions
/////////////////////

// public func new()
deClassCamera::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsCamera, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	sCamNatDat &nd = *( ( sCamNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	// clear (important)
	nd.camera = NULL;
	
	// create camera
	nd.camera = ds.GetGameEngine()->GetCameraManager()->CreateCamera();
	nd.camera->SetEnableGI( ds.GetClassEngine()->GetDefaultEnableGI() );
}

// public func destructor()
deClassCamera::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsCamera, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCamNatDat &nd = *( ( sCamNatDat* )p_GetNativeData( myself ) );
	
	if( nd.camera ){
		nd.camera->FreeReference();
		nd.camera = NULL;
	}
}



// public func DVector getPosition()
deClassCamera::nfGetPosition::nfGetPosition( const sInitData &init ) :
dsFunction( init.clsCamera, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector ){
}
void deClassCamera::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	ds.GetClassDVector()->PushDVector( rt, camera.GetPosition() );
}

// public func void setPosition( DVector position )
deClassCamera::nfSetPosition::nfSetPosition( const sInitData &init ) :
dsFunction( init.clsCamera, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVector ); // position
}
void deClassCamera::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	dsRealObject * const obj = rt->GetValue( 0 )->GetRealObject();
	
	camera.SetPosition( ds.GetClassDVector()->GetDVector( obj ) );
}

// public func Quaternion getOrientation()
deClassCamera::nfGetOrientation::nfGetOrientation( const sInitData &init ) :
dsFunction( init.clsCamera, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
}
void deClassCamera::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, camera.GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassCamera::nfSetOrientation::nfSetOrientation( const sInitData &init ) :
dsFunction( init.clsCamera, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassCamera::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	dsRealObject * const obj = rt->GetValue( 0 )->GetRealObject();
	
	camera.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( obj ) );
}

// public func float getFov()
deClassCamera::nfGetFov::nfGetFov( const sInitData &init ) :
dsFunction( init.clsCamera, "getFov", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetFov::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetFov() * RAD2DEG );
}

// public func void setFov( float fov )
deClassCamera::nfSetFov::nfSetFov( const sInitData &init ) :
dsFunction( init.clsCamera, "setFov", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // fov
}
void deClassCamera::nfSetFov::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetFov( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
}

// public func float getFovRatio()
deClassCamera::nfGetFovRatio::nfGetFovRatio( const sInitData &init ) :
dsFunction( init.clsCamera, "getFovRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetFovRatio::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetFovRatio() );
}

// public func void setFovRatio( float fovRatio )
deClassCamera::nfSetFovRatio::nfSetFovRatio( const sInitData &init ) :
dsFunction( init.clsCamera, "setFovRatio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // fovRatio
}
void deClassCamera::nfSetFovRatio::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetFovRatio( rt->GetValue( 0 )->GetFloat() );
}

// public func float getImageDistance()
deClassCamera::nfGetImageDistance::nfGetImageDistance( const sInitData &init ) :
dsFunction( init.clsCamera, "getImageDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetImageDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetImageDistance() );
}

// public func void setImageDistance( float distance )
deClassCamera::nfSetImageDistance::nfSetImageDistance( const sInitData &init ) :
dsFunction( init.clsCamera, "setImageDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // distance
}
void deClassCamera::nfSetImageDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetImageDistance( rt->GetValue( 0 )->GetFloat() );
}

// public func float getViewDistance()
deClassCamera::nfGetViewDistance::nfGetViewDistance( const sInitData &init ) :
dsFunction( init.clsCamera, "getViewDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetViewDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetViewDistance() );
}

// public func void setViewDistance( float distance )
deClassCamera::nfSetViewDistance::nfSetViewDistance( const sInitData &init ) :
dsFunction( init.clsCamera, "setViewDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // distance
}
void deClassCamera::nfSetViewDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetViewDistance( rt->GetValue( 0 )->GetFloat() );
}



// public func bool getEnableHDRR()
deClassCamera::nfGetEnableHDRR::nfGetEnableHDRR( const sInitData &init ) :
dsFunction( init.clsCamera, "getEnableHDRR", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBoolean ){
}
void deClassCamera::nfGetEnableHDRR::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushBool( camera.GetEnableHDRR() );
}

// public func void setEnableHDRR( bool enable )
deClassCamera::nfSetEnableHDRR::nfSetEnableHDRR( const sInitData &init ) :
dsFunction( init.clsCamera, "setEnableHDRR", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBoolean ); // enable
}
void deClassCamera::nfSetEnableHDRR::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetEnableHDRR( rt->GetValue( 0 )->GetBool() );
}

// public func float getExposure()
deClassCamera::nfGetExposure::nfGetExposure( const sInitData &init ) :
dsFunction( init.clsCamera, "getExposure", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetExposure::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetExposure() );
}

// public func void setExposure( float exposure )
deClassCamera::nfSetExposure::nfSetExposure( const sInitData &init ) :
dsFunction( init.clsCamera, "setExposure", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // exposure
}
void deClassCamera::nfSetExposure::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetExposure( rt->GetValue( 0 )->GetFloat() );
}

// public func float getLowestIntensity()
deClassCamera::nfGetLowestIntensity::nfGetLowestIntensity( const sInitData &init ) :
dsFunction( init.clsCamera, "getLowestIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetLowestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetLowestIntensity() );
}

// public func void setLowestIntensity( float lowestIntensity )
deClassCamera::nfSetLowestIntensity::nfSetLowestIntensity( const sInitData &init ) :
dsFunction( init.clsCamera, "setLowestIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // lowestIntensity
}
void deClassCamera::nfSetLowestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetLowestIntensity( rt->GetValue( 0 )->GetFloat() );
}

// public func float getHighestIntensity()
deClassCamera::nfGetHighestIntensity::nfGetHighestIntensity( const sInitData &init ) :
dsFunction( init.clsCamera, "getHighestIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetHighestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetHighestIntensity() );
}

// public func void setHighestIntensity( float highestIntensity )
deClassCamera::nfSetHighestIntensity::nfSetHighestIntensity( const sInitData &init ) :
dsFunction( init.clsCamera, "setHighestIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // highestIntensity
}
void deClassCamera::nfSetHighestIntensity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetHighestIntensity( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdaptionTime()
deClassCamera::nfGetAdaptionTime::nfGetAdaptionTime( const sInitData &init ) :
dsFunction( init.clsCamera, "getAdaptionTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassCamera::nfGetAdaptionTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushFloat( camera.GetAdaptionTime() );
}

// public func void setAdaptionTime( float adaptionTime )
deClassCamera::nfSetAdaptionTime::nfSetAdaptionTime( const sInitData &init ) :
dsFunction( init.clsCamera, "setAdaptionTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // adaptionTime
}
void deClassCamera::nfSetAdaptionTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetAdaptionTime( rt->GetValue( 0 )->GetFloat() );
}

// public func void resetAdaptedIntensity()
deClassCamera::nfResetAdaptedIntensity::nfResetAdaptedIntensity( const sInitData &init ) :
dsFunction( init.clsCamera, "resetAdaptedIntensity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfResetAdaptedIntensity::RunFunction( dsRunTime*, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	camera.ResetAdaptedIntensity();
}



// public func bool getEnableGI()
deClassCamera::nfGetEnableGI::nfGetEnableGI( const sInitData &init ) :
dsFunction( init.clsCamera, "getEnableGI", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBoolean ){
}
void deClassCamera::nfGetEnableGI::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	rt->PushBool( camera.GetEnableGI() );
}

// public func void setEnableGI( bool enable )
deClassCamera::nfSetEnableGI::nfSetEnableGI( const sInitData &init ) :
dsFunction( init.clsCamera, "setEnableGI", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBoolean ); // enable
}
void deClassCamera::nfSetEnableGI::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.SetEnableGI( rt->GetValue( 0 )->GetBool() );
}



// public func Point project( Point viewportSize, DVector position )
deClassCamera::nfProject::nfProject( const sInitData &init ) :
dsFunction( init.clsCamera, "project", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint ){
	p_AddParameter( init.clsPoint ); // viewportSize
	p_AddParameter( init.clsDVector ); // position
}
void deClassCamera::nfProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
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
deClassCamera::nfBackProject::nfBackProject( const sInitData &init ) :
dsFunction( init.clsCamera, "backProject", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
	p_AddParameter( init.clsPoint ); // viewportSize
	p_AddParameter( init.clsPoint ); // position
}
void deClassCamera::nfBackProject::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
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
deClassCamera::nfBackProject2::nfBackProject2( const sInitData &init ) :
dsFunction( init.clsCamera, "backProject", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
	p_AddParameter( init.clsInteger ); // width
	p_AddParameter( init.clsInteger ); // height
	p_AddParameter( init.clsInteger ); // x
	p_AddParameter( init.clsInteger ); // y
}
void deClassCamera::nfBackProject2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
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
deClassCamera::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) :
dsFunction( init.clsCamera, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassCamera::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	ds.GetClassLayerMask()->PushLayerMask( rt, camera.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassCamera::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) :
dsFunction( init.clsCamera, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassCamera::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	camera.GetLayerMask() = ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() );
	camera.NotifyLayerMaskChanged();
}

// public func World getParentWorld()
deClassCamera::nfGetParentWorld::nfGetParentWorld( const sInitData &init ) :
dsFunction( init.clsCamera, "getParentWorld", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld ){
}
void deClassCamera::nfGetParentWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	ds.GetClassWorld()->PushWorld( rt, camera.GetParentWorld() );
}



// public func DMatrix getMatrix()
deClassCamera::nfGetMatrix::nfGetMatrix( const sInitData &init ) :
dsFunction( init.clsWorld, "getMatrix", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
}
void deClassCamera::nfGetMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	
	ds.GetClassDMatrix()->PushDMatrix( rt, decDMatrix::CreateWorld(
		camera.GetPosition(), camera.GetOrientation() ) );
}



// Efects
///////////

// public func int getEffectCount()
deClassCamera::nfGetEffectCount::nfGetEffectCount( const sInitData &init ) :
dsFunction( init.clsCamera, "getEffectCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassCamera::nfGetEffectCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	rt->PushInt( camera.GetEffectCount() );
}

// public func Effect getEffectAt( int index )
deClassCamera::nfGetEffectAt::nfGetEffectAt( const sInitData &init ) :
dsFunction( init.clsCamera, "getEffect", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsEffect ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassCamera::nfGetEffectAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassEffect()->PushEffect( rt, camera.GetEffectAt( index ) );
}

// public func void addEffect( Effect effect )
deClassCamera::nfAddEffect::nfAddEffect( const sInitData &init ) :
dsFunction( init.clsCamera, "addEffect", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsEffect ); // effect
}
void deClassCamera::nfAddEffect::RunFunction(dsRunTime *rt, dsValue *myself){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera.AddEffect( ds.GetClassEffect()->GetEffect( obj ) );
}

// public func void removeEffect( Effect effect )
deClassCamera::nfRemoveEffect::nfRemoveEffect( const sInitData &init ) :
dsFunction( init.clsCamera, "removeEffect", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsEffect ); // effect
}
void deClassCamera::nfRemoveEffect::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	const deScriptingDragonScript &ds = ( ( deClassCamera* )GetOwnerClass() )->GetDS();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	if( ! obj ) DSTHROW( dueNullPointer );
	
	camera.RemoveEffect( ds.GetClassEffect()->GetEffect( obj ) );
}

// public func void removeAllEffects()
deClassCamera::nfRemoveAllEffects::nfRemoveAllEffects( const sInitData &init ) :
dsFunction( init.clsCamera, "removeAllEffects", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCamera::nfRemoveAllEffects::RunFunction( dsRunTime*, dsValue *myself ){
	deCamera &camera = *( ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera );
	
	camera.RemoveAllEffects();
}



// Misc
/////////

// public func int hashCode()
deClassCamera::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCamera, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassCamera::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera * const camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )camera );
}

// public func bool equals( Object object )
deClassCamera::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCamera, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBoolean ){
	p_AddParameter( init.clsObject ); // object
}
void deClassCamera::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCamera * const camera = ( ( sCamNatDat* )p_GetNativeData( myself ) )->camera;
	deClassCamera * const clsCamera = ( deClassCamera* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCamera ) ){
		rt->PushBool( false );
		
	}else{
		const deCamera * const otherCamera = ( ( sCamNatDat* )p_GetNativeData( obj ) )->camera;
		rt->PushBool( camera == otherCamera );
	}
}



// Class deClassCamera
////////////////////////

// Constructor
////////////////

deClassCamera::deClassCamera( deScriptingDragonScript &ds ) :
dsClass( "Camera", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sCamNatDat ) );
}

deClassCamera::~deClassCamera(){
}



// Management
///////////////

void deClassCamera::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCamera = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBoolean = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsVector = pDS.GetClassVector();
	init.clsDVector = pDS.GetClassDVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsEffect = pDS.GetClassEffect();
	init.clsPoint = pDS.GetClassPoint();
	init.clsLayerMask = pDS.GetClassLayerMask();
	init.clsWorld = pDS.GetClassWorld();
	init.clsDMatrix = pDS.GetClassDMatrix();
	
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
	
	AddFunction( new nfGetEnableHDRR( init ) );
	AddFunction( new nfSetEnableHDRR( init ) );
	AddFunction( new nfGetExposure( init ) );
	AddFunction( new nfSetExposure( init ) );
	AddFunction( new nfGetLowestIntensity( init ) );
	AddFunction( new nfSetLowestIntensity( init ) );
	AddFunction( new nfGetHighestIntensity( init ) );
	AddFunction( new nfSetHighestIntensity( init ) );
	AddFunction( new nfGetAdaptionTime( init ) );
	AddFunction( new nfSetAdaptionTime( init ) );
	AddFunction( new nfResetAdaptedIntensity( init ) );
	AddFunction( new nfProject( init ) );
	AddFunction( new nfBackProject( init ) );
	AddFunction( new nfBackProject2( init ) );
	
	AddFunction( new nfGetEnableGI( init ) );
	AddFunction( new nfSetEnableGI( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	AddFunction( new nfGetParentWorld( init ) );
	
	AddFunction( new nfGetMatrix( init ) );
	
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
