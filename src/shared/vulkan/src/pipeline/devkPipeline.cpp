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

#include "devkPipeline.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../descriptor/devkDescriptorSetLayout.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkPipeline
///////////////////////

devkPipeline::devkPipeline( devkDevice &device, const devkPipelineConfiguration &configuration ) :
pDevice( device ),
pConfiguration( configuration ),
pLayout( nullptr ),
pPipeline( nullptr ),
pCache( nullptr ),
pSaveCache( false )
{
	try{
		deSharedVulkan &vulkan = device.GetInstance().GetVulkan();
		
		const VkDescriptorSetLayout layouts[ 1 ] = {
			configuration.GetDescriptorSetLayout()->GetLayout()
		};
		
		VkPipelineLayoutCreateInfo layoutInfo;
		memset( &layoutInfo, 0, sizeof( layoutInfo ) );
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = layouts;
		
		VK_CHECK( vulkan, device.vkCreatePipelineLayout(
			device.GetDevice(), &layoutInfo, nullptr, &pLayout ) );
		
		VkPipelineCacheCreateInfo cacheInfo;
		memset( &cacheInfo, 0, sizeof( cacheInfo ) );
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		
		// load cache if present
		deBaseModule &baseModule = vulkan.GetModule();
		deVirtualFileSystem &vfs = baseModule.GetVFS();
		const decPath path( pCachePath() );
		
		try{
			if( vfs.ExistsFile( path ) ){
				baseModule.LogInfoFormat( "Vulkan Pipeline: Read Cache for device %x", pDevice.GetProperties().deviceID );
				
				decBaseFileReaderReference reader;
				reader.TakeOver( vfs.OpenFileForReading( path ) );
				
				cacheInfo.initialDataSize = reader->GetLength();
				if( cacheInfo.initialDataSize > 0 ){
					cacheInfo.pInitialData = new char[ cacheInfo.initialDataSize ];
					reader->Read( ( char* )cacheInfo.pInitialData, cacheInfo.initialDataSize );
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
				device.GetDevice(), &cacheInfo, nullptr, &pCache ) );
			
		}catch( const deException &e ){
			if( cacheInfo.pInitialData ){
				baseModule.LogException( e );
				
				pDropCache();
				
				cacheInfo.pInitialData = nullptr;
				cacheInfo.initialDataSize = 0;
				
				VK_CHECK( vulkan, pDevice.vkCreatePipelineCache(
					device.GetDevice(), &cacheInfo, nullptr, &pCache ) );
				
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



// Private Functions
//////////////////////

void devkPipeline::pCleanUp(){
	VkDevice device = pDevice.GetDevice();
	
	if( pPipeline ){
		pDevice.vkDestroyPipeline( device, pPipeline, nullptr );
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
				decBaseFileWriterReference writer;
				writer.TakeOver( vfs.OpenFileForWriting( path ) );
				
				size_t sizeData = 0;
				VK_CHECK( vulkan, pDevice.vkGetPipelineCacheData( device, pCache, &sizeData, nullptr ) );
				
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
		
		pDevice.vkDestroyPipelineCache( device, pCache, nullptr );
	}
	if( pLayout ){
		pDevice.vkDestroyPipelineLayout( device, pLayout, nullptr );
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
