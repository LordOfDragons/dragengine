/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECOORDSYSTEM_H_
#define _CECOORDSYSTEM_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Conversation coordinate system.
 */
class ceCoordSystem : public deObject{
private:
	ceConversation *pConversation;
	
	decString pID;
	decString pAliasID;
	decVector pPosition;
	decVector pOrientation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new coordinate system. */
	ceCoordSystem();
	/** \brief Cleans up the conversation actor. */
	virtual ~ceCoordSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** \brief Sets the parent conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Retrieves the id. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the id. */
	void SetID( const char *id );
	/** \brief Retrieves the alias id. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** \brief Sets the alias id. */
	void SetAliasID( const char *id );
	/** \brief Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition( const decVector &position );
	/** \brief Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Notifies all that the coordinate system changed. */
	void NotifyCoordSystemChanged();
	/*@}*/
};

#endif
