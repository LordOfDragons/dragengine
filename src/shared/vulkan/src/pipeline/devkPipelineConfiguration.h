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

#ifndef _DEVKPIPELINECONFIGURATION_H_
#define _DEVKPIPELINECONFIGURATION_H_

#include "devkSpecialization.h"
#include "../devkBasics.h"
#include "../renderpass/devkRenderPass.h"

class devkDescriptorSetLayout;
class devkShaderModule;


/**
 * Vulkan pipeline.
 */
class devkPipelineConfiguration{
public:
	/** Type. */
	enum eType{
		etGraphics,
		etCompute,
		etRaytracing
	};
	
	/** Blending. */
	enum eBlending{
		ebDisable,
		ebBlend,
		ebAdd,
		ebMax,
		ebMin
	};
	
	/** Attribute format. */
	enum eAttributeFormat{
		eafHalfFloat1,
		eafHalfFloat2,
		eafHalfFloat4,
		eafFloat1,
		eafFloat2,
		eafFloat3,
		eafFloat4,
		eafUFloatB10G11R11,
		eafIntNormA8B8G8R8,
		eafUIntNormA8B8G8R8,
		eafIntA8B8G8R8,
		eafUIntA8B8G8R8,
		eafIntNormA2B10G10R10,
		eafUIntNormA2B10G10R10,
		eafIntA2B10G10R10,
		eafUIntA2B10G10R10
	};
	
	
	
private:
	eType pType;
	devkDescriptorSetLayout *pDescriptorSetLayout;
	devkRenderPass::Ref pRenderPass;
	
	devkShaderModule *pShaderVertex;
	devkShaderModule *pShaderTessellationControl;
	devkShaderModule *pShaderTessellationEvaluation;
	devkShaderModule *pShaderGeometry;
	devkShaderModule *pShaderFragment;
	
	devkShaderModule *pShaderCompute;
	
	devkShaderModule *pShaderRayGen;
	devkShaderModule *pShaderAnyHit;
	devkShaderModule *pShaderClosestHit;
	devkShaderModule *pShaderMiss;
	devkShaderModule *pShaderIntersection;
	devkShaderModule *pShaderCallable;
	
	devkShaderModule *pShaderTask;
	devkShaderModule *pShaderMesh;
	
	devkSpecialization::Ref pSpecialization;
	
	VkPrimitiveTopology pTopology;
	VkCullModeFlagBits pCulling;
	eBlending pBlendColor;
	eBlending pBlendAlpha;
	bool pColorWriteMask[4];
	bool pDepthWriteMask;
	VkCompareOp pDepthFunction;
	bool pDepthTest;
	bool pStencilTest;
	
	int pBindingCount;
	VkVertexInputBindingDescription *pBindings;
	
	int pAttributeCount;
	VkVertexInputAttributeDescription *pAttributes;
	
	bool pDynamicDepthBias;
	bool pDynamicStencil;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline configuration. */
	devkPipelineConfiguration();
	
	/** Create copy of pipeline configuration. */
	devkPipelineConfiguration(const devkPipelineConfiguration &configuration);
	
	/** Clean up pipeline configuration. */
	~devkPipelineConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Set type. */
	void SetType(eType type);
	
	/** Descriptor set layout. */
	inline devkDescriptorSetLayout *GetDescriptorSetLayout() const{ return pDescriptorSetLayout; }
	
	/** Set descriptor set layout. */
	void SetDescriptorSetLayout(devkDescriptorSetLayout *layout);
	
	/** Render pass. */
	inline devkRenderPass *GetRenderPass() const{ return pRenderPass; }
	
	/** Set render pass. */
	void SetRenderPass(devkRenderPass *renderPass);
	
	
	
	/** Vertex shader module or nullptr. */
	inline devkShaderModule *GetShaderVertex() const{ return pShaderVertex; }
	
	/** Set vertex shader module or nullptr. */
	void SetShaderVertex(devkShaderModule *shader);
	
	/** Tessellation control shader module or nullptr. */
	inline devkShaderModule *GetShaderTessellationControl() const{ return pShaderTessellationControl; }
	
	/** Set tessellation control shader module or nullptr. */
	void SetShaderTessellationControl(devkShaderModule *shader);
	
	/** Tessellation evaluation shader module or nullptr. */
	inline devkShaderModule *GetShaderTessellationEvaluation() const{ return pShaderTessellationEvaluation; }
	
	/** Set tessellation evaluation shader module or nullptr. */
	void SetShaderTessellationEvaluation(devkShaderModule *shader);
	
	/** Geometry shader module or nullptr. */
	inline devkShaderModule *GetShaderGeometry() const{ return pShaderGeometry; }
	
	/** Set geometry shader module or nullptr. */
	void SetShaderGeometry(devkShaderModule *shader);
	
	/** Fragment shader module or nullptr. */
	inline devkShaderModule *GetShaderFragment() const{ return pShaderFragment; }
	
	/** Set fragment shader module or nullptr. */
	void SetShaderFragment(devkShaderModule *shader);
	
	
	
	/** Compute shader module or nullptr. */
	inline devkShaderModule *GetShaderCompute() const{ return pShaderCompute; }
	
	/** Set compute shader module or nullptr. */
	void SetShaderCompute(devkShaderModule *shader);
	
	
	
	/** Ray generation shader module or nullptr. */
	inline devkShaderModule *GetShaderRayGen() const{ return pShaderRayGen; }
	
	/** Set ray generation shader module or nullptr. */
	void SetShaderRayGen(devkShaderModule *shader);
	
	/** AnyHit shader module or nullptr. */
	inline devkShaderModule *GetShaderAnyHit() const{ return pShaderAnyHit; }
	
