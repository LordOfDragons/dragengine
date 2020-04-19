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

#include "deClassCollider.h"
#include "deClassColliderBreakingListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <libdscript/exceptions.h>



// Native Functions
/////////////////////

// func void colliderConstraintBroke( Collider owner, int constraint )
deClassColliderBreakingListener::nfColliderConstraintBroke::nfColliderConstraintBroke( const sInitData &init ) :
dsFunction( init.clsCBL, "colliderConstraintBroke", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // owner
	p_AddParameter( init.clsInt ); // constraint
}
void deClassColliderBreakingListener::nfColliderConstraintBroke::RunFunction( dsRunTime *RT, dsValue *This ){
}

// func void rigConstraintBroke( Collider owner, int bone, int constraint )
deClassColliderBreakingListener::nfRigConstraintBroke::nfRigConstraintBroke( const sInitData &init ) :
dsFunction( init.clsCBL, "rigConstraintBroke", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCol ); // owner
	p_AddParameter( init.clsInt ); // bone
	p_AddParameter( init.clsInt ); // constraint
}
void deClassColliderBreakingListener::nfRigConstraintBroke::RunFunction( dsRunTime *RT, dsValue *This ){
}



// Class deClassColliderBreakingListener
//////////////////////////////////////////

// Constructor
////////////////

deClassColliderBreakingListener::deClassColliderBreakingListener( deScriptingDragonScript *ds ) :
dsClass( "ColliderBreakingListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_SCENERY );
	
	p_SetNativeDataSize( 0 );
}

deClassColliderBreakingListener::~deClassColliderBreakingListener(){
}



// Management
///////////////

void deClassColliderBreakingListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCBL = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsFlt = engine->GetClassFloat();
	init.clsInt = engine->GetClassInt();
	init.clsCol = pDS->GetClassCollider();
	
	// add functions
	AddFunction( new nfColliderConstraintBroke( init ) ); // function 0
	AddFunction( new nfRigConstraintBroke( init ) ); // function 1
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexColliderConstraintBroke = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexRigConstraintBroke = funcList.GetIndexOf( GetFunction( 1 ) );
}
