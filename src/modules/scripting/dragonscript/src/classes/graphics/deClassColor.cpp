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

#include "deClassColor.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassArray.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// native structure
struct sClrNatDat{
	decColor color;
};



// native functions
/////////////////////

// constructors, destructor
/////////////////////////////

// public func new( float red, float green, float blue )
deClassColor::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsClr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // red
	p_AddParameter( init.clsFlt ); // green
	p_AddParameter( init.clsFlt ); // blue
}
void deClassColor::nfNew::RunFunction( dsRunTime *RT, dsValue *This ){
	decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	color.r = RT->GetValue( 0 )->GetFloat();
	color.g = RT->GetValue( 1 )->GetFloat();
	color.b = RT->GetValue( 2 )->GetFloat();
	color.a = 1;
}

// public func new( float red, float green, float blue, float alpha )
deClassColor::nfNew2::nfNew2( const sInitData &init ) : dsFunction( init.clsClr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // red
	p_AddParameter( init.clsFlt ); // green
	p_AddParameter( init.clsFlt ); // blue
	p_AddParameter( init.clsFlt ); // alpha
}
void deClassColor::nfNew2::RunFunction( dsRunTime *RT, dsValue *This ){
	decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	color.r = RT->GetValue( 0 )->GetFloat();
	color.g = RT->GetValue( 1 )->GetFloat();
	color.b = RT->GetValue( 2 )->GetFloat();
	color.a = RT->GetValue( 3 )->GetFloat();
}

// public func new( Color c, float alpha )
deClassColor::nfNew3::nfNew3( const sInitData &init ) : dsFunction( init.clsClr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsClr ); // c
	p_AddParameter( init.clsFlt ); // alpha
}
void deClassColor::nfNew3::RunFunction( dsRunTime *RT, dsValue *This ){
	decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	
	const decColor &copyColor = clsColor.GetColor( RT->GetValue( 0 )->GetRealObject() );
	const float alpha = RT->GetValue( 1 )->GetFloat();
	
	color.Set( copyColor.r, copyColor.g, copyColor.b, alpha );
}

// public static func Color newRGB( int red, int green, int blue )
deClassColor::nfNewRGB::nfNewRGB( const sInitData &init ) : dsFunction( init.clsClr,
"newRGB", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsInt ); // red
	p_AddParameter( init.clsInt ); // green
	p_AddParameter( init.clsInt ); // blue
}
void deClassColor::nfNewRGB::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	float r = ( float )RT->GetValue( 0 )->GetInt() / ( float )255;
	float g = ( float )RT->GetValue( 1 )->GetInt() / ( float )255;
	float b = ( float )RT->GetValue( 2 )->GetInt() / ( float )255;
	clsColor->PushColor( RT, decColor( r, g, b, 1 ) );
}

// public static func Color newRGBA( int red, int green, int blue, int alpha )
deClassColor::nfNewRGBA::nfNewRGBA( const sInitData &init ) : dsFunction( init.clsClr,
"newRGBA", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsInt ); // red
	p_AddParameter( init.clsInt ); // green
	p_AddParameter( init.clsInt ); // blue
	p_AddParameter( init.clsInt ); // alpha
}
void deClassColor::nfNewRGBA::RunFunction( dsRunTime *RT, dsValue *This ){
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	float r = ( float )RT->GetValue( 0 )->GetInt() / ( float )255;
	float g = ( float )RT->GetValue( 1 )->GetInt() / ( float )255;
	float b = ( float )RT->GetValue( 2 )->GetInt() / ( float )255;
	float a = ( float )RT->GetValue( 3 )->GetInt() / ( float )255;
	clsColor->PushColor( RT, decColor( r, g, b, a ) );
}

// public static func Color newHSV(float hue, float saturation, float value)
deClassColor::nfNewHSV::nfNewHSV( const sInitData &init ) :
dsFunction( init.clsClr, "newHSV", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsFlt ); // hue
	p_AddParameter( init.clsFlt ); // saturation
	p_AddParameter( init.clsFlt ); // value
}
void deClassColor::nfNewHSV::RunFunction( dsRunTime *rt, dsValue* ){
	deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	const float hue = rt->GetValue( 0 )->GetFloat();
	const float saturation = rt->GetValue( 1 )->GetFloat();
	const float value = rt->GetValue( 2 )->GetFloat();
	clsColor.PushColor( rt, decColor::CreateHSV( hue, saturation, value ) );
}

