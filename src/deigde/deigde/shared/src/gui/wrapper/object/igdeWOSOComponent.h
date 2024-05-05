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

#ifndef _IGDEWOSOCOMPONENT_H_
#define _IGDEWOSOCOMPONENT_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/resources/animator/deAnimatorInstanceReference.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/component/deComponent.h>


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
	deComponent::Ref pComponent;
	deComponent::Ref pComponentInteraction;
	deColliderComponent::Ref pCollider;
	deColliderComponent::Ref pColliderInteraction;
	igdeResourceLoaderListenerReference pResLoad;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	deAnimatorInstanceReference pAnimator;
	int pPlaybackControllerIndex;
	decString pPathAnimator;
	decString pMove;
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
	inline const deComponent::Ref &GetComponent() const{ return pComponent; }
	
	/** \brief Component interaction. */
	inline const deComponent::Ref &GetComponentInteraction() const{ return pComponentInteraction; }
	
	/** \brief Collider. */
	inline const deColliderComponent::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Interaction collider. */
	inline const deColliderComponent::Ref &GetColliderInteraction() const{ return pColliderInteraction; }
	
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
