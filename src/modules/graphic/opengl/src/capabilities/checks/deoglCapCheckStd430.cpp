/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
#include <stdlib.h>
#include <string.h>

#include "deoglCapCheckStd430.h"
#include "../deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shaders/paramblock/deoglSPBMapBufferRead.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckStd430
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckStd430::deoglCapCheckStd430(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( false ){
}



// Management
///////////////

void deoglCapCheckStd430::Check(){
	// Summary:
	//    std430 is an improved GLSL buffer layout. compared to std140 it especially removes
	//    certain limitations to allow better packing, namely arrays of floats are no more
	//    aligned to 16-byte boundaries. This check checks if std430 is supported.
	//    
	// Background:
	//    Basically std430 means OpenGL Core 4.3 or higher. But compute shaders can be supported
	//    also on OpenGL 3.3 type hardware. Unfortunately there is no extension that can be
	//    checked for std430 to be supported. The only safe way is to use it only if a 4.3
	//    core context can be created. This though rules out lower hardware so a capability
	//    check is used instead.
	//    
	// Test Procedure:
	//    A shader with std430 layout is tried to be compiled and linked. If linking throws
	//    no error std430 is supported otherwise not.
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	if( ! renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_compute_shader )
	|| ! renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
		return;
	}
	
	try{
		deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
		
		const deoglShaderSources * const sources = shaderManager.GetSourcesNamed( "Test std430 Support" );
		DEASSERT_NOTNULL( sources )
		const deoglShaderProgram * const shader = shaderManager.GetProgramWith( sources, deoglShaderDefines() );
		DEASSERT_NOTNULL( shader )
		
		pWorking = true;
		
	}catch( const deException & ){
		pWorking = false;
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: std430 Layout: Supported" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: std430 Layout: Not Supported!" );
	}
}
