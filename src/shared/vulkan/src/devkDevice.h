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
#include "devkQueue.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>
#include <dragengine/common/collection/decObjectList.h>

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
	
	
	
private:
	devkInstance &pInstance;
	
	VkPhysicalDevice pPhysicalDevice;
	VkPhysicalDeviceProperties pPhysicalDeviceProperties;
	const DeviceConfig pConfig;
	uint32_t pFamilyIndexGraphic;
	uint32_t pFamilyIndexCompute;
	uint32_t pFamilyIndexTransfer;
	
	VkDevice pDevice;
	decObjectList pGraphicQueues;
	decObjectList pComputeQueues;
	decObjectList pTransferQueues;
	
	
	
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
	inline const VkPhysicalDeviceProperties &GetPhysicalDeviceProperties() const{ return pPhysicalDeviceProperties; }
	
	/** Configuration. */
	inline const DeviceConfig &GetConfig() const{ return pConfig; }
	
	/** Device. */
	inline VkDevice GetDevice() const{ return pDevice; }
	
	
	
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
	void pLoadFunctions();
};

#endif

