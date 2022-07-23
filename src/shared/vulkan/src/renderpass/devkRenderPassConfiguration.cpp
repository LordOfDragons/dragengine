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

#include "devkRenderPassConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkRenderPassConfiguration
//////////////////////////////////////

devkRenderPassConfiguration::devkRenderPassConfiguration() :
pAttachmentCount( 0 ),
pAttachments( nullptr ),
pClearValues( nullptr ),
pSubPassCount( 0 ),
pSubPasses( nullptr ){
}

devkRenderPassConfiguration::devkRenderPassConfiguration( const devkRenderPassConfiguration &configuration ) :
pAttachmentCount( 0 ),
pAttachments( nullptr ),
pClearValues( nullptr ),
pSubPassCount( 0 ),
pSubPasses( nullptr )
{
	*this = configuration;
}

devkRenderPassConfiguration::~devkRenderPassConfiguration(){
	if( pSubPasses ){
		delete [] pSubPasses;
	}
	if( pAttachments ){
		delete [] pAttachments;
	}
	if( pClearValues ){
		delete [] pClearValues;
	}
}



// Management
///////////////

void devkRenderPassConfiguration::SetAttachmentCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pAttachments ){
		delete [] pAttachments;
		pAttachments = nullptr;
	}
	if( pClearValues ){
		delete [] pClearValues;
		pClearValues = nullptr;
	}
	
	pAttachmentCount = 0;
	
	if( count == 0 ){
		return;
	}
	
	pAttachments = new VkAttachmentDescription[ count ];
	memset( pAttachments, 0, sizeof( VkAttachmentDescription ) * count );
	
	pClearValues = new VkClearValue[ count ];
	memset( pClearValues, 0, sizeof( VkClearAttachment ) * count );
	
	pAttachmentCount = count;
}

const VkAttachmentDescription &devkRenderPassConfiguration::GetAttachmentAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	return pAttachments[ index ];
}

void devkRenderPassConfiguration::SetAttachmentAt( int index, const VkAttachmentDescription &attachment ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	pAttachments[ index ] = attachment;
}

void devkRenderPassConfiguration::SetColorAttachmentAt( int index, VkFormat format, pInType in, eOutType out ){
	VkAttachmentDescription description;
	memset( &description, 0, sizeof( description ) );
	
	description.format = format;
	description.samples = VK_SAMPLE_COUNT_1_BIT;
	description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	
	switch( in ){
	case eitClear:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		break;
		
	case eitLoad:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		break;
		
	case eitDiscard:
	default:
		description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	}
	
	switch( out ){
	case eotRender:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		break;
		
	case eotShader:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		break;
		
	case eotReadBack:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		break;
		
	case eotDisplay:
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		break;
		
	case eotDiscard:
	default:
		description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // ?
	}
	
	SetAttachmentAt( index, description );
}

const VkClearValue &devkRenderPassConfiguration::GetClearValueAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	return pClearValues[ index ];
}

void devkRenderPassConfiguration::SetClearValueAt( int index, const VkClearValue &clearValue ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pAttachmentCount ){
		DETHROW_INFO( deeInvalidParam, "index >= attachmentCount" );
	}
	
	pClearValues[ index ] = clearValue;
}

void devkRenderPassConfiguration::SetClearValueColorFloatAt( int index, float red, float green, float blue, float alpha ){
	VkClearValue clear;
	memset( &clear, 0, sizeof( clear ) );
	clear.color.float32[ 0 ] = red;
	clear.color.float32[ 1 ] = green;
	clear.color.float32[ 2 ] = blue;
	clear.color.float32[ 3 ] = alpha;
	SetClearValueAt( index, clear );
}

void devkRenderPassConfiguration::SetClearValueColorIntAt( int index, int red, int green, int blue, int alpha ){
	VkClearValue clear;
	memset( &clear, 0, sizeof( clear ) );
	clear.color.int32[ 0 ] = ( int32_t )red;
	clear.color.int32[ 1 ] = ( int32_t )green;
	clear.color.int32[ 2 ] = ( int32_t )blue;
	clear.color.int32[ 3 ] = ( int32_t )alpha;
	SetClearValueAt( index, clear );
}

