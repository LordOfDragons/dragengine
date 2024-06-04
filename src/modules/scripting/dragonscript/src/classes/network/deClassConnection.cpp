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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassConnection.h"
#include "deClassNetworkState.h"
#include "deClassNetworkMessage.h"
#include "deClassConnectionListener.h"
#include "../../deClassPathes.h"
#include "../../peers/dedsConnection.h"
#include "../../deScriptingDragonScript.h"
#include "dragengine/resources/deResource.h"
#include "dragengine/resources/deResourceManager.h"
#include "dragengine/resources/network/deConnection.h"
#include "dragengine/resources/network/deNetworkState.h"
#include "dragengine/resources/network/deNetworkMessage.h"
#include "dragengine/resources/network/deConnectionManager.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sConNatDat{
	deConnection *connection;
};



// Native functions
/////////////////////

// Constructors, destructors
//////////////////////////////

// public func new()
deClassConnection::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCon,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassConnection::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sConNatDat *nd = ( sConNatDat* )p_GetNativeData( myself );
	deClassConnection *clsCon = ( deClassConnection* )GetOwnerClass();
	deConnectionManager *connectionMgr = clsCon->GetGameEngine()->GetConnectionManager();
	
	// clear (important)
	nd->connection = NULL;
	
	// create connection
	nd->connection = connectionMgr->CreateConnection();
	if( ! nd->connection ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassConnection::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCon,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassConnection::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sConNatDat *nd = ( sConNatDat* )p_GetNativeData( myself );
	
	if( nd->connection ){
		nd->connection->FreeReference();
		nd->connection = NULL;
	}
}



// Management
///////////////

// public func String getLocalAddress()
deClassConnection::nfGetLocalAddress::nfGetLocalAddress( const sInitData &init ) : dsFunction( init.clsCon,
"getLocalAddress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassConnection::nfGetLocalAddress::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	rt->PushString( connection->GetLocalAddress() );
}

// public func String getRemoteAddress()
deClassConnection::nfGetRemoteAddress::nfGetRemoteAddress( const sInitData &init ) : dsFunction( init.clsCon,
"getRemoteAddress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassConnection::nfGetRemoteAddress::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	rt->PushString( connection->GetRemoteAddress() );
}

// public func bool getConnected()
deClassConnection::nfGetConnected::nfGetConnected( const sInitData &init ) : dsFunction( init.clsCon,
"getConnected", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassConnection::nfGetConnected::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	rt->PushBool( connection->GetConnected() );
}



// public func bool connectTo( String address )
deClassConnection::nfConnectTo::nfConnectTo( const sInitData &init ) : dsFunction( init.clsCon,
"connectTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsStr ); // address
}
void deClassConnection::nfConnectTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	rt->PushBool( connection->ConnectTo( rt->GetValue( 0 )->GetString() ) );
}

// public func void disconnect()
deClassConnection::nfDisconnect::nfDisconnect( const sInitData &init ) : dsFunction( init.clsCon,
"disconnect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassConnection::nfDisconnect::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	connection->Disconnect();
}

// public func void sendMessage( NetworkMessage message, int maxDelay )
deClassConnection::nfSendMessage::nfSendMessage( const sInitData &init ) : dsFunction( init.clsCon,
"sendMessage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNM ); // message
	p_AddParameter( init.clsInt ); // maxDelay
}
void deClassConnection::nfSendMessage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	deClassNetworkMessage *clsNM = ( ( deClassConnection* )GetOwnerClass() )->GetClassNetworkMessage();
	
	dsRealObject *objMessage = rt->GetValue( 0 )->GetRealObject();
	if( ! objMessage ) DSTHROW( dueInvalidParam );
	
	deNetworkMessage * const message = clsNM->GetNetworkMessage( objMessage );
	const int maxDelay = rt->GetValue( 1 )->GetInt();
	
	connection->SendMessage( message, maxDelay );
}

// public func void sendReliableMessage( NetworkMessage message )
deClassConnection::nfSendReliableMessage::nfSendReliableMessage( const sInitData &init ) : dsFunction( init.clsCon,
"sendReliableMessage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNM ); // message
}
void deClassConnection::nfSendReliableMessage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	deClassNetworkMessage *clsNM = ( ( deClassConnection* )GetOwnerClass() )->GetClassNetworkMessage();
	
	dsRealObject *objMessage = rt->GetValue( 0 )->GetRealObject();
	if( ! objMessage ) DSTHROW( dueInvalidParam );
	
	connection->SendReliableMessage( clsNM->GetNetworkMessage( objMessage ) );
}

