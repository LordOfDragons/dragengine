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
pInstance( nullptr ),
pPhysicalDevices( nullptr ),
pPhysicalDeviceCount( 0 )
{
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) name = nullptr;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) name = nullptr;
	
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

VkPhysicalDevice devkInstance::GetPhysicalDeviceAt( int index ) const{
	if( index < 0 || index >= pPhysicalDeviceCount ){
		DETHROW( deeInvalidParam );
	}
	return pPhysicalDevices[ index ];
}

devkDevice::Ref devkInstance::CreateDevice( int index, const devkDevice::DeviceConfig &config ){
	return devkDevice::Ref::New( new devkDevice( *this, GetPhysicalDeviceAt( index ), config ) );
}

devkDevice::Ref devkInstance::CreateDeviceHeadlessComputeOnly( int index ){
	devkDevice::DeviceConfig config;
	config.computeQueueCount = 1;
	return CreateDevice( index, config );
}




// Private Functions
//////////////////////

void devkInstance::pCleanUp(){
	if( pPhysicalDevices ){
		delete [] pPhysicalDevices;
	}
	if( pInstance ){
		// DEBUG
		if( debugReportCallback ){
			PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback =
				( PFN_vkDestroyDebugReportCallbackEXT )vkGetInstanceProcAddr( pInstance, "vkDestroyDebugReportCallbackEXT" );
			if( vkDestroyDebugReportCallback ){
				vkDestroyDebugReportCallback( pInstance, debugReportCallback, nullptr );
			}
		}
		// DEBUG
		
		vkDestroyInstance( pInstance, nullptr );
	}
}

#ifdef WITH_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData)
{
	devkInstance * const instance = ( devkInstance* )pUserData;
	instance->GetVulkan().GetModule().LogInfoFormat( "VALIDATION: %s - %s", pLayerPrefix, pMessage );
	return VK_FALSE;
}
#endif

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
	#ifdef WITH_DEBUG
// 	const char* validationLayers[] = { "VK_LAYER_LUNARG_standard_validation" };
	const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
	const uint32_t layerCount = 1;
	#endif
	
	#ifdef WITH_DEBUG
	// check if layers are available
	uint32_t instanceLayerCount = 0;
	vkEnumerateInstanceLayerProperties( &instanceLayerCount, nullptr );
	
	VkLayerProperties *instanceLayers = nullptr;
	if( instanceLayerCount > 0 ){
		instanceLayers = new VkLayerProperties[ instanceLayerCount ];
		vkEnumerateInstanceLayerProperties( &instanceLayerCount, instanceLayers );
	}
	
	bool layersAvailable = true;
	uint32_t i;
	
	pVulkan.GetModule().LogInfo( "Available Vulkan Layers:" );
	for( i=0; i<instanceLayerCount; i++ ){
		pVulkan.GetModule().LogInfoFormat( "- %s", instanceLayers[ i ].layerName );
	}
	
	for( i=0; i<layerCount; i++ ){
		bool layerAvailable = false;
		uint32_t j;
		
		for( j=0; j<instanceLayerCount; j++ ){
			if( strcmp( instanceLayers[ j ].layerName, validationLayers[ i ] ) == 0 ) {
				layerAvailable = true;
				break;
			}
		}
		
		if( ! layerAvailable ){
			pVulkan.GetModule().LogInfoFormat( "Layer '%s' not found", validationLayers[ i ] );
			layersAvailable = false;
			break;
		}
	}
	
	if( instanceLayers ){
		delete [] instanceLayers;
	}
	
	const char * const validationExt = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	if( layersAvailable ){
		pVulkan.GetModule().LogInfo( "Create Layers Available" );
		instanceCreateInfo.ppEnabledLayerNames = validationLayers;
		instanceCreateInfo.enabledLayerCount = layerCount;
		instanceCreateInfo.enabledExtensionCount = 1;
		instanceCreateInfo.ppEnabledExtensionNames = &validationExt;
	}
	#endif
	
	VK_CHECK( pVulkan, vkCreateInstance( &instanceCreateInfo, nullptr, &pInstance ) );
	
	// debug
	#ifdef WITH_DEBUG
	if( layersAvailable ){
		VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = {};
		debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debugReportCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		debugReportCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugMessageCallback;
		debugReportCreateInfo.pUserData = this;
		
		PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
			( PFN_vkCreateDebugReportCallbackEXT )vkGetInstanceProcAddr( pInstance, "vkCreateDebugReportCallbackEXT");
		if( ! vkCreateDebugReportCallbackEXT ){
			DETHROW( deeInvalidAction );
		}
		VK_CHECK( pVulkan, vkCreateDebugReportCallbackEXT( pInstance, &debugReportCreateInfo, nullptr, &debugReportCallback ) );
	}
	#endif
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
	VK_CHECK( pVulkan, vkEnumeratePhysicalDevices( pInstance, &deviceCount, nullptr ) );
	
	if( deviceCount > 0 ){
		pPhysicalDevices = new VkPhysicalDevice[ deviceCount ];
		VK_CHECK( pVulkan, vkEnumeratePhysicalDevices( pInstance, &deviceCount, pPhysicalDevices ) );
		pPhysicalDeviceCount = deviceCount;
	}
	
	pVulkan.GetModule().LogInfo( "Vulkan Devices:" );
	
	VkPhysicalDeviceProperties properties;
	int i;
	
	for( i=0; i<pPhysicalDeviceCount; i++ ){
		memset( &properties, 0, sizeof( properties ) );
		vkGetPhysicalDeviceProperties( pPhysicalDevices[ i ], &properties );
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
