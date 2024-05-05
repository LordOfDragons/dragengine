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
