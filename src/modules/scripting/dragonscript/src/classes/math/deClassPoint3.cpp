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

#include "deClassPoint3.h"
#include "deClassVector.h"
#include "deClassDVector.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sPt3NatDat{
	decPoint3 point;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new()
deClassPoint3::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsPt3,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassPoint3::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	point.Set( 0, 0, 0 );
}

// public func new( int x, int y, int z )
deClassPoint3::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsPt3,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // x
	p_AddParameter( init.clsInt ); // y
	p_AddParameter( init.clsInt ); // z
}
void deClassPoint3::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	point.x = rt->GetValue( 0 )->GetInt();
	point.y = rt->GetValue( 1 )->GetInt();
	point.z = rt->GetValue( 2 )->GetInt();
}

// public func new( Point3 v )
deClassPoint3::nfNew3::nfNew3( const sInitData &init ) : dsFunction( init.clsPt3,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfNew3::RunFunction( dsRunTime *rt, dsValue *myself ){
	decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	point = clsPt3->GetPoint( objPt );
}

// public func destructor()
deClassPoint3::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsPt3,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassPoint3::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// accessors
//////////////

// public func int getX()
deClassPoint3::nfGetX::nfGetX( const sInitData &init ) : dsFunction( init.clsPt3,
"getX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassPoint3::nfGetX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushInt( point.x );
}

// public func int getY()
deClassPoint3::nfGetY::nfGetY( const sInitData &init ) : dsFunction( init.clsPt3,
"getY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassPoint3::nfGetY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushInt( point.y );
}

// public func int getZ()
deClassPoint3::nfGetZ::nfGetZ( const sInitData &init ) : dsFunction( init.clsPt3,
"getZ", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassPoint3::nfGetZ::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushInt( point.z );
}

// public func int get( int component )
deClassPoint3::nfGet::nfGet( const sInitData &init ) : dsFunction( init.clsPt3,
"get", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // component
}
void deClassPoint3::nfGet::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	int component = rt->GetValue( 0 )->GetInt();
	if( component == 0 ){
		rt->PushInt( point.x );
	}else if( component == 1 ){
		rt->PushInt( point.y );
	}else if( component == 2 ){
		rt->PushInt( point.z );
	}else{
		DSTHROW( dueInvalidParam );
	}
}

// public func int getLength()
deClassPoint3::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsPt3,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassPoint3::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushFloat( point.Length() );
}



// public func Point3 smallest( Point3 p )
deClassPoint3::nfSmallest::nfSmallest( const sInitData &init ) : dsFunction( init.clsPt3,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // p
}
void deClassPoint3::nfSmallest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	decPoint3 newPoint3( point );
	
	dsRealObject * const objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ){
		DSTHROW( dueNullPointer );
	}
	const decPoint3 &otherPoint3 = clsPoint3.GetPoint( objPt );
	
	if( otherPoint3.x < newPoint3.x ){
		newPoint3.x = otherPoint3.x;
	}
	if( otherPoint3.y < newPoint3.y ){
		newPoint3.y = otherPoint3.y;
	}
	if( otherPoint3.z < newPoint3.z ){
		newPoint3.z = otherPoint3.z;
	}
	
	clsPoint3.PushPoint( rt, newPoint3 );
}

// public func Point3 smallest( int x, int y, int z )
deClassPoint3::nfSmallest2::nfSmallest2( const sInitData &init ) : dsFunction( init.clsPt3,
"smallest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsInt ); // x
	p_AddParameter( init.clsInt ); // y
	p_AddParameter( init.clsInt ); // z
}
void deClassPoint3::nfSmallest2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	const int x = rt->GetValue( 0 )->GetInt();
	const int y = rt->GetValue( 1 )->GetInt();
	const int z = rt->GetValue( 2 )->GetInt();
	
	decPoint3 newPoint3( point );
	
	if( x < newPoint3.x ){
		newPoint3.x = x;
	}
	if( y < newPoint3.y ){
		newPoint3.y = y;
	}
	if( z < newPoint3.z ){
		newPoint3.z = z;
	}
	
	clsPoint3.PushPoint( rt, newPoint3 );
}

// public func Point3 largest( Point3 p )
deClassPoint3::nfLargest::nfLargest( const sInitData &init ) : dsFunction( init.clsPt3,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // p
}
void deClassPoint3::nfLargest::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	dsRealObject * const objPt = rt->GetValue( 0 )->GetRealObject();
	decPoint3 newPoint3( point );
	
	if( ! objPt ){
		DSTHROW( dueNullPointer );
	}
	const decPoint3 &otherPoint3 = clsPoint3.GetPoint( objPt );
	
	if( otherPoint3.x > newPoint3.x ){
		newPoint3.x = otherPoint3.x;
	}
	if( otherPoint3.y > newPoint3.y ){
		newPoint3.y = otherPoint3.y;
	}
	if( otherPoint3.z > newPoint3.z ){
		newPoint3.z = otherPoint3.z;
	}
	
	clsPoint3.PushPoint( rt, newPoint3 );
}

