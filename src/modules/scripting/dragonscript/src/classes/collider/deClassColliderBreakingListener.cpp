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
