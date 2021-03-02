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

#ifndef _DEOGLRCOMPONENTCOMPONENT_H_
#define _DEOGLRCOMPONENTCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

#include "../skin/deoglSkinTexture.h"

class deoglRComponent;
class deoglRDynamicSkin;
class deoglRSkin;
class deoglShaderParameterBlock;
class deoglShaderProgram;
class deoglSharedSPBElement;
class deoglSharedSPBRTIGroup;
class deoglSharedVideoPlayerList;
class deoglSkinShader;
class deoglSkinState;
class deoglSkinTexture;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;


/**
 * \brief Render component texture.
 */
class deoglRComponentTexture : public deObject{
private:
	deoglRComponent &pComponent;
	const int pIndex;
	
	decTexMatrix2 pTransform;
	
	deoglRSkin *pSkin;
	deoglRDynamicSkin *pDynamicSkin;
	deoglSkinState *pSkinState;
	
	deoglRSkin *pUseSkin;
	int pUseTextureNumber;
	deoglSkinTexture *pUseSkinTexture;
	deoglSkinState *pUseSkinState;
	deoglRDynamicSkin *pUseDynamicSkin;
	bool pUseDoubleSided;
	bool pUseDecal;
	
	deoglSPBlockUBO *pParamBlockDepth;
	deoglSPBlockUBO *pParamBlockGeometry;
	deoglSPBlockUBO *pParamBlockEnvMap;
	deoglSharedSPBElement *pSharedSPBElement;
	decObjectList pSharedSPBRTIGroup;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCCounter;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCShadowCube;
	deoglTexUnitsConfig *pTUCEnvMap;
	deoglTexUnitsConfig *pTUCOutlineDepth;
	deoglTexUnitsConfig *pTUCOutlineGeometry;
	deoglTexUnitsConfig *pTUCOutlineCounter;
	deoglTexUnitsConfig *pTUCLuminance;
	deoglTexUnitsConfig *pTUCGIMaterial;
	
	bool pValidParamBlockDepth;
	bool pValidParamBlockGeometry;
	bool pValidParamBlockEnvMap;
	bool pDirtyParamBlockDepth;
	bool pDirtyParamBlockGeometry;
	bool pDirtyParamBlockEnvMap;
	bool pDirtySharedSPBElement;
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCCounter;
	bool pDirtyTUCShadow;
	bool pDirtyTUCShadowCube;
	bool pDirtyTUCEnvMap;
	bool pDirtyTUCOutlineDepth;
	bool pDirtyTUCOutlineGeometry;
	bool pDirtyTUCOutlineCounter;
	bool pDirtyTUCLuminance;
	bool pDirtyTUCGIMaterial;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component texture. */
	deoglRComponentTexture( deoglRComponent &component, int index );
	
	/** \brief Clean up component texture. */
	virtual ~deoglRComponentTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent component. */
	inline deoglRComponent &GetComponent() const{ return pComponent; }
	
	/** \brief Texture index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	
	
	/** \brief Skin or \em NULL if there is none. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or \em NULL if there is none. */
	void SetSkin( deoglRSkin *skin );
	
	/** \brief Dynamic skin or \em NULL if there is none. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or \em NULL if there is none. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Skin state or \em NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * \brief Set skin state or \em NULL if there is none.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetSkinState( deoglSkinState *skinState );
	
	/** \brief Check if skin has dynamic channels. */
	void CheckSkinDynamicChannels();
	
	/**
	 * \brief Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState();
	
	/** \brief Skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	
	/** \brief Skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	
	/** \brief Skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** \brief Skin state to use. */
	inline deoglSkinState *GetUseSkinState() const{ return pUseSkinState; }
	
	/** \brief Dynamic skin to use. */
	inline deoglRDynamicSkin *GetUseDynamicSkin() const{ return pUseDynamicSkin; }
	
	/** \brief Texture to use is double sided. */
	inline bool GetUseDoubleSided() const{ return pUseDoubleSided; }
	
	/** \brief Texture has to be rendered as decal. */
	inline bool GetUseDecal() const{ return pUseDecal; }
	
