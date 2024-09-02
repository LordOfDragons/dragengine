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
