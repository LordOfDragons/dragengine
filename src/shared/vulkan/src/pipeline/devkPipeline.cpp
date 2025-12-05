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

#include "devkPipeline.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../shader/devkShaderModule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkPipeline
///////////////////////

devkPipeline::devkPipeline( devkDevice &device, const devkPipelineConfiguration &configuration ) :
pDevice( device ),
pConfiguration( configuration ),
pBindPoint( VK_PIPELINE_BIND_POINT_GRAPHICS ),
pLayout( VK_NULL_HANDLE ),
pPipeline( VK_NULL_HANDLE ),
pCache( VK_NULL_HANDLE ),
pSaveCache( false )
{
	try{
		switch( configuration.GetType() ){
		case devkPipelineConfiguration::etGraphics:
			pBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			break;
			
		case devkPipelineConfiguration::etCompute:
			pBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
			break;
			
		case devkPipelineConfiguration::etRaytracing:
			pBindPoint = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
			break;
		}
		
		deSharedVulkan &vulkan = device.GetInstance().GetVulkan();
		
		VkDescriptorSetLayout layouts;
		
		VkPipelineLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		
		if( configuration.GetDescriptorSetLayout() ){
			layouts = configuration.GetDescriptorSetLayout()->GetLayout();
			
			layoutInfo.setLayoutCount = 1;
			layoutInfo.pSetLayouts = &layouts;
		}
		
		VK_CHECK( vulkan, device.vkCreatePipelineLayout(
			device.GetDevice(), &layoutInfo, VK_NULL_HANDLE, &pLayout ) );
		
		VkPipelineCacheCreateInfo cacheInfo{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO};
		
		// load cache if present
		deBaseModule &baseModule = vulkan.GetModule();
		deVirtualFileSystem &vfs = baseModule.GetVFS();
		const decPath path( pCachePath() );
		
		try{
			if( vfs.ExistsFile( path ) ){
				baseModule.LogInfoFormat( "Vulkan Pipeline: Read Cache for device %x", pDevice.GetProperties().deviceID );
				
				decBaseFileReader::Ref reader;
				reader.TakeOver( vfs.OpenFileForReading( path ) );
				
				cacheInfo.initialDataSize = reader->GetLength();
				if( cacheInfo.initialDataSize > 0 ){
					cacheInfo.pInitialData = new char[ cacheInfo.initialDataSize ];
					reader->Read( ( char* )cacheInfo.pInitialData, ( int )cacheInfo.initialDataSize );
				}
			}
			
		}catch( const deException &e ){
			baseModule.LogException( e );
			if( cacheInfo.pInitialData ){
				delete [] ( char* )cacheInfo.pInitialData;
			}
			pDropCache();
		}
		
		// create pipeline cache. if this failes drop the cache since it might be the
		// reason for the cache failing then try again
		try{
			VK_CHECK( vulkan, pDevice.vkCreatePipelineCache(
				device.GetDevice(), &cacheInfo, VK_NULL_HANDLE, &pCache ) );
			
		}catch( const deException &e ){
			if( cacheInfo.pInitialData ){
				baseModule.LogException( e );
				
				pDropCache();
				
				cacheInfo.pInitialData = nullptr;
				cacheInfo.initialDataSize = 0;
				
				VK_CHECK( vulkan, pDevice.vkCreatePipelineCache(
					device.GetDevice(), &cacheInfo, VK_NULL_HANDLE, &pCache ) );
				
			}else{
				throw;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkPipeline::~devkPipeline(){
	pCleanUp();
}



// Management
///////////////



// Protected Functions
////////////////////////

void devkPipeline::pInitShaderStage( VkPipelineShaderStageCreateInfo &info,
VkShaderStageFlagBits stage, devkShaderModule &module,
const VkSpecializationInfo *specialization ){
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.stage = stage;
	info.module = module.GetModule();
	info.pName = "main";
	info.pSpecializationInfo = specialization;
}



// Private Functions
//////////////////////

void devkPipeline::pCleanUp(){
	VkDevice device = pDevice.GetDevice();
	
	if( pPipeline ){
		pDevice.vkDestroyPipeline( device, pPipeline, VK_NULL_HANDLE );
	}
	
	if( pCache ){
		if( pSaveCache ){
			deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan();
			deBaseModule &baseModule = vulkan.GetModule();
			deVirtualFileSystem &vfs = baseModule.GetVFS();
			const decPath path( pCachePath() );
			char *data = nullptr;
			
			baseModule.LogInfoFormat( "Vulkan Pipeline: Save Cache for device %x", pDevice.GetProperties().deviceID );
			
			try{
				decBaseFileWriter::Ref writer;
				writer.TakeOver( vfs.OpenFileForWriting( path ) );
				
				size_t sizeData = 0;
				VK_CHECK( vulkan, pDevice.vkGetPipelineCacheData( device, pCache, &sizeData, VK_NULL_HANDLE ) );
				
				if( sizeData > 0 ){
					data = new char[ sizeData ];
					VK_CHECK( vulkan, pDevice.vkGetPipelineCacheData( device, pCache, &sizeData, data ) );
					
					writer->Write( data, ( int )sizeData );
				}
				
			}catch( const deException &e ){
				baseModule.LogException( e );
				if( data ){
					delete [] data;
				}
				pDropCache();
			}
		}
		
		pDevice.vkDestroyPipelineCache( device, pCache, VK_NULL_HANDLE );
	}
	if( pLayout ){
		pDevice.vkDestroyPipelineLayout( device, pLayout, VK_NULL_HANDLE );
	}
}

decPath devkPipeline::pCachePath() const{
	decString directory;
	directory.Format( "pipelines-%x", pDevice.GetProperties().deviceID );
	
	decPath path( pDevice.GetInstance().GetVulkan().GetCachePath() );
	path.AddComponent( directory );
	
	return path;
}

void devkPipeline::pDropCache(){
	deSharedVulkan &vulkan = pDevice.GetInstance().GetVulkan();
	deBaseModule &baseModule = vulkan.GetModule();
	deVirtualFileSystem &vfs = baseModule.GetVFS();
	const decPath path( pCachePath() );
	
	baseModule.LogInfoFormat( "Vulkan Pipeline: Drop Cache for device %x", pDevice.GetProperties().deviceID );
	try{
		if( vfs.ExistsFile( path ) ){
			vfs.DeleteFile( path );
		}
		
	}catch( const deException &e ){
		baseModule.LogException( e );
	}
}
