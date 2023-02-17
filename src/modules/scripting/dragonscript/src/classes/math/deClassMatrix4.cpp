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
#include <string.h>

#include "deClassDMatrix.h"
#include "deClassDMatrix4.h"
#include "deClassMatrix.h"
#include "deClassMatrix4.h"
#include "deClassVector.h"
#include "deClassQuaternion.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Native structure
/////////////////////

struct sMatNatDat{
	decMatrix matrix;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassMatrix4::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsMatrix4,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMatrix4::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.SetIdentity();
}

// public func new( float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24,
// float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44 )
deClassMatrix4::nfNewValues::nfNewValues( const sInitData &init ) : dsFunction( init.clsMatrix4,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // a11
	p_AddParameter( init.clsFlt ); // a12
	p_AddParameter( init.clsFlt ); // a13
	p_AddParameter( init.clsFlt ); // a14
	p_AddParameter( init.clsFlt ); // a21
	p_AddParameter( init.clsFlt ); // a22
	p_AddParameter( init.clsFlt ); // a23
	p_AddParameter( init.clsFlt ); // a24
	p_AddParameter( init.clsFlt ); // a31
	p_AddParameter( init.clsFlt ); // a32
	p_AddParameter( init.clsFlt ); // a33
	p_AddParameter( init.clsFlt ); // a34
	p_AddParameter( init.clsFlt ); // a41
	p_AddParameter( init.clsFlt ); // a42
	p_AddParameter( init.clsFlt ); // a43
	p_AddParameter( init.clsFlt ); // a44
}
void deClassMatrix4::nfNewValues::RunFunction( dsRunTime *rt, dsValue *myself ){
	decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.a11 = rt->GetValue( 0 )->GetFloat();
	matrix.a12 = rt->GetValue( 1 )->GetFloat();
	matrix.a13 = rt->GetValue( 2 )->GetFloat();
	matrix.a14 = rt->GetValue( 3 )->GetFloat();
	matrix.a21 = rt->GetValue( 4 )->GetFloat();
	matrix.a22 = rt->GetValue( 5 )->GetFloat();
	matrix.a23 = rt->GetValue( 6 )->GetFloat();
	matrix.a24 = rt->GetValue( 7 )->GetFloat();
	matrix.a31 = rt->GetValue( 8 )->GetFloat();
	matrix.a32 = rt->GetValue( 9 )->GetFloat();
	matrix.a33 = rt->GetValue( 10 )->GetFloat();
	matrix.a34 = rt->GetValue( 11 )->GetFloat();
	matrix.a41 = rt->GetValue( 12 )->GetFloat();
	matrix.a42 = rt->GetValue( 13 )->GetFloat();
	matrix.a43 = rt->GetValue( 14 )->GetFloat();
	matrix.a44 = rt->GetValue( 15 )->GetFloat();
}

// public static func Matrix newTranslation( Vector translation )
deClassMatrix4::nfNewTranslation::nfNewTranslation( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // translation
}
void deClassMatrix4::nfNewTranslation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objTranslation = rt->GetValue( 0 )->GetRealObject();
	
	const decVector &translation = clsVec.GetVector( objTranslation );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateTranslation( translation ) );
}

// public static func Matrix newTranslation( float tx, float ty, float tz )
deClassMatrix4::nfNewTranslation2::nfNewTranslation2( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // tx
	p_AddParameter( init.clsFlt ); // ty
	p_AddParameter( init.clsFlt ); // tz
}
void deClassMatrix4::nfNewTranslation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float tx = rt->GetValue( 0 )->GetFloat();
	const float ty = rt->GetValue( 1 )->GetFloat();
	const float tz = rt->GetValue( 2 )->GetFloat();
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateTranslation( tx, ty, tz ) );
}

// public static func Matrix newScaling( Vector scaling )
deClassMatrix4::nfNewScaling::nfNewScaling( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // scaling
}
void deClassMatrix4::nfNewScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector( objScaling );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateScale( scaling ) );
}

