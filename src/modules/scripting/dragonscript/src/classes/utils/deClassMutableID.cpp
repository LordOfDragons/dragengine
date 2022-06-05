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

#include "deClassUniqueID.h"
#include "deClassMutableID.h"
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
struct sMUIDNatDat{
	decUniqueID *id;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassMutableID::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsMUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMutableID::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	nd.id = new decUniqueID;
}

// public func new( int value )
deClassMutableID::nfNewValue::nfNewValue( const sInitData &init ) : dsFunction( init.clsMUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // value
}
void deClassMutableID::nfNewValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	nd.id = new decUniqueID( rt->GetValue( 0 )->GetInt() );
}

// public func new( UniqueID id )
deClassMutableID::nfNewUniqueID::nfNewUniqueID( const sInitData &init ) : dsFunction( init.clsMUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUID ); // id
}
void deClassMutableID::nfNewUniqueID::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	const decUniqueID &uid = ds.GetClassUniqueID()->GetUniqueID( rt->GetValue( 0 )->GetRealObject() );
	nd.id = new decUniqueID( uid );
}

// public func new( MutableID copy )
deClassMutableID::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsMUID,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMUID ); // copy
}
void deClassMutableID::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( myself ) );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	
	// clear ( important )
	nd.id = NULL;
	
	// create id
	const decUniqueID &uid = clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() );
	nd.id = new decUniqueID( uid );
}

// public func destructor()
deClassMutableID::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsMUID,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMutableID::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( myself ) );
	
	if( nd.id ){
		delete nd.id;
		nd.id = NULL;
	}
}



// Management
///////////////

// public func int getBitCount()
deClassMutableID::nfGetBitCount::nfGetBitCount( const sInitData &init ) : dsFunction( init.clsMUID,
"getBitCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassMutableID::nfGetBitCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	rt->PushInt( id.GetBitCount() );
}

// public func void setBitCount( int bitCount )
deClassMutableID::nfSetBitCount::nfSetBitCount( const sInitData &init ) : dsFunction( init.clsMUID,
"setBitCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bitCount
}
void deClassMutableID::nfSetBitCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	id.SetBitCount( rt->GetValue( 0 )->GetInt() );
}

// public func int getByteAt( int position )
deClassMutableID::nfGetByteAt::nfGetByteAt( const sInitData &init ) : dsFunction( init.clsMUID,
"getByteAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsInt ); // position
}
void deClassMutableID::nfGetByteAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	rt->PushInt( id.GetByteAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func void setByteAt( int position, int value )
deClassMutableID::nfSetByteAt::nfSetByteAt( const sInitData &init ) : dsFunction( init.clsMUID,
"setByteAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // position
	p_AddParameter( init.clsInt ); // value
}
void deClassMutableID::nfSetByteAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	const int position = rt->GetValue( 0 )->GetInt();
	const int value = rt->GetValue( 1 )->GetInt();
	
	id.SetByteAt( position, value );
}

// public func void increment()
deClassMutableID::nfIncrement::nfIncrement( const sInitData &init ) : dsFunction( init.clsMUID,
"increment", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMutableID::nfIncrement::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	id.Increment();
}

// public func void incrementBy( int amount )
deClassMutableID::nfIncrementByInt::nfIncrementByInt( const sInitData &init ) : dsFunction( init.clsMUID,
"incrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // amount
}
void deClassMutableID::nfIncrementByInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	id.IncrementBy( rt->GetValue( 0 )->GetInt() );
}

