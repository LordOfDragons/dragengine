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
		VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
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
