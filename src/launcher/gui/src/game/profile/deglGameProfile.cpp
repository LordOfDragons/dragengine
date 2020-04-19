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

#include "deglGameProfile.h"
#include "deglGPModule.h"
#include "deglGPDisableModuleVersion.h"
#include "../../deglLauncher.h"
#include "../../engine/deglEngine.h"
#include "../../engine/deglEngineInstance.h"
#include "../../engine/modules/deglEngineModule.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class deglGameProfile
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameProfile::deglGameProfile( const char *name ) :
pName( name ),
pReplaceRunArgs( false ),
pFullScreen( true ),
pWidth( 1680 ),
pHeight( 1050 ),
pValid( false ){
}

deglGameProfile::deglGameProfile( const deglGameProfile &profile ){
	CopyFrom( profile );
	pName = profile.pName;
}

deglGameProfile::~deglGameProfile(){
}



// Management
///////////////

void deglGameProfile::SetName( const char *name ){
	pName = name;
}



void deglGameProfile::SetModuleGraphic( const char *moduleName ){
	pModuleGraphic = moduleName;
}

void deglGameProfile::SetModuleInput( const char *moduleName ){
	pModuleInput = moduleName;
}

void deglGameProfile::SetModulePhysics( const char *moduleName ){
	pModulePhysics = moduleName;
}

void deglGameProfile::SetModuleAnimator( const char *moduleName ){
	pModuleAnimator = moduleName;
}

void deglGameProfile::SetModuleAI( const char *moduleName ){
	pModuleAI = moduleName;
}

void deglGameProfile::SetModuleCrashRecovery( const char *moduleName ){
	pModuleCrashRecovery = moduleName;
}

void deglGameProfile::SetModuleAudio( const char *moduleName ){
	pModuleAudio = moduleName;
}

void deglGameProfile::SetModuleSynthesizer( const char *moduleName ){
	pModuleSynthesizer = moduleName;
}

void deglGameProfile::SetModuleNetwork( const char *moduleName ){
	pModuleNetwork = moduleName;
}



void deglGameProfile::SetModuleGraphicVersion( const char *moduleVersion ){
	pModuleGraphicVersion = moduleVersion;
}

void deglGameProfile::SetModuleInputVersion( const char *moduleVersion ){
	pModuleInputVersion = moduleVersion;
}

void deglGameProfile::SetModulePhysicsVersion( const char *moduleVersion ){
	pModulePhysicsVersion = moduleVersion;
}

void deglGameProfile::SetModuleAnimatorVersion( const char *moduleVersion ){
	pModuleAnimatorVersion = moduleVersion;
}

void deglGameProfile::SetModuleAIVersion( const char *moduleVersion ){
	pModuleAIVersion = moduleVersion;
}

void deglGameProfile::SetModuleCrashRecoveryVersion( const char *moduleVersion ){
	pModuleCrashRecoveryVersion = moduleVersion;
}

void deglGameProfile::SetModuleAudioVersion( const char *moduleVersion ){
	pModuleAudioVersion = moduleVersion;
}

void deglGameProfile::SetModuleSynthesizerVersion( const char *moduleVersion ){
	pModuleSynthesizerVersion = moduleVersion;
}

void deglGameProfile::SetModuleNetworkVersion( const char *moduleVersion ){
	pModuleNetworkVersion = moduleVersion;
}



void deglGameProfile::SetRunArguments( const char *arguments ){
	pRunArgs = arguments;
}

void deglGameProfile::SetReplaceRunArguments( bool replaceRunArguments ){
	pReplaceRunArgs = replaceRunArguments;
}

void deglGameProfile::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}

void deglGameProfile::SetWidth( int width ){
	pWidth = decMath::max( width, 100 );
}

void deglGameProfile::SetHeight( int height ){
	pHeight = decMath::max( height, 75 );
}



void deglGameProfile::CopyFrom( const deglGameProfile &profile ){
	pModuleGraphic = profile.pModuleGraphic;
	pModuleInput = profile.pModuleInput;
	pModulePhysics = profile.pModulePhysics;
	pModuleAnimator = profile.pModuleAnimator;
	pModuleAI = profile.pModuleAI;
	pModuleCrashRecovery = profile.pModuleCrashRecovery;
	pModuleAudio = profile.pModuleAudio;
	pModuleSynthesizer = profile.pModuleSynthesizer;
	pModuleNetwork = profile.pModuleNetwork;
	
	pModuleGraphicVersion = profile.pModuleGraphicVersion;
	pModuleInputVersion = profile.pModuleInputVersion;
	pModulePhysicsVersion = profile.pModulePhysicsVersion;
	pModuleAnimatorVersion = profile.pModuleAnimatorVersion;
	pModuleAIVersion = profile.pModuleAIVersion;
	pModuleCrashRecoveryVersion = profile.pModuleCrashRecoveryVersion;
	pModuleAudioVersion = profile.pModuleAudioVersion;
	pModuleSynthesizerVersion = profile.pModuleSynthesizerVersion;
	pModuleNetworkVersion = profile.pModuleNetworkVersion;
	
	pDisableModuleVersionList = profile.pDisableModuleVersionList;
	
	pModuleList.RemoveAllModules();
	
	const deglGPModuleList &moduleList = profile.GetModuleList();
	const int moduleCount = moduleList.GetModuleCount();
	deObjectReference module;
	int i;
	for( i=0; i<moduleCount; i++ ){
		module.TakeOver( new deglGPModule( *moduleList.GetModuleAt( i ) ) );
		pModuleList.AddModule( ( deglGPModule* )( deObject* )module );
	}
	
	pRunArgs = profile.pRunArgs;
	pReplaceRunArgs = profile.pReplaceRunArgs;
	pFullScreen = profile.pFullScreen;
	pWidth = profile.pWidth;
	pHeight = profile.pHeight;
	
	pValid = profile.pValid;
}

