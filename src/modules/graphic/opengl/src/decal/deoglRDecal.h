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

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

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
class deoglRenderTaskSharedInstance;



/**
 * Render decal.
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
	
	bool pDirtyPrepareSkinStateRenderables;
	
	deoglSharedVBOBlock *pVBOBlock;
	int pPointCount;
	
	bool pDirtyVBO;
	bool pDirtyUseTexture;
	
	deoglRComponent *pParentComponent;
	bool pComponentMarkedRemove;
	
	deoglSharedSPBElement *pSharedSPBElement;
	deoglRenderTaskSharedInstance *pRTSInstance;
	
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCEnvMap;
	
	bool pDirtySharedSPBElement;
	bool pDirtyTUCs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render decal. */
	deoglRDecal( deoglRenderThread &renderThread );
	
	/** Clean up render decal. */
	virtual ~deoglRDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decVector &position );
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** Set size. */
	void SetSize( const decVector &size );
	
	/** Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** Decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set decal is visible. */
	void SetVisible( bool visible );
	
	
	
	/** Update skin. */
	void UpdateSkin( float elapsed );
	
	/** Set vbo dirty. */
	void SetDirtyVBO();
	
	
	
	/** Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or \em NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** Dynamic skin or \em NULL if not set. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or \em NULL if not set. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Retrieves the skin state or NULL if there is none. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/**
	 * Set skin state or \em NULL if there is none.
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
	
	/**
	 * Update skin state depending on skin and dynamic skin.
	 * \warning Only call from main thread during synchronization.
	 */
	void UpdateSkinState();
	
	/** Retrieves the vbo block. */
	inline deoglSharedVBOBlock *GetVBOBlock() const{ return pVBOBlock; }
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	
	
	void DirtyPrepareSkinStateRenderables();
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	
	
	/** Parent component. */
	inline deoglRComponent *GetParentComponent() const{ return pParentComponent; }
	
	/**
	 * Set parent component or \em NULL.
	 * \warning Only call from main thread during synchronization.
	 */
	void SetParentComponent( deoglRComponent *component );
	
	/**
	 * Marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	inline bool GetComponentMarkedRemove() const{ return pComponentMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglComponent only. Non-thread safe.
	 */
	void SetComponentMarkedRemove( bool marked );
	
	
	
	/** Shared shader parameter block element. */
	inline deoglSharedSPBElement *GetSharedSPBElement() const{ return pSharedSPBElement; }
	
	/** Render task shared instance or NULL. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ) const;
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	/**
	 * Texture units configuration for shadow type shaders or NULL if empty.
	 * This texture units configuration works for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	inline deoglTexUnitsConfig *GetTUCShadow() const{ return pTUCShadow; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	
	
	/** Prepare for render. Called by owner deoglRComponent if registered previously. */
	void PrepareForRender( deoglRenderPlan &plan );
	
	/** Prepare for quick disposal of decal. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
private:
	void pCreateMeshComponent();
	void pPrepareVBO();
	void pUpdateUseSkin();
	void pPrepareTUCs();
	void pPrepareParamBlocks();
	void pPrepareSkinStateRenderables();
	void pUpdateRTSInstance();
	void pUpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
		int element, deoglSkinShader &skinShader );
	
	void pRequiresPrepareForRender();
};

#endif
