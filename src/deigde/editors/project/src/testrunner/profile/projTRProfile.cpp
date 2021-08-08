/* 
 * Drag[en]gine IGDE Project Editor
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

#include "projTRProfile.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class projTRProfile
////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTRProfile::projTRProfile() :
pReplaceRunArgs( false ),
pWidth( 800 ),
pHeight( 600 ),
pFullScreen( false ),

pValid( false ){
}

projTRProfile::~projTRProfile(){
}



// Management
///////////////

void projTRProfile::SetName( const char *name ){
	pName = name;
}

void projTRProfile::SetModuleGraphic( const char *moduleName ){
	pModuleGraphic = moduleName;
}

void projTRProfile::SetModuleInput( const char *moduleName ){
	pModuleInput = moduleName;
}

void projTRProfile::SetModulePhysics( const char *moduleName ){
	pModulePhysics = moduleName;
}

void projTRProfile::SetModuleAnimator( const char *moduleName ){
	pModuleAnimator = moduleName;
}

void projTRProfile::SetModuleAI( const char *moduleName ){
	pModuleAI = moduleName;
}

void projTRProfile::SetModuleCrashRecovery( const char *moduleName ){
	pModuleCrashRecovery = moduleName;
}

void projTRProfile::SetModuleAudio( const char *moduleName ){
	pModuleAudio = moduleName;
}

void projTRProfile::SetModuleSynthesizer( const char *moduleName ){
	pModuleSynthesizer = moduleName;
}

void projTRProfile::SetModuleNetwork( const char *moduleName ){
	pModuleNetwork = moduleName;
}

void projTRProfile::SetModuleVR( const char *moduleName ){
	pModuleVR = moduleName;
}



void projTRProfile::SetRunArguments( const char *arguments ){
	pRunArgs = arguments;
}

void projTRProfile::SetReplaceRunArguments( bool replaceRunArguments ){
	pReplaceRunArgs = replaceRunArguments;
}

void projTRProfile::SetWidth( int width ){
	if( width < 1 ){
		DETHROW( deeInvalidParam );
	}
	pWidth = width;
}

void projTRProfile::SetHeight( int height ){
	if( height < 1 ){
		DETHROW( deeInvalidParam );
	}
	pHeight = height;
}

void projTRProfile::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}



void projTRProfile::Verify( deEngine &engine ){
	pValid = true;
	pValid &= VerifyModule( engine, pModuleGraphic, deModuleSystem::emtGraphic );
	pValid &= VerifyModule( engine, pModuleInput, deModuleSystem::emtInput );
	pValid &= VerifyModule( engine, pModulePhysics, deModuleSystem::emtPhysics );
	pValid &= VerifyModule( engine, pModuleAnimator, deModuleSystem::emtAnimator );
	pValid &= VerifyModule( engine, pModuleAI, deModuleSystem::emtAI );
	pValid &= VerifyModule( engine, pModuleCrashRecovery, deModuleSystem::emtCrashRecovery );
	pValid &= VerifyModule( engine, pModuleAudio, deModuleSystem::emtAudio );
	pValid &= VerifyModule( engine, pModuleSynthesizer, deModuleSystem::emtSynthesizer );
	pValid &= VerifyModule( engine, pModuleNetwork, deModuleSystem::emtNetwork );
	pValid &= VerifyModule( engine, pModuleVR, deModuleSystem::emtVR );
}

bool projTRProfile::VerifyModule( deEngine &engine, const char *moduleName,
deModuleSystem::eModuleTypes requiredType ) const{
	deLoadableModule * const module = engine.GetModuleSystem()->GetModuleNamed( moduleName );
	return module
		&& module->GetType() == requiredType
		&& module->IsLoaded()
		&& module->GetEnabled();
}
