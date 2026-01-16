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

#include "deoglRenderTask.h"
#include "deoglRenderTaskPipeline.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "config/deoglRenderTaskConfigTexture.h"
#include "shared/deoglRenderTaskSharedTexture.h"
#include "shared/deoglRenderTaskSharedVAO.h"
#include "shared/deoglRenderTaskSharedInstance.h"
#include "shared/deoglRenderTaskSharedPool.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderProgramUnit.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../utils/deoglQuickSorter.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglRenderTask
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTask::deoglRenderTask(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pRenderParamBlock(nullptr),
pTBOInstances(0),
pSPBInstanceMaxEntries(0),
pSPBInstanceIndexCount(0),
pUseSPBInstanceFlags(false),
pRenderVSStereo(false),
pVBODrawIndirect(0),
//pVBODrawIndirectSize( 0 ),
pPipelineCount(0),

pConfigTextures(nullptr),
pConfigTextureCount(0),
pConfigTextureSize(0){
}

deoglRenderTask::~deoglRenderTask(){
	pPipelines.RemoveAll();
	
	if(pConfigTextures){
		delete [] pConfigTextures;
	}
	
	pRenderThread.GetDelayedOperations().DeleteOpenGLBuffer(pVBODrawIndirect);
}



// Management
///////////////

void deoglRenderTask::Clear(){
	RemoveAllConfigTextures();
	pHasPipeline.RemoveAll();
	pPipelineCount = 0;
	
	SetRenderParamBlock(nullptr);
	SetTBOInstances(0);
	pUseSPBInstanceFlags = false;
	pRenderVSStereo = false;
}

void deoglRenderTask::PrepareForRender(){
	if(pPipelineCount == 0){
		return;
	}
	
	pCalcSPBInstancesMaxEntries();
	pAssignSPBInstances();
	pUpdateSPBInstances();
	
	if(pRenderVSStereo){
		pUpdateVBODrawIndirect();
	}
}

void deoglRenderTask::SortInstancesByDistance(deoglQuickSorter &sorter,
const decDVector &position, const decDVector &direction){
	const double posDotDir = position * direction;
	
	pPipelines.Visit([&](const deoglRenderTaskPipeline &pipeline){
		pipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &texture){
			texture.GetVAOs().Visit([&](deoglRenderTaskVAO &vao){
				vao.SortInstancesByDistance(sorter, position, direction, posDotDir);
			}, 0, texture.GetVAOCount());
		}, 0, pipeline.GetTextureCount());
	}, 0, pPipelineCount);
}

void deoglRenderTask::SetRenderParamBlock(deoglSPBlockUBO *paramBlock){
	pRenderParamBlock = paramBlock;
}

void deoglRenderTask::SetTBOInstances(GLuint tbo){
	pTBOInstances = tbo;
}

void deoglRenderTask::SetUseSPBInstanceFlags(bool useFlags){
	pUseSPBInstanceFlags = useFlags;
}

void deoglRenderTask::SetRenderVSStereo(bool renderVSStereo){
	pRenderVSStereo = renderVSStereo;
}



deoglRenderTaskPipeline *deoglRenderTask::GetPipelineAt(int index) const{
	return pPipelines.GetAt(index);
}

deoglRenderTaskPipeline *deoglRenderTask::AddPipeline(const deoglPipeline *pipeline){
	DEASSERT_NOTNULL(pipeline)
	
	const int index = pipeline->GetRTSIndex();
	DEASSERT_TRUE(index >= 0)
	
	while(index >= pHasPipeline.GetCount()){
		pHasPipeline.Add(nullptr);
	}
	
	deoglRenderTaskPipeline *rtpipeline = pHasPipeline.GetAt(index);
	if(rtpipeline){
		return rtpipeline;
	}
	
	if(pPipelineCount == pPipelines.GetCount()){
		pPipelines.Add(deoglRenderTaskPipeline::Ref::New());
		rtpipeline = pPipelines.Last();
		
	}else{
		rtpipeline = pPipelines.GetAt(pPipelineCount);
		rtpipeline->Reset();
	}
	pPipelineCount++;
	
	rtpipeline->SetPipeline(pipeline);
	pHasPipeline.SetAt(index, rtpipeline);
	return rtpipeline;
}

