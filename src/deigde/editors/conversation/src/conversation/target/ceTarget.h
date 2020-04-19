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

#ifndef _CETARGET_H_
#define _CETARGET_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceConversation;
class cePlayback;


/**
 * \brief Target.
 * Targets define a coordinate system for various conversation objects to
 * align with. A target can be an actor or a game entity and is called the
 * target object. Game entities allow to integrate conversations easily
 * into the dynamic flow of the game. An actor can be defined either using
 * an actor ID or the actor number. The position and orientation are relative
 * to the the coordinate system of the target object. If no target object is
 * defined the conversation coordinate system is used. If multiple target
 * object definitions exist the first defined one is used from this list:
 * entity id, actor id, actor index, conversation coordinate system.
 */
class ceTarget : public deObject{
private:
	ceConversation *pConversation;
	
	decString pName;
	
	decString pActor;
	decString pCoordSystem;
	decString pBone;
	decVector pPosition;
	decVector pOrientation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new target. */
	ceTarget( const char *name = "Target" );
	/** Creates a new target. */
	ceTarget( const ceTarget &target );
	/** Cleans up the target. */
	virtual ~ceTarget();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the conversation or NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the conversation or NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	
	/** Retrieves the actor ID or an empty string if not set. */
	inline const decString &GetActor() const{ return pActor; }
	/** Sets the actor ID or an empty string if not set. */
	void SetActor( const char *id );
	/** Retrieves the coordinate system ID or an empty string if not set. */
	inline const decString &GetCoordSystem() const{ return pCoordSystem; }
	/** Sets the coordinate system ID or an empty string if not set. */
	void SetCoordSystem( const char *id );
	
	/** Retrieves the bone name to use as coordinate frame or an empty string to use the origin. */
	inline const decString &GetBone() const{ return pBone; }
	/** Sets the bone name to use as coordinate frame or an empty string to use the origin. */
	void SetBone( const char *bone );
	/** Retrieves the position relative to the target object. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the start position relative to the target object. */
	void SetPosition( const decVector &position );
	/** Retrieves the orientation in degrees relative to the target object. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation in degrees relative to the target object. */
	void SetOrientation( const decVector &orientation );
	
	/** Retrieves the coordinate system for this target. */
	void GetCoordinateSystem( cePlayback &playback, decMatrix &coordinateSystem );
	/*@}*/
};

#endif
