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

#ifndef _MEOBJECTSNAPPOINT_H_
#define _MEOBJECTSNAPPOINT_H_

#include "../meColliderOwner.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

class meWorld;
class meColliderOwner;
class igdeGDCSnapPoint;
class igdeWDebugDrawerShape;

class deColliderVolume;



/**
 * \brief Object snap point.
 */
class meObjectSnapPoint : public deObject{
private:
	meWorld *pWorld;
	meObject *pObject;
	igdeGDCSnapPoint *pSnapPoint;
	
	igdeWDebugDrawerShape *pDDShape;
	deColliderVolume *pCollider;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create snap point. */
	meObjectSnapPoint( meObject *object, igdeGDCSnapPoint *snapPoint );
	
	/** \brief Clean up snap point. */
	virtual ~meObjectSnapPoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Object. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Snap point. */
	inline igdeGDCSnapPoint *GetSnapPoint() const{ return pSnapPoint; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
