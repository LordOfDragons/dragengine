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

#ifndef _DEOGLRENDERTASK_H_
#define _DEOGLRENDERTASK_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"
#include "../../shaders/paramblock/deoglShaderParameterBlockList.h"

class deoglQuickSorter;
class deoglRenderTaskPipeline;
class deoglRenderTaskInstance;
class deoglRenderTaskTexture;
class deoglRenderTaskVAO;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskConfigTexture;
class deoglRenderThread;
class deoglRTLogger;
class deoglSPBlockSSBO;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglPipeline;


/**
 * Render Task.
 *
 * Stores a list of render steps to be carried out. Each step contains the combination of shader,
 * texture and vao required. The render task is usually not used directly for rendering. Instead
 * a batched render task is generated from this list where matching steps are grouped together
 * for optimized rendering.
 */
class deoglRenderTask{
private:
	struct sConfigTexture{
		const deoglPipeline *pipeline;
		const deoglRenderTaskSharedTexture *texture;
		const deoglRenderTaskSharedVAO *vao;
		const deoglRenderTaskSharedInstance *instance;
		deoglRenderTaskPipeline *rtpipeline;
		deoglRenderTaskTexture *rttexture;
		deoglRenderTaskVAO *rtvao;
		deoglRenderTaskInstance *rtinstance;
// 		int pipelineIndex;
// 		int textureIndex;
// 		int vaoIndex;
// 		int instanceIndex;
		int groupIndex;
		int specialFlags;
	};
	
	deoglRenderThread &pRenderThread;
	
	deoglSPBlockUBO *pRenderParamBlock;
	GLuint pTBOInstances;
	deoglShaderParameterBlockList pSPBInstances;
	int pSPBInstanceMaxEntries;
	int pSPBInstanceIndexCount;
	bool pUseSPBInstanceFlags;
	bool pRenderVSStereo;
	GLuint pVBODrawIndirect;
	//int pVBODrawIndirectSize;
	
	decPointerList pPipelines;
	int pPipelineCount;
	deoglRenderTaskPipeline **pHasPipeline;
	int pHasPipelineCount;
	int pHasPipelineSize;
	
	sConfigTexture *pConfigTextures;
	int pConfigTextureCount;
	int pConfigTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task. */
	deoglRenderTask(deoglRenderThread &renderThread);
	
	/** Clean up render task. */
	~deoglRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Clear render task. */
	void Clear();
	
	/** Prepare task to be used for rendering. */
	void PrepareForRender();
	
	/** Sort instances by distance. */
	void SortInstancesByDistance(deoglQuickSorter &sorter,
		const decDVector &position, const decDVector &direction);
	
	/** Render parameter shader parameter block or NULL. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{return pRenderParamBlock;}
	
	/** Set render parameter shader parameter block or NULL to use none. */
	void SetRenderParamBlock(deoglSPBlockUBO *paramBlock);
	
	/** Instances texture buffer object or NULL. */
	inline GLuint GetTBOInstances() const{return pTBOInstances;}
	
	/** Set instances texture buffer object or NULL to use none. */
	void SetTBOInstances(GLuint tbo);
	
	/** Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{return pUseSPBInstanceFlags;}
	
	/** Set use instance flags. */
	void SetUseSPBInstanceFlags(bool useFlags);
	
	/** Use vertex shader stereo rendering. */
	inline bool GetRenderVSStereo() const{return pRenderVSStereo;}
	
	/** Set use vertex shader stereo rendering. */
	void SetRenderVSStereo(bool renderVSStereo);
	
	/**
	 * VBO draw indirect. Content is an array of oglDrawIndirectCommand. This is a union
	 * struct of oglDrawElementsIndirectCommand and oglDrawArraysIndirectCommand allowing
	 * to  use one VBO for both. Use sizeof(oglDrawIndirectCommand) as stride.
	 */
	inline GLuint GetVBODrawIndirect() const{return pVBODrawIndirect;}
	
	
	
	/** Number of pipelines. */
	inline int GetPipelineCount() const{return pPipelineCount;}
	
	/** Pipeline at index. */
	deoglRenderTaskPipeline *GetPipelineAt(int index) const;
	
	/** Add pipeline. */
	deoglRenderTaskPipeline *AddPipeline(const deoglPipeline *pipeline);
	deoglRenderTaskPipeline *AddPipelineDirect(const deoglPipeline *pipeline);
	
	
	
	/** Add configuration texture. */
	void AddConfigTexture(const deoglRenderTaskConfigTexture &texture, int specialFlags);
	
	/** Remove all configuration textures. */
	void RemoveAllConfigTextures();
	
	/** Apply configuration textures. */
	void ApplyConfigTextures();
	
	
	
	/** Number of points in all steps. */
	int GetTotalPointCount() const;
	
	/** Total amount of textures. */
	int GetTotalTextureCount() const;
	
	/** Total amount of vaos. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances. */
	int GetTotalSubInstanceCount() const;
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Debug print. */
	void DebugPrint(deoglRTLogger &rtlogger);
	void DebugSimple(deoglRTLogger &logger);
	/*@}*/
	
	
	
private:
	void pCalcSPBInstancesMaxEntries();
	void pAssignSPBInstances();
	void pUpdateSPBInstances();
	void pCreateSPBInstanceParamBlock();
	void pUpdateVBODrawIndirect();
};

#endif
