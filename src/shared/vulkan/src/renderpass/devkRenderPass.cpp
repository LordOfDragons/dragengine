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

#include "devkRenderPass.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkRenderPass
///////////////////////

devkRenderPass::devkRenderPass( devkDevice &device, const devkRenderPassConfiguration &configuration ) :
pDevice( device ),
pConfiguration( configuration ),
pRenderPass( VK_NULL_HANDLE )
{
	const int subPassCount = configuration.GetSubPassCount();
	if( subPassCount < 1 ){
		DETHROW_INFO( deeInvalidParam, "subPassCount < 1" );
	}
	
	const int attachmentCount = configuration.GetAttachmentCount();
	int i, j, k;
	
	for( i=0; i<subPassCount; i++ ){
		const devkRenderPassConfiguration::sSubPass &subPass = configuration.GetSubPassAt( i );
		
		int att = subPass.depthStencilAttachment;
		if( att < -1 ){
			decString error;
			error.Format( "subPass[%d].depthStencilAttachment(%d) < -1", i, att );
			DETHROW_INFO( deeInvalidParam, error );
		}
		if( att >= attachmentCount ){
			decString error;
			error.Format( "subPass[%d].depthStencilAttachment(%d) >= attachmentCount(%d)",
				i, att, attachmentCount );
			DETHROW_INFO( deeInvalidParam, error );
		}
		
		for( j=0; j<8; j++ ){
			att = subPass.colorAttachments[ j ];
			if( att < -1 ){
				decString error;
				error.Format( "subPass[%d].colorAttachmentCount[%d](%d) < -1", i, j, att );
				DETHROW_INFO( deeInvalidParam, error );
			}
			if( att >= attachmentCount ){
				decString error;
				error.Format( "subPass[%d].colorAttachmentCount[%d](%d) >= attachmentCount(%d)",
					i, j, att, attachmentCount );
				DETHROW_INFO( deeInvalidParam, error );
			}
		}
	}
	
	VkSubpassDescription *subPassInfo = nullptr;
	VkAttachmentReference *attRef = nullptr;
	uint32_t *attPreserve = nullptr;
	int *attRetain = nullptr;
	
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan(); )
		
		VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
		
		// attachments we can use directly
		renderPassInfo.attachmentCount = attachmentCount;
		renderPassInfo.pAttachments = configuration.GetAttachments();
		
		// sub passes we have to calculate. this is a bit of a mess since vulkan requires
		// us to explicitly define what is implicitely defined. this requires building
		// appropriate temporary data structures
		
		// sub pass maximum color attachments: 8
		// sub pass maximum depth/stencil attachments: 1
		const int maxAttRef = subPassCount * ( 8 + 1 );
		attRef = new VkAttachmentReference[ maxAttRef ]{}; // max 8 color + 1 depth/stencil
		
		if( attachmentCount > 0 ){
			attPreserve = new uint32_t[ attachmentCount ];
		}
		
		subPassInfo = new VkSubpassDescription[ subPassCount ]{};
		
		// init sub pass configuration
		for( i=0; i<subPassCount; i++ ){
			subPassInfo[ i ].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		}
		
		// set the attachments used by the sub pass
		VkAttachmentReference *ptrAttRef = attRef;
		
		for( i=0; i<subPassCount; i++ ){
			const devkRenderPassConfiguration::sSubPass &subPass = configuration.GetSubPassAt( i );
			
			if( subPass.depthStencilAttachment != -1 ){
				subPassInfo[ i ].pDepthStencilAttachment = ptrAttRef;
				ptrAttRef->attachment = ( uint32_t )subPass.depthStencilAttachment;
				ptrAttRef->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				ptrAttRef++;
			}
			
			for( j=0; j<8; j++ ){
				if( subPass.colorAttachments[ j ] == -1 ){
					break;
				}
				
				VkAttachmentReference &ref = ptrAttRef[ subPassInfo[ i ].colorAttachmentCount++ ];
				ref.attachment = ( uint32_t )subPass.colorAttachments[ j ];
				ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
			
			if( subPassInfo[ i ].colorAttachmentCount > 0 ){
				subPassInfo[ i ].pColorAttachments = ptrAttRef;
				ptrAttRef += subPassInfo[ i ].colorAttachmentCount;
			}
		}
		
		// calculate the last sub pass requiring each individual attachment. if the attachment
		// is required to be retained beyond the render pass sub pass count is used
		if( attachmentCount > 0 ){
			attRetain = new int[ attachmentCount ];
			
			for( i=0; i<attachmentCount; i++ ){
				const VkAttachmentDescription &desc = configuration.GetAttachmentAt( i );
				
				if( desc.storeOp == VK_ATTACHMENT_STORE_OP_STORE ){
					attRetain[ i ] = subPassCount;
					continue;
				}
				
				for( j=0; j<subPassCount; j++ ){
					const devkRenderPassConfiguration::sSubPass &subPass = configuration.GetSubPassAt( j );
					
					if( subPass.depthStencilAttachment == i ){
						attRetain[ i ] = j;
						continue;
					}
					
					for( k=0; k<8; k++ ){
						if( subPass.colorAttachments[ k ] == i ){
							attRetain[ i ] = j;
							break;
						}
					}
				}
			}
		}
		
		// set attachment preservation. this is implicitely defined but vulkan needs it explcit
		uint32_t *ptrAttPreserve = attPreserve;
		
		for( i=0; i<subPassCount; i++ ){
			const devkRenderPassConfiguration::sSubPass &subPass = configuration.GetSubPassAt( i );
			
			for( j=0; j<attachmentCount; j++ ){
				bool isUsed = false;
				
				if( subPass.depthStencilAttachment == j ){
					isUsed = true;
					
				}else{
					for( k=0; k<8; k++ ){
						if( subPass.colorAttachments[ k ] == j ){
							isUsed = true;
							break;
						}
					}
				}
				
				if( ! isUsed && i < attRetain[ j ] ){
					ptrAttPreserve[ subPassInfo[ i ].preserveAttachmentCount++ ] = ( uint32_t )j;
				}
			}
			
			if( subPassInfo[ i ].preserveAttachmentCount > 0 ){
				subPassInfo[ i ].pPreserveAttachments = ptrAttPreserve;
				ptrAttPreserve += subPassInfo[ i ].preserveAttachmentCount;
			}
		}
		
		// finish sub pass
		renderPassInfo.subpassCount = subPassCount;
		renderPassInfo.pSubpasses = subPassInfo;
		
		// create render pass
		VK_CHECK( vulkan, device.vkCreateRenderPass( device.GetDevice(),
			&renderPassInfo, VK_NULL_HANDLE, &pRenderPass ) );
		
		// clean up temporaries
		if( subPassInfo ){
			delete [] subPassInfo;
		}
		if( attPreserve ){
			delete [] attPreserve;
		}
		if( attRef ){
			delete [] attRef;
		}
		if( attRetain ){
			delete [] attRetain;
		}
		
	}catch( const deException & ){
		if( subPassInfo ){
			delete [] subPassInfo;
		}
		if( attPreserve ){
			delete [] attPreserve;
		}
		if( attRef ){
			delete [] attRef;
		}
		if( attRetain ){
			delete [] attRetain;
		}
		pCleanUp();
		throw;
	}
}

devkRenderPass::~devkRenderPass(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkRenderPass::pCleanUp(){
	VkDevice device = pDevice.GetDevice();
	
	if( pRenderPass ){
		pDevice.vkDestroyRenderPass( device, pRenderPass, VK_NULL_HANDLE );
	}
}
