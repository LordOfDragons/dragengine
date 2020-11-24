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

#include <libdscript/libdscript.h>

#include "dedsPropField.h"
#include "../deScriptingDragonScript.h"
#include "../classes/propfield/deClassPropField.h"
#include "../classes/propfield/deClassPropFieldListener.h"

#include <dragengine/resources/propfield/dePropField.h>
#include <libdscript/exceptions.h>



// Class dedsPropField
////////////////////////

// Constructor, destructor
////////////////////////////

dedsPropField::dedsPropField( deScriptingDragonScript *ds, dePropField *propField ){
	if( ! ds || ! propField ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	pPropField = propField;
	pHasCB = false;
	pValCB = ds->GetScriptEngine()->GetMainRunTime()->CreateValue( ds->GetClassPropFieldListener() );
	pDelegee = NULL;
}

dedsPropField::~dedsPropField(){
	if( ! pValCB ){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pPropField && pPropField->GetRefCount() > 0 ){
		pDS->AddValueDeleteLater( pValCB );
		
	}else{
		pDS->GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsPropField::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsPropField::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS->GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		rt.CastValueTo( pValCB, pValCB, pDS->GetClassPropFieldListener() );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS->GetClassPropFieldListener() );
		pHasCB = false;
	}
}

void dedsPropField::SetDelegee( deBaseScriptingPropField *delegee ){
	pDelegee = delegee;
}



// Notifications
//////////////////

void dedsPropField::CreateInstances( float density ){
	if( pHasCB ){
		const int funcIndex = pDS->GetClassPropFieldListener()->GetFuncIndexCreateInstances();
		dsRunTime * const rt = pDS->GetScriptEngine()->GetMainRunTime();
		deClassPropField &clsPF = *pDS->GetClassPropField();
		
		try{
			rt->PushFloat( density ); // density
			clsPF.PushPropField( rt, pPropField ); // propfield
			rt->RunFunctionFast( pValCB, funcIndex );
			
		}catch( const duException &e ){
			rt->PrintExceptionTrace();
			e.PrintError();
		}
		
	}else if( pDelegee ){
		pDelegee->CreateInstances( density );
	}
}
