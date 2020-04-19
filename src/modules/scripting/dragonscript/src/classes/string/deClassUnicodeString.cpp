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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <libdscript/libdscript.h>

#include "deClassUnicodeString.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/decString.h>
#include <libdscript/packages/default/dsClassObject.h>
#include <libdscript/exceptions.h>


// native structure
struct sUSNatDat{
	decUnicodeString *string;
};



// native functions
/////////////////////



// Constructors, destructors
//////////////////////////////

// public func new()
deClassUnicodeString::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsUS,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassUnicodeString::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sUSNatDat *nd = ( sUSNatDat* )p_GetNativeData( myself );
	// clear ( important )
	nd->string = NULL;
	nd->string = new decUnicodeString;
	if( ! nd->string ) DSTHROW( dueOutOfMemory );
}

// public func new( UnicodeString string )
deClassUnicodeString::nfNewFrom::nfNewFrom( const sInitData &init ) : dsFunction( init.clsUS,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUS ); // string
}
void deClassUnicodeString::nfNewFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	sUSNatDat *nd = ( sUSNatDat* )p_GetNativeData( myself );
	
	// clear ( important )
	nd->string = NULL;
	
	// retrieve the other string
	dsRealObject *objStr = rt->GetValue( 0 )->GetRealObject();
	if( ! objStr ) DSTHROW( dueNullPointer );
	sUSNatDat *other = ( sUSNatDat* )p_GetNativeData( rt->GetValue( 0 ) );
	
	// create copy
	nd->string = new decUnicodeString( *other->string );
	if( ! nd->string ) DSTHROW( dueOutOfMemory );
}

// public static func newFromUTF8( String string )
deClassUnicodeString::nfNewFromUTF8::nfNewFromUTF8( const sInitData &init ) : dsFunction( init.clsUS,
"newFromUTF8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_STATIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsStr ); // string
}
void deClassUnicodeString::nfNewFromUTF8::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	clsUS.PushUTF8( rt, rt->GetValue( 0 )->GetString() );
}

// public func new( int character )
deClassUnicodeString::nfNewFromCharacter::nfNewFromCharacter( const sInitData &init ) : dsFunction( init.clsUS,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // character
}
void deClassUnicodeString::nfNewFromCharacter::RunFunction( dsRunTime *rt, dsValue *myself ){
	sUSNatDat *nd = ( sUSNatDat* )p_GetNativeData( myself );
	// clear ( important )
	nd->string = NULL;
	// create copy
	nd->string = new decUnicodeString( rt->GetValue( 0 )->GetInt() );
	if( ! nd->string ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassUnicodeString::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsUS,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassUnicodeString::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sUSNatDat *nd = ( sUSNatDat* )p_GetNativeData( myself );
	
	if( nd->string ){
		delete nd->string;
		nd->string = NULL;
	}
}



// Accessors
//////////////

// public func int getLength()
deClassUnicodeString::nfGetLength::nfGetLength( const sInitData &init ) : dsFunction( init.clsUS,
"getLength", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassUnicodeString::nfGetLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	rt->PushInt( string->GetLength() );
}

// public func int getCharacterAt( int position )
deClassUnicodeString::nfGetCharacterAt::nfGetCharacterAt( const sInitData &init ) : dsFunction( init.clsUS,
"getCharacterAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // position
}
void deClassUnicodeString::nfGetCharacterAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	rt->PushInt( string->GetAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func UnicodeString getLeft( int count )
deClassUnicodeString::nfGetLeft::nfGetLeft( const sInitData &init ) : dsFunction( init.clsUS,
"getLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsInt ); // count
}
void deClassUnicodeString::nfGetLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetLeft( rt->GetValue( 0 )->GetInt() ) );
}

// public func UnicodeString getRight( int count )
deClassUnicodeString::nfGetRight::nfGetRight( const sInitData &init ) : dsFunction( init.clsUS,
"getRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsInt ); // count
}
void deClassUnicodeString::nfGetRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetRight( rt->GetValue( 0 )->GetInt() ) );
}

