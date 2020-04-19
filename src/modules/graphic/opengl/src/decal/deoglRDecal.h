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

#ifndef _DEOGLRDECAL_H_
#define _DEOGLRDECAL_H_

#include "../skin/deoglSkinTexture.h"
#include "../rendering/task/deoglRenderTaskInstanceGroup.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRenderPlan;
class deoglRComponent;
class deoglRDynamicSkin;
class deoglRenderThread;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglSharedVBOBlock;
class deoglSharedSPBElement;
class deoglShaderParameterBlock;
class deoglSkinShader;
class deoglSkinState;
class deoglTexUnitsConfig;
class deoglVAO;



/**
 * \brief Render decal.
 */
class deoglRDecal : public deObject{
public:
	deoglRenderThread &pRenderThread;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	decTexMatrix2 pTransform;
	bool pVisible;
	
	deoglRSkin *pSkin;
	deoglRDynamicSkin *pDynamicSkin;
	deoglSkinState *pSkinState;
	
	deoglRSkin *pUseSkin;
	int pUseTextureNumber;
	deoglSkinTexture *pUseSkinTexture;
	deoglRDynamicSkin *pUseDynamicSkin;
	deoglSkinState *pUseSkinState;
	
	deoglSharedVBOBlock *pVBOBlock;
	int pPointCount;
	
	bool pDirtyVBO;
	bool pDirtyUseTexture;
	bool pDirtyRenderables;
	
	deoglRComponent *pParentComponent;
	bool pComponentMarkedRemove;
	
	deoglSPBlockUBO *pParamBlockGeometry;
	deoglSPBlockUBO *pParamBlockEnvMap;
	
	deoglSharedSPBElement *pSharedSPBElement;
	deoglRenderTaskInstanceGroup pRTIGroup;
	
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCEnvMap;
	
	bool pValidParamBlockGeometry;
	bool pValidParamBlockEnvMap;
	bool pDirtyParamBlockGeometry;
	bool pDirtyParamBlockEnvMap;
	bool pDirtySharedSPBElement;
	
	bool pDirtyTUCGeometry;
	bool pDirtyTUCShadow;
	bool pDirtyTUCEnvMap;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render decal. */
	deoglRDecal( deoglRenderThread &renderThread );
	
	/** \brief Clean up render decal. */
	virtual ~deoglRDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector &size );
	
	/** \brief Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** \brief Decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set decal is visible. */
	void SetVisible( bool visible );
	
	
	
	/** \brief Update skin. */
	void UpdateSkin( float elapsed );
	
	/** \brief Update vbo if required. */
	void UpdateVBO();
	
	/** \brief Set vbo dirty. */
	void SetDirtyVBO();
	
	/** \brief Update renderables in the decal if existing */
	void UpdateRenderables( deoglRenderPlan &plan );
	
	
	
	/** \brief Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or \em NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** \brief Dynamic skin or \em NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or \em NULL if not set. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Retrieves the skin state or NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * \brief Set skin state or \em NULL if there is none.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetSkinState( deoglSkinState *skinState );
	
	/** Retrieves the actual skin to use. */
	inline deoglRSkin *GetUseSkin() const{ return pUseSkin; }
	/** Retrieves the actual skin texture number to use. */
	inline int GetUseTextureNumber() const{ return pUseTextureNumber; }
	/** Retrieves the actual skin texture to use. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	/** Retrieves the actual dynamic skin to use. */
	inline deoglRDynamicSkin *GetUseDynamicSkin() const{ return pUseDynamicSkin; }
	/** Retrieves the actual skin state to use. */
	inline deoglSkinState *GetUseSkinState() const{ return pUseSkinState; }
	/** Updates the actual texture parameters to use. */
	void UpdateUseSkin();
	
	/**
	 * \brief Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState();
	
	/** Retrieves the vbo block. */
	inline deoglSharedVBOBlock *GetVBOBlock() const{ return pVBOBlock; }
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	
	
	/** \brief Parent component. */
	inline deoglRComponent *GetParentComponent() const{ return pParentComponent; }
	
	/**
	 * \brief Set parent component or \em NULL.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetParentComponent( deoglRComponent *component );
	
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	inline bool GetComponentMarkedRemove() const{ return pComponentMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	void SetComponentMarkedRemove( bool marked );
	
	
	
	/** \brief Shared shader parameter block element. */
	deoglSharedSPBElement *GetSharedSPBElement();
	
	/** \brief Render task instance group. */
	inline deoglRenderTaskInstanceGroup &GetRTIGroup(){ return pRTIGroup; }
	
	/** Retrieves the shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	/**
	 * Retrieves the geometry shader parameter block or NULL if there is no valid skin texture.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	deoglSPBlockUBO *GetParamBlockGeometry();
	/**
	 * Retrieves the environment map shader parameter block or NULL if there is no valid skin texture.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	deoglSPBlockUBO *GetParamBlockEnvMap();
	/** Retrieves the texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	/**
	 * Retrieves the texture units configuration for geometry type shaders or NULL if empty.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	/**
	 * Retrieves the texture units configuration for shadow type shaders or NULL if empty.
	 * This texture units configuration works for the shader types estComponentShadowProjection,
	 * estComponentShadowOrthogonal and estComponentShadowDistance.
	 */
	deoglTexUnitsConfig *GetTUCShadow();
	/**
	 * Retrieves the texture units configuration for the environment map shader or NULL if empty.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	deoglTexUnitsConfig *GetTUCEnvMap();
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType );
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock, int element, deoglSkinShader &skinShader );
	
	
	
	/** \brief Prepare for quick disposal of decal. */
	void PrepareQuickDispose();
	/*@}*/
	
private:
	void pUpdateRenderables();
	void pCreateMeshComponent();
};

#endif