deoglRenderTaskPipeline *deoglRenderTask::AddPipelineDirect(const deoglPipeline *pipeline){
	deoglRenderTaskPipeline *rtpipeline;
	if(pPipelineCount == pPipelines.GetCount()){
		pPipelines.Add(deoglRenderTaskPipeline::Ref::New());
		rtpipeline = pPipelines.Last();
		
	}else{
		rtpipeline = pPipelines.GetAt(pPipelineCount);
		rtpipeline->Reset();
	}
	pPipelineCount++;
	
	rtpipeline->SetPipeline(pipeline);
	return rtpipeline;
}



void deoglRenderTask::AddConfigTexture(const deoglRenderTaskConfigTexture &texture, int specialFlags){
	if(pConfigTextureCount == pConfigTextureSize){
		const int newSize = pConfigTextureCount * 3 / 2 + 1;
		sConfigTexture * const newArray = new sConfigTexture[newSize];
		if(pConfigTextures){
			memcpy(newArray, pConfigTextures, sizeof(sConfigTexture) * pConfigTextureCount);
			delete [] pConfigTextures;
		}
		pConfigTextures = newArray;
		pConfigTextureSize = newSize;
	}
	
	sConfigTexture &ct = pConfigTextures[pConfigTextureCount++];
	ct.pipeline = texture.GetPipeline();
	ct.texture = texture.GetTexture();
	ct.vao = texture.GetVAO();
	ct.instance = texture.GetInstance();
	ct.rtpipeline = nullptr;
	ct.rttexture = nullptr;
	ct.rtvao = nullptr;
	ct.rtinstance = nullptr;
// 	ct.shaderIndex = texture.GetShaderIndex();
// 	ct.textureIndex = texture.GetTextureIndex();
// 	ct.vaoIndex = texture.GetVAOIndex();
// 	ct.instanceIndex = texture.GetInstanceIndex();
	ct.groupIndex = texture.GetGroupIndex();
	ct.specialFlags = specialFlags;
}

void deoglRenderTask::RemoveAllConfigTextures(){
	pConfigTextureCount = 0;
}

