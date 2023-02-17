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

#include <stdlib.h>
#include <string.h>

#include "devkDynamicState.h"
#include "devkPipeline.h"
#include "../devkDevice.h"
#include "../deSharedVulkan.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/common/exceptions.h>


// class devkDynamicState
///////////////////////////

devkDynamicState::devkDynamicState(){
	memset( this, 0, sizeof( devkDynamicState ) );
}

devkDynamicState::devkDynamicState( const devkDynamicState &state ){
	memset( this, 0, sizeof( devkDynamicState ) );
	*this = state;
}

devkDynamicState::~devkDynamicState(){
}



// Management
///////////////

void devkDynamicState::Apply( devkCommandBuffer &commandBuffer, const devkPipeline &pipeline ) const{
	const devkPipelineConfiguration &config = pipeline.GetConfiguration();
	devkDevice &device = pipeline.GetDevice();
	
	if( config.GetDynamicDepthBias() ){
		device.vkCmdSetDepthBias( commandBuffer.GetBuffer(),
			depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
	}
	
	if( config.GetDynamicStencil() ){
		#define SET_FRONT_BACK(func,front,back) \
			if( front == back ){ \
				device.func( commandBuffer.GetBuffer(), VK_STENCIL_FACE_FRONT_AND_BACK, front ); \
			}else{ \
				device.func( commandBuffer.GetBuffer(), VK_STENCIL_FACE_FRONT_BIT, front ); \
				device.func( commandBuffer.GetBuffer(), VK_STENCIL_FACE_BACK_BIT, back ); \
			}
		
		SET_FRONT_BACK( vkCmdSetStencilCompareMask, stencilFrontCompareMask, stencilBackCompareMask )
		SET_FRONT_BACK( vkCmdSetStencilWriteMask, stencilFrontWriteMask, stencilBackWriteMask )
		SET_FRONT_BACK( vkCmdSetStencilReference, stencilFrontReference, stencilBackWriteMask )
		
		#undef SET_FRONT_BACK
	}
}



// Operators
//////////////

bool devkDynamicState::operator==( const devkDynamicState &state ) const{
	return memcmp( this, &state, sizeof( devkDynamicState ) ) == 0;
}

devkDynamicState &devkDynamicState::operator=( const devkDynamicState &state ){
	memcpy( this, &state, sizeof( devkDynamicState ) );
	return *this;
}
