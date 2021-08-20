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
#include <libdscript/libdscript.h>

#include "deClassQuaternion.h"
#include "deClassDVector.h"
#include "deClassVector.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sQuatNatDat{
	decQuaternion quaternion;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new()
deClassQuaternion::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsQuat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassQuaternion::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	quaternion.SetZero();
}

// public func new( float x, float y, float z, float w )
deClassQuaternion::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsQuat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // y
	p_AddParameter( init.clsFlt ); // z
	p_AddParameter( init.clsFlt ); // w
}
void deClassQuaternion::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	
	quaternion.x = rt->GetValue( 0 )->GetFloat();
	quaternion.y = rt->GetValue( 1 )->GetFloat();
	quaternion.z = rt->GetValue( 2 )->GetFloat();
	quaternion.w = rt->GetValue( 3 )->GetFloat();
}

// public func new( Quaternion q )
deClassQuaternion::nfNew3::nfNew3( const sInitData &init ) : dsFunction( init.clsQuat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // q
}
void deClassQuaternion::nfNew3::RunFunction( dsRunTime *rt, dsValue *myself ){
	decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	
	quaternion.Set( clsQuaternion.GetQuaternion( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func destructor()
deClassQuaternion::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsQuat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassQuaternion::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}

// static public func Quaternion newFromEuler( float rx, float ry, float rz )
deClassQuaternion::nfNewFromEuler::nfNewFromEuler( const sInitData &init ) : dsFunction( init.clsQuat,
"newFromEuler", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // rx
	p_AddParameter( init.clsFlt ); // ry
	p_AddParameter( init.clsFlt ); // rz
}
void deClassQuaternion::nfNewFromEuler::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	
	const float rx = rt->GetValue( 0 )->GetFloat() * DEG2RAD;
	const float ry = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	const float rz = rt->GetValue( 2 )->GetFloat() * DEG2RAD;
	
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromEuler( rx, ry, rz ) );
}

// static public func Quaternion newFromEuler( Vector angles )
deClassQuaternion::nfNewFromEuler2::nfNewFromEuler2( const sInitData &init ) : dsFunction( init.clsQuat,
"newFromEuler", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsVec ); // angles
}
void deClassQuaternion::nfNewFromEuler2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsQuaternion.GetScriptModule();
	
	const decVector &angles = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromEuler( angles * DEG2RAD ) );
}

// static public func Quaternion newFromEulerX( float angle )
deClassQuaternion::nfNewFromEulerX::nfNewFromEulerX( const sInitData &init ) : dsFunction( init.clsQuat,
"newFromEulerX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // angle
}
void deClassQuaternion::nfNewFromEulerX::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromEulerX( rt->GetValue( 0 )->GetFloat() * DEG2RAD ) );
}

// static public func Quaternion newFromEulerY( float angle )
deClassQuaternion::nfNewFromEulerY::nfNewFromEulerY( const sInitData &init ) : dsFunction( init.clsQuat,
"newFromEulerY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // angle
}
void deClassQuaternion::nfNewFromEulerY::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromEulerY( rt->GetValue( 0 )->GetFloat() * DEG2RAD ) );
}

// static public func Quaternion newFromEulerZ( float angle )
deClassQuaternion::nfNewFromEulerZ::nfNewFromEulerZ( const sInitData &init ) : dsFunction( init.clsQuat,
"newFromEulerZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // angle
}
void deClassQuaternion::nfNewFromEulerZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromEulerZ( rt->GetValue( 0 )->GetFloat() * DEG2RAD ) );
}

// static public func Quaternion newFromAxis( Vector axis, float angle )
deClassQuaternion::nfNewFromAxis::nfNewFromAxis( const sInitData &init ) :
dsFunction( init.clsQuat, "newFromAxis", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsVec ); // axis
	p_AddParameter( init.clsFlt ); // angle
}
void deClassQuaternion::nfNewFromAxis::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsQuaternion.GetScriptModule();
	
	const decVector &axis = ds.GetClassVector()->GetVector( rt->GetValue( 0 )->GetRealObject() );
	const float angle = rt->GetValue( 1 )->GetFloat() * DEG2RAD;
	
	clsQuaternion.PushQuaternion( rt, decQuaternion::CreateFromAxis( axis, angle ) );
}



