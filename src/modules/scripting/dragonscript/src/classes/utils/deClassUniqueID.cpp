/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassUniqueID.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>


// native structure
struct sUIDNatDat{
	decUniqueID *id;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassUniqueID::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassUniqueID::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sUIDNatDat &nd = *( ( sUIDNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	nd.id = new decUniqueID;
}

// public func new( int value )
deClassUniqueID::nfNewInt::nfNewInt( const sInitData &init ) : dsFunction( init.clsUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassUniqueID::nfNewInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	sUIDNatDat &nd = *( ( sUIDNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	nd.id = new decUniqueID( rt->GetValue( 0 )->GetInt() );
}

// public func destructor()
deClassUniqueID::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsUID,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassUniqueID::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sUIDNatDat &nd = *( ( sUIDNatDat* )p_GetNativeData( myself ) );
	
	if( nd.id ){
		delete nd.id;
		nd.id = NULL;
	}
}



// Management
///////////////

// public func int getBitCount()
deClassUniqueID::nfGetBitCount::nfGetBitCount( const sInitData &init ) : dsFunction( init.clsUID,
"getBitCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassUniqueID::nfGetBitCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	rt->PushInt( id.GetBitCount() );
}

// public func int getByteAt( int position )
deClassUniqueID::nfGetByteAt::nfGetByteAt( const sInitData &init ) : dsFunction( init.clsUID,
"getByteAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // position
}
void deClassUniqueID::nfGetByteAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	rt->PushInt( id.GetByteAt( rt->GetValue( 0 )->GetInt() ) );
}



// public func String toHexString()
deClassUniqueID::nfToHexString::nfToHexString( const sInitData &init ) : dsFunction( init.clsUID,
"toHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassUniqueID::nfToHexString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	rt->PushString( id.ToHexString() );
}

// static public func UniqueID newFromHexString( String string )
deClassUniqueID::nfNewFromHexString::nfNewFromHexString( const sInitData &init ) : dsFunction( init.clsUID,
"newFromHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsUID ){
	p_AddParameter( init.clsStr ); // string
}
void deClassUniqueID::nfNewFromHexString::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	clsUID.PushUniqueID( rt, decUniqueID( rt->GetValue( 0 )->GetString() ) );
}

// func bool zero()
deClassUniqueID::nfZero::nfZero(const sInitData &init) :
dsFunction(init.clsUID, "zero", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassUniqueID::nfZero::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *(((sUIDNatDat*)p_GetNativeData(myself))->id);
	rt->PushBool(id.IsZero());
}

// func bool notZero()
deClassUniqueID::nfNotZero::nfNotZero(const sInitData &init) :
dsFunction(init.clsUID, "notZero", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassUniqueID::nfNotZero::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *(((sUIDNatDat*)p_GetNativeData(myself))->id);
	rt->PushBool(!id.IsZero());
}



// File Handling
//////////////////

// static public func MutableID readFromFile( FileReader reader )
deClassUniqueID::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsUID,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsUID ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassUniqueID::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsUID.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	decUniqueID id;
	const int byteCount = reader->ReadByte();
	int i;
	
	id.SetBitCount( byteCount * 8 );
	for( i=0; i<byteCount; i++ ){
		id.SetByteAt( i, reader->ReadByte() );
	}
	
	clsUID.PushUniqueID( rt, id );
}

// public func void writeToFile( FileWriter writer )
deClassUniqueID::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsUID,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassUniqueID::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	const deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsUID.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	const int byteCount = id.GetBitCount() / 8; // bit count is only allowed to be multiple of 8
	writer->WriteByte( ( uint8_t )byteCount );
	int i;
	
	for( i=0; i<byteCount; i++ ){
		writer->WriteByte( ( uint8_t )id.GetByteAt( i ) );
	}
}



// public func UniqueID +( int value )
deClassUniqueID::nfOpAddInt::nfOpAddInt( const sInitData &init ) : dsFunction( init.clsUID,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
	p_AddParameter( init.clsInt ); // value
}
void deClassUniqueID::nfOpAddInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	
	decUniqueID newID( id );
	newID.IncrementBy( rt->GetValue( 0 )->GetInt() );
	clsUID.PushUniqueID( rt, newID );
}

// public func UniqueID +( UniqueID id )
deClassUniqueID::nfOpAddID::nfOpAddID( const sInitData &init ) : dsFunction( init.clsUID,
"+", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpAddID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	
	decUniqueID newID( id );
	newID.IncrementBy( clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
	clsUID.PushUniqueID( rt, newID );
}

// public func UniqueID -( int value )
deClassUniqueID::nfOpSubInt::nfOpSubInt( const sInitData &init ) : dsFunction( init.clsUID,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
	p_AddParameter( init.clsInt ); // value
}
void deClassUniqueID::nfOpSubInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	
	decUniqueID newID( id );
	newID.DecrementBy( rt->GetValue( 0 )->GetInt() );
	clsUID.PushUniqueID( rt, newID );
}

