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
#include <dragengine/systems/modules/audio/deBaseAudioComponent.h>

class deoalComponentDebug;
class deoalComponentTexture;
class deoalModel;
class deoalSkin;
class deoalWorld;

class deComponent;
class deAudioOpenAL;



/**
 * \brief Component resource peer.
 */
class deoalComponent : public deBaseAudioComponent{
private:
	deAudioOpenAL &pOal;
	deComponent &pComponent;
	deoalAComponent::Ref pAComponent;
	
	deoalComponentTexture **pTextures;
	int pTextureCount;
	
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
	/** \brief Create component peer. */
	deoalComponent(deAudioOpenAL &audioThread, deComponent &component);
	
	/** \brief Clean up component peer. */
	~deoalComponent() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Component. */
	inline deComponent &GetComponent() const{ return pComponent; }
	
	/** \brief Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deoalWorld *world);
	
	/** \brief Model or NULL. */
	inline deoalModel *GetModel() const{ return pModel; }
	
	/** \brief Skin or NULL. */
	inline deoalSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Audio model or NULL. */
	inline deoalModel *GetAudioModel() const{ return pAudioModel; }
	
	/** \brief Audio component. */
	inline const deoalAComponent::Ref &GetAComponent() const{ return pAComponent; }
	
	
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Mark affects sound and octree dirty. */
	void SetOctreeDirty();
	
	/** \brief Developer mode changed. */
	void DevModeChanged();
	
	
	/** \brief Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at index. */
	deoalComponentTexture &GetTextureAt(int index) const;
	
	
	
	/** \brief Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	/** \brief Texture dynamic skin needs sync. */
	void TextureDynamicSkinRequiresSync();
	
	/** \brief Drop dynamic skin because it is about to be deleted. */
	void DropDynamicSkin();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	void PositionChanged() override;
	
	/** \brief Scaling changed. */
	void ScalingChanged() override;
	
	/** \brief Orientation changed. */
	void OrientationChanged() override;
	
	/** \brief Model object changed. */
	void ModelChanged() override;
	
	/** \brief Rig object changed. */
	void RigChanged() override;
	
	/** \brief Skin object changed. */
	void SkinChanged() override;
	
	/** \brief Model and skin object changed. */
	void ModelAndSkinChanged() override;
	
	/** \brief Audio model changed. */
	void AudioModelChanged() override;
	
	/** \brief Visitility changed. */
	void VisibilityChanged() override;
	
	/** \brief Extends changed. */
	void ExtendsChanged() override;
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	void MeshDirty() override;
	
	/** \brief Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** \brief Parameter or hint changed. */
	void ParametersChanged() override;
	
	/** \brief Texture changed. */
	void TextureChanged(int index, deComponentTexture &texture) override;
	
	/** \brief Dynamic skin changed. */
	void DynamicSkinChanged() override;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
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