// public static func Matrix newScaling( float sx, float sy, float sz )
deClassMatrix4::nfNewScaling2::nfNewScaling2( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // sx
	p_AddParameter( init.clsFlt ); // sy
	p_AddParameter( init.clsFlt ); // sz
}
void deClassMatrix4::nfNewScaling2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float sx = rt->GetValue( 0 )->GetFloat();
	const float sy = rt->GetValue( 1 )->GetFloat();
	const float sz = rt->GetValue( 2 )->GetFloat();
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateScale( sx, sy, sz ) );
}

// public static func Matrix newRotationX( float rotation )
deClassMatrix4::nfNewRotationX::nfNewRotationX( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotationX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassMatrix4::nfNewRotationX::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float rotation = rt->GetValue( 0 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotationX( rotation ) );
}

// public static func Matrix newRotationY( float rotation )
deClassMatrix4::nfNewRotationY::nfNewRotationY( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotationY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassMatrix4::nfNewRotationY::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float rotation = rt->GetValue( 0 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotationY( rotation ) );
}

// public static func Matrix newRotationZ( float rotation )
deClassMatrix4::nfNewRotationZ::nfNewRotationZ( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotationZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassMatrix4::nfNewRotationZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float rotation = rt->GetValue( 0 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotationZ( rotation ) );
}

// public static func Matrix newRotation( Vector rotation )
deClassMatrix4::nfNewRotation::nfNewRotation( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // rotation
}
void deClassMatrix4::nfNewRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	
	const decVector rotation = clsVec.GetVector( objRotation ) * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotation( rotation ) );
}

// public static func Matrix newRotation( float rx, float ry, float rz )
deClassMatrix4::nfNewRotation2::nfNewRotation2( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // rx
	p_AddParameter( init.clsFlt ); // ry
	p_AddParameter( init.clsFlt ); // rz
}
void deClassMatrix4::nfNewRotation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float rx = rt->GetValue( 0 )->GetFloat() * DEG2RAD;
	const float ry = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	const float rz = rt->GetValue( 2 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotation( rx, ry, rz ) );
}

// public static func Matrix newRotationAxis( Vector axis, float rotation )
deClassMatrix4::nfNewRotationAxis::nfNewRotationAxis( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRotationAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // axis
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassMatrix4::nfNewRotationAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	
	const decVector &axis = clsVec.GetVector( rt->GetValue( 0 )->GetRealObject() );
	const float rotation = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRotationAxis( axis, rotation ) );
}

// public static func Matrix newSRT( Vector scaling, Vector rotation, Vector translation )
deClassMatrix4::nfNewSRT::nfNewSRT( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // scaling
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsVec ); // translation
}
void deClassMatrix4::nfNewSRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objRotation = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 2 )->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector( objScaling );
	const decVector rotation = clsVec.GetVector( objRotation ) * DEG2RAD;
	const decVector &translation = clsVec.GetVector( objTranslation );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateSRT( scaling, rotation, translation ) );
}

// public static func Matrix newRT( Vector rotation, Vector translation )
deClassMatrix4::nfNewRT::nfNewRT( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsVec ); // translation
}
void deClassMatrix4::nfNewRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 1 )->GetRealObject();
	
	const decVector rotation = clsVec.GetVector( objRotation ) * DEG2RAD;
	const decVector &translation = clsVec.GetVector( objTranslation );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateRT( rotation, translation ) );
}

// public static func Matrix newSVUT( Vector scaling, Vector view, Vector up, Vector translation )
deClassMatrix4::nfNewSVUT::nfNewSVUT( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newSVUT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // rotation
	p_AddParameter( init.clsVec ); // view
	p_AddParameter( init.clsVec ); // up
	p_AddParameter( init.clsVec ); // translation
}
void deClassMatrix4::nfNewSVUT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 3 )->GetRealObject();
	
	const decVector &scaling = clsVec.GetVector( objScaling );
	const decVector &view = clsVec.GetVector( objView );
	const decVector &up = clsVec.GetVector( objUp );
	const decVector &translation = clsVec.GetVector( objTranslation );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateSVUT( scaling, view, up, translation ) );
}

// public static func Matrix newVU( Vector view, Vector up )
deClassMatrix4::nfNewVU::nfNewVU( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newVU", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // view
	p_AddParameter( init.clsVec ); // up
}
void deClassMatrix4::nfNewVU::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objView = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 1 )->GetRealObject();
	
	const decVector &view = clsVec.GetVector( objView );
	const decVector &up = clsVec.GetVector( objUp );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateVU( view, up ) );
}

