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
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglPipeline
////////////////////////

// Constructor, destructor
////////////////////////////

deoglPipeline::deoglPipeline( deoglRenderThread &renderThread,
	const deoglPipelineConfiguration &configuration ) :
pRenderThread( renderThread ),
pRTSIndex( -1 ),
pGlConfiguration( nullptr ),
pVkPipeline( nullptr )
{
	try{
		pGlConfiguration = new deoglPipelineConfiguration( configuration );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPipeline::deoglPipeline( deoglRenderThread &renderThread,
	const devkPipelineConfiguration &configuration ) :
pRenderThread( renderThread ),
pRTSIndex( -1 ),
pGlConfiguration( nullptr ),
pVkPipeline( nullptr )
{
	try{
		pVkPipeline = renderThread.GetVulkanDevice()->GetPipelineManager().GetWith( configuration );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglPipeline::~deoglPipeline(){
	pCleanUp();
}



// Management
///////////////

const deoglPipelineConfiguration &deoglPipeline::GetGlConfiguration() const{
	DEASSERT_NOTNULL( pGlConfiguration )
	return *pGlConfiguration;
}

deoglShaderCompiled &deoglPipeline::GetGlShader() const{
	return *GetGlConfiguration().GetShaderRef().GetCompiled();
}

void deoglPipeline::SetRTSIndex( int index ){
	pRTSIndex = index;
}

void deoglPipeline::Activate() const{
	if( pGlConfiguration ){
		pGlConfiguration->Activate( pRenderThread );
		
	}else if( pVkPipeline ){
		DETHROW_INFO( deeInvalidAction, "missing implementation" );
	}
}



// Private Functions
//////////////////////

void deoglPipeline::pCleanUp(){
	// opengl: delete has to be done from render thread
	
	if( pGlConfiguration ){
		delete pGlConfiguration;
	}
}