void deoglRenderTask::ApplyConfigTextures(){
#if 0
	/*
	int i;
	for(i=0; i<pConfigTextureCount; i++){
		const sConfigTexture &ct = pConfigTextures[i];
		AddShader(ct.shader, ct.shaderIndex)->AddTexture(ct.texture, ct.textureIndex)->
			AddVAO(ct.vao, ct.vaoIndex)->AddInstance(ct.instance, ct.instanceIndex)->
			AddSubInstance(ct.groupIndex, ct.specialFlags);
	}
	*/
	
	// shader
	int i, j;
	for(i=0; i<pConfigTextureCount;){
		const deoglRenderTaskSharedShader *shader = pConfigTextures[i].shader;
		deoglRenderTaskShader * const rtshader = AddShaderDirect(shader);
		pConfigTextures[i].rtshader = rtshader;
		
		for(j=i+1; j<pConfigTextureCount; j++){
			if(pConfigTextures[j].shader == shader){
				pConfigTextures[j].rtshader = rtshader;
			}
		}
		
		for(i++; i<pConfigTextureCount && !pConfigTextures[i].rtshader; i++);
	}
	
	// texture
	int p;
	for(p=0; p<pPipelineCount; p++){
		deoglRenderTaskPipeline * const rtpipeline = pPipelines[p];
		
		for(i=0; i<pConfigTextureCount;){
			if(pConfigTextures[i].pipeline != 
			const deoglRenderTaskSharedTexture *texture = pConfigTextures[i].texture;
			pConfigTextures[i].marker = true;
			AddShaderDirect(shader);
			
			for(j=i+1; j<pConfigTextureCount; j++){
				if(pConfigTextures[j].shader == shader){
					pConfigTextures[j].marker = true;
				}
			}
			
			for(i++; i<pConfigTextureCount && pConfigTextures[i].marker; i++);
		}
	}
#endif
}


int deoglRenderTask::GetTotalPointCount() const{
	return pPipelines.Inject(0, [](int sum, const deoglRenderTaskPipeline &pipeline){
		return sum + pipeline.GetTotalPointCount();
	}, 0, pPipelineCount);
}

int deoglRenderTask::GetTotalTextureCount() const{
	return pPipelines.Inject(0, [](int sum, const deoglRenderTaskPipeline &pipeline){
		return sum + pipeline.GetTextureCount();
	}, 0, pPipelineCount);
}

int deoglRenderTask::GetTotalVAOCount() const{
	return pPipelines.Inject(0, [](int sum, const deoglRenderTaskPipeline &pipeline){
		return sum + pipeline.GetTotalVAOCount();
	}, 0, pPipelineCount);
}

int deoglRenderTask::GetTotalInstanceCount() const{
	return pPipelines.Inject(0, [](int sum, const deoglRenderTaskPipeline &pipeline){
		return sum + pipeline.GetTotalInstanceCount();
	}, 0, pPipelineCount);
}

int deoglRenderTask::GetTotalSubInstanceCount() const{
	return pPipelines.Inject(0, [](int sum, const deoglRenderTaskPipeline &pipeline){
		return sum + pipeline.GetTotalSubInstanceCount();
	}, 0, pPipelineCount);
}



// Debugging
//////////////

void deoglRenderTask::DebugPrint(deoglRTLogger &rtlogger){
	const bool detailsInstances = true; //false;
	int i, u, d, sic;
	decString text;
	
	rtlogger.LogInfoFormat("RenderTask %p: spb=%p pipelines=%d points=%d textures=%d",
		this, pRenderParamBlock, pHasPipeline.GetCount(), GetTotalPointCount(), GetTotalTextureCount());
	
	pPipelines.VisitIndexed([&](int s, const deoglRenderTaskPipeline &pipeline){
		const deoglShaderProgram &shader = pipeline.GetPipeline()->GetConfiguration().GetShaderRef();
		const deoglShaderDefines &defines = shader.GetDefines();
		
		rtlogger.LogInfoFormat("- pipeline %d: shader=%p textures=%d points=%d vaos=%d "
			"instances=%d subInstances=%d", s, &shader, pipeline.GetTextureCount(),
			pipeline.GetTotalPointCount(), pipeline.GetTotalVAOCount(),
			pipeline.GetTotalInstanceCount(), pipeline.GetTotalSubInstanceCount());
		
		rtlogger.LogInfo("  - configuration:");
		rtlogger.LogInfoFormat("    - vertex %s", shader.GetUnitVertex()
			? shader.GetUnitVertex()->GetSources()->GetName().GetString() : "-");
		rtlogger.LogInfoFormat("    - geometry %s", shader.GetUnitGeometry()
			? shader.GetUnitGeometry()->GetSources()->GetName().GetString() : "-");
		rtlogger.LogInfoFormat("    - fragment %s", shader.GetUnitFragment()
			? shader.GetUnitFragment()->GetSources()->GetName().GetString() : "-");
		
		text = "    - defines: ";
		const int defineCount = defines.GetDefineCount();
		for(d=0; d<defineCount; d++){
			const char *defineName = defines.GetDefineNameAt(d);
			const char *defineValue = defines.GetDefineValueAt(d);
			
			if(strlen(defineValue) > 10){
				text.AppendFormat("%s %s=%.10s...", d == 0 ? "" : ",", defineName, defineValue);
				
			}else{
				text.AppendFormat("%s %s=%s", d == 0 ? "" : ",", defineName, defineValue);
			}
		}
		rtlogger.LogInfo(text.GetString());
		
		pipeline.GetTextures().VisitIndexed([&](int t, const deoglRenderTaskTexture &rttexture){
			rtlogger.LogInfoFormat("  - texture %i: tuc=%p texture=%p vaos=%i "
				"points=%i instances=%i subInstances=%i", t, rttexture.GetTexture()->GetTUC(),
				rttexture.GetTexture(), rttexture.GetVAOCount(), rttexture.GetTotalPointCount(),
				rttexture.GetTotalInstanceCount(), rttexture.GetTotalSubInstanceCount());
			
			const int unitCount = rttexture.GetTexture()->GetTUC()->GetUnitCount();
			text.Format("    units(");
			for(u=0; u<unitCount; u++){
				const deoglTexUnitConfig &unit = rttexture.GetTexture()->GetTUC()->GetUnitAt(u);
				if(unit.GetTexture()){
					text.AppendFormat(" T%i", unit.GetTexture()->GetTexture());
					
				}else if(unit.GetCubeMap()){
					text.AppendFormat(" C%i", unit.GetCubeMap()->GetTexture());
					
				}else if(unit.GetSpecial() != deoglTexUnitConfig::estNone){
					text.AppendFormat(" S%i", unit.GetSpecial());
					
				}else{
					text.AppendFormat(" -");
				}
			}
			text.AppendFormat(")");
			rtlogger.LogInfo(text.GetString());
			
			rttexture.GetVAOs().VisitIndexed([&](int v, const deoglRenderTaskVAO &rtvao){
				sic = rtvao.GetTotalSubInstanceCount();
				
				rtlogger.LogInfoFormat("    - vao %i: vao=%i instances=%i points=%i "
					"subInstances=%i", v, rtvao.GetVAO()->GetVAO()->GetVAO(),
					rtvao.GetInstanceCount(), rtvao.GetTotalPointCount(), sic);
				
				if(detailsInstances){
					const int instanceCount = rtvao.GetInstanceCount();
					for(i=0; i<instanceCount; i++){
						const deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt(i);
						const deoglRenderTaskSharedInstance &instance = *rtinstance.GetInstance();
						if(instance.GetSubInstanceSPB()){
							rtlogger.LogInfoFormat("        - instance %i: fp=%i pc=%i "
								"fi=%i ic=%i sic=%i sispbi=%p sispbfi=%d", i,
								instance.GetFirstPoint(), instance.GetPointCount(),
								instance.GetFirstIndex(), instance.GetIndexCount(),
								instance.GetSubInstanceCount(), rtinstance.GetSIIndexInstanceSPB(),
								rtinstance.GetSIIndexInstanceFirst());
						}
					}
				}
			}, 0, rttexture.GetVAOCount());
		}, 0, pipeline.GetTextureCount());
	}, 0, pPipelineCount);
}

void deoglRenderTask::DebugSimple(deoglRTLogger &logger){
	int i, s, step = 0;
	decString text;
	
	logger.LogInfoFormat("RenderTask %p", this);
	
	pPipelines.Visit([&](const deoglRenderTaskPipeline &pipeline){
		const int pi = pipeline.GetPipeline()->GetRTSIndex();
		pipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &texture){
			const int ti = texture.GetTexture()->GetIndex();
			texture.GetVAOs().Visit([&](const deoglRenderTaskVAO &vao){
				const int vi = vao.GetVAO()->GetIndex();
				const int instanceCount = vao.GetInstanceCount();
				for(i=0; i<instanceCount; i++){
					const deoglRenderTaskInstance &instance = *vao.GetInstanceAt(i);
					const int ii = instance.GetInstance()->GetIndex();
					const int subInstanceCount = instance.GetSubInstanceCount();
					text.Format("p=%d t=%d v=%d i=%d [pc=%d fp=%d ic=%d fi=%d]", pi, ti, vi, ii,
						instance.GetInstance()->GetPointCount(), instance.GetInstance()->GetFirstPoint(),
						instance.GetInstance()->GetIndexCount(), instance.GetInstance()->GetFirstIndex());
					for(s=0; s<subInstanceCount; s++){
						const deoglRenderTaskInstance::sSubInstance &si = instance.GetSubInstanceAt(s);
						logger.LogInfoFormat("- %d: %s si[i=%d f=%x]", step++, text.GetString(), si.instance, si.flags);
					}
					if(subInstanceCount == 0){
						logger.LogInfoFormat("- %d: %s", step++, text.GetString());
					}
				}
			}, 0, texture.GetVAOCount());
		}, 0, pipeline.GetTextureCount());
	}, 0, pPipelineCount);
}