// public func UnicodeString getSubString( int position,  int count )
deClassUnicodeString::nfGetSubString::nfGetSubString( const sInitData &init ) : dsFunction( init.clsUS,
"getSubString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsInt ); // position
	p_AddParameter( init.clsInt ); // count
}
void deClassUnicodeString::nfGetSubString::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	int position = rt->GetValue( 0 )->GetInt();
	int count = rt->GetValue( 1 )->GetInt();
	
	clsUS.PushUnicodeString( rt, string->GetMiddle( position, position + count ) );
}

// public func UnicodeString reverse()
deClassUnicodeString::nfReverse::nfReverse( const sInitData &init ) : dsFunction( init.clsUS,
"reverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfReverse::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetReversed() );
}



// Trimming
/////////////

// public func UnicodeString trimLeft()
deClassUnicodeString::nfTrimLeft::nfTrimLeft( const sInitData &init ) : dsFunction( init.clsUS,
"trimLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfTrimLeft::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetTrimmedLeft() );
}

// public func UnicodeString trimRight()
deClassUnicodeString::nfTrimRight::nfTrimRight( const sInitData &init ) : dsFunction( init.clsUS,
"trimRight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfTrimRight::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetTrimmedRight() );
}

// public func UnicodeString trimBoth()
deClassUnicodeString::nfTrimBoth::nfTrimBoth( const sInitData &init ) : dsFunction( init.clsUS,
"trimBoth", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfTrimBoth::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetTrimmed() );
}



// Searching
//////////////

// public func int findCharacter( int character, int position )
deClassUnicodeString::nfFindChar::nfFindChar( const sInitData &init ) : dsFunction( init.clsUS,
"findCharacter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // character
	p_AddParameter( init.clsInt ); // position
}
void deClassUnicodeString::nfFindChar::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	int character = rt->GetValue( 0 )->GetInt();
	int position = rt->GetValue( 1 )->GetInt();
	
	rt->PushInt( string->Find( character, position ) );
}

// public func int findCharacterReverse( int character, int position )
deClassUnicodeString::nfFindCharReverse::nfFindCharReverse( const sInitData &init ) : dsFunction( init.clsUS,
"findCharacterReverse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // character
	p_AddParameter( init.clsInt ); // position
}
void deClassUnicodeString::nfFindCharReverse::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	int character = rt->GetValue( 0 )->GetInt();
	int position = rt->GetValue( 1 )->GetInt();
	
	rt->PushInt( string->FindReverse( character, 0, -position ) );
}



// Comparison
///////////////

// public func int compareTo( UnicodeString string )
deClassUnicodeString::nfCompareTo::nfCompareTo( const sInitData &init ) : dsFunction( init.clsUS,
"compareTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsUS ); // string
}
void deClassUnicodeString::nfCompareTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	// retrieve the other string
	dsRealObject *objStr = rt->GetValue( 0 )->GetRealObject();
	if( ! objStr ) DSTHROW( dueNullPointer );
	sUSNatDat *other = ( sUSNatDat* )p_GetNativeData( rt->GetValue( 0 ) );
	// compare
	rt->PushInt( string->Compare( *other->string ) );
}

// public func int compareToNoCase( UnicodeString string )
deClassUnicodeString::nfCompareToNoCase::nfCompareToNoCase( const sInitData &init ) : dsFunction( init.clsUS,
"compareToNoCase", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsUS ); // string
}
void deClassUnicodeString::nfCompareToNoCase::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	// retrieve the other string
	dsRealObject *objStr = rt->GetValue( 0 )->GetRealObject();
	if( ! objStr ) DSTHROW( dueNullPointer );
	sUSNatDat *other = ( sUSNatDat* )p_GetNativeData( rt->GetValue( 0 ) );
	// compare
	rt->PushInt( string->CompareInsensitive( *other->string ) );
}

