/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEVKDYNAMICSTATE_H_
#define _DEVKDYNAMICSTATE_H_

#include "../devkBasics.h"

class devkCommandBuffer;
class devkPipeline;


/**
 * Vulkan pipeline dynamic state.
 */
class devkDynamicState{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline dynamic state. */
	devkDynamicState();
	
	/** Create copy of pipeline dynamic state. */
	devkDynamicState( const devkDynamicState &state );
	
	/** Clean up pipeline dynamic state. */
	~devkDynamicState();
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/**
	 * VK_DYNAMIC_STATE_VIEWPORT specifies that the pViewports state in
	 * VkPipelineViewportStateCreateInfo will be ignored and must be set dynamically with
	 * vkCmdSetViewport before any drawing commands.
	 */
	// VkViewport viewport => done by devkCommandBuffer::BeginRenderPass
	
	/**
	 * VK_DYNAMIC_STATE_SCISSOR specifies that the pScissors state in
	 * VkPipelineViewportStateCreateInfo will be ignored and must be set dynamically with
	 * vkCmdSetScissor before any drawing commands.
	 */
	// VkRect2D scissor => done by devkCommandBuffer::BeginRenderPass
	
	/**
	 * VK_DYNAMIC_STATE_LINE_WIDTH specifies that the lineWidth state in
	 * VkPipelineRasterizationStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetLineWidth before any drawing commands that generate line primitives
	 * for the rasterizer.
	 */
	// float lineWidth;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_BIAS specifies that the depthBiasConstantFactor, depthBiasClamp
	 * and depthBiasSlopeFactor states in VkPipelineRasterizationStateCreateInfo will be ignored
	 * and must be set dynamically with vkCmdSetDepthBias before any draws are performed with
	 * depthBiasEnable in VkPipelineRasterizationStateCreateInfo set to VK_TRUE.
	 */
	float depthBiasConstantFactor;
	float depthBiasClamp;
	float depthBiasSlopeFactor;
	
	/**
	 * VK_DYNAMIC_STATE_BLEND_CONSTANTS specifies that the blendConstants state in
	 * VkPipelineColorBlendStateCreateInfo will be ignored and must be set dynamically with
	 * vkCmdSetBlendConstants before any draws are performed with a pipeline state with
	 * VkPipelineColorBlendAttachmentState member blendEnable set to VK_TRUE and any of the
	 * blend functions using a constant blend color.
	 */
	// float blendConstants[ 4 ];
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_BOUNDS specifies that the minDepthBounds and maxDepthBounds states
	 * of VkPipelineDepthStencilStateCreateInfo will be ignored and must be set dynamically with
	 * vkCmdSetDepthBounds before any draws are performed with a pipeline state with
	 * VkPipelineDepthStencilStateCreateInfo member depthBoundsTestEnable set to VK_TRUE.
	 */
	// float depthBoundsMin;
	// float depthBoundsMax;
	
	/**
	 * VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK specifies that the compareMask state in
	 * VkPipelineDepthStencilStateCreateInfo for both front and back will be ignored
	 * and must be set dynamically with vkCmdSetStencilCompareMask before any draws
	 * are performed with a pipeline state with VkPipelineDepthStencilStateCreateInfo
	 * member stencilTestEnable set to VK_TRUE.
	 */
	uint32_t stencilFrontCompareMask;
	uint32_t stencilBackCompareMask;
	
	/**
	 * VK_DYNAMIC_STATE_STENCIL_WRITE_MASK specifies that the writeMask state in
	 * VkPipelineDepthStencilStateCreateInfo for both front and back will be ignored
	 * and must be set dynamically with vkCmdSetStencilWriteMask before any draws
	 * are performed with a pipeline state with VkPipelineDepthStencilStateCreateInfo
	 * member stencilTestEnable set to VK_TRUE.
	 */
	uint32_t stencilFrontWriteMask;
	uint32_t stencilBackWriteMask;
	
