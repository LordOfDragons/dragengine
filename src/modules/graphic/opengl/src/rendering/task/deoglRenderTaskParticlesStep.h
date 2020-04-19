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

#ifndef _DEOGLRENDERTASKPARTICLESSTEP_H_
#define _DEOGLRENDERTASKPARTICLESSTEP_H_

#include "../../deoglBasics.h"
#include "../../particle/deoglRParticleEmitterInstance.h"

#include <dragengine/common/math/decMath.h>

class deoglCubeMap;
class deoglRDynamicSkin;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglRSkin;
class deoglTexture;
class deoglTexUnitsConfig;
class deoglVAO;

/**
 * @brief Render Task Particles Step.
 */
class deoglRenderTaskParticlesStep{
public:
	struct sInstance{
		const deoglRParticleEmitterInstance::sParticle *particle;
	};
	
private:
	deoglShaderProgram *pShader;
	deoglSPBlockUBO *pParamBlockTexture;
	deoglSPBlockUBO *pParamBlockInstance;
	deoglTexUnitsConfig *pTUC;
	
	deoglVAO *pVAO;
	deoglRSkin *pSkin;
	deoglRDynamicSkin *pDynamicSkin;
	int pTexture;
	
	sInstance *pInstances;
	int pInstanceCount;
	int pInstanceSize;
	
	int pFirstIndex;
	int pIndexCount;
	GLenum pPrimitiveType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task particles step. */
	deoglRenderTaskParticlesStep();
	/** Cleans up the render task particles step. */
	~deoglRenderTaskParticlesStep();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the step. */
	void Reset();
	
	/** Retrieves the shader. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	/** Sets the shader. */
	void SetShader( deoglShaderProgram *shader );
	/** Retrieves the texture shader parameter block or NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlockTexture() const{ return pParamBlockTexture; }
	/** Sets the texture shader parameter block or NULL if not used. */
	void SetParameterBlockTexture( deoglSPBlockUBO *block );
	/** Retrieves the instance shader parameter block or NULL if not used. */
	inline deoglSPBlockUBO *GetParameterBlockInstance() const{ return pParamBlockInstance; }
	/** Sets the instance shader parameter block or NULL if not used. */
	void SetParameterBlockInstance( deoglSPBlockUBO *block );
	/** Retrieves the texture units configuration. */
	inline deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	/** Sets the texture units configuration. */
	void SetTUC( deoglTexUnitsConfig *tuc );
	
	/** Retrieves the vao. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	/** Sets the vao. */
	void SetVAO( deoglVAO *vao );
	/** Retrieves the skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	/** Sets the skin. */
	void SetSkin( deoglRSkin *skin );
	/** Retrieves the dynamic skin. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	/** Sets the dynamic skin. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	/** Retrieves the texture index. */
	inline int GetTexture() const{ return pTexture; }
	/** Sets the texture index. */
	void SetTexture( int texture );
	
	/** Retrieves the first index to render. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	/** Sets the first index to render. */
	void SetFirstIndex( int firstIndex );
	/** Retrieves the number of indices to render. */
	inline int GetIndexCount() const{ return pIndexCount; }
	/** Sets the number of indices to render. */
	void SetIndexCount( int indexCount );
	/** Increment the number if indices to render. */
	void IncrementIndexCount( int amount );
	/** Retrieves the primitive type. */
	inline GLenum GetPrimitiveType() const{ return pPrimitiveType; }
	/** Sets the primitive type. */
	void SetPrimitiveType( GLenum primitiveType );
	/*@}*/
	
	/** @name Instances */
	/*@{*/
	/** Retrieves the direct pointer to the instances. */
	inline sInstance *GetInstances() const{ return pInstances; }
	/** Retrieves the number of instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	/** Retrieves the instance at the given position. */
	sInstance &GetInstanceAt( int index ) const;
	/** Adds an instance. */
	sInstance &AddInstance();
	/** Removes all instances. */
	void RemoveAllInstances();
	/*@}*/
};

#endif