// public func bool equals( Object object )
deClassUnicodeString::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsUS, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassUnicodeString::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString *clsUS = ( deClassUnicodeString* )GetOwnerClass();
	dsValue *object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsUS ) ){
		rt->PushBool( false );
	}else{
		decUnicodeString *otherString = ( ( sUSNatDat* )p_GetNativeData( object ) )->string;
		rt->PushBool( *string == *otherString );
	}
}

// public func int hashCode()
deClassUnicodeString::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsUS, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassUnicodeString::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	rt->PushInt( string->Hash() );
}



// Conversion
///////////////

// public func UnicodeString toLower()
deClassUnicodeString::nfToLower::nfToLower( const sInitData &init ) : dsFunction( init.clsUS,
"toLower", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfToLower::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetLower() );
}

// public func UnicodeString toUpper()
deClassUnicodeString::nfToUpper::nfToUpper( const sInitData &init ) : dsFunction( init.clsUS,
"toUpper", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
}
void deClassUnicodeString::nfToUpper::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, string->GetUpper() );
}

// public func int toInt()
deClassUnicodeString::nfToInt::nfToInt( const sInitData &init ) : dsFunction( init.clsUS,
"toInt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassUnicodeString::nfToInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	rt->PushInt( string->ToInt() );
}

// public func float toFloat()
deClassUnicodeString::nfToFloat::nfToFloat( const sInitData &init ) : dsFunction( init.clsUS,
"toFloat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassUnicodeString::nfToFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	rt->PushFloat( string->ToFloat() );
}

// public func String toString()
deClassUnicodeString::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsUS,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassUnicodeString::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	clsUS.PushUnicodeAsUTF8( rt, myself );
}

// public func String toUTF8()
deClassUnicodeString::nfToUTF8::nfToUTF8( const sInitData &init ) : dsFunction( init.clsUS,
"toUTF8", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassUnicodeString::nfToUTF8::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	clsUS.PushUnicodeAsUTF8( rt, myself );
}



// Operators
//////////////

// public func UnicodeString +( UnicodeString string )
deClassUnicodeString::nfOpAdd::nfOpAdd( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsUS ); // string
}
void deClassUnicodeString::nfOpAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	// retrieve the other string
	dsRealObject *objStr = rt->GetValue( 0 )->GetRealObject();
	if( ! objStr ) DSTHROW( dueNullPointer );
	sUSNatDat *other = ( sUSNatDat* )p_GetNativeData( rt->GetValue( 0 ) );
	
	clsUS.PushUnicodeString( rt, *string + *other->string );
}

// public func UnicodeString +( byte value )
deClassUnicodeString::nfOpAddByte::nfOpAddByte( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsByte ); // value
}
void deClassUnicodeString::nfOpAddByte::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	decUnicodeString unicode( *string );
	unicode.AppendCharacter( rt->GetValue( 0 )->GetByte() );
	clsUS.PushUnicodeString( rt, unicode );
}

// public func UnicodeString +( bool value )
deClassUnicodeString::nfOpAddBool::nfOpAddBool( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsBool ); // value
}
void deClassUnicodeString::nfOpAddBool::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, *string + rt->GetValue( 0 )->GetBool() );
}

// public func UnicodeString +( int value )
deClassUnicodeString::nfOpAddInt::nfOpAddInt( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsInt ); // value
}
void deClassUnicodeString::nfOpAddInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, *string + rt->GetValue( 0 )->GetInt() );
}

// public func UnicodeString +( float value )
deClassUnicodeString::nfOpAddFloat::nfOpAddFloat( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassUnicodeString::nfOpAddFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	clsUS.PushUnicodeString( rt, *string + rt->GetValue( 0 )->GetFloat() );
}