// public func UniqueID -( UniqueID id )
deClassUniqueID::nfOpSubID::nfOpSubID( const sInitData &init ) : dsFunction( init.clsUID,
"-", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpSubID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	
	decUniqueID newID( id );
	newID.DecrementBy( clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
	clsUID.PushUniqueID( rt, newID );
}

// public func bool <( UniqueID id )
deClassUniqueID::nfOpLess::nfOpLess( const sInitData &init ) : dsFunction( init.clsUID,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpLess::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	rt->PushBool( id < clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool <=( UniqueID id )
deClassUniqueID::nfOpLessEqual::nfOpLessEqual( const sInitData &init ) : dsFunction( init.clsUID,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpLessEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	rt->PushBool( id <= clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool >( UniqueID id )
deClassUniqueID::nfOpGreater::nfOpGreater( const sInitData &init ) : dsFunction( init.clsUID,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpGreater::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	rt->PushBool( id > clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool >=( UniqueID id )
deClassUniqueID::nfOpGreaterEqual::nfOpGreaterEqual( const sInitData &init ) : dsFunction( init.clsUID,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsUID ); // id
}
void deClassUniqueID::nfOpGreaterEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID &clsUID = *( ( deClassUniqueID* )GetOwnerClass() );
	rt->PushBool( id >= clsUID.GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}



// Misc
/////////

// public func int hashCode()
deClassUniqueID::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsUID, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassUniqueID::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	const int count = decMath::min( id.GetBitCount() / 8, 3 );
	int i, hash = 0;
	for( i=0; i<count; i++ ){
		hash = ( hash << 8 ) | id.GetByteAt( 0 );
	}
	
	rt->PushInt( hash );
}

// public func bool equals( Object object )
deClassUniqueID::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsUID, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassUniqueID::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID * const clsUID = ( deClassUniqueID* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsUID ) ){
		rt->PushBool( false );
		
	}else{
		const decUniqueID &otherUniqueID = *( ( ( sUIDNatDat* )p_GetNativeData( obj ) )->id );
		rt->PushBool( id == otherUniqueID );
	}
}

// public func int compare( Object object )
deClassUniqueID::nfCompare::nfCompare( const sInitData &init ) :
dsFunction( init.clsUID, "compare", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsObj ); // object
}
void deClassUniqueID::nfCompare::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID * const clsUID = ( deClassUniqueID* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsUID ) ){
		rt->PushInt( 0 );
		
	}else{
		const decUniqueID &otherUniqueID = *( ( ( sUIDNatDat* )p_GetNativeData( obj ) )->id );
		
		if( id == otherUniqueID ){
			rt->PushInt( 0 );
			
		}else if( id < otherUniqueID ){
			rt->PushInt( -1 );
			
		}else{
			rt->PushInt( 1 );
		}
	}
}

// public func String toString()
deClassUniqueID::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsUID,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassUniqueID::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sUIDNatDat* )p_GetNativeData( myself ) )->id );
	int b, offset, value, byteCount = id.GetBitCount() / 8;
	char buffer[ 100 ];
	
	offset = 0;
	
	buffer[ offset++ ] = '0';
	buffer[ offset++ ] = 'h';
	
	for( b=byteCount-1; b>=0; b-- ){
		const int vbyte = id.GetByteAt( b );
		
		value = ( vbyte & 0xf0 ) >> 4;
		if( value < 10 ){
			buffer[ offset++ ] = '0' + value;
			
		}else{
			buffer[ offset++ ] = 'A' + ( value - 10 );
		}
		
		value = vbyte & 0x0f;
		if( value < 10 ){
			buffer[ offset++ ] = '0' + value;
			
		}else{
			buffer[ offset++ ] = 'A' + ( value - 10 );
		}
	}
	
	buffer[ offset++ ] = '\0';
	
	rt->PushString( buffer );
}



// Class deClassUniqueID
//////////////////////////

// Constructor
////////////////

deClassUniqueID::deClassUniqueID( deScriptingDragonScript &ds ) :
dsClass( "UniqueID", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sUIDNatDat ) );
}

deClassUniqueID::~deClassUniqueID(){
}



// Management
///////////////

void deClassUniqueID::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsUID = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewInt( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetBitCount( init ) );
	AddFunction( new nfGetByteAt( init ) );
	
	AddFunction( new nfToHexString( init ) );
	AddFunction( new nfNewFromHexString( init ) );
	AddFunction(new nfZero(init));
	AddFunction(new nfNotZero(init));
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpAddInt( init ) );
	AddFunction( new nfOpAddID( init ) );
	AddFunction( new nfOpSubInt( init ) );
	AddFunction( new nfOpSubID( init ) );
	AddFunction( new nfOpLess( init ) );
	AddFunction( new nfOpLessEqual( init ) );
	AddFunction( new nfOpGreater( init ) );
	AddFunction( new nfOpGreaterEqual( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfCompare( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

decUniqueID &deClassUniqueID::GetUniqueID( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( sUIDNatDat* )p_GetNativeData( myself->GetBuffer() ) )->id;
}

void deClassUniqueID::PushUniqueID( dsRunTime *rt, const decUniqueID &id ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sUIDNatDat &nd = *( ( sUIDNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.id = NULL;
	
	try{
		nd.id = new decUniqueID( id );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}

void deClassUniqueID::PushUniqueID( dsRunTime *rt, int byteCount, unsigned char *bytes ){
	if( ! rt || byteCount < 1 || ! bytes ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sUIDNatDat &nd = *( ( sUIDNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.id = NULL;
	
	try{
		nd.id = new decUniqueID;
		
		nd.id->SetBitCount( byteCount * 8 );
		int b;
		for( b=0; b<byteCount; b++ ){
			nd.id->SetByteAt( b, bytes[ b ] );
		}
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
