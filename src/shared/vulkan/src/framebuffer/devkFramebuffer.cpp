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

#include "devkFramebuffer.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"
#include "../renderpass/devkRenderPass.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkFramebuffer
//////////////////////////

devkFramebuffer::devkFramebuffer( devkRenderPass *renderPass, const devkFramebufferConfiguration &configuration ) :
pRenderPass( renderPass ),
pConfiguration( configuration ),
pFramebuffer( VK_NULL_HANDLE )
{
	if( ! renderPass ){
		DETHROW_INFO( deeNullPointer, "renderPass" );
	}
	
	const int attachmentCount = configuration.GetAttachmentCount();
	int i;
	
	VkImageView *attachments = nullptr;
	
	try{
		devkDevice &device = renderPass->GetDevice();
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan(); )
		
		VkFramebufferCreateInfo framebufferInfo;
		memset( &framebufferInfo, 0, sizeof( framebufferInfo ) );
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->GetRenderPass();
		framebufferInfo.width = ( uint32_t )configuration.GetSize().x;
		framebufferInfo.height = ( uint32_t )configuration.GetSize().y;
		framebufferInfo.layers = ( uint32_t )configuration.GetLayerCount();
		
		if( attachmentCount > 0 ){
			attachments  = new VkImageView[ attachmentCount ];
			for( i=0; i<attachmentCount; i++ ){
				attachments[ i ] = configuration.GetAttachmentAt( i )->GetImageView();
			}
			
			framebufferInfo.attachmentCount = attachmentCount;
			framebufferInfo.pAttachments = attachments;
		}
		
		VK_CHECK( vulkan, device.vkCreateFramebuffer( device.GetDevice(),
			&framebufferInfo, VK_NULL_HANDLE, &pFramebuffer ) );
		
		if( attachments ){
			delete [] attachments;
		}
		
	}catch( const deException & ){
		if( attachments ){
			delete [] attachments;
		}
		pCleanUp();
		throw;
	}
}

devkFramebuffer::~devkFramebuffer(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkFramebuffer::pCleanUp(){
	if( pFramebuffer ){
		pRenderPass->GetDevice().vkDestroyFramebuffer(
			pRenderPass->GetDevice().GetDevice(), pFramebuffer, VK_NULL_HANDLE );
	}
}
