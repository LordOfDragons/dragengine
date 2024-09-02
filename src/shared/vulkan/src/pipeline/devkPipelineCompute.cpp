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
	
	const devkSpecialization * const specialization = configuration.GetSpecialization();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	VkComputePipelineCreateInfo pipelineInfo;
	memset( &pipelineInfo, 0, sizeof( pipelineInfo ) );
	pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineInfo.layout = pLayout;
	pipelineInfo.flags = 0;
	
	VkSpecializationInfo specializationInfo;
	memset( &specializationInfo, 0, sizeof( specializationInfo ) );
	if( specialization ){
		specializationInfo.pData = specialization->GetData();
		specializationInfo.dataSize = specialization->GetDataSize();
		specializationInfo.mapEntryCount = specialization->GetEntryCount();
		specializationInfo.pMapEntries = specialization->GetEntries();
	}
	
	VkSpecializationInfo * const useSpecialization = specialization ? &specializationInfo : nullptr;
	
	pInitShaderStage( pipelineInfo.stage, VK_SHADER_STAGE_COMPUTE_BIT,
		*configuration.GetShaderCompute(), useSpecialization );
	
	VK_CHECK( vulkan, pDevice.vkCreateComputePipelines( device.GetDevice(),
		pCache, 1, &pipelineInfo, VK_NULL_HANDLE, &pPipeline ) );
	
	pSaveCache = true;
}

devkPipelineCompute::~devkPipelineCompute(){
}



// Management
///////////////
