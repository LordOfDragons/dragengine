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

#include "deClassMatrix.h"
#include "deClassMatrix4.h"
#include "deClassDMatrix.h"
#include "deClassDMatrix4.h"
#include "deClassDVector.h"
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
	decDMatrix matrix;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassDMatrix::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsDMatrix,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassDMatrix::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.SetIdentity();
}

// public func new( float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24,
// float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44 )
deClassDMatrix::nfNewValues::nfNewValues( const sInitData &init ) : dsFunction( init.clsDMatrix,
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
void deClassDMatrix::nfNewValues::RunFunction( dsRunTime *rt, dsValue *myself ){
	decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.a11 = ( double )rt->GetValue( 0 )->GetFloat();
	matrix.a12 = ( double )rt->GetValue( 1 )->GetFloat();
	matrix.a13 = ( double )rt->GetValue( 2 )->GetFloat();
	matrix.a14 = ( double )rt->GetValue( 3 )->GetFloat();
	matrix.a21 = ( double )rt->GetValue( 4 )->GetFloat();
	matrix.a22 = ( double )rt->GetValue( 5 )->GetFloat();
	matrix.a23 = ( double )rt->GetValue( 6 )->GetFloat();
	matrix.a24 = ( double )rt->GetValue( 7 )->GetFloat();
	matrix.a31 = ( double )rt->GetValue( 8 )->GetFloat();
	matrix.a32 = ( double )rt->GetValue( 9 )->GetFloat();
	matrix.a33 = ( double )rt->GetValue( 10 )->GetFloat();
	matrix.a34 = ( double )rt->GetValue( 11 )->GetFloat();
	matrix.a41 = ( double )rt->GetValue( 12 )->GetFloat();
	matrix.a42 = ( double )rt->GetValue( 13 )->GetFloat();
	matrix.a43 = ( double )rt->GetValue( 14 )->GetFloat();
	matrix.a44 = ( double )rt->GetValue( 15 )->GetFloat();
}

// public static func DMatrix newTranslation( DVector translation )
deClassDMatrix::nfNewTranslation::nfNewTranslation( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix::nfNewTranslation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objTranslation = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateTranslation( translation ) );
}

// public static func DMatrix newTranslation( float tx, float ty, float tz )
deClassDMatrix::nfNewTranslation2::nfNewTranslation2( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // tx
	p_AddParameter( init.clsFlt ); // ty
	p_AddParameter( init.clsFlt ); // tz
}
void deClassDMatrix::nfNewTranslation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double tx = ( double )rt->GetValue( 0 )->GetFloat();
	const double ty = ( double )rt->GetValue( 1 )->GetFloat();
	const double tz = ( double )rt->GetValue( 2 )->GetFloat();
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateTranslation( tx, ty, tz ) );
}

// public static func DMatrix newScaling( DVector scaling )
deClassDMatrix::nfNewScaling::nfNewScaling( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // scaling
}
void deClassDMatrix::nfNewScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &scaling = clsDVec.GetDVector( objScaling );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateScale( scaling ) );
}

// public static func DMatrix newScaling( float sx, float sy, float sz )
deClassDMatrix::nfNewScaling2::nfNewScaling2( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // sx
	p_AddParameter( init.clsFlt ); // sy
	p_AddParameter( init.clsFlt ); // sz
}
void deClassDMatrix::nfNewScaling2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double sx = ( double )rt->GetValue( 0 )->GetFloat();
	const double sy = ( double )rt->GetValue( 1 )->GetFloat();
	const double sz = ( double )rt->GetValue( 2 )->GetFloat();
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateScale( sx, sy, sz ) );
}

// public static func DMatrix newRotationX( float rotation )
deClassDMatrix::nfNewRotationX::nfNewRotationX( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotationX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix::nfNewRotationX::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRotationX( rotation ) );
}

// public static func DMatrix newRotationY( float rotation )
deClassDMatrix::nfNewRotationY::nfNewRotationY( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotationY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix::nfNewRotationY::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRotationY( rotation ) );
}

