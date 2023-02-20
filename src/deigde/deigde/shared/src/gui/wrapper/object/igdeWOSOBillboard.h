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

#ifndef _IGDEWOSOBILLBOARD_H_
#define _IGDEWOSOBILLBOARD_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"

#include <dragengine/resources/billboard/deBillboardReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDCBillboard;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOBillboard : public igdeWOSubObject{
private:
	const igdeGDCBillboard &pGDBillboard;
	deBillboardReference pBillboard;
	igdeResourceLoaderListenerReference pResLoad;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	decVector2 pBaseOffset;
	bool pRenderEnvMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOBillboard( igdeWObject &wrapper, const igdeGDCBillboard &gdBillboard, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Billboard resource. */
	inline deBillboard *GetBillboard() const{ return pBillboard; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Geometry changed. */
	virtual void UpdateGeometry();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Camera changed. */
	virtual void CameraChanged();
	
	/** \brief Frame update. */
	virtual void Update( float elapsed );
	
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
	void pUpdateBillboard();
	void pDestroyBillboard();
	void pUpdateExtends();
};

#endif
