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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderParticles.h"
#include "defren/deoglDeferredRendering.h"
#include "deoglRenderGeometry.h"
#include "plan/deoglRenderPlan.h"
#include "task/deoglRenderTaskParticles.h"
#include "task/deoglRenderTaskParticlesStep.h"
#include "../collidelist/deoglCollideList.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FACTOR_EXPONENT		255.0f

enum eSPDepth{
	spdMatrixMV,
	spdMatrixProj,
	spdMaterialGamma,
	spdClipPlane,
	spdViewport,
	spdAttributes1,
	spdPosition
};

enum eSPRender{
	sprMatrixMV,
	sprMatrixProj,
	sprMaterialAmbient,
	sprMaterialDiffuse,
	sprMaterialSpecular,
	sprMaterialEmissive,
	sprMaterialGamma,
	sprViewport,
	sprDistortion,
	sprMatrixEnvMap,
	sprSampleParams,
	sprRibbonSheetCount
};




// Class deoglRenderParticles
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderParticles::deoglRenderParticles(deoglRenderThread &renderThread) : deoglRenderBase(renderThread){
}

deoglRenderParticles::~deoglRenderParticles(){
}



// Rendering
//////////////

void deoglRenderParticles::RenderTaskParticles(const deoglRenderTaskParticles &renderTask){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	const bool renderVSStereo = renderTask.GetRenderVSStereo();
	const int stepCount = renderTask.GetStepCount();
	const deoglVAO *curVAO = NULL;
	int i;
	
	if(stepCount == 0){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace(renderThread, "Particles.RenderTaskParticles");
	
	for(i=0; i<stepCount; i++){
		const deoglRenderTaskParticlesStep &renderTaskStep = *renderTask.GetStepAt(i);
		
		const int instanceCount = renderTaskStep.GetInstanceCount();
		if(instanceCount == 0){
			continue;
		}
		
		renderTaskStep.GetPipeline()->Activate();
		
		if(renderParamBlock){
			renderParamBlock->Activate();
		}
		
		// set texture
		if(renderTaskStep.GetParameterBlockTexture()){
			renderTaskStep.GetParameterBlockTexture()->Activate();
		}
		
		const deoglTexUnitsConfig * const tuc = renderTaskStep.GetTUC();
		if(tuc){
			tuc->Apply();
			if(tuc->GetParameterBlock()){
				tuc->GetParameterBlock()->Activate();
			}
		}
		
		// activate vao
		const deoglVAO * const vao = renderTaskStep.GetVAO();
		if(vao != curVAO){
			pglBindVertexArray(vao->GetVAO());
			curVAO = vao;
		}
		
		// set instance parameter block
		if(renderTaskStep.GetParameterBlockInstance()){
			renderTaskStep.GetParameterBlockInstance()->Activate();
		}
		
		// render instances
		const GLenum indexGLType = vao->GetIndexGLType();
		const int indexSize = vao->GetIndexSize();
		
		//GetOgl().LogInfoFormat( "RenderTaskParticles: s=%i sha=%p tuc=%p vao=%i ic=%i",
		//	s, renderTaskStep.GetShader(), renderTaskStep.GetTUC(), vao->GetVAO(),
		//	renderTaskStep.GetIndexCount() );
		if(renderVSStereo){
			const GLsizei count[2] = {renderTaskStep.GetIndexCount(), renderTaskStep.GetIndexCount()};
			const void * const indices[2] = {
				(void*)(intptr_t)(indexSize * renderTaskStep.GetFirstIndex()),
				(void*)(intptr_t)(indexSize * renderTaskStep.GetFirstIndex())};
			OGL_CHECK(renderThread, pglMultiDrawElements(renderTaskStep.GetPrimitiveType(), count, indexGLType, indices, 2));
			
		}else{
			OGL_CHECK(renderThread, glDrawElements(renderTaskStep.GetPrimitiveType(),
				renderTaskStep.GetIndexCount(), indexGLType,
				(GLvoid*)(intptr_t)(indexSize * renderTaskStep.GetFirstIndex())));
		}
	}
	
	pglBindVertexArray(0);
}
