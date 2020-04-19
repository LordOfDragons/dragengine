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
struct sVecNatDat{
	decVector vector;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new()
deClassVector::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVector::nfNew::RunFunction( dsRunTime *RT, dsValue *This ){
	decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	vector.Set( 0, 0, 0 );
}

// public func new( float x, float y, float z )
deClassVector::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // y
	p_AddParameter( init.clsFlt ); // z
}
void deClassVector::nfNew2::RunFunction( dsRunTime *RT, dsValue *This ){
	decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	vector.x = RT->GetValue( 0 )->GetFloat();
	vector.y = RT->GetValue( 1 )->GetFloat();
	vector.z = RT->GetValue( 2 )->GetFloat();
}

// public func new( Vector v )
deClassVector::nfNew3::nfNew3( const sInitData &init ) : dsFunction( init.clsVec,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfNew3::RunFunction( dsRunTime *RT, dsValue *This ){
	decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	vector = clsVector->GetVector( objVec );
}

// public func destructor()
deClassVector::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsVec,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVector::nfDestructor::RunFunction( dsRunTime *RT, dsValue *This ){
}



// accessors
//////////////

// public func float getX()
deClassVector::nfGetX::nfGetX( const sInitData &init ) : dsFunction( init.clsVec,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector::nfGetX::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushFloat( vector.x );
}

// public func float getY()
deClassVector::nfGetY::nfGetY( const sInitData &init ) : dsFunction( init.clsVec,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector::nfGetY::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushFloat( vector.y );
}

// public func float getZ()
deClassVector::nfGetZ::nfGetZ( const sInitData &init ) : dsFunction( init.clsVec,
"getZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector::nfGetZ::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushFloat( vector.z );
}

// public func float get( int component )
deClassVector::nfGet::nfGet( const sInitData &init ) : dsFunction( init.clsVec,
"get", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // component
}
void deClassVector::nfGet::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	int component = RT->GetValue( 0 )->GetInt();
	if( component == 0 ){
		RT->PushFloat( vector.x );
	}else if( component == 1 ){
		RT->PushFloat( vector.y );
	}else if( component == 2 ){
		RT->PushFloat( vector.z );
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float getLength()
deClassVector::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsVec,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector::nfGetLength::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushFloat( vector.Length() );
}

// public func float getLengthSquared()
deClassVector::nfGetLengthSquared::nfGetLengthSquared( const sInitData &init ) : dsFunction( init.clsVec,
"getLengthSquared", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector::nfGetLengthSquared::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushFloat( vector.LengthSquared() );
}

// public func Vector normalize()
deClassVector::nfNormalize::nfNormalize( const sInitData &init ) : dsFunction( init.clsVec,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassVector::nfNormalize::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const float len = vector.Length();
	
	if( len > 0.00001f ){
		clsVector.PushVector( RT, vector / len );
		
	}else{
		DSTHROW( dueDivisionByZero );
	}
}

// public func Vector compMultiply( Vector v )
deClassVector::nfCompMultiply::nfCompMultiply( const sInitData &init ) : dsFunction( init.clsVec,
"compMultiply", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfCompMultiply::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	const decVector &otherVector = clsVector->GetVector( objVec );
	clsVector->PushVector( RT, decVector( vector.x * otherVector.x, vector.y * otherVector.y, vector.z * otherVector.z ) );
}

// public func Vector compDivide( Vector v )
deClassVector::nfCompDivide::nfCompDivide( const sInitData &init ) : dsFunction( init.clsVec,
"compDivide", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfCompDivide::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	const decVector &otherVector = clsVector->GetVector( objVec );
	clsVector->PushVector( RT, decVector( vector.x / otherVector.x, vector.y / otherVector.y, vector.z / otherVector.z ) );
}

// public func Vector compSelect( bool x, bool y, bool z )
deClassVector::nfCompSelect::nfCompSelect( const sInitData &init ) : dsFunction( init.clsVec,
"compSelect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsBool ); // x
	p_AddParameter( init.clsBool ); // y
	p_AddParameter( init.clsBool ); // z
}
void deClassVector::nfCompSelect::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	
	decVector result;
	
	if( rt->GetValue( 0 )->GetBool() ){
		result.x = vector.x;
	}
	if( rt->GetValue( 1 )->GetBool() ){
		result.y = vector.y;
	}
	if( rt->GetValue( 2 )->GetBool() ){
		result.z = vector.z;
	}
	
	clsVector.PushVector( rt, result );
}

