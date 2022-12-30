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

#ifndef _DEOGLRPARTICLEEMITTERINSTANCETYPE_H_
#define _DEOGLRPARTICLEEMITTERINSTANCETYPE_H_

#include "../skin/deoglSkinTexture.h"

#include <dragengine/deObject.h>

class deoglRDynamicSkin;
class deoglRParticleEmitterInstance;
class deoglSPBlockUBO;
class deoglRSkin;
class deoglSkinShader;
class deoglSkinTexture;
class deoglTexUnitsConfig;
class deoglRenderTaskSharedInstance;


/**
 * Render particle emitter instance type.
 */
class deoglRParticleEmitterInstanceType : public deObject{
private:
	deoglRParticleEmitterInstance &pEmitterInstance;
	const int pIndex;
	
	int pFirstParticle;
	int pParticleCount;
	int pFirstIndex;
	int pIndexCount;
	
	deoglRDynamicSkin *pDynamicSkin;
	
	deoglRSkin *pUseSkin;
	int pUseTextureNumber;
	deoglSkinTexture *pUseSkinTexture;
	
	deoglSPBlockUBO *pParamBlock;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCGeometryDepthTest;
	
	bool pValidParamBlock;
	bool pDirtyParamBlock;
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCCounter;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCGeometryDepthTest;
	
	deoglSPBlockUBO *pParamBlockLightInstance;
	deoglRenderTaskSharedInstance *pRTSInstance;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create type. */
	deoglRParticleEmitterInstanceType( deoglRParticleEmitterInstance &instance, int index );
	
	/** Clean up type. */
	virtual ~deoglRParticleEmitterInstanceType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Emitter instance. */
	inline deoglRParticleEmitterInstance &GetEmitterInstance(){ return pEmitterInstance; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** First particle. */
	inline int GetFirstParticle() const{ return pFirstParticle; }
	
	/** Set first particle. */
	void SetFirstParticle( int first );
	
	/** Particle count. */
	inline int GetParticleCount() const{ return pParticleCount; }
	
	/** Set particle count. */
	void SetParticleCount( int count );
	
	/** First index. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** Set first index. */
	void SetFirstIndex( int index );
	
	/** Index count. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/** Set index count. */
	void SetIndexCount( int count );
	
	
	
	/** Dynamic skin or NULL if there is none. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or NULL if there is none. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	
	/** Set skin to use. */
	void SetUseSkin( deoglRSkin *skin );
	
	/** Skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	
	/** Skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	
	
	/** Skin pipelines type. */
	deoglSkinTexturePipelinesList::ePipelineTypes GetSkinPipelinesType() const;
	
	/** Skin shader pipelines for emitter type. */
	const deoglSkinTexturePipelines &GetUseSkinPipelines() const;
	
	/** Shader parameter block or NULL if there is no valid skin texture. */
	deoglSPBlockUBO *GetParamBlock();
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType ( deoglSkinTexturePipelines::eTypes type );
	
	/**
	 * Texture units configuration for depth type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	
	/**
	 * Texture units configuration for counter type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCCounter();
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	
	/**
	 * Texture units configuration for geometry type shaders with depth test or NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometryDepthTest();
	
	/**
	 * Texture units configuration for a shader type.
	 * \details Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexturePipelines::eTypes type ) const;
	
	
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Mark texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader );
	
	/** Light instance parameter block. */
	deoglSPBlockUBO *GetLightInstanceParameterBlock();
	
	/** Drop light parameter block. */
	void DropLightBlocks();
	
	
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	/*@}*/
};

#endif
