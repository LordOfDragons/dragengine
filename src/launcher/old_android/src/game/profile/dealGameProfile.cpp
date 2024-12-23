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

#include "dealGameProfile.h"
#include "dealGPModule.h"
#include "dealGPDisableModuleVersion.h"
#include "../../dealLauncher.h"
#include "../../engine/dealEngine.h"
#include "../../engine/modules/dealEngineModule.h"

#include "../../../srcengine/dealIEngineInstance.h"

#include "../../common/exceptions.h"



// Class dealGameProfile
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameProfile::dealGameProfile(){
	pReplaceRunArgs = false;
	pWidth = 800;
	pHeight = 600;
	pFullScreen = false;
	
	pValid = false;
}

dealGameProfile::dealGameProfile( const dealGameProfile &profile ){
	CopyFrom( profile );
}

dealGameProfile::~dealGameProfile(){
}



// Management
///////////////

void dealGameProfile::SetName( const char *name ){
	pName = name;
}



void dealGameProfile::SetModuleGraphic( const char *moduleName ){
	pModuleGraphic = moduleName;
}

void dealGameProfile::SetModuleInput( const char *moduleName ){
	pModuleInput = moduleName;
}

void dealGameProfile::SetModulePhysics( const char *moduleName ){
	pModulePhysics = moduleName;
}

void dealGameProfile::SetModuleAnimator( const char *moduleName ){
	pModuleAnimator = moduleName;
}

void dealGameProfile::SetModuleAI( const char *moduleName ){
	pModuleAI = moduleName;
}

void dealGameProfile::SetModuleCrashRecovery( const char *moduleName ){
	pModuleCrashRecovery = moduleName;
}

void dealGameProfile::SetModuleAudio( const char *moduleName ){
	pModuleAudio = moduleName;
}

void dealGameProfile::SetModuleSynthesizer( const char *moduleName ){
	pModuleSynthesizer = moduleName;
}

void dealGameProfile::SetModuleNetwork( const char *moduleName ){
	pModuleNetwork = moduleName;
}



void dealGameProfile::SetModuleGraphicVersion( const char *moduleVersion ){
	pModuleGraphicVersion = moduleVersion;
}

void dealGameProfile::SetModuleInputVersion( const char *moduleVersion ){
	pModuleInputVersion = moduleVersion;
}

void dealGameProfile::SetModulePhysicsVersion( const char *moduleVersion ){
	pModulePhysicsVersion = moduleVersion;
}

void dealGameProfile::SetModuleAnimatorVersion( const char *moduleVersion ){
	pModuleAnimatorVersion = moduleVersion;
}

void dealGameProfile::SetModuleAIVersion( const char *moduleVersion ){
	pModuleAIVersion = moduleVersion;
}

void dealGameProfile::SetModuleCrashRecoveryVersion( const char *moduleVersion ){
	pModuleCrashRecoveryVersion = moduleVersion;
}

void dealGameProfile::SetModuleAudioVersion( const char *moduleVersion ){
	pModuleAudioVersion = moduleVersion;
}

void dealGameProfile::SetModuleSynthesizerVersion( const char *moduleVersion ){
	pModuleSynthesizerVersion = moduleVersion;
}

void dealGameProfile::SetModuleNetworkVersion( const char *moduleVersion ){
	pModuleNetworkVersion = moduleVersion;
}



void dealGameProfile::SetRunArguments( const char *arguments ){
	pRunArgs = arguments;
}

void dealGameProfile::SetReplaceRunArguments( bool replaceRunArguments ){
	pReplaceRunArgs = replaceRunArguments;
}

void dealGameProfile::SetWidth( int width ){
	if( width < 1 ){
		DETHROW( deeInvalidParam );
	}
	pWidth = width;
}

void dealGameProfile::SetHeight( int height ){
	if( height < 1 ){
		DETHROW( deeInvalidParam );
	}
	pHeight = height;
}

void dealGameProfile::SetFullScreen( bool fullScreen ){
	pFullScreen = fullScreen;
}



