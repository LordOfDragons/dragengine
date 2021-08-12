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
deClassDMatrix4::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsDMatrix4,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassDMatrix4::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.SetIdentity();
}

// public func new( float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24,
// float a31, float a32, float a33, float a34 )
deClassDMatrix4::nfNewValues::nfNewValues( const sInitData &init ) : dsFunction( init.clsDMatrix4,
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
}
void deClassDMatrix4::nfNewValues::RunFunction( dsRunTime *rt, dsValue *myself ){
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
	matrix.a41 = 0.0;
	matrix.a42 = 0.0;
	matrix.a43 = 0.0;
	matrix.a44 = 1.0;
}

// public static func DMatrix newTranslation( DVector translation )
deClassDMatrix4::nfNewTranslation::nfNewTranslation( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix4::nfNewTranslation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objTranslation = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateTranslation( translation ) );
}

// public static func DMatrix newTranslation( float tx, float ty, float tz )
deClassDMatrix4::nfNewTranslation2::nfNewTranslation2( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // tx
	p_AddParameter( init.clsFlt ); // ty
	p_AddParameter( init.clsFlt ); // tz
}
void deClassDMatrix4::nfNewTranslation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double tx = ( double )rt->GetValue( 0 )->GetFloat();
	const double ty = ( double )rt->GetValue( 1 )->GetFloat();
	const double tz = ( double )rt->GetValue( 2 )->GetFloat();
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateTranslation( tx, ty, tz ) );
}

// public static func DMatrix newScaling( DVector scaling )
deClassDMatrix4::nfNewScaling::nfNewScaling( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // scaling
}
void deClassDMatrix4::nfNewScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &scaling = clsDVec.GetDVector( objScaling );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateScale( scaling ) );
}

// public static func DMatrix newScaling( float sx, float sy, float sz )
deClassDMatrix4::nfNewScaling2::nfNewScaling2( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // sx
	p_AddParameter( init.clsFlt ); // sy
	p_AddParameter( init.clsFlt ); // sz
}
void deClassDMatrix4::nfNewScaling2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double sx = ( double )rt->GetValue( 0 )->GetFloat();
	const double sy = ( double )rt->GetValue( 1 )->GetFloat();
	const double sz = ( double )rt->GetValue( 2 )->GetFloat();
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateScale( sx, sy, sz ) );
}

// public static func DMatrix newRotationX( float rotation )
deClassDMatrix4::nfNewRotationX::nfNewRotationX( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotationX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix4::nfNewRotationX::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRotationX( rotation ) );
}

// public static func DMatrix newRotationY( float rotation )
deClassDMatrix4::nfNewRotationY::nfNewRotationY( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotationY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix4::nfNewRotationY::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRotationY( rotation ) );
}

// public static func DMatrix newRotationZ( float rotation )
deClassDMatrix4::nfNewRotationZ::nfNewRotationZ( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotationZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix4::nfNewRotationZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double rotation = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRotationZ( rotation ) );
}

// public static func DMatrix newRotation( DVector rotation )
deClassDMatrix4::nfNewRotation::nfNewRotation( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // rotation
}
void deClassDMatrix4::nfNewRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRotation( rotation ) );
}

// public static func DMatrix newRotation( float rx, float ry, float rz )
deClassDMatrix4::nfNewRotation2::nfNewRotation2( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // rx
	p_AddParameter( init.clsFlt ); // ry
	p_AddParameter( init.clsFlt ); // rz
}
void deClassDMatrix4::nfNewRotation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double rx = ( double )( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
	const double ry = ( double )( rt->GetValue( 1 )->GetFloat() * DEG2RAD );
	const double rz = ( double )( rt->GetValue( 2 )->GetFloat() * DEG2RAD );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRotation( rx, ry, rz ) );
}

// public static func Matrix newRotationAxis( Vector axis, float rotation )
deClassDMatrix4::nfNewRotationAxis::nfNewRotationAxis( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRotationAxis", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // axis
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassDMatrix4::nfNewRotationAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	
	const decDVector &axis = clsDVec.GetDVector( rt->GetValue( 0 )->GetRealObject() );
	const float rotation = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	
	clsMatrix.PushDMatrix4( rt, decDMatrix::CreateRotationAxis( axis, ( double )rotation ) );
}

// public static func DMatrix newSRT( DVector scaling, DVector rotation, DVector translation )
deClassDMatrix4::nfNewSRT::nfNewSRT( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // scaling
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix4::nfNewSRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objScaling = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objRotation = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &scaling = clsDVec.GetDVector( objScaling );
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateSRT( scaling, rotation, translation ) );
}

