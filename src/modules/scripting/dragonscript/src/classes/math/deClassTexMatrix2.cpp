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

#include "deClassVector2.h"
#include "deClassTexMatrix2.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <libdscript/exceptions.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Native Structure
/////////////////////

struct sTMatNatDat{
	decTexMatrix2 matrix;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassTexMatrix2::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsTexMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassTexMatrix2::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	matrix.SetIdentity();
}

// public func new( TexMatrix matrix )
deClassTexMatrix2::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsTexMat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsTexMat ); // matrix
}
void deClassTexMatrix2::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	
	const decTexMatrix2 &copy = clsTexMatrix2.GetTexMatrix( rt->GetValue( 0 )->GetRealObject() );
	
	matrix = copy;
}

// public static func TexMatrix newTranslation( Vector2 translation )
deClassTexMatrix2::nfNewTranslation::nfNewTranslation( const sInitData &init ) : dsFunction( init.clsTexMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // translation
}
void deClassTexMatrix2::nfNewTranslation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVector2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &translation = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject()  );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateTranslation( translation ) );
}

// public static func TexMatrix newTranslation( float u, float v )
deClassTexMatrix2::nfNewTranslation2::nfNewTranslation2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newTranslation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // u
	p_AddParameter( init.clsFlt ); // v
}
void deClassTexMatrix2::nfNewTranslation2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float u = rt->GetValue( 0 )->GetFloat();
	const float v = rt->GetValue( 1 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateTranslation( u, v ) );
}

// public static func TexMatrix newScaling( Vector2 scaling )
deClassTexMatrix2::nfNewScaling::nfNewScaling( const sInitData &init ) : dsFunction( init.clsTexMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // scaling
}
void deClassTexMatrix2::nfNewScaling::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVec2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &scaling = clsVec2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateScale( scaling ) );
}

// public static func TexMatrix newScaling( float u, float v )
deClassTexMatrix2::nfNewScaling2::nfNewScaling2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // u
	p_AddParameter( init.clsFlt ); // v
}
void deClassTexMatrix2::nfNewScaling2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float u = rt->GetValue( 0 )->GetFloat();
	const float v = rt->GetValue( 1 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateScale( u, v ) );
}

// public static func TexMatrix newRotation( float rotation )
deClassTexMatrix2::nfNewRotation::nfNewRotation( const sInitData &init ) : dsFunction( init.clsTexMat,
"newRotation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // rotation
}
void deClassTexMatrix2::nfNewRotation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float rotation = rt->GetValue( 0 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateRotation( rotation * DEG2RAD ) );
}

// public static func TexMatrix newShear( Vector2 shear )
deClassTexMatrix2::nfNewShear::nfNewShear( const sInitData &init ) : dsFunction( init.clsTexMat,
"newShear", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // shear
}
void deClassTexMatrix2::nfNewShear::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVec2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &shear = clsVec2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateShear( shear ) );
}

// public static func TexMatrix newShear( float u, float v )
deClassTexMatrix2::nfNewShear2::nfNewShear2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newShear", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // u
	p_AddParameter( init.clsFlt ); // v
}
void deClassTexMatrix2::nfNewShear2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float u = rt->GetValue( 0 )->GetFloat();
	const float v = rt->GetValue( 1 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateShear( u, v ) );
}

// public static func TexMatrix newST( Vector2 scaling, Vector2 translation )
deClassTexMatrix2::nfNewST::nfNewST( const sInitData &init ) : dsFunction( init.clsTexMat,
"newST", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // scaling
	p_AddParameter( init.clsVec2 ); // translation
}
void deClassTexMatrix2::nfNewST::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVector2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	const decVector2 &translation = clsVector2.GetVector2( rt->GetValue( 1 )->GetRealObject() );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateST( scaling, translation ) );
}

// public static func TexMatrix newST( float scalingU, float scalingV, float translationU, float translationV )
deClassTexMatrix2::nfNewST2::nfNewST2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newST", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // scalingU
	p_AddParameter( init.clsFlt ); // scalingV
	p_AddParameter( init.clsFlt ); // translationU
	p_AddParameter( init.clsFlt ); // translationV
}
void deClassTexMatrix2::nfNewST2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float scalingU = rt->GetValue( 0 )->GetFloat();
	const float scalingV = rt->GetValue( 1 )->GetFloat();
	const float translationU = rt->GetValue( 2 )->GetFloat();
	const float translationV = rt->GetValue( 3 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateST( scalingU, scalingV, translationU, translationV ) );
}

