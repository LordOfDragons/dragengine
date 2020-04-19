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

#include "deClassColliderListener.h"
#include "deClassCollider.h"
#include "../graphics/deClassComponent.h"
#include "../physics/deClassCollisionInfo.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

#include <libdscript/exceptions.h>


// Native Functions
/////////////////////

// public func void collisionResponse( Collider owner, CollisionInfo info, float elapsed )
deClassColliderListener::nfCollisionResponse::nfCollisionResponse( const sInitData &init ) :
dsFunction( init.clsCLL, "collisionResponse", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // owner
	p_AddParameter( init.clsCI ); // info
}
void deClassColliderListener::nfCollisionResponse::RunFunction( dsRunTime *RT, dsValue *This ){
}

// public func bool canHitCollider( Collider owner, Collider collider )
deClassColliderListener::nfCanHitCollider::nfCanHitCollider( const sInitData &init ) :
dsFunction( init.clsCLL, "canHitCollider", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsCol ); // owner
	p_AddParameter( init.clsCol ); // collider
}
void deClassColliderListener::nfCanHitCollider::RunFunction( dsRunTime *RT, dsValue *This ){
}

// public func void colliderChanged( Collider owner )
deClassColliderListener::nfColliderChanged::nfColliderChanged( const sInitData &init ) :
dsFunction( init.clsCLL, "colliderChanged", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // owner
}
void deClassColliderListener::nfColliderChanged::RunFunction( dsRunTime *RT, dsValue *This ){
}



// class deClassColliderListener
//////////////////////////////////

// constructor
deClassColliderListener::deClassColliderListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass( "ColliderListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	// set parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	// do the rest
	p_SetNativeDataSize( 0 );
}
deClassColliderListener::~deClassColliderListener(){
}

// management
void deClassColliderListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCLL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsCI = pScrMgr->GetClassCollisionInfo();
	init.clsCol = pScrMgr->GetClassCollider();
	init.clsComp = pScrMgr->GetClassComponent();
	
	// add functions
	AddFunction( new nfCollisionResponse( init ) ); // function 0
	AddFunction( new nfCanHitCollider( init ) ); // function 1
	AddFunction( new nfColliderChanged( init ) ); // function 2
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexCollisionResponse = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexCanHitCollider = funcList.GetIndexOf( GetFunction( 1 ) );
	pFuncIndexColliderChanged = funcList.GetIndexOf( GetFunction( 2 ) );
}

