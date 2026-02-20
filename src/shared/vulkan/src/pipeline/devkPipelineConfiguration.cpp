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

#include "devkPipelineConfiguration.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../descriptor/devkDescriptorSetLayoutConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkPipelineConfiguration
////////////////////////////////////

devkPipelineConfiguration::devkPipelineConfiguration() :
pType(etGraphics),
pDescriptorSetLayout(nullptr),
pShaderVertex(nullptr),
pShaderTessellationControl(nullptr),
pShaderTessellationEvaluation(nullptr),
pShaderGeometry(nullptr),
pShaderFragment(nullptr),
pShaderCompute(nullptr),
pShaderRayGen(nullptr),
pShaderAnyHit(nullptr),
pShaderClosestHit(nullptr),
pShaderMiss(nullptr),
pShaderIntersection(nullptr),
pShaderCallable(nullptr),
pShaderTask(nullptr),
pShaderMesh(nullptr),
pTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN),
pCulling(VK_CULL_MODE_NONE),
pBlendColor(ebDisable),
pBlendAlpha(ebDisable),
pDepthWriteMask(false),
pDepthFunction(VK_COMPARE_OP_LESS_OR_EQUAL),
pDepthTest(false),
pStencilTest(false),
pDynamicDepthBias(false),
pDynamicStencil(false)
{
	pColorWriteMask[0] = true;
	pColorWriteMask[1] = true;
	pColorWriteMask[2] = true;
	pColorWriteMask[3] = true;
}

devkPipelineConfiguration::devkPipelineConfiguration(const devkPipelineConfiguration &configuration) :
pType(etGraphics),
pDescriptorSetLayout(nullptr),
pShaderVertex(nullptr),
pShaderTessellationControl(nullptr),
pShaderTessellationEvaluation(nullptr),
pShaderGeometry(nullptr),
pShaderFragment(nullptr),
pShaderCompute(nullptr),
pShaderRayGen(nullptr),
pShaderAnyHit(nullptr),
pShaderClosestHit(nullptr),
pShaderMiss(nullptr),
pShaderIntersection(nullptr),
pShaderCallable(nullptr),
pShaderTask(nullptr),
pShaderMesh(nullptr),
pTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN),
pCulling(VK_CULL_MODE_NONE),
pBlendColor(ebDisable),
pBlendAlpha(ebDisable),
pDepthWriteMask(false),
pDepthFunction(VK_COMPARE_OP_LESS_OR_EQUAL),
pDepthTest(false),
pStencilTest(false),
pDynamicDepthBias(false),
pDynamicStencil(false)
{
	pColorWriteMask[0] = true;
	pColorWriteMask[1] = true;
	pColorWriteMask[2] = true;
	pColorWriteMask[3] = true;
	*this = configuration;
}

devkPipelineConfiguration::~devkPipelineConfiguration(){
}



// Management
///////////////

void devkPipelineConfiguration::SetType(eType type){
	pType = type;
}

void devkPipelineConfiguration::SetDescriptorSetLayout(devkDescriptorSetLayout *layout){
	pDescriptorSetLayout = layout;
}

void devkPipelineConfiguration::SetRenderPass(devkRenderPass *renderPass){
	pRenderPass = renderPass;
}



void devkPipelineConfiguration::SetShaderVertex(devkShaderModule *shader){
	pShaderVertex = shader;
}

void devkPipelineConfiguration::SetShaderTessellationControl(devkShaderModule *shader){
	pShaderTessellationControl = shader;
}

void devkPipelineConfiguration::SetShaderTessellationEvaluation(devkShaderModule *shader){
	pShaderTessellationEvaluation = shader;
}

void devkPipelineConfiguration::SetShaderGeometry(devkShaderModule *shader){
	pShaderGeometry = shader;
}

void devkPipelineConfiguration::SetShaderFragment(devkShaderModule *shader){
	pShaderFragment = shader;
}



void devkPipelineConfiguration::SetShaderCompute(devkShaderModule *shader){
	pShaderCompute = shader;
}



void devkPipelineConfiguration::SetShaderRayGen(devkShaderModule *shader){
	pShaderRayGen = shader;
}

void devkPipelineConfiguration::SetShaderAnyHit(devkShaderModule *shader){
	pShaderAnyHit = shader;
}

void devkPipelineConfiguration::SetShaderClosestHit(devkShaderModule *shader){
	pShaderClosestHit = shader;
}

void devkPipelineConfiguration::SetShaderMiss(devkShaderModule *shader){
	pShaderMiss = shader;
}

void devkPipelineConfiguration::SetShaderIntersection(devkShaderModule *shader){
	pShaderIntersection = shader;
}

void devkPipelineConfiguration::SetShaderCallable(devkShaderModule *shader){
	pShaderCallable = shader;
}


void devkPipelineConfiguration::SetShaderTask(devkShaderModule *shader){
	pShaderTask = shader;
}

void devkPipelineConfiguration::SetShaderMesh(devkShaderModule *shader){
	pShaderMesh = shader;
}



