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

#ifndef _DEOGLCOMPONENT_H_
#define _DEOGLCOMPONENT_H_

#include "deoglRComponent.h"
#include "../skin/dynamic/deoglDynamicSkinListener.h"

#include <dragengine/common/collection/decTLinkedList.h>
#include <dragengine/common/collection/decTUniqueList.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicComponent.h>

class deoglDynamicSkin;
class deoglComponentLOD;
class deoglComponentTexture;
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
	deoglRComponent::Ref pRComponent;
	
	deoglWorld *pParentWorld;
	deoglSkinStateController *pSkinStateController;
	
	deoglDynamicSkin *pDynamicSkin;
	
	decTUniqueList<deoglComponentLOD> pLODs;
	decTUniqueList<deoglComponentTexture> pTextures;
	
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
	bool pDirtySkinStateStates;
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
	
	decTLinkedList<deoglComponent>::Element pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component peer. */
	deoglComponent(deGraphicOpenGl &ogl, deComponent &component);
	
	/** Clean up component peer. */
	~deoglComponent() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Engine resource component. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	
	
	/** Render component. */
	inline const deoglRComponent::Ref &GetRComponent() const{ return pRComponent; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update. */
	void Update(float elapsed);
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld(deoglWorld *world);
	
	
	
	/** LODs. */
	inline const decTUniqueList<deoglComponentLOD> &GetLODs() const{ return pLODs; }
	
	/** Number of LODs. */
	inline int GetLODCount() const{ return pLODs.GetCount(); }
	
	/** LOD at index. */
	deoglComponentLOD &GetLODAt(int index) const;
	
	
	
	/** Textures. */
	inline const decTUniqueList<deoglComponentTexture> &GetTextures() const{ return pTextures; }
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextures.GetCount(); }
	
	/** Texture at index. */
	deoglComponentTexture &GetTextureAt(int index);
	
	
	
	/** Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	void TextureDynamicSkinRenderableChanged();
	void TextureDynamicSkinRequiresSync();
	
	void DirtyRenderableMapping();
	void DirtyTextureUseSkin();
	
	void DirtyAllTexturesUpdateRenderableMappings();
	
	/** Decal needs sync. */
	void DecalRequiresSync();
	/*@}*/
	
	
	
	/** \name Dynamic skin listener */
	/*@{*/
	void DynamicSkinDestroyed() override;
	void DynamicSkinRenderablesChanged() override;
	void DynamicSkinRenderableChanged(deoglDSRenderable &renderable) override;
	void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable) override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Scaling changed. */
	void ScalingChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Model object changed. */
	void ModelChanged() override;
	
	/** Skin object changed. */
	void SkinChanged() override;
	
	/** Model and skin object changed. */
	void ModelAndSkinChanged() override;
	
	/** Rig object changed. */
	void RigChanged() override;
	
	/** Visitility changed. */
	void VisibilityChanged() override;
	
	/** Extends changed. */
	void ExtendsChanged() override;
	
	/**
	 * Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	void MeshDirty() override;
	
	/** Occlusion mesh changed. */
	void OcclusionMeshChanged() override;
	
	/** Parameter or hint changed. */
	void ParametersChanged() override;
	
	
	
	/** Texture changed. */
	void TextureChanged(int index, deComponentTexture& texture) override;
	
	/** Dynamic skin changed. */
	void DynamicSkinChanged() override;
	
	
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	
	
	
	/** Decal has been added. */
	void DecalAdded(deDecal *decal) override;
	
	/** Decal has been removed. */
	void DecalRemoved(deDecal *decal) override;
	
	/** All decals have been removed. */
	void AllDecalsRemoved() override;
	
	
	
	/**
	 * Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * \details Limits the search to the provided radius.
	 */
	int IndexOfTextureClosestTo(const decVector &vector, float radius) override;
	
	/** Retrieve index of the face closest to a position or -1 if not found. */
	//int IndexOfFaceClosestTo( const decVector &vector, float radius );
	
	/** Retrieve index of the vertex closest to a position or -1 if not found. */
	//int IndexOfVertexClosestTo( const decVector &vector );
	
	
	
	/** World syncing linked list. */
	inline decTLinkedList<deoglComponent>::Element &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decTLinkedList<deoglComponent>::Element &GetLLSyncWorld() const{ return pLLSyncWorld; }
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
