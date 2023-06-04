/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoglPipeline.h"
#include "deoglPipelineManager.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglPipelineManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPipelineManager::deoglPipelineManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pState( renderThread ){
}

deoglPipelineManager::~deoglPipelineManager(){
}



// Management
///////////////

const deoglPipeline *deoglPipelineManager::GetWithRTSIndex( int index ){
	const deMutexGuard guard( pMutex );
	return ( const deoglPipeline* )pRTSPipelines.GetAt( index );
}

const deoglPipeline *deoglPipelineManager::GetWith(
const deoglPipelineConfiguration &configuration, bool assignRTSIndex ){
	const deMutexGuard guard( pMutex );
	const int count = pPipelines.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglPipeline * const pipeline = ( deoglPipeline* )pPipelines.GetAt( i );
		if( pipeline->GetGlConfiguration() == configuration ){
			return pipeline;
		}
	}
	
	const deoglPipeline::Ref pipeline( deoglPipeline::Ref::New(
		new deoglPipeline( pRenderThread, configuration ) ) );
	pPipelines.Add( pipeline );
	
	if( assignRTSIndex ){
		pipeline->SetRTSIndex( pRTSPipelines.GetCount() );
		pRTSPipelines.Add( pipeline );
	}
	
	return pipeline; // caller does not hold reference
}
