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
