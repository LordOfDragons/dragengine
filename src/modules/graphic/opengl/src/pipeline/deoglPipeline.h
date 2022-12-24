/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPIPELINE_H_
#define _DEOGLPIPELINE_H_

#include "deoglPipelineConfiguration.h"
#include "../shaders/deoglShaderProgram.h"

#include <pipeline/devkPipeline.h>

#include <dragengine/deObject.h>

class deoglRenderThread;


/**
 * Wraps pipeline usable for both opengl and vulkan.
 * 
 * OpenGL states encapsulated by Vulkan pipeline:
 * 
 * - VkPipelineShaderStageCreateInfo
 *   deoglShaderProgram
 * 
 * - VkPipelineVertexInputStateCreateInfo
 *   deoglVAO, deoglVBO. Only the layout. Actual binding is dynamic
 * 
 * - VkPipelineInputAssemblyStateCreateInfo
 *   GL_TRIANGLE_FAN and so forth
 * 
 * - VkPipelineTessellationStateCreateInfo
 *   tesseleation parameters
 * 
 * - VkPipelineViewportStateCreateInfo
 *   viewport: dynamic using VK_DYNAMIC_STATE_VIEWPORT
 *   scissor: dynamic using VK_DYNAMIC_STATE_SCISSOR
 * 
 * - VkPipelineRasterizationStateCreateInfo
 *   depth test parameters: dynamic using VK_DYNAMIC_STATE_DEPTH_BIAS
 *   -> (depthBiasConstantFactor, depthBiasClamp and depthBiasSlopeFactor)
 *   depth bias
 *   line width: dynamic using VK_DYNAMIC_STATE_LINE_WIDTH
 *   cull mode: dynamic using VK_DYNAMIC_STATE_CULL_MODE (1.3)
 *   front face: dynamic using VK_DYNAMIC_STATE_FRONT_FACE (1.3)
 *   rasterizer discard enable: dynamic using VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE (1.3)
 *   depth bias enable: dynamic using VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE (1.3)
 * 
 * - VkPipelineMultisampleStateCreateInfo
 *   multi sample state
 * 
 * - VkPipelineDepthStencilStateCreateInfo
 *   stencil operators front and back
 *   depth bounds: VK_DYNAMIC_STATE_DEPTH_BOUNDS
 *   front/back.compareMask: dynamic using VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK
 *   front/back.writeMask: dynamic using VK_DYNAMIC_STATE_STENCIL_WRITE_MASK
 *   front/back.reference: dynamic using VK_DYNAMIC_STATE_STENCIL_REFERENCE
 *   depth test enable: dynamic using VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE (1.3)
 *   depth write enable: dynamic using VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE (1.3)
 *   depth compare op: dynamic using VK_DYNAMIC_STATE_DEPTH_COMPARE_OP (1.3)
 *   depth bounds test: dynamic using VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE (1.3)
 *   stencil test enable: dynamic using VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE (1.3)
 *   front/back.failOp/passOp/depthFailOp/compareOp: dynamic using VK_DYNAMIC_STATE_STENCIL_OP (1.3)
 * 
 * - VkPipelineColorBlendStateCreateInfo
 *   logic op enable, logic op, blend state per attachment (can be dynamic),
 *   blend constants: dynamic using VK_DYNAMIC_STATE_BLEND_CONSTANTS
 *   logic op: dynamic using VK_DYNAMIC_STATE_LOGIC_OP_EXT (1.3)
 * 
 * VkPipelineDynamicStateCreateInfo allows to set some states dynamic. using them as much
 * as possible avoids creating multiple pipelines:
 * 
 * - VK_DYNAMIC_STATE_VIEWPORT: true
 * - VK_DYNAMIC_STATE_SCISSOR: true
 * - VK_DYNAMIC_STATE_DEPTH_BIAS: false
 * - VK_DYNAMIC_STATE_LINE_WIDTH: false
 * - VK_DYNAMIC_STATE_BLEND_CONSTANTS: false
 * - VK_DYNAMIC_STATE_DEPTH_BOUNDS: false
 * - VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK: true
 * - VK_DYNAMIC_STATE_STENCIL_WRITE_MASK: true
 * - VK_DYNAMIC_STATE_STENCIL_REFERENCE: true
 * - 1.3 VK_DYNAMIC_STATE_CULL_MODE
 * - 1.3 VK_DYNAMIC_STATE_FRONT_FACE
 * - 1.3 VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE
 * - 1.3 VK_DYNAMIC_STATE_DEPTH_COMPARE_OP
 * - 1.3 VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE
 * - 1.3 VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE
 * - 1.3 VK_DYNAMIC_STATE_STENCIL_OP
 * - 1.3 VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE
 * - 1.3 VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE
 * - 1.3 VK_DYNAMIC_STATE_LOGIC_OP_EXT
 */
class deoglPipeline : public deObject{
public:
	typedef deTObjectReference<deoglPipeline> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	const int pIndex;
	
	// opengl
	const deoglPipelineConfiguration *pGlConfiguration;
	
	// vulkan
	devkPipeline *pVkPipeline;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
	deoglPipeline( deoglRenderThread &renderThread, int index,
		const deoglPipelineConfiguration &configuration );
	
	deoglPipeline( deoglRenderThread &renderThread, int index,
		const devkPipelineConfiguration &configuration );
	
	/** Clean up pipeline. */
	virtual ~deoglPipeline();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** OpenGL configuration or nullptr if not Vulkan. */
	const deoglPipelineConfiguration &GetGlConfiguration() const;
	
	/** OpenGL shader. */
	const deoglShaderProgram::Ref &GetGlShader() const;
	
	
	
	/** Vulkan pipeline. */
	inline devkPipeline *GetVkPipeline() const{ return pVkPipeline; }
	
	
	
	/** Activate pipeline. */
	void Activate() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
