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

#ifndef _MENAVIGATIONSPACE_H_
#define _MENAVIGATIONSPACE_H_

#include "../meColliderOwner.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decIntList.h>

class deRig;
class deDebugDrawer;
class deEngine;
class igdeEnvironment;
class igdeWObject;
class meWorld;
class igdeWDebugDrawerShape;
class deNavigationSpace;
class deColliderComponent;
class deComponent;



/**
 * \brief Navigation Space.
 */
class meNavigationSpace : public deObject{
private:
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSNavSpace;
	deNavigationSpace *pEngNavSpace;
	deColliderComponent *pEngCollider;
	deComponent *pEngColComponent;
	deRig *pEngRig;
	igdeWObject *pObjectPlaceholder;
	
	decDVector pPosition;
	decVector pOrientation;
	decString pFilename;
	
	decIntList pUsedCostTypes;
	
	bool pSelected;
	bool pActive;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new navigation space. */
	meNavigationSpace( igdeEnvironment *environment );
	/** \brief Cleans up the navigation space. */
	virtual ~meNavigationSpace();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the game engine. */
	deEngine *GetEngine() const;
	/** \brief Retrieves the engine collider. */
	inline deColliderComponent *GetEngineCollider() const{ return pEngCollider; }
	/** \brief Retrieves the engine navigation space. */
	inline deNavigationSpace *GetEngineNavSpace() const{ return pEngNavSpace; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld( meWorld *world );
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition( const decDVector &position );
	/** \brief Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation( const decVector &orientation );
	/** \brief Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/** \brief Sets the filename. */
	void SetFilename( const char *filename );
	
	/** \brief Retrieves the list of cost types used in the loaded navigation space. */
	inline decIntList &GetUsedCostTypes(){ return pUsedCostTypes; }
	inline const decIntList &GetUsedCostTypes() const{ return pUsedCostTypes; }
	
	/** \brief Determines if the navigation space is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the navigation space is selected. */
	void SetSelected( bool selected );
	/** \brief Determines if the navigation space is active. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the navigation space is active. */
	void SetActive( bool active );
	
	/** \brief Load Navigation Space from the file with the stored filename. */
	void LoadFromFile();
	/** \brief Save Navigation Space to the file with the stored filename. */
	void SaveToFile();
	
	/** \brief Notifies all that the navigation space changed. */
	void NotifyChanged();
	/** \brief Notifies all that the geometry changed. */
	void NotifyGeometryChanged();
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateShapes();
	void pUpdateDDSSpace();
	void pUpdateDDSColors();
	
	void pUpdateUsedCostTypes();
};

#endif
