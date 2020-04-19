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

#ifndef _CEPOSE_H_
#define _CEPOSE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Pose.
 * A pose is a state of an actor. Every pose has an idle animation assigned
 * which is played back as long as the actor is in this pose state. While
 * changing the pose the old and new pose are blended over. The transition
 * between two poses is usually hidden using a pose leading from the old
 * pose to the new pose. This way the pose can be changed using only one
 * animation rule in the animator.
 */
class cePose : public deObject{
private:
	ceConversation *pConversation;
	
	decString pName;
	decString pMove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new pose. */
	cePose( const char *name = "Pose" );
	/** Cleans up the pose. */
	virtual ~cePose();
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
	/** Retrieves the move to use. */
	inline const decString &GetMove() const{ return pMove; }
	/** Sets the move to use. */
	void SetMove( const char *move );
	/*@}*/
};

#endif
