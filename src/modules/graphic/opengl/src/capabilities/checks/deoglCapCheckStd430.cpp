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
deoglCapabilities &capabilities) :
pCapabilities(capabilities),
pWorking(false){
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
	const deoglExtensions &ext = renderThread.GetExtensions();
	
	if(ext.GetGLESVersion() < deoglExtensions::evgles3p1
	&& !ext.GetHasExtension(deoglExtensions::ext_ARB_shader_storage_buffer_object)){
		return;
	}
	
	try{
		deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
		
		const deoglShaderSources * const sources = shaderManager.GetSourcesNamed("Test std430 Support");
		DEASSERT_NOTNULL(sources)
		const deoglShaderProgram * const shader = shaderManager.GetProgramWith(sources, deoglShaderDefines());
		DEASSERT_NOTNULL(shader)
		
		pWorking = true;
		
	}catch(const deException &){
		pWorking = false;
	}
	
	if(pWorking){
		renderThread.GetLogger().LogInfo("Capabilities: std430 Layout: Supported");
		
	}else{
		renderThread.GetLogger().LogWarn("Capabilities: std430 Layout: Not Supported!");
	}
}
