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
	if( pListener ){
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pListener );
	}
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
