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
class deoglShaderParameterBlock;
class deoglPipeline;
class deoglRSkin;
class deoglTexture;
class deoglTexUnitsConfig;
class deoglVAO;

/**
 * Render Task Particles Step.
 */
class deoglRenderTaskParticlesStep{
public:
	struct sInstance{
		const deoglRParticleEmitterInstance::sParticle *particle;
	};
	
private:
	const deoglPipeline *pPipeline;
	const deoglShaderParameterBlock *pParamBlockTexture;
	const deoglShaderParameterBlock *pParamBlockInstance;
	const deoglTexUnitsConfig *pTUC;
	
	const deoglVAO *pVAO;
	const deoglRSkin *pSkin;
	const deoglRDynamicSkin *pDynamicSkin;
	int pTexture;
	
	sInstance *pInstances;
	int pInstanceCount;
	int pInstanceSize;
	
	int pFirstIndex;
	int pIndexCount;
	GLenum pPrimitiveType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render task particles step. */
	deoglRenderTaskParticlesStep();
	/** Cleans up the render task particles step. */
	~deoglRenderTaskParticlesStep();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Resets the step. */
	void Reset();
	
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Set pipeline. */
	void SetPipeline( const deoglPipeline *pipeline );
	
	/** Retrieves the texture shader parameter block or NULL if not used. */
	inline const deoglShaderParameterBlock *GetParameterBlockTexture() const{ return pParamBlockTexture; }
	/** Sets the texture shader parameter block or NULL if not used. */
	void SetParameterBlockTexture( const deoglShaderParameterBlock *block );
	/** Retrieves the instance shader parameter block or NULL if not used. */
	inline const deoglShaderParameterBlock *GetParameterBlockInstance() const{ return pParamBlockInstance; }
	/** Sets the instance shader parameter block or NULL if not used. */
	void SetParameterBlockInstance( const deoglShaderParameterBlock *block );
	/** Retrieves the texture units configuration. */
	inline const deoglTexUnitsConfig *GetTUC() const{ return pTUC; }
	/** Sets the texture units configuration. */
	void SetTUC( const deoglTexUnitsConfig *tuc );
	
	/** Retrieves the vao. */
	inline const deoglVAO *GetVAO() const{ return pVAO; }
	/** Sets the vao. */
	void SetVAO( const deoglVAO *vao );
	/** Retrieves the skin. */
	inline const deoglRSkin *GetSkin() const{ return pSkin; }
	/** Sets the skin. */
	void SetSkin( const deoglRSkin *skin );
	/** Retrieves the dynamic skin. */
	inline const deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	/** Sets the dynamic skin. */
	void SetDynamicSkin( const deoglRDynamicSkin *dynamicSkin );
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
	
	/** \name Instances */
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
