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

#include "devkQueue.h"
#include "devkDevice.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkQueue
////////////////////

devkQueue::devkQueue( devkDevice &device, uint32_t family, VkQueue queue ) :
pDevice( device ),
pFamily( family ),
pQueue( queue )
{
	if( ! queue ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkQueue::~devkQueue(){
	pCleanUp();
}



// Management
///////////////

devkCommandPool::Ref devkQueue::CreateCommandPool(){
	return devkCommandPool::Ref::With( new devkCommandPool( pDevice, pFamily ) );
}



// Private Functions
//////////////////////

void devkQueue::pCleanUp(){
}