void dealGameProfile::CopyFrom( const dealGameProfile &profile ){
	const dealGPModuleList &moduleList = profile.GetModuleList();
	const int moduleCount = moduleList.GetModuleCount();
	dealGPModule *module = NULL;
	int i;
	
	pName = profile.pName;
	
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
	
	try{
		for( i=0; i<moduleCount; i++ ){
			module = new dealGPModule( *moduleList.GetModuleAt( i ) );
			pModuleList.AddModule( module );
			module = NULL;
		}
		
	}catch( const deException & ){
		if( module ){
			module->FreeReference();
		}
		throw;
	}
	
	pRunArgs = profile.pRunArgs;
	pReplaceRunArgs = profile.pReplaceRunArgs;
	pWidth = profile.pWidth;
	pHeight = profile.pHeight;
	pFullScreen = profile.pFullScreen;
	
	pValid = profile.pValid;
}

void dealGameProfile::Verify( dealLauncher &launcher ){
	pValid = true;
	pValid &= VerifyModule( launcher, pModuleGraphic, pModuleGraphicVersion, dealEngineModule::emtGraphic );
	pValid &= VerifyModule( launcher, pModuleInput, pModuleInputVersion, dealEngineModule::emtInput );
	pValid &= VerifyModule( launcher, pModulePhysics, pModulePhysicsVersion, dealEngineModule::emtPhysics );
	pValid &= VerifyModule( launcher, pModuleAnimator, pModuleAnimatorVersion, dealEngineModule::emtAnimator );
	pValid &= VerifyModule( launcher, pModuleAI, pModuleAIVersion, dealEngineModule::emtAI );
	pValid &= VerifyModule( launcher, pModuleCrashRecovery, pModuleCrashRecoveryVersion, dealEngineModule::emtCrashRecovery );
	pValid &= VerifyModule( launcher, pModuleAudio, pModuleAudioVersion, dealEngineModule::emtAudio );
	pValid &= VerifyModule( launcher, pModuleSynthesizer, pModuleSynthesizerVersion, dealEngineModule::emtSynthesizer );
	pValid &= VerifyModule( launcher, pModuleNetwork, pModuleNetworkVersion, dealEngineModule::emtNetwork );
}

bool dealGameProfile::VerifyModule( dealLauncher &launcher, const char *moduleName, const char *moduleVersion, int requiredType ) const{
	const dealEngineModuleList &moduleList = launcher.GetEngine().GetModuleList();
	dealEngineModule *module;
	
	if( strlen( moduleVersion ) == 0 ){
		module = moduleList.GetModuleNamed( moduleName );
		
	}else{
		module = moduleList.GetModuleNamed( moduleName, moduleVersion );
	}
	
	if( ! module ){
		return false;
	}
	if( module->GetType() != requiredType ){
		return false;
	}
	if( module->GetStatus() != dealEngineModule::emsReady ){
		return false;
	}
	
	return true;
}

void dealGameProfile::Activate( dealLauncher &launcher ) const{
	// disable module versions. this affects all modules
	dealIEngineInstance &engineInstance = *launcher.GetEngine().GetEngine();
	const dealEngineModuleList &engineModuleList = launcher.GetEngine().GetModuleList();
	const int moduleCount = pModuleList.GetModuleCount();
	const int disableModuleVersionCount = pDisableModuleVersionList.GetCount();
	int i;
	
	for( i=0; i<disableModuleVersionCount; i++ ){
		const dealGPDisableModuleVersion &version = *pDisableModuleVersionList.GetAt( i );
		dealEngineModule * const module = engineModuleList.GetModuleNamed( version.GetName(), version.GetVersion() );
		if( module ){
			if( ! engineInstance.EnableModule( module->GetName(), module->GetVersion(), false ) ){
				DETHROW( deeInvalidAction );
			}
		}
	}
	
	// activate modules
	if( ! engineInstance.ActivateModule( pModuleCrashRecovery, pModuleCrashRecoveryVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleGraphic, pModuleGraphicVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleInput, pModuleInputVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModulePhysics, pModulePhysicsVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleAnimator, pModuleAnimatorVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleAI, pModuleAIVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleAudio, pModuleAudioVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleSynthesizer, pModuleSynthesizerVersion ) ){
		DETHROW( deeInvalidAction );
	}
	if( ! engineInstance.ActivateModule( pModuleNetwork, pModuleNetworkVersion ) ){
		DETHROW( deeInvalidAction );
	}
	
	// set module properties
	for( i=0; i<moduleCount; i++ ){
		dealGPModule &module = *pModuleList.GetModuleAt( i );
		dealEngineModule * const engineModule = engineModuleList.GetModuleNamed( module.GetName() );
		if( engineModule ){
			module.ApplyParameters( engineModule->GetVersion(), launcher, engineInstance );
		}
	}
}
