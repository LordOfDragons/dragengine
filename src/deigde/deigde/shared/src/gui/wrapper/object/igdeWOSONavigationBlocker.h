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

#ifndef _IGDEWOSONAVIGATIONBLOCKER_H_
#define _IGDEWOSONAVIGATIONBLOCKER_H_

#include "igdeWOSubObject.h"

#include <dragengine/resources/navigation/blocker/deNavigationBlockerReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDCNavigationBlocker;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSONavigationBlocker : public igdeWOSubObject{
private:
	const igdeGDCNavigationBlocker &pGDNavigationBlocker;
	deNavigationBlockerReference pNavigationBlocker;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSONavigationBlocker( igdeWObject &wrapper, const igdeGDCNavigationBlocker &gdNavigationBlocker, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSONavigationBlocker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief NavigationBlocker resource. */
	inline deNavigationBlocker *GetNavigationBlocker() const{ return pNavigationBlocker; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
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
	void pUpdateNavigationBlocker();
	void pDestroyNavigationBlocker();
};

#endif
