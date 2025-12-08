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

#ifndef _IGDEWOSOPARTICLEEMITTER_H_
#define _IGDEWOSOPARTICLEEMITTER_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../triggersystem/igdeTriggerExpression.h"

#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/collider/deCollider.h>


class deColliderAttachment;
class igdeGDCParticleEmitter;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOParticleEmitter : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOParticleEmitter> Ref;
	
	
private:
	const igdeGDCParticleEmitter &pGDParticleEmitter;
	deParticleEmitterInstance::Ref pParticleEmitter;
	igdeResourceLoaderListener::Ref pResLoad;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	igdeTriggerExpression::Ref pTriggerCasting;
	decString pPathEmitter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOParticleEmitter(igdeWObject &wrapper, const igdeGDCParticleEmitter &gdParticleEmitter, const decString &prefix);
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief ParticleEmitter resource. */
	inline const deParticleEmitterInstance::Ref &GetParticleEmitter() const{ return pParticleEmitter; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief Init triggers. */
	virtual void InitTriggers();
	
	/** \brief Update trigger. */
	virtual void UpdateTriggers();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Collision filter changed. */
	virtual void UpdateCollisionFilter();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Visit. */
	virtual void Visit(igdeWOSOVisitor &visitor);
	
	/** \brief For internal use only. */
	void AsyncLoadFinished(bool success);
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pLoadResources();
	void pUpdateParticleEmitter();
	void pDestroyParticleEmitter();
};

#endif
