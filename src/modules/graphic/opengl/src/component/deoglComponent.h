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
 * \brief Component peer.
 */
class deoglComponent : public deBaseGraphicComponent{
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
	bool pDirtyRenderables;
	bool pDirtyBoneMatrices;
	bool pDirtyLODErrorScaling;
	bool pDirtyMesh;
	bool pDirtySkinStateCalculatedProperties;
	bool pDirtyStaticTexture;
	
	bool pDynamicSkinRequiresSync;
	bool pTextureDynamicSkinRequiresSync;
	bool pDecalRequiresSync;
	bool pRequiresUpdateEverySync;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component peer. */
	deoglComponent( deGraphicOpenGl &ogl, deComponent &component );
	
	/** \brief Clean up component peer. */
	virtual ~deoglComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Engine resource component. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	
	
	/** \brief Render component. */
	inline deoglRComponent *GetRComponent() const{ return pRComponent; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	/** \brief Update. */
	void Update( float elapsed );
	
	
	
	/** \brief Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * \brief Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld( deoglWorld *world );
	
	
	
	/** \brief Number of LODs. */
	inline int GetLODCount() const{ return pLODCount; }
	
	/** \brief LOD at index. */
	deoglComponentLOD &GetLODAt( int index ) const;
	
	
	
	/** \brief Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at index. */
	deoglComponentTexture &GetTextureAt( int index );
	
	
	
	/** \brief Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	/** \brief Texture dynamic skin needs sync. */
	void TextureDynamicSkinRequiresSync();
	
	/** \brief Drop dynamic skin because it is about to be deleted. */
	void DropDynamicSkin();
	
	/** \brief Decal needs sync. */
	void DecalRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Skin object changed. */
	virtual void SkinChanged();
	
	/** \brief Model and skin object changed. */
	virtual void ModelAndSkinChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** \brief Occlusion mesh changed. */
	virtual void OcclusionMeshChanged();
	
	/** \brief Parameter or hint changed. */
	virtual void ParametersChanged();
	
	
	
	/** \brief Texture changed. */
	virtual void TextureChanged(int index, deComponentTexture& texture);
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	
	
	
	/**
	 * \brief Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * \details Limits the search to the provided radius.
	 */
	virtual int IndexOfTextureClosestTo( const decVector &vector, float radius );
	
	/** \brief Retrieve index of the face closest to a position or -1 if not found. */
	//int IndexOfFaceClosestTo( const decVector &vector, float radius );
	
	/** \brief Retrieve index of the vertex closest to a position or -1 if not found. */
	//int IndexOfVertexClosestTo( const decVector &vector );
	
	
	
	/** \brief World syncing linked list. */
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
	void pCheckRequiresUpdateEverySync();
	
	void pRequiresSync();
};

#endif
