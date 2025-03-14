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

#include <stdlib.h>
#include <string.h>

#include "devkDevice.h"
#include "devkInstance.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"
#include "devkFormat.h"
#include "devkTestFormatConfig.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Struct devkDevice::DeviceConfig
////////////////////////////////////

devkDevice::DeviceConfig::DeviceConfig() :
graphicQueueCount(0),
computeQueueCount(0),
transferQueueCount(0){
}


// Class devkDevice
///////////////////////

devkDevice::devkDevice(devkInstance &instance, VkPhysicalDevice physicalDevice,
const DeviceConfig &config) :
pInstance(instance),
pPhysicalDevice(physicalDevice),
pConfig(config),
pFamilyIndexGraphic(0),
pFamilyIndexCompute(0),
pFamilyIndexTransfer(0),
pSupportedFormats(nullptr),
pSupportedFormatCount(0),
pDevice(VK_NULL_HANDLE),
pQueues(nullptr),
pDescriptorSetLayoutManager(*this),
pShaderModuleManager(*this),
pPipelineManager(*this)
{
	pSupportsExtension[extKHRMaintenance3].name = VK_KHR_MAINTENANCE3_EXTENSION_NAME;
	pSupportsExtension[extEXTDescriptorIndexing].name = VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME;
	
	DEASSERT_NOTNULL(physicalDevice)
	DEASSERT_TRUE(config.graphicQueueCount >= 0)
	DEASSERT_TRUE(config.computeQueueCount >= 0)
	DEASSERT_TRUE(config.transferQueueCount >= 0)
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION(name) name = VK_NULL_HANDLE;
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name,extension) name = VK_NULL_HANDLE;
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	
	try{
		pCreateDevice();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkDevice::~devkDevice(){
	pCleanUp();
}



// Management
///////////////

bool devkDevice::SupportsExtension(eExtension extension) const{
	return pSupportsExtension[extension].version != 0;
}

uint32_t devkDevice::ExtensionVersion(eExtension extension) const{
	return pSupportsExtension[extension].version;
}

const devkFormat &devkDevice::GetSupportedFormatAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pSupportedFormatCount)
	return pSupportedFormats[index];
}

const devkFormat *devkDevice::GetUseTexFormat(eFormats format) const{
	return pUseTexFormats[format];
}

const devkFormat *devkDevice::GetUseFboFormat(eFormats format) const{
	return pUseFboFormats[format];
}

uint32_t devkDevice::IndexOfMemoryType(VkMemoryPropertyFlags property, uint32_t bits) const{
	uint32_t i;
	for(i=0; i<pMemoryProperties.memoryTypeCount; i++){
		if((bits & 1) == 1 && (pMemoryProperties.memoryTypes[ i ].propertyFlags & property) == property) {
			return i;
		}
		bits >>= 1;
	}
	
	DETHROW(deeInvalidParam);
}

uint32_t devkDevice::IndexOfMemoryType(VkMemoryPropertyFlags property,
const VkMemoryRequirements& requirements) const{
	return IndexOfMemoryType(property, requirements.memoryTypeBits);
}



int devkDevice::GetGraphicQueueCount() const{
	return pConfig.graphicQueueCount;
}

devkQueue &devkDevice::GetGraphicQueueAt(int index) const{
	if(index < 0 || index >= pConfig.graphicQueueCount){
		DETHROW(deeInvalidParam);
	}
	return pQueues[ index ];
}

int devkDevice::GetComputeQueueCount() const{
	return pConfig.computeQueueCount;
}

devkQueue &devkDevice::GetComputeQueueAt(int index) const{
	if(index < 0 || index >= pConfig.computeQueueCount){
		DETHROW(deeInvalidParam);
	}
	return pQueues[ pConfig.graphicQueueCount + index ];
}

int devkDevice::GetTransferQueueCount() const{
	return pConfig.transferQueueCount;
}

devkQueue &devkDevice::GetTransferQueueAt(int index) const{
	if(index < 0 || index >= pConfig.transferQueueCount){
		DETHROW(deeInvalidParam);
	}
	return pQueues[ pConfig.graphicQueueCount + pConfig.computeQueueCount + index ];
}



