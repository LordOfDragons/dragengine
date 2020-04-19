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

#include "dedsTouchSensor.h"
#include "../deScriptingDragonScript.h"
#include "../classes/collider/deClassCollider.h"
#include "../classes/physics/deClassTouchSensor.h"
#include "../classes/physics/deClassTouchSensorListener.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/sensor/deTouchSensor.h>



// Class dedsTouchSensor
//////////////////////////

// Constructor, destructor
////////////////////////////

dedsTouchSensor::dedsTouchSensor( deScriptingDragonScript &ds, deTouchSensor *touchSensor ) :
pDS( ds ),
pTouchSensor( touchSensor ),
pValCB( NULL ),
pHasCB( false )
{
	if( ! touchSensor ){
		DSTHROW( dueInvalidParam );
	}
	
	pValCB = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassTouchSensorListener() );
}

dedsTouchSensor::~dedsTouchSensor(){
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pTouchSensor && pTouchSensor->GetRefCount() > 0 ){
		if( pValCB ){
			if( pValCB->GetRealObject() ){
				pDS.AddValueDeleteLater( pValCB );
			}
			pValCB = NULL;
			pHasCB = false;
		}
		
	}else{
		if( pValCB ){
			if( pValCB->GetRealObject() ){
				pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
			}
			pValCB = NULL;
			pHasCB = false;
		}
	}
}



// Management
///////////////

dsRealObject *dedsTouchSensor::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsTouchSensor::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		rt.CastValueTo( pValCB, pValCB, pDS.GetClassTouchSensorListener() );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS.GetClassTouchSensorListener() );
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsTouchSensor::ColliderEntered( deCollider *collider ){
	if( ! collider ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! pHasCB ){
		return;
	}
	
	const int funcIndex = pDS.GetClassTouchSensorListener()->GetFuncIndexColliderEntered();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	// colliderEntered( collider )
	try{
		clsCol.PushCollider( rt, collider ); // collider
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsTouchSensor::ColliderLeft( deCollider *collider ){
	if( ! collider ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! pHasCB ){
		return;
	}
	
	const int funcIndex = pDS.GetClassTouchSensorListener()->GetFuncIndexColliderLeft();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	deClassCollider &clsCol = *pDS.GetClassCollider();
	
	// colliderLeft( collider )
	try{
		clsCol.PushCollider( rt, collider ); // collider
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}
