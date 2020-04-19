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
