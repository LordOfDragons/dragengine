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
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglPipeline
////////////////////////

int deoglPipeline::pNextRTSPipelineIndex = 0;

// Constructor, destructor
////////////////////////////

deoglPipeline::deoglPipeline( deoglRenderThread &renderThread,
	const deoglPipelineConfiguration &configuration ) :
pRenderThread( renderThread ),
pRTSPipelineIndex( -1 ),
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
pRTSPipelineIndex( -1 ),
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

void deoglPipeline::AssignRTSPipelineIndex(){
	if( pRTSPipelineIndex == -1 ){
		pRTSPipelineIndex = pNextRTSPipelineIndex++;
	}
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