// public static func DMatrix newRotationZ( float rotation )
deClassDMatrix::nfNewRotationZ::nfNewRotationZ( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotationZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix::nfNewRotationZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRotationZ( rotation ) );
}

// public static func DMatrix newRotation( DVector rotation )
deClassDMatrix::nfNewRotation::nfNewRotation( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // rotation
}
void deClassDMatrix::nfNewRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRotation( rotation ) );
}

// public static func DMatrix newRotation( float rx, float ry, float rz )
deClassDMatrix::nfNewRotation2::nfNewRotation2( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // rx
	p_AddParameter( init.clsFlt ); // ry
	p_AddParameter( init.clsFlt ); // rz
}
void deClassDMatrix::nfNewRotation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double rx = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	const double ry = ( double )( rt->GetValue( 1 )->GetFloat() * DEG2RAD );
	const double rz = ( double )( rt->GetValue( 2 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRotation( rx, ry, rz ) );
}

// public static func Matrix newRotationAxis( Vector axis, float rotation )
deClassDMatrix::nfNewRotationAxis::nfNewRotationAxis( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRotationAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // axis
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix::nfNewRotationAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	
	const decDVector &axis = clsDVec.GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const float rotation = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushDMatrix( rt, decDMatrix::CreateRotationAxis( axis, ( double )rotation ) );
}

// public static func DMatrix newSRT( DVector scaling, DVector rotation, DVector translation )
deClassDMatrix::nfNewSRT::nfNewSRT( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // scaling
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix::nfNewSRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objRotation = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &scaling = clsDVec.GetDVector( objScaling );
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateSRT( scaling, rotation, translation ) );
}

// public static func DMatrix newRT( DVector rotation, DVector translation )
deClassDMatrix::nfNewRT::nfNewRT( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix::nfNewRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateRT( rotation, translation ) );
}

// public static func DMatrix newSVUT( DVector scaling, DVector view, DVector up, DVector translation )
deClassDMatrix::nfNewSVUT::nfNewSVUT( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newSVUT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix::nfNewSVUT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 3 )->GetRealObject();
	
	const decDVector &scaling = clsDVec.GetDVector( objScaling );
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateSVUT( scaling, view, up, translation ) );
}

// public static func DMatrix newVU( DVector view, DVector up )
deClassDMatrix::nfNewVU::nfNewVU( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newVU", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix::nfNewVU::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objView = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateVU( view, up ) );
}

// public static func DMatrix newCamera( DVector position, DVector view, DVector up )
deClassDMatrix::nfNewCamera::nfNewCamera( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix::nfNewCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateCamera( position, view, up ) );
}

// public static func DMatrix newWorld( DVector position, DVector view, DVector up )
deClassDMatrix::nfNewWorld::nfNewWorld( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix::nfNewWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateWorld( position, view, up ) );
}

// public static func DMatrix newWorld( DVector position, Quaternion orientation )
deClassDMatrix::nfNewWorld2::nfNewWorld2( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassDMatrix::nfNewWorld2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objOrientation = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decQuaternion &orientation = clsQuat.GetQuaternion( objOrientation );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateWorld( position, orientation ) );
}

// public static func DMatrix newProjection( int width, int height, float fov, float fovRatio, float znear, float zfar )
deClassDMatrix::nfNewProjection::nfNewProjection( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newProjection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsInt ); // width
	p_AddParameter( init.clsInt ); // height
	p_AddParameter( init.clsFlt ); // fov
	p_AddParameter( init.clsFlt ); // fovRatio
	p_AddParameter( init.clsFlt ); // znear
	p_AddParameter( init.clsFlt ); // zfar
}
void deClassDMatrix::nfNewProjection::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const int width = rt->GetValue( 0 )->GetInt();
	const int height = rt->GetValue( 1 )->GetInt();
	const double fov = ( double )( rt->GetValue( 2 )->GetFloat() * DEG2RAD );
	const double fovRatio = ( double )rt->GetValue( 3 )->GetFloat();
	const double znear = ( double )rt->GetValue( 4 )->GetFloat();
	const double zfar = ( double )rt->GetValue( 5 )->GetFloat();
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateProjection( width, height, fov, fovRatio, znear, zfar ) );
}

