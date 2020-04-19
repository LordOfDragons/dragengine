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

#ifndef _CECONVERSATIONACTION_H_
#define _CECONVERSATIONACTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decIntSet.h>



/**
 * \brief Conversation Action Base Class.
 */
class ceConversationAction : public deObject{
public:
	/** \brief Action types. */
	enum eActionTypes{
		/** \brief Change camera shot. */
		eatCameraShot,
		/** \brief Change music. */
		eatMusic,
		/** \brief Actor speaking. */
		eatActorSpeak,
		/** \brief Process one of two lists of actions depending on a condition. */
		eatIfElse,
		/** \brief Player can pick from a list of choices processing the matching actions. */
		eatPlayerChoice,
		/** \brief Stops the conversation. */
		eatStopConversation,
		/** \brief Stops the topic. */
		eatStopTopic,
		/** \brief Use a named snippet of actions. */
		eatSnippet,
		/** \brief Sets variable. */
		eatSetVariable,
		/** \brief Sets actor parameter. */
		eatSetActorParameter,
		/** \brief Send command to an actor. */
		eatActorCommand,
		/** \brief Send command to the game. */
		eatGameCommand,
		/** \brief Wait for a condition to become true. */
		eatWait,
		/** \brief Operate a trigger. */
		eatTrigger,
		/** \brief Adds an actor to the conversation. */
		eatActorAdd,
		/** \brief Removes an actor from the conversation. */
		eatActorRemove,
		/** \brief Adds a coordinate system to the conversation. */
		eatCoordSystemAdd,
		/** \brief Removes a coordinate system from the conversation. */
		eatCoordSystemRemove,
		/** \brief Comment. */
		eatComment,
		/** \brief Number of action types. */
		EAT_COUNT
	};
	
private:
	eActionTypes pType;
	bool pWaitForActor;
	decString pWaitForActorID;
	float pDelay;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceConversationAction( eActionTypes type );
	/** \brief Creates a new conversation action. */
	ceConversationAction( const ceConversationAction &action );
	/** \brief Cleans up the conversation action. */
	virtual ~ceConversationAction();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the type. */
	inline eActionTypes GetType() const{ return pType; }
	/** \brief Wait for actor. */
	inline bool GetWaitForActor() const{ return pWaitForActor; }
	/** \brief Set to wait for actor. */
	void SetWaitForActor( bool wait );
	/** \brief ID of actor to wait for or empty string to wait for all actors. */
	inline const decString &GetWaitForActorID() const{ return pWaitForActorID; }
	/** \brief Set ID of actor to wait for or empty string to wait for all actors. */
	void SetWaitForActorID( const char *id );
	/** \brief Retrieves the delay in seconds before processing this action. */
	inline float GetDelay() const{ return pDelay; }
	/** \brief Sets the delay in seconds before processing this action. */
	void SetDelay( float delay );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const = 0;
	/*@}*/
};

#endif
