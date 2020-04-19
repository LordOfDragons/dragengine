/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALCOMPONENT_H_
#define _DEOALCOMPONENT_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/audio/deBaseAudioComponent.h>

class deoalAComponent;
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
	deoalAComponent *pAComponent;
	
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
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component peer. */
	deoalComponent( deAudioOpenAL &audioThread, deComponent &component );
	
	/** \brief Clean up component peer. */
	virtual ~deoalComponent();
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
	void SetParentWorld( deoalWorld *world );
	
	/** \brief Model or NULL. */
	inline deoalModel *GetModel() const{ return pModel; }
	
	/** \brief Skin or NULL. */
	inline deoalSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Audio model or NULL. */
	inline deoalModel *GetAudioModel() const{ return pAudioModel; }
	
	/** \brief Audio component. */
	inline deoalAComponent *GetAComponent() const{ return pAComponent; }
	
	
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Mark affects sound and octree dirty. */
	void SetOctreeDirty();
	
	/** \brief Developer mode changed. */
	void DevModeChanged();
	
	
	/** \brief Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** \brief Texture at index. */
	deoalComponentTexture &GetTextureAt( int index ) const;
	
	
	
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
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Skin object changed. */
	virtual void SkinChanged();
	
	/** \brief Model and skin object changed. */
	virtual void ModelAndSkinChanged();
	
	/** \brief Audio model changed. */
	virtual void AudioModelChanged();
	
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
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Parameter or hint changed. */
	virtual void ParametersChanged();
	
	/** \brief Texture changed. */
	virtual void TextureChanged( int index, deComponentTexture &texture );
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
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
