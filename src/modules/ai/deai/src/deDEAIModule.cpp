/* 
 * Drag[en]gine AI Module
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
#include <string.h>

#include "deDEAIModule.h"
#include "navigation/dedaiNavSpace.h"
#include "navigation/dedaiNavigator.h"
#include "navigation/blocker/dedaiNavBlocker.h"
#include "navigation/heightterrain/dedaiHeightTerrain.h"
#include "world/dedaiWorld.h"
#include "devmode/dedaiDeveloperMode.h"
#include "dedaiCommandExecuter.h"

#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/deEngine.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEAICreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



deBaseModule *DEAICreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deDEAIModule( *loadableModule );
		
	}catch( const deException & ){
		module = NULL; // just to be safe
	}
	
	return module;
}



// Class deDEAIModule
///////////////////////

// Constructor, destructor
////////////////////////////

deDEAIModule::deDEAIModule( deLoadableModule &loadableModule ) :
deBaseAIModule( loadableModule ){
	pDeveloperMode = NULL;
	pCommandExecuter = NULL;
	
	// create objects existing at all times
	pCommandExecuter = new dedaiCommandExecuter( this );
	pDeveloperMode = new dedaiDeveloperMode( *this );
}

deDEAIModule::~deDEAIModule(){
	if( pDeveloperMode ){
		delete pDeveloperMode;
	}
	if( pCommandExecuter ){
		delete pCommandExecuter;
	}
}



// Module Management
//////////////////////

bool deDEAIModule::Init(){
	return true;
}

void deDEAIModule::CleanUp(){
}



// Management
///////////////

deBaseAIWorld *deDEAIModule::CreateWorld( deWorld *world ){
	return new dedaiWorld( *this, *world );
}

deBaseAINavigationSpace *deDEAIModule::CreateNavigationSpace( deNavigationSpace *navspace ){
	return new dedaiNavSpace( *this, *navspace );
}

deBaseAINavigationBlocker *deDEAIModule::CreateNavigationBlocker( deNavigationBlocker *blocker ){
	return new dedaiNavBlocker( *this, *blocker );
}

deBaseAINavigator *deDEAIModule::CreateNavigator( deNavigator *navigator ){
	return new dedaiNavigator( *this, *navigator );
}

deBaseAIHeightTerrain *deDEAIModule::CreateHeightTerrain( deHeightTerrain &heightTerrain ){
	return new dedaiHeightTerrain( *this, heightTerrain );
}



// Debugging
//////////////

void deDEAIModule::SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pCommandExecuter ){
		pCommandExecuter->ExecuteCommand( command, answer );
		
	}else{
		answer.SetFromUTF8( "Internal Error!" );
	}
}