void devkPipelineConfiguration::SetSpecialization(devkSpecialization *specialization){
	pSpecialization = specialization;
}



void devkPipelineConfiguration::SetTopology(VkPrimitiveTopology topology){
	pTopology = topology;
}

void devkPipelineConfiguration::SetCulling(VkCullModeFlagBits culling){
	pCulling = culling;
}

void devkPipelineConfiguration::SetBlendColor(eBlending blending){
	pBlendColor = blending;
}

void devkPipelineConfiguration::SetBlendAlpha(eBlending blending){
	pBlendAlpha = blending;
}

void devkPipelineConfiguration::SetBlending(eBlending blending){
	pBlendColor = blending;
	pBlendAlpha = blending;
}

bool devkPipelineConfiguration::GetColorWriteMaskAt(int component) const{
	if(component < 0){
		DETHROW_INFO(deeInvalidParam, "component < 0");
	}
	if(component > 3){
		DETHROW_INFO(deeInvalidParam, "component > 3");
	}
	return pColorWriteMask[component];
}

void devkPipelineConfiguration::SetColorWriteMaskAt(int component, bool enable){
	if(component < 0){
		DETHROW_INFO(deeInvalidParam, "component < 0");
	}
	if(component > 3){
		DETHROW_INFO(deeInvalidParam, "component > 3");
	}
	pColorWriteMask[component] = enable;
}

void devkPipelineConfiguration::SetColorWriteMask(bool red, bool green, bool blue, bool alpha){
	pColorWriteMask[0] = red;
	pColorWriteMask[1] = green;
	pColorWriteMask[2] = blue;
	pColorWriteMask[3] = alpha;
}

void devkPipelineConfiguration::SetDepthWriteMask(bool enable){
	pDepthWriteMask = enable;
}

void devkPipelineConfiguration::SetDepthFunction(VkCompareOp function){
	pDepthFunction = function;
}

void devkPipelineConfiguration::SetDepthTest(bool enable){
	pDepthTest = enable;
}

void devkPipelineConfiguration::SetStencilTest(bool enable){
	pStencilTest = enable;
}



void devkPipelineConfiguration::SetBindingCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pBindings.SetAll(count, {});
}

void devkPipelineConfiguration::SetBindingAt(int index, const VkVertexInputBindingDescription &binding){
	pBindings[index] = binding;
}

void devkPipelineConfiguration::SetBindingAt(int index, int binding, int stride, VkVertexInputRate inputRate){
	VkVertexInputBindingDescription description{};
	description.binding = (uint32_t)binding;
	description.stride = (uint32_t)stride;
	description.inputRate = inputRate;
	pBindings[index] = description;
}



void devkPipelineConfiguration::SetAttributeCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pAttributes.SetAll(count, {});
}

void devkPipelineConfiguration::SetAttributeAt(int index, const VkVertexInputAttributeDescription &attribute){
	pAttributes[index] = attribute;
}

void devkPipelineConfiguration::SetAttributeAt(int index, int location, int binding,
eAttributeFormat format, int offset){
	VkVertexInputAttributeDescription description{};
	description.location = (uint32_t)location;
	description.binding = (uint32_t)binding;
	description.offset = (uint32_t)offset;
	
	switch(format){
	case eafHalfFloat1:
		description.format = VK_FORMAT_R16_SFLOAT;
		break;
		
	case eafHalfFloat2:
		description.format = VK_FORMAT_R16G16_SFLOAT;
		break;
		
	case eafHalfFloat4:
		description.format = VK_FORMAT_R16G16B16A16_SFLOAT;
		break;
		
	case eafFloat1:
		description.format = VK_FORMAT_R32_SFLOAT;
		break;
		
	case eafFloat2:
		description.format = VK_FORMAT_R32G32_SFLOAT;
		break;
		
	case eafFloat3:
		description.format = VK_FORMAT_R32G32B32_SFLOAT;
		break;
		
	case eafFloat4:
		description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		break;
		
	case eafUFloatB10G11R11:
		description.format = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
		break;
		
	case eafIntNormA8B8G8R8:
		description.format = VK_FORMAT_A8B8G8R8_SNORM_PACK32;
		break;
		
	case eafUIntNormA8B8G8R8:
		description.format = VK_FORMAT_A8B8G8R8_UNORM_PACK32;
		break;
		
	case eafIntA8B8G8R8:
		description.format = VK_FORMAT_A8B8G8R8_SINT_PACK32;
		break;
		
	case eafUIntA8B8G8R8:
		description.format = VK_FORMAT_A8B8G8R8_UINT_PACK32;
		break;
		
	case eafIntNormA2B10G10R10:
		description.format = VK_FORMAT_A2B10G10R10_SNORM_PACK32;
		break;
		
	case eafUIntNormA2B10G10R10:
		description.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
		break;
		
	case eafIntA2B10G10R10:
		description.format = VK_FORMAT_A2B10G10R10_SINT_PACK32;
		break;
		
	case eafUIntA2B10G10R10:
		description.format = VK_FORMAT_A2B10G10R10_UINT_PACK32;
		break;
	}
	
	pAttributes[index] = description;
}