// public static func Matrix newCamera( Vector position, Vector view, Vector up )
deClassMatrix4::nfNewCamera::nfNewCamera( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsVec ); // view
	p_AddParameter( init.clsVec ); // up
}
void deClassMatrix4::nfNewCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decVector &position = clsVec.GetVector( objPosition );
	const decVector &view = clsVec.GetVector( objView );
	const decVector &up = clsVec.GetVector( objUp );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateCamera( position, view, up ) );
}

// public static func Matrix newWorld( Vector position, Vector view, Vector up )
deClassMatrix4::nfNewWorld::nfNewWorld( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsVec ); // view
	p_AddParameter( init.clsVec ); // up
}
void deClassMatrix4::nfNewWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decVector &position = clsVec.GetVector( objPosition );
	const decVector &view = clsVec.GetVector( objView );
	const decVector &up = clsVec.GetVector( objUp );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateWorld( position, view, up ) );
}

// public static func Matrix newWorld( Vector position, Quaternion orientation )
deClassMatrix4::nfNewWorld2::nfNewWorld2( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsVec ); // position
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassMatrix4::nfNewWorld2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassVector &clsVec = *ds.GetClassVector();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objOrientation = rt->GetValue( 1 )->GetRealObject();
	
	const decVector &position = clsVec.GetVector( objPosition );
	const decQuaternion &orientation = clsQuat.GetQuaternion( objOrientation );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateWorld( position, orientation ) );
}

// public static func Matrix newProjection( int width, int height, float fov, float fovRatio, float znear, float zfar )
deClassMatrix4::nfNewProjection::nfNewProjection( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newProjection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsInt ); // width
	p_AddParameter( init.clsInt ); // height
	p_AddParameter( init.clsFlt ); // fov
	p_AddParameter( init.clsFlt ); // fovRatio
	p_AddParameter( init.clsFlt ); // znear
	p_AddParameter( init.clsFlt ); // zfar
}
void deClassMatrix4::nfNewProjection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const int width = rt->GetValue( 0 )->GetInt();
	const int height = rt->GetValue( 1 )->GetInt();
	const float fov = rt->GetValue( 2 )->GetFloat() * DEG2RAD;
	const float fovRatio = rt->GetValue( 3 )->GetFloat();
	const float znear = rt->GetValue( 4 )->GetFloat();
	const float zfar = rt->GetValue( 5 )->GetFloat();
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateProjection( width, height, fov, fovRatio, znear, zfar ) );
}

// public static func Matrix newOrtho( int left, int right, int top, int bottom, float znear, float zfar )
deClassMatrix4::nfNewOrtho::nfNewOrtho( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newProjection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsInt ); // left
	p_AddParameter( init.clsInt ); // right
	p_AddParameter( init.clsInt ); // top
	p_AddParameter( init.clsInt ); // bottom
	p_AddParameter( init.clsInt ); // znear
	p_AddParameter( init.clsInt ); // zfar
}
void deClassMatrix4::nfNewOrtho::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const int left = rt->GetValue( 0 )->GetInt();
	const int right = rt->GetValue( 1 )->GetInt();
	const int top = rt->GetValue( 2 )->GetInt();
	const int bottom = rt->GetValue( 3 )->GetInt();
	const int znear = rt->GetValue( 4 )->GetInt();
	const int zfar = rt->GetValue( 5 )->GetInt();
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateOrtho( left, right, top, bottom, znear, zfar ) );
}

// public static func Matrix newFromQuaternion( Quaternion quaternion )
deClassMatrix4::nfNewFromQuaternion::nfNewFromQuaternion( const sInitData &init ) : dsFunction( init.clsMatrix4,
"newFromQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsQuat ); // quaternion
}
void deClassMatrix4::nfNewFromQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objQuaternion = rt->GetValue( 0 )->GetRealObject();
	
	const decQuaternion &quaternion = clsQuat.GetQuaternion( objQuaternion );
	
	clsMatrix.PushMatrix4( rt, decMatrix::CreateFromQuaternion( quaternion ) );
}

// public func destructor()
deClassMatrix4::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsMatrix4,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMatrix4::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Accessors
//////////////