// public static func TexMatrix newSRT( Vector2 scaling, float rotation, Vector2 translation )
deClassTexMatrix2::nfNewSRT::nfNewSRT( const sInitData &init ) : dsFunction( init.clsTexMat,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // scaling
	p_AddParameter( init.clsFlt ); // rotation
	p_AddParameter( init.clsVec2 ); // translation
}
void deClassTexMatrix2::nfNewSRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVector2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	const float rotation = rt->GetValue( 1 )->GetFloat();
	const decVector2 &translation = clsVector2.GetVector2( rt->GetValue( 2 )->GetRealObject() );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateSRT( scaling, rotation * DEG2RAD, translation ) );
}

// public static func TexMatrix newSRT( float scalingU, float scalingV, float rotation, float translationU, float translationV )
deClassTexMatrix2::nfNewSRT2::nfNewSRT2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // scalingU
	p_AddParameter( init.clsFlt ); // scalingV
	p_AddParameter( init.clsFlt ); // rotation
	p_AddParameter( init.clsFlt ); // translationU
	p_AddParameter( init.clsFlt ); // translationV
}
void deClassTexMatrix2::nfNewCenterSRT2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float scalingU = rt->GetValue( 0 )->GetFloat();
	const float scalingV = rt->GetValue( 1 )->GetFloat();
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float translationU = rt->GetValue( 3 )->GetFloat();
	const float translationV = rt->GetValue( 4 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateSRT( scalingU, scalingV, rotation * DEG2RAD, translationU, translationV ) );
}

// public static func TexMatrix newCenterSRT( Vector2 scaling, float rotation, Vector2 translation )
deClassTexMatrix2::nfNewCenterSRT::nfNewCenterSRT( const sInitData &init ) : dsFunction( init.clsTexMat,
"newCenterSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsVec2 ); // scaling
	p_AddParameter( init.clsFlt ); // rotation
	p_AddParameter( init.clsVec2 ); // translation
}
void deClassTexMatrix2::nfNewCenterSRT::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassVector2 &clsVector2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	const decVector2 &scaling = clsVector2.GetVector2( rt->GetValue( 0 )->GetRealObject() );
	const float rotation = rt->GetValue( 1 )->GetFloat();
	const decVector2 &translation = clsVector2.GetVector2( rt->GetValue( 2 )->GetRealObject() );
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateCenterSRT( scaling, rotation * DEG2RAD, translation ) );
}

// public static func TexMatrix newCenterSRT( float scalingU, float scalingV, float rotation, float translationU, float translationV )
deClassTexMatrix2::nfNewCenterSRT2::nfNewCenterSRT2( const sInitData &init ) : dsFunction( init.clsTexMat,
"newCenterSRT", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // scalingU
	p_AddParameter( init.clsFlt ); // scalingV
	p_AddParameter( init.clsFlt ); // rotation
	p_AddParameter( init.clsFlt ); // translationU
	p_AddParameter( init.clsFlt ); // translationV
}
void deClassTexMatrix2::nfNewSRT2::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float scalingU = rt->GetValue( 0 )->GetFloat();
	const float scalingV = rt->GetValue( 1 )->GetFloat();
	const float rotation = rt->GetValue( 2 )->GetFloat();
	const float translationU = rt->GetValue( 3 )->GetFloat();
	const float translationV = rt->GetValue( 4 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, decTexMatrix2::CreateCenterSRT( scalingU, scalingV, rotation * DEG2RAD, translationU, translationV ) );
}

// public func destructor()
deClassTexMatrix2::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsTexMat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassTexMatrix2::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Management
///////////////

// public func float getAt( int row, int col )
deClassTexMatrix2::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsTexMat,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
	p_AddParameter( init.clsInt ); // row
	p_AddParameter( init.clsInt ); // col
}
void deClassTexMatrix2::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const int row = rt->GetValue( 0 )->GetInt();
	const int col = rt->GetValue( 1 )->GetInt();
	const float * const values = &matrix.a11;
	
	if( row < 0 || row > 2 || col < 0 || col > 2 ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->PushFloat( values[ row * 3 + col ] );
}

// public func float get11()
deClassTexMatrix2::nfGet11::nfGet11( const sInitData &init ) : dsFunction( init.clsTexMat,
"get11", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet11::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a11 );
}

