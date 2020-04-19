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