// public static func DMatrix newOrtho( int left, int right, int top, int bottom, float znear, float zfar )
deClassDMatrix::nfNewOrtho::nfNewOrtho( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newProjection", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsInt ); // left
	p_AddParameter( init.clsInt ); // right
	p_AddParameter( init.clsInt ); // top
	p_AddParameter( init.clsInt ); // bottom
	p_AddParameter( init.clsInt ); // znear
	p_AddParameter( init.clsInt ); // zfar
}
void deClassDMatrix::nfNewOrtho::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const int left = rt->GetValue( 0 )->GetInt();
	const int right = rt->GetValue( 1 )->GetInt();
	const int top = rt->GetValue( 2 )->GetInt();
	const int bottom = rt->GetValue( 3 )->GetInt();
	const int znear = rt->GetValue( 4 )->GetInt();
	const int zfar = rt->GetValue( 5 )->GetInt();
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateOrtho( left, right, top, bottom, znear, zfar ) );
}

// public static func DMatrix newFromQuaternion( Quaternion quaternion )
deClassDMatrix::nfNewFromQuaternion::nfNewFromQuaternion( const sInitData &init ) : dsFunction( init.clsDMatrix,
"newFromQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsQuat ); // quaternion
}
void deClassDMatrix::nfNewFromQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objQuaternion = rt->GetValue( 0 )->GetRealObject();
	
	const decQuaternion &quaternion = clsQuat.GetQuaternion( objQuaternion );
	
	clsDMatrix.PushDMatrix( rt, decDMatrix::CreateFromQuaternion( quaternion ) );
}

// public func destructor()
deClassDMatrix::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsDMatrix,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassDMatrix::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Accessors
//////////////

// public func float getAt( int row, int col )
deClassDMatrix::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // row
	p_AddParameter( init.clsInt ); // col
}
void deClassDMatrix::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const int row = rt->GetValue( 0 )->GetInt();
	const int col = rt->GetValue( 1 )->GetInt();
	
	if( row == 0 ){
		if( col == 0 ){
			rt->PushFloat( ( float )matrix.a11 );
			
		}else if( col == 1 ){
			rt->PushFloat( ( float )matrix.a12 );
			
		}else if( col == 2 ){
			rt->PushFloat( ( float )matrix.a13 );
			
		}else if( col == 3 ){
			rt->PushFloat( ( float )matrix.a14 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 1 ){
		if( col == 0 ){
			rt->PushFloat( ( float )matrix.a21 );
			
		}else if( col == 1 ){
			rt->PushFloat( ( float )matrix.a22 );
			
		}else if( col == 2 ){
			rt->PushFloat( ( float )matrix.a23 );
			
		}else if( col == 3 ){
			rt->PushFloat( ( float )matrix.a24 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 2 ){
		if( col == 0 ){
			rt->PushFloat( ( float )matrix.a31 );
			
		}else if( col == 1 ){
			rt->PushFloat( ( float )matrix.a32 );
			
		}else if( col == 2 ){
			rt->PushFloat( ( float )matrix.a33 );
			
		}else if( col == 3 ){
			rt->PushFloat( ( float )matrix.a34 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else if( row == 3 ){
		if( col == 0 ){
			rt->PushFloat( ( float )matrix.a41 );
			
		}else if( col == 1 ){
			rt->PushFloat( ( float )matrix.a42 );
			
		}else if( col == 2 ){
			rt->PushFloat( ( float )matrix.a43 );
			
		}else if( col == 3 ){
			rt->PushFloat( ( float )matrix.a44 );
			
		}else{
			DSTHROW( dueInvalidParam );
		}
		
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float get11()
deClassDMatrix::nfGet11::nfGet11( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet11::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a11 );
}

// public func float get12()
deClassDMatrix::nfGet12::nfGet12( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet12::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a12 );
}

// public func float get13()
deClassDMatrix::nfGet13::nfGet13( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet13::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a13 );
}

// public func float get14()
deClassDMatrix::nfGet14::nfGet14( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get14", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet14::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a14 );
}

// public func float get21()
deClassDMatrix::nfGet21::nfGet21( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet21::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a21 );
}

// public func float get22()
deClassDMatrix::nfGet22::nfGet22( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet22::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a22 );
}

// public func float get23()
deClassDMatrix::nfGet23::nfGet23( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet23::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a23 );
}

// public func float get24()
deClassDMatrix::nfGet24::nfGet24( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get24", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet24::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a24 );
}

// public func float get31()
deClassDMatrix::nfGet31::nfGet31( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet31::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a31 );
}

// public func float get32()
deClassDMatrix::nfGet32::nfGet32( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet32::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a32 );
}

