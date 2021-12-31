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

#include "devkImage.h"
#include "devkImageView.h"
#include "../devkDevice.h"
#include "../devkInstance.h"
#include "../deSharedVulkan.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkImageView
////////////////////////

devkImageView::devkImageView( devkImage *image ) :
pImage( image ),
pImageView( VK_NULL_HANDLE )
{
	if( ! image ){
		DETHROW_INFO( deeNullPointer, "image" );
	}
	
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = image->GetDevice().GetInstance().GetVulkan(); )
		const devkImageConfiguration &config = image->GetConfiguration();
		
		VkImageViewCreateInfo imageViewInfo;
		memset( &imageViewInfo, 0, sizeof( imageViewInfo ) );
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = image->GetImage();
		
		switch( config.GetType() ){
		case VK_IMAGE_TYPE_1D:
			if( config.GetLayerCount() > 1 ){
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
				
			}else{
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
			}
			break;
			
		case VK_IMAGE_TYPE_2D:
			if( config.GetFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ){
				if( config.GetLayerCount() > 6 ){
					imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
					
				}else{
					imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				}
				
			}else if( config.GetLayerCount() > 1 ){
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
				
			}else{
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			}
			break;
			
		case VK_IMAGE_TYPE_3D:
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "unsupported image type" );
		}
		
		imageViewInfo.format = config.GetFormat();
		
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = config.GetMipMapCount();
		
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = config.GetLayerCount();
		
		if( config.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			
		}else{
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
		}
		
		VK_CHECK( vulkan, image->GetDevice().vkCreateImageView( image->GetDevice().GetDevice(),
			&imageViewInfo, VK_NULL_HANDLE, &pImageView ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkImageView::devkImageView( devkImage *image, int layer ) :
pImage( image ),
pImageView( VK_NULL_HANDLE )
{
	if( ! image ){
		DETHROW_INFO( deeNullPointer, "image" );
	}
	
	const devkImageConfiguration &config = image->GetConfiguration();
	
	if( layer < 0 ){
		DETHROW_INFO( deeNullPointer, "layer < 0" );
	}
	if( layer >= config.GetLayerCount() ){
		DETHROW_INFO( deeNullPointer, "layer >= image.layerCount" );
	}
	
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = image->GetDevice().GetInstance().GetVulkan(); )
		
		VkImageViewCreateInfo imageViewInfo;
		memset( &imageViewInfo, 0, sizeof( imageViewInfo ) );
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = image->GetImage();
		
		switch( config.GetType() ){
		case VK_IMAGE_TYPE_1D:
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
			break;
			
		case VK_IMAGE_TYPE_2D:
			if( config.GetFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ){
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				
			}else{
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			}
			break;
			
		case VK_IMAGE_TYPE_3D:
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "unsupported image type" );
		}
		
		imageViewInfo.format = config.GetFormat();
		
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = config.GetMipMapCount();
		
		if( config.GetFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ){
			imageViewInfo.subresourceRange.baseArrayLayer = ( uint32_t )( 6 * layer );
			imageViewInfo.subresourceRange.layerCount = 6;
			
		}else{
			imageViewInfo.subresourceRange.baseArrayLayer = ( uint32_t )layer;
			imageViewInfo.subresourceRange.layerCount = 1;
		}
		
		if( config.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			
		}else{
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
		}
		
		VK_CHECK( vulkan, image->GetDevice().vkCreateImageView( image->GetDevice().GetDevice(),
			&imageViewInfo, VK_NULL_HANDLE, &pImageView ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkImageView::devkImageView( devkImage *image, int layer, int level ) :
pImage( image ),
pImageView( VK_NULL_HANDLE )
{
	if( ! image ){
		DETHROW_INFO( deeNullPointer, "image" );
	}
	
	const devkImageConfiguration &config = image->GetConfiguration();
	
	if( layer < 0 ){
		DETHROW_INFO( deeNullPointer, "layer < 0" );
	}
	if( layer >= config.GetLayerCount() ){
		DETHROW_INFO( deeNullPointer, "layer >= image.layerCount" );
	}
	if( level < 0 ){
		DETHROW_INFO( deeNullPointer, "level < 0" );
	}
	if( level >= config.GetMipMapCount() ){
		DETHROW_INFO( deeNullPointer, "level >= image.mipMapCount" );
	}
	
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = image->GetDevice().GetInstance().GetVulkan(); )
		
		VkImageViewCreateInfo imageViewInfo;
		memset( &imageViewInfo, 0, sizeof( imageViewInfo ) );
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = image->GetImage();
		
		switch( config.GetType() ){
		case VK_IMAGE_TYPE_1D:
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
			break;
			
		case VK_IMAGE_TYPE_2D:
			if( config.GetFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ){
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				
			}else{
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			}
			break;
			
		case VK_IMAGE_TYPE_3D:
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "unsupported image type" );
		}
		
		imageViewInfo.format = config.GetFormat();
		
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		imageViewInfo.subresourceRange.baseMipLevel = ( uint32_t )level;
		imageViewInfo.subresourceRange.levelCount = 1;
		
		if( config.GetFlags() & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ){
			imageViewInfo.subresourceRange.baseArrayLayer = ( uint32_t )( 6 * layer );
			imageViewInfo.subresourceRange.layerCount = 6;
			
		}else{
			imageViewInfo.subresourceRange.baseArrayLayer = ( uint32_t )layer;
			imageViewInfo.subresourceRange.layerCount = 1;
		}
		
		if( config.GetFlags() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ){
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			
		}else{
			imageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_COLOR_BIT;
		}
		
		VK_CHECK( vulkan, image->GetDevice().vkCreateImageView( image->GetDevice().GetDevice(),
			&imageViewInfo, VK_NULL_HANDLE, &pImageView ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkImageView::~devkImageView(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkImageView::pCleanUp(){
	if( pImage && pImageView ){
		pImage->GetDevice().vkDestroyImageView( pImage->GetDevice().GetDevice(), pImageView, VK_NULL_HANDLE );
	}
}