// public static func DMatrix newRT( DVector rotation, DVector translation )
deClassDMatrix4::nfNewRT::nfNewRT( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix4::nfNewRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objRotation = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objTranslation = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector rotation = clsDVec.GetDVector( objRotation ) * DEG2RAD;
	const decDVector &translation = clsDVec.GetDVector( objTranslation );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateRT( rotation, translation ) );
}

// public static func DMatrix newSVUT( DVector scaling, DVector view, DVector up, DVector translation )
deClassDMatrix4::nfNewSVUT::nfNewSVUT( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newSVUT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // rotation
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
	p_AddParameter( init.clsDVec ); // translation
}
void deClassDMatrix4::nfNewSVUT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
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
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateSVUT( scaling, view, up, translation ) );
}

// public static func DMatrix newVU( DVector view, DVector up )
deClassDMatrix4::nfNewVU::nfNewVU( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newVU", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix4::nfNewVU::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objView = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateVU( view, up ) );
}

// public static func DMatrix newCamera( DVector position, DVector view, DVector up )
deClassDMatrix4::nfNewCamera::nfNewCamera( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newCamera", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix4::nfNewCamera::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateCamera( position, view, up ) );
}

// public static func DMatrix newWorld( DVector position, DVector view, DVector up )
deClassDMatrix4::nfNewWorld::nfNewWorld( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsDVec ); // view
	p_AddParameter( init.clsDVec ); // up
}
void deClassDMatrix4::nfNewWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objView = rt->GetValue( 1 )->GetRealObject();
	dsRealObject * const objUp = rt->GetValue( 2 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decDVector &view = clsDVec.GetDVector( objView );
	const decDVector &up = clsDVec.GetDVector( objUp );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateWorld( position, view, up ) );
}

// public static func DMatrix newWorld( DVector position, Quaternion orientation )
deClassDMatrix4::nfNewWorld2::nfNewWorld2( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDVec ); // position
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassDMatrix4::nfNewWorld2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassDVector &clsDVec = *ds.GetClassDVector();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objPosition = rt->GetValue( 0 )->GetRealObject();
	dsRealObject * const objOrientation = rt->GetValue( 1 )->GetRealObject();
	
	const decDVector &position = clsDVec.GetDVector( objPosition );
	const decQuaternion &orientation = clsQuat.GetQuaternion( objOrientation );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateWorld( position, orientation ) );
}

// public static func DMatrix newFromQuaternion( Quaternion quaternion )
deClassDMatrix4::nfNewFromQuaternion::nfNewFromQuaternion( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"newFromQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsQuat ); // quaternion
}
void deClassDMatrix4::nfNewFromQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	const deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	dsRealObject * const objQuaternion = rt->GetValue( 0 )->GetRealObject();
	
	const decQuaternion &quaternion = clsQuat.GetQuaternion( objQuaternion );
	
	clsDMatrix.PushDMatrix4( rt, decDMatrix::CreateFromQuaternion( quaternion ) );
}

// public func destructor()
deClassDMatrix4::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsDMatrix4,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassDMatrix4::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Accessors
//////////////

// public func float getAt( int row, int col )
deClassDMatrix4::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // row
	p_AddParameter( init.clsInt ); // col
}
void deClassDMatrix4::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
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
		
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float get11()
deClassDMatrix4::nfGet11::nfGet11( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet11::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a11 );
}

// public func float get12()
deClassDMatrix4::nfGet12::nfGet12( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet12::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a12 );
}

// public func float get13()
deClassDMatrix4::nfGet13::nfGet13( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet13::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a13 );
}

// public func float get14()
deClassDMatrix4::nfGet14::nfGet14( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get14", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet14::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a14 );
}

// public func float get21()
deClassDMatrix4::nfGet21::nfGet21( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet21::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a21 );
}

// public func float get22()
deClassDMatrix4::nfGet22::nfGet22( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet22::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a22 );
}

// public func float get23()
deClassDMatrix4::nfGet23::nfGet23( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet23::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a23 );
}

// public func float get24()
deClassDMatrix4::nfGet24::nfGet24( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get24", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet24::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a24 );
}

// public func float get31()
deClassDMatrix4::nfGet31::nfGet31( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get31", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet31::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a31 );
}

// public func float get32()
deClassDMatrix4::nfGet32::nfGet32( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get32", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet32::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a32 );
}

// public func float get33()
deClassDMatrix4::nfGet33::nfGet33( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get33", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet33::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a33 );
}

// public func float get34()
deClassDMatrix4::nfGet34::nfGet34( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"get34", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassDMatrix4::nfGet34::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( ( float )matrix.a34 );
}

// public func DVector getViewVector()
deClassDMatrix4::nfGetViewVector::nfGetViewVector( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getViewVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix4::nfGetViewVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a13, matrix.a23, matrix.a33 ) );
}