	/** \brief Updates the actual texture parameters to use. */
	void UpdateUseSkin();
	
	
	
	/** \brief Shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Depth shader parameter block or \em NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentDepth
	 *          - deoglSkinTexture::estComponentDepthClipPlane
	 *          - deoglSkinTexture::estComponentCounter
	 *          - deoglSkinTexture::estComponentCounterClipPlane
	 *          - deoglSkinTexture::estComponentShadowProjection
	 *          - deoglSkinTexture::estComponentShadowOrthogonal
	 *          - deoglSkinTexture::estComponentShadowDistance
	 */
	deoglSPBlockUBO *GetParamBlockDepth();
	
	/**
	 * \brief Geometry shader parameter block or \em NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentGeometry
	 */
	deoglSPBlockUBO *GetParamBlockGeometry();
	
	/**
	 * \brief Environment map shader parameter block or NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estEnvMap
	 */
	deoglSPBlockUBO *GetParamBlockEnvMap();
	
	/** \brief Shared shader parameter block element. */
	deoglSharedSPBElement *GetSharedSPBElement();
	
	/** \brief Shared SPB render task instance group. */
	deoglSharedSPBRTIGroup &GetSharedSPBRTIGroup( int lodLevel );
	
	/** \brief Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * \brief Texture units configuration for depth type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentDepth
	 *          - deoglSkinTexture::estComponentDepthClipPlane
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	
	/**
	 * \brief Texture units configuration for geometry type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentGeometry
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	
	/**
	 * \brief Texture units configuration for counter type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentCounter
	 *          - deoglSkinTexture::estComponentCounterClipPlane
	 */
	deoglTexUnitsConfig *GetTUCCounter();
	
	/**
	 * \brief Texture units configuration for shadow type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types
	 *          - deoglSkinTexture::estComponentShadowProjection
	 *          - deoglSkinTexture::estComponentShadowOrthogonal
	 *          - deoglSkinTexture::estComponentShadowDistance
	 */
	deoglTexUnitsConfig *GetTUCShadow();
	
	/**
	 * \brief Texture units configuration for shadow cube type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types
	 *          - deoglSkinTexture::estComponentShadowDistanceCube
	 */
	deoglTexUnitsConfig *GetTUCShadowCube();
	
	/**
	 * \brief Texture units configuration for the environment map shader or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estEnvMap
	 */
	deoglTexUnitsConfig *GetTUCEnvMap();
	
	/**
	 * \brief Texture units configuration for outline geometry type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estOutlineGeometry
	 */
	deoglTexUnitsConfig *GetTUCOutlineGeometry();
	
	/**
	 * \brief Texture units configuration for outline depth type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estOutlineDepth
	 *          - deoglSkinTexture::estOutlineDepthClipPlane
	 */
	deoglTexUnitsConfig *GetTUCOutlineDepth();
	
	/**
	 * \brief Texture units configuration for outline counter type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estOutlineCounter
	 *          - deoglSkinTexture::estOutlineCounterClipPlane
	 */
	deoglTexUnitsConfig *GetTUCOutlineCounter();
	
	/**
	 * \brief Texture units configuration for luminance type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentLuminance
	 */
	deoglTexUnitsConfig *GetTUCLuminance();
	
	/**
	 * \brief Texture units configuration for GI material type shaders or \em NULL if empty.
	 * \details This texture units configuration works for the shader types:
	 *          - deoglSkinTexture::estComponentGIMaterial
	 */
	deoglTexUnitsConfig *GetTUCGIMaterial();
	
	/**
	 * \brief Obtain texture units configuration for a shader type.
	 * \details Bare call not to be used directly.
	 */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/** \brief Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** \brief Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** \brief Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/**
	 * \brief Update instance parameter shader parameter block.
	 * 
	 * Parameter block has to be mapped while calling this method.
	 * Caller is responsible to properly unmap in case of exceptions.
	 */
	void UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
		int element, deoglSkinShader &skinShader );
	
	/** \brief Prepare skin state renderables if dirty. */
	void PrepareSkinStateRenderables();
	/*@}*/
};

#endif
