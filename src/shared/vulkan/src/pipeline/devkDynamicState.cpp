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