// public func UnicodeString +( Object object )
deClassUnicodeString::nfOpAddObject::nfOpAddObject( const sInitData &init ) : dsFunction( init.clsUS,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUS ){
	p_AddParameter( init.clsObj ); // object
}
void deClassUnicodeString::nfOpAddObject::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUnicodeString *string = ( ( sUSNatDat* )p_GetNativeData( myself ) )->string;
	deClassUnicodeString &clsUS = *( ( deClassUnicodeString* )GetOwnerClass() );
	
	// get string by calling toString on the object if the object is not null
	dsValue *object = rt->GetValue( 0 );
	const char *objectString = NULL;
	if( object->GetRealType()->GetPrimitiveType() != DSPT_OBJECT || object->GetRealObject() ){
		const int funcIndexToString = ( ( dsClassObject* )rt->GetEngine()->GetClassObject() )->GetFuncIndexToString();
		rt->RunFunctionFast( object, funcIndexToString );
		objectString = rt->GetReturnString();
	}
	if( ! objectString ) objectString = "<null>";
	
	decUnicodeString unicode( *string );
	unicode.AppendFromUTF8( objectString );
	clsUS.PushUnicodeString( rt, unicode );
}



// class deClassUnicodeString
///////////////////////////////

// constructor
////////////////

deClassUnicodeString::deClassUnicodeString() : dsClass( "UnicodeString", DSCT_CLASS,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	GetParserInfo()->SetBase( "Object" );
	p_SetNativeDataSize( sizeof( sUSNatDat ) );
}

deClassUnicodeString::~deClassUnicodeString(){
}



// management
///////////////

void deClassUnicodeString::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	// store classes
	init.clsUS = this;
	init.clsObj = engine->GetClassObject();
	init.clsVoid = engine->GetClassVoid();
	init.clsByte = engine->GetClassByte();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewFrom( init ) );
	AddFunction( new nfNewFromCharacter( init ) );
	AddFunction( new nfNewFromUTF8( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetLength( init ) );
	AddFunction( new nfGetCharacterAt( init ) );
	AddFunction( new nfGetLeft( init ) );
	AddFunction( new nfGetRight( init ) );
	AddFunction( new nfGetSubString( init ) );
	AddFunction( new nfReverse( init ) );
	
	AddFunction( new nfTrimLeft( init ) );
	AddFunction( new nfTrimRight( init ) );
	AddFunction( new nfTrimBoth( init ) );
	
	AddFunction( new nfFindChar( init ) );
	AddFunction( new nfFindCharReverse( init ) );
	
	AddFunction( new nfCompareTo( init ) );
	AddFunction( new nfCompareToNoCase( init ) );
	
	AddFunction( new nfToLower( init ) );
	AddFunction( new nfToUpper( init ) );
	AddFunction( new nfToInt( init ) );
	AddFunction( new nfToFloat( init ) );
	AddFunction( new nfToString( init ) );
	AddFunction( new nfToUTF8( init ) );
	
	AddFunction( new nfOpAdd( init ) );
	AddFunction( new nfOpAddByte( init ) );
	AddFunction( new nfOpAddBool( init ) );
	AddFunction( new nfOpAddInt( init ) );
	AddFunction( new nfOpAddFloat( init ) );
	AddFunction( new nfOpAddObject( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

const decUnicodeString &deClassUnicodeString::GetUnicodeString( dsRealObject *myself ) const{
	sUSNatDat *nd = ( sUSNatDat* )p_GetNativeData( myself->GetBuffer() );
	return ( const decUnicodeString & )*nd->string;
}

const decString deClassUnicodeString::GetUTF8( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	return ( ( sUSNatDat* )p_GetNativeData( myself->GetBuffer() ) )->string->ToUTF8();
}

void deClassUnicodeString::PushUnicodeString( dsRunTime *rt, const decUnicodeString &string ){
	rt->CreateObjectNakedOnStack( this );
	sUSNatDat &nd = *( ( sUSNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.string = NULL;
	
	try{
		nd.string = new decUnicodeString( string );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}

void deClassUnicodeString::PushUTF8( dsRunTime *rt, const char *utf8 ){
	PushUnicodeString( rt, decUnicodeString::NewFromUTF8( utf8 ) );
}

void deClassUnicodeString::PushUnicodeAsUTF8( dsRunTime *rt, dsValue *myself ){
	rt->PushString( GetUTF8( myself->GetRealObject() ) );
}
