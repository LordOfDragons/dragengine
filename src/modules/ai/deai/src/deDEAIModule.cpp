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


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEAICreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deaiModuleInternal : public deInternalModule{
public:
deaiModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DEAI");
		SetDescription("Provides AI support to the engine.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAI);
		SetDirectoryName("deai");
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(DEAICreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deaiRegisterInternalModule(deModuleSystem *system){
	return new deaiModuleInternal(system);
}
#endif
