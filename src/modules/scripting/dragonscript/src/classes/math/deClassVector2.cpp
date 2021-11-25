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

#include "deClassPoint.h"
#include "deClassVector2.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sVec2NatDat{
	decVector2 vector;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassVector2::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsVec2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVector2::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	vector.Set( 0.0f, 0.0f );
}

// public func new( float x, float y )
deClassVector2::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsVec2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // y
}
void deClassVector2::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	vector.x = rt->GetValue( 0 )->GetFloat();
	vector.y = rt->GetValue( 1 )->GetFloat();
}

// public func new( Vector2 v )
deClassVector2::nfNew3::nfNew3( const sInitData &init ) : dsFunction( init.clsVec2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfNew3::RunFunction( dsRunTime *rt, dsValue *myself ){
	decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	vector = clsVector2->GetVector2( objVec );
}

// public func new( Point p )
deClassVector2::nfNew4::nfNew4( const sInitData &init ) : dsFunction( init.clsVec2,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPoint ); // p
}
void deClassVector2::nfNew4::RunFunction( dsRunTime *rt, dsValue *myself ){
	decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deScriptingDragonScript &ds = *( ( ( deClassVector2* )GetOwnerClass() )->GetScriptModule() );
	
	vector = decVector2( ds.GetClassPoint()->GetPoint( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func destructor()
deClassVector2::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsVec2,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVector2::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Management
///////////////

// public func float getX()
deClassVector2::nfGetX::nfGetX( const sInitData &init ) : dsFunction( init.clsVec2,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector2::nfGetX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	rt->PushFloat( vector.x );
}

// public func float getY()
deClassVector2::nfGetY::nfGetY( const sInitData &init ) : dsFunction( init.clsVec2,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector2::nfGetY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	rt->PushFloat( vector.y );
}

// public func float getComponentAt( int component )
deClassVector2::nfGetComponentAt::nfGetComponentAt( const sInitData &init ) : dsFunction( init.clsVec2,
"getComponentAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // component
}
void deClassVector2::nfGetComponentAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	int component = rt->GetValue( 0 )->GetInt();
	
	if( component == 0 ){
		rt->PushFloat( vector.x );
		
	}else if( component == 1 ){
		rt->PushFloat( vector.y );
		
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func float getLength()
deClassVector2::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsVec2,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVector2::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	rt->PushFloat( vector.Length() );
}

// public func Vector2 normalize()
deClassVector2::nfNormalize::nfNormalize( const sInitData &init ) : dsFunction( init.clsVec2,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
}
void deClassVector2::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	float len = vector.Length();
	
	if( fabsf( len ) < -1e-6 ){
		clsVector2->PushVector2( rt, vector / len );
		
	}else{
		DSTHROW( dueDivisionByZero );
	}
}

// public func Vector2 absolute()
deClassVector2::nfAbsolute::nfAbsolute( const sInitData &init ) : dsFunction( init.clsVec2,
"absolute", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
}
void deClassVector2::nfAbsolute::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	clsVector2->PushVector2( rt, vector.Absolute() );
}

// public func Vector2 smallest( Vector2 v )
deClassVector2::nfSmallest::nfSmallest( const sInitData &init ) : dsFunction( init.clsVec2,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfSmallest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const decVector2 &otherVector2 = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	
	clsVector2.PushVector2( rt, vector.Smallest( otherVector2 ) );
}

// public func Vector2 largest( Vector2 v )
deClassVector2::nfLargest::nfLargest( const sInitData &init ) : dsFunction( init.clsVec2,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfLargest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const decVector2 &otherVector2 = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	
	clsVector2.PushVector2( rt, vector.Largest( otherVector2 ) );
}

// public func Vector2 clamped( Vector2 min, Vector2 max )
deClassVector2::nfClamped::nfClamped( const sInitData &init ) : dsFunction( init.clsVec2,
"clamped", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // min
	p_AddParameter( init.clsVec2 ); // max
}
void deClassVector2::nfClamped::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const decVector2 &min = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	const decVector2 &max = clsVector2.GetVector2( rt->GetValue( 1 )->GetRealObject() );
	
	clsVector2.PushVector2( rt, vector.Clamped( min, max ) );
}

