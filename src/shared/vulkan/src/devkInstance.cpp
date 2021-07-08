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

#include "devkInstance.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkInstance
///////////////////////

devkInstance::devkInstance( deSharedVulkan &vulkan ) :
pVulkan( vulkan ),
pInstance( NULL ),
pDevices( NULL ),
pDeviceCount( 0 )
{
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) name = NULL;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) name = NULL;
	
	#include "devkFunctionNames.h"
	
	try{
		pCreateInstance();
		pLoadFunctions();
		pFindDevices();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkInstance::~devkInstance(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void devkInstance::pCleanUp(){
	if( pDevices ){
		delete [] pDevices;
	}
	if( pInstance ){
		vkDestroyInstance( pInstance, NULL );
	}
}

void devkInstance::pCreateInstance(){
	pVulkan.GetModule().LogInfo( "Create Vulkan Instance" );
	
	// set up application information
	VkApplicationInfo appInfo;
	memset( &appInfo, 0, sizeof( appInfo ) );
	
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Drag[en]gine";
	appInfo.pEngineName = "Drag[en]gine";
	appInfo.apiVersion = VK_API_VERSION_1_0;
	
	// create instance
	VkInstanceCreateInfo instanceCreateInfo;
	memset( &instanceCreateInfo, 0, sizeof( instanceCreateInfo ) );
	
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	
	// init layers
	const char* validationLayers[] = { "VK_LAYER_LUNARG_standard_validation" };
	const uint32_t layerCount = 1;
	
	#ifdef WITH_DEBUG
	// check if layers are available
	uint32_t instanceLayerCount = 0;
	vkEnumerateInstanceLayerProperties( &instanceLayerCount, NULL );
	
	VkLayerProperties *instanceLayers = NULL;
	if( instanceLayerCount > 0 ){
		instanceLayers = new VkLayerProperties[ instanceLayerCount ];
		vkEnumerateInstanceLayerProperties( &instanceLayerCount, instanceLayers );
	}
	
	bool layersAvailable = true;
	uint32_t i;
	for( i=0; i<layerCount; i++ ){
		bool layerAvailable = false;
		uint32_t j;
		
		for( j=0; j<instanceLayerCount; j++ ){
			if( strcmp( instanceLayers[ j ].layerName, validationLayers[ i ] ) == 0 ) {
				layerAvailable = true;
				break;
			}
			if( ! layerAvailable ){
				layersAvailable = false;
				break;
			}
		}
	}
	
	if( instanceLayers ){
		delete [] instanceLayers;
	}
	
	const char * const validationExt = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	if( layersAvailable ){
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;
		instanceCreateInfo.enabledLayerCount = layerCount;
		instanceCreateInfo.enabledExtensionCount = 1;
		instanceCreateInfo.ppEnabledExtensionNames = &validationExt;
	}
	#endif
	
	VK_CHECK( pVulkan, vkCreateInstance( &instanceCreateInfo, NULL, &pInstance ) );
}

void devkInstance::pLoadFunctions(){
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) \
		name = ( PFN_##name )vkGetInstanceProcAddr( pInstance, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Instance function " #name " not found" ); \
		}
	
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) \
		name = ( PFN_##name )vkGetInstanceProcAddr( pInstance, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Instance function " #name " not found" ); \
		}
	
	#include "devkFunctionNames.h"
}

void devkInstance::pFindDevices(){
	uint32_t deviceCount = 0;
	VK_CHECK( pVulkan, vkEnumeratePhysicalDevices( pInstance, &deviceCount, NULL ) );
	
	if( deviceCount > 0 ){
		pDevices = new VkPhysicalDevice[ deviceCount ];
		VK_CHECK( pVulkan, vkEnumeratePhysicalDevices( pInstance, &deviceCount, pDevices ) );
		pDeviceCount = deviceCount;
	}
	
	pVulkan.GetModule().LogInfo( "Vulkan Devices:" );
	
	VkPhysicalDeviceProperties properties;
	int i;
	
	for( i=0; i<pDeviceCount; i++ ){
		memset( &properties, 0, sizeof( properties ) );
		vkGetPhysicalDeviceProperties( pDevices[ i ], &properties );
		pVulkan.GetModule().LogInfoFormat( "- %s (id=%d vendor=%d version=%d.%d.%d.%d api=%d.%d.%d.%d)",
			properties.deviceName, properties.deviceID, properties.vendorID,
			VK_API_VERSION_MAJOR( properties.driverVersion ),
			VK_API_VERSION_MINOR( properties.driverVersion ),
			VK_API_VERSION_PATCH( properties.driverVersion ),
			VK_API_VERSION_VARIANT( properties.driverVersion ),
			VK_API_VERSION_MAJOR( properties.apiVersion ),
			VK_API_VERSION_MINOR( properties.apiVersion ),
			VK_API_VERSION_PATCH( properties.apiVersion ),
			VK_API_VERSION_VARIANT( properties.apiVersion ) );
	}
}