// public func void incrementBy( UniqueID amount )
deClassMutableID::nfIncrementByUniqueID::nfIncrementByUniqueID( const sInitData &init ) : dsFunction( init.clsMUID,
"incrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUID ); // amount
}
void deClassMutableID::nfIncrementByUniqueID::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	id.IncrementBy( ds.GetClassUniqueID()->GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void incrementBy( MutableID amount )
deClassMutableID::nfIncrementByMutableID::nfIncrementByMutableID( const sInitData &init ) : dsFunction( init.clsMUID,
"incrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMUID ); // amount
}
void deClassMutableID::nfIncrementByMutableID::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMutableID = *( ( deClassMutableID* )GetOwnerClass() );
	id.IncrementBy( clsMutableID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void decrement()
deClassMutableID::nfDecrement::nfDecrement( const sInitData &init ) : dsFunction( init.clsMUID,
"decrement", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMutableID::nfDecrement::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	id.Decrement();
}

// public func void decrementBy( int amount )
deClassMutableID::nfDecrementByInt::nfDecrementByInt( const sInitData &init ) : dsFunction( init.clsMUID,
"decrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // amount
}
void deClassMutableID::nfDecrementByInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	id.DecrementBy( rt->GetValue( 0 )->GetInt() );
}

// public func void decrementBy( UniqueID amount )
deClassMutableID::nfDecrementByUniqueID::nfDecrementByUniqueID( const sInitData &init ) : dsFunction( init.clsMUID,
"decrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUID ); // amount
}
void deClassMutableID::nfDecrementByUniqueID::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	id.DecrementBy( ds.GetClassUniqueID()->GetUniqueID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void decrementBy( MutableID amount )
deClassMutableID::nfDecrementByMutableID::nfDecrementByMutableID( const sInitData &init ) : dsFunction( init.clsMUID,
"decrementBy", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMUID ); // amount
}
void deClassMutableID::nfDecrementByMutableID::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMutableID = *( ( deClassMutableID* )GetOwnerClass() );
	id.DecrementBy( clsMutableID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func void reset()
deClassMutableID::nfReset::nfReset( const sInitData &init ) : dsFunction( init.clsMUID,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMutableID::nfReset::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	
	id.Reset();
}

// public func void setFrom( UniqueID id )
deClassMutableID::nfSetFrom::nfSetFrom( const sInitData &init ) : dsFunction( init.clsMUID,
"setFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsUID ); // id
}
void deClassMutableID::nfSetFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	
	const decUniqueID &uid = ds.GetClassUniqueID()->GetUniqueID( rt->GetValue( 0 )->GetRealObject() );
	
	id = uid;
}

// public func void setFromMutable( MutableID id )
deClassMutableID::nfSetFromMutable::nfSetFromMutable( const sInitData &init ) : dsFunction( init.clsMUID,
"setFromMutable", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMUID ); // id
}
void deClassMutableID::nfSetFromMutable::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	
	const decUniqueID &uid = clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() );
	
	id = uid;
}

// public func UniqueID toUniqueID()
deClassMutableID::nfToUniqueID::nfToUniqueID( const sInitData &init ) : dsFunction( init.clsMUID,
"toUniqueID", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
}
void deClassMutableID::nfToUniqueID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	
	ds.GetClassUniqueID()->PushUniqueID( rt, id );
}

// public func UniqueID nextID()
deClassMutableID::nfNextID::nfNextID( const sInitData &init ) : dsFunction( init.clsMUID,
"nextID", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsUID ){
}
void deClassMutableID::nfNextID::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deScriptingDragonScript &ds = ( ( deClassMutableID* )GetOwnerClass() )->GetDS();
	
	ds.GetClassUniqueID()->PushUniqueID( rt, id );
	id.Increment();
}



// public func String toHexString()
deClassMutableID::nfToHexString::nfToHexString( const sInitData &init ) : dsFunction( init.clsMUID,
"toHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassMutableID::nfToHexString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	rt->PushString( id.ToHexString() );
}

// public func void setFromHexString( String string )
deClassMutableID::nfSetFromHexString::nfSetFromHexString( const sInitData &init ) : dsFunction( init.clsMUID,
"setFromHexString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // string
}
void deClassMutableID::nfSetFromHexString::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	id = decUniqueID( rt->GetValue( 0 )->GetString() );
}



// File Handling
//////////////////

// public func void readFromFile( FileReader reader )
deClassMutableID::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsMUID,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassMutableID::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsMUID.GetDS().GetClassFileReader();
	
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	const int byteCount = reader->ReadByte();
	id.SetBitCount( byteCount * 8 );
	int i;
	
	for( i=0; i<byteCount; i++ ){
		id.SetByteAt( i, reader->ReadByte() );
	}
}

// public func void writeToFile( FileWriter writer )
deClassMutableID::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsMUID,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassMutableID::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	const deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsMUID.GetDS().GetClassFileWriter();
	
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