// public func Point round()
deClassVector2::nfRound::nfRound( const sInitData &init ) : dsFunction( init.clsVec2,
"round", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint ){
}
void deClassVector2::nfRound::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deScriptingDragonScript &ds = *( ( deClassVector2* )GetOwnerClass() )->GetScriptModule();
	ds.GetClassPoint()->PushPoint( rt, vector.Round() );
}

// public func Vector2 round(float unit)
deClassVector2::nfRound2::nfRound2( const sInitData &init ) :
dsFunction( init.clsVec2, "round", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsFlt ); // unit
}
void deClassVector2::nfRound2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decVector2 vector( ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector );
	deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const float unit = rt->GetValue( 0 )->GetFloat();
	
	vector /= unit;
	vector.x = floor( vector.x + 0.5f );
	vector.y = floor( vector.y + 0.5f );
	vector *= unit;
	clsVector2.PushVector2( rt, vector );
}



// Testing
////////////

// public func bool isEqualTo( Vector2 v, float delta )
deClassVector2::nfIsEqualTo::nfIsEqualTo( const sInitData &init ) : dsFunction( init.clsVec2,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec2 ); // v
	p_AddParameter( init.clsFlt ); // delta
}
void deClassVector2::nfIsEqualTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	const decVector2 &other = clsVector2->GetVector2( rt->GetValue( 0 )->GetRealObject() );
	const float delta = rt->GetValue( 1 )->GetFloat();
	
	rt->PushBool( vector.IsEqualTo( other, delta ) );
}

// public func bool isAtLeast( float value )
deClassVector2::nfIsAtLeast::nfIsAtLeast( const sInitData &init ) : dsFunction( init.clsVec2,
"isAtLeast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassVector2::nfIsAtLeast::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	float value = rt->GetValue( 0 )->GetFloat();
	rt->PushBool( vector.x >= value && vector.y >= value );
}

// public func bool isAtMost( float value )
deClassVector2::nfIsAtMost::nfIsAtMost( const sInitData &init ) : dsFunction( init.clsVec2,
"isAtMost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassVector2::nfIsAtMost::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	float value = rt->GetValue( 0 )->GetFloat();
	rt->PushBool( vector.x <= value && vector.y <= value );
}

// public func bool isZero()
deClassVector2::nfIsZero::nfIsZero( const sInitData &init ) :
dsFunction( init.clsVec2, "isZero", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVector2::nfIsZero::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	rt->PushBool( vector.IsEqualTo( decVector2() ) );
}



// File Handling
//////////////////

// static public func Vector2 readFromFile( FileReader reader )
deClassVector2::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsVec2,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVec2 ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassVector2::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsVector2.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decVector2 vector;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	clsVector2.PushVector2( rt, reader->ReadVector2() );
}

// public func void writeToFile( FileWriter writer )
deClassVector2::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsVec2,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassVector2::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	const deClassVector2 &clsVector2 = *( ( deClassVector2* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsVector2.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteVector2( vector );
}



// Operators
//////////////
		
// public func Vector2 -()
deClassVector2::nfOpMinus::nfOpMinus( const sInitData &init ) : dsFunction( init.clsVec2,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
}
void deClassVector2::nfOpMinus::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	clsVector2->PushVector2( rt, -vector );
}

// public func Vector2 +( Vector2 v )
deClassVector2::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsVec2,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	clsVector2->PushVector2( rt, vector + other );
}

// public func Vector2 -( Vector2 v )
deClassVector2::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsVec2,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	clsVector2->PushVector2( rt, vector - other );
}

// public func Vector2 *( float k )
deClassVector2::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsVec2,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassVector2::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	clsVector2->PushVector2( rt, vector * rt->GetValue( 0 )->GetFloat() );
}

// public func Vector2 /( float k )
deClassVector2::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsVec2,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassVector2::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	clsVector2->PushVector2( rt, vector / rt->GetValue( 0 )->GetFloat() );
}

