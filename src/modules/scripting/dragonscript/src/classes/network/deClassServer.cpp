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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassServer.h"
#include "deClassServerListener.h"
#include "../../peers/dedsServer.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/resources/deResource.h"
#include "dragengine/resources/deResourceManager.h"
#include "dragengine/resources/network/deServer.h"
#include "dragengine/resources/network/deServerManager.h"
#include "dragengine/deEngine.h"
#include <libdscript/exceptions.h>



// native structure
struct sSvrNatDat{
	deServer *server;
};



// Native functions
/////////////////////

// Constructor, destructors
/////////////////////////////

// public func new()
deClassServer::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSvr,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassServer::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSvrNatDat *nd = ( sSvrNatDat* )p_GetNativeData( myself );
	deClassServer *clsSvr = ( deClassServer* )GetOwnerClass();
	deServerManager *svrMgr = clsSvr->GetGameEngine()->GetServerManager();
	
	// clear (important)
	nd->server = NULL;
	
	// create server
	nd->server = svrMgr->CreateServer();
	if( ! nd->server ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassServer::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSvr,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassServer::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSvrNatDat *nd = ( sSvrNatDat* )p_GetNativeData( myself );
	
	if( nd->server ){
		nd->server->FreeReference();
		nd->server = NULL;
	}
}



// Management
///////////////

// public func String getAddress()
deClassServer::nfGetAddress::nfGetAddress( const sInitData &init ) : dsFunction( init.clsSvr,
"getAddress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassServer::nfGetAddress::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	
	rt->PushString( server->GetAddress() );
}

// public func bool listenOn( String address )
deClassServer::nfListenOn::nfListenOn( const sInitData &init ) : dsFunction( init.clsSvr,
"listenOn", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsStr ); // address
}
void deClassServer::nfListenOn::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	
	rt->PushBool( server->ListenOn( rt->GetValue( 0 )->GetString() ) );
}

// public func void stopListening()
deClassServer::nfStopListening::nfStopListening( const sInitData &init ) : dsFunction( init.clsSvr,
"stopListening", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassServer::nfStopListening::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	
	server->StopListening();
}



// Notifications
//////////////////

// public func ServerListener getServerListener()
deClassServer::nfGetServerListener::nfGetServerListener( const sInitData &init ) : dsFunction( init.clsSvr,
"getServerListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSvrL ){
}
void deClassServer::nfGetServerListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	dedsServer *scrServer = ( dedsServer* )server->GetPeerScripting();
	deClassServer *clsServer = ( deClassServer* )GetOwnerClass();
	
	rt->PushObject( scrServer->GetCallback(), clsServer->GetClassServerListener() );
}

// public func void setServerListener( ServerListener listener )
deClassServer::nfSetServerListener::nfSetServerListener( const sInitData &init ) : dsFunction( init.clsSvr,
"setServerListener", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSvrL ); // listener
}
void deClassServer::nfSetServerListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	dedsServer *scrServer = ( dedsServer* )server->GetPeerScripting();
	
	scrServer->SetCallback( rt->GetValue( 0 )->GetRealObject() );
}



// Misc
/////////

// public func int hashCode()
deClassServer::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsSvr, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassServer::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	
	rt->PushInt( ( intptr_t )server );
}

// public func bool equals( Object object )
deClassServer::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSvr, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassServer::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deServer *server = ( ( sSvrNatDat* )p_GetNativeData( myself ) )->server;
	deClassServer *clsSvr = ( deClassServer* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSvr ) ){
		rt->PushBool( false );
	}else{
		deServer *otherServer = ( ( sSvrNatDat* )p_GetNativeData( obj ) )->server;
		rt->PushBool( server == otherServer );
	}
}



// Class deClassServer
/////////////////////////

// Constructor
////////////////

deClassServer::deClassServer( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "Server", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store informations into parser info
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sSvrNatDat ) );
}

deClassServer::~deClassServer(){
}



// Management
///////////////

void deClassServer::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsSvrL = pScrMgr->GetClassServerListener();
	
	init.clsSvr = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsSvrL = pClsSvrL;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetAddress( init ) );
	
	AddFunction( new nfListenOn( init ) );
	AddFunction( new nfStopListening( init ) );
	
	AddFunction( new nfGetServerListener( init ) );
	AddFunction( new nfSetServerListener( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deServer *deClassServer::GetServer( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSvrNatDat* )p_GetNativeData( myself->GetBuffer() ) )->server;
}

void deClassServer::PushServer( dsRunTime *rt, deServer *server ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! server ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sSvrNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->server = server;
	server->AddReference();
}