// public func DVector getUpVector()
deClassDMatrix4::nfGetUpVector::nfGetUpVector( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getUpVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix4::nfGetUpVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a12, matrix.a22, matrix.a32 ) );
}

// public func DVector getRightVector()
deClassDMatrix4::nfGetRightVector::nfGetRightVector( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getRightVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix4::nfGetRightVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a11, matrix.a21, matrix.a31 ) );
}

// public func DVector getPosition()
deClassDMatrix4::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix4::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, decDVector( matrix.a14, matrix.a24, matrix.a34 ) );
}

// public func DVector transformNormal( DVector normal )
deClassDMatrix4::nfTransformNormal::nfTransformNormal( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"transformNormal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsDVec ); // normal
}
void deClassDMatrix4::nfTransformNormal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objNormal = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &normal = clsDVec.GetDVector( objNormal );
	
	clsDVec.PushDVector( rt, matrix.TransformNormal( normal ) );
}

// public func DVector getEulerAngles()
deClassDMatrix4::nfGetEulerAngles::nfGetEulerAngles( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getEulerAngles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassDMatrix4::nfGetEulerAngles::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	
	clsDVec.PushDVector( rt, matrix.GetEulerAngles() / ( double )DEG2RAD );
}

// public func DMatrix getInverse()
deClassDMatrix4::nfGetInverse::nfGetInverse( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"getInverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
}
void deClassDMatrix4::nfGetInverse::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	
	clsDMatrix.PushDMatrix4( rt, matrix.QuickInvert() );
}

// public func DMatrix4 normalize()
deClassDMatrix4::nfNormalize::nfNormalize( const sInitData &init ) :
dsFunction( init.clsDMatrix4, "normalize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
}
void deClassDMatrix4::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix4 = *( ( deClassDMatrix4* )GetOwnerClass() );
	clsDMatrix4.PushDMatrix4( rt, matrix.Normalized() );
}

// public func Quaternion toQuaternion()
deClassDMatrix4::nfToQuaternion::nfToQuaternion( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"toQuaternion", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassDMatrix4::nfToQuaternion::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassQuaternion &clsQuat = *ds.GetClassQuaternion();
	
	clsQuat.PushQuaternion( rt, matrix.ToQuaternion() );
}

// public func DMatrix toDMatrix()
deClassDMatrix4::nfToDMatrix::nfToDMatrix( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"toDMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix ){
}
void deClassDMatrix4::nfToDMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix4* )GetOwnerClass() )->GetDS();
	deClassDMatrix &clsDMatrix = *ds.GetClassDMatrix();
	
	clsDMatrix.PushDMatrix( rt, matrix );
}

// public func Matrix4 toMatrix4()
deClassDMatrix4::nfToMatrix4::nfToMatrix4( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"toMatrix4", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix4 ){
}
void deClassDMatrix4::nfToMatrix4::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix4* )GetOwnerClass() )->GetDS();
	deClassMatrix4 &clsMatrix4 = *ds.GetClassMatrix4();
	
	clsMatrix4.PushMatrix4( rt, matrix.ToMatrix() );
}

// public func Matrix toMatrix()
deClassDMatrix4::nfToMatrix::nfToMatrix( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"toMatrix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix ){
}
void deClassDMatrix4::nfToMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deScriptingDragonScript &ds = *( ( deClassDMatrix4* )GetOwnerClass() )->GetDS();
	deClassMatrix &clsMatrix = *ds.GetClassMatrix();
	
	clsMatrix.PushMatrix( rt, matrix.ToMatrix() );
}



// File Handling
//////////////////

// static public func DMatrix readFromFile( FileReader reader )
deClassDMatrix4::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsDMatrix4 ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassDMatrix4::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsDMatrix.GetDS()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decDMatrix matrix;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	double * const values = &matrix.a11;
	int i;
	for( i=0; i<12; i++ ){
		values[ i ] = reader->ReadDouble();
	}
	
	clsDMatrix.PushDMatrix4( rt, matrix );
}

// public func void writeToFile( FileWriter writer )
deClassDMatrix4::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassDMatrix4::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsDMatrix.GetDS()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	const double * const values = &matrix.a11;
	int i;
	for( i=0; i<12; i++ ){
		writer->WriteDouble( values[ i ] );
	}
}



// Operators
//////////////

// public func DMatrix +( DMatrix marix )
deClassDMatrix4::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDMatrix4 ); // matrix
}
void deClassDMatrix4::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix4( objMatrix );
	
	clsDMatrix.PushDMatrix4( rt, matrix + otherMatrix );
}