void devkPipelineConfiguration::SetDynamicDepthBias(bool dynamic){
	pDynamicDepthBias = dynamic;
}

void devkPipelineConfiguration::SetDynamicStencil(bool dynamic){
	pDynamicStencil = dynamic;
}



// Operators
//////////////

static bool operator==(const VkVertexInputBindingDescription &a, const VkVertexInputBindingDescription &b){
	return a.binding == b.binding
		&& a.stride == b.stride
		&& a.inputRate == b.inputRate;
}

static bool operator==(const VkVertexInputAttributeDescription &a, const VkVertexInputAttributeDescription &b){
	return a.location == b.location
		&& a.binding == b.binding
		&& a.format == b.format
		&& a.offset == b.offset;
}

bool devkPipelineConfiguration::operator==(const devkPipelineConfiguration &configuration) const{
	return pType == configuration.pType
		&& pDescriptorSetLayout == configuration.pDescriptorSetLayout
		&& pRenderPass == configuration.pRenderPass
		&& pShaderVertex == configuration.pShaderVertex
		&& pShaderTessellationControl == configuration.pShaderTessellationControl
		&& pShaderTessellationEvaluation == configuration.pShaderTessellationEvaluation
		&& pShaderGeometry == configuration.pShaderGeometry
		&& pShaderFragment == configuration.pShaderFragment
		&& pShaderCompute == configuration.pShaderCompute
		&& pShaderRayGen == configuration.pShaderRayGen
		&& pShaderAnyHit == configuration.pShaderAnyHit
		&& pShaderClosestHit == configuration.pShaderClosestHit
		&& pShaderMiss == configuration.pShaderMiss
		&& pShaderIntersection == configuration.pShaderIntersection
		&& pShaderCallable == configuration.pShaderCallable
		&& pShaderTask == configuration.pShaderTask
		&& pShaderMesh == configuration.pShaderMesh
		&& pSpecialization == configuration.pSpecialization
		&& pTopology == configuration.pTopology
		&& pCulling == configuration.pCulling
		&& pBlendColor == configuration.pBlendColor
		&& pBlendAlpha == configuration.pBlendAlpha
		&& pColorWriteMask[0] == configuration.pColorWriteMask[0]
		&& pColorWriteMask[1] == configuration.pColorWriteMask[1]
		&& pColorWriteMask[2] == configuration.pColorWriteMask[2]
		&& pColorWriteMask[3] == configuration.pColorWriteMask[3]
		&& pDepthWriteMask == configuration.pDepthWriteMask
		&& pDepthFunction == configuration.pDepthFunction
		&& pDepthTest == configuration.pDepthTest
		&& pStencilTest == configuration.pStencilTest
		&& pDynamicDepthBias == configuration.pDynamicDepthBias
		&& pDynamicStencil == configuration.pDynamicStencil
		&& pBindings == configuration.pBindings
		&& pAttributes == configuration.pAttributes;
}

devkPipelineConfiguration &devkPipelineConfiguration::operator=(const devkPipelineConfiguration &configuration){
	pType = configuration.pType;
	pDescriptorSetLayout = configuration.pDescriptorSetLayout;
	pRenderPass = configuration.pRenderPass;
	pShaderVertex = configuration.pShaderVertex;
	pShaderTessellationControl = configuration.pShaderTessellationControl;
	pShaderTessellationEvaluation = configuration.pShaderTessellationEvaluation;
	pShaderGeometry = configuration.pShaderGeometry;
	pShaderFragment = configuration.pShaderFragment;
	pShaderCompute = configuration.pShaderCompute;
	pShaderRayGen = configuration.pShaderRayGen;
	pShaderAnyHit = configuration.pShaderAnyHit;
	pShaderClosestHit = configuration.pShaderClosestHit;
	pShaderMiss = configuration.pShaderMiss;
	pShaderIntersection = configuration.pShaderIntersection;
	pShaderCallable = configuration.pShaderCallable;
	pShaderTask = configuration.pShaderTask;
	pShaderMesh = configuration.pShaderMesh;
	pSpecialization = configuration.pSpecialization;
	pTopology = configuration.pTopology;
	pCulling = configuration.pCulling;
	pBlendColor = configuration.pBlendColor;
	pBlendAlpha = configuration.pBlendAlpha;
	pColorWriteMask[0] = configuration.pColorWriteMask[0];
	pColorWriteMask[1] = configuration.pColorWriteMask[1];
	pColorWriteMask[2] = configuration.pColorWriteMask[2];
	pColorWriteMask[3] = configuration.pColorWriteMask[3];
	pDepthWriteMask = configuration.pDepthWriteMask;
	pDepthFunction = configuration.pDepthFunction;
	pDepthTest = configuration.pDepthTest;
	pStencilTest = configuration.pStencilTest;
	pDynamicDepthBias = configuration.pDynamicDepthBias;
	pDynamicStencil = configuration.pDynamicStencil;
	pBindings = configuration.pBindings;
	pAttributes = configuration.pAttributes;
	
	return *this;
}
