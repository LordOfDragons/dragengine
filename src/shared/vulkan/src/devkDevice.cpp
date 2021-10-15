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
pDevice( VK_NULL_HANDLE ),
pQueues( nullptr ),
pDescriptorSetLayoutManager( *this ),
pShaderModuleManager( *this ),
pPipelineManager( *this )
{
	memset( pSupportsExtension, 0, sizeof( pSupportsExtension ) );
	pSupportsExtension[ extKHRMaintenance3 ].name = VK_KHR_MAINTENANCE3_EXTENSION_NAME;
	pSupportsExtension[ extEXTDescriptorIndexing ].name = VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME;
	
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
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) name = VK_NULL_HANDLE;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) name = VK_NULL_HANDLE;
	
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

bool devkDevice::SupportsExtension( eExtension extension ) const{
	return pSupportsExtension[ extension ].version != 0;
}

uint32_t devkDevice::ExtensionVersion( eExtension extension ) const{
	return pSupportsExtension[ extension ].version;
}

uint32_t devkDevice::IndexOfMemoryType( VkMemoryPropertyFlags property, uint32_t bits ) const{
	uint32_t i;
	for( i=0; i<pMemoryProperties.memoryTypeCount; i++ ){
		if( ( bits & 1 ) == 1 && ( pMemoryProperties.memoryTypes[ i ].propertyFlags & property ) == property ) {
			return i;
		}
		bits >>= 1;
	}
	
	DETHROW( deeInvalidParam );
}

uint32_t devkDevice::IndexOfMemoryType( VkMemoryPropertyFlags property,
const VkMemoryRequirements& requirements ) const{
	return IndexOfMemoryType( property, requirements.memoryTypeBits );
}



int devkDevice::GetGraphicQueueCount() const{
	return pConfig.graphicQueueCount;
}

devkQueue &devkDevice::GetGraphicQueueAt( int index ) const{
	if( index < 0 || index >= pConfig.graphicQueueCount ){
		DETHROW( deeInvalidParam );
	}
	return pQueues[ index ];
}

int devkDevice::GetComputeQueueCount() const{
	return pConfig.computeQueueCount;
}

devkQueue &devkDevice::GetComputeQueueAt( int index ) const{
	if( index < 0 || index >= pConfig.computeQueueCount ){
		DETHROW( deeInvalidParam );
	}
	return pQueues[ pConfig.graphicQueueCount + index ];
}

int devkDevice::GetTransferQueueCount() const{
	return pConfig.transferQueueCount;
}

devkQueue &devkDevice::GetTransferQueueAt( int index ) const{
	if( index < 0 || index >= pConfig.transferQueueCount ){
		DETHROW( deeInvalidParam );
	}
	return pQueues[ pConfig.graphicQueueCount + pConfig.computeQueueCount + index ];
}



// Private Functions
//////////////////////

void devkDevice::pCleanUp(){
	// ensure all queues are idle before releasing them
	const int queueCount = pConfig.graphicQueueCount + pConfig.computeQueueCount + pConfig.transferQueueCount;
	int i;
	for( i=0; i<queueCount; i++ ){
		pQueues[ i ]->WaitIdle();
	}
	
	// not it is save to release queues
	if( pQueues ){
		delete [] pQueues;
	}
	
	// ensure all resources cleaned up using destructor calls are cleaned up before
	// the device is destroyed
	pPipelineManager.Clear();
	pShaderModuleManager.Clear();
	pDescriptorSetLayoutManager.Clear();
	
	// now it is safe to destroy the device
	if( pDevice ){
		vkDestroyDevice( pDevice, VK_NULL_HANDLE );
		pDevice = VK_NULL_HANDLE;
	}
}

