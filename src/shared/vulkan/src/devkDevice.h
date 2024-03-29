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

#ifndef _DEVKDEVICE_H_
#define _DEVKDEVICE_H_

#include "devkBasics.h"
#include "descriptor/devkDescriptorSetLayoutManager.h"
#include "queue/devkQueue.h"
#include "pipeline/devkPipelineManager.h"
#include "shader/devkShaderModuleManager.h"

#include <dragengine/deObject.h>

class devkInstance;


/**
 * Vulkan device.
 */
class devkDevice : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDevice> Ref;
	
	/** Configuration to use for new device. */
	struct DeviceConfig{
		/** Count of graphic queues to create. */
		int graphicQueueCount;
		
		/** Count of compute queues to create. */
		int computeQueueCount;
		
		/** Count of transfer queues to create. */
		int transferQueueCount;
		
		/** Create struct. */
		DeviceConfig();
	};
	
	/** Extension. */
	enum eExtension{
		extKHRMaintenance3, //<! VK_KHR_MAINTENANCE3_EXTENSION_NAME
		extEXTDescriptorIndexing //<! VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME 
	};
	
	static const int ExtensionCount = extEXTDescriptorIndexing + 1;
	
	
	
private:
	struct sExtension{
		const char *name;
		uint32_t version;
	};
	
	devkInstance &pInstance;
	
	VkPhysicalDevice pPhysicalDevice;
	VkPhysicalDeviceProperties pProperties;
	VkPhysicalDeviceMemoryProperties pMemoryProperties;
	const DeviceConfig pConfig;
	uint32_t pFamilyIndexGraphic;
	uint32_t pFamilyIndexCompute;
	uint32_t pFamilyIndexTransfer;
	
	sExtension pSupportsExtension[ ExtensionCount ];
	
	VkDevice pDevice;
	devkQueue::Ref *pQueues;
	
	devkDescriptorSetLayoutManager pDescriptorSetLayoutManager;
	devkShaderModuleManager pShaderModuleManager;
	devkPipelineManager pPipelineManager;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device. */
	devkDevice( devkInstance &instance, VkPhysicalDevice physicalDevice, const DeviceConfig &config );
	
protected:
	/** Clean up device. */
	virtual ~devkDevice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Owner instance. */
	inline devkInstance &GetInstance() const{ return pInstance; }
	
	/** Physical device. */
	inline VkPhysicalDevice GetPhysicalDevice() const{ return pPhysicalDevice; }
	
	/** Device properties. */
	inline const VkPhysicalDeviceProperties &GetProperties() const{ return pProperties; }
	
	/** Configuration. */
	inline const DeviceConfig &GetConfig() const{ return pConfig; }
	
	/** Device. */
	inline VkDevice GetDevice() const{ return pDevice; }
	
	/** Descriptor set layout manager. */
	inline devkDescriptorSetLayoutManager &GetDescriptorSetLayoutManager(){ return pDescriptorSetLayoutManager; }
	inline const devkDescriptorSetLayoutManager &GetDescriptorSetLayoutManager() const{ return pDescriptorSetLayoutManager; }
	
	/** Shader module manager. */
	inline devkShaderModuleManager &GetShaderModuleManager(){ return pShaderModuleManager; }
	inline const devkShaderModuleManager &GetShaderModuleManager() const{ return pShaderModuleManager; }
	
	/** Pipeline manager. */
	inline devkPipelineManager &GetPipelineManager(){ return pPipelineManager; }
	inline const devkPipelineManager &GetPipelineManager() const{ return pPipelineManager; }
	
	/** Extension is supported. */
	bool SupportsExtension( eExtension extension ) const;
	
	/** Extension version or 0 if not supported. */
	uint32_t ExtensionVersion( eExtension extension ) const;
	
	
	
	/** Device memory properties. */
	inline const VkPhysicalDeviceMemoryProperties &GetMemoryProperties() const{ return pMemoryProperties; }
	
	/** Index of matching memory type. */
	uint32_t IndexOfMemoryType( VkMemoryPropertyFlags property, uint32_t bits ) const;
	uint32_t IndexOfMemoryType( VkMemoryPropertyFlags property, const VkMemoryRequirements &requirements ) const;
	
	
	
	/** Count of graphic queues. */
	int GetGraphicQueueCount() const;
	
	/** Graphic queue at index. */
	devkQueue &GetGraphicQueueAt( int index ) const;
	
	/** First graphic queue. */
	inline devkQueue &GetGraphicQueue() const{ return GetGraphicQueueAt( 0 ); }
	
	/** Count of compute queues. */
	int GetComputeQueueCount() const;
	
	/** Compute queue at index. */
	devkQueue &GetComputeQueueAt( int index ) const;
	
	/** First compute queue. */
	devkQueue &GetComputeQueue() const{ return GetComputeQueueAt( 0 ); }
	
	/** Count of transfer queues. */
	int GetTransferQueueCount() const;
	
	/** Get transfer queue at index. */
	devkQueue &GetTransferQueueAt( int index ) const;
	
	/** Get first transfer queue. */
	devkQueue &GetTransferQueue() const{ return GetTransferQueueAt( 0 ); }
	/*@}*/
	
	
	
	/** \name Vulkan Functions */
	/*@{*/
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateDevice();
	void pGetProperties();
	void pDetectExtensions();
	void pLoadFunctions();
};

#endif