// public func float get33()
deClassDMatrix::nfGet33::nfGet33( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet33::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a33 );
}

// public func float get34()
deClassDMatrix::nfGet34::nfGet34( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get34", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet34::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a34 );
}

// public func float get41()
deClassDMatrix::nfGet41::nfGet41( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get41", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet41::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a41 );
}

// public func float get42()
deClassDMatrix::nfGet42::nfGet42( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get42", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet42::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a42 );
}

// public func float get43()
deClassDMatrix::nfGet43::nfGet43( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get43", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet43::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a43 );
}

// public func float get44()
deClassDMatrix::nfGet44::nfGet44( const sInitData &init ) : dsFunction( init.clsDMatrix,
"get44", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix::nfGet44::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a44 );
}

// public func DVector getViewVector()
deClassDMatrix::nfGetViewVector::nfGetViewVector( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getViewVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix::nfGetViewVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a13, matrix.a23, matrix.a33 ) );
}

// public func DVector getUpVector()
deClassDMatrix::nfGetUpVector::nfGetUpVector( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getUpVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix::nfGetUpVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a12, matrix.a22, matrix.a32 ) );
}

// public func DVector getRightVector()
deClassDMatrix::nfGetRightVector::nfGetRightVector( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getRightVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix::nfGetRightVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a11, matrix.a21, matrix.a31 ) );
}

// public func DVector getPosition()
deClassDMatrix::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a14, matrix.a24, matrix.a34 ) );
}

// public func DVector transformNormal( DVector normal )
deClassDMatrix::nfTransformNormal::nfTransformNormal( const sInitData &init ) : dsFunction( init.clsDMatrix,
"transformNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsDVec ); // normal
}
void deClassDMatrix::nfTransformNormal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objNormal = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &normal = clsDVec.GetDVector( objNormal );
	
	clsDVec.PushDVector( rt, matrix.TransformNormal( normal ) );
}

// public func DVector getEulerAngles()
deClassDMatrix::nfGetEulerAngles::nfGetEulerAngles( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getEulerAngles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix::nfGetEulerAngles::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, matrix.GetEulerAngles() / ( double )DEG2RAD );
}

// public func DMatrix getInverse()
deClassDMatrix::nfGetInverse::nfGetInverse( const sInitData &init ) : dsFunction( init.clsDMatrix,
"getInverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
}
void deClassDMatrix::nfGetInverse::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	
	clsDMatrix.PushDMatrix( rt, matrix.Invert() );
}

// public func DMatrix normalize()
deClassDMatrix::nfNormalize::nfNormalize( const sInitData &init ) :
dsFunction( init.clsDMatrix, "normalize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
}
void deClassDMatrix::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	clsDMatrix.PushDMatrix( rt, matrix.Normalized() );
}

// public func Quaternion toQuaternion()
deClassDMatrix::nfToQuaternion::nfToQuaternion( const sInitData &init ) : dsFunction( init.clsDMatrix,
"toQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassDMatrix::nfToQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	
	clsQuat.PushQuaternion( rt, matrix.ToQuaternion() );
}

