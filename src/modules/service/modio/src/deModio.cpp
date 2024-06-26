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

#include <string.h>

#include "deModio.h"
#include "deModioService.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSRedirect.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *ModioCreateModule( deLoadableModule *loadableModule );
#ifdef __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *ModioCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deModio( *loadableModule );
	}catch( ... ){
		return nullptr;
	}
}


// Class deModio
//////////////////

// Constructor, destructor
////////////////////////////

deModio::deModio( deLoadableModule& loadableModule ) :
deBaseServiceModule( loadableModule ),
pRequiresEventHandlingCount( 0 ){
}

deModio::~deModio(){
}


// Management
///////////////

decStringSet deModio::GetSupportedServices(){
	decStringSet names;
	names.Add( deModioService::serviceName );
	return names;
}

deBaseServiceService* deModio::CreateService( deService *service,
const char *name, const deServiceObject::Ref &data ){
	DEASSERT_NOTNULL( service )
	
	if( strcmp( name, deModioService::serviceName ) == 0 ){
		LogInfo( "deModioService: Create" );
		return new deModioService( *this, service, data );
	}
	
	return nullptr;
}

void deModio::FrameUpdate( float elapsed ){
	if( pRequiresEventHandlingCount > 0 ){
		Modio::RunPendingHandlers();
	}
	
	if( deModioService::Global() ){
		deModioService::Global()->FrameUpdate( elapsed );
	}
}

void deModio::AddVFSContainers( deVirtualFileSystem &vfs, const char *stage ){
	DEASSERT_NOTNULL( stage )
	
	if( strcmp( stage, deModuleSystem::VFSStageMods ) == 0 ){
		if( ! pVFSMods ){
			pVFSMods.TakeOver( new deVirtualFileSystem );
		}
		
		const decPath rootDir( decPath::CreatePathUnix( "/" ) );
		vfs.AddContainer( deVFSRedirect::Ref::New(
			new deVFSRedirect( rootDir, rootDir, pVFSMods, true ) ) );
	}
}

void deModio::AddRequiresEventHandlingCount(){
	pRequiresEventHandlingCount++;
}

void deModio::RemoveRequiresEventHandlingCount(){
	if( pRequiresEventHandlingCount == 0 ){
		LogWarn("RemoveRequiresEventHandlingCount called with pRequiresEventHandlingCount == 0");
		return;
	}
	
	pRequiresEventHandlingCount--;
}
