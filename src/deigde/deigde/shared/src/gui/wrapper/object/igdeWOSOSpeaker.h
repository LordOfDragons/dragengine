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

#ifndef _IGDEWOSOSPEAKER_H_
#define _IGDEWOSOSPEAKER_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"
#include "../../../triggersystem/igdeTriggerExpressionReference.h"

#include <dragengine/resources/sound/deSpeakerReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDCSpeaker;


/**
 * \brief Object wrapper sub object.
 */
class igdeWOSOSpeaker : public igdeWOSubObject{
private:
	const igdeGDCSpeaker &pGDSpeaker;
	deSpeakerReference pSpeaker;
	igdeResourceLoaderListenerReference pResLoad;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	igdeTriggerExpressionReference pTriggerPlaying;
	igdeTriggerExpressionReference pTriggerMuted;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOSpeaker( igdeWObject &wrapper, const igdeGDCSpeaker &gdSpeaker, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Speaker resource. */
	inline deSpeaker *GetSpeaker() const{ return pSpeaker; }
	
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
	virtual void Visit( igdeWOSOVisitor &visitor );
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
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