// Private Functions
//////////////////////

void deoglRenderTask::pCalcSPBInstancesMaxEntries(){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4].
	//pSPBInstanceMaxEntries = ( renderThread.GetCapabilities().GetUBOMaxSize() / 16 ) * 4;
	pSPBInstanceMaxEntries = pRenderThread.GetBufferObject().GetInstanceArraySizeUBO();
	
	if(pUseSPBInstanceFlags){
		// if instance flags are used the vector <instanceIndex, instanceFlags> is
		// used instead. this reduces the max entries count by factor 2 but allows
		// to transport a scratch flags value in a fast way
		pSPBInstanceMaxEntries /= 2;
	}
}

void deoglRenderTask::pAssignSPBInstances(){
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		if(pSPBInstances.GetCount() == 0){
			pCreateSPBInstanceParamBlock();
		}
		
		deoglShaderParameterBlock * const paramBlock = pSPBInstances.GetAt(0);
		int l;
		
		pSPBInstanceIndexCount = 0;
		
		pPipelines.Visit([&](const deoglRenderTaskPipeline &rtpipeline){
			rtpipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &rttexture){
				rttexture.GetVAOs().Visit([&](deoglRenderTaskVAO &rtvao){
					const int instanceCount = rtvao.GetInstanceCount();
					
					for(l=0; l<instanceCount; l++){
						deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt(l);
						rtinstance.SetSIIndexInstanceParam(paramBlock, pSPBInstanceIndexCount);
						pSPBInstanceIndexCount += rtinstance.GetSubInstanceCount();
					}
				}, 0, rttexture.GetVAOCount());
			}, 0, rtpipeline.GetTextureCount());
		}, 0, pPipelineCount);
		
		if(pSPBInstanceIndexCount > paramBlock->GetElementCount()){
			paramBlock->SetElementCount(pSPBInstanceIndexCount);
		}
		
	}else{
		const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
		deoglShaderParameterBlock *paramBlock = nullptr;
		int paramBlockCount = 0;
		int firstIndex = 0;
		int l;
		
		pPipelines.Visit([&](const deoglRenderTaskPipeline &rtpipeline){
			rtpipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &rttexture){
				rttexture.GetVAOs().Visit([&](deoglRenderTaskVAO &rtvao){;
					const int instanceCount = rtvao.GetInstanceCount();
					
					for(l=0; l<instanceCount; l++){
						deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt(l);
						
						if(!paramBlock || firstIndex + rtinstance.GetSubInstanceCount() > pSPBInstanceMaxEntries){
							if(paramBlock){
								const int ecount = componentsPerIndex * decMath::max(((firstIndex - 1) / 4) + 1, 1);
								if(ecount > paramBlock->GetElementCount()){
									paramBlock->SetElementCount(ecount);
								}
							}
							
							if(paramBlockCount == pSPBInstances.GetCount()){
								pCreateSPBInstanceParamBlock();
							}
							paramBlock = pSPBInstances.GetAt(paramBlockCount++);
							firstIndex = 0;
						}
						
						rtinstance.SetSIIndexInstanceParam(paramBlock, firstIndex);
						firstIndex += rtinstance.GetSubInstanceCount();
					}
				}, 0, rttexture.GetVAOCount());
			}, 0, rtpipeline.GetTextureCount());
		}, 0, pPipelineCount);
		
		if(paramBlock){
			const int ecount = componentsPerIndex * decMath::max(((firstIndex - 1) / 4) + 1, 1);
			if(ecount > paramBlock->GetElementCount()){
				paramBlock->SetElementCount(ecount);
			}
		}
	}
}