	/**
	 * VK_DYNAMIC_STATE_STENCIL_REFERENCE specifies that the reference state in
	 * VkPipelineDepthStencilStateCreateInfo for both front and back will be ignored
	 * and must be set dynamically with vkCmdSetStencilReference before any draws
	 * are performed with a pipeline state with VkPipelineDepthStencilStateCreateInfo
	 * member stencilTestEnable set to VK_TRUE.
	 */
	uint32_t stencilFrontReference;
	uint32_t stencilBackReference;
	
	/**
	 * VK_DYNAMIC_STATE_CULL_MODE specifies that the cullMode state in
	 * VkPipelineRasterizationStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetCullMode before any drawing commands.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkCullModeFlags cullMode;
	
	/**
	 * VK_DYNAMIC_STATE_FRONT_FACE specifies that the frontFace state in
	 * VkPipelineRasterizationStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetFrontFace before any drawing commands.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkFrontFace frontFace;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE specifies that the depthTestEnable state in
	 * VkPipelineDepthStencilStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetDepthTestEnable before any draw call.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkBool32 depthTestEnable;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE specifies that the depthWriteEnable state in
	 * VkPipelineDepthStencilStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetDepthWriteEnable before any draw call.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkBool32 depthWriteEnable;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_COMPARE_OP specifies that the depthCompareOp state in
	 * VkPipelineDepthStencilStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetDepthCompareOp before any draw call.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkCompareOp depthCompareOp;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE specifies that the depthBoundsTestEnable
	 * state in VkPipelineDepthStencilStateCreateInfo will be ignored and must be set
	 * dynamically with vkCmdSetDepthBoundsTestEnable before any draw call.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkBool32 depthBoundsTestEnable;
	
	/**
	 * VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE specifies that the stencilTestEnable state in
	 * VkPipelineDepthStencilStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetStencilTestEnable before any draw call.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkBool32 stencilTestEnable;
	
	/**
	 * VK_DYNAMIC_STATE_STENCIL_OP specifies that the failOp, passOp, depthFailOp, and
	 * compareOp states in VkPipelineDepthStencilStateCreateInfo for both front and back
	 * will be ignored and must be set dynamically with vkCmdSetStencilOp before any draws
	 * are performed with a pipeline state with VkPipelineDepthStencilStateCreateInfo
	 * member stencilTestEnable set to VK_TRUE.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state
	 */
	// VkStencilOp stencilFrontFailOp;
	// VkStencilOp stencilFrontPassOp;
	// VkStencilOp stencilFrontDepthFailOp;
	// VkCompareOp stencilFrontCompareOp;
	// VkStencilOp stencilBackFailOp;
	// VkStencilOp stencilBackPassOp;
	// VkStencilOp stencilBackDepthFailOp;
	// VkCompareOp stencilBackCompareOp;
	
	/**
	 * VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE specifies that the rasterizerDiscardEnable
	 * state in VkPipelineRasterizationStateCreateInfo will be ignored and must be set
	 * dynamically with vkCmdSetRasterizerDiscardEnable before any drawing commands.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state2
	 */
	// VkBool32 rasterizerDiscardEnable;
	
	/**
	 * VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE specifies that the depthBiasEnable state in
	 * VkPipelineRasterizationStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetDepthBiasEnable before any drawing commands.
	 * 
	 * Version 1.3 or VK_EXT_extended_dynamic_state2
	 */
	// VkBool32 depthBiasEnable;
	
	/**
	 * VK_DYNAMIC_STATE_LOGIC_OP_EXT specifies that the logicOp state in
	 * VkPipelineColorBlendStateCreateInfo will be ignored and must be set dynamically
	 * with vkCmdSetLogicOpEXT before any drawing commands.
	 * 
	 * VK_EXT_extended_dynamic_state2
	 */
	// VkLogicOp logicOp;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Dynamic states are equal. */
	bool operator==( const devkDynamicState &state ) const;
	
	/** Copy dynamic state. */
	devkDynamicState &operator=( const devkDynamicState &state );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Apply dynamic state. */
	void Apply( devkCommandBuffer &commandBuffer, const devkPipeline &pipeline ) const;
	/*@}*/
};

#endif

