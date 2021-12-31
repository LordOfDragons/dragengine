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

#include "devkPipelineConfiguration.h"
#include "../descriptor/devkDescriptorSetLayout.h"
#include "../descriptor/devkDescriptorSetLayoutConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkPipelineConfiguration
////////////////////////////////////

devkPipelineConfiguration::devkPipelineConfiguration() :
pType( etGraphics ),
pDescriptorSetLayout( nullptr ),
pShaderVertex( nullptr ),
pShaderTessellationControl( nullptr ),
pShaderTessellationEvaluation( nullptr ),
pShaderGeometry( nullptr ),
pShaderFragment( nullptr ),
pShaderCompute( nullptr ),
pShaderRayGen( nullptr ),
pShaderAnyHit( nullptr ),
pShaderClosestHit( nullptr ),
pShaderMiss( nullptr ),
pShaderIntersection( nullptr ),
pShaderCallable( nullptr ),
pShaderTask( nullptr ),
pShaderMesh( nullptr ),
pTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN ),
pCulling( VK_CULL_MODE_NONE ),
pBlendColor( ebDisable ),
pBlendAlpha( ebDisable ),
pDepthWriteMask( false ),
pDepthFunction( VK_COMPARE_OP_LESS_OR_EQUAL ),
pDepthTest( false ),
pStencilTest( false ),
pBindingCount( 0 ),
pBindings( nullptr ),
pAttributeCount( 0 ),
pAttributes( nullptr )
{
	pColorWriteMask[ 0 ] = true;
	pColorWriteMask[ 1 ] = true;
	pColorWriteMask[ 2 ] = true;
	pColorWriteMask[ 3 ] = true;
}

devkPipelineConfiguration::devkPipelineConfiguration( const devkPipelineConfiguration &configuration ) :
pType( etGraphics ),
pDescriptorSetLayout( nullptr ),
pShaderVertex( nullptr ),
pShaderTessellationControl( nullptr ),
pShaderTessellationEvaluation( nullptr ),
pShaderGeometry( nullptr ),
pShaderFragment( nullptr ),
pShaderCompute( nullptr ),
pShaderRayGen( nullptr ),
pShaderAnyHit( nullptr ),
pShaderClosestHit( nullptr ),
pShaderMiss( nullptr ),
pShaderIntersection( nullptr ),
pShaderCallable( nullptr ),
pShaderTask( nullptr ),
pShaderMesh( nullptr ),
pTopology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN ),
pCulling( VK_CULL_MODE_NONE ),
pBlendColor( ebDisable ),
pBlendAlpha( ebDisable ),
pDepthWriteMask( false ),
pDepthFunction( VK_COMPARE_OP_LESS_OR_EQUAL ),
pDepthTest( false ),
pStencilTest( false ),
pBindingCount( 0 ),
pBindings( nullptr ),
pAttributeCount( 0 ),
pAttributes( nullptr )
{
	pColorWriteMask[ 0 ] = true;
	pColorWriteMask[ 1 ] = true;
	pColorWriteMask[ 2 ] = true;
	pColorWriteMask[ 3 ] = true;
	*this = configuration;
}

devkPipelineConfiguration::~devkPipelineConfiguration(){
}



// Management
///////////////

void devkPipelineConfiguration::SetType( eType type ){
	pType = type;
}

void devkPipelineConfiguration::SetDescriptorSetLayout( devkDescriptorSetLayout *layout ){
	pDescriptorSetLayout = layout;
}

void devkPipelineConfiguration::SetRenderPass( devkRenderPass *renderPass ){
	pRenderPass = renderPass;
}



void devkPipelineConfiguration::SetShaderVertex( devkShaderModule *shader ){
	pShaderVertex = shader;
}

void devkPipelineConfiguration::SetShaderTessellationControl( devkShaderModule *shader ){
	pShaderTessellationControl = shader;
}

void devkPipelineConfiguration::SetShaderTessellationEvaluation( devkShaderModule *shader ){
	pShaderTessellationEvaluation = shader;
}