// public static func Color newHSL(float hue, float saturation, float lightness)
deClassColor::nfNewHSL::nfNewHSL( const sInitData &init ) :
dsFunction( init.clsClr, "newHSL", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsFlt ); // hue
	p_AddParameter( init.clsFlt ); // saturation
	p_AddParameter( init.clsFlt ); // lightness
}
void deClassColor::nfNewHSL::RunFunction( dsRunTime *rt, dsValue* ){
	deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	const float hue = rt->GetValue( 0 )->GetFloat();
	const float saturation = rt->GetValue( 1 )->GetFloat();
	const float lightness = rt->GetValue( 2 )->GetFloat();
	clsColor.PushColor( rt, decColor::CreateHSL( hue, saturation, lightness ) );
}

// public static func Color newCMYK(float cyan, float magenta, float yellow, float black)
deClassColor::nfNewCMYK::nfNewCMYK( const sInitData &init ) :
dsFunction( init.clsClr, "newCMYK", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsFlt ); // cyan
	p_AddParameter( init.clsFlt ); // magenta
	p_AddParameter( init.clsFlt ); // yellow
	p_AddParameter( init.clsFlt ); // black
}
void deClassColor::nfNewCMYK::RunFunction( dsRunTime *rt, dsValue* ){
	deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	const float cyan = rt->GetValue( 0 )->GetFloat();
	const float magenty = rt->GetValue( 1 )->GetFloat();
	const float yellow = rt->GetValue( 2 )->GetFloat();
	const float black = rt->GetValue( 3 )->GetFloat();
	clsColor.PushColor( rt, decColor::CreateCMYK( cyan, magenty, yellow, black ) );
}

////public static func Color newNamed( String name )

// public func destructor()
deClassColor::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsClr,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassColor::nfDestructor::RunFunction( dsRunTime *RT, dsValue *This ){
}



// accessors
//////////////

// public func float getRed()
deClassColor::nfGetRed::nfGetRed( const sInitData &init ) : dsFunction( init.clsClr,
"getRed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColor::nfGetRed::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	RT->PushFloat( color.r );
}
	
// public func float getGreen()
deClassColor::nfGetGreen::nfGetGreen( const sInitData &init ) : dsFunction( init.clsClr,
"getGreen", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColor::nfGetGreen::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	RT->PushFloat( color.g );
}
	
// public func float getBlue()
deClassColor::nfGetBlue::nfGetBlue( const sInitData &init ) : dsFunction( init.clsClr,
"getBlue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColor::nfGetBlue::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	RT->PushFloat( color.b );
}

// public func float getAlpha()
deClassColor::nfGetAlpha::nfGetAlpha( const sInitData &init ) : dsFunction( init.clsClr,
"getAlpha", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassColor::nfGetAlpha::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	RT->PushFloat( color.a );
}

// public func Color clamp()
deClassColor::nfClamp::nfClamp( const sInitData &init ) : dsFunction( init.clsClr,
"clamp", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
}
void deClassColor::nfClamp::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor * const clsColor = ( deClassColor* )GetOwnerClass();
	clsColor->PushColor( RT, color.Normalized() );
}

// public func Color invert()
deClassColor::nfInvert::nfInvert( const sInitData &init ) : dsFunction( init.clsClr,
"invert", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
}
void deClassColor::nfInvert::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor * const clsColor = ( deClassColor* )GetOwnerClass();
	clsColor->PushColor( RT, color.Inverted() );
}



// testing
////////////

// public func bool isEqual( Color c )
deClassColor::nfIsEqual::nfIsEqual( const sInitData &init ) : dsFunction( init.clsClr,
"isEqual", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsClr ); // v
}
void deClassColor::nfIsEqual::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	dsRealObject *objClr = RT->GetValue( 0 )->GetRealObject();
	if( ! objClr ) DSTHROW( dueNullPointer );
	RT->PushBool( color.IsEqualTo( clsColor->GetColor( objClr ) ) );
}

