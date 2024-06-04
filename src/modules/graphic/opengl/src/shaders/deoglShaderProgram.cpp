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

#include "deoglShaderCompiled.h"
#include "deoglShaderProgram.h"
#include "deoglShaderSources.h"
#include "deoglShaderUnitSourceCode.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Class deoglShaderProgram
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderProgram::deoglShaderProgram( deoglRenderThread &renderThread, const deoglShaderSources *sources ) :
pRenderThread( renderThread )
{
	DEASSERT_NOTNULL( sources )
	
	pSCCompute = NULL;
	pSCTessellationControl = NULL;
	pSCTessellationEvaluation = NULL;
	pSCGeometry = NULL;
	pSCVertex = NULL;
	pSCFragment = NULL;
	
	pCompiled = NULL;
	pSources = sources;
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglShaderProgram::deoglShaderProgram( deoglRenderThread &renderThread,
const deoglShaderSources *sources, const deoglShaderDefines &defines ) :
pRenderThread( renderThread )
{
	DEASSERT_NOTNULL( sources )
	
	pSCCompute = NULL;
	pSCTessellationControl = NULL;
	pSCTessellationEvaluation = NULL;
	pSCGeometry = NULL;
	pSCVertex = NULL;
	pSCFragment = NULL;
	
	pCompiled = NULL;
	pSources = sources;
	
	pDefines = defines;
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglShaderProgram::~deoglShaderProgram(){
	if( pCompiled ){
		delete pCompiled;
	}
	
	pRenderThread.GetUniqueKey().Return( pUniqueKey );
}



// Management
///////////////

void deoglShaderProgram::SetCacheId( const decString &id ){
	pCacheId = id;
}

void deoglShaderProgram::SetComputeSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCCompute = sourceCode;
}

void deoglShaderProgram::SetTessellationControlSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCTessellationControl = sourceCode;
}

void deoglShaderProgram::SetTessellationEvaluationSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCTessellationEvaluation = sourceCode;
}

void deoglShaderProgram::SetGeometrySourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCGeometry = sourceCode;
}

void deoglShaderProgram::SetVertexSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCVertex = sourceCode;
}

void deoglShaderProgram::SetFragmentSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	pSCFragment = sourceCode;
}

void deoglShaderProgram::SetCompiled( deoglShaderCompiled *compiled ){
	if( compiled == pCompiled ){
		return;
	}
	
	if( pCompiled ){
		delete pCompiled;
	}
	
	pCompiled = compiled;
}
