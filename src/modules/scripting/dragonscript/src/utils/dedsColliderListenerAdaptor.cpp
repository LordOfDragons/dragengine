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

#include "dedsColliderListenerAdaptor.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/collider/deClassColliderListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <dragengine/resources/collider/deCollisionInfo.h>

#include <libdscript/exceptions.h>



// Class dedsColliderListenerAdaptor
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsColliderListenerAdaptor::dedsColliderListenerAdaptor( deScriptingDragonScript &ds ) :
pDS( ds ),
pListener( NULL ),
pHasListener( false )
{
	pListener = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassColliderListener() );
}

dedsColliderListenerAdaptor::~dedsColliderListenerAdaptor(){
	if( ! pListener ){
		return;
	}
	
	pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pListener );
	pListener = NULL;
}



// Management
///////////////

void dedsColliderListenerAdaptor::SetListener( dsRealObject *object ){
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

void dedsColliderListenerAdaptor::Reset(){
	pDS.GetCollisionInfo()->Clear();
}



// Notifications
//////////////////

void dedsColliderListenerAdaptor::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! pHasListener ){
		return;
	}
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCollisionResponse();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollisionInfo &clsCI = *pDS.GetClassCollisionInfo();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		clsCI.PushInfo( rt, info ); // info
		clsCol.PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pListener, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

bool dedsColliderListenerAdaptor::CanHitCollider( deCollider *owner, deCollider *collider ){
	if( ! pHasListener ){
		return false;
	}
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	bool canHitCollider = false;
	
	try{
		clsCol.PushCollider( rt, collider ); // collider
		clsCol.PushCollider( rt, NULL ); // owner
		rt->RunFunctionFast( pListener, funcIndex );
		
		canHitCollider = rt->GetReturnBool();
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
	return canHitCollider;
}
