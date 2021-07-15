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

#ifndef _DEVKINSTANCE_H_
#define _DEVKINSTANCE_H_

#include "devkBasics.h"
#include "devkInstance.h"
#include "devkDevice.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>

class deSharedVulkan;


/**
 * Vulkan instance.
 */
class devkInstance : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkInstance> Ref;
	
	
	
private:
	deSharedVulkan &pVulkan;
	
	VkInstance pInstance;
	
	VkPhysicalDevice *pPhysicalDevices;
	int pPhysicalDeviceCount;
	
	// DEBUG
	VkDebugReportCallbackEXT debugReportCallback{};
	// DEBUG
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create instance. */
	devkInstance( deSharedVulkan &vulkan );
	
protected:
	/** Clean up instance. */
	virtual ~devkInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shared vulkan. */
	inline deSharedVulkan &GetVulkan() const{ return pVulkan; }
	
	/** Count of physical devices. */
	inline int GetPhysicalDeviceCount() const{ return pPhysicalDeviceCount; }
	
	/** Physical device at index. */
	VkPhysicalDevice GetPhysicalDeviceAt( int index ) const;
	
	/** Create device. */
	devkDevice::Ref CreateDevice( int index, const devkDevice::DeviceConfig &config );
	
	/** Create headless device for compute use only. */
	devkDevice::Ref CreateDeviceHeadlessComputeOnly( int index );
	/*@}*/
	
	
	
	/** \name Vulkan Functions */
	/*@{*/
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
	
	#include "devkFunctionNames.h"
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateInstance();
	void pLoadFunctions();
	void pFindDevices();
};

#endif