void devkRenderPassConfiguration::SetClearValueColorUIntAt( int index, int red, int green, int blue, int alpha ){
	VkClearValue clear;
	memset( &clear, 0, sizeof( clear ) );
	clear.color.uint32[ 0 ] = ( uint32_t )red;
	clear.color.uint32[ 1 ] = ( uint32_t )green;
	clear.color.uint32[ 2 ] = ( uint32_t )blue;
	clear.color.uint32[ 3 ] = ( uint32_t )alpha;
	SetClearValueAt( index, clear );
}

void devkRenderPassConfiguration::SetClearValueDepthAt( int index, float depth, int stencil ){
	VkClearValue clear;
	memset( &clear, 0, sizeof( clear ) );
	clear.depthStencil.depth = depth;
	clear.depthStencil.stencil = ( uint32_t )stencil;
	SetClearValueAt( index, clear );
}



void devkRenderPassConfiguration::SetSubPassCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pSubPasses ){
		delete [] pSubPasses;
		pSubPasses = nullptr;
		pSubPassCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pSubPasses = new sSubPass[ count ];
	memset( pSubPasses, 0, sizeof( sSubPass ) * count );
	pSubPassCount = count;
}

const devkRenderPassConfiguration::sSubPass &devkRenderPassConfiguration::GetSubPassAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pSubPassCount ){
		DETHROW_INFO( deeInvalidParam, "index >= subPassCount" );
	}
	
	return pSubPasses[ index ];
}

void devkRenderPassConfiguration::SetSubPassAt( int index, const sSubPass &subpass ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pSubPassCount ){
		DETHROW_INFO( deeInvalidParam, "index >= subPassCount" );
	}
	
	pSubPasses[ index ] = subpass;
}

void devkRenderPassConfiguration::SetSubPassAt( int index, int depthStencil, int color1,
int color2, int color3, int color4, int color5, int color6, int color7, int color8 ){
	sSubPass subPass;
	memset( &subPass, 0, sizeof( subPass ) );
	subPass.depthStencilAttachment = depthStencil;
	subPass.colorAttachments[ 0 ] = color1;
	subPass.colorAttachments[ 1 ] = color2;
	subPass.colorAttachments[ 2 ] = color3;
	subPass.colorAttachments[ 3 ] = color4;
	subPass.colorAttachments[ 4 ] = color5;
	subPass.colorAttachments[ 5 ] = color6;
	subPass.colorAttachments[ 6 ] = color7;
	subPass.colorAttachments[ 7 ] = color8;
	SetSubPassAt( index, subPass );
}



// Operators
//////////////

bool devkRenderPassConfiguration::operator==( const devkRenderPassConfiguration &configuration ) const{
	if( pAttachmentCount != configuration.pAttachmentCount
	|| pSubPassCount != configuration.pSubPassCount ){
		return false;
	}
	
	if( pAttachmentCount > 0 ){
		if( memcmp( pAttachments, configuration.pAttachments, sizeof( VkAttachmentDescription ) * pAttachmentCount ) ){
			return false;
		}
		if( memcmp( pClearValues, configuration.pClearValues, sizeof( VkClearValue ) * pAttachmentCount ) ){
			return false;
		}
	}
	
	if( pSubPassCount > 0 && memcmp( pSubPasses, configuration.pSubPasses, sizeof( sSubPass ) * pSubPassCount ) ){
		return false;
	}
	
	return true;
}

devkRenderPassConfiguration &devkRenderPassConfiguration::operator=( const devkRenderPassConfiguration &configuration ){
	SetAttachmentCount( configuration.pAttachmentCount );
	if( pAttachmentCount > 0 ){
		memcpy( pAttachments, configuration.pAttachments, sizeof( VkAttachmentDescription ) * pAttachmentCount );
		memcpy( pClearValues, configuration.pClearValues, sizeof( VkClearValue ) * pAttachmentCount );
	}
	
	SetSubPassCount( configuration.pSubPassCount );
	if( pSubPassCount > 0 ){
		memcpy( pSubPasses, configuration.pSubPasses, sizeof( sSubPass ) * pSubPassCount );
	}
	
	return *this;
}