void devkPipelineConfiguration::SetShaderGeometry( devkShaderModule *shader ){
	pShaderGeometry = shader;
}

void devkPipelineConfiguration::SetShaderFragment( devkShaderModule *shader ){
	pShaderFragment = shader;
}



void devkPipelineConfiguration::SetShaderCompute( devkShaderModule *shader ){
	pShaderCompute = shader;
}



void devkPipelineConfiguration::SetShaderRayGen( devkShaderModule *shader ){
	pShaderRayGen = shader;
}

void devkPipelineConfiguration::SetShaderAnyHit( devkShaderModule *shader ){
	pShaderAnyHit = shader;
}

void devkPipelineConfiguration::SetShaderClosestHit( devkShaderModule *shader ){
	pShaderClosestHit = shader;
}

void devkPipelineConfiguration::SetShaderMiss( devkShaderModule *shader ){
	pShaderMiss = shader;
}

void devkPipelineConfiguration::SetShaderIntersection( devkShaderModule *shader ){
	pShaderIntersection = shader;
}

void devkPipelineConfiguration::SetShaderCallable( devkShaderModule *shader ){
	pShaderCallable = shader;
}


void devkPipelineConfiguration::SetShaderTask( devkShaderModule *shader ){
	pShaderTask = shader;
}

void devkPipelineConfiguration::SetShaderMesh( devkShaderModule *shader ){
	pShaderMesh = shader;
}



void devkPipelineConfiguration::SetSpecialization( devkSpecialization *specialization ){
	pSpecialization = specialization;
}



void devkPipelineConfiguration::SetTopology( VkPrimitiveTopology topology ){
	pTopology = topology;
}

void devkPipelineConfiguration::SetCulling( VkCullModeFlagBits culling ){
	pCulling = culling;
}

void devkPipelineConfiguration::SetBlendColor( eBlending blending ){
	pBlendColor = blending;
}

void devkPipelineConfiguration::SetBlendAlpha( eBlending blending ){
	pBlendAlpha = blending;
}

void devkPipelineConfiguration::SetBlending( eBlending blending ){
	pBlendColor = blending;
	pBlendAlpha = blending;
}

bool devkPipelineConfiguration::GetColorWriteMaskAt( int component ) const{
	if( component < 0 ){
		DETHROW_INFO( deeInvalidParam, "component < 0" );
	}
	if( component > 3 ){
		DETHROW_INFO( deeInvalidParam, "component > 3" );
	}
	return pColorWriteMask[ component ];
}

void devkPipelineConfiguration::SetColorWriteMaskAt( int component, bool enable ){
	if( component < 0 ){
		DETHROW_INFO( deeInvalidParam, "component < 0" );
	}
	if( component > 3 ){
		DETHROW_INFO( deeInvalidParam, "component > 3" );
	}
	pColorWriteMask[ component ] = enable;
}

void devkPipelineConfiguration::SetColorWriteMask( bool red, bool green, bool blue, bool alpha ){
	pColorWriteMask[ 0 ] = red;
	pColorWriteMask[ 1 ] = green;
	pColorWriteMask[ 2 ] = blue;
	pColorWriteMask[ 3 ] = alpha;
}

void devkPipelineConfiguration::SetDepthWriteMask( bool enable ){
	pDepthWriteMask = enable;
}

void devkPipelineConfiguration::SetDepthFunction( VkCompareOp function ){
	pDepthFunction = function;
}

void devkPipelineConfiguration::SetDepthTest( bool enable ){
	pDepthTest = enable;
}

void devkPipelineConfiguration::SetStencilTest( bool enable ){
	pStencilTest = enable;
}



void devkPipelineConfiguration::SetBindingCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pBindings ){
		delete [] pBindings;
		pBindings = nullptr;
		pBindingCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pBindings = new VkVertexInputBindingDescription[ count ];
	pBindingCount = count;
}

const VkVertexInputBindingDescription &devkPipelineConfiguration::GetBindingAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= bindingCount" );
	}
	
	return pBindings[ index ];
}