	/** Set any hit shader module or nullptr. */
	void SetShaderAnyHit(devkShaderModule *shader);
	
	/** ClosestHit shader module or nullptr. */
	inline devkShaderModule *GetShaderClosestHit() const{ return pShaderClosestHit; }
	
	/** Set closest hit shader module or nullptr. */
	void SetShaderClosestHit(devkShaderModule *shader);
	
	/** Miss shader module or nullptr. */
	inline devkShaderModule *GetShaderMiss() const{ return pShaderMiss; }
	
	/** Set miss shader module or nullptr. */
	void SetShaderMiss(devkShaderModule *shader);
	
	/** Intersection shader module or nullptr. */
	inline devkShaderModule *GetShaderIntersection() const{ return pShaderIntersection; }
	
	/** Set intersection shader module or nullptr. */
	void SetShaderIntersection(devkShaderModule *shader);
	
	/** Callable shader module or nullptr. */
	inline devkShaderModule *GetShaderCallable() const{ return pShaderCallable; }
	
	/** Set callable shader module or nullptr. */
	void SetShaderCallable(devkShaderModule *shader);
	
	
	
	/** Task shader module or nullptr. */
	inline devkShaderModule *GetShaderTask() const{ return pShaderTask; }
	
	/** Set task shader module or nullptr. */
	void SetShaderTask(devkShaderModule *shader);
	
	/** Mesh shader module or nullptr. */
	inline devkShaderModule *GetShaderMesh() const{ return pShaderMesh; }
	
	/** Set mesh shader module or nullptr. */
	void SetShaderMesh(devkShaderModule *shader);
	
	
	
	/** Specialization. */
	inline devkSpecialization *GetSpecialization() const{ return pSpecialization; }
	
	/** Set specialization. */
	void SetSpecialization(devkSpecialization *specialization);
	
	
	
	/** Topology. */
	inline VkPrimitiveTopology GetTopology() const{ return pTopology; }
	
	/** Set topology. */
	void SetTopology(VkPrimitiveTopology topology);
	
	/** Culling. */
	inline VkCullModeFlagBits GetCulling() const{ return pCulling; }
	
	/** Set culling. */
	void SetCulling(VkCullModeFlagBits culling);
	
	
	
	/** Color blending. */
	inline eBlending GetBlendColor() const{ return pBlendColor; }
	
	/** Set color blending. */
	void SetBlendColor(eBlending blending);
	
	/** Alpha blending. */
	inline eBlending GetBlendAlpha() const{ return pBlendAlpha; }
	
	/** Set alpha blending. */
	void SetBlendAlpha(eBlending blending);
	
	/** Set color and alpha blending. */
	void SetBlending(eBlending blending);
	
	
	
	/** Color write mask. */
	bool GetColorWriteMaskAt(int component) const;
	
	/** Set color write mask. */
	void SetColorWriteMaskAt(int component, bool enable);
	
	/** Set color write mask. */
	void SetColorWriteMask(bool red, bool green, bool blue, bool alpha);
	
	/** Depth write mask. */
	inline bool GetDepthWriteMask() const{ return pDepthWriteMask; }
	
	/** Set depth write mask. */
	void SetDepthWriteMask(bool enable);
	
	/** Depth function. */
	inline VkCompareOp GetDepthFunction() const{ return pDepthFunction; }
	
	/** Set depth function. */
	void SetDepthFunction(VkCompareOp function);
	
	/** Depth test. */
	inline bool GetDepthTest() const{ return pDepthTest; }
	
	/** Set depth test. */
	void SetDepthTest(bool enable);
	
	/** Stencil test. */
	inline bool GetStencilTest() const{ return pStencilTest; }
	
	/** Set stencil test. */
	void SetStencilTest(bool enable);
	
	
	
	/** Binding count. */
	inline int GetBindingCount() const{ return pBindingCount; }
	
	/** Set binding count. */
	void SetBindingCount(int count);
	
	/** Binding at index. */
	const VkVertexInputBindingDescription &GetBindingAt(int index) const;
	
	/** Set binding at index. */
	void SetBindingAt(int index, const VkVertexInputBindingDescription &binding);
	
	/** Set binding at index. */
	void SetBindingAt(int index, int binding, int stride,
		VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
	
	/** Internal use only. */
	inline const VkVertexInputBindingDescription *GetBindings() const{ return pBindings; }
	
	
	
	/** Attribute count. */
	inline int GetAttributeCount() const{ return pAttributeCount; }
	
	/** Set attribute count. */
	void SetAttributeCount(int count);
	
	/** Attribute at index. */
	const VkVertexInputAttributeDescription &GetAttributeAt(int index) const;
	
	/** Set attribute at index. */
	void SetAttributeAt(int index, const VkVertexInputAttributeDescription &attribute);
	
	/** Set attribute at index. */
	void SetAttributeAt(int index, int location, int binding, eAttributeFormat format, int offset);
	
	/** Internal use only. */
	inline const VkVertexInputAttributeDescription *GetAttributes() const{ return pAttributes; }
	
	
	
	/** Dynamic depth bias. */
	inline bool GetDynamicDepthBias() const{ return pDynamicDepthBias; }
	
	/** Set dynamic depth bias. */
	void SetDynamicDepthBias(bool dynamic);
	
	/** Dynamic stencil. */
	inline bool GetDynamicStencil() const{ return pDynamicStencil; }
	
	/** Set dynamic stencil. */
	void SetDynamicStencil(bool dynamic);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==(const devkPipelineConfiguration &configuration) const;
	
	/** Copy configuration. */
	devkPipelineConfiguration &operator=(const devkPipelineConfiguration &configuration);
	/*@}*/
};

#endif