// public func DMatrix -( DMatrix matrix )
deClassDMatrix4::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDMatrix4 ); // matrix
}
void deClassDMatrix4::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix4( objMatrix );
	
	clsDMatrix.PushDMatrix4( rt, matrix - otherMatrix );
}

// public func DMatrix *( float k )
deClassDMatrix4::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassDMatrix4::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double k = ( double )rt->GetValue( 0 )->GetFloat();
	
	clsDMatrix.PushDMatrix4( rt, matrix.QuickMultiply( k ) );
}

// public func DMatrix /( float k )
deClassDMatrix4::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassDMatrix4::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const double k = ( double )rt->GetValue( 0 )->GetFloat();
	
	clsDMatrix.PushDMatrix4( rt, matrix / k );
}

// public func DMatrix *( DMatrix matrix )
deClassDMatrix4::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDMatrix4 ){
	p_AddParameter( init.clsDMatrix4 ); // matrix
}
void deClassDMatrix4::nfOpMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	dsRealObject * const objMatrix = rt->GetValue( 0 )->GetRealObject();
	
	const decDMatrix &otherMatrix = clsDMatrix.GetDMatrix4( objMatrix );
	
	clsDMatrix.PushDMatrix4( rt, matrix.QuickMultiply( otherMatrix ) );
}

// public func DMatrix *( DVector vector )
deClassDMatrix4::nfOpMultiply2::nfOpMultiply2( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsDVec ); // vector
}
void deClassDMatrix4::nfOpMultiply2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 &clsDMatrix = *( ( deClassDMatrix4* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsDMatrix.GetDS();
	deClassDVector &clsDVec = *ds.GetClassDVector();
	dsRealObject * const objVector = rt->GetValue( 0 )->GetRealObject();
	
	const decDVector &vector = clsDVec.GetDVector( objVector );
	
	clsDVec.PushDVector( rt, matrix * vector );
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassDMatrix4::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassDMatrix4::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassDMatrix4 * const clsDMatrix = ( deClassDMatrix4* )GetOwnerClass();
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
			&& matrix.a34 == otherMatrix.a34;
		rt->PushBool( equal );
	}
}

// public func int hashCode()
deClassDMatrix4::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassDMatrix4::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
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
		+ ( int )( matrix.a34 * ( double )0x0800 );
	rt->PushInt( hash );
}

// public func String toString()
deClassDMatrix4::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsDMatrix4,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassDMatrix4::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( "[[%g,%g,%g,%g],[%g,%g,%g,%g],[%g,%g,%g,%g]]",
		matrix.a11, matrix.a12, matrix.a13, matrix.a14,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34 );
	
	rt->PushString( str );
}

// public func String toString( int precision )
deClassDMatrix4::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsDMatrix4, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassDMatrix4::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 6 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 6" );
	}
	
	const unsigned short p = ( unsigned short )precision;
	char format[ 68 ];
	sprintf( format, "[[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf],"
		"[%%.%huf,%%.%huf,%%.%huf,%%.%huf]]",
		p, p, p, p, p, p, p, p, p, p, p, p );
	
	const decDMatrix &matrix = ( ( sMatNatDat* )p_GetNativeData( myself ) )->matrix;
	decString str;
	
	str.Format( format,
		matrix.a11, matrix.a12, matrix.a13, matrix.a14,
		matrix.a21, matrix.a22, matrix.a23, matrix.a24,
		matrix.a31, matrix.a32, matrix.a33, matrix.a34 );
	
	rt->PushString( str );
}



// Class deClassDMatrix4
/////////////////////////

// Constructor, Destructor
////////////////////////////

deClassDMatrix4::deClassDMatrix4( deScriptingDragonScript *ds ) :
dsClass( "DMatrix4", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMatNatDat ) );
}

deClassDMatrix4::~deClassDMatrix4(){
}



// Management
///////////////

void deClassDMatrix4::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsDMatrix4 = this;
	
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsObj = engine->GetClassObject();
	init.clsStr = engine->GetClassString();
	init.clsVoid = engine->GetClassVoid();
	
	init.clsDMatrix = pDS->GetClassDMatrix();
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
	AddFunction( new nfGetViewVector( init ) );
	AddFunction( new nfGetUpVector( init ) );
	AddFunction( new nfGetRightVector( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfTransformNormal( init ) );
	AddFunction( new nfGetEulerAngles( init ) );
	AddFunction( new nfGetInverse( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfToQuaternion( init ) );
	AddFunction( new nfToDMatrix( init ) );
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

const decDMatrix &deClassDMatrix4::GetDMatrix4( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decDMatrix & )( ( sMatNatDat* )p_GetNativeData( myself->GetBuffer() ) )->matrix;
}

void deClassDMatrix4::PushDMatrix4( dsRunTime *rt, const decDMatrix &matrix ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMatNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->matrix = matrix;
}