// public func bool isEqual( Color c, float threshold )
deClassColor::nfIsEqual2::nfIsEqual2( const sInitData &init ) : dsFunction( init.clsClr,
"isEqual", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsClr ); // v
	p_AddParameter( init.clsFlt ); // threshold
}
void deClassColor::nfIsEqual2::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	const deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	dsRealObject *objClr = RT->GetValue( 0 )->GetRealObject();
	const float threshold = RT->GetValue( 1 )->GetFloat();
	RT->PushBool( color.IsEqualTo( clsColor.GetColor( objClr ), threshold ) );
}



// File Handling
//////////////////

// static public func Color readFromFile( FileReader reader )
deClassColor::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsClr,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsClr ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassColor::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsColor.GetScriptModule()->GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	decColor color;
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	clsColor.PushColor( rt, reader->ReadColor() );
}

// public func void writeToFile( FileWriter writer )
deClassColor::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsClr,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassColor::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( myself ) )->color;
	const deClassColor &clsColor = *( ( deClassColor* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsColor.GetScriptModule()->GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteColor( color );
}



// operators
//////////////

// public func Color +( Color c )
deClassColor::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsClr,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
	p_AddParameter( init.clsClr ); // v
}
void deClassColor::nfOpAdd::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	dsRealObject *objClr = RT->GetValue( 0 )->GetRealObject();
	if( ! objClr ) DSTHROW( dueNullPointer );
	clsColor->PushColor( RT, color + clsColor->GetColor( objClr ) );
}
	
// public func Color -( Color c )
deClassColor::nfOpSubtract::nfOpSubtract( const sInitData &init ) : dsFunction( init.clsClr,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
	p_AddParameter( init.clsClr ); // v
}
void deClassColor::nfOpSubtract::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	dsRealObject *objClr = RT->GetValue( 0 )->GetRealObject();
	if( ! objClr ) DSTHROW( dueNullPointer );
	clsColor->PushColor( RT, color - clsColor->GetColor( objClr ) );
}
	
// public func Color *( float k )
deClassColor::nfOpScale::nfOpScale( const sInitData &init ) : dsFunction( init.clsClr,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassColor::nfOpScale::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	clsColor->PushColor( RT, color * RT->GetValue( 0 )->GetFloat() );
}

// public func Color /( float k )
deClassColor::nfOpDivide::nfOpDivide( const sInitData &init ) : dsFunction( init.clsClr,
"/", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
	p_AddParameter( init.clsFlt ); // k
}
void deClassColor::nfOpDivide::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	clsColor->PushColor( RT, color / RT->GetValue( 0 )->GetFloat() );
}
	
// public func Color *( Color c )
deClassColor::nfOpMultiply::nfOpMultiply( const sInitData &init ) : dsFunction( init.clsClr,
"*", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsClr ){
	p_AddParameter( init.clsClr ); // v
}
void deClassColor::nfOpMultiply::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	dsRealObject *objClr = RT->GetValue( 0 )->GetRealObject();
	if( ! objClr ) DSTHROW( dueNullPointer );
	clsColor->PushColor( RT, color * clsColor->GetColor( objClr ) );
}



// special functions
//////////////////////

// public func bool equals( Object other )
deClassColor::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsClr,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassColor::nfEquals::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	deClassColor *clsColor = ( deClassColor* )GetOwnerClass();
	dsValue *obj = RT->GetValue( 0 );
	if( ! p_IsObjOfType( obj, clsColor ) ){
		RT->PushBool( false );
	}else{
		const decColor &otherColor = ( ( sClrNatDat* )p_GetNativeData( obj ) )->color;
		RT->PushBool( color.IsEqualTo( otherColor ) );
	}
}

// public func int hashCode()
deClassColor::nfHashCode::nfHashCode( const sInitData &init ) : dsFunction( init.clsClr,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassColor::nfHashCode::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	RT->PushInt( ( int )( color.r * 1000000 )
		+ ( int )( color.g * 1000000 )
		+ ( int )( color.b * 1000000 )
		+ ( int )( color.a * 1000000 ) );
}

// public func String toString()
deClassColor::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsClr,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassColor::nfToString::RunFunction( dsRunTime *RT, dsValue *This ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( This ) )->color;
	char buffer[ 50 ];
	snprintf( ( char* )&buffer, sizeof( buffer ), "(%f,%f,%f,%f)", color.r, color.g, color.b, color.a );
	RT->PushString( buffer );
}

