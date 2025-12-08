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

#ifndef _IGDEWOSOSPEAKER_H_
#define _IGDEWOSOSPEAKER_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../triggersystem/igdeTriggerExpression.h"

#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/collider/deCollider.h>


class deColliderAttachment;
class igdeGDCSpeaker;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOSpeaker : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOSpeaker> Ref;
	
	
private:
	const igdeGDCSpeaker &pGDSpeaker;
	deSpeaker::Ref pSpeaker;
	igdeResourceLoaderListener::Ref pResLoad;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	igdeTriggerExpression::Ref pTriggerPlaying;
	igdeTriggerExpression::Ref pTriggerMuted;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOSpeaker(igdeWObject &wrapper, const igdeGDCSpeaker &gdSpeaker, const decString &prefix);
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Speaker resource. */
	inline const deSpeaker::Ref &GetSpeaker() const{ return pSpeaker; }
	
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
	void pUpdateSpeaker();
	void pDestroySpeaker();
	bool pEvalPlaying();
};

#endif
