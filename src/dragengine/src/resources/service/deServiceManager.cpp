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

#include "deServiceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/service/deBaseServiceModule.h"
#include "../../systems/modules/service/deBaseServiceService.h"


// Class deServiceManager
///////////////////////////

// Constructor, Destructor
////////////////////////////

deServiceManager::deServiceManager( deEngine &engine ) :
pEngine( engine ){
}

deServiceManager::~deServiceManager(){
}



// Management
///////////////

decStringSet deServiceManager::GetAllSupportedSerices() const{
	const deModuleSystem &modsys = *pEngine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	decStringSet names;
	int i;
	
	for( i=0; i<count; i++ ){
		const deLoadableModule &loadmod = *modsys.GetModuleAt( i );
		if( loadmod.GetType() != deModuleSystem::emtService ){
			continue;
		}
		if( ! loadmod.IsLoaded() ){
			continue;
		}
		if( ! loadmod.GetModule() ){
			continue;
		}
		
		deBaseServiceModule &srvmod = *( ( deBaseServiceModule* )loadmod.GetModule() );
		names += srvmod.GetSupportedServices();
	}
	
	return names;
}

deService::Ref deServiceManager::CreateService( const char *name ){
	DEASSERT_NOTNULL( name )
	
	const deModuleSystem &modsys = *pEngine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deLoadableModule &loadmod = *modsys.GetModuleAt( i );
		if( loadmod.GetType() != deModuleSystem::emtService ){
			continue;
		}
		if( ! loadmod.IsLoaded() ){
			continue;
		}
		if( ! loadmod.GetModule() ){
			continue;
		}
		
		deBaseServiceModule &srvmod = *( ( deBaseServiceModule* )loadmod.GetModule() );
		deBaseServiceService * const peer = srvmod.CreateService( name );
		if( ! peer ){
			continue;
		}
		
		const deService::Ref service( deService::Ref::New( new deService( srvmod, name ) ) );
		service->SetPeerService( peer );
		pEngine.GetScriptingSystem()->CreateService( service );
		return service;
	}
	
	DETHROW_INFO( deeInvalidParam, "Named service not supported" );
}
