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
 * \brief Render particle emitter instance type.
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
	
	deoglSPBlockUBO *pParamBlockDepth;
	deoglSPBlockUBO *pParamBlockGeometry;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCGeometryDepthTest;
	
	bool pValidParamBlockDepth;
	bool pValidParamBlockGeometry;
	bool pDirtyParamBlockDepth;
	bool pDirtyParamBlockGeometry;
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCCounter;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCGeometryDepthTest;
	
	deoglSPBlockUBO *pParamBlockLightInstance;
	deoglRenderTaskSharedInstance *pRTSInstance;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create type. */
	deoglRParticleEmitterInstanceType( deoglRParticleEmitterInstance &instance, int index );
	
	/** \brief Clean up type. */
	virtual ~deoglRParticleEmitterInstanceType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter instance. */
	inline deoglRParticleEmitterInstance &GetEmitterInstance(){ return pEmitterInstance; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** \brief First particle. */
	inline int GetFirstParticle() const{ return pFirstParticle; }
	
	/** \brief Set first particle. */
	void SetFirstParticle( int first );
	
	/** \brief Particle count. */
	inline int GetParticleCount() const{ return pParticleCount; }
	
	/** \brief Set particle count. */
	void SetParticleCount( int count );
	
	/** \brief First index. */
	inline int GetFirstIndex() const{ return pFirstIndex; }
	
	/** \brief Set first index. */
	void SetFirstIndex( int index );
	
	/** \brief Index count. */
	inline int GetIndexCount() const{ return pIndexCount; }
	
	/** \brief Set index count. */
	void SetIndexCount( int count );
	
	
	
	/** \brief Dynamic skin or \em NULL if there is none. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or \em NULL if there is none. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	
	/** \brief Set skin to use. */
	void SetUseSkin( deoglRSkin *skin );
	
	/** \brief Skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	
	/** \brief Skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	
	
	/** \brief Shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Geometry shader parameter block or \em NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglSPBlockUBO *GetParamBlockDepth();
	
	/**
	 * \brief Geometry shader parameter block or \em NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglSPBlockUBO *GetParamBlockGeometry();
	
	/** \brief Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Texture units configuration for depth type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	
	/**
	 * \brief Texture units configuration for counter type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCCounter();
	
	/**
	 * \brief Texture units configuration for geometry type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	
	/**
	 * \brief Texture units configuration for geometry type shaders with depth test or \em NULL if empty.
	 * \details This texture units configuration works for the shader type estParticleGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometryDepthTest();
	
	/**
	 * \brief Texture units configuration for a shader type.
	 * \details Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	
	
	/** \brief Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** \brief Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** \brief Mark texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** \brief Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader );
	
	/** \brief Light instance parameter block. */
	deoglSPBlockUBO *GetLightInstanceParameterBlock();
	
	/** \brief Drop light parameter block. */
	void DropLightBlocks();
	
	
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	/*@}*/
};

#endif