// public func float *( Vector2 v )
deClassVector2::nfOpDot::nfOpDot( const sInitData &init ) : dsFunction( init.clsVec2,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpDot::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	rt->PushFloat( vector * other );
}

// public func bool <( Vector2 v )
deClassVector2::nfOpLess::nfOpLess( const sInitData &init ) : dsFunction( init.clsVec2,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpLess::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	rt->PushBool( vector < other );
}

// public func bool <=( Vector2 v )
deClassVector2::nfOpLessEqual::nfOpLessEqual( const sInitData &init ) : dsFunction( init.clsVec2,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpLessEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	rt->PushBool( vector <= other );
}

// public func bool >( Vector2 v )
deClassVector2::nfOpGreater::nfOpGreater( const sInitData &init ) : dsFunction( init.clsVec2,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpGreater::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	rt->PushBool( vector > other );
}

// public func bool >=( Vector2 v )
deClassVector2::nfOpGreaterEqual::nfOpGreaterEqual( const sInitData &init ) : dsFunction( init.clsVec2,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsVec2 ); // v
}
void deClassVector2::nfOpGreaterEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	
	dsRealObject *objVec = rt->GetValue( 0 )->GetRealObject();
	if( ! objVec ) DSTHROW( dueNullPointer );
	
	const decVector2 &other = clsVector2->GetVector2( objVec );
	
	rt->PushBool( vector >= other );
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassVector2::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsVec2,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassVector2::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	deClassVector2 *clsVector2 = ( deClassVector2* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsVector2 ) ){
		rt->PushBool( false );
	}else{
		const decVector2 &otherVector2 = ( ( sVec2NatDat* )p_GetNativeData( obj ) )->vector;
		rt->PushBool( vector.IsEqualTo( otherVector2 ) );
	}
}

// public func int hashCode()
deClassVector2::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsVec2,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassVector2::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	rt->PushInt( ( int )( vector.x * 1000000 ) + ( int )( vector.y * 1000 ) );
}

// public func String toString()
deClassVector2::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsVec2,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassVector2::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	decString str;
	str.Format( "(%f,%f)", vector.x, vector.y );
	rt->PushString( str );
}

// public func String toString( int precision )
deClassVector2::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsVec2, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassVector2::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 9 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 9" );
	}
	
	const unsigned short p = ( unsigned short )precision;
	char format[ 12 ];
	sprintf( format, "(%%.%huf,%%.%huf)", p, p );
	
	const decVector2 &vector = ( ( sVec2NatDat* )p_GetNativeData( myself ) )->vector;
	decString str;
	str.Format( format, vector.x, vector.y );
	rt->PushString( str );
}



// Class deClassVector2
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassVector2::deClassVector2( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "Vector2", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sVec2NatDat ) );
}

deClassVector2::~deClassVector2(){
}



// Management
///////////////

void deClassVector2::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsVec2 = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	init.clsPoint = pScrMgr->GetClassPoint();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfNew3( init ) );
	AddFunction( new nfNew4( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetX( init ) );
	AddFunction( new nfGetY( init ) );
	AddFunction( new nfGetComponentAt( init ) );
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfAbsolute( init ) );
	AddFunction( new nfSmallest( init ) );
	AddFunction( new nfLargest( init ) );
	AddFunction( new nfClamped( init ) );
	AddFunction( new nfRound( init ) );
	AddFunction( new nfRound2( init ) );
	
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
	AddFunction( new nfOpLess( init ) );
	AddFunction( new nfOpLessEqual( init ) );
	AddFunction( new nfOpGreater( init ) );
	AddFunction( new nfOpGreaterEqual( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	AddFunction( new nfToStringPrecision( init ) );
}

const decVector2 &deClassVector2::GetVector2( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( ( sVec2NatDat* )p_GetNativeData( myself->GetBuffer() ) )->vector;
}

void deClassVector2::PushVector2( dsRunTime *rt, const decVector2 &vector ){
	if( ! rt ) DSTHROW( dueInvalidParam );
	
	rt->CreateObjectNakedOnStack( this );
	( ( sVec2NatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->vector = vector;
}