// public func DMatrix4 toDMatrix4()
deClassDMatrix::nfToDMatrix4::nfToDMatrix4( const sInitData &init ) : dsFunction( init.clsDMatrix,
"toDMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
}
void deClassDMatrix::nfToDMatrix4::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix* )GetOwnerClass() )->GetDS();
	deClassDMatrix4 &clsDMatrix4 = *ds.GetClassDMatrix4();
	
	clsDMatrix4.PushDMatrix4( rt, matrix );
}

// public func Matrix4 toMatrix4()
deClassDMatrix::nfToMatrix4::nfToMatrix4( const sInitData &init ) : dsFunction( init.clsDMatrix,
"toMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
}
void deClassDMatrix::nfToMatrix4::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix* )GetOwnerClass() )->GetDS();
	deClassMatrix4 &clsMatrix4 = *ds.GetClassMatrix4();
	
	clsMatrix4.PushMatrix4( rt, matrix.ToMatrix() );
}

// public func Matrix toMatrix()
deClassDMatrix::nfToMatrix::nfToMatrix( const sInitData &init ) : dsFunction( init.clsDMatrix,
"toMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix ){
}
void deClassDMatrix::nfToMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix* )GetOwnerClass() )->GetDS();
	deClassMatrix &clsMatrix = *ds.GetClassMatrix();
	
	clsMatrix.PushMatrix( rt, matrix.ToMatrix() );
}



// File Handling
//////////////////

// static public func DMatrix readFromFile( FileReader reader )
deClassDMatrix::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsDMatrix,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsDMatrix ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassDMatrix::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsDMatrix.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decDMatrix matrix;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	double * const values = &matrix.a11;
	int i;
	for( i=0; i<16; i++ ){
		values[ i ] = reader->ReadDouble();
	}
	
	clsDMatrix.PushDMatrix( rt, matrix );
}

// public func void writeToFile( FileWriter writer )
deClassDMatrix::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsDMatrix,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassDMatrix::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsDMatrix.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	const double * const values = &matrix.a11;
	int i;
	for( i=0; i<16; i++ ){
		writer->WriteDouble( values[ i ] );
	}
}



// Operators
//////////////

// public func DMatrix +( DMatrix marix )
deClassDMatrix::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsDMatrix,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDMatrix ); // matrix
}
void deClassDMatrix::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix( objMatrix );
	
	clsDMatrix.PushDMatrix( rt, matrix + otherMatrix );
}

// public func DMatrix -( DMatrix matrix )
deClassDMatrix::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsDMatrix,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDMatrix ); // matrix
}
void deClassDMatrix::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix( objMatrix );
	
	clsDMatrix.PushDMatrix( rt, matrix - otherMatrix );
}

// public func DMatrix *( float k )
deClassDMatrix::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsDMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassDMatrix::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double k = ( double )rt->GetValue( 0 )->GetFloat();
	
	clsDMatrix.PushDMatrix( rt, matrix * k );
}

// public func DMatrix /( float k )
deClassDMatrix::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsDMatrix,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassDMatrix::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const double k = ( double )rt->GetValue( 0 )->GetFloat();
	
	clsDMatrix.PushDMatrix( rt, matrix / k );
}

// public func DMatrix *( DMatrix matrix )
deClassDMatrix::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsDMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
	p_AddParameter( init.clsDMatrix ); // matrix
}
void deClassDMatrix::nfOpMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix( objMatrix );
	
	clsDMatrix.PushDMatrix( rt, matrix * otherMatrix );
}

