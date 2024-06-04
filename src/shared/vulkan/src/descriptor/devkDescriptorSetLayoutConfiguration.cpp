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

#include "devkDescriptorSetLayoutConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkDescriptorSetLayoutConfiguration
///////////////////////////////////////////////

devkDescriptorSetLayoutConfiguration::devkDescriptorSetLayoutConfiguration() :
pType( VK_DESCRIPTOR_TYPE_SAMPLER ),
pShaderStageFlags( VK_SHADER_STAGE_ALL ),
pLayoutBindings( nullptr ),
pLayoutBindingCount( 0 ){
}

devkDescriptorSetLayoutConfiguration::devkDescriptorSetLayoutConfiguration(
	const devkDescriptorSetLayoutConfiguration &configuration ) :
pType( VK_DESCRIPTOR_TYPE_SAMPLER ),
pShaderStageFlags( VK_SHADER_STAGE_ALL ),
pLayoutBindings( nullptr ),
pLayoutBindingCount( 0 )
{
	*this = configuration;
}

devkDescriptorSetLayoutConfiguration::~devkDescriptorSetLayoutConfiguration(){
	if( pLayoutBindings ){
		delete [] pLayoutBindings;
	}
}



// Management
///////////////

void devkDescriptorSetLayoutConfiguration::SetType( VkDescriptorType type ){
	pType = type;
}

void devkDescriptorSetLayoutConfiguration::SetShaderStageFlags( VkShaderStageFlags flags ){
	pShaderStageFlags = flags;
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pLayoutBindings ){
		delete [] pLayoutBindings;
		pLayoutBindings = nullptr;
		pLayoutBindingCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pLayoutBindings = new VkDescriptorSetLayoutBinding[ count ];
	memset( pLayoutBindings, 0, sizeof( VkDescriptorSetLayoutBinding ) * count );
	pLayoutBindingCount = count;
}

const VkDescriptorSetLayoutBinding &devkDescriptorSetLayoutConfiguration::GetLayoutBindingAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pLayoutBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= layoutBindingCount" );
	}
	return pLayoutBindings[ index ];
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingAt(
int index, const VkDescriptorSetLayoutBinding &binding ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pLayoutBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= layoutBindingCount" );
	}
	if( binding.pImmutableSamplers ){
		DETHROW_INFO( deeInvalidParam, "binding.pImmutableSamplers not nullptr" );
	}
	
	pLayoutBindings[ index ] = binding;
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingAt(
int index, int binding, VkDescriptorType type, VkShaderStageFlags flags ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pLayoutBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= layoutBindingCount" );
	}
	
	pLayoutBindings[ index ].binding = binding;
	pLayoutBindings[ index ].stageFlags = flags;
	pLayoutBindings[ index ].descriptorType = type;
	pLayoutBindings[ index ].descriptorCount = 1;
}



// Operators
//////////////

bool devkDescriptorSetLayoutConfiguration::operator==(
const devkDescriptorSetLayoutConfiguration &configuration ) const{
	if( pType != configuration.pType
	|| pShaderStageFlags != configuration.pShaderStageFlags
	|| pLayoutBindingCount != configuration.pLayoutBindingCount ){
		return false;
	}
	
	if( pLayoutBindingCount > 0 && memcmp( pLayoutBindings, configuration.pLayoutBindings,
	sizeof( VkDescriptorSetLayoutBinding ) * pLayoutBindingCount ) ){
		return false;
	}
	
	return true;
}

devkDescriptorSetLayoutConfiguration &devkDescriptorSetLayoutConfiguration::operator=(
const devkDescriptorSetLayoutConfiguration &configuration ){
	pType = configuration.pType;
	pShaderStageFlags = configuration.pShaderStageFlags;
	
	SetLayoutBindingCount( configuration.pLayoutBindingCount );
	if( pLayoutBindingCount > 0 ){
		memcpy( pLayoutBindings, configuration.pLayoutBindings,
			sizeof( VkDescriptorSetLayoutBinding ) * pLayoutBindingCount );
	}
	
	return *this;
}
