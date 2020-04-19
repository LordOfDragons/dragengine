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
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassConnection.h"
#include "deClassNetworkState.h"
#include "deClassNetworkMessage.h"
#include "deClassConnectionListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingConnection.h"
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public func void connectionClosed()
deClassConnectionListener::nfConnectionClosed::nfConnectionClosed( const sInitData &init ) :
dsFunction( init.clsConL, "connectionClosed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
}
void deClassConnectionListener::nfConnectionClosed::RunFunction( dsRunTime*, dsValue* ){
}

// public func void messageProgress( int bytesReceived )
deClassConnectionListener::nfMessageProgress::nfMessageProgress( const sInitData &init ) :
dsFunction( init.clsConL, "messageProgress", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // bytesReceived
}
void deClassConnectionListener::nfMessageProgress::RunFunction( dsRunTime*, dsValue* ){
}

// public func void messageReceived( NetworkMessage message )
deClassConnectionListener::nfMessageReceived::nfMessageReceived( const sInitData &init ) :
dsFunction( init.clsConL, "messageReceived", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsNM ); // message
}
void deClassConnectionListener::nfMessageReceived::RunFunction( dsRunTime*, dsValue* ){
}

// public func bool linkState( NetworkState state, NetworkMessage message )
deClassConnectionListener::nfLinkState::nfLinkState( const sInitData &init ) :
dsFunction( init.clsConL, "linkState", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsNS ); // state
	p_AddParameter( init.clsNM ); // message
}
void deClassConnectionListener::nfLinkState::RunFunction( dsRunTime*, dsValue* ){
}



// Class deClassConnectionListener
////////////////////////////////////

// Constructor, Destructors
/////////////////////////////

deClassConnectionListener::deClassConnectionListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass( "ConnectionListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// set parser info
	GetParserInfo()->SetParent( DENS_NETWORKING );
	
	// do the rest
	p_SetNativeDataSize( 0 );
}

deClassConnectionListener::~deClassConnectionListener(){
}



// Management
///////////////

void deClassConnectionListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	pClsNM = pScrMgr->GetClassNetworkMessage();
	pClsNS = pScrMgr->GetClassNetworkState();
	
	init.clsConL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsBool = engine->GetClassBool();
	init.clsNM = pClsNM;
	init.clsNS = pClsNS;
	
	// add functions
	AddFunction( new nfConnectionClosed( init ) ); // function 0
	AddFunction( new nfMessageProgress( init ) ); // function 1
	AddFunction( new nfMessageReceived( init ) ); // function 2
	AddFunction( new nfLinkState( init ) ); // function 3
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexMessageReceived = funcList.GetIndexOf( GetFunction( 2 ) );
	pFuncIndexLinkState = funcList.GetIndexOf( GetFunction( 3 ) );
}

