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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "deClassStringID.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native data
////////////////

struct sStrIDNatDat{
	int index;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassStringID::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsStringID, DSFUNC_CONSTRUCTOR,
DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassStringID::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sStrIDNatDat &nd = *( ( sStrIDNatDat* )p_GetNativeData( myself ) );
	
	nd.index = 0; // 0 is the empty string inserted first in the table
}

// public func new( String string )
deClassStringID::nfNew2::nfNew2( const sInitData &init ) :
dsFunction( init.clsStringID, DSFUNC_CONSTRUCTOR,
DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // string
}
void deClassStringID::nfNew2::RunFunction( dsRunTime *rt, dsValue *myself ){
	sStrIDNatDat &nd = *( ( sStrIDNatDat* )p_GetNativeData( myself ) );
	deClassStringID &sclass = *( ( deClassStringID* )GetOwnerClass() );
	
	nd.index = sclass.InsertString( rt->GetValue( 0 )->GetString() );
}

// public func destructor()
deClassStringID::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsStringID, DSFUNC_DESTRUCTOR,
DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassStringID::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Management
///////////////

// public func String getString()
deClassStringID::nfGetString::nfGetString( const sInitData &init ) :
dsFunction( init.clsStringID, "getString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassStringID::nfGetString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int index = ( ( sStrIDNatDat* )p_GetNativeData( myself ) )->index;
	const deClassStringID &clsStringID = *( ( deClassStringID* )GetOwnerClass() );
	
	rt->PushString( clsStringID.GetStringAt( index ) );
}



// File Handling
//////////////////

// static public func StringID readFromFile( FileReader reader )
deClassStringID::nfReadFromFile::nfReadFromFile( const sInitData &init ) :
dsFunction( init.clsStringID, "readFromFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStringID ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassStringID::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassStringID &sclass = *( ( deClassStringID* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *sclass.GetDS().GetClassFileReader();
	
	decBaseFileReader * const reader = clsFileReader.GetFileReader(
		rt->GetValue( 0 )->GetRealObject() );
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	sclass.PushStringID( rt, sclass.InsertString( reader->ReadString8() ) );
}

// public func void writeToFile( FileWriter writer )
deClassStringID::nfWriteToFile::nfWriteToFile( const sInitData &init ) :
dsFunction( init.clsStringID, "writeToFile", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassStringID::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int index = ( ( sStrIDNatDat* )p_GetNativeData( myself ) )->index;
	const deClassStringID &sclass = *( ( deClassStringID* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *sclass.GetDS().GetClassFileWriter();
	
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter(
		rt->GetValue( 0 )->GetRealObject() );
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteString8( sclass.GetStringAt( index ) );
}



// public func int hashCode()
deClassStringID::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsStringID, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassStringID::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushInt( ( ( sStrIDNatDat* )p_GetNativeData( myself ) )->index );
}

// public func bool equals( Object object )
deClassStringID::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsStringID, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // object
}
void deClassStringID::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int index = ( ( sStrIDNatDat* )p_GetNativeData( myself ) )->index;
	deClassStringID * const sclass = ( deClassStringID* )GetOwnerClass();
	dsClass * const scString = rt->GetEngine()->GetClassString();
	dsValue * const object = rt->GetValue( 0 );
	
	if( p_IsObjOfType( object, sclass ) ){
		const int otherIndex = ( ( sStrIDNatDat* )p_GetNativeData( object ) )->index;
		rt->PushBool( index == otherIndex );
		
	}else if( p_IsObjOfType( object, scString ) ){
		rt->PushBool( sclass->GetStringAt( index ) == object->GetString() );
		
	}else{
		rt->PushBool( false );
	}
}

// public func String toString()
deClassStringID::nfToString::nfToString( const sInitData &init ) :
dsFunction( init.clsStringID, "toString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassStringID::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int index = ( ( sStrIDNatDat* )p_GetNativeData( myself ) )->index;
	const deClassStringID &sclass = *( ( deClassStringID* )GetOwnerClass() );
	
	rt->PushString( sclass.GetStringAt( index ) );
}



// Class deClassStringID
//////////////////////////

// Constructor
////////////////

deClassStringID::deClassStringID( deScriptingDragonScript &ds ) :
dsClass( "StringID", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	// empty string is the first entry in the table
	pStrings.Add( "" );
	pLookUpTable.SetAt( "", ( void* )( intptr_t )0 );
	
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sStrIDNatDat ) );
}

deClassStringID::~deClassStringID(){
}



// Management
///////////////

void deClassStringID::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsStringID = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNew2( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetString( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfToString( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

int deClassStringID::GetStringCount() const{
	return pStrings.GetCount();
}

int deClassStringID::GetStringID( dsRealObject *myself ){
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	return ( ( sStrIDNatDat* )p_GetNativeData( myself->GetBuffer() ) )->index;
}

void deClassStringID::PushStringID( dsRunTime *rt, int index ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sStrIDNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->index = index;
}

int deClassStringID::InsertString( const char *string ){
	if( ! string ){
		DSTHROW( dueInvalidParam );
	}
	
	void *entry;
	if( pLookUpTable.GetAt( string, &entry ) ){
		return ( int )( intptr_t )entry;
	}
	
	const int index = pStrings.GetCount();
	pStrings.Add( string );
	pLookUpTable.SetAt( string, ( void* )( intptr_t )index );
	return index;
}

const decString &deClassStringID::GetStringAt( int index ) const{
	return pStrings.GetAt( index );
}
