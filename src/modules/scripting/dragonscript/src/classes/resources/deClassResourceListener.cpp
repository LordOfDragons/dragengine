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

#include "deClassResourceListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>

#include <libdscript/packages/default/dsClassEnumeration.h>



// Native functions
/////////////////////

// public func void finishedLoading( String filename, ResourceLoaderType resourceType, Object resource )
deClassResourceListener::nfFinishedLoading::nfFinishedLoading( const sInitData &init ) :
dsFunction( init.clsRN, "finishedLoading", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
	p_AddParameter( init.clsResourceLoaderType ); // resourceType
	p_AddParameter( init.clsObj ); // resource
}
void deClassResourceListener::nfFinishedLoading::RunFunction( dsRunTime *rt, dsValue *myself ){
}

// public func void failedLoading( String filename, ResourceLoaderType resourceType )
deClassResourceListener::nfFailedLoading::nfFailedLoading( const sInitData &init ) :
dsFunction( init.clsRN, "failedLoading", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsStr ); // filename
	p_AddParameter( init.clsResourceLoaderType ); // resourceType
}
void deClassResourceListener::nfFailedLoading::RunFunction( dsRunTime *rt, dsValue *myself ){
}



// Class deClassResourceListener
//////////////////////////////////

// Constructor, Destructors
/////////////////////////////

deClassResourceListener::deClassResourceListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass( "ResourceListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// set parser info
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	
	// do the rest
	p_SetNativeDataSize( 0 );
}

deClassResourceListener::~deClassResourceListener(){
}



// Management
///////////////

void deClassResourceListener::CreateClassMembers( dsEngine *engine ){
	pClsResourceLoaderType = engine->GetClass( "Dragengine.ResourceLoaderType" );
	
	sInitData init;
	init.clsRN = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsResourceLoaderType = pClsResourceLoaderType;
	
	// add functions
	AddFunction( new nfFinishedLoading( init ) ); // function 0
	AddFunction( new nfFailedLoading( init ) ); // function 1
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexFinishedLoading = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexFailedLoading = funcList.GetIndexOf( GetFunction( 1 ) );
}

