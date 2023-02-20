/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWOSOCOMPONENT_H_
#define _IGDEWOSOCOMPONENT_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/resources/animator/deAnimatorInstanceReference.h>
#include <dragengine/resources/component/deComponentReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDClass;
class igdeGDCComponent;
class deAnimatorController;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOComponent : public igdeWOSubObject{
private:
	const igdeGDCComponent &pGDComponent;
	deComponentReference pComponent;
	deColliderReference pCollider;
	igdeResourceLoaderListenerReference pResLoad;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	deAnimatorInstanceReference pAnimator;
	int pPlaybackControllerIndex;
	decString pPathAnimator;
	bool pRenderEnvMap;
	bool pAffectsAudio;
	bool pLightShadowIgnore;
	decObjectDictionary pTextureSkins;
	bool pColliderCanInteract;
	bool pColliderAddedToWorld;
	deComponentReference pOutlineComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOComponent( igdeWObject &wrapper, const igdeGDCComponent &gdComponent, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Component. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Collider. */
	inline deColliderComponent *GetCollider() const{ return ( deColliderComponent* )( deCollider* )pCollider; }
	
	/** \brief Animator. */
	inline deAnimatorInstance *GetAnimator() const{ return pAnimator; }
	
	/** \brief Playback controller index. */
	inline int GetPlaybackControllerIndex() const{ return pPlaybackControllerIndex; }
	
	/** \brief Ignore component while casting shadows by lights present in the same object. */
	inline bool GetLightShadowIgnore() const{ return pLightShadowIgnore; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Collision filter changed. */
	virtual void UpdateCollisionFilter();
	
	/** \brief Geometry changed. */
	virtual void UpdateGeometry();
	
	/** \brief Update collider response type. */
	virtual void UpdateColliderResponseType();
	
	/** \brief Frame update. */
	virtual void Update( float elapsed );
	
	/** \brief Reset physics. */
	virtual void ResetPhysics();
	
	/** \brief Reset component textures. */
	virtual void ResetComponentTextures();
	
	/** \brief Camera changed. */
	virtual void CameraChanged();
	
	/** \brief Outline skin changed. */
	virtual void OutlineSkinChanged();
	
	/** \brief Visit. */
	virtual void Visit( igdeWOSOVisitor &visitor );
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pLoadResources();
	void pUpdateComponent();
	void pUpdateTextures();
	void pDestroyComponent();
	bool pIsVisible() const;
	void pUpdateOutlineComponent();
	void pReleaseOutlineComponent();
};

#endif
