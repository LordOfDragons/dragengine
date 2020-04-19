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
#include "deClassNetworkStateListener.h"
#include "deClassNetworkState.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "dragengine/deEngine.h"
#include "dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h"
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public func void stateValueChanged( int index )
deClassNetworkStateListener::nfStateValueChanged::nfStateValueChanged( const sInitData &init ) :
dsFunction( init.clsNSL, "stateValueChanged", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassNetworkStateListener::nfStateValueChanged::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// class deClassNetworkStateListener
//////////////////////////////////////

// constructor
deClassNetworkStateListener::deClassNetworkStateListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass( "NetworkStateListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// set parser info
	GetParserInfo()->SetParent( DENS_NETWORKING );
	
	// do the rest
	p_SetNativeDataSize( 0 );
}
deClassNetworkStateListener::~deClassNetworkStateListener(){
}

// management
void deClassNetworkStateListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsNSL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	
	// add functions
	AddFunction( new nfStateValueChanged( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