void devkDevice::pCreateDevice(){
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	module.LogInfo( "Create Vulkan Device:" );
	
	pGetProperties();
	pDetectExtensions();
	
	// find requested queue families
	const float defaultQueuePriority = 0.0f;
	
	uint32_t queueFamilyCount;
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties( pPhysicalDevice, &queueFamilyCount, VK_NULL_HANDLE );
	
	VkQueueFamilyProperties *queueFamilyProperties = nullptr;
	if( queueFamilyCount == 0 ){
		DETHROW_INFO( deeInvalidAction, "no queues found" );
	}
	
	queueFamilyProperties = new VkQueueFamilyProperties[ queueFamilyCount ];
	
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties( pPhysicalDevice, &queueFamilyCount, queueFamilyProperties );
	int i;
	
	VkDeviceQueueCreateInfo queueCreateInfo[ 3 ];
	memset( &queueCreateInfo, 0, sizeof( queueCreateInfo ) );
	
	int requiredQueueCount = 0;
	int queueGraphic = -1;
	int queueCompute = -1;
	int queueTransfer = -1;
	
	if( pConfig.graphicQueueCount > 0 ){
		queueGraphic = requiredQueueCount++;
	}
	if( pConfig.computeQueueCount > 0 ){
		queueCompute = requiredQueueCount++;
	}
	if( pConfig.transferQueueCount > 0 ){
		queueTransfer = requiredQueueCount++;
	}
	
	for( i=0; i<(int)queueFamilyCount; i++ ){
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) == VK_QUEUE_GRAPHICS_BIT
		&& pFamilyIndexGraphic == 0 && queueGraphic != -1 ){
			queueCreateInfo[ queueGraphic ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueGraphic ].queueFamilyIndex = i;
			queueCreateInfo[ queueGraphic ].queueCount = pConfig.graphicQueueCount;
			queueCreateInfo[ queueGraphic ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexGraphic = i;
		}
		
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_COMPUTE_BIT ) == VK_QUEUE_COMPUTE_BIT
		&& pFamilyIndexCompute == 0 && queueCompute != -1 ){
			queueCreateInfo[ queueCompute ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueCompute ].queueFamilyIndex = i;
			queueCreateInfo[ queueCompute ].queueCount = pConfig.computeQueueCount;
			queueCreateInfo[ queueCompute ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexCompute = i;
		}
		
		if( ( queueFamilyProperties[ i ].queueFlags & VK_QUEUE_TRANSFER_BIT ) == VK_QUEUE_TRANSFER_BIT
		&& pFamilyIndexTransfer == 0 && queueTransfer != -1 ){
			queueCreateInfo[ queueTransfer ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueTransfer ].queueFamilyIndex = i;
			queueCreateInfo[ queueTransfer ].queueCount = pConfig.transferQueueCount;
			queueCreateInfo[ queueTransfer ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexTransfer = i;
		}
	}
	
	if( queueFamilyProperties ){
		delete [] queueFamilyProperties;
	}
	
	if( queueGraphic != -1
	&& queueCreateInfo[ queueGraphic ].queueCount != ( uint32_t )pConfig.graphicQueueCount ){
		DETHROW_INFO( deeInvalidAction, "graphic queue count mismatch"  );
	}
	if( queueCompute != -1
	&& queueCreateInfo[ queueCompute ].queueCount != ( uint32_t )pConfig.computeQueueCount ){
		DETHROW_INFO( deeInvalidAction, "compute queue count mismatch"  );
	}
	if( queueTransfer != -1
	&& queueCreateInfo[ queueTransfer ].queueCount != ( uint32_t )pConfig.transferQueueCount ){
		DETHROW_INFO( deeInvalidAction, "transfer queue count mismatch"  );
	}
	
	// create logical device
	VkDeviceCreateInfo deviceCreateInfo;
	memset( &deviceCreateInfo, 0, sizeof( deviceCreateInfo ) );
	
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = requiredQueueCount;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo[ 0 ];
	VK_CHECK( pInstance.GetVulkan(), pInstance.vkCreateDevice( pPhysicalDevice,
		&deviceCreateInfo, VK_NULL_HANDLE, &pDevice ) );
	
	// load functions. below requires them
	pLoadFunctions();
	
	// create queue instances
	if( pConfig.graphicQueueCount + pConfig.computeQueueCount + pConfig.transferQueueCount > 0 ){
		pQueues = new devkQueue::Ref[ pConfig.graphicQueueCount
			+ pConfig.computeQueueCount + pConfig.transferQueueCount ];
		
		int nextQueue = 0;
		
		for( i=0; i<pConfig.graphicQueueCount; i++ ){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue( pDevice, pFamilyIndexGraphic, i, &queue );
			if( ! queue ){
				DETHROW_INFO( deeInvalidAction, "get graphic device queue failed" );
			}
			pQueues[ nextQueue++ ].TakeOver( new devkQueue( *this, pFamilyIndexGraphic, queue ) );
		}
		
		for( i=0; i<pConfig.computeQueueCount; i++ ){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue( pDevice, pFamilyIndexCompute, i, &queue );
			if( ! queue ){
				DETHROW_INFO( deeInvalidAction, "get compute device queue failed" );
			}
			pQueues[ nextQueue++ ].TakeOver( new devkQueue( *this, pFamilyIndexCompute, queue ) );
		}
		
		for( i=0; i<pConfig.transferQueueCount; i++ ){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue( pDevice, pFamilyIndexTransfer, i, &queue );
			if( ! queue ){
				DETHROW_INFO( deeInvalidAction, "get transfer device queue failed" );
			}
			pQueues[ nextQueue++ ].TakeOver( new devkQueue( *this, pFamilyIndexTransfer, queue ) );
		}
	}
}

