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

#include "deClassNetworkMessage.h"
#include "../file/deClassFileReader.h"
#include "../file/deClassFileWriter.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/deNetworkMessageReader.h>
#include <dragengine/resources/network/deNetworkMessageWriter.h>

#include <libdscript/exceptions.h>


// native structure
struct sNMNatDat{
	deNetworkMessage *message;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassNetworkMessage::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNetworkMessage::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	sNMNatDat &nd = *( ( sNMNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.message = NULL;
	
	// create message
	nd.message = new deNetworkMessage;
}

// public func destructor()
deClassNetworkMessage::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNetworkMessage::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sNMNatDat &nd = *( ( sNMNatDat* )p_GetNativeData( myself ) );
	
	if( nd.message ){
		nd.message->FreeReference();
		nd.message = NULL;
	}
}



// Management
///////////////

// public func int getDataLength()
deClassNetworkMessage::nfGetDataLength::nfGetDataLength( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "getDataLength", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassNetworkMessage::nfGetDataLength::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deNetworkMessage &message = *( ( ( sNMNatDat* )p_GetNativeData( myself ) )->message );
	rt->PushInt( message.GetDataLength() );
}

// public func void clear()
deClassNetworkMessage::nfClear::nfClear( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "clear", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassNetworkMessage::nfClear::RunFunction( dsRunTime*, dsValue *myself ){
	deNetworkMessage &message = *( ( ( sNMNatDat* )p_GetNativeData( myself ) )->message );
	message.Clear();
}



// public func FileReader getReader()
deClassNetworkMessage::nfGetReader::nfGetReader( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "getReader", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileReader ){
}
void deClassNetworkMessage::nfGetReader::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNetworkMessage * const message = ( ( sNMNatDat* )p_GetNativeData( myself ) )->message;
	deScriptingDragonScript &ds = ( ( deClassNetworkMessage* )GetOwnerClass() )->GetDS();
	
	decBaseFileReaderReference reader;
	reader.TakeOver( new deNetworkMessageReader( message ) );
	ds.GetClassFileReader()->PushFileReader( rt, reader );
}

// public func FileWriter getWriter( bool append )
deClassNetworkMessage::nfGetWriter::nfGetWriter( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "getWriter", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFileWrite ){
	p_AddParameter( init.clsBool ); // append
}
void deClassNetworkMessage::nfGetWriter::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNetworkMessage * const message = ( ( sNMNatDat* )p_GetNativeData( myself ) )->message;
	deScriptingDragonScript &ds = ( ( deClassNetworkMessage* )GetOwnerClass() )->GetDS();
	
	const bool append = rt->GetValue( 0 )->GetBool();
	decBaseFileWriterReference writer;
	writer.TakeOver( new deNetworkMessageWriter( message, append ) );
	ds.GetClassFileWriter()->PushFileWriter( rt, writer );
}



// Misc
/////////

// public func int hashCode()
deClassNetworkMessage::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassNetworkMessage::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNetworkMessage * const message = ( ( sNMNatDat* )p_GetNativeData( myself ) )->message;
	rt->PushInt( ( int )( intptr_t )message );
}

// public func bool equals( Object object )
deClassNetworkMessage::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsNetworkMessage, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // object
}
void deClassNetworkMessage::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deNetworkMessage * const message = ( ( sNMNatDat* )p_GetNativeData( myself ) )->message;
	deClassNetworkMessage *clsNetworkMessage = ( deClassNetworkMessage* )GetOwnerClass();
	dsValue * const object = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( object, clsNetworkMessage ) ){
		rt->PushBool( false );
		
	}else{
		deNetworkMessage * const otherMessage = ( ( sNMNatDat* )p_GetNativeData( object ) )->message;
		rt->PushBool( message == otherMessage );
	}
}



// Class deClassNetworkMessage
////////////////////////////////

// Constructor
////////////////

deClassNetworkMessage::deClassNetworkMessage( deScriptingDragonScript &ds ) :
dsClass( "NetworkMessage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_NETWORKING );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sNMNatDat ) );
}

deClassNetworkMessage::~deClassNetworkMessage(){
}



// Management
///////////////

void deClassNetworkMessage::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsNetworkMessage = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWrite = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetDataLength( init ) );
	AddFunction( new nfClear( init ) );
	AddFunction( new nfGetReader( init ) );
	AddFunction( new nfGetWriter( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deNetworkMessage *deClassNetworkMessage::GetNetworkMessage( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sNMNatDat* )p_GetNativeData( myself->GetBuffer() ) )->message;
}

void deClassNetworkMessage::PushNetworkMessage( dsRunTime *rt, deNetworkMessage *message ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! message ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sNMNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->message = message;
	message->AddReference();
}