// public func void linkState( NetworkMessage message, NetworkState state, bool readOnly )
deClassConnection::nfLinkState::nfLinkState( const sInitData &init ) : dsFunction( init.clsCon,
"linkState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNM ); // message
	p_AddParameter( init.clsNS ); // state
	p_AddParameter( init.clsBool ); // readOnly
}
void deClassConnection::nfLinkState::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	deClassNetworkMessage *clsNM = ( ( deClassConnection* )GetOwnerClass() )->GetClassNetworkMessage();
	deClassNetworkState *clsNS = ( ( deClassConnection* )GetOwnerClass() )->GetClassNetworkState();
	
	dsRealObject *objMessage = rt->GetValue( 0 )->GetRealObject();
	dsRealObject *objState = rt->GetValue( 1 )->GetRealObject();
	const bool readOnly = rt->GetValue( 2 )->GetBool();
	if( ! objMessage || ! objState ) DSTHROW( dueInvalidParam );
	
	deNetworkMessage * const message = clsNM->GetNetworkMessage( objMessage );
	deNetworkState * const state = clsNS->GetNetworkState( objState );
	
	connection->LinkState( message, state, readOnly );
}



// Notifications
//////////////////

// public func ConnectionListener getConnectionListener()
deClassConnection::nfGetConnectionListener::nfGetConnectionListener( const sInitData &init ) : dsFunction( init.clsCon,
"getConnectionListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsConL ){
}
void deClassConnection::nfGetConnectionListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	dedsConnection *scrConnection = ( dedsConnection* )connection->GetPeerScripting();
	deClassConnection *clsConnection = ( deClassConnection* )GetOwnerClass();
	
	if( scrConnection ){
		rt->PushObject( scrConnection->GetCallback(), clsConnection->GetClassConnectionListener() );
		
	}else{
		rt->PushObject( NULL, clsConnection->GetClassConnectionListener() );
	}
}

// public func void setConnectionListener( ConnectionListener listener )
deClassConnection::nfSetConnectionListener::nfSetConnectionListener( const sInitData &init ) : dsFunction( init.clsCon,
"setConnectionListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsConL ); // listener
}
void deClassConnection::nfSetConnectionListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	dedsConnection *scrConnection = ( dedsConnection* )connection->GetPeerScripting();
	if( scrConnection ){
		scrConnection->SetCallback( rt->GetValue( 0 )->GetRealObject() );
	}
}



// Misc
/////////

// public func int hashCode()
deClassConnection::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCon, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassConnection::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	
	rt->PushInt( ( int )( intptr_t )connection );
}

// public func bool equals( Object object )
deClassConnection::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCon, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassConnection::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deConnection *connection = ( ( sConNatDat* )p_GetNativeData( myself ) )->connection;
	deClassConnection *clsCon = ( deClassConnection* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCon ) ){
		rt->PushBool( false );
	}else{
		deConnection *otherConnection = ( ( sConNatDat* )p_GetNativeData( obj ) )->connection;
		rt->PushBool( connection == otherConnection );
	}
}



// Class deClassConnection
//////////////////////////////

// Constructor
////////////////

deClassConnection::deClassConnection( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "Connection", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store information into parser info
	GetParserInfo()->SetParent( DENS_NETWORKING );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sConNatDat ) );
}

deClassConnection::~deClassConnection(){
}



// Management
///////////////

void deClassConnection::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsNS = pScrMgr->GetClassNetworkState();
	pClsNM = pScrMgr->GetClassNetworkMessage();
	pClsConL = pScrMgr->GetClassConnectionListener();
	
	init.clsCon = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsNS = pClsNS;
	init.clsNM = pClsNM;
	init.clsConL = pClsConL;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetLocalAddress( init ) );
	AddFunction( new nfGetRemoteAddress( init ) );
	AddFunction( new nfGetConnected( init ) );
	
	AddFunction( new nfConnectTo( init ) );
	AddFunction( new nfDisconnect( init ) );
	AddFunction( new nfSendMessage( init ) );
	AddFunction( new nfSendReliableMessage( init ) );
	AddFunction( new nfLinkState( init ) );
	
	AddFunction( new nfGetConnectionListener( init ) );
	AddFunction( new nfSetConnectionListener( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deConnection *deClassConnection::GetConnection( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sConNatDat* )p_GetNativeData( myself->GetBuffer() ) )->connection;
}

void deClassConnection::PushConnection( dsRunTime *rt, deConnection *connection ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! connection ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sConNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->connection = connection;
	connection->AddReference();
}