// public func float get12()
deClassTexMatrix2::nfGet12::nfGet12( const sInitData &init ) : dsFunction( init.clsTexMat,
"get12", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet12::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a12 );
}

// public func float get13()
deClassTexMatrix2::nfGet13::nfGet13( const sInitData &init ) : dsFunction( init.clsTexMat,
"get13", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet13::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a13 );
}

// public func float get21()
deClassTexMatrix2::nfGet21::nfGet21( const sInitData &init ) : dsFunction( init.clsTexMat,
"get21", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet21::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a21 );
}

// public func float get22()
deClassTexMatrix2::nfGet22::nfGet22( const sInitData &init ) : dsFunction( init.clsTexMat,
"get22", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet22::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a22 );
}

// public func float get23()
deClassTexMatrix2::nfGet23::nfGet23( const sInitData &init ) : dsFunction( init.clsTexMat,
"get23", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassTexMatrix2::nfGet23::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	rt->PushFloat( matrix.a23 );
}



// static public func TexMatrix readFromFile( FileReader reader )
deClassTexMatrix2::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsTexMat,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsTexMat ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassTexMatrix2::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsTexMatrix2.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decTexMatrix2 matrix;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	float * const values = &matrix.a11;
	int i;
	for( i=0; i<6; i++ ){
		values[ i ] = reader->ReadFloat();
	}
	
	clsTexMatrix2.PushTexMatrix( rt, matrix );
}

// public func void writeToFile( FileWriter writer )
deClassTexMatrix2::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsTexMat,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassTexMatrix2::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsTexMatrix2.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	const float * const values = &matrix.a11;
	int i;
	for( i=0; i<6; i++ ){
		writer->WriteFloat( values[ i ] );
	}
}



// Operators
//////////////

// public func TexMatrix +( TexMatrix matrix )
deClassTexMatrix2::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsTexMat,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsTexMat ); // matrix
}
void deClassTexMatrix2::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	
	dsRealObject * const objMat = rt->GetValue( 0 )->GetRealObject();
	if( ! objMat ){
		DSTHROW( dueNullPointer );
	}
	
	const decTexMatrix2 &otherMatrix = clsTexMatrix2.GetTexMatrix( objMat );
	
	clsTexMatrix2.PushTexMatrix( rt, matrix + otherMatrix );
}

// public func TexMatrix -( TexMatrix matrix )
deClassTexMatrix2::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsTexMat,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsTexMat ); // matrix
}
void deClassTexMatrix2::nfOpSubtract::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	
	dsRealObject * const objMat = rt->GetValue( 0 )->GetRealObject();
	if( ! objMat ){
		DSTHROW( dueNullPointer );
	}
	
	const decTexMatrix2 &otherMatrix = clsTexMatrix2.GetTexMatrix( objMat );
	
	clsTexMatrix2.PushTexMatrix( rt, matrix - otherMatrix );
}

// public func TexMatrix *( float k )
deClassTexMatrix2::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassTexMatrix2::nfOpScale::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float k = rt->GetValue( 0 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, matrix * k );
}

// public func TexMatrix /( float k )
deClassTexMatrix2::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsTexMat,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassTexMatrix2::nfOpDivide::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	const float k = rt->GetValue( 0 )->GetFloat();
	
	clsTexMatrix2.PushTexMatrix( rt, matrix / k );
}

// public func TexMatrix *( TexMatrix matrix )
deClassTexMatrix2::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsTexMat ){
	p_AddParameter( init.clsTexMat ); // matrix
}
void deClassTexMatrix2::nfOpMultiply::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	
	dsRealObject * const objMat = rt->GetValue( 0 )->GetRealObject();
	if( ! objMat ){
		DSTHROW( dueNullPointer );
	}
	
	const decTexMatrix2 &otherMatrix = clsTexMatrix2.GetTexMatrix( objMat );
	
	clsTexMatrix2.PushTexMatrix( rt, matrix * otherMatrix );
}

// public func TexMatrix *( Vector2 texcoord )
deClassTexMatrix2::nfOpMultiply2::nfOpMultiply2( const sInitData &init ) : dsFunction( init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsVec2 ); // texcoord
}
void deClassTexMatrix2::nfOpMultiply2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	deClassVector2 &clsVec2 = *clsTexMatrix2.GetDS().GetClassVector2();
	
	dsRealObject * const objTexCoord = rt->GetValue( 0 )->GetRealObject();
	if( ! objTexCoord ){
		DSTHROW( dueNullPointer );
	}
	
	const decVector2 &texcoord = clsVec2.GetVector2( objTexCoord );
	
	clsVec2.PushVector2( rt, matrix * texcoord );
}

