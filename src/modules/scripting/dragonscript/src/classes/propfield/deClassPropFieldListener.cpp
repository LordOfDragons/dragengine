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
#include <string.h>

#include "deClassPropField.h"
#include "deClassPropFieldListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>
#include <libdscript/exceptions.h>



// Native Functions
/////////////////////



// public func void createInstances( PropField propfield, float density )
deClassPropFieldListener::nfCreateInstances::nfCreateInstances( const sInitData &init ) :
dsFunction( init.clsPFL, "createInstances", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsPF ); // propfield
	p_AddParameter( init.clsFlt ); // density
}
void deClassPropFieldListener::nfCreateInstances::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Class deClassPropFieldListener
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassPropFieldListener::deClassPropFieldListener( deScriptingDragonScript *ds ) :
dsClass( "PropFieldListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	
	p_SetNativeDataSize( 0 );
}

deClassPropFieldListener::~deClassPropFieldListener(){
}



// Management
///////////////

void deClassPropFieldListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsPFL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsFlt = engine->GetClassFloat();
	init.clsPF = pDS->GetClassPropField();
	
	// add functions
	AddFunction( new nfCreateInstances( init ) ); // function 0
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function index for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexCreateInstances = funcList.GetIndexOf( GetFunction( 0 ) );
}