void devkPipelineConfiguration::SetBindingAt( int index, const VkVertexInputBindingDescription &binding ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= bindingCount" );
	}
	
	memcpy( pBindings + index, &binding, sizeof( binding ) );
}

void devkPipelineConfiguration::SetBindingAt( int index, int binding, int stride, VkVertexInputRate inputRate ){
	VkVertexInputBindingDescription description;
	description.binding = ( uint32_t )binding;
	description.stride = ( uint32_t )stride;
	description.inputRate = inputRate;
	SetBindingAt( index, description );
}



void devkPipelineConfiguration::SetAttributeCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pAttributes ){
		delete [] pAttributes;
		pAttributes = nullptr;
		pAttributeCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pAttributes = new VkVertexInputAttributeDescription[ count ];
	pAttributeCount = count;
}

const VkVertexInputAttributeDescription &devkPipelineConfiguration::GetAttributeAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttributeCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attributeCount" );
	}
	
	return pAttributes[ index ];
}

void devkPipelineConfiguration::SetAttributeAt( int index, const VkVertexInputAttributeDescription &attribute ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttributeCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attributeCount" );
	}
	
	memcpy( pAttributes + index, &attribute, sizeof( attribute ) );
}

void devkPipelineConfiguration::SetAttributeAt( int index, int location, int binding,
eAttributeFormat format, int offset ){
	VkVertexInputAttributeDescription description;
	description.location = ( uint32_t )location;
	description.binding = ( uint32_t )binding;
	description.offset = ( uint32_t )offset;
	
	switch( format ){
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
	
	SetAttributeAt( index, description );
}



// Operators
//////////////

bool devkPipelineConfiguration::operator==( const devkPipelineConfiguration &configuration ) const{
	if( pBindingCount != configuration.pBindingCount ){
		return false;
	}
	if( pBindingCount > 0 && memcmp( pBindings, configuration.pBindings,
	sizeof( VkVertexInputBindingDescription ) * pBindingCount ) ){
		return false;
	}
	
	if( pAttributeCount != configuration.pAttributeCount ){
		return false;
	}
	if( pAttributeCount > 0 && memcmp( pAttributes, configuration.pAttributes,
	sizeof( VkVertexInputAttributeDescription ) * pAttributeCount ) ){
		return false;
	}
	
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
	&& pColorWriteMask[ 0 ] == configuration.pColorWriteMask[ 0 ]
	&& pColorWriteMask[ 1 ] == configuration.pColorWriteMask[ 1 ]
	&& pColorWriteMask[ 2 ] == configuration.pColorWriteMask[ 2 ]
	&& pColorWriteMask[ 3 ] == configuration.pColorWriteMask[ 3 ]
	&& pDepthWriteMask == configuration.pDepthWriteMask
	&& pDepthFunction == configuration.pDepthFunction
	&& pDepthTest == configuration.pDepthTest
	&& pStencilTest == configuration.pStencilTest;
}

devkPipelineConfiguration &devkPipelineConfiguration::operator=( const devkPipelineConfiguration &configuration ){
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
	pColorWriteMask[ 0 ] = configuration.pColorWriteMask[ 0 ];
	pColorWriteMask[ 1 ] = configuration.pColorWriteMask[ 1 ];
	pColorWriteMask[ 2 ] = configuration.pColorWriteMask[ 2 ];
	pColorWriteMask[ 3 ] = configuration.pColorWriteMask[ 3 ];
	pDepthWriteMask = configuration.pDepthWriteMask;
	pDepthFunction = configuration.pDepthFunction;
	pDepthTest = configuration.pDepthTest;
	pStencilTest = configuration.pStencilTest;
	
	SetBindingCount( configuration.pBindingCount );
	if( pBindingCount > 0 ){
		memcpy( pBindings, configuration.pBindings,
			sizeof( VkVertexInputBindingDescription ) * pBindingCount );
	}
	
	SetAttributeCount( configuration.pAttributeCount );
	if( pAttributeCount > 0 ){
		memcpy( pAttributes, configuration.pAttributes,
			sizeof( VkVertexInputAttributeDescription ) * pAttributeCount );
	}

	return *this;
}
