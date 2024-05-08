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
		
	case deModuleSystem::emtService:
		pTypeString = "Service";
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
