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
