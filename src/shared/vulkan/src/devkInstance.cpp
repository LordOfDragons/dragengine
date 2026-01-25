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

#include "devkInstance.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkInstance
///////////////////////

devkInstance::devkInstance(deSharedVulkan &vulkan, bool enableDebug) :
pVulkan(vulkan),
pDebug(*this),
pInstance(VK_NULL_HANDLE)
{
	#ifndef WITH_DEBUG
	(void)enableDebug;
	#endif
	
	memset(&pSupportsExtension, 0, sizeof(pSupportsExtension));
	pSupportsExtension[extKHRSurface].name = VK_KHR_SURFACE_EXTENSION_NAME;
	pSupportsExtension[extKHRDisplay].name = VK_KHR_DISPLAY_EXTENSION_NAME;
	pSupportsExtension[extKHRGetDisplayProperties2].name = VK_KHR_GET_DISPLAY_PROPERTIES_2_EXTENSION_NAME;
	pSupportsExtension[extKHRGetPhysicalDeviceProperties2].name = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
	pSupportsExtension[extKHRGetSurfaceCapabilities2].name = VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME;
	pSupportsExtension[extKHRXcbSurface].name = "VK_KHR_xcb_surface";
	pSupportsExtension[extKHRXlibSurface].name = "VK_KHR_xlib_surface";
	pSupportsExtension[extKHRExternalFenceCapabilities].name = VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME;
	pSupportsExtension[extKHRExternalMemoryCapabilities].name = VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME;
	pSupportsExtension[extKHRExternalSemaphoreCapabilities].name = VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME;
	pSupportsExtension[extEXTDebugReport].name = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	pSupportsExtension[extEXTDebugUtils].name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	
	memset(&pSupportsLayer, 0, sizeof(pSupportsLayer));
	pSupportsLayer[layerKhronosValidation].name = "VK_LAYER_KHRONOS_validation";
	pSupportsLayer[layerLunargStandardValidation].name = "VK_LAYER_LUNARG_standard_validation";
	pSupportsLayer[layerRenderdocCapture].name = "VK_LAYER_RENDERDOC_Capture";
	pSupportsLayer[layerValveSteamOverlay64].name = "VK_LAYER_VALVE_steam_overlay_64";
	pSupportsLayer[layerValveSteamOverlay32].name = "VK_LAYER_VALVE_steam_overlay_32";
	
	#define INSTANCE_LEVEL_VULKAN_FUNCTION(name) name = VK_NULL_HANDLE;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) name = VK_NULL_HANDLE;
	
	#include "devkFunctionNames.h"
	
	try{
		#ifdef WITH_DEBUG
			pCreateInstance(enableDebug);
		#else
			pCreateInstance(false);
		#endif
		
		pLoadFunctions();
		
		#ifdef WITH_DEBUG
			pDebug.SetEnabled(enableDebug);
		#endif
		
		pFindDevices();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkInstance::~devkInstance(){
	pCleanUp();
}



// Management
///////////////

bool devkInstance::SupportsExtension(eExtension extension) const{
	return pSupportsExtension[extension].version != 0;
}

uint32_t devkInstance::ExtensionVersion(eExtension extension) const{
	return pSupportsExtension[extension].version;
}

bool devkInstance::SupportsLayer(eLayer layer) const{
	return pSupportsLayer[layer].implementationVersion != 0;
}

uint32_t devkInstance::LayerVersion(eLayer layer) const{
	return pSupportsLayer[layer].implementationVersion;
}

devkDevice::Ref devkInstance::CreateDevice(int index, const devkDevice::DeviceConfig &config){
	return devkDevice::Ref::New(*this, pPhysicalDevices[index], config);
}

devkDevice::Ref devkInstance::CreateDeviceHeadlessComputeOnly(int index){
	devkDevice::DeviceConfig config;
	config.computeQueueCount = 1;
	return CreateDevice(index, config);
}

devkDevice::Ref devkInstance::CreateDeviceHeadlessGraphic(int index){
	devkDevice::DeviceConfig config;
	config.graphicQueueCount = 1;
	config.computeQueueCount = 1;
	config.transferQueueCount = 1;
	return CreateDevice(index, config);
}




// Private Functions
//////////////////////

void devkInstance::pCleanUp(){
	pDebug.SetEnabled(false);
	
	if(pInstance){
		vkDestroyInstance(pInstance, VK_NULL_HANDLE);
	}
}

void devkInstance::pDetectExtensions(){
	uint32_t count = 0;
	VK_CHECK(pVulkan, pvkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &count, VK_NULL_HANDLE));
	if(count == 0){
		return;
	}
	
	decTList<VkExtensionProperties> extensions((int)count, VkExtensionProperties{});
	VK_CHECK(pVulkan, pvkEnumerateInstanceExtensionProperties(
		VK_NULL_HANDLE, &count, extensions.GetArrayPointer()));
	
	// report all extensions for debug purpose
	deBaseModule &baseModule = pVulkan.GetModule();
	
	baseModule.LogInfo("Extensions:");
	extensions.Visit([&](const VkExtensionProperties &ext){
		baseModule.LogInfoFormat("- %s: %d", ext.extensionName, ext.specVersion);
	});
	
	// store supported extensions
	int i, j;
	for(i=0; i<(int)count; i++){
		for(j=0; j<ExtensionCount; j++){
			if(strcmp(pSupportsExtension[j].name, extensions[i].extensionName) == 0){
				pSupportsExtension[j].version = extensions[i].specVersion;
				break;
			}
		}
	}
	
	// report support extensions
	baseModule.LogInfo("Supported Extensions:");
	for(i=0; i<ExtensionCount; i++){
		if(pSupportsExtension[i].version){
			baseModule.LogInfoFormat("- %s: %d", pSupportsExtension[i].name,
				pSupportsExtension[i].version);
		}
	}
	
	// report support extensions
	baseModule.LogInfo("Not upported Extensions:");
	for(i=0; i<ExtensionCount; i++){
		if(!pSupportsExtension[i].version){
			baseModule.LogInfoFormat("- %s", pSupportsExtension[i].name);
		}
	}
}

void devkInstance::pDetectLayers(){
	uint32_t count = 0;
	VK_CHECK(pVulkan, pvkEnumerateInstanceLayerProperties(&count, VK_NULL_HANDLE));
	if(count == 0){
		return;
	}
	
	decTList<VkLayerProperties> layers((int)count, VkLayerProperties{});
	VK_CHECK(pVulkan, pvkEnumerateInstanceLayerProperties(&count, layers.GetArrayPointer()));
	
	// report all layers for debug purpose
	deBaseModule &baseModule = pVulkan.GetModule();
	
	baseModule.LogInfo("Layers:");
	layers.Visit([&](const VkLayerProperties &layer){
		baseModule.LogInfoFormat("- %s: %d (%d.%d.%d.%d)",
			layer.layerName, layer.implementationVersion,
			VK_API_VERSION_MAJOR(layer.specVersion),
			VK_API_VERSION_MINOR(layer.specVersion),
			VK_API_VERSION_PATCH(layer.specVersion),
			VK_API_VERSION_VARIANT(layer.specVersion));
	});
	
	// store supported layers
	int i, j;
	for(i=0; i<(int)count; i++){
		for(j=0; j<LayerCount; j++){
			if(strcmp(pSupportsLayer[j].name, layers[i].layerName) == 0){
				pSupportsLayer[j].version = layers[i].specVersion;
				pSupportsLayer[j].implementationVersion = layers[i].implementationVersion;
				break;
			}
		}
	}
	
	// report support layers
	baseModule.LogInfo("Supported Layers:");
	for(i=0; i<LayerCount; i++){
		if(pSupportsLayer[i].implementationVersion){
			baseModule.LogInfoFormat("- %s: %d (%d.%d.%d.%d)",
				pSupportsLayer[i].name, pSupportsLayer[i].implementationVersion,
				VK_API_VERSION_MAJOR(pSupportsLayer[i].implementationVersion),
				VK_API_VERSION_MINOR(pSupportsLayer[i].implementationVersion),
				VK_API_VERSION_PATCH(pSupportsLayer[i].implementationVersion),
				VK_API_VERSION_VARIANT(pSupportsLayer[i].implementationVersion));
		}
	}
	
	baseModule.LogInfo("Not Supported Layers:");
	for(i=0; i<LayerCount; i++){
		if(!pSupportsLayer[i].implementationVersion){
			baseModule.LogInfoFormat("- %s", pSupportsLayer[i].name);
		}
	}
}

void devkInstance::pCreateInstance(bool enableValidationLayers){
	pVulkan.GetModule().LogInfo("Create Vulkan Instance");
	
	// set up application information
	VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
	appInfo.pApplicationName = "Drag[en]gine";
	appInfo.pEngineName = "Drag[en]gine";
	appInfo.apiVersion = VK_API_VERSION_1_0;
	
	// create instance
	VkInstanceCreateInfo instanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
	instanceCreateInfo.pApplicationInfo = &appInfo;
	
	// detect extensions and layers
	pDetectExtensions();
	pDetectLayers();
	
	// enable layers
	const char *layers[LayerCount];
	uint32_t layerCount = 0;
	
	if(enableValidationLayers && SupportsExtension(extEXTDebugReport)){
		if(SupportsLayer(layerLunargStandardValidation)){
			layers[layerCount++] = pSupportsLayer[layerLunargStandardValidation].name;
			
		}else if(SupportsLayer(layerKhronosValidation)){
			layers[layerCount++] = pSupportsLayer[layerKhronosValidation].name;
		}
	}
	
	// enable extensions
	const char *extensions[ExtensionCount];
	uint32_t i, extensionCount = 0;
	
	for(i=0; i<ExtensionCount; i++){
		if(pSupportsExtension[i].version){
			extensions[extensionCount++] = pSupportsExtension[i].name;
		}
	}
	
	pVulkan.GetModule().LogInfo("Enable Layers:");
	for(i=0; i<layerCount; i++){
		pVulkan.GetModule().LogInfoFormat("- %s", layers[i]);
	}
	
	if(layerCount > 0){
		instanceCreateInfo.ppEnabledLayerNames = layers;
		instanceCreateInfo.enabledLayerCount = layerCount;
	}
	if(extensionCount > 0){
		instanceCreateInfo.ppEnabledExtensionNames = extensions;
		instanceCreateInfo.enabledExtensionCount = extensionCount;
	}
	
	// create device
	VK_CHECK(pVulkan, pvkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &pInstance));
}

