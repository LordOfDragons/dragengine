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

#include "devkDescriptorPoolSlot.h"
#include "devkDescriptorPoolPool.h"

#include <dragengine/common/exceptions.h>


// class devkDescriptorPoolSlot
/////////////////////////////////

devkDescriptorPoolSlot::devkDescriptorPoolSlot( devkDescriptorPoolPool &pool, VkDescriptorSet set ) :
pPool( pool ),
pSet( set )
{
	if( ! set ){
		DETHROW_INFO( deeNullPointer, "set" );
	}
}

devkDescriptorPoolSlot::~devkDescriptorPoolSlot(){
	// descriptor sets are automatically removed if the pool is deleted
}



// Management
///////////////

void devkDescriptorPoolSlot::ReturnToPool(){
	pPool.Return( this );
}
