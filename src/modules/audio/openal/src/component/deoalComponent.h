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

#ifndef _DEOALCOMPONENT_H_
#define _DEOALCOMPONENT_H_

#include "deoalAComponent.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTLinkedList.h>
#include <dragengine/common/collection/decTUniqueList.h>
#include <dragengine/systems/modules/audio/deBaseAudioComponent.h>

class deoalComponentDebug;
class deoalComponentTexture;
class deoalModel;
class deoalSkin;
class deoalWorld;

class deComponent;
class deAudioOpenAL;



/**
 * Component resource peer.
 */
class deoalComponent : public deBaseAudioComponent{
private:
	deAudioOpenAL &pOal;
	deComponent &pComponent;
	deoalAComponent::Ref pAComponent;
	
	decTUniqueList<deoalComponentTexture> pTextures;
	
	deoalWorld *pParentWorld;
	deoalModel *pModel;
	deoalSkin *pSkin;
	deoalModel *pAudioModel;
	
	bool pDirtyComponent;
	bool pDirtyAffectsSound;
	bool pDirtyOctreeNode;
	bool pDirtyGeometry;
	bool pDirtyModel;
	bool pDirtySkin;
	bool pDirtyRig;
	bool pDirtyDynamicSkin;
	bool pDirtyLayerMask;
	bool pDirtyBones;
	bool pDirtyBonesGeometry;
	bool pDirtyTextures;
	
	bool pDynamicSkinRequiresSync;
	bool pTextureDynamicSkinRequiresSync;
	
	deoalComponentDebug *pDebug;
	
	decTLinkedList<deoalComponent>::Element pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component peer. */
	deoalComponent(deAudioOpenAL &audioThread, deComponent &component);
	
	/** Clean up component peer. */
	~deoalComponent() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** Component. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	/** Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or NULL. */
	void SetParentWorld(deoalWorld *world);
	
	/** Model or NULL. */
	inline deoalModel *GetModel() const{ return pModel; }
	
	/** Skin or NULL. */
	inline deoalSkin *GetSkin() const{ return pSkin; }
	
	/** Audio model or NULL. */
	inline deoalModel *GetAudioModel() const{ return pAudioModel; }
	
	/** Audio component. */
	inline const deoalAComponent::Ref &GetAComponent() const{ return pAComponent; }
	
	
	
	/** Synchronize. */
	void Synchronize();
	
	/** Mark affects sound and octree dirty. */
	void SetOctreeDirty();
	
	/** Developer mode changed. */
	void DevModeChanged();
	
	
	/** Textures. */
	inline const decTUniqueList<deoalComponentTexture> &GetTextures() const{ return pTextures; }
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextures.GetCount(); }
	
	/** Texture at index. */
	deoalComponentTexture &GetTextureAt(int index) const;
	
	
	
	/** Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	/** Texture dynamic skin needs sync. */
	void TextureDynamicSkinRequiresSync();
	
	/** Drop dynamic skin because it is about to be deleted. */
	void DropDynamicSkin();
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
	
	/** Rig object changed. */
	void RigChanged() override;
	
	/** Skin object changed. */
	void SkinChanged() override;
	
	/** Model and skin object changed. */
	void ModelAndSkinChanged() override;
	
	/** Audio model changed. */
	void AudioModelChanged() override;
	
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
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** Parameter or hint changed. */
	void ParametersChanged() override;
	
	/** Texture changed. */
	void TextureChanged(int index, deComponentTexture &texture) override;
	
	/** Dynamic skin changed. */
	void DynamicSkinChanged() override;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** World syncing linked list. */
	inline decTLinkedList<deoalComponent>::Element &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decTLinkedList<deoalComponent>::Element &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pDropTextures();
	void pCreateTextures();
	
	void pSyncModel();
	void pSyncSkin();
	void pSyncDynamicSkin();
	void pSyncTextures();
	
	void pSyncDebug();
	
	void pRequiresSync();
};

#endif
