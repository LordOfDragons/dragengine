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

#include <stdio.h>
#include <string.h>

#include "devkDescriptorSetLayout.h"
#include "devkDescriptorSetLayoutManager.h"

#include <dragengine/common/exceptions.h>


// Class devkDescriptorSetLayoutManager
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

devkDescriptorSetLayoutManager::devkDescriptorSetLayoutManager( devkDevice &device ) :
pDevice( device ){
}

devkDescriptorSetLayoutManager::~devkDescriptorSetLayoutManager(){
}



// Management
///////////////

int devkDescriptorSetLayoutManager::GetCount() const{
	return pLayouts.GetCount();
}

devkDescriptorSetLayout *devkDescriptorSetLayoutManager::GetAt( int index ) const{
	return ( devkDescriptorSetLayout* )pLayouts.GetAt( index );
}

devkDescriptorSetLayout *devkDescriptorSetLayoutManager::GetWith(
const devkDescriptorSetLayoutConfiguration &configuration ){
	const int count = pLayouts.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		devkDescriptorSetLayout * const layout = ( devkDescriptorSetLayout* )pLayouts.GetAt( i );
		if( layout->GetConfiguration() == configuration ){
			return layout;
		}
	}
	
	devkDescriptorSetLayout::Ref layout;
	layout.TakeOver( new devkDescriptorSetLayout( pDevice, configuration ) );
	pLayouts.Add( layout );
	return layout; // called does not hold reference
}

bool devkDescriptorSetLayoutManager::HasWith( const devkDescriptorSetLayoutConfiguration &configuration ) const{
	const int count = pLayouts.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( devkDescriptorSetLayout* )pLayouts.GetAt( i ) )->GetConfiguration() == configuration ){
			return true;
		}
	}
	
	return false;
}

void devkDescriptorSetLayoutManager::Clear(){
	pLayouts.RemoveAll();
}
