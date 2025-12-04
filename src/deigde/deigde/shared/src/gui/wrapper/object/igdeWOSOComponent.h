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
#include "../../../resourceloader/igdeResourceLoaderListener::Ref.h"

#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/resources/animator/deAnimatorInstance::Ref.h>
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
	igdeResourceLoaderListener::Ref pResLoad;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	deAnimatorInstance::Ref pAnimator;
	int pPlaybackControllerIndex;
	decString pPathAnimator;
	decString pMove;
	bool pRenderEnvMap;
	bool pAffectsAudio;
	bool pLightShadowIgnore;
	decObjectDictionary pTextureSkins;
	bool pColliderCanInteract;
	bool pColliderAddedToWorld;
	deComponent::Ref pOutlineComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOComponent( igdeWObject &wrapper, const igdeGDCComponent &gdComponent, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	~igdeWOSOComponent() override;
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
	void UpdateParameters() override;
	
	/** \brief All sub components finished loading. */
	void OnAllSubObjectsFinishedLoading() override;
	
	/** \brief Update visibility. */
	void UpdateVisibility() override;
	
	/** \brief Layer masks changed. */
	void UpdateLayerMasks() override;
	
	/** \brief Collision filter changed. */
	void UpdateCollisionFilter() override;
	
	/** \brief Geometry changed. */
	void UpdateGeometry() override;
	
	/** \brief Update collider response type. */
	void UpdateColliderResponseType() override;
	
	/** \brief Frame update. */
	void Update( float elapsed ) override;
	
	/** \brief Reset physics. */
	void ResetPhysics() override;
	
	/** \brief Reset component textures. */
	void ResetComponentTextures() override;
	
	/** \brief Camera changed. */
	void CameraChanged() override;
	
	/** \brief Outline skin changed. */
	void OutlineSkinChanged() override;
	
	/** \brief Visit. */
	void Visit( igdeWOSOVisitor &visitor ) override;
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
	
	/**
	 * \brief Sub object is visible.
	 * 
	 * Returns true if wrapped resources is visible. For example a component resource without
	 * valid model or with no texture containing a valid skin is invisible. This check does not
	 * include the visibility state of the object. Hence invisible content is still considered
	 * visible if it would be visible otherwise.
	 */
	bool IsContentVisible() override;
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
