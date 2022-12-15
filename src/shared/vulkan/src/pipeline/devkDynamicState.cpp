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

void devkDynamicState::Apply( devkCommandBuffer &commandBuffer, const devkPipeline &pipeline ){
	const devkPipelineConfiguration &config = pipeline.GetConfiguration();
	
	if( config.GetDynamicViewport() ){
		// VK_DYNAMIC_STATE_VIEWPORT;
	}
	
	if( config.GetDynamicScissor() ){
		// VK_DYNAMIC_STATE_SCISSOR;
	}
	
	if( config.GetDynamicDepthBias() ){
		// VK_DYNAMIC_STATE_DEPTH_BIAS;
	}
	
	if( config.GetDynamicStencil() ){
		// VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		// VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		// VK_DYNAMIC_STATE_STENCIL_REFERENCE;
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
