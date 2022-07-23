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

#include "devkShaderModule.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkShaderModule
///////////////////////////

devkShaderModule::devkShaderModule( devkDevice &device, const char *path, decBaseFileReader &reader ) :
pDevice( device ),
pPath( path ),
pSource( nullptr ),
pSourceLength( 0 ),
pModule( VK_NULL_HANDLE )
{
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
		
		// load source
		pSourceLength = reader.GetLength();
		if( pSourceLength == 0 ){
			DETHROW_INFO( deeInvalidParam, "empty sources" );
		}
		
		pSource = new char[ pSourceLength ];
		reader.Read( pSource, pSourceLength );
		
		// create shader module
		VkShaderModuleCreateInfo info;
		memset( &info, 0, sizeof( info ) );
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = pSourceLength;
		info.pCode = ( const uint32_t * )pSource;
		
		VK_CHECK( vulkan, device.vkCreateShaderModule( device.GetDevice(), &info, NULL, &pModule ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkShaderModule::~devkShaderModule(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkShaderModule::pCleanUp(){
	if( pModule ){
		pDevice.vkDestroyShaderModule( pDevice.GetDevice(), pModule, VK_NULL_HANDLE );
	}
	if( pSource ){
		delete [] pSource;
	}
}
