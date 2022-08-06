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

#ifndef _DEOGLRENDERTASK_H_
#define _DEOGLRENDERTASK_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"
#include "../../shaders/paramblock/deoglShaderParameterBlockList.h"

class deoglQuickSorter;
class deoglRenderTaskInstance;
class deoglRenderTaskShader;
class deoglRenderTaskTexture;
class deoglRenderTaskVAO;
class deoglRenderTaskSharedShader;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskConfigTexture;
class deoglRenderThread;
class deoglRTLogger;
class deoglSPBlockSSBO;
class deoglSPBlockUBO;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;


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
		const deoglRenderTaskSharedShader *shader;
		const deoglRenderTaskSharedTexture *texture;
		const deoglRenderTaskSharedVAO *vao;
		const deoglRenderTaskSharedInstance *instance;
		deoglRenderTaskShader *rtshader;
		deoglRenderTaskTexture *rttexture;
		deoglRenderTaskVAO *rtvao;
		deoglRenderTaskInstance *rtinstance;
// 		int shaderIndex;
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
	bool pUseSPBInstanceFlags;
	bool pForceDoubleSided;
	bool pRenderVSStereo;
	GLuint pVBODrawIndirect;
	int pVBODrawIndirectSize;
	
	decPointerList pShaders;
	int pShaderCount;
	deoglRenderTaskShader **pHasShader;
	int pHasShaderCount;
	int pHasShaderSize;
	
	sConfigTexture *pConfigTextures;
	int pConfigTextureCount;
	int pConfigTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task. */
	deoglRenderTask( deoglRenderThread &renderThread );
	
	/** Clean up render task. */
	~deoglRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Clear render task. */
	void Clear();
	
	/** Prepare task to be used for rendering. */
	void PrepareForRender();
	
	/** Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter,
		const decDVector &position, const decDVector &direction );
	
	/** Render parameter shader parameter block or NULL. */
	inline deoglSPBlockUBO *GetRenderParamBlock() const{ return pRenderParamBlock; }
	
	/** Set render parameter shader parameter block or NULL to use none. */
	void SetRenderParamBlock( deoglSPBlockUBO *paramBlock );
	
	/** Instances texture buffer object or NULL. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	
	/** Set instances texture buffer object or NULL to use none. */
	void SetTBOInstances( GLuint tbo );
	
	/** Use SPB instance flags. */
	inline bool GetUseSPBInstanceFlags() const{ return pUseSPBInstanceFlags; }
	
	/** Set use instance flags. */
	void SetUseSPBInstanceFlags( bool useFlags );
	
	/** Force double sided rendering. */
	inline bool GetForceDoubleSided() const{ return pForceDoubleSided; }
	
	/** Set force double sided rendering. */
	void SetForceDoubleSided( bool forceDoubleSided );
	
	/** Use vertex shader stereo rendering. */
	inline bool GetRenderVSStereo() const{ return pRenderVSStereo; }
	
	/** Set use vertex shader stereo rendering. */
	void SetRenderVSStereo( bool renderVSStereo );
	
	/**
	 * VBO draw indirect. Content is an array of oglDrawIndirectCommand. This is a union
	 * struct of oglDrawElementsIndirectCommand and oglDrawArraysIndirectCommand allowing
	 * to  use one VBO for both. Use sizeof(oglDrawIndirectCommand) as stride.
	 */
	inline GLuint GetVBODrawIndirect() const{ return pVBODrawIndirect; }
	
	
	
	/** Number of shaders. */
	inline int GetShaderCount() const{ return pShaderCount; }
	
	/** Shader at index. */
	deoglRenderTaskShader *GetShaderAt( int index ) const;
	
	/** Add shader. */
	deoglRenderTaskShader *AddShader( const deoglRenderTaskSharedShader *shader );
	deoglRenderTaskShader *AddShaderDirect( const deoglRenderTaskSharedShader *shader );
	
	
	
	/** Add configuration texture. */
	void AddConfigTexture( const deoglRenderTaskConfigTexture &texture, int specialFlags );
	
	/** Remove all configuration textures. */
	void RemoveAllConfigTextures();
	
	/** Apply configuration textures. */
	void ApplyConfigTextures();
	
	
	
	/** Number of points in all steps. */
	int GetTotalPointCount() const;
	
	/** Total amount of textures in this shader. */
	int GetTotalTextureCount() const;
	
	/** Total amount of vaos in this shader. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this shader. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this shader. */
	int GetTotalSubInstanceCount() const;
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Debug print. */
	void DebugPrint( deoglRTLogger &rtlogger );
	/*@}*/
	
	
	
private:
	void pCalcSPBInstancesMaxEntries();
	void pAssignSPBInstances();
	void pUpdateSPBInstances();
	void pCreateSPBInstanceParamBlock();
	void pUpdateVBODrawIndirect();
};

#endif
