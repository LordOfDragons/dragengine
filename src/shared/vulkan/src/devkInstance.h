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

#ifndef _DEVKINSTANCE_H_
#define _DEVKINSTANCE_H_

#include "devkBasics.h"
#include "devkDevice.h"
#include "devkDebug.h"

#include <dragengine/deObject.h>

class deSharedVulkan;


/**
 * Vulkan instance.
 */
class devkInstance : public deObject{
public:
	/** Extension. */
	enum eExtension{
		extKHRSurface, //<! VK_KHR_surface
		extKHRDisplay, //<! VK_KHR_display
		extKHRGetDisplayProperties2, //<! VK_KHR_get_display_properties2
		extKHRGetPhysicalDeviceProperties2, //<! VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
		extKHRGetSurfaceCapabilities2, //<! VK_KHR_get_surface_capabilities2
		extKHRXcbSurface, //<! VK_KHR_xcb_surface
		extKHRXlibSurface, //<! VK_KHR_xlib_surface
		extKHRExternalFenceCapabilities, //<! VK_KHR_external_fence_capabilities
		extKHRExternalMemoryCapabilities, //<! VK_KHR_external_memory_capabilities
		extKHRExternalSemaphoreCapabilities, //<! VK_KHR_external_semaphore_capabilities
		extEXTDebugReport, //<! VK_EXT_debug_report
		extEXTDebugUtils //<! VK_EXT_debug_utils
	};
	
	static const int ExtensionCount = extEXTDebugUtils + 1;
	
	/** Layers. */
	enum eLayer{
		layerKhronosValidation, //<! VK_LAYER_KHRONOS_validation
		layerLunargStandardValidation, //<! VK_LAYER_LUNARG_standard_validation
		layerRenderdocCapture, //<! VK_LAYER_RENDERDOC_Capture
		layerValveSteamOverlay64, //<! VK_LAYER_VALVE_steam_overlay_64
		layerValveSteamOverlay32, //<! VK_LAYER_VALVE_steam_overlay_32
	};
	
	static const int LayerCount = layerValveSteamOverlay32 + 1;
	
	/** Reference. */
	typedef deTObjectReference<devkInstance> Ref;
	
	
private:
	struct sExtension{
		const char *name;
		uint32_t version;
	};
	struct sLayer{
		const char *name;
		uint32_t version;
		uint32_t implementationVersion;
	};
	
	deSharedVulkan &pVulkan;
	devkDebug pDebug;
	
	VkInstance pInstance;
	
	sExtension pSupportsExtension[ ExtensionCount ];
	sLayer pSupportsLayer[ LayerCount ];
	
	VkPhysicalDevice *pPhysicalDevices;
	int pPhysicalDeviceCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create instance. */
	devkInstance( deSharedVulkan &vulkan, bool enableDebug );
	
protected:
	/** Clean up instance. */
	virtual ~devkInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shared vulkan. */
	inline deSharedVulkan &GetVulkan() const{ return pVulkan; }
	
	/** Debug support. */
	inline devkDebug &GetDebug(){ return pDebug; }
	
	
	
	/** Extension is supported. */
	bool SupportsExtension( eExtension extension ) const;
	
	/** Extension version or 0 if not supported. */
	uint32_t ExtensionVersion( eExtension extension ) const;
	
	
	
	/** Layer is supported. */
	bool SupportsLayer( eLayer layer ) const;
	
	/** Layer version or 0 if not supported. */
	uint32_t LayerVersion( eLayer layer ) const;
	
	
	
	/** Instance. */
	inline VkInstance GetInstance() const{ return pInstance; }
	
	/** Count of physical devices. */
	inline int GetPhysicalDeviceCount() const{ return pPhysicalDeviceCount; }
	
	/** Physical device at index. */
	VkPhysicalDevice GetPhysicalDeviceAt( int index ) const;
	
	/** Create device. */
	devkDevice::Ref CreateDevice( int index, const devkDevice::DeviceConfig &config );
	
	/** Create headless device for compute use only. */
	devkDevice::Ref CreateDeviceHeadlessComputeOnly( int index );
	
	/** Create headless device for graphic use. */
	devkDevice::Ref CreateDeviceHeadlessGraphic( int index );
	/*@}*/
	
	
	
	/** \name Vulkan Functions */
	/*@{*/
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
	
	#include "devkFunctionNames.h"
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pDetectExtensions();
	void pDetectLayers();
	void pCreateInstance( bool enableValidationLayers );
	void pLoadFunctions();
	void pFindDevices();
};

#endif