// public func float getAt( int row, int col )
deClassMatrix4::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // row
	p_AddParameter( init.clsInt ); // col
}
void deClassMatrix4::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const int row = rt->GetValue( 0 )->GetInt();
	const int col = rt->GetValue( 1 )->GetInt();
	
	if( row == 0 ){
		if( col == 0 ){
			rt->PushFloat( matrix.a11 );
			
		}else if( col == 1 ){
			rt->PushFloat( matrix.a12 );
			
		}else if( col == 2 ){
			rt->PushFloat( matrix.a13 );
			
		}else if( col == 3 ){
			rt->PushFloat( matrix.a14 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 1 ){
		if( col == 0 ){
			rt->PushFloat( matrix.a21 );
			
		}else if( col == 1 ){
			rt->PushFloat( matrix.a22 );
			
		}else if( col == 2 ){
			rt->PushFloat( matrix.a23 );
			
		}else if( col == 3 ){
			rt->PushFloat( matrix.a24 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 2 ){
		if( col == 0 ){
			rt->PushFloat( matrix.a31 );
			
		}else if( col == 1 ){
			rt->PushFloat( matrix.a32 );
			
		}else if( col == 2 ){
			rt->PushFloat( matrix.a33 );
			
		}else if( col == 3 ){
			rt->PushFloat( matrix.a34 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 3 ){
		if( col == 0 ){
			rt->PushFloat( matrix.a41 );
			
		}else if( col == 1 ){
			rt->PushFloat( matrix.a42 );
			
		}else if( col == 2 ){
			rt->PushFloat( matrix.a43 );
			
		}else if( col == 3 ){
			rt->PushFloat( matrix.a44 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float get11()
deClassMatrix4::nfGet11::nfGet11( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet11::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a11 );
}

// public func float get12()
deClassMatrix4::nfGet12::nfGet12( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet12::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a12 );
}

// public func float get13()
deClassMatrix4::nfGet13::nfGet13( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet13::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a13 );
}

// public func float get14()
deClassMatrix4::nfGet14::nfGet14( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get14", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet14::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a14 );
}

// public func float get21()
deClassMatrix4::nfGet21::nfGet21( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet21::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a21 );
}

// public func float get22()
deClassMatrix4::nfGet22::nfGet22( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet22::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a22 );
}

// public func float get23()
deClassMatrix4::nfGet23::nfGet23( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet23::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a23 );
}

// public func float get24()
deClassMatrix4::nfGet24::nfGet24( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get24", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet24::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a24 );
}

// public func float get31()
deClassMatrix4::nfGet31::nfGet31( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet31::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a31 );
}

// public func float get32()
deClassMatrix4::nfGet32::nfGet32( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet32::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a32 );
}

// public func float get33()
deClassMatrix4::nfGet33::nfGet33( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet33::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a33 );
}

// public func float get34()
deClassMatrix4::nfGet34::nfGet34( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get34", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet34::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a34 );
}

// public func float get41()
deClassMatrix4::nfGet41::nfGet41( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get41", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet41::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a41 );
}

// public func float get42()
deClassMatrix4::nfGet42::nfGet42( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get42", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet42::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a42 );
}

// public func float get43()
deClassMatrix4::nfGet43::nfGet43( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get43", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet43::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a43 );
}

// public func float get44()
deClassMatrix4::nfGet44::nfGet44( const sInitData &init ) : dsFunction( init.clsMatrix4,
"get44", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMatrix4::nfGet44::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a44 );
}

// public func Vector getViewVector()
deClassMatrix4::nfGetViewVector::nfGetViewVector( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getViewVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMatrix4::nfGetViewVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector( rt, decVector( matrix.a13, matrix.a23, matrix.a33 ) );
}

// public func Vector getUpVector()
deClassMatrix4::nfGetUpVector::nfGetUpVector( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getUpVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMatrix4::nfGetUpVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector( rt, decVector( matrix.a12, matrix.a22, matrix.a32 ) );
}

// public func Vector getRightVector()
deClassMatrix4::nfGetRightVector::nfGetRightVector( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getRightVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMatrix4::nfGetRightVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector( rt, decVector( matrix.a11, matrix.a21, matrix.a31 ) );
}

