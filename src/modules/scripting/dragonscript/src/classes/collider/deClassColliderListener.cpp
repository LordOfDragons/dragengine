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