// accessors
//////////////

// public func float getX()
deClassQuaternion::nfGetX::nfGetX( const sInitData &init ) : dsFunction( init.clsQuat,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassQuaternion::nfGetX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushFloat( quaternion.x );
}

// public func float getY()
deClassQuaternion::nfGetY::nfGetY( const sInitData &init ) : dsFunction( init.clsQuat,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassQuaternion::nfGetY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushFloat( quaternion.y );
}

// public func float getZ()
deClassQuaternion::nfGetZ::nfGetZ( const sInitData &init ) : dsFunction( init.clsQuat,
"getZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassQuaternion::nfGetZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushFloat( quaternion.z );
}

// public func float getW()
deClassQuaternion::nfGetW::nfGetW( const sInitData &init ) : dsFunction( init.clsQuat,
"getW", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassQuaternion::nfGetW::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushFloat( quaternion.w );
}

// public func float get( int component )
deClassQuaternion::nfGet::nfGet( const sInitData &init ) : dsFunction( init.clsQuat,
"get", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // component
}
void deClassQuaternion::nfGet::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	const int component = rt->GetValue( 0 )->GetInt();
	
	if( component == 0 ){
		rt->PushFloat( quaternion.x );
		
	}else if( component == 1 ){
		rt->PushFloat( quaternion.y );
		
	}else if( component == 2 ){
		rt->PushFloat( quaternion.z );
		
	}else if( component == 3 ){
		rt->PushFloat( quaternion.w );
		
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float getLength()
deClassQuaternion::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsQuat,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassQuaternion::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushFloat( quaternion.Length() );
}

// public func Quaternion normalize()
deClassQuaternion::nfNormalize::nfNormalize( const sInitData &init ) : dsFunction( init.clsQuat,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassQuaternion::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, quaternion.Normalized() );
}

// public func Quaternion conjugate()
deClassQuaternion::nfConjugate::nfConjugate( const sInitData &init ) : dsFunction( init.clsQuat,
"conjugate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassQuaternion::nfConjugate::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, quaternion.Conjugate() );
}

// public func Quaternion absolute()
deClassQuaternion::nfAbsolute::nfAbsolute( const sInitData &init ) : dsFunction( init.clsQuat,
"absolute", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassQuaternion::nfAbsolute::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	clsQuaternion.PushQuaternion( rt, quaternion.Absolute() );
}

// public func Vector getEulerAngles()
deClassQuaternion::nfGetEulerAngles::nfGetEulerAngles( const sInitData &init ) : dsFunction( init.clsQuat,
"getEulerAngles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassQuaternion::nfGetEulerAngles::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	const deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *clsQuaternion.GetScriptModule();
	
	ds.GetClassVector()->PushVector( rt, quaternion.GetEulerAngles() / DEG2RAD );
}

// public func Quaternion slerp( Quaternion other, float factor )
deClassQuaternion::nfSlerp::nfSlerp( const sInitData &init ) : dsFunction( init.clsQuat,
"slerp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsQuat ); // other
	p_AddParameter( init.clsFlt ); // factor
}
void deClassQuaternion::nfSlerp::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	
	const decQuaternion &other = clsQuaternion.GetQuaternion( rt->GetValue( 0 )->GetRealObject() );
	const float factor = rt->GetValue( 1 )->GetFloat();
	
	clsQuaternion.PushQuaternion( rt, quaternion.Slerp( other, factor ) );
}



// testing
////////////