void deglGameProfile::Verify( deglLauncher &launcher ){
	pValid = true;
	pValid &= VerifyModule( launcher, pModuleGraphic, pModuleGraphicVersion, deModuleSystem::emtGraphic );
	pValid &= VerifyModule( launcher, pModuleInput, pModuleInputVersion, deModuleSystem::emtInput );
	pValid &= VerifyModule( launcher, pModulePhysics, pModulePhysicsVersion, deModuleSystem::emtPhysics );
	pValid &= VerifyModule( launcher, pModuleAnimator, pModuleAnimatorVersion, deModuleSystem::emtAnimator );
	pValid &= VerifyModule( launcher, pModuleAI, pModuleAIVersion, deModuleSystem::emtAI );
	pValid &= VerifyModule( launcher, pModuleCrashRecovery, pModuleCrashRecoveryVersion, deModuleSystem::emtCrashRecovery );
	pValid &= VerifyModule( launcher, pModuleAudio, pModuleAudioVersion, deModuleSystem::emtAudio );
	pValid &= VerifyModule( launcher, pModuleSynthesizer, pModuleSynthesizerVersion, deModuleSystem::emtSynthesizer );
	pValid &= VerifyModule( launcher, pModuleNetwork, pModuleNetworkVersion, deModuleSystem::emtNetwork );
}

bool deglGameProfile::VerifyModule( deglLauncher &launcher, const char *moduleName, const char *moduleVersion, int requiredType ) const{
	const deglEngineModuleList &moduleList = launcher.GetEngine()->GetModuleList();
	deglEngineModule *module;
	
	if( strlen( moduleVersion ) == 0 ){
		module = moduleList.GetModuleNamed( moduleName );
		
	}else{
		module = moduleList.GetModuleNamed( moduleName, moduleVersion );
	}
	
	if( ! module
	|| module->GetType() != requiredType
	|| module->GetStatus() != deglEngineModule::emsReady ){
		return false;
	}
	
	return true;
}

void deglGameProfile::Activate( deglLauncher &launcher, deglEngineInstance &engineInstance ) const{
	// disable module versions. this affects all modules
	const deglEngineModuleList &engineModuleList = launcher.GetEngine()->GetModuleList();
	const int moduleCount = pModuleList.GetModuleCount();
	const int disableModuleVersionCount = pDisableModuleVersionList.GetCount();
	int i;
	
	for( i=0; i<disableModuleVersionCount; i++ ){
		const deglGPDisableModuleVersion &version = *pDisableModuleVersionList.GetAt( i );
		deglEngineModule * const module = engineModuleList.GetModuleNamed( version.GetName(), version.GetVersion() );
		if( module ){
			engineInstance.EnableModule( module->GetName(), module->GetVersion(), false );
		}
	}
	
	// activate modules
	engineInstance.ActivateModule( pModuleCrashRecovery, pModuleCrashRecoveryVersion );
	engineInstance.ActivateModule( pModuleGraphic, pModuleGraphicVersion );
	engineInstance.ActivateModule( pModuleInput, pModuleInputVersion );
	engineInstance.ActivateModule( pModulePhysics, pModulePhysicsVersion );
	engineInstance.ActivateModule( pModuleAnimator, pModuleAnimatorVersion );
	engineInstance.ActivateModule( pModuleAI, pModuleAIVersion );
	engineInstance.ActivateModule( pModuleAudio, pModuleAudioVersion );
	engineInstance.ActivateModule( pModuleSynthesizer, pModuleSynthesizerVersion );
	engineInstance.ActivateModule( pModuleNetwork, pModuleNetworkVersion );
	
	// set module properties
	for( i=0; i<moduleCount; i++ ){
		deglGPModule &module = *pModuleList.GetModuleAt( i );
		deglEngineModule * const engineModule = engineModuleList.GetModuleNamed( module.GetName() );
		if( engineModule ){
			module.ApplyParameters( engineModule->GetVersion(), launcher, engineInstance );
		}
	}
}