// public func Point3 largest( int x, int y, int z )
deClassPoint3::nfLargest2::nfLargest2( const sInitData &init ) : dsFunction( init.clsPt3,
"largest", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsInt ); // x
	p_AddParameter( init.clsInt ); // y
	p_AddParameter( init.clsInt ); // z
}
void deClassPoint3::nfLargest2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	const int x = rt->GetValue( 0 )->GetInt();
	const int y = rt->GetValue( 1 )->GetInt();
	const int z = rt->GetValue( 2 )->GetInt();
	
	decPoint3 newPoint3( point );
	
	if( x > newPoint3.x ){
		newPoint3.x = x;
	}
	if( y > newPoint3.y ){
		newPoint3.y = y;
	}
	if( z > newPoint3.z ){
		newPoint3.z = z;
	}
	
	clsPoint3.PushPoint( rt, newPoint3 );
}



// public func Point3 compMultiply( Point3 v )
deClassPoint3::nfCompMultiply::nfCompMultiply( const sInitData &init ) : dsFunction( init.clsPt3,
"compMultiply", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfCompMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	const decPoint3 &otherPoint = clsPt3->GetPoint( objPt );
	clsPt3->PushPoint( rt, decPoint3( point.x * otherPoint.x, point.y * otherPoint.y, point.z * otherPoint.z ) );
}

// public func Point3 compDivide( Point3 v )
deClassPoint3::nfCompDivide::nfCompDivide( const sInitData &init ) : dsFunction( init.clsPt3,
"compDivide", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfCompDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	const decPoint3 &otherPoint = clsPt3->GetPoint( objPt );
	clsPt3->PushPoint( rt, decPoint3( point.x / otherPoint.x, point.y / otherPoint.y, point.z / otherPoint.z ) );
}



// testing
////////////

// public func bool isEqualTo( Point3 v )
deClassPoint3::nfIsEqualTo::nfIsEqualTo( const sInitData &init ) : dsFunction( init.clsPt3,
"isEqualTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfIsEqualTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushBool( point == clsPt3->GetPoint( objPt ) );
}

// public func bool isAtLeast( int value )
deClassPoint3::nfIsAtLeast::nfIsAtLeast( const sInitData &init ) : dsFunction( init.clsPt3,
"isAtLeast", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassPoint3::nfIsAtLeast::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	int value = rt->GetValue( 0 )->GetInt();
	rt->PushBool( point.x >= value && point.y >= value && point.z >= value );
}

// public func bool isAtMost( int value )
deClassPoint3::nfIsAtMost::nfIsAtMost( const sInitData &init ) : dsFunction( init.clsPt3,
"isAtMost", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassPoint3::nfIsAtMost::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	int value = rt->GetValue( 0 )->GetInt();
	rt->PushBool( point.x <= value && point.y <= value && point.z <= value );
}

// public func bool isZero()
deClassPoint3::nfIsZero::nfIsZero( const sInitData &init ) :
dsFunction( init.clsPt3, "isZero", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassPoint3::nfIsZero::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushBool( point == decPoint3() );
}



// public func Vector toVector()
deClassPoint3::nfToVector::nfToVector( const sInitData &init ) :
dsFunction( init.clsPt3, "toVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}
void deClassPoint3::nfToVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	( ( deClassPoint3* )GetOwnerClass() )->GetScriptModule()->GetClassVector()->
		PushVector( rt, decVector( point ) );
}

// public func DVector toDVector()
deClassPoint3::nfToDVector::nfToDVector( const sInitData &init ) :
dsFunction( init.clsPt3, "toDVector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector ){
}
void deClassPoint3::nfToDVector::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	( ( deClassPoint3* )GetOwnerClass() )->GetScriptModule()->GetClassDVector()->
		PushDVector( rt, decDVector( point ) );
}



// File Handling
//////////////////

// static public func Point3 readFromFile( FileReader reader )
deClassPoint3::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsPt3,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsPt3 ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassPoint3::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsPoint3.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decPoint3 point;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	clsPoint3.PushPoint( rt, reader->ReadPoint3() );
}

// public func void writeToFile( FileWriter writer )
deClassPoint3::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsPt3,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassPoint3::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	const deClassPoint3 &clsPoint3 = *( ( deClassPoint3* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsPoint3.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WritePoint3( point );
}



// operators
//////////////
		
// public func Point3 -()
deClassPoint3::nfOpMinus::nfOpMinus( const sInitData &init ) : dsFunction( init.clsPt3,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
}
void deClassPoint3::nfOpMinus::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	clsPt3->PushPoint( rt, -point );
}