// public func Vector getPosition()
deClassMatrix4::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMatrix4::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector( rt, decVector( matrix.a14, matrix.a24, matrix.a34 ) );
}

// public func Vector transformNormal( Vector normal )
deClassMatrix4::nfTransformNormal::nfTransformNormal( const sInitData &init ) : dsFunction( init.clsMatrix4,
"transformNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // normal
}
void deClassMatrix4::nfTransformNormal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objNormal = rt->GetValue( 0 )->GetRealObject();
	
	const decVector &normal = clsVec.GetVector( objNormal );
	
	clsVec.PushVector( rt, matrix.TransformNormal( normal ) );
}

// public func Vector getEulerAngles()
deClassMatrix4::nfGetEulerAngles::nfGetEulerAngles( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getEulerAngles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMatrix4::nfGetEulerAngles::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	
	clsVec.PushVector( rt, matrix.GetEulerAngles() / DEG2RAD );
}

// public func Matrix getInverse()
deClassMatrix4::nfGetInverse::nfGetInverse( const sInitData &init ) : dsFunction( init.clsMatrix4,
"getInverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
}
void deClassMatrix4::nfGetInverse::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	
	clsMatrix.PushMatrix4( rt, matrix.Invert() );
}

// public func Matrix4 normalize()
deClassMatrix4::nfNormalize::nfNormalize( const sInitData &init ) :
dsFunction( init.clsMatrix4, "normalize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
}
void deClassMatrix4::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix &clsMatrix = *( ( deClassMatrix* )GetOwnerClass() );
	clsMatrix.PushMatrix( rt, matrix.Normalized() );
}

// public func Quaternion toQuaternion()
deClassMatrix4::nfToQuaternion::nfToQuaternion( const sInitData &init ) : dsFunction( init.clsMatrix4,
"toQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassMatrix4::nfToQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	
	clsQuat.PushQuaternion( rt, matrix.ToQuaternion() );
}

// public func DMatrix4 toDMatrix4()
deClassMatrix4::nfToDMatrix4::nfToDMatrix4( const sInitData &init ) : dsFunction( init.clsMatrix4,
"toDMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
}
void deClassMatrix4::nfToDMatrix4::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassMatrix4* )GetOwnerClass() )->GetDS();
	deClassDMatrix4 &clsDMatrix4 = *ds.GetClassDMatrix4();
	
	clsDMatrix4.PushDMatrix4( rt, decDMatrix( matrix ) );
}

// public func DMatrix toDMatrix()
deClassMatrix4::nfToDMatrix::nfToDMatrix( const sInitData &init ) : dsFunction( init.clsMatrix4,
"toDMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
}
void deClassMatrix4::nfToDMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassMatrix4* )GetOwnerClass() )->GetDS();
	deClassDMatrix &clsDMatrix = *ds.GetClassDMatrix();
	
	clsDMatrix.PushDMatrix( rt, decDMatrix( matrix ) );
}

// public func Matrix toMatrix()
deClassMatrix4::nfToMatrix::nfToMatrix( const sInitData &init ) : dsFunction( init.clsMatrix4,
"toMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix ){
}
void deClassMatrix4::nfToMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassMatrix4* )GetOwnerClass() )->GetDS();
	deClassMatrix4 &clsMatrix4 = *ds.GetClassMatrix4();
	
	clsMatrix4.PushMatrix4( rt, matrix );
}



// File Handling
//////////////////

// static public func Matrix readFromFile( FileReader reader )
deClassMatrix4::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsMatrix4,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsMatrix4 ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassMatrix4::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsMatrix.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decMatrix matrix;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	float * const values = &matrix.a11;
	int i;
	for( i=0; i<16; i++ ){
		values[ i ] = reader->ReadFloat();
	}
	
	clsMatrix.PushMatrix4( rt, matrix );
}

// public func void writeToFile( FileWriter writer )
deClassMatrix4::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsMatrix4,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassMatrix4::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsMatrix.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	const float * const values = &matrix.a11;
	int i;
	for( i=0; i<16; i++ ){
		writer->WriteFloat( values[ i ] );
	}
}



// Operators
//////////////

