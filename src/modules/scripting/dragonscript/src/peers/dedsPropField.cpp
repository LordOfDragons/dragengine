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
