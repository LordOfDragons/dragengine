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

#include "devkDevice.h"
#include "devkInstance.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDevice
///////////////////////

devkDevice::devkDevice( devkInstance &instance ) :
pInstance( instance ),
pDevice( NULL )
{
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) name = NULL;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) name = NULL;
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	
	try{
		pCreateDevice();
		pLoadFunctions();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkDevice::~devkDevice(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void devkDevice::pCleanUp(){
	if( pDevice ){
		vkDestroyDevice( pDevice, NULL );
	}
}

void devkDevice::pCreateDevice(){
	pInstance.GetVulkan().GetModule().LogInfo( "Create Vulkan Device" );
}

void devkDevice::pLoadFunctions(){
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) \
		name = ( PFN_##name )vkGetDeviceProcAddr( pDevice, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Device function " #name " not found" ); \
		}
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) \
		name = ( PFN_##name )vkGetInstanceProcAddr( pInstance, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Device function " #name " not found" ); \
		}
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
}