// public func Matrix +( Matrix marix )
deClassMatrix4::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsMatrix4,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsMatrix4 ); // matrix
}
void deClassMatrix4::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix4( objMatrix );
	
	clsMatrix.PushMatrix4( rt, matrix + otherMatrix );
}

// public func Matrix -( Matrix matrix )
deClassMatrix4::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsMatrix4,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsMatrix4 ); // matrix
}
void deClassMatrix4::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix4( objMatrix );
	
	clsMatrix.PushMatrix4( rt, matrix - otherMatrix );
}

// public func Matrix *( float k )
deClassMatrix4::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassMatrix4::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float k = rt->GetValue( 0 )->GetFloat();
	
	clsMatrix.PushMatrix4( rt, matrix * k );
}

// public func Matrix /( float k )
deClassMatrix4::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsMatrix4,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassMatrix4::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const float k = rt->GetValue( 0 )->GetFloat();
	
	clsMatrix.PushMatrix4( rt, matrix / k );
}

// public func Matrix *( Matrix matrix )
deClassMatrix4::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
	p_AddParameter( init.clsMatrix4 ); // matrix
}
void deClassMatrix4::nfOpMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decMatrix &otherMatrix = clsMatrix.GetMatrix4( objMatrix );
	
	clsMatrix.PushMatrix4( rt, matrix * otherMatrix );
}

// public func Matrix *( Vector vector )
deClassMatrix4::nfOpMultiply2::nfOpMultiply2( const sInitData &init ) : dsFunction( init.clsMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // vector
}
void deClassMatrix4::nfOpMultiply2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 &clsMatrix = *( ( deClassMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	deClassVector &clsVec = *ds.GetClassVector();
	dsRealObject * const objVector = rt->GetValue( 0 )->GetRealObject();
	
	const decVector &vector = clsVec.GetVector( objVector );
	
	clsVec.PushVector( rt, matrix * vector );
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassMatrix4::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsMatrix4,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassMatrix4::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassMatrix4 * const clsMatrix = ( deClassMatrix4* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsMatrix ) ){
		rt->PushBool( false );
		
	}else{
		const decMatrix &otherMatrix = ( ( sMatNatDat* )p_GetNativeData( obj ) )->matrix;
		const bool equal = matrix.a11 == otherMatrix.a11
			&& matrix.a12 == otherMatrix.a12
			&& matrix.a13 == otherMatrix.a13
			&& matrix.a14 == otherMatrix.a14
			&& matrix.a21 == otherMatrix.a21
			&& matrix.a22 == otherMatrix.a22
			&& matrix.a23 == otherMatrix.a23
			&& matrix.a24 == otherMatrix.a24
			&& matrix.a31 == otherMatrix.a31
			&& matrix.a32 == otherMatrix.a32
			&& matrix.a33 == otherMatrix.a33
			&& matrix.a34 == otherMatrix.a34
			&& matrix.a41 == otherMatrix.a41
			&& matrix.a42 == otherMatrix.a42
			&& matrix.a43 == otherMatrix.a43
			&& matrix.a44 == otherMatrix.a44;
		rt->PushBool( equal );
	}
}

// public func int hashCode()
deClassMatrix4::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsMatrix4,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassMatrix4::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const int hash = ( int )( matrix.a11 * 0x0001 )
		+ ( int )( matrix.a12 * 0x0002 )
		+ ( int )( matrix.a13 * 0x0004 )
		+ ( int )( matrix.a14 * 0x0008 )
		+ ( int )( matrix.a21 * 0x0010 )
		+ ( int )( matrix.a22 * 0x0020 )
		+ ( int )( matrix.a23 * 0x0040 )
		+ ( int )( matrix.a24 * 0x0080 )
		+ ( int )( matrix.a31 * 0x0100 )
		+ ( int )( matrix.a32 * 0x0200 )
		+ ( int )( matrix.a33 * 0x0400 )
		+ ( int )( matrix.a34 * 0x0800 )
		+ ( int )( matrix.a41 * 0x1000 )
		+ ( int )( matrix.a42 * 0x2000 )
		+ ( int )( matrix.a43 * 0x4000 )
		+ ( int )( matrix.a44 * 0x8000 );
	rt->PushInt( hash );
}

