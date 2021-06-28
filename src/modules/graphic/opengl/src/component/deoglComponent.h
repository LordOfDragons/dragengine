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

#ifndef _DEOGLCOMPONENT_H_
#define _DEOGLCOMPONENT_H_

#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicComponent.h>

class deoglDynamicSkin;
class deoglComponentLOD;
class deoglComponentTexture;
class deoglRComponent;
class deoglSkinStateController;
class deoglWorld;

class deComponent;
class deGraphicOpenGl;


/**
 * Component peer.
 */
class deoglComponent : public deBaseGraphicComponent, deoglDynamicSkinListener{
public:
	deGraphicOpenGl &pOgl;
	deComponent &pComponent;
	deoglRComponent *pRComponent;
	
	deoglWorld *pParentWorld;
	deoglSkinStateController *pSkinStateController;
	
	deoglDynamicSkin *pDynamicSkin;
	
	deoglComponentLOD **pLODs;
	int pLODCount;
	
	deoglComponentTexture **pTextures;
	int pTextureCount;
	
	float pAccumUpdate;
	
	bool pDirtyComponent;
	bool pDirtySkinStateController;
	bool pDirtyLodTextures;
	bool pDirtyOctreeNode;
	bool pDirtyExtends;
	bool pDirtyMatrices;
	bool pDirtyDecals;
	bool pDirtyCullSphere;
	bool pDirtyRenderEnvMap;
	bool pDirtyResetStatic;
	bool pDirtyModel;
	bool pDirtySkin;
	bool pDirtyRig;
	bool pDirtyDynamicSkin;
	bool pDirtyOcclusionMesh;
	bool pDirtyOcclusionMeshBones;
	bool pDirtyRenderableMapping;
	bool pDirtyBoneMatrices;
	bool pDirtyLODErrorScaling;
	bool pDirtyMesh;
	bool pDirtySkinStateCalculatedProperties;
	bool pSkinStatePrepareRenderables;
	bool pDirtyStaticTexture;
	bool pNotifyTexturesChanged;
	bool pNotifyTUCChanged;
	bool pDirtySolid;
	bool pDirtyTextureUseSkin;
	
	bool pDynamicSkinRenderablesChanged;
	bool pDynamicSkinRequiresSync;
	
	bool pTextureDynamicSkinRenderablesChanged;
	bool pTextureDynamicSkinRequiresSync;
	bool pDecalRequiresSync;
	bool pRequiresUpdateEverySync;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component peer. */
	deoglComponent( deGraphicOpenGl &ogl, deComponent &component );
	
	/** Clean up component peer. */
	virtual ~deoglComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Engine resource component. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	
	
	/** Render component. */
	inline deoglRComponent *GetRComponent() const{ return pRComponent; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update. */
	void Update( float elapsed );
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld( deoglWorld *world );
	
	
	
	/** Number of LODs. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** LOD at index. */
	deoglComponentLOD &GetLODAt( int index ) const;
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Texture at index. */
	deoglComponentTexture &GetTextureAt( int index );
	
	
	
	/** Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	void TextureDynamicSkinRenderableChanged();
	void TextureDynamicSkinRequiresSync();
	
	void DirtyRenderableMapping();
	void DirtyTextureUseSkin();
	
	/** Decal needs sync. */
	void DecalRequiresSync();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged( deoglDSRenderable &renderable );
	virtual void DynamicSkinRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Scaling changed. */
	virtual void ScalingChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Model object changed. */
	virtual void ModelChanged();
	
	/** Skin object changed. */
	virtual void SkinChanged();
	
	/** Model and skin object changed. */
	virtual void ModelAndSkinChanged();
	
	/** Rig object changed. */
	virtual void RigChanged();
	
	/** Visitility changed. */
	virtual void VisibilityChanged();
	
	/** Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** Occlusion mesh changed. */
	virtual void OcclusionMeshChanged();
	
	/** Parameter or hint changed. */
	virtual void ParametersChanged();
	
	
	
	/** Texture changed. */
	virtual void TextureChanged(int index, deComponentTexture& texture);
	
	/** Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** All decals have been removed. */
	virtual void AllDecalsRemoved();
	
	
	
	/**
	 * Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * \details Limits the search to the provided radius.
	 */
	virtual int IndexOfTextureClosestTo( const decVector &vector, float radius );
	
	/** Retrieve index of the face closest to a position or -1 if not found. */
	//int IndexOfFaceClosestTo( const decVector &vector, float radius );
	
	/** Retrieve index of the vertex closest to a position or -1 if not found. */
	//int IndexOfVertexClosestTo( const decVector &vector );
	
	
	
	/** World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pDropLODs();
	void pCreateLODs();
	void pDropTextures();
	void pCreateTextures();
	
	void pSyncModel();
	void pSyncSkin();
	void pSyncDynamicSkin();
	void pSyncOcclusionMesh();
	void pSyncLodTextures();
	void pSyncDecals();
	void pSyncTextureDynamicSkinRenderablesChanged();
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
