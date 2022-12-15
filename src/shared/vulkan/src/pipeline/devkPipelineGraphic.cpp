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

#include "devkPipelineGraphic.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../shader/devkShaderModule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkPipelineGraphic
//////////////////////////////

devkPipelineGraphic::devkPipelineGraphic( devkDevice &device, const devkPipelineConfiguration &configuration ) :
devkPipeline( device, configuration )
{
	if( ! configuration.GetRenderPass() ){
		DETHROW_INFO( deeNullPointer, "configuration.renderPass" );
	}
	if( ! configuration.GetShaderVertex() && ! configuration.GetShaderFragment() ){
		DETHROW_INFO( deeNullPointer, "configuration.shaderVertex and configuration.shaderFragment" );
	}
	
	const devkSpecialization * const specialization = configuration.GetSpecialization();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	VkGraphicsPipelineCreateInfo pipelineInfo;
	memset( &pipelineInfo, 0, sizeof( pipelineInfo ) );
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.layout = pLayout;
	pipelineInfo.flags = 0;
	pipelineInfo.renderPass = configuration.GetRenderPass()->GetRenderPass();
	
	// specialization
	VkSpecializationInfo specializationInfo;
	memset( &specializationInfo, 0, sizeof( specializationInfo ) );
	if( specialization ){
		specializationInfo.pData = specialization->GetData();
		specializationInfo.dataSize = specialization->GetDataSize();
		specializationInfo.mapEntryCount = specialization->GetEntryCount();
		specializationInfo.pMapEntries = specialization->GetEntries();
	}
	
	VkSpecializationInfo * const useSpecialization = specialization ? &specializationInfo : nullptr;
	
	// shader stages
	VkPipelineShaderStageCreateInfo stageInfo[ 5 ];
	memset( stageInfo, 0, sizeof( stageInfo ) );
	
	pipelineInfo.pStages = stageInfo;
	
	devkShaderModule *shader = configuration.GetShaderVertex();
	if( shader ){
		pInitShaderStage( stageInfo[ pipelineInfo.stageCount++ ],
			VK_SHADER_STAGE_VERTEX_BIT, *shader, useSpecialization );
	}
	
	shader = configuration.GetShaderTessellationControl();
	if( shader ){
		pInitShaderStage( stageInfo[ pipelineInfo.stageCount++ ],
			VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, *shader, useSpecialization );
	}
	
	shader = configuration.GetShaderTessellationEvaluation();
	if( shader ){
		pInitShaderStage( stageInfo[ pipelineInfo.stageCount++ ],
			VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, *shader, useSpecialization );
	}
	
	shader = configuration.GetShaderGeometry();
	if( shader ){
		pInitShaderStage( stageInfo[ pipelineInfo.stageCount++ ],
			VK_SHADER_STAGE_GEOMETRY_BIT, *shader, useSpecialization );
	}
	
	shader = configuration.GetShaderFragment();
	if( shader ){
		pInitShaderStage( stageInfo[ pipelineInfo.stageCount++ ],
			VK_SHADER_STAGE_FRAGMENT_BIT, *shader, useSpecialization );
	}
	
	// vertex input data
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	memset( &vertexInputInfo, 0, sizeof( vertexInputInfo ) );
	
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = configuration.GetBindingCount();
	vertexInputInfo.pVertexBindingDescriptions = configuration.GetBindings();
	vertexInputInfo.vertexAttributeDescriptionCount = configuration.GetAttributeCount();
	vertexInputInfo.pVertexAttributeDescriptions = configuration.GetAttributes();
	
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	
	// input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	memset( &inputAssemblyInfo, 0, sizeof( inputAssemblyInfo ) );
	
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = configuration.GetTopology();
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	
	// rasterization
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	memset( &rasterizationInfo, 0, sizeof( rasterizationInfo ) );
	
	rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationInfo.depthClampEnable = VK_FALSE;
	rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationInfo.lineWidth = 1.0f;
	
	rasterizationInfo.cullMode = configuration.GetCulling();
	rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		// dragengine uses clockwise. OpenGL uses counter clockwise. to keep the code
		// portable between vulkan and opengl use counter clockwise
	
	rasterizationInfo.depthBiasEnable = VK_FALSE;
	rasterizationInfo.depthBiasConstantFactor = 0.0f;
	rasterizationInfo.depthBiasClamp = 0.0f;
	rasterizationInfo.depthBiasSlopeFactor = 0.0f;
	
	pipelineInfo.pRasterizationState = &rasterizationInfo;
	
	// multi sampling
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	memset( &multisampleInfo, 0, sizeof( multisampleInfo ) );
	
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = nullptr;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;
	
	pipelineInfo.pMultisampleState = &multisampleInfo;
	
	// viewport
	VkPipelineViewportStateCreateInfo viewportInfo;
	memset( &viewportInfo, 0, sizeof( viewportInfo ) );
	
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.scissorCount = 1;
	
	pipelineInfo.pViewportState = &viewportInfo;
	
	// depth stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	memset( &depthStencilInfo, 0, sizeof( depthStencilInfo ) );
	
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.depthTestEnable = configuration.GetDepthTest();
	depthStencilInfo.depthWriteEnable = configuration.GetDepthWriteMask();
	depthStencilInfo.depthCompareOp = configuration.GetDepthFunction();
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = configuration.GetStencilTest();
	depthStencilInfo.minDepthBounds = 0.0f;
	depthStencilInfo.maxDepthBounds = 1.0f;
	
	pipelineInfo.pDepthStencilState = &depthStencilInfo;
	
	// blending
	VkPipelineColorBlendStateCreateInfo blendInfo;
	memset( &blendInfo, 0, sizeof( blendInfo ) );
	
	blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;
	
	VkPipelineColorBlendAttachmentState blendAttachment;
	memset( &blendAttachment, 0, sizeof( blendAttachment ) );
	
	if( configuration.GetBlendColor() != devkPipelineConfiguration::ebDisable
	|| configuration.GetBlendAlpha() != devkPipelineConfiguration::ebDisable ){
		blendAttachment.blendEnable = VK_TRUE;
		
		switch( configuration.GetBlendColor() ){
		case devkPipelineConfiguration::ebDisable:
			blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			break;
			
		case devkPipelineConfiguration::ebBlend:
			blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
			
		case devkPipelineConfiguration::ebAdd:
			blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
			
		case devkPipelineConfiguration::ebMin:
			blendAttachment.colorBlendOp = VK_BLEND_OP_MIN;
			blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
			
		case devkPipelineConfiguration::ebMax:
			blendAttachment.colorBlendOp = VK_BLEND_OP_MAX;
			blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
		}
		
		switch( configuration.GetBlendAlpha() ){
		case devkPipelineConfiguration::ebDisable:
			blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			break;
			
		case devkPipelineConfiguration::ebBlend:
			blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
			
		case devkPipelineConfiguration::ebAdd:
			blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
			
		case devkPipelineConfiguration::ebMin:
			blendAttachment.alphaBlendOp = VK_BLEND_OP_MIN;
			blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
			
		case devkPipelineConfiguration::ebMax:
			blendAttachment.alphaBlendOp = VK_BLEND_OP_MAX;
			blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			break;
		}
		
	}else{
		blendAttachment.blendEnable = VK_FALSE;
	}
	
	blendAttachment.colorWriteMask = 0;
	if( configuration.GetColorWriteMaskAt( 0 ) ){
		blendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
	}
	if( configuration.GetColorWriteMaskAt( 1 ) ){
		blendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
	}
	if( configuration.GetColorWriteMaskAt( 2 ) ){
		blendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
	}
	if( configuration.GetColorWriteMaskAt( 3 ) ){
		blendAttachment.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
	}
	
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = &blendAttachment;
	
	pipelineInfo.pColorBlendState = &blendInfo;
	
	// dynamic states
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	memset( &dynamicStateInfo, 0, sizeof( dynamicStateInfo ) );
	
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	VkDynamicState dynamicStates[ 8 ];
	int countDynamicStates = 0;
	
	if( configuration.GetDynamicViewport() ){
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_VIEWPORT;
	}
	if( configuration.GetDynamicScissor() ){
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_SCISSOR;
	}
	if( configuration.GetDynamicDepthBias() ){
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_DEPTH_BIAS;
	}
	if( configuration.GetDynamicStencil() ){
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
	}
	// VK_DYNAMIC_STATE_BLEND_CONSTANTS
	// VK_DYNAMIC_STATE_DEPTH_BOUNDS
	
	dynamicStateInfo.dynamicStateCount = countDynamicStates;
	dynamicStateInfo.pDynamicStates = dynamicStates;
	
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	
	// create pipeline
	VK_CHECK( vulkan, pDevice.vkCreateGraphicsPipelines( device.GetDevice(),
		pCache, 1, &pipelineInfo, VK_NULL_HANDLE, &pPipeline ) );
	
	pSaveCache = true;
}

devkPipelineGraphic::~devkPipelineGraphic(){
}



// Management
///////////////
