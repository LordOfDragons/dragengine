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

int deoglPipelineManager::GetCount() const{
	return pPipelines.GetCount();
}

const deoglPipeline *deoglPipelineManager::GetAt( int index ) const{
	return ( deoglPipeline* )pPipelines.GetAt( index );
}

const deoglPipeline *deoglPipelineManager::GetWithRTSIndex( int index ) const{
	return ( const deoglPipeline* )pRTSPipelines.GetAt( index );
}

const deoglPipeline *deoglPipelineManager::GetWith(
const deoglPipelineConfiguration &configuration, bool assignRTSIndex ){
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

bool deoglPipelineManager::HasWith( const deoglPipelineConfiguration &configuration ) const{
	const int count = pPipelines.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglPipeline* )pPipelines.GetAt( i ) )->GetGlConfiguration() == configuration ){
			return true;
		}
	}
	
	return false;
}

void deoglPipelineManager::Clear(){
	pPipelines.RemoveAll();
}