// public func DVector combine( DVector vector, bool x, bool y, bool z )
deClassVector::nfCombine::nfCombine( const sInitData &init ) : dsFunction( init.clsVec,
"combine", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // vector
	p_AddParameter( init.clsBool ); // x
	p_AddParameter( init.clsBool ); // y
	p_AddParameter( init.clsBool ); // z
}
void deClassVector::nfCombine::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	
	const decVector &otherVector = clsVector.GetVector( rt->GetValue( 0 )->GetRealObject() );
	const bool x = rt->GetValue( 1 )->GetBool();
	const bool y = rt->GetValue( 2 )->GetBool();
	const bool z = rt->GetValue( 3 )->GetBool();
	
	clsVector.PushVector( rt, decVector(
		x ? vector.x : otherVector.x,
		y ? vector.y : otherVector.y,
		z ? vector.z : otherVector.z
	) );
}

// public func Vector smallest( Vector v )
deClassVector::nfSmallest::nfSmallest( const sInitData &init ) : dsFunction( init.clsVec,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfSmallest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const decVector &otherVector = clsVector.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	clsVector.PushVector( rt, vector.Smallest( otherVector ) );
}

// public func Vector largest( Vector v )
deClassVector::nfLargest::nfLargest( const sInitData &init ) : dsFunction( init.clsVec,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfLargest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const decVector &otherVector = clsVector.GetVector( rt->GetValue( 0 )->GetRealObject() );
	
	clsVector.PushVector( rt, vector.Largest( otherVector ) );
}

// public func Vector clamped( Vector min, Vector max )
deClassVector::nfClamped::nfClamped( const sInitData &init ) : dsFunction( init.clsVec,
"clamped", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // min
	p_AddParameter( init.clsVec ); // max
}
void deClassVector::nfClamped::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const decVector &min = clsVector.GetVector( rt->GetValue( 0 )->GetRealObject() );
	const decVector &max = clsVector.GetVector( rt->GetValue( 1 )->GetRealObject() );
	
	clsVector.PushVector( rt, vector.Clamped( min, max ) );
}



// testing
////////////

// public func bool isEqualTo( Vector v, float delta )
deClassVector::nfIsEqualTo::nfIsEqualTo( const sInitData &init ) : dsFunction( init.clsVec,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec ); // v
	p_AddParameter( init.clsFlt ); // delta
}
void deClassVector::nfIsEqualTo::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	float delta = RT->GetValue( 1 )->GetFloat();
	RT->PushBool( vector.IsEqualTo( clsVector->GetVector( objVec ), delta ) );
}

// public func bool isAtLeast( float value )
deClassVector::nfIsAtLeast::nfIsAtLeast( const sInitData &init ) : dsFunction( init.clsVec,
"isAtLeast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassVector::nfIsAtLeast::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	float value = RT->GetValue( 0 )->GetFloat();
	RT->PushBool( vector.x >= value && vector.y >= value && vector.z >= value );
}

// public func bool isAtMost( float value )
deClassVector::nfIsAtMost::nfIsAtMost( const sInitData &init ) : dsFunction( init.clsVec,
"isAtMost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassVector::nfIsAtMost::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	float value = RT->GetValue( 0 )->GetFloat();
	RT->PushBool( vector.x <= value && vector.y <= value && vector.z <= value );
}

// public func bool isZero()
deClassVector::nfIsZero::nfIsZero( const sInitData &init ) :
dsFunction( init.clsVec, "isZero", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVector::nfIsZero::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushBool( vector.IsZero() );
}



// File Handling
//////////////////

// static public func Vector readFromFile( FileReader reader )
deClassVector::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsVec,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVec ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassVector::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsVector.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decVector vector;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	clsVector.PushVector( rt, reader->ReadVector() );
}

// public func void writeToFile( FileWriter writer )
deClassVector::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsVec,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassVector::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( myself ) )->vector;
	const deClassVector &clsVector = *( ( deClassVector* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsVector.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteVector( vector );
}



// operators
//////////////

// public func Vector -()
deClassVector::nfOpMinus::nfOpMinus( const sInitData &init ) : dsFunction( init.clsVec,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassVector::nfOpMinus::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	clsVector->PushVector( RT, -vector );
}

// public func Vector +( Vector v )
deClassVector::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsVec,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpAdd::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	clsVector->PushVector( RT, vector + clsVector->GetVector( objVec ) );
}

// public func Vector -( Vector v )
deClassVector::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsVec,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpSubtract::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	clsVector->PushVector( RT, vector - clsVector->GetVector( objVec ) );
}

// public func Vector *( float k )
deClassVector::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsVec,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassVector::nfOpScale::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	clsVector->PushVector( RT, vector * RT->GetValue( 0 )->GetFloat() );
}

// public func Vector /( float k )
deClassVector::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsVec,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassVector::nfOpDivide::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	clsVector->PushVector( RT, vector / RT->GetValue( 0 )->GetFloat() );
}