void deoglRenderTask::pUpdateSPBInstances(){
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		if(pSPBInstanceIndexCount == 0){
			return;
		}
		
		const deoglSPBMapBuffer mapped(*pSPBInstances.GetAt(0), 0, pSPBInstanceIndexCount);
		int l;
		
		pPipelines.Visit([&](const deoglRenderTaskPipeline &pipeline){
			pipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &texture){
				texture.GetVAOs().Visit([&](deoglRenderTaskVAO &vao){
					const int instanceCount = vao.GetInstanceCount();
					
					for(l=0; l<instanceCount; l++){
						vao.GetInstanceAt(l)->WriteSIIndexInstanceCompute();
					}
				}, 0, texture.GetVAOCount());
			}, 0, pipeline.GetTextureCount());
		}, 0, pPipelineCount);
		
	}else{
		deoglShaderParameterBlock *paramBlock = nullptr;
		int l;
		
		try{
			pPipelines.Visit([&](const deoglRenderTaskPipeline &pipeline){
				pipeline.GetTextures().Visit([&](const deoglRenderTaskTexture &texture){
					texture.GetVAOs().Visit([&](deoglRenderTaskVAO &vao){;
						const int instanceCount = vao.GetInstanceCount();
						
						for(l=0; l<instanceCount; l++){
							deoglRenderTaskInstance &instance = *vao.GetInstanceAt(l);
							
							if(instance.GetSIIndexInstanceSPB() != paramBlock){
								if(paramBlock){
									paramBlock->UnmapBuffer();
									paramBlock = nullptr;
								}
								
								instance.GetSIIndexInstanceSPB()->MapBuffer();
								paramBlock = instance.GetSIIndexInstanceSPB();
							}
							
							instance.WriteSIIndexInstanceInt(pUseSPBInstanceFlags);
						}
					}, 0, texture.GetVAOCount());
				}, 0, pipeline.GetTextureCount());
			}, 0, pPipelineCount);
			
			if(paramBlock){
				paramBlock->UnmapBuffer();
			}
			
		}catch(const deException &){
			if(paramBlock){
				paramBlock->UnmapBuffer();
			}
			throw;
		}
	}
}

