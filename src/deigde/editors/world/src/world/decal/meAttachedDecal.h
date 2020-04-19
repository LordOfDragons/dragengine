/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEATTACHEDDECAL_H_
#define _MEATTACHEDDECAL_H_

#include <dragengine/resources/decal/deDecalReference.h>

class meDecal;
class meObject;

class deEngine;
class deSkin;



/**
 * \brief Attached Decal.
 */
class meAttachedDecal{
private:
	deEngine *pEngine;
	deDecalReference pEngDecal;
	
	meDecal *pDecal;
	meObject *pParentObject;
	bool pAttached;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create attached decal. */
	meAttachedDecal( deEngine *engine, meDecal *decal );
	
	/** \brief Clean up attached decal. */
	~meAttachedDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Engine decal or NULL. */
	inline deDecal *GetEngineDecal() const{ return pEngDecal; }
	
	/** \brief Parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	
	/** \brief Set parent object. */
	void SetParentObject( meObject *object );
	
	/** \brief Attach decal to parent. */
	void AttachToParent();
	
	/** \brief Remove decal from the parent. */
	void RemoveFromParent();
	/*@}*/
};

#endif
