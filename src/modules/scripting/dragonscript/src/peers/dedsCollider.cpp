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

#include "dedsCollider.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/collider/deClassColliderListener.h"
#include "../classes/collider/deClassColliderBreakingListener.h"
#include "../classes/physics/deClassCollisionInfo.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class dedsCollider
///////////////////////

// Constructor, destructor
////////////////////////////

dedsCollider::dedsCollider( deScriptingDragonScript &ds, deCollider *collider ) :
pDS( ds ),
pCollider( collider ),
pValOwner( NULL ),
pEnableCanHitCallback( false ),
pValCB( NULL ),
pHasCB( false ),
pValCBBreaking( NULL ),
pHasCBBreaking( false )
{
	if( ! collider ){
		DSTHROW( dueInvalidParam );
	}
	
	dsRunTime &rt = *ds.GetScriptEngine()->GetMainRunTime();
	
	pValOwner = rt.CreateValue();
	pValCB = rt.CreateValue( pDS.GetClassColliderListener() );
	pValCBBreaking = rt.CreateValue( ds.GetClassColliderBreakingListener() );
}

dedsCollider::~dedsCollider(){
	// check if the collider resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the collider resource
	// being deleted due to links breaking while freeing the value. if this is the
	// case delay the deletion until a safe time
	if( pCollider && pCollider->GetRefCount() > 0 ){
		if( pValCBBreaking ){
			if( pValCBBreaking->GetRealObject() ){
				pDS.AddValueDeleteLater( pValCBBreaking );
			}
			pValCBBreaking = NULL;
			pHasCBBreaking = false;
		}
		
		if( pValCB ){
			if( pValCB->GetRealObject() ){
				pDS.AddValueDeleteLater( pValCB );
			}
			pValCB = NULL;
			pHasCB = false;
		}
		
		if( pValOwner ){
			if( pValOwner->GetRealObject() ){
				pDS.AddValueDeleteLater( pValOwner );
			}
			pValOwner = NULL;
		}
		
	}else{
		if( pValCBBreaking ){
			if( pValCBBreaking->GetRealObject() ){
				pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCBBreaking );
			}
			pValCBBreaking = NULL;
			pHasCBBreaking = false;
		}
		
		if( pValCB ){
			if( pValCB->GetRealObject() ){
				pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
			}
			pValCB = NULL;
			pHasCB = false;
		}
		
		if( pValOwner ){
			if( pValOwner->GetRealObject() ){
				pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValOwner );
			}
			pValOwner = NULL;
		}
	}
}



// Management
///////////////

void dedsCollider::SetEnableCanHitCallback( bool enable ){
	pEnableCanHitCallback = enable;
}



dsRealObject *dedsCollider::GetOwner() const{
	return pValOwner->GetRealObject();
}

void dedsCollider::SetOwner( dsRealObject *object ){
	if( ! pValOwner ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValOwner, object );
		
	}else{
		rt.ClearValue( pValOwner );
	}
}



dsRealObject *dedsCollider::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsCollider::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		rt.CastValueTo( pValCB, pValCB, pDS.GetClassColliderListener() );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS.GetClassColliderListener() );
		pHasCB = false;
	}
}



dsRealObject *dedsCollider::GetCallbackBreaking() const{
	return pValCBBreaking->GetRealObject();
}

void dedsCollider::SetCallbackBreaking( dsRealObject *object ){
	if( ! pValCBBreaking ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCBBreaking, object );
		rt.CastValueTo( pValCBBreaking, pValCBBreaking, pDS.GetClassColliderBreakingListener() );
		pHasCBBreaking = true;
		
	}else{
		rt.SetNull( pValCBBreaking, pDS.GetClassColliderBreakingListener() );
		pHasCBBreaking = false;
	}
}



// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	int timerCollisionResponse = 0;
	int timerCollisionResponseCount = 0;
	int timerCanHitCollider = 0;
	int timerCanHitColliderCount = 0;
	int timerColliderChanged = 0;
	int timerColliderChangedCount = 0;
#endif

void dedsCollider::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! pHasCB ){
		return;
	}
	
	if( ! owner || ! info ){
		DSTHROW( dueInvalidParam );
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCollisionResponse();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollisionInfo &clsCI = *pDS.GetClassCollisionInfo();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		clsCI.PushInfo( rt, info ); // info
		clsCol.PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerCollisionResponse += ( int )( timer.GetElapsedTime() * 1e6f );
		timerCollisionResponseCount++;
		#endif
}

bool dedsCollider::CanHitCollider( deCollider *owner, deCollider *collider ){
	if( ! pEnableCanHitCallback || ! pHasCB ){
		return true;
	}
	
	if( ! owner || ! collider ){
		DSTHROW( dueInvalidParam );
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
		
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexCanHitCollider();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider *clsCol = pDS.GetClassCollider();
	bool retVal = true;
	
	try{
		clsCol->PushCollider( rt, collider ); // collider
		clsCol->PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pValCB, funcIndex );
		retVal = rt->GetReturnBool();
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerCanHitCollider += ( int )( timer.GetElapsedTime() * 1e6f );
		timerCanHitColliderCount++;
		#endif
	
	return retVal;
}

void dedsCollider::ColliderChanged( deCollider *owner ){
	if( ! pHasCB ){
		return;
	}
	
	if( ! owner ){
		DSTHROW( dueInvalidParam );
	}
	
		#ifdef DO_TIMING
		decTimer timer;
		timer.Reset();
		#endif
	
	const int funcIndex = pDS.GetClassColliderListener()->GetFuncIndexColliderChanged();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider *clsCol = pDS.GetClassCollider();
	
	try{
		clsCol->PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
	
		#ifdef DO_TIMING
		timerColliderChanged += ( int )( timer.GetElapsedTime() * 1e6f );
		timerColliderChangedCount++;
		#endif
}



void dedsCollider::ColliderConstraintBroke( deCollider *owner,
int index, deColliderConstraint *constraint ){
	if( ! pHasCBBreaking ){
		return;
	}
	
	if( ! owner ){
		DSTHROW( dueInvalidParam );
	}
	
	const int funcIndex = pDS.GetClassColliderBreakingListener()->GetFuncIndexColliderConstraintBroke();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		rt->PushInt( index ); // index
		clsCol.PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pValCBBreaking, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsCollider::RigConstraintBroke( deCollider *owner,
int bone, int index, deRigConstraint *constraint ) {
	if( ! pHasCBBreaking ){
		return;
	}
	
	if( ! owner ){
		DSTHROW( dueInvalidParam );
	}
	
	const int funcIndex = pDS.GetClassColliderBreakingListener()->GetFuncIndexRigConstraintBroke();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	try{
		rt->PushInt( index ); // index
		rt->PushInt( bone ); // bone
		clsCol.PushCollider( rt, owner ); // owner
		rt->RunFunctionFast( pValCBBreaking, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}
