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

#include "devkPipelineCompute.h"
#include "devkPipelineGraphic.h"
#include "devkPipelineManager.h"

#include <dragengine/common/exceptions.h>


// Class devkPipelineManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

devkPipelineManager::devkPipelineManager( devkDevice &device ) :
pDevice( device ){
}

devkPipelineManager::~devkPipelineManager(){
}



// Management
///////////////

int devkPipelineManager::GetCount() const{
	return pLayouts.GetCount();
}

devkPipeline *devkPipelineManager::GetAt( int index ) const{
	return ( devkPipeline* )pLayouts.GetAt( index );
}

devkPipeline *devkPipelineManager::GetWith( const devkPipelineConfiguration &configuration ){
	const int count = pLayouts.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		devkPipeline * const layout = ( devkPipeline* )pLayouts.GetAt( i );
		if( layout->GetConfiguration() == configuration ){
			return layout;
		}
	}
	
	devkPipeline::Ref layout;
	
	switch( configuration.GetType() ){
	case devkPipelineConfiguration::etGraphics:
		layout.TakeOver( new devkPipelineGraphic( pDevice, configuration ) );
		break;
		
	case devkPipelineConfiguration::etCompute:
		layout.TakeOver( new devkPipelineCompute( pDevice, configuration ) );
		break;
		
	case devkPipelineConfiguration::etRaytracing:
		DETHROW( deeInvalidParam );
	}
	
	pLayouts.Add( layout );
	return layout;
}

bool devkPipelineManager::HasWith( const devkPipelineConfiguration &configuration ) const{
	const int count = pLayouts.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( devkPipeline* )pLayouts.GetAt( i ) )->GetConfiguration() == configuration ){
			return true;
		}
	}
	
	return false;
}

void devkPipelineManager::Clear(){
	pLayouts.RemoveAll();
}
