/* 
 * Drag[en]gine GUI Launcher
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

#include "deglPEListItemModule.h"
#include "deglPanelEngine.h"
#include "../../engine/modules/deglEngineModule.h"

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>



// Class deglPEListItemModule
///////////////////////////////

FXIMPLEMENT( deglPEListItemModule, FXIconItem, NULL, 0 )

// Constructor, destructor
////////////////////////////

deglPEListItemModule::deglPEListItemModule(){ }

deglPEListItemModule::deglPEListItemModule( deglPanelEngine *panelEngine, deglEngineModule *module ) :
FXIconItem( "", NULL, NULL, NULL ){
	if( ! panelEngine || ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const int status = module->GetStatus();
	const int type = module->GetType();
	FXString text;
	
	pPanelEngine = panelEngine;
	pModule = module;
	module->AddReference();
	
	const decString &moduleName = module->GetName();
	const decString &moduleVersion = module->GetVersion();
	
	if( status == deglEngineModule::emsReady ){
		pStatusString = "Ready";
		
	}else if( status == deglEngineModule::emsNotTested ){
		pStatusString = "Not Tested";
		
	}else if( status == deglEngineModule::emsBroken ){
		pStatusString = "Broken";
		
	}else{
		pStatusString = "?";
	}
	
	if( type == deModuleSystem::emtGraphic ){
		pTypeString = "Graphic";
		
	}else if( type == deModuleSystem::emtAudio ){
		pTypeString = "Audio";
		
	}else if( type == deModuleSystem::emtInput ){
		pTypeString = "Input";
		
	}else if( type == deModuleSystem::emtNetwork ){
		pTypeString = "Network";
		
	}else if( type == deModuleSystem::emtPhysics ){
		pTypeString = "Physics";
		
	}else if( type == deModuleSystem::emtImage ){
		pTypeString = "Image";
		
	}else if( type == deModuleSystem::emtVideo ){
		pTypeString = "Video";
		
	}else if( type == deModuleSystem::emtScript ){
		pTypeString = "Script";
		
	}else if( type == deModuleSystem::emtModel ){
		pTypeString = "Model";
		
	}else if( type == deModuleSystem::emtRig ){
		pTypeString = "Rig";
		
	}else if( type == deModuleSystem::emtSkin ){
		pTypeString = "Skin";
		
	}else if( type == deModuleSystem::emtAnimation ){
		pTypeString = "Animation";
		
	}else if( type == deModuleSystem::emtFont ){
		pTypeString = "Font";
		
	}else if( type == deModuleSystem::emtCrashRecovery ){
		pTypeString = "Crash Recovery";
		
	}else if( type == deModuleSystem::emtLanguagePack ){
		pTypeString = "Language Pack";
		
	}else if( type == deModuleSystem::emtAnimator ){
		pTypeString = "Animator";
		
	}else if( type == deModuleSystem::emtSound ){
		pTypeString = "Sound";
		
	}else if( type == deModuleSystem::emtAI ){
		pTypeString = "AI";
		
	}else if( type == deModuleSystem::emtOcclusionMesh ){
		pTypeString = "Occlusion Mesh";
		
	}else if( type == deModuleSystem::emtSynthesizer ){
		pTypeString = "Synthesizer";
		
	}else if( type == deModuleSystem::emtArchive ){
		pTypeString = "Archive";
		
	}else{
		pTypeString = "?";
	}
	
	text.format( "%s\t%s\t%s\t%s", moduleName.GetString(), pTypeString.GetString(), moduleVersion.GetString(), pStatusString.GetString() );
	setText( text );
}

deglPEListItemModule::~deglPEListItemModule(){
	if( pModule ){
		pModule->FreeReference();
	}
}



// Management
///////////////

