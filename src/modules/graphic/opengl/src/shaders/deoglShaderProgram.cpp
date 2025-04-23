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
#include "deoglShaderProgramUnit.h"
#include "deoglShaderSources.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>


// Class deoglShaderProgram
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderProgram::deoglShaderProgram(deoglRenderThread &renderThread,
	const deoglShaderSources *sources) :
pRenderThread(renderThread),
pSources(sources),
pUnitCompute(nullptr),
pUnitTessellationControl(nullptr),
pUnitTessellationEvaluation(nullptr),
pUnitGeometry(nullptr),
pUnitVertex(nullptr),
pUnitFragment(nullptr),
pCompiled(nullptr),
ready(false),
isCompiling(false)
{
	DEASSERT_NOTNULL(sources)
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglShaderProgram::deoglShaderProgram(deoglRenderThread &renderThread,
const deoglShaderSources *sources, const deoglShaderDefines &defines) :
pRenderThread(renderThread),
pSources(sources),
pDefines(defines),
pUnitCompute(nullptr),
pUnitTessellationControl(nullptr),
pUnitTessellationEvaluation(nullptr),
pUnitGeometry(nullptr),
pUnitVertex(nullptr),
pUnitFragment(nullptr),
pCompiled(nullptr),
ready(false),
isCompiling(false)
{
	DEASSERT_NOTNULL(sources)
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglShaderProgram::~deoglShaderProgram(){
	if(pCompiled){
		delete pCompiled;
	}
	pRenderThread.GetUniqueKey().Return(pUniqueKey);
}



// Management
///////////////

void deoglShaderProgram::SetCacheId(const decString &id){
	pCacheId = id;
}

void deoglShaderProgram::SetUnitCompute(deoglShaderProgramUnit *unit){
	pUnitCompute = unit;
}

void deoglShaderProgram::SetUnitTessellationControl(deoglShaderProgramUnit *unit){
	pUnitTessellationControl = unit;
}

void deoglShaderProgram::SetUnitTessellationEvaluation(deoglShaderProgramUnit *unit){
	pUnitTessellationEvaluation = unit;
}

void deoglShaderProgram::SetUnitGeometry(deoglShaderProgramUnit *unit){
	pUnitGeometry = unit;
}

void deoglShaderProgram::SetUnitVertex(deoglShaderProgramUnit *unit){
	pUnitVertex = unit;
}

void deoglShaderProgram::SetUnitFragment(deoglShaderProgramUnit *unit){
	pUnitFragment = unit;
}

bool deoglShaderProgram::CanCompile() const{
	return !(pUnitCompute && pUnitCompute->isCompiling)
		&& !(pUnitVertex && pUnitVertex->isCompiling)
		&& !(pUnitGeometry && pUnitGeometry->isCompiling)
		&& !(pUnitFragment && pUnitFragment->isCompiling)
		&& !(pUnitTessellationControl && pUnitTessellationControl->isCompiling)
		&& !(pUnitTessellationEvaluation && pUnitTessellationEvaluation->isCompiling);
}

void deoglShaderProgram::SetCompiled(deoglShaderCompiled *compiled){
	if(compiled == pCompiled){
		return;
	}
	
	if(pCompiled){
		delete pCompiled;
	}
	pCompiled = compiled;
}

void deoglShaderProgram::MoveCompiled(deoglShaderProgram &program){
	if(&program == this){
		return;
	}
	
	SetCompiled(nullptr);
	pCompiled = program.pCompiled;
	program.pCompiled = nullptr;
}