// public func DMatrix *( DVector vector )
deClassDMatrix::nfOpMultiply2::nfOpMultiply2( const sInitData &init ) : dsFunction( init.clsDMatrix,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsDVec ); // vector
}
void deClassDMatrix::nfOpMultiply2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix &clsDMatrix = *( ( deClassDMatrix* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objVector = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &vector = clsDVec.GetDVector( objVector );
	
	clsDVec.PushDVector( rt, matrix * vector );
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassDMatrix::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsDMatrix,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassDMatrix::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix * const clsDMatrix = ( deClassDMatrix* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsDMatrix ) ){
		rt->PushBool( false );
		
	}else{
		const decDMatrix &otherMatrix = ( ( sMatNatDat* )p_GetNativeData( obj ) )->matrix;
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
deClassDMatrix::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsDMatrix,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassDMatrix::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const int hash = ( int )( matrix.a11 * ( double )0x0001 )
		+ ( int )( matrix.a12 * ( double )0x0002 )
		+ ( int )( matrix.a13 * ( double )0x0004 )
		+ ( int )( matrix.a14 * ( double )0x0008 )
		+ ( int )( matrix.a21 * ( double )0x0010 )
		+ ( int )( matrix.a22 * ( double )0x0020 )
		+ ( int )( matrix.a23 * ( double )0x0040 )
		+ ( int )( matrix.a24 * ( double )0x0080 )
		+ ( int )( matrix.a31 * ( double )0x0100 )
		+ ( int )( matrix.a32 * ( double )0x0200 )
		+ ( int )( matrix.a33 * ( double )0x0400 )
		+ ( int )( matrix.a34 * ( double )0x0800 )
		+ ( int )( matrix.a41 * ( double )0x1000 )
		+ ( int )( matrix.a42 * ( double )0x2000 )
		+ ( int )( matrix.a43 * ( double )0x4000 )
		+ ( int )( matrix.a44 * ( double )0x8000 );
	rt->PushInt( hash );
}

// public func String toString()
deClassDMatrix::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsDMatrix,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassDMatrix::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( "[[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a41, matrix.a42, matrix.a43, matrix.a44 );
	
	rt->PushString( str );
}

// public func String toString( int precision )
deClassDMatrix::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsDMatrix, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassDMatrix::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 6 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 6" );
	}
	
	const unsigned char p = ( unsigned char )precision;
	char format[ 90 ];
	sprintf( format, "[[%%.%hhuf,%%.%hhuf,%%.%hhuf,%%.%hhuf],"
		"[%%.%hhuf,%%.%hhuf,%%.%hhuf,%%.%hhuf],"
		"[%%.%hhuf,%%.%hhuf,%%.%hhuf,%%.%hhuf],"
		"[%%.%hhuf,%%.%hhuf,%%.%hhuf,%%.%hhuf]]",
		p, p, p, p, p, p, p, p, p, p, p, p, p, p, p, p );
	
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( format,
		matrix.a11, matrix.a12, matrix.a13, matrix.a14, matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34, matrix.a41, matrix.a42, matrix.a43, matrix.a44 );
	
	rt->PushString( str );
}



// Class deClassDMatrix
/////////////////////////

// Constructor, Destructor
////////////////////////////

deClassDMatrix::deClassDMatrix( deScriptingDragonScript *ds ) :
dsClass( "DMatrix", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMatNatDat ) );
}

deClassDMatrix::~deClassDMatrix(){
}



// Management
///////////////

void deClassDMatrix::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsDMatrix = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsStr = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	init.clsDMatrix4 = pDS->GetClassDMatrix4();
	init.clsDVec = pDS->GetClassDVector();
	init.clsFileReader = pDS->GetClassFileReader();
	init.clsFileWriter = pDS->GetClassFileWriter();
	init.clsMatrix = pDS->GetClassMatrix();
	init.clsMatrix4 = pDS->GetClassMatrix4();
	init.clsQuat = pDS->GetClassQuaternion();
	
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
	AddFunction( new nfToMatrix4( init ) );
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

const decDMatrix &deClassDMatrix::GetDMatrix( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decDMatrix & )( ( sMatNatDat* )p_GetNativeData( myself->GetBuffer() ) )->matrix;
}

void deClassDMatrix::PushDMatrix( dsRunTime *rt, const decDMatrix &matrix ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMatNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->matrix = matrix;
}
