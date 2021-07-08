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


// Struct devkDevice::DeviceConfig
////////////////////////////////////

devkDevice::DeviceConfig::DeviceConfig() :
graphicQueueCount( 0 ),
computeQueueCount( 0 ),
transferQueueCount( 0 ){
}


// Class devkDevice
///////////////////////

devkDevice::devkDevice( devkInstance &instance, VkPhysicalDevice physicalDevice,
const DeviceConfig &config ) :
pInstance( instance ),
pPhysicalDevice( physicalDevice ),
pConfig( config ),
pFamilyIndexGraphic( 0 ),
pFamilyIndexCompute( 0 ),
pFamilyIndexTransfer( 0 ),
pDevice( nullptr )
{
	if( ! physicalDevice ){
		DETHROW_INFO( deeNullPointer, "physicalDevice" );
	}
	if( config.graphicQueueCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "config.graphicQueueCount" );
	}
	if( config.computeQueueCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "config.computeQueueCount" );
	}
	if( config.transferQueueCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "config.transferQueueCount" );
	}
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) name = nullptr;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) name = nullptr;
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	
	try{
		pCreateDevice();
		
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
	pTransferQueues.RemoveAll();
	pComputeQueues.RemoveAll();
	pGraphicQueues.RemoveAll();
	
	if( pDevice ){
		vkDestroyDevice( pDevice, nullptr );
	}
}

void devkDevice::pCreateDevice(){
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	module.LogInfo( "Create Vulkan Device:" );
	
	// get device properties and log them
	pInstance.vkGetPhysicalDeviceProperties( pPhysicalDevice, &pPhysicalDeviceProperties );
	
	module.LogInfoFormat( "- Device Name: %s", pPhysicalDeviceProperties.deviceName );
	module.LogInfoFormat( "- Device ID: %d", pPhysicalDeviceProperties.deviceID );
	module.LogInfoFormat( "- Vendor ID: %d", pPhysicalDeviceProperties.vendorID );
	module.LogInfoFormat( "- Driver Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR( pPhysicalDeviceProperties.driverVersion ),
		VK_API_VERSION_MINOR( pPhysicalDeviceProperties.driverVersion ),
		VK_API_VERSION_PATCH( pPhysicalDeviceProperties.driverVersion ),
		VK_API_VERSION_VARIANT( pPhysicalDeviceProperties.driverVersion ) );
	module.LogInfoFormat( "- API Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR( pPhysicalDeviceProperties.apiVersion ),
		VK_API_VERSION_MINOR( pPhysicalDeviceProperties.apiVersion ),
		VK_API_VERSION_PATCH( pPhysicalDeviceProperties.apiVersion ),
		VK_API_VERSION_VARIANT( pPhysicalDeviceProperties.apiVersion ) );
	
	// find requested queue families
	const float defaultQueuePriority = 0.0f;
	VkDeviceQueueCreateInfo queueCreateInfo[ 3 ];
	memset( &queueCreateInfo, 0, sizeof( queueCreateInfo ) );
	
	uint32_t queueFamilyCount;
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties( pPhysicalDevice, &queueFamilyCount, nullptr );
	
	VkQueueFamilyProperties *queueFamilyProperties = nullptr;
	if( queueFamilyCount == 0 ){
		DETHROW_INFO( deeInvalidAction, "no queues found" );
	}
	
	queueFamilyProperties = new VkQueueFamilyProperties[ queueFamilyCount ];
	
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties( pPhysicalDevice, &queueFamilyCount, queueFamilyProperties );
	uint32_t i;
	
	for( i=0; i<queueFamilyCount; i++ ){
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) == VK_QUEUE_GRAPHICS_BIT
		&& pFamilyIndexGraphic == 0 ){
			queueCreateInfo[ 0 ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ 0 ].queueFamilyIndex = i;
			queueCreateInfo[ 0 ].queueCount = pConfig.graphicQueueCount;
			queueCreateInfo[ 0 ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexGraphic = i;
		}
		
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_COMPUTE_BIT ) == VK_QUEUE_COMPUTE_BIT
		&& pFamilyIndexCompute == 0 ){
			queueCreateInfo[ 1 ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ 1 ].queueFamilyIndex = i;
			queueCreateInfo[ 1 ].queueCount = pConfig.computeQueueCount;
			queueCreateInfo[ 1 ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexCompute = i;
		}
		
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_TRANSFER_BIT ) == VK_QUEUE_TRANSFER_BIT
		&& pFamilyIndexTransfer == 0 ){
			queueCreateInfo[ 2 ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ 2 ].queueFamilyIndex = i;
			queueCreateInfo[ 2 ].queueCount = pConfig.transferQueueCount;
			queueCreateInfo[ 2 ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexTransfer = i;
		}
	}
	
	if( queueFamilyProperties ){
		delete [] queueFamilyProperties;
	}
	
	if( pConfig.graphicQueueCount > 0 && pFamilyIndexGraphic == 0 ){
		DETHROW_INFO( deeInvalidAction, "device supports no graphic queues"  );
	}
	if( pConfig.computeQueueCount > 0 && pFamilyIndexCompute == 0 ){
		DETHROW_INFO( deeInvalidAction, "device supports no compute queues"  );
	}
	if( pConfig.transferQueueCount > 0 && pFamilyIndexTransfer == 0 ){
		DETHROW_INFO( deeInvalidAction, "device supports no transfer queues"  );
	}
	
	// create logical device
	VkDeviceCreateInfo deviceCreateInfo;
	memset( &deviceCreateInfo, 0, sizeof( deviceCreateInfo ) );
	
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 3;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo[ 0 ];
	VK_CHECK( pInstance.GetVulkan(), pInstance.vkCreateDevice( pPhysicalDevice,
		&deviceCreateInfo, nullptr, &pDevice ) );
	
	// load functions. below requires them
	pLoadFunctions();
	
	// create queue instances
	VkQueue queue;
	
	for( i=0; i<queueCreateInfo[ 0 ].queueCount; i++ ){
		queue = nullptr;
		vkGetDeviceQueue( pDevice, pFamilyIndexGraphic, i, &queue );
		if( ! queue ){
			DETHROW_INFO( deeInvalidAction, "get graphic device queue failed" );
		}
		pGraphicQueues.Add( devkQueue::Ref::With( new devkQueue( *this, queue ) ) );
	}
	
	for( i=0; i<queueCreateInfo[ 1 ].queueCount; i++ ){
		queue = nullptr;
		vkGetDeviceQueue( pDevice, pFamilyIndexCompute, i, &queue );
		if( ! queue ){
			DETHROW_INFO( deeInvalidAction, "get compute device queue failed" );
		}
		pComputeQueues.Add( devkQueue::Ref::With( new devkQueue( *this, queue ) ) );
	}
	
	for( i=0; i<queueCreateInfo[ 2 ].queueCount; i++ ){
		queue = nullptr;
		vkGetDeviceQueue( pDevice, pFamilyIndexTransfer, i, &queue );
		if( ! queue ){
			DETHROW_INFO( deeInvalidAction, "get transfer device queue failed" );
		}
		pTransferQueues.Add( devkQueue::Ref::With( new devkQueue( *this, queue ) ) );
	}
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
