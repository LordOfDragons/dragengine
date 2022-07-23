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

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>



// Class deglPEListItemModule
///////////////////////////////

FXIMPLEMENT( deglPEListItemModule, FXIconItem, nullptr, 0 )

// Constructor, destructor
////////////////////////////

deglPEListItemModule::deglPEListItemModule(){ }

deglPEListItemModule::deglPEListItemModule( deglPanelEngine *panelEngine, delEngineModule *module ) :
FXIconItem( "", nullptr, nullptr, nullptr ),
pPanelEngine( panelEngine ),
pModule( module )
{
	if( ! panelEngine || ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const delEngineModule::eModuleStatus status = module->GetStatus();
	const deModuleSystem::eModuleTypes type = module->GetType();
	FXString text;
	
	const decString &moduleName = module->GetName();
	const decString &moduleVersion = module->GetVersion();
	
	switch( status ){
	case delEngineModule::emsReady:
		pStatusString = "Ready";
		break;
		
	case delEngineModule::emsNotTested:
		pStatusString = "Not Tested";
		break;
		
	case delEngineModule::emsBroken:
		pStatusString = "Broken";
		break;
	}
	
	switch( type ){
	case deModuleSystem::emtGraphic:
		pTypeString = "Graphic";
		break;
		
	case deModuleSystem::emtAudio:
		pTypeString = "Audio";
		break;
		
	case deModuleSystem::emtInput:
		pTypeString = "Input";
		break;
		
	case deModuleSystem::emtNetwork:
		pTypeString = "Network";
		break;
		
	case deModuleSystem::emtPhysics:
		pTypeString = "Physics";
		break;
		
	case deModuleSystem::emtImage:
		pTypeString = "Image";
		break;
		
	case deModuleSystem::emtVideo:
		pTypeString = "Video";
		break;
		
	case deModuleSystem::emtScript:
		pTypeString = "Script";
		break;
		
	case deModuleSystem::emtModel:
		pTypeString = "Model";
		break;
		
	case deModuleSystem::emtRig:
		pTypeString = "Rig";
		break;
		
	case deModuleSystem::emtSkin:
		pTypeString = "Skin";
		break;
		
	case deModuleSystem::emtAnimation:
		pTypeString = "Animation";
		break;
		
	case deModuleSystem::emtFont:
		pTypeString = "Font";
		break;
		
	case deModuleSystem::emtCrashRecovery:
		pTypeString = "Crash Recovery";
		break;
		
	case deModuleSystem::emtLanguagePack:
		pTypeString = "Language Pack";
		break;
		
	case deModuleSystem::emtAnimator:
		pTypeString = "Animator";
		break;
		
	case deModuleSystem::emtSound:
		pTypeString = "Sound";
		break;
		
	case deModuleSystem::emtAI:
		pTypeString = "AI";
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		pTypeString = "Occlusion Mesh";
		break;
		
	case deModuleSystem::emtSynthesizer:
		pTypeString = "Synthesizer";
		break;
		
	case deModuleSystem::emtArchive:
		pTypeString = "Archive";
		break;
		
	case deModuleSystem::emtVR:
		pTypeString = "VR";
		break;
		
	case deModuleSystem::emtUnknown:
		pTypeString = "Unknown";
	}
	
	text.format( "%s\t%s\t%s\t%s", moduleName.GetString(), pTypeString.GetString(),
		moduleVersion.GetString(), pStatusString.GetString() );
	setText( text );
}

deglPEListItemModule::~deglPEListItemModule(){
}
