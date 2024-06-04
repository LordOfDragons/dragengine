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

#include <stdio.h>
#include <stdlib.h>


#include "dedsColliderListenerClosest.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/collider/deClassColliderListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>

#include <libdscript/exceptions.h>



// Class dedsColliderListenerClosest
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsColliderListenerClosest::dedsColliderListenerClosest( deScriptingDragonScript &ds ) :
pDS( ds ),

pListener( NULL ),
pHasListener( false ),

pOwner( NULL ),
pHasCollision( false )
{
	pListener = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassColliderListener() );
}

dedsColliderListenerClosest::~dedsColliderListenerClosest(){
	if( ! pListener ){
		return;
	}
	
	pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pListener );
	pListener = NULL;
}



// Management
///////////////

void dedsColliderListenerClosest::SetListener( dsRealObject *object ){
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pListener, object );
		rt.CastValueTo( pListener, pListener, pDS.GetClassColliderListener() );
		pHasListener = true;
		
	}else{
		rt.SetNull( pListener, pDS.GetClassColliderListener() );
		pHasListener = false;
	}
}

void dedsColliderListenerClosest::Reset(){
	pDS.GetCollisionInfo()->Clear();
	pOwner = NULL;
	pHasCollision = false;
}

void dedsColliderListenerClosest::NotifyListener(){
	if( ! pHasCollision || ! pHasListener ){
		return;
	}
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCollisionResponse();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollisionInfo &clsCI = *pDS.GetClassCollisionInfo();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		clsCI.PushInfo( rt, pDS.GetCollisionInfo() ); // info
		clsCol.PushCollider( rt, pOwner ); // owner
		rt->RunFunctionFast( pListener, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}



// Notifications
//////////////////

void dedsColliderListenerClosest::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	deCollisionInfo &colInfo = *pDS.GetCollisionInfo();
	
	if( pHasCollision && info->GetDistance() >= colInfo.GetDistance() ){
		return;
	}
	
	if( info->IsHTSector() ){
		colInfo.SetHTSector( info->GetHeightTerrain(), info->GetHTSector() );
		
	}else{
		colInfo.SetCollider( info->GetCollider(), info->GetBone(), info->GetShape(), info->GetFace() );
	}
	
	colInfo.SetDistance( info->GetDistance() );
	colInfo.SetNormal( info->GetNormal() );
	
	pOwner = owner;
	pHasCollision = true;
}

bool dedsColliderListenerClosest::CanHitCollider( deCollider *owner, deCollider *collider ){
	if( ! pHasListener ){
		return false;
	}
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	bool canHitCollider = false;
	
	try{
		clsCol.PushCollider( rt, collider ); // collider
		clsCol.PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pListener, funcIndex ); // canHitCollider(owner, collider)
		
		canHitCollider = rt->GetReturnBool();
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
	return canHitCollider;
}