void devkDevice::pGetProperties(){
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	pInstance.vkGetPhysicalDeviceProperties( pPhysicalDevice, &pProperties );
	pInstance.vkGetPhysicalDeviceMemoryProperties( pPhysicalDevice, &pMemoryProperties );
	
	module.LogInfoFormat( "- Device Name: %s", pProperties.deviceName );
	module.LogInfoFormat( "- Device ID: %d", pProperties.deviceID );
	module.LogInfoFormat( "- Vendor ID: %d", pProperties.vendorID );
	module.LogInfoFormat( "- Driver Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR( pProperties.driverVersion ),
		VK_API_VERSION_MINOR( pProperties.driverVersion ),
		VK_API_VERSION_PATCH( pProperties.driverVersion ),
		VK_API_VERSION_VARIANT( pProperties.driverVersion ) );
	module.LogInfoFormat( "- API Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR( pProperties.apiVersion ),
		VK_API_VERSION_MINOR( pProperties.apiVersion ),
		VK_API_VERSION_PATCH( pProperties.apiVersion ),
		VK_API_VERSION_VARIANT( pProperties.apiVersion ) );
	
}

void devkDevice::pDetectExtensions(){
	deSharedVulkan &vulkan = pInstance.GetVulkan();
	uint32_t count = 0;
	VK_CHECK( vulkan, pInstance.vkEnumerateDeviceExtensionProperties(
		pPhysicalDevice, VK_NULL_HANDLE, &count, VK_NULL_HANDLE ) );
	if( count == 0 ){
		return;
	}
	
	VkExtensionProperties * const extensions = new VkExtensionProperties[ count ];
	try{
		VK_CHECK( vulkan, pInstance.vkEnumerateDeviceExtensionProperties(
			pPhysicalDevice, VK_NULL_HANDLE, &count, extensions ) );
		
		// report all extensions reported for debug purpose
		deBaseModule &baseModule = vulkan.GetModule();
		uint32_t i;
		
		baseModule.LogInfo( "Device Extensions:" );
		for( i=0; i<count; i++ ){
			baseModule.LogInfoFormat( "- %s: %d", extensions[ i ].extensionName, extensions[ i ].specVersion );
		}
		
		// store supported extensions
		int j;
		for( i=0; i<count; i++ ){
			for( j=0; j<ExtensionCount; j++ ){
				if( strcmp( pSupportsExtension[ j ].name, extensions[ i ].extensionName ) == 0 ){
					pSupportsExtension[ j ].version = extensions[ i ].specVersion;
					break;
				}
			}
		}
		
		// report support extensions
		baseModule.LogInfo( "Supported Extensions:" );
		for( i=0; i<ExtensionCount; i++ ){
			if( pSupportsExtension[ i ].version ){
				baseModule.LogInfoFormat( "- %s: %d", pSupportsExtension[ i ].name,
					pSupportsExtension[ i ].version );
			}
		}
		
		baseModule.LogInfo( "Not Supported Extensions:" );
		for( i=0; i<ExtensionCount; i++ ){
			if( ! pSupportsExtension[ i ].version ){
				baseModule.LogInfoFormat( "- %s", pSupportsExtension[ i ].name );
			}
		}
		
		delete [] extensions;
		
	}catch( const deException & ){
		delete [] extensions;
		throw;
	}
}

void devkDevice::pLoadFunctions(){
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) \
		name = ( PFN_##name )vkGetDeviceProcAddr( pDevice, #name ); \
		if( ! name ){ \
			DETHROW_INFO( deeInvalidAction, "Device function " #name " not found" ); \
		}
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) \
		if( pInstance.SupportsExtension( extension ) ){ \
			DEVICE_LEVEL_VULKAN_FUNCTION( name ) \
		}
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
}