// public func String toString()
deClassMatrix4::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsMatrix4,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassMatrix4::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( "[[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a41, matrix.a42, matrix.a43, matrix.a44 );
	
	rt->PushString( str );
}


// public func String nfToStringPrecision()
deClassMatrix4::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsMatrix4, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassMatrix4::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 9 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 9" );
	}
	
	const unsigned short p = ( unsigned short )precision;
	char format[ 90 ];
	snprintf( format, sizeof( format ), "[[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf]]",
		p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p );
	
	const decMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( format,
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a41, matrix.a42, matrix.a43, matrix.a44 );
	
	rt->PushString( str );
}



// Class deClassMatrix4
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassMatrix4::deClassMatrix4( deScriptingDragonScript *ds ) :
dsClass( "Matrix4", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMatNatDat ) );
}

deClassMatrix4::~deClassMatrix4(){
}



// Management
///////////////

void deClassMatrix4::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsMatrix4 = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsStr = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	init.clsDMatrix4 = pDS->GetClassDMatrix4();
	init.clsDMatrix = pDS->GetClassDMatrix();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	init.clsMatrix = pDS->GetClassMatrix();
	init.clsQuat = pDS->GetClassQuaternion();
	init.clsVec = pDS->GetClassVector();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewValues( init ) );
	AddFunction( new nfNewTranslation( init ) );
	AddFunction( new nfNewTranslation2( init ) );
	AddFunction( new nfNewScaling( init ) );
	AddFunction( new nfNewScaling2( init ) );
	AddFunction( new nfNewRotationX( init ) );
	AddFunction( new nfNewRotationY( init ) );
	AddFunction( new nfNewRotationZ( init ) );
	AddFunction( new nfNewRotation( init ) );
	AddFunction( new nfNewRotation2( init ) );
	AddFunction( new nfNewRotationAxis( init ) );
	AddFunction( new nfNewSRT( init ) );
	AddFunction( new nfNewRT( init ) );
	AddFunction( new nfNewSVUT( init ) );
	AddFunction( new nfNewVU( init ) );
	AddFunction( new nfNewCamera( init ) );
	AddFunction( new nfNewWorld( init ) );
	AddFunction( new nfNewWorld2( init ) );
	AddFunction( new nfNewProjection( init ) );
	AddFunction( new nfNewOrtho( init ) );
	AddFunction( new nfNewFromQuaternion( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetAt( init ) );
	AddFunction( new nfGet11( init ) );
	AddFunction( new nfGet12( init ) );
	AddFunction( new nfGet13( init ) );
	AddFunction( new nfGet14( init ) );
	AddFunction( new nfGet21( init ) );
	AddFunction( new nfGet22( init ) );
	AddFunction( new nfGet23( init ) );
	AddFunction( new nfGet24( init ) );
	AddFunction( new nfGet31( init ) );
	AddFunction( new nfGet32( init ) );
	AddFunction( new nfGet33( init ) );
	AddFunction( new nfGet34( init ) );
	AddFunction( new nfGet41( init ) );
	AddFunction( new nfGet42( init ) );
	AddFunction( new nfGet43( init ) );
	AddFunction( new nfGet44( init ) );
	AddFunction( new nfGetViewVector( init ) );
	AddFunction( new nfGetUpVector( init ) );
	AddFunction( new nfGetRightVector( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfTransformNormal( init ) );
	AddFunction( new nfGetEulerAngles( init ) );
	AddFunction( new nfGetInverse( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfToQuaternion( init ) );
	AddFunction( new nfToDMatrix4( init ) );
	AddFunction( new nfToDMatrix( init ) );
	AddFunction( new nfToMatrix( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpSubtract( init ) );
	AddFunction( new nfOpScale( init ) );
	AddFunction( new nfOpDivide( init ) );
	AddFunction( new nfOpMultiply( init ) );
	AddFunction( new nfOpMultiply2( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	AddFunction( new nfToStringPrecision( init ) );
}

const decMatrix &deClassMatrix4::GetMatrix4( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decMatrix & )( ( sMatNatDat* )p_GetNativeData( myself->GetBuffer() ) )->matrix;
}

void deClassMatrix4::PushMatrix4( dsRunTime *rt, const decMatrix &matrix ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMatNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->matrix = matrix;
}
