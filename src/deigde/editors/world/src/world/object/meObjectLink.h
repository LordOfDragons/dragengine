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

#ifndef _MEOBJECTLINK_H_
#define _MEOBJECTLINK_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class meObject;
class meWorld;

class igdeEnvironment;
class igdeWConnection;

class deEngine;
class deDebugDrawer;
class deDebugDrawerShape;



/**
 * \brief Draws a link from one object to another.
 */
class meObjectLink : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<meObjectLink> Ref;
	
	
	
private:
	igdeEnvironment *pEnvironment;
	meWorld *pWorld;
	
	deDebugDrawer *pDebugDrawer;
	igdeWConnection *pDDSConnection;
	
	meObject *pAnchor;
	meObject *pTarget;
	decString pAnchorProperty;
	decString pTargetProperty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new object link. */
	meObjectLink( igdeEnvironment *environment, meObject *anchor, meObject *target );
	/** \brief Cleans up an object link. */
	virtual ~meObjectLink();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	/** \brief Retrieves the anchor object. */
	inline meObject *GetAnchor() const{ return pAnchor; }
	/** \brief Retrieves the target object. */
	inline meObject *GetTarget() const{ return pTarget; }
	
	/** \brief Retrieves the anchor property name. */
	inline const decString &GetAnchorProperty() const{ return pAnchorProperty; }
	/** \brief Sets the anchor property name. */
	void SetAnchorProperty( const char *property );
	/** \brief Retrieves the target property name. */
	inline const decString &GetTargetProperty() const{ return pTargetProperty; }
	/** \brief Sets the target property name. */
	void SetTargetProperty( const char *property );
	
	/** \brief Retrieves the world this link resides in. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world this link resides in. */
	void SetWorld( meWorld *world );
	
	/** \brief Notifies that the anchor or target moved. */
	void ObjectsMoved();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateConnection();
};

#endif