// public func TexMatrix *( float u, float v )
deClassTexMatrix2::nfOpMultiply3::nfOpMultiply3( const sInitData &init ) : dsFunction( init.clsTexMat,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2 ){
	p_AddParameter( init.clsFlt ); // u
	p_AddParameter( init.clsFlt ); // v
}
void deClassTexMatrix2::nfOpMultiply3::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	const deClassTexMatrix2 &clsTexMatrix2 = *( ( deClassTexMatrix2* )GetOwnerClass() );
	deClassVector2 &clsVec2 = *clsTexMatrix2.GetDS().GetClassVector2();
	const float u = rt->GetValue( 0 )->GetFloat();
	const float v = rt->GetValue( 0 )->GetFloat();
	
	clsVec2.PushVector2( rt, matrix * decVector2( u, v ) );
}



// public func bool equals( Object other )
deClassTexMatrix2::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsTexMat,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassTexMatrix2::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	deClassTexMatrix2 * const clsTexMatrix2 = ( deClassTexMatrix2* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsTexMatrix2 ) ){
		rt->PushBool( false );
		
	}else{
		const decTexMatrix2 &otherMatrix = ( ( sTMatNatDat* )p_GetNativeData( obj ) )->matrix;
		rt->PushBool( matrix.IsEqualTo( otherMatrix ) );
	}
}

// public func int hashCode()
deClassTexMatrix2::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsTexMat,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassTexMatrix2::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decTexMatrix2 &matrix = ( ( sTMatNatDat* )p_GetNativeData( myself ) )->matrix;
	
	const int hash = ( int )( matrix.a11 * 0x0001 )
		+ ( int )( matrix.a12 * 0x0002 )
		+ ( int )( matrix.a13 * 0x0004 )
		+ ( int )( matrix.a21 * 0x0010 )
		+ ( int )( matrix.a22 * 0x0020 )
		+ ( int )( matrix.a23 * 0x0040 );
	
	rt->PushInt( hash );
}



// Class deClassTexMatrix2
///////////////////////////

// Constructor, destructor
////////////////////////////

deClassTexMatrix2::deClassTexMatrix2( deScriptingDragonScript &ds ) :
dsClass( "TexMatrix2", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sTMatNatDat ) );
}

deClassTexMatrix2::~deClassTexMatrix2(){
}



// Management
///////////////

void deClassTexMatrix2::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	memset( &init, '\0', sizeof( init ) );
	
	init.clsTexMat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsVec2 = pDS.GetClassVector2();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfNewTranslation( init ) );
	AddFunction( new nfNewTranslation2( init ) );
	AddFunction( new nfNewScaling( init ) );
	AddFunction( new nfNewScaling2( init ) );
	AddFunction( new nfNewRotation( init ) );
	AddFunction( new nfNewShear( init ) );
	AddFunction( new nfNewShear2( init ) );
	AddFunction( new nfNewST( init ) );
	AddFunction( new nfNewST2( init ) );
	AddFunction( new nfNewSRT( init ) );
	AddFunction( new nfNewSRT2( init ) );
	AddFunction( new nfNewCenterSRT( init ) );
	AddFunction( new nfNewCenterSRT2( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetAt( init ) );
	AddFunction( new nfGet11( init ) );
	AddFunction( new nfGet12( init ) );
	AddFunction( new nfGet13( init ) );
	AddFunction( new nfGet21( init ) );
	AddFunction( new nfGet22( init ) );
	AddFunction( new nfGet23( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpSubtract( init ) );
	AddFunction( new nfOpScale( init ) );
	AddFunction( new nfOpDivide( init ) );
	AddFunction( new nfOpMultiply( init ) );
	AddFunction( new nfOpMultiply2( init ) );
	AddFunction( new nfOpMultiply3( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
}

const decTexMatrix2 &deClassTexMatrix2::GetTexMatrix( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decTexMatrix2 & )( ( sTMatNatDat* )p_GetNativeData( myself->GetBuffer() ) )->matrix;
}

void deClassTexMatrix2::PushTexMatrix( dsRunTime *rt, const decTexMatrix2 &matrix ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sTMatNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->matrix = matrix;
}
