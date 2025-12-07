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

#ifndef _DEOGLPIPELINE_H_
#define _DEOGLPIPELINE_H_

#ifdef BACKEND_OPENGL
	#include "deoglPipelineConfiguration.h"
#elif defined BACKEND_VULKAN
	#include <pipeline/devkPipeline.h>
#endif

#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglShaderCompiled;


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
	int pRTSIndex;
	
#ifdef BACKEND_OPENGL
	const deoglPipelineConfiguration *pConfiguration;
#elif defined BACKEND_VULKAN
	devkPipeline *pPipeline;
#endif
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
#ifdef BACKEND_OPENGL
	deoglPipeline(deoglRenderThread &renderThread, const deoglPipelineConfiguration &configuration);
#elif defined BACKEND_VULKAN
	deoglPipeline(deoglRenderThread &renderThread, const devkPipelineConfiguration &configuration);
#endif
	
	/** Clean up pipeline. */
	~deoglPipeline() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
#ifdef BACKEND_OPENGL
	/** Configuration. */
	inline const deoglPipelineConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** Shader program. */
	deoglShaderCompiled &GetShader() const;
	
#elif defined BACKEND_VULKAN
	/** Pipeline. */
	inline devkPipeline *GetVkPipeline() const{ return pVkPipeline; }
#endif
	
	
	
	/** Render task shared pipeline index or -1. */
	inline int GetRTSIndex() const{ return pRTSIndex; }
	
	/** Set render task shader pipeline. */
	void SetRTSIndex(int index);
	
	
	
	/** Activate pipeline. */
	void Activate() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