// public func Point3 +( Point3 v )
deClassPoint3::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsPt3,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	clsPt3->PushPoint( rt, point + clsPt3->GetPoint( objPt ) );
}

// public func Point3 -( Point3 v )
deClassPoint3::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsPt3,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	clsPt3->PushPoint( rt, point - clsPt3->GetPoint( objPt ) );
}

// public func Point3 *( int k )
deClassPoint3::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsPt3,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsInt ); // k
}
void deClassPoint3::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	clsPt3->PushPoint( rt, point * rt->GetValue( 0 )->GetInt() );
}

// public func Point3 /( int k )
deClassPoint3::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsPt3,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsPt3 ){
	p_AddParameter( init.clsInt ); // k
}
void deClassPoint3::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	clsPt3->PushPoint( rt, point / rt->GetValue( 0 )->GetInt() );
}

// public func int *( Point3 v )
deClassPoint3::nfOpDot::nfOpDot( const sInitData &init ) : dsFunction( init.clsPt3,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpDot::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushInt( point * clsPt3->GetPoint( objPt ) );
}

// public func bool <( Point3 v )
deClassPoint3::nfOpLess::nfOpLess( const sInitData &init ) : dsFunction( init.clsPt3,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpLess::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushBool( point < clsPt3->GetPoint( objPt ) );
}

// public func bool <=( Point3 v )
deClassPoint3::nfOpLessEqual::nfOpLessEqual( const sInitData &init ) : dsFunction( init.clsPt3,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpLessEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushBool( point <= clsPt3->GetPoint( objPt ) );
}

// public func bool >( Point3 v )
deClassPoint3::nfOpGreater::nfOpGreater( const sInitData &init ) : dsFunction( init.clsPt3,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpGreater::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushBool( point > clsPt3->GetPoint( objPt ) );
}

// public func bool >=( Point3 v )
deClassPoint3::nfOpGreaterEqual::nfOpGreaterEqual( const sInitData &init ) : dsFunction( init.clsPt3,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsPt3 ); // v
}
void deClassPoint3::nfOpGreaterEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsRealObject *objPt = rt->GetValue( 0 )->GetRealObject();
	if( ! objPt ) DSTHROW( dueNullPointer );
	rt->PushBool( point >= clsPt3->GetPoint( objPt ) );
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassPoint3::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsPt3,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassPoint3::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	deClassPoint3 *clsPt3 = ( deClassPoint3* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsPt3 ) ){
		rt->PushBool( false );
	}else{
		const decPoint3 &otherPoint = ( ( sPt3NatDat* )p_GetNativeData( obj ) )->point;
		rt->PushBool( point == otherPoint );
	}
}

// public func int hashCode()
deClassPoint3::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsPt3,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassPoint3::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	rt->PushInt( ( int )( point.x * 1000000 )
		+ ( int )( point.y * 10000 )
		+ ( int )( point.z * 100 ) );
}

// public func String toString()
deClassPoint3::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsPt3,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassPoint3::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decPoint3 &point = ( ( sPt3NatDat* )p_GetNativeData( myself ) )->point;
	char buffer[ 50 ];
	snprintf( ( char* )&buffer, sizeof( buffer ), "(%i,%i,%i)", point.x, point.y, point.z );
	rt->PushString( buffer );
}



// class deClassPoint3
////////////////////////
// constructor
deClassPoint3::deClassPoint3( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "Point3", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sPt3NatDat ) );
}
deClassPoint3::~deClassPoint3(){
}
// management
void deClassPoint3::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsPt3 = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	init.clsVector = pScrMgr->GetClassVector();
	init.clsDVector = pScrMgr->GetClassDVector();
	
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
	AddFunction( new nfSmallest( init ) );
	AddFunction( new nfSmallest2( init ) );
	AddFunction( new nfLargest( init ) );
	AddFunction( new nfLargest2( init ) );
	
	AddFunction( new nfIsEqualTo( init ) );
	AddFunction( new nfIsAtLeast( init ) );
	AddFunction( new nfIsAtMost( init ) );
	AddFunction( new nfIsZero( init ) );
	
	AddFunction( new nfToVector( init ) );
	AddFunction( new nfToDVector( init ) );
	
	AddFunction( new nfCompMultiply( init ) );
	AddFunction( new nfCompDivide( init ) );
	
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
}
const decPoint3 &deClassPoint3::GetPoint( dsRealObject *myself ) const{
	return ( const decPoint3 & )( ( sPt3NatDat* )p_GetNativeData( myself->GetBuffer() ) )->point;
}
void deClassPoint3::PushPoint( dsRunTime *rt, const decPoint3 &point ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sPt3NatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->point = point;
}