// public func bool <( MutableID id )
deClassMutableID::nfOpLess::nfOpLess( const sInitData &init ) : dsFunction( init.clsMUID,
"<", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsMUID ); // id
}
void deClassMutableID::nfOpLess::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	rt->PushBool( id < clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool <=( MutableID id )
deClassMutableID::nfOpLessEqual::nfOpLessEqual( const sInitData &init ) : dsFunction( init.clsMUID,
"<=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsMUID ); // id
}
void deClassMutableID::nfOpLessEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	rt->PushBool( id <= clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool >( MutableID id )
deClassMutableID::nfOpGreater::nfOpGreater( const sInitData &init ) : dsFunction( init.clsMUID,
">", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsMUID ); // id
}
void deClassMutableID::nfOpGreater::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	rt->PushBool( id > clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool >=( MutableID id )
deClassMutableID::nfOpGreaterEqual::nfOpGreaterEqual( const sInitData &init ) : dsFunction( init.clsMUID,
">=", DSFT_OPERATOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsMUID ); // id
}
void deClassMutableID::nfOpGreaterEqual::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassMutableID &clsMUID = *( ( deClassMutableID* )GetOwnerClass() );
	rt->PushBool( id >= clsMUID.GetMutableID( rt->GetValue( 0 )->GetRealObject() ) );
}



// Misc
/////////

// public func int hashCode()
deClassMutableID::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsMUID, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassMutableID::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID * const id = ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id;
	
	rt->PushInt( ( intptr_t )id );
}

// public func bool equals( Object object )
deClassMutableID::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsMUID, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassMutableID::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
	deClassUniqueID * const clsMUID = ( deClassUniqueID* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsMUID ) ){
		rt->PushBool( false );
		
	}else{
		const decUniqueID &otherUniqueID = *( ( ( sMUIDNatDat* )p_GetNativeData( obj ) )->id );
		rt->PushBool( id == otherUniqueID );
	}
}

// public func String toString()
deClassMutableID::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsMUID,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassMutableID::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decUniqueID &id = *( ( ( sMUIDNatDat* )p_GetNativeData( myself ) )->id );
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

deClassMutableID::deClassMutableID( deScriptingDragonScript &ds ) :
dsClass( "MutableID", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sMUIDNatDat ) );
}

deClassMutableID::~deClassMutableID(){
}



// Management
///////////////

void deClassMutableID::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsMUID = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsUID = pDS.GetClassUniqueID();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewValue( init ) );
	AddFunction( new nfNewUniqueID( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetBitCount( init ) );
	AddFunction( new nfSetBitCount( init ) );
	AddFunction( new nfGetByteAt( init ) );
	AddFunction( new nfSetByteAt( init ) );
	AddFunction( new nfIncrement( init ) );
	AddFunction( new nfIncrementByInt( init ) );
	AddFunction( new nfIncrementByUniqueID( init ) );
	AddFunction( new nfIncrementByMutableID( init ) );
	AddFunction( new nfDecrement( init ) );
	AddFunction( new nfDecrementByInt( init ) );
	AddFunction( new nfDecrementByUniqueID( init ) );
	AddFunction( new nfDecrementByMutableID( init ) );
	AddFunction( new nfReset( init ) );
	
	AddFunction( new nfSetFrom( init ) );
	AddFunction( new nfSetFromMutable( init ) );
	
	AddFunction( new nfToUniqueID( init ) );
	AddFunction( new nfNextID( init ) );
	
	AddFunction( new nfToHexString( init ) );
	AddFunction( new nfSetFromHexString( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfOpLess( init ) );
	AddFunction( new nfOpLessEqual( init ) );
	AddFunction( new nfOpGreater( init ) );
	AddFunction( new nfOpGreaterEqual( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

decUniqueID &deClassMutableID::GetMutableID( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( sMUIDNatDat* )p_GetNativeData( myself->GetBuffer() ) )->id;
}

void deClassMutableID::PushMutableID( dsRunTime *rt, const decUniqueID &id ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sMUIDNatDat &nd = *( ( sMUIDNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.id = NULL;
	
	try{
		nd.id = new decUniqueID( id );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