// public func String toString( int precision )
deClassColor::nfToStringPrecision::nfToStringPrecision( const sInitData &init ) :
dsFunction( init.clsClr, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
	p_AddParameter( init.clsInt ); // precision
}
void deClassColor::nfToStringPrecision::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int precision = rt->GetValue( 0 )->GetInt();
	if( precision < 0 ){
		DSTHROW_INFO( dueInvalidParam, "precision < 0" );
	}
	if( precision > 9 ){
		DSTHROW_INFO( dueInvalidParam, "precision > 9" );
	}
	
	const unsigned short p = ( unsigned short )precision;
	char format[ 22 ];
	snprintf( format, sizeof( format ), "(%%.%huf,%%.%huf,%%.%huf,%%.%huf)", p, p, p, p );
	
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( myself ) )->color;
	decString str;
	str.Format( str, color.r, color.g, color.b, color.a );
	rt->PushString( str );
}

// public func Array toHSV()
deClassColor::nfToHSV::nfToHSV( const sInitData &init ) :
dsFunction( init.clsClr, "toHSV", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsArray ){
}
void deClassColor::nfToHSV::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( myself ) )->color;
	float hue, saturation, value;
	color.ToHSV( hue, saturation, value );
	
	const deScriptingDragonScript &ds = *( ( deClassColor* )GetOwnerClass() )->GetScriptModule();
	dsValue * const valueHsv = rt->CreateValue( ds.GetScriptEngine()->GetClassArray() );
	
	try{
		rt->CreateObject( valueHsv, ds.GetScriptEngine()->GetClassArray(), 0 );
		
		rt->PushFloat( hue );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( saturation );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( value );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushValue( valueHsv );
		rt->FreeValue( valueHsv );
		
	}catch( ... ){
		if( valueHsv ){
			rt->FreeValue( valueHsv );
		}
		throw;
	}
}

// public func Array toHSL()
deClassColor::nfToHSL::nfToHSL( const sInitData &init ) :
dsFunction( init.clsClr, "toHSL", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsArray ){
}
void deClassColor::nfToHSL::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( myself ) )->color;
	float hue, saturation, lightness;
	color.ToHSL( hue, saturation, lightness );
	
	const deScriptingDragonScript &ds = *( ( deClassColor* )GetOwnerClass() )->GetScriptModule();
	dsValue * const valueHsv = rt->CreateValue( ds.GetScriptEngine()->GetClassArray() );
	
	try{
		rt->CreateObject( valueHsv, ds.GetScriptEngine()->GetClassArray(), 0 );
		
		rt->PushFloat( hue );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( saturation );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( lightness );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushValue( valueHsv );
		rt->FreeValue( valueHsv );
		
	}catch( ... ){
		if( valueHsv ){
			rt->FreeValue( valueHsv );
		}
		throw;
	}
}

// public func Array toCMYK()
deClassColor::nfToCMYK::nfToCMYK( const sInitData &init ) :
dsFunction( init.clsClr, "toCMYK", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsArray ){
}
void deClassColor::nfToCMYK::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decColor &color = ( ( sClrNatDat* )p_GetNativeData( myself ) )->color;
	float cyan, magenta, yellow, black;
	color.ToCMYK( cyan, magenta, yellow, black );
	
	const deScriptingDragonScript &ds = *( ( deClassColor* )GetOwnerClass() )->GetScriptModule();
	dsValue * const valueHsv = rt->CreateValue( ds.GetScriptEngine()->GetClassArray() );
	
	try{
		rt->CreateObject( valueHsv, ds.GetScriptEngine()->GetClassArray(), 0 );
		
		rt->PushFloat( cyan );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( magenta );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( yellow );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushFloat( black );
		rt->RunFunction( valueHsv, "add", 1 );
		
		rt->PushValue( valueHsv );
		rt->FreeValue( valueHsv );
		
	}catch( ... ){
		if( valueHsv ){
			rt->FreeValue( valueHsv );
		}
		throw;
	}
}



