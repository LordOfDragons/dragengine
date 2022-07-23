/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deSharedVulkan.h"
#include "devkLoader.h"
#include "devkInstance.h"
#include "devkGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class deSharedVulkan
/////////////////////////

deSharedVulkan::deSharedVulkan( deBaseModule &module, bool enableDebug ) :
pModule( module ),
pLoader( NULL ),
pCachePath( decPath::CreatePathUnix( "/cache/local/vulkan" ) )
{
	try{
		pLoader = new devkLoader( *this );
		pInstance.TakeOver( new devkInstance( *this, enableDebug ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deSharedVulkan::~deSharedVulkan(){
	pCleanUp();
}



// Management
///////////////

void deSharedVulkan::SetCachePath( const decPath &path ){
	pCachePath = path;
}




// Private Functions
//////////////////////

void deSharedVulkan::pCleanUp(){
	pInstance = nullptr;
	
	if( pLoader ){
		delete pLoader;
	}
}
