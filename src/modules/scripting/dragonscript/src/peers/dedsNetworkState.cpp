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

#include <libdscript/libdscript.h>
#include <stdio.h>
#include <stdlib.h>

#include "dedsNetworkState.h"
#include "../deScriptingDragonScript.h"
#include "../classes/network/deClassNetworkState.h"
#include "../classes/network/deClassNetworkStateListener.h"

#include <dragengine/resources/network/deNetworkState.h>
#include <libdscript/exceptions.h>



// Class dedsNetworkState
///////////////////////////

// Constructor, destructor
////////////////////////////

dedsNetworkState::dedsNetworkState( deScriptingDragonScript &ds, deNetworkState *state ) :
pDS( ds ), 
pNetworkState( state ),
pValCB( NULL ),
pHasCB( false )
{
	if( ! state ){
		DSTHROW( dueInvalidParam );
	}
	
	pValCB = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassNetworkStateListener() );
}

dedsNetworkState::~dedsNetworkState(){
	if( pValCB ){
		if( pValCB->GetRealObject() ){
			// check if the resource is in progress of being deleted. if this is not
			// the case we can end up re-entering this destructor due to the resource
			// being deleted due to links breaking while freeing the value. if this
			// is the case delay the deletion until a safe time
			if( pNetworkState && pNetworkState->GetRefCount() > 0 ){
				pDS.AddValueDeleteLater( pValCB );
				
			}else{
				pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
			}
		}
		
		pValCB = NULL;
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsNetworkState::StateValueChanged( int index ){
	if( ! pHasCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	try{
		rt.PushInt( index ); // index
		rt.RunFunction( pValCB, "stateValueChanged", 1 );
		
	}catch( const duException &e ){
		rt.PrintExceptionTrace();
		e.PrintError();
	}
}



// Management
///////////////

dsRealObject *dedsNetworkState::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsNetworkState::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS.GetClassNetworkStateListener() );
		pHasCB = false;
	}
}
