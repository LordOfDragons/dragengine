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

#include "devkPipelineCompute.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../shader/devkShaderModule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkPipelineCompute
//////////////////////////////

devkPipelineCompute::devkPipelineCompute( devkDevice &device, const devkPipelineConfiguration &configuration ) :
devkPipeline( device, configuration )
{
	if( ! configuration.GetShaderCompute() ){
		DETHROW_INFO( deeNullPointer, "configuration.shaderCompute" );
	}
	
	const int specializationCount = configuration.GetSpecializationCount();
	VkSpecializationMapEntry *specializationEntries = nullptr;
	int i;
	
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
		
		VkComputePipelineCreateInfo pipelineInfo;
		memset( &pipelineInfo, 0, sizeof( pipelineInfo ) );
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.layout = pLayout;
		pipelineInfo.flags = 0;
		
		if( specializationCount > 0 ){
			specializationEntries = new VkSpecializationMapEntry[ specializationCount ];
			for( i=0; i<specializationCount; i++ ){
				specializationEntries->constantID = i;
				specializationEntries->offset = sizeof( devkPipelineConfiguration::Specialization ) * i;
				specializationEntries->size = sizeof( devkPipelineConfiguration::Specialization );
			}
		}
		
		VkSpecializationInfo specializationInfo;
		memset( &specializationInfo, 0, sizeof( specializationInfo ) );
		specializationInfo.pData = configuration.GetSpecializations();
		specializationInfo.dataSize = sizeof( devkPipelineConfiguration::Specialization ) * specializationCount;
		specializationInfo.mapEntryCount = specializationCount;
		specializationInfo.pMapEntries = specializationEntries;
		
		pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		pipelineInfo.stage.module = configuration.GetShaderCompute()->GetModule();
		pipelineInfo.stage.pName = "main";
		pipelineInfo.stage.pSpecializationInfo = &specializationInfo;
		
		VK_CHECK( vulkan, pDevice.vkCreateComputePipelines( device.GetDevice(),
			pCache, 1, &pipelineInfo, nullptr, &pPipeline ) );
		
		if( specializationEntries ){
			delete [] specializationEntries;
		}
		
		pSaveCache = true;
		
	}catch( const deException & ){
		if( specializationEntries ){
			delete [] specializationEntries;
		}
		throw;
	}
}

devkPipelineCompute::~devkPipelineCompute(){
}



// Management
///////////////