// Private Functions
//////////////////////

void devkDevice::pCleanUp(){
	// ensure all queues are idle before releasing them
	if(pQueues){
		const int queueCount = pConfig.graphicQueueCount + pConfig.computeQueueCount + pConfig.transferQueueCount;
		int i;
		for(i=0; i<queueCount; i++){
			pQueues[ i ]->WaitIdle();
		}
	}
	
	// now it is save to release queues
	if(pQueues){
		delete [] pQueues;
	}
	
	// ensure all resources cleaned up using destructor calls are cleaned up before
	// the device is destroyed
	pPipelineManager.Clear();
	pShaderModuleManager.Clear();
	pDescriptorSetLayoutManager.Clear();
	
	// now it is safe to destroy the device
	if(pDevice){
		vkDestroyDevice(pDevice, VK_NULL_HANDLE);
		pDevice = VK_NULL_HANDLE;
	}
	
	// cleaning up
	if(pSupportedFormats){
		delete [] pSupportedFormats;
	}
}

void devkDevice::pCreateDevice(){
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	module.LogInfo("Create Vulkan Device:");
	
	pGetProperties();
	pDetectExtensions();
	pDetectCapabilities();
	
	// find requested queue families
	const float defaultQueuePriority = 0.0f;
	
	uint32_t queueFamilyCount;
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &queueFamilyCount, VK_NULL_HANDLE);
	
	VkQueueFamilyProperties *queueFamilyProperties = nullptr;
	if(queueFamilyCount == 0){
		DETHROW_INFO(deeInvalidAction, "no queues found");
	}
	
	queueFamilyProperties = new VkQueueFamilyProperties[ queueFamilyCount ];
	memset(queueFamilyProperties, 0, sizeof(VkQueueFamilyProperties) * queueFamilyCount);
	
	uint32_t writtenQueueFamilyCount = 0;
	pInstance.vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &writtenQueueFamilyCount, queueFamilyProperties);
	DEASSERT_TRUE(writtenQueueFamilyCount <= queueFamilyCount)
	int i;
	
	VkDeviceQueueCreateInfo queueCreateInfo[ 3 ]{};
	int requiredQueueCount = 0;
	int queueGraphic = -1;
	int queueCompute = -1;
	int queueTransfer = -1;
	
	// this is messy here due to the way vulkan defines things. the queue family index is not
	// allowed to be used multiple times. but graphic queues are required to support also compute
	// and transfer queues. so we have to do some juggling here to fix this mess.
	int allowGraphicQueueCount = 0;
	int allowComputeQueueCount = 0;
	int allowTransferQueueCount = 0;
	
	if(pConfig.graphicQueueCount > 0){
		allowGraphicQueueCount = pConfig.graphicQueueCount;
		queueGraphic = requiredQueueCount++;
	}
	
	if(pConfig.computeQueueCount > 0){
		allowComputeQueueCount = pConfig.computeQueueCount;
		if(pConfig.graphicQueueCount > 0){
			allowComputeQueueCount = decMath::max(allowComputeQueueCount - pConfig.graphicQueueCount, 0);
		}
		if(allowComputeQueueCount > 0){
			queueCompute = requiredQueueCount++;
		}
	}
	
	if(pConfig.transferQueueCount > 0){
		allowTransferQueueCount = pConfig.transferQueueCount;
		if(pConfig.graphicQueueCount > 0){
			allowTransferQueueCount = decMath::max(allowTransferQueueCount - pConfig.graphicQueueCount, 0);
		}
		if(allowTransferQueueCount > 0){
			queueTransfer = requiredQueueCount++;
		}
	}
	
	module.LogInfo("Queue Families");
	for(i=0; i<(int)writtenQueueFamilyCount; i++){
		const VkQueueFlags flags = queueFamilyProperties[ i ].queueFlags;
		
		decString text("-");
		if(flags & VK_QUEUE_GRAPHICS_BIT){
			text.Append(" graphics");
		}
		if(flags & VK_QUEUE_COMPUTE_BIT){
			text.Append(" compute");
		}
		if(flags & VK_QUEUE_TRANSFER_BIT){
			text.Append(" transfer");
		}
		if(flags & VK_QUEUE_SPARSE_BINDING_BIT){
			text.Append(" sparse");
		}
		if(flags & VK_QUEUE_PROTECTED_BIT){
			text.Append(" protected");
		}
		text.AppendFormat(": %d", queueFamilyProperties[ i ].queueCount);
		module.LogInfo(text);
		
		if((flags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT
		&& pFamilyIndexGraphic == 0 && queueGraphic != -1){
			queueCreateInfo[ queueGraphic ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueGraphic ].queueFamilyIndex = i;
			queueCreateInfo[ queueGraphic ].queueCount = allowGraphicQueueCount;
			queueCreateInfo[ queueGraphic ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexGraphic = i;
		}
		
		if((flags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) == VK_QUEUE_COMPUTE_BIT
		&& pFamilyIndexCompute == 0 && queueCompute != -1){
			queueCreateInfo[ queueCompute ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueCompute ].queueFamilyIndex = i;
			queueCreateInfo[ queueCompute ].queueCount = allowComputeQueueCount;
			queueCreateInfo[ queueCompute ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexCompute = i;
		}
		
		if((flags & (VK_QUEUE_TRANSFER_BIT | VK_QUEUE_GRAPHICS_BIT)) == VK_QUEUE_TRANSFER_BIT
		&& pFamilyIndexTransfer == 0 && queueTransfer != -1){
			queueCreateInfo[ queueTransfer ].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[ queueTransfer ].queueFamilyIndex = i;
			queueCreateInfo[ queueTransfer ].queueCount = allowTransferQueueCount;
			queueCreateInfo[ queueTransfer ].pQueuePriorities = &defaultQueuePriority;
			pFamilyIndexTransfer = i;
		}
	}
	
	if(queueFamilyProperties){
		delete [] queueFamilyProperties;
	}
	
	if(queueGraphic != -1
	&& queueCreateInfo[ queueGraphic ].queueCount != (uint32_t)allowGraphicQueueCount){
		DETHROW_INFO(deeInvalidAction, "graphic queue count mismatch" );
	}
	if(queueCompute != -1
	&& queueCreateInfo[ queueCompute ].queueCount != (uint32_t)allowComputeQueueCount){
		DETHROW_INFO(deeInvalidAction, "compute queue count mismatch" );
	}
	if(queueTransfer != -1
	&& queueCreateInfo[ queueTransfer ].queueCount != (uint32_t)allowTransferQueueCount){
		DETHROW_INFO(deeInvalidAction, "transfer queue count mismatch" );
	}
	
	// create logical device
	VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
	deviceCreateInfo.queueCreateInfoCount = requiredQueueCount;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo[ 0 ];
	VK_CHECK(pInstance.GetVulkan(), pInstance.vkCreateDevice(pPhysicalDevice,
		&deviceCreateInfo, VK_NULL_HANDLE, &pDevice));
	
	// load functions. below requires them
	pLoadFunctions();
	
	// create queue instances
	if(pConfig.graphicQueueCount + pConfig.computeQueueCount + pConfig.transferQueueCount > 0){
		pQueues = new devkQueue::Ref[ pConfig.graphicQueueCount
			+ pConfig.computeQueueCount + pConfig.transferQueueCount ];
		
		int nextQueue = 0;
		
		// graphic queues
		for(i=0; i<pConfig.graphicQueueCount; i++){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(pDevice, pFamilyIndexGraphic, i, &queue);
			if(! queue){
				DETHROW_INFO(deeInvalidAction, "get graphic device queue failed");
			}
			pQueues[ nextQueue++ ].TakeOver(new devkQueue(*this, pFamilyIndexGraphic, i, queue));
		}
		
		// compute queues
		for(i=0; i<decMath::min(pConfig.graphicQueueCount, pConfig.computeQueueCount); i++){
			pQueues[ nextQueue++ ] = pQueues[ i ]; // shared
		}
		
		for(i=0; i<allowComputeQueueCount; i++){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(pDevice, pFamilyIndexCompute, i, &queue);
			if(! queue){
				DETHROW_INFO(deeInvalidAction, "get compute device queue failed");
			}
			pQueues[ nextQueue++ ].TakeOver(new devkQueue(*this, pFamilyIndexCompute, i, queue));
		}
		
		// transfer queues
		for(i=0; i<decMath::min(pConfig.graphicQueueCount, pConfig.transferQueueCount); i++){
			pQueues[ nextQueue++ ] = pQueues[ i ]; // shared
		}
		
		for(i=0; i<allowTransferQueueCount; i++){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(pDevice, pFamilyIndexTransfer, i, &queue);
			if(! queue){
				DETHROW_INFO(deeInvalidAction, "get transfer device queue failed");
			}
			pQueues[ nextQueue++ ].TakeOver(new devkQueue(*this, pFamilyIndexTransfer, i, queue));
		}
	}
}

void devkDevice::pGetProperties(){
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	pInstance.vkGetPhysicalDeviceProperties(pPhysicalDevice, &pProperties);
	pInstance.vkGetPhysicalDeviceMemoryProperties(pPhysicalDevice, &pMemoryProperties);
	
	module.LogInfoFormat("- Device Name: %s", pProperties.deviceName);
	module.LogInfoFormat("- Device ID: %d", pProperties.deviceID);
	module.LogInfoFormat("- Vendor ID: %d", pProperties.vendorID);
	module.LogInfoFormat("- Driver Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR(pProperties.driverVersion),
		VK_API_VERSION_MINOR(pProperties.driverVersion),
		VK_API_VERSION_PATCH(pProperties.driverVersion),
		VK_API_VERSION_VARIANT(pProperties.driverVersion));
	module.LogInfoFormat("- API Version: %d.%d.%d.%d",
		VK_API_VERSION_MAJOR(pProperties.apiVersion),
		VK_API_VERSION_MINOR(pProperties.apiVersion),
		VK_API_VERSION_PATCH(pProperties.apiVersion),
		VK_API_VERSION_VARIANT(pProperties.apiVersion));
}

void devkDevice::pDetectExtensions(){
	deSharedVulkan &vulkan = pInstance.GetVulkan();
	uint32_t count = 0;
	VK_CHECK(vulkan, pInstance.vkEnumerateDeviceExtensionProperties(
		pPhysicalDevice, VK_NULL_HANDLE, &count, VK_NULL_HANDLE));
	if(count == 0){
		return;
	}
	
	VkExtensionProperties * const extensions = new VkExtensionProperties[ count ];
	try{
		uint32_t writtenCount = 0;
		VK_CHECK(vulkan, pInstance.vkEnumerateDeviceExtensionProperties(
			pPhysicalDevice, VK_NULL_HANDLE, &writtenCount, extensions));
		DEASSERT_TRUE(writtenCount <= count);
		
		// report all extensions reported for debug purpose
		deBaseModule &baseModule = vulkan.GetModule();
		uint32_t i;
		
		baseModule.LogInfo("Device Extensions:");
		for(i=0; i<writtenCount; i++){
			baseModule.LogInfoFormat("- %s: %d", extensions[ i ].extensionName, extensions[ i ].specVersion);
		}
		
		// store supported extensions
		int j;
		for(i=0; i<writtenCount; i++){
			for(j=0; j<ExtensionCount; j++){
				if(strcmp(pSupportsExtension[ j ].name, extensions[ i ].extensionName) == 0){
					pSupportsExtension[ j ].version = extensions[ i ].specVersion;
					break;
				}
			}
		}
		
		// report support extensions
		baseModule.LogInfo("Supported Extensions:");
		for(i=0; i<ExtensionCount; i++){
			if(pSupportsExtension[ i ].version){
				baseModule.LogInfoFormat("- %s: %d", pSupportsExtension[ i ].name,
					pSupportsExtension[ i ].version);
			}
		}
		
		baseModule.LogInfo("Not Supported Extensions:");
		for(i=0; i<ExtensionCount; i++){
			if(! pSupportsExtension[ i ].version){
				baseModule.LogInfoFormat("- %s", pSupportsExtension[ i ].name);
			}
		}
		
		delete [] extensions;
		
	}catch(const deException &){
		delete [] extensions;
		throw;
	}
}

void devkDevice::pDetectCapabilities(){
	pSupportedFormats = new devkFormat[TEST_PROGRAM_COUNT];
	pSupportedFormatCount = 0;
	
	int i;
	for(i=0; i<TEST_PROGRAM_COUNT; i++){
		const sTestCase &tp = vTestProgram[i];
		const sTestTextureFormat &ttf = vTestTextureFormats[tp.testFormat];
		VkFormatProperties props{};
		
		pInstance.vkGetPhysicalDeviceFormatProperties(pPhysicalDevice, ttf.format, &props);
		
		const int ltf = props.linearTilingFeatures;
		const int otf = props.optimalTilingFeatures;
		// const int bf = props.bufferFeatures;
		const int imgf = ltf | otf;
		
		if(!imgf){
			continue;
		}
		
		devkFormat &f = pSupportedFormats[pSupportedFormatCount++];
		f.SetFormat(ttf.format);
		f.SetBitsPerPixel(ttf.bitsPerPixel);
		f.SetIsDepth(HAS_FLAG_DEPTH(ttf.flags));
		f.SetIsDepthFloat(HAS_FLAG_DEPTH_FLOAT(ttf.flags));
		f.SetIsStencil(HAS_FLAG_STENCIL(ttf.flags));
		f.SetIsCompressed(HAS_FLAG_COMPRESSED(ttf.flags));
		f.SetName(ttf.name);
		
		f.SetCanSample(imgf & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
		f.SetCanSampleFilter(imgf & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
		f.SetCanImageStore(imgf & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT);
		f.SetCanImageStoreAtomic(imgf & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT);
		
		if(f.GetIsDepth() || f.GetIsDepthFloat() || f.GetIsStencil()){
			f.SetCanAttach(imgf & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
			
		}else{
			f.SetCanAttach(imgf & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);
			f.SetCanAttachBlend(imgf & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT);
		}
		
		if(f.GetCanSample()){
			pUseTexFormats[tp.target] = &f;
		}
		
		if(f.GetCanAttach()){
			pUseFboFormats[tp.target] = &f;
		}
	}
	
	for(i=0; i<TEST_FALLBACK_COUNT; i++){
		if(!pUseTexFormats[vTestFallback[i].target]){
			pUseTexFormats[vTestFallback[i].target] = pUseTexFormats[vTestFallback[i].fallbackTarget];
		}
		if(!pUseFboFormats[vTestFallback[i].target]){
			pUseFboFormats[vTestFallback[i].target] = pUseFboFormats[vTestFallback[i].fallbackTarget];
		}
	}
	
	deBaseModule &module = pInstance.GetVulkan().GetModule();
	module.LogInfo("Supported Formats:");
	for(i=0; i<pSupportedFormatCount; i++){
		module.LogInfoFormat("- %s", pSupportedFormats[i].GetName().GetString());
	}
	
	module.LogInfo("Use Format Texture:");
	for(i=0; i<FormatCount; i++){
		module.LogInfoFormat("- %s: %s", vTextureFormatNames[i],
			pUseTexFormats[i] ? pUseTexFormats[i]->GetName().GetString() : "-");
	}
	
	module.LogInfo("Use Format FBO:");
	for(i=0; i<FormatCount; i++){
		module.LogInfoFormat("- %s: %s", vTextureFormatNames[i],
			pUseFboFormats[i] ? pUseFboFormats[i]->GetName().GetString() : "-");
	}
}

void devkDevice::pLoadFunctions(){
	#define DEVICE_LEVEL_VULKAN_FUNCTION(name) \
		name = (PFN_##name)vkGetDeviceProcAddr(pDevice, #name); \
		if(! name){ \
			DETHROW_INFO(deeInvalidAction, "Device function " #name " not found"); \
		}
	
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) \
		if(pInstance.SupportsExtension(extension)){ \
			DEVICE_LEVEL_VULKAN_FUNCTION(name) \
		}
	
	#include "devkFunctionNames.h"
	
	#undef DEVICE_LEVEL_VULKAN_FUNCTION
	#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
}