// public func bool isEqualTo( Quaternion q, float delta )
deClassQuaternion::nfIsEqualTo::nfIsEqualTo( const sInitData &init ) : dsFunction( init.clsQuat,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsQuat ); // q
	p_AddParameter( init.clsFlt ); // delta
}
void deClassQuaternion::nfIsEqualTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	dsRealObject *objQuat = rt->GetValue( 0 )->GetRealObject();
	if( ! objQuat ) DSTHROW( dueNullPointer );
	float delta = rt->GetValue( 1 )->GetFloat();
	rt->PushBool( quaternion.IsEqualTo( clsQuaternion->GetQuaternion( objQuat ), delta ) );
}



// File Handling
//////////////////

// static public func Quaternion readFromFile( FileReader reader )
deClassQuaternion::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsQuat,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsQuat ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassQuaternion::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsQuaternion.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decQuaternion quaternion;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	clsQuaternion.PushQuaternion( rt, reader->ReadQuaternion() );
}

// public func void writeToFile( FileWriter writer )
deClassQuaternion::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsQuat,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassQuaternion::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	const deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsQuaternion.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteQuaternion( quaternion );
}



// operators
//////////////
		
// public func Quaternion -()
deClassQuaternion::nfOpMinus::nfOpMinus( const sInitData &init ) : dsFunction( init.clsQuat,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassQuaternion::nfOpMinus::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	clsQuaternion->PushQuaternion( rt, -quaternion );
}

// public func Quaternion +( Quaternion q )
deClassQuaternion::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsQuat,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsQuat ); // q
}
void deClassQuaternion::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	dsRealObject *objQuat = rt->GetValue( 0 )->GetRealObject();
	if( ! objQuat ) DSTHROW( dueNullPointer );
	clsQuaternion->PushQuaternion( rt, quaternion + clsQuaternion->GetQuaternion( objQuat ) );
}

// public func Quaternion -( Quaternion q )
deClassQuaternion::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsQuat,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsQuat ); // q
}
void deClassQuaternion::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	dsRealObject *objQuat = rt->GetValue( 0 )->GetRealObject();
	if( ! objQuat ) DSTHROW( dueNullPointer );
	clsQuaternion->PushQuaternion( rt, quaternion - clsQuaternion->GetQuaternion( objQuat ) );
}

// public func Quaternion *( float k )
deClassQuaternion::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsQuat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassQuaternion::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	clsQuaternion->PushQuaternion( rt, quaternion * rt->GetValue( 0 )->GetFloat() );
}

// public func Quaternion /( float k )
deClassQuaternion::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsQuat,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassQuaternion::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	clsQuaternion->PushQuaternion( rt, quaternion / rt->GetValue( 0 )->GetFloat() );
}

// public func Quaternion *( Quaternion q )
deClassQuaternion::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsQuat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
	p_AddParameter( init.clsQuat ); // q
}
void deClassQuaternion::nfOpMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	
	clsQuaternion->PushQuaternion( rt, quaternion * clsQuaternion->GetQuaternion( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Vector *( Vector v )
deClassQuaternion::nfOpMultiplyVector::nfOpMultiplyVector( const sInitData &init ) : dsFunction( init.clsQuat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassQuaternion::nfOpMultiplyVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *( ( deScriptingDragonScript* )clsQuaternion.GetScriptModule() );
	deClassVector &clsVector = *ds.GetClassVector();
	
	const decVector &vector = clsVector.GetVector( rt->GetValue( 0 )->GetRealObject() );
	clsVector.PushVector( rt, quaternion * vector );
}

// public func DVector *( DVector v )
deClassQuaternion::nfOpMultiplyDVector::nfOpMultiplyDVector( const sInitData &init ) : dsFunction( init.clsQuat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
	p_AddParameter( init.clsDVec ); // v
}
void deClassQuaternion::nfOpMultiplyDVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion &clsQuaternion = *( ( deClassQuaternion* )GetOwnerClass() );
	const deScriptingDragonScript &ds = *( ( deScriptingDragonScript* )clsQuaternion.GetScriptModule() );
	deClassDVector &clsDVector = *ds.GetClassDVector();
	
	const decDVector &vector = clsDVector.GetDVector( rt->GetValue( 0 )->GetRealObject() );
	clsDVector.PushDVector( rt, quaternion * vector );
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassQuaternion::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsQuat,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassQuaternion::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	deClassQuaternion *clsQuaternion = ( deClassQuaternion* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsQuaternion ) ){
		rt->PushBool( false );
	}else{
		const decQuaternion &otherQuaternion = ( ( sQuatNatDat* )p_GetNativeData( obj ) )->quaternion;
		rt->PushBool( quaternion.IsEqualTo( otherQuaternion ) );
	}
}

