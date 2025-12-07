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
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component peer. */
	deoglComponent(deGraphicOpenGl &ogl, deComponent &component);
	
	/** Clean up component peer. */
	virtual ~deoglComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl module. */
	inline deGraphicOpenGl &GetOgl() const{return pOgl;}
	
	/** Engine resource component. */
	inline deComponent &GetComponent() const{return pComponent;}
	
	
	
	/** Render component. */
	inline deoglRComponent *GetRComponent() const{return pRComponent;}
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update. */
	void Update(float elapsed);
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{return pParentWorld;}
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld(deoglWorld *world);
	
	
	
	/** Number of LODs. */
	inline int GetLODCount() const{return pLODCount;}
	
	/** LOD at index. */
	deoglComponentLOD &GetLODAt(int index) const;
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{return pTextureCount;}
	
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
	virtual void DynamicSkinDestroyed();
	virtual void DynamicSkinRenderablesChanged();
	virtual void DynamicSkinRenderableChanged(deoglDSRenderable &renderable);
	virtual void DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable);
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
	virtual void DecalAdded(deDecal *decal);
	
	/** Decal has been removed. */
	virtual void DecalRemoved(deDecal *decal);
	
	/** All decals have been removed. */
	virtual void AllDecalsRemoved();
	
	
	
	/**
	 * Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * \details Limits the search to the provided radius.
	 */
	virtual int IndexOfTextureClosestTo(const decVector &vector, float radius);
	
	/** Retrieve index of the face closest to a position or -1 if not found. */
	//int IndexOfFaceClosestTo( const decVector &vector, float radius );
	
	/** Retrieve index of the vertex closest to a position or -1 if not found. */
	//int IndexOfVertexClosestTo( const decVector &vector );
	
	
	
	/** World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){return pLLSyncWorld;}
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{return pLLSyncWorld;}
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
