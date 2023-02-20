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

#ifndef _IGDEWOSONAVIGATIONSPACE_H_
#define _IGDEWOSONAVIGATIONSPACE_H_

#include "igdeWOSubObject.h"

#include <dragengine/resources/navigation/space/deNavigationSpaceReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDCNavigationSpace;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSONavigationSpace : public igdeWOSubObject{
private:
	const igdeGDCNavigationSpace &pGDNavigationSpace;
	deNavigationSpaceReference pNavigationSpace;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	decString pPathNavigationSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSONavigationSpace( igdeWObject &wrapper, const igdeGDCNavigationSpace &gdNavigationSpace, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSONavigationSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief NavigationSpace resource. */
	inline deNavigationSpace *GetNavigationSpace() const{ return pNavigationSpace; }
	
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
	void pUpdateNavigationSpace();
	void pDestroyNavigationSpace();
};

#endif