// public func float *( Vector v )
deClassVector::nfOpDot::nfOpDot( const sInitData &init ) : dsFunction( init.clsVec,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpDot::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	RT->PushFloat( vector * clsVector->GetVector( objVec ) );
}

// public func Vector %( Vector v )
deClassVector::nfOpCross::nfOpCross( const sInitData &init ) : dsFunction( init.clsVec,
"%", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpCross::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	clsVector->PushVector( RT, vector % clsVector->GetVector( objVec ) );
}

// public func bool <( Vector v )
deClassVector::nfOpLess::nfOpLess( const sInitData &init ) : dsFunction( init.clsVec,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpLess::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	RT->PushBool( vector < clsVector->GetVector( objVec ) );
}

// public func bool <=( Vector v )
deClassVector::nfOpLessEqual::nfOpLessEqual( const sInitData &init ) : dsFunction( init.clsVec,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpLessEqual::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	RT->PushBool( vector <= clsVector->GetVector( objVec ) );
}

// public func bool >( Vector v )
deClassVector::nfOpGreater::nfOpGreater( const sInitData &init ) : dsFunction( init.clsVec,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpGreater::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	RT->PushBool( vector > clsVector->GetVector( objVec ) );
}

// public func bool >=( Vector v )
deClassVector::nfOpGreaterEqual::nfOpGreaterEqual( const sInitData &init ) : dsFunction( init.clsVec,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec ); // v
}
void deClassVector::nfOpGreaterEqual::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsRealObject *objVec = RT->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	RT->PushBool( vector >= clsVector->GetVector( objVec ) );
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassVector::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsVec,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassVector::nfEquals::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	deClassVector *clsVector = ( deClassVector* )GetOwnerClass();
	dsValue *obj = RT->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsVector ) ){
		RT->PushBool( false );
	}else{
		const decVector &otherVector = ( ( sVecNatDat* )p_GetNativeData( obj ) )->vector;
		RT->PushBool( vector.IsEqualTo( otherVector ) );
	}
}

// public func int hashCode()
deClassVector::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsVec,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassVector::nfHashCode::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	RT->PushInt( ( int )( vector.x * 1000000 )
		+ ( int )( vector.y * 10000 )
		+ ( int )( vector.z * 100 ) );
}

// public func String toString()
deClassVector::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsVec,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassVector::nfToString::RunFunction( dsRunTime *RT, dsValue *This ){
	const decVector &vector = ( ( sVecNatDat* )p_GetNativeData( This ) )->vector;
	char buffer[ 50 ];
	sprintf( ( char* )&buffer, "(%f,%f,%f)", vector.x, vector.y, vector.z );
	RT->PushString( buffer );
}



// class deClassVector
////////////////////////
// constructor
deClassVector::deClassVector( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "Vector", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sVecNatDat ) );
}
deClassVector::~deClassVector(){
}
// management
void deClassVector::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsVec = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfNew3( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetX( init ) );
	AddFunction( new nfGetY( init ) );
	AddFunction( new nfGetZ( init ) );
	AddFunction( new nfGet( init ) );
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfGetLengthSquared( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfCompMultiply( init ) );
	AddFunction( new nfCompDivide( init ) );
	AddFunction( new nfCompSelect( init ) );
	AddFunction( new nfCombine( init ) );
	AddFunction( new nfSmallest( init ) );
	AddFunction( new nfLargest( init ) );
	AddFunction( new nfClamped( init ) );
	
	AddFunction( new nfIsEqualTo( init ) );
	AddFunction( new nfIsAtLeast( init ) );
	AddFunction( new nfIsAtMost( init ) );
	AddFunction( new nfIsZero( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpMinus( init ) );
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpSubtract( init ) );
	AddFunction( new nfOpScale( init ) );
	AddFunction( new nfOpDivide( init ) );
	AddFunction( new nfOpDot( init ) );
	AddFunction( new nfOpCross( init ) );
	AddFunction( new nfOpLess( init ) );
	AddFunction( new nfOpLessEqual( init ) );
	AddFunction( new nfOpGreater( init ) );
	AddFunction( new nfOpGreaterEqual( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
}

const decVector &deClassVector::GetVector( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decVector & )( ( sVecNatDat* )p_GetNativeData( myself->GetBuffer() ) )->vector;
}

void deClassVector::PushVector( dsRunTime *rt, const decVector &vector ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	// TODO spread this code version to all script classes
	rt->CreateObjectNakedOnStack( this );
	( ( sVecNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->vector = vector;
	
	/*
	rt->CreateObjectNakedOnStack( this );
	try{
		( ( sVecNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->vector = vector;
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
	*/
}