// public func int hashCode()
deClassQuaternion::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsQuat,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassQuaternion::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	rt->PushInt( ( int )( quaternion.x * 1000000 )
		+ ( int )( quaternion.y * 10000 )
		+ ( int )( quaternion.z * 100 )
		+ ( int )( quaternion.w * 1 ) );
}

// public func String toString()
deClassQuaternion::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsQuat,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassQuaternion::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	decString str;
	str.Format( "(%g,%g,%g,%g)", quaternion.x, quaternion.y, quaternion.z, quaternion.w );
	rt->PushString( str );
}

// public func String toString( int precision )
deClassQuaternion::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsQuat, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassQuaternion::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 9 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 9" );
	}
	
	const unsigned short p = ( unsigned short )precision;
	char format[ 22 ];
	sprintf( format, "(%%.%huf,%%.%huf,%%.%huf,%%.%huf)", p, p, p, p );
	
	const decQuaternion &quaternion = ( ( sQuatNatDat* )p_GetNativeData( myself ) )->quaternion;
	decString str;
	str.Format( format, quaternion.x, quaternion.y, quaternion.z, quaternion.w );
	rt->PushString( str );
}



// class deClassQuaternion
////////////////////////
// constructor
deClassQuaternion::deClassQuaternion( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "Quaternion", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sQuatNatDat ) );
}
deClassQuaternion::~deClassQuaternion(){
}
// management
void deClassQuaternion::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsQuat = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	
	init.clsVec = pScrMgr->GetClassVector();
	init.clsDVec = pScrMgr->GetClassDVector();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfNew3( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfNewFromEuler( init ) );
	AddFunction( new nfNewFromEuler2( init ) );
	AddFunction( new nfNewFromEulerX( init ) );
	AddFunction( new nfNewFromEulerY( init ) );
	AddFunction( new nfNewFromEulerZ( init ) );
	AddFunction( new nfNewFromAxis( init ) );
	
	AddFunction( new nfGetX( init ) );
	AddFunction( new nfGetY( init ) );
	AddFunction( new nfGetZ( init ) );
	AddFunction( new nfGetW( init ) );
	AddFunction( new nfGet( init ) );
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfConjugate( init ) );
	AddFunction( new nfAbsolute( init ) );
	AddFunction( new nfGetEulerAngles( init ) );
	AddFunction( new nfSlerp( init ) );
	
	AddFunction( new nfIsEqualTo( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpMinus( init ) );
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpSubtract( init ) );
	AddFunction( new nfOpScale( init ) );
	AddFunction( new nfOpDivide( init ) );
	AddFunction( new nfOpMultiply( init ) );
	AddFunction( new nfOpMultiplyVector( init ) );
	AddFunction( new nfOpMultiplyDVector( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	AddFunction( new nfToStringPrecision( init ) );
}

const decQuaternion &deClassQuaternion::GetQuaternion( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decQuaternion & )( ( sQuatNatDat* )p_GetNativeData( myself->GetBuffer() ) )->quaternion;
}

void deClassQuaternion::PushQuaternion( dsRunTime *rt, const decQuaternion &quaternion ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sQuatNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->quaternion = quaternion;
}
