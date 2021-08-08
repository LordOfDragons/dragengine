/* 
 * Drag[en]gine Console Launcher
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

#include "declGameProfile.h"
#include "declGPModule.h"
#include "declGPMParameter.h"
#include "declGPDisableModuleVersion.h"
#include "../../declLauncher.h"
#include "../../engine/declEngine.h"
#include "../../engine/modules/declEngineModule.h"
#include "../../engine/modules/parameter/declEMParameter.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class declGameProfile
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

declGameProfile::declGameProfile( const char *name ) :
pName( name ),
pReplaceRunArgs( false ),
pFullScreen( true ),
pWidth( 1680 ),
pHeight( 1050 ),
pValid( false ){
}

declGameProfile::declGameProfile( const declGameProfile &profile ){
	CopyFrom( profile );
	pName = profile.pName;
}

declGameProfile::~declGameProfile(){
}



// Management
///////////////

void declGameProfile::SetName( const char *name ){
	pName = name;
}

void declGameProfile::SetModuleGraphic( const char *moduleName ){
	pModuleGraphic = moduleName;
}

void declGameProfile::SetModuleInput( const char *moduleName ){
	pModuleInput = moduleName;
}

void declGameProfile::SetModuleVR( const char *moduleName ){
	pModuleVR = moduleName;
}

void declGameProfile::SetModulePhysics( const char *moduleName ){
	pModulePhysics = moduleName;
}

void declGameProfile::SetModuleAnimator( const char *moduleName ){
	pModuleAnimator = moduleName;
}

void declGameProfile::SetModuleAI( const char *moduleName ){
	pModuleAI = moduleName;
}

void declGameProfile::SetModuleCrashRecovery( const char *moduleName ){
	pModuleCrashRecovery = moduleName;
}

void declGameProfile::SetModuleAudio( const char *moduleName ){
	pModuleAudio = moduleName;
}

void declGameProfile::SetModuleSynthesizer( const char *moduleName ){
	pModuleSynthesizer = moduleName;
}

void declGameProfile::SetModuleNetwork( const char *moduleName ){
	pModuleNetwork = moduleName;
}



void declGameProfile::SetModuleGraphicVersion( const char *moduleVersion ){
	pModuleGraphicVersion = moduleVersion;
}

void declGameProfile::SetModuleInputVersion( const char *moduleVersion ){
	pModuleInputVersion = moduleVersion;
}

void declGameProfile::SetModuleVRVersion( const char *moduleVersion ){
	pModuleVRVersion = moduleVersion;
}

void declGameProfile::SetModulePhysicsVersion( const char *moduleVersion ){
	pModulePhysicsVersion = moduleVersion;
}

void declGameProfile::SetModuleAnimatorVersion( const char *moduleVersion ){
	pModuleAnimatorVersion = moduleVersion;
}

void declGameProfile::SetModuleAIVersion( const char *moduleVersion ){
	pModuleAIVersion = moduleVersion;
}

void declGameProfile::SetModuleCrashRecoveryVersion( const char *moduleVersion ){
	pModuleCrashRecoveryVersion = moduleVersion;
}

void declGameProfile::SetModuleAudioVersion( const char *moduleVersion ){
	pModuleAudioVersion = moduleVersion;
}

void declGameProfile::SetModuleSynthesizerVersion( const char *moduleVersion ){
	pModuleSynthesizerVersion = moduleVersion;
}

void declGameProfile::SetModuleNetworkVersion( const char *moduleVersion ){
	pModuleNetworkVersion = moduleVersion;
}



void declGameProfile::SetRunArguments( const char *arguments ){
	pRunArgs = arguments;
}

void declGameProfile::SetReplaceRunArguments( bool replaceRunArguments ){
	pReplaceRunArgs = replaceRunArguments;
}

void declGameProfile::SetWidth( int width ){
	pWidth = decMath::max( width, 1 );
}

void declGameProfile::SetHeight( int height ){
	pHeight = decMath::max( height, 1 );
}

void declGameProfile::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}



void declGameProfile::CopyFrom( const declGameProfile &profile ){
	pModuleGraphic = profile.pModuleGraphic;
	pModuleInput = profile.pModuleInput;
	pModuleVR = profile.pModuleVR;
	pModulePhysics = profile.pModulePhysics;
	pModuleAnimator = profile.pModuleAnimator;
	pModuleAI = profile.pModuleAI;
	pModuleCrashRecovery = profile.pModuleCrashRecovery;
	pModuleAudio = profile.pModuleAudio;
	pModuleSynthesizer = profile.pModuleSynthesizer;
	pModuleNetwork = profile.pModuleNetwork;
	
	pModuleGraphicVersion = profile.pModuleGraphicVersion;
	pModuleInputVersion = profile.pModuleInputVersion;
	pModuleVRVersion = profile.pModuleVRVersion;
	pModulePhysicsVersion = profile.pModulePhysicsVersion;
	pModuleAnimatorVersion = profile.pModuleAnimatorVersion;
	pModuleAIVersion = profile.pModuleAIVersion;
	pModuleCrashRecoveryVersion = profile.pModuleCrashRecoveryVersion;
	pModuleAudioVersion = profile.pModuleAudioVersion;
	pModuleSynthesizerVersion = profile.pModuleSynthesizerVersion;
	pModuleNetworkVersion = profile.pModuleNetworkVersion;
	
	pDisableModuleVersionList = profile.pDisableModuleVersionList;
	
	pModuleList.RemoveAll();
	
	const declGPModuleList &moduleList = profile.GetModuleList();
	const int moduleCount = moduleList.GetCount();
	deObjectReference module;
	int i;
	for( i=0; i<moduleCount; i++ ){
		module.TakeOver( new declGPModule( *moduleList.GetAt( i ) ) );
		pModuleList.Add( ( declGPModule* )( deObject* )module );
	}
	
	pRunArgs = profile.pRunArgs;
	pReplaceRunArgs = profile.pReplaceRunArgs;
	pFullScreen = profile.pFullScreen;
	pWidth = profile.pWidth;
	pHeight = profile.pHeight;
	
	pValid = profile.pValid;
}

void declGameProfile::Verify( declLauncher &launcher ){
	pValid = true;
	pValid &= VerifyModule( launcher, pModuleGraphic, deModuleSystem::emtGraphic );
	pValid &= VerifyModule( launcher, pModuleInput, deModuleSystem::emtInput );
	pValid &= VerifyModule( launcher, pModulePhysics, deModuleSystem::emtPhysics );
	pValid &= VerifyModule( launcher, pModuleAnimator, deModuleSystem::emtAnimator );
	pValid &= VerifyModule( launcher, pModuleAI, deModuleSystem::emtAI );
	pValid &= VerifyModule( launcher, pModuleCrashRecovery, deModuleSystem::emtCrashRecovery );
	pValid &= VerifyModule( launcher, pModuleAudio, deModuleSystem::emtAudio );
	pValid &= VerifyModule( launcher, pModuleSynthesizer, deModuleSystem::emtSynthesizer );
	pValid &= VerifyModule( launcher, pModuleNetwork, deModuleSystem::emtNetwork );
	pValid &= VerifyModule( launcher, pModuleVR, deModuleSystem::emtVR );
}

bool declGameProfile::VerifyModule( declLauncher &launcher, const char *moduleName, int requiredType ) const{
	const declEngineModuleList &moduleList = launcher.GetEngine()->GetModuleList();
	
	declEngineModule * const module = moduleList.GetNamed( moduleName );
	if( ! module ){
		return false;
	}
	
	if( module->GetType() != requiredType ){
		return false;
	}
	if( module->GetStatus() != declEngineModule::emsReady ){
		return false;
	}
	
	return true;
}

void declGameProfile::Activate( declLauncher &launcher ) const{
	deEngine * const engine = launcher.GetEngine()->GetEngine();
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	// disable module versions. this affects all modules
	const declEngineModuleList &engineModuleList = launcher.GetEngine()->GetModuleList();
	const int disableModuleVersionCount = pDisableModuleVersionList.GetCount();
	int i;
	
	for( i=0; i<disableModuleVersionCount; i++ ){
		const declGPDisableModuleVersion &version = *pDisableModuleVersionList.GetAt( i );
		declEngineModule * const module = engineModuleList.GetNamed( version.GetName(), version.GetVersion() );
		if( module ){
			module->GetLoadableModule()->SetEnabled( false );
		}
	}
	
	// activate modules
	declEngineModule *engineModule = NULL;
	
	engineModule = engineModuleList.GetNamed( pModuleCrashRecovery.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetCrashRecoverySystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleGraphic.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetGraphicSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleInput.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetInputSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModulePhysics.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetPhysicsSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleAnimator.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetAnimatorSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleAI.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetAISystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleAudio.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetAudioSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleSynthesizer );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetSynthesizerSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleNetwork.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetNetworkSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	engineModule = engineModuleList.GetNamed( pModuleVR.GetString() );
	if( ! engineModule || ! engineModule->GetLoadableModule() ){
		DETHROW( deeInvalidParam );
	}
	engine->GetVRSystem()->SetActiveModule( engineModule->GetLoadableModule() );
	
	// set module properties
	pModuleList.Apply( launcher );
}
