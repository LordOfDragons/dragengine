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
	DEASSERT_NOTNULL( configuration.GetRenderPass() )
	DEASSERT_TRUE( configuration.GetShaderVertex() || configuration.GetShaderFragment() )
	
	const devkSpecialization * const specialization = configuration.GetSpecialization();
	VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
	
	VkGraphicsPipelineCreateInfo pipelineInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
	pipelineInfo.layout = pLayout;
	pipelineInfo.flags = 0;
	pipelineInfo.renderPass = configuration.GetRenderPass()->GetRenderPass();
	
	// specialization
	VkSpecializationInfo specializationInfo{};
	if( specialization ){
		specializationInfo.pData = specialization->GetData();
		specializationInfo.dataSize = specialization->GetDataSize();
		specializationInfo.mapEntryCount = specialization->GetEntryCount();
		specializationInfo.pMapEntries = specialization->GetEntries();
	}
	
	VkSpecializationInfo * const useSpecialization = specialization ? &specializationInfo : nullptr;
	
	// shader stages
	VkPipelineShaderStageCreateInfo stageInfo[ 5 ]{};
	
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
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
	vertexInputInfo.vertexBindingDescriptionCount = configuration.GetBindingCount();
	vertexInputInfo.pVertexBindingDescriptions = configuration.GetBindings();
	vertexInputInfo.vertexAttributeDescriptionCount = configuration.GetAttributeCount();
	vertexInputInfo.pVertexAttributeDescriptions = configuration.GetAttributes();
	
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	
	// input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
	inputAssemblyInfo.topology = configuration.GetTopology();
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	
	// rasterization
	VkPipelineRasterizationStateCreateInfo rasterizationInfo{
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
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
	VkPipelineMultisampleStateCreateInfo multisampleInfo{
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = nullptr;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;
	
	pipelineInfo.pMultisampleState = &multisampleInfo;
	
	// viewport
	VkPipelineViewportStateCreateInfo viewportInfo{
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
	viewportInfo.viewportCount = 1;
	viewportInfo.scissorCount = 1;
	
	pipelineInfo.pViewportState = &viewportInfo;
	
	// depth stencil
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo{
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
	depthStencilInfo.depthTestEnable = configuration.GetDepthTest();
	depthStencilInfo.depthWriteEnable = configuration.GetDepthWriteMask();
	depthStencilInfo.depthCompareOp = configuration.GetDepthFunction();
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = configuration.GetStencilTest();
	depthStencilInfo.minDepthBounds = 0.0f;
	depthStencilInfo.maxDepthBounds = 1.0f;
	
	pipelineInfo.pDepthStencilState = &depthStencilInfo;
	
	// blending
	VkPipelineColorBlendStateCreateInfo blendInfo{
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;
	
	VkPipelineColorBlendAttachmentState blendAttachment{};
	
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
	VkPipelineDynamicStateCreateInfo dynamicStateInfo{
		VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
	VkDynamicState dynamicStates[ 8 ];
	int countDynamicStates = 0;
	
	dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_VIEWPORT;
	dynamicStates[ countDynamicStates++ ] = VK_DYNAMIC_STATE_SCISSOR;
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
