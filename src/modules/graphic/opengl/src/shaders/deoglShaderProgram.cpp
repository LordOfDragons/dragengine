/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include "deoglShaderCompiled.h"
#include "deoglShaderProgram.h"
#include "deoglShaderUnitSourceCode.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderProgram
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderProgram::deoglShaderProgram( deoglRenderThread &renderThread, deoglShaderSources *sources ) :
pRenderThread( renderThread )
{
	if( ! sources ){
		DETHROW( deeInvalidParam );
	}
	
	pSCCompute = NULL;
	pSCTessellationControl = NULL;
	pSCTessellationEvaluation = NULL;
	pSCGeometry = NULL;
	pSCVertex = NULL;
	pSCFragment = NULL;
	
	pCompiled = NULL;
	pSources = sources;
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
	
	pUsageCount = 1;
}

deoglShaderProgram::deoglShaderProgram( deoglRenderThread &renderThread,
deoglShaderSources *sources, const deoglShaderDefines &defines ) :
pRenderThread( renderThread )
{
	if( ! sources ){
		DETHROW( deeInvalidParam );
	}
	
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
	
	pUsageCount = 1;
}

deoglShaderProgram::~deoglShaderProgram(){
	if( pCompiled ){
		delete pCompiled;
	}
	
	pRenderThread.GetUniqueKey().Return( pUniqueKey );
}



// Management
///////////////

void deoglShaderProgram::SetComputeCode( deoglShaderUnitSourceCode *sourceCode ){
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
	if( compiled != pCompiled ){
		if( pCompiled ) delete pCompiled;
		
		pCompiled = compiled;
	}
}



void deoglShaderProgram::AddUsage(){
	pUsageCount++;
}

void deoglShaderProgram::RemoveUsage(){
	if( pUsageCount == 0 ){
		//DETHROW( deeInvalidParam );
		// this is bad. we let it slip though and let the shader manager notify about what happened the next possible time
	}
	
	pUsageCount--;
}