void devkInstance::pLoadFunctions(){
	#define INSTANCE_LEVEL_VULKAN_FUNCTION(name) \
		name = (PFN_##name)pvkGetInstanceProcAddr(pInstance, #name); \
		if(!name){ \
			DETHROW_INFO(deeInvalidAction, "Instance function " #name " not found"); \
		}
	
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) \
		if(pSupportsExtension[extension].version != 0){ \
			INSTANCE_LEVEL_VULKAN_FUNCTION(name) \
		}
	
	#include "devkFunctionNames.h"
}

void devkInstance::pFindDevices(){
	uint32_t deviceCount = 0;
	VK_CHECK(pVulkan, vkEnumeratePhysicalDevices(pInstance, &deviceCount, VK_NULL_HANDLE));
	
	if(deviceCount > 0){
		pPhysicalDevices.SetAll(deviceCount, {});
		VK_CHECK(pVulkan, vkEnumeratePhysicalDevices(pInstance,
			&deviceCount, pPhysicalDevices.GetArrayPointer()));
	}
	
	pVulkan.GetModule().LogInfo("Vulkan Devices:");
	
	pPhysicalDevices.Visit([&](VkPhysicalDevice &device){
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device, &properties);
		pVulkan.GetModule().LogInfoFormat("- %s (id=%d vendor=%d version=%d.%d.%d.%d api=%d.%d.%d.%d)",
			properties.deviceName, properties.deviceID, properties.vendorID,
			VK_API_VERSION_MAJOR(properties.driverVersion),
			VK_API_VERSION_MINOR(properties.driverVersion),
			VK_API_VERSION_PATCH(properties.driverVersion),
			VK_API_VERSION_VARIANT(properties.driverVersion),
			VK_API_VERSION_MAJOR(properties.apiVersion),
			VK_API_VERSION_MINOR(properties.apiVersion),
			VK_API_VERSION_PATCH(properties.apiVersion),
			VK_API_VERSION_VARIANT(properties.apiVersion));
	});
}
