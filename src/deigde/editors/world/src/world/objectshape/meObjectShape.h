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

#ifndef _MEOBJECTSHAPE_H_
#define _MEOBJECTSHAPE_H_

#include "../meColliderOwner.h"
#include "../object/meObject.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class meObject;
class meWorld;

class igdeWDebugDrawerShape;
class igdeEnvironment;

class deColliderVolume;
class deDebugDrawer;
class deEngine;
class decShape;



/**
 * \brief Object Shape.
 * \details A single shape in an object shape. Whereas the meObjectShapeList corresponds to a decShapeList the
 *          meObjectShape corresponds to an actual decShape inside the list. The shape is located in the coordinate
 *          system of the world. The user is responsible to transform the shape from and to the world coordinate
 *          system and whatever target element holds the object shape list.
 */
class meObjectShape : public deObject{
private:
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	meObject *pParentObject;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSShape;
	deColliderVolume *pEngCollider;
	
	decShape *pShape;
	
	bool pSelected;
	bool pActive;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new object shape. */
	meObjectShape( igdeEnvironment *environment, const decShape &shape );
	/** \brief Cleans up the object shape. */
	virtual ~meObjectShape();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Retrieves the engine collider. */
	inline deColliderVolume *GetEngineCollider() const{ return pEngCollider; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld( meWorld *world );
	/** \brief Retrieves the parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	/** \brief Sets the parent object or NULL. */
	void SetParentObject( meObject *parentObject );
	
	/** \brief Retrieves the shape. */
	inline const decShape *GetShape() const{ return pShape; }
	/** \brief Sets the shape. Make sure it is of the matching shape type. */
	void SetShape( const decShape &shape );
	
	/** \brief Determines if the shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the shape is selected. */
	void SetSelected( bool selected );
	/** \brief Determines if the shape is active. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the shape is active. */
	void SetActive( bool active );
	
	/** \brief Update collider and debug drawer shape, position, orientation and scaling. */
	void UpdateShape();
	/** \brief Update debug drawer colors. */
	void UpdateDDSColors();
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