void deoglRenderTask::pCreateSPBInstanceParamBlock(){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4]
	if(pRenderThread.GetChoices().GetUseComputeRenderTask()){
		const deoglSPBlockSSBO::Ref ssbo(deoglSPBlockSSBO::Ref::New(pRenderThread, deoglSPBlockSSBO::etStream));
		ssbo->SetRowMajor(pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
		ssbo->SetParameterCount(2); // just 8 components in total
		ssbo->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
		ssbo->GetParameterAt(1).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
		ssbo->MapToStd140();
		ssbo->SetBindingPoint(deoglSkinShader::essboInstanceIndex);
		ssbo->EnsureBuffer();
		pSPBInstances.Add(ssbo);
		
	}else{
		const deoglSPBlockUBO::Ref ubo(deoglSPBlockUBO::Ref::New(pRenderThread));
		ubo->SetRowMajor(pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
		ubo->SetParameterCount(1);
		ubo->GetParameterAt(0).SetAll(deoglSPBParameter::evtInt, 4, 1, 1);
		ubo->MapToStd140();
		ubo->SetBindingPoint(deoglSkinShader::eubInstanceIndex);
		pSPBInstances.Add(ubo);
	}
}

void deoglRenderTask::pUpdateVBODrawIndirect(){
	// this is not working. calls to pglMultiDrawArraysIndirect and pglMultiDrawElementsIndirect
	// are 2x slower than calling the non-indirect counter parts. this is unusable
#if 0
	const int drawCallCount = GetTotalInstanceCount() * 2;
	if(drawCallCount == 0){
		return;
	}

	if(!pVBODrawIndirect){
		OGL_CHECK(pRenderThread, pglGenBuffers(1, &pVBODrawIndirect));
		DEASSERT_NOTNULL(pVBODrawIndirect);
	}
	
	int i, j, k, l;
	try{
		const int bufferSize = sizeof(oglDrawIndirectCommand) * drawCallCount;
		
		OGL_CHECK(pRenderThread, pglBindBuffer(GL_DRAW_INDIRECT_BUFFER, pVBODrawIndirect));
		
		if(drawCallCount > pVBODrawIndirectSize){
			OGL_CHECK(pRenderThread, pglBufferData(GL_DRAW_INDIRECT_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW));
			pVBODrawIndirectSize = drawCallCount;
		}
		
		oglDrawIndirectCommand *drawCalls;
		OGL_CHECK(pRenderThread, drawCalls = (oglDrawIndirectCommand*)pglMapBufferRange(
			GL_DRAW_INDIRECT_BUFFER, 0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
		DEASSERT_NOTNULL(drawCalls);
		
		int drawCallIndex = 0;
		for(i=0; i<pPipelineCount; i++){
			const deoglRenderTaskPipeline &pipeline = *pPipelines.GetAt(i);
			const int textureCount = pipeline.GetTextureCount();
			
			for(j=0; j<textureCount; j++){
				const deoglRenderTaskTexture &texture = *pipeline.GetTextureAt(j);
				const int vaoCount = texture.GetVAOCount();
				
				for(k=0; k<vaoCount; k++){
					const deoglRenderTaskVAO &vao = *texture.GetVAOAt(k);
					const int instanceCount = vao.GetInstanceCount();
					
					for(l=0; l<instanceCount; l++){
						deoglRenderTaskInstance &instance = *vao.GetInstanceAt(l);
						const deoglRenderTaskSharedInstance &sharedInstance = *instance.GetInstance();
						const int subInstanceCount = instance.GetSubInstanceCount() + sharedInstance.GetSubInstanceCount();
						
						instance.SetDrawIndirectIndex(drawCallIndex);
						instance.SetDrawIndirectCount(2);
						
						if(sharedInstance.GetIndexCount() == 0){
							oglDrawIndirectCommand::Array &draw = drawCalls[drawCallIndex++].array;
							draw.count = sharedInstance.GetPointCount();
							draw.instanceCount = subInstanceCount;
							draw.first = sharedInstance.GetFirstPoint();
							draw.baseInstance = 0;
							
							drawCalls[drawCallIndex++].array = draw;
							
						}else{
							oglDrawIndirectCommand::Element &draw = drawCalls[drawCallIndex++].element;
							draw.count = sharedInstance.GetIndexCount();
							draw.instanceCount = subInstanceCount;
							draw.firstIndex = sharedInstance.GetFirstIndex();
							
							if(pRenderThread.GetChoices().GetSharedVBOUseBaseVertex()){
								draw.baseVertex = sharedInstance.GetFirstPoint();
								
							}else{
								draw.baseVertex = 0;
							}
							
							draw.baseInstance = 0;
							
							drawCalls[drawCallIndex++].element = draw;
						}
					}
				}
			}
		}
		
		pglUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		pglBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		
	}catch(const deException &){
		pglUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
		pglBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		throw;
	}
#endif
}
