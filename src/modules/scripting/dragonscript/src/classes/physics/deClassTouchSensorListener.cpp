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

#include "deClassTouchSensorListener.h"
#include "deClassTouchSensor.h"
#include "../collider/deClassCollider.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>

#include <libdscript/exceptions.h>


// native functions
/////////////////////



// public func void colliderEntered( int shape, Collider collider )
deClassTouchSensorListener::nfColliderEntered::nfColliderEntered( const sInitData &init ) :
dsFunction( init.clsTSL, "colliderEntered", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassTouchSensorListener::nfColliderEntered::RunFunction( dsRunTime *RT, dsValue *This ){
}

// public func void colliderLeft( int shape, Collider collider )
deClassTouchSensorListener::nfColliderLeft::nfColliderLeft( const sInitData &init ) :
dsFunction( init.clsTSL, "colliderLeft", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // collider
}
void deClassTouchSensorListener::nfColliderLeft::RunFunction( dsRunTime *RT, dsValue *This ){
}



// class deClassTouchSensorListener
//////////////////////////////////

// constructor
deClassTouchSensorListener::deClassTouchSensorListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass( "TouchSensorListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	// do the rest
	p_SetNativeDataSize( 0 );
}
deClassTouchSensorListener::~deClassTouchSensorListener(){
}

// management
void deClassTouchSensorListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsTSL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsCol = pScrMgr->GetClassCollider();
	
	// add functions
	AddFunction( new nfColliderEntered( init ) ); // function 0
	AddFunction( new nfColliderLeft( init ) ); // function 1
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexColliderEntered = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexColliderLeft = funcList.GetIndexOf( GetFunction( 1 ) );
}