// class deClassColor
////////////////////////
// constructor
deClassColor::deClassColor( deEngine *gameEngine, deScriptingDragonScript *scriptManager ) :
dsClass( "Color", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! gameEngine || ! scriptManager ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scriptManager;
	// set parser info
	GetParserInfo()->SetParent( DENS_GUI );
	GetParserInfo()->SetBase( "Object" );
	// do the rest
	p_SetNativeDataSize( sizeof( sClrNatDat ) );
}
deClassColor::~deClassColor(){
}
// management
void deClassColor::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsClr = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsFileReader = pScrMgr->GetClassFileReader();
	init.clsFileWriter = pScrMgr->GetClassFileWriter();
	init.clsArray = engine->GetClassArray();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfNew3( init ) );
	AddFunction( new nfNewRGB( init ) );
	AddFunction( new nfNewRGBA( init ) );
	AddFunction( new nfNewHSV( init ) );
	AddFunction( new nfNewHSL( init ) );
	AddFunction( new nfNewCMYK( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetRed( init ) );
	AddFunction( new nfGetGreen( init ) );
	AddFunction( new nfGetBlue( init ) );
	AddFunction( new nfGetAlpha( init ) );
	AddFunction( new nfClamp( init ) );
	AddFunction( new nfInvert( init ) );
	
	AddFunction( new nfIsEqual( init ) );
	AddFunction( new nfIsEqual2( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpSubtract( init ) );
	AddFunction( new nfOpScale( init ) );
	AddFunction( new nfOpDivide( init ) );
	AddFunction( new nfOpMultiply( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	AddFunction( new nfToStringPrecision( init ) );
	AddFunction( new nfToHSV( init ) );
	AddFunction( new nfToHSL( init ) );
	AddFunction( new nfToCMYK( init ) );
	
	// add constant variables
	AddVariable( new dsVariable( this, "white", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "lightGray", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "gray", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "darkGray", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "black", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "red", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "darkRed", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "green", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "darkGreen", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "blue", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "darkBlue", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "yellow", this, DSTM_PUBLIC | DSTM_STATIC ) );
	AddVariable( new dsVariable( this, "transparent", this, DSTM_PUBLIC | DSTM_STATIC ) );
}
void deClassColor::InitStatics( dsRunTime *RT ){
	AddColorConstant( RT, "white", decColor( 1, 1, 1, 1 ) );
	AddColorConstant( RT, "lightGray", decColor( 0.75, 0.75, 0.75, 1 ) );
	AddColorConstant( RT, "gray", decColor( 0.5, 0.5, 0.5, 1 ) );
	AddColorConstant( RT, "darkGray", decColor( 0.25, 0.25, 0.25, 1 ) );
	AddColorConstant( RT, "black", decColor( 0, 0, 0, 1 ) );
	AddColorConstant( RT, "red", decColor( 1, 0, 0, 1 ) );
	AddColorConstant( RT, "darkRed", decColor( 0.5, 0, 0, 1 ) );
	AddColorConstant( RT, "green", decColor( 0, 1, 0, 1 ) );
	AddColorConstant( RT, "darkGreen", decColor( 0, 0.5, 0, 1 ) );
	AddColorConstant( RT, "blue", decColor( 0, 0, 1, 1 ) );
	AddColorConstant( RT, "darkBlue", decColor( 0, 0, 0.5, 1 ) );
	AddColorConstant( RT, "yellow", decColor( 1, 1, 0, 1 ) );
	AddColorConstant( RT, "transparent", decColor( 0, 0, 0, 0 ) );
}
const decColor &deClassColor::GetColor( dsRealObject *This ) const{
	if( ! This ){
		DSTHROW( dueNullPointer );
	}
	
	return ( const decColor & )( ( sClrNatDat* )p_GetNativeData( This->GetBuffer() ) )->color;
}
void deClassColor::PushColor( dsRunTime *rt, const decColor &color ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sClrNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->color = color;
}
void deClassColor::AddColorConstant( dsRunTime *rt, const char *name, const decColor &color ){
	dsVariable * const variable = FindVariable( name );
	if( ! variable ){
		DSTHROW( dueInvalidParam );
	}
	
	dsValue * const value = variable->GetStaticValue();
	rt->CreateObjectNaked( value, this );
	( ( sClrNatDat* )p_GetNativeData( value->GetRealObject()->GetBuffer() ) )->color = color;
}
