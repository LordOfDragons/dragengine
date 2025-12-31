/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _CECONVERSATIONACTION_H_
#define _CECONVERSATIONACTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTSet.h>


/**
 * \brief Conversation Action Base Class.
 */
class ceConversationAction : public deObject{
public:
	typedef deTObjectReference<ceConversationAction> Ref;
	typedef decTObjectOrderedSet<ceConversationAction> List;
	
	
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
		eatComment
	};
	
private:
	eActionTypes pType;
	bool pWaitForActor, pWaitSpeakOnly;
	decString pWaitForActorID;
	float pDelay;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceConversationAction(eActionTypes type);
	/** \brief Creates a new conversation action. */
	ceConversationAction(const ceConversationAction &action);
	/** \brief Cleans up the conversation action. */
protected:
	virtual ~ceConversationAction();
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the type. */
	inline eActionTypes GetType() const{ return pType; }
	
	/** \brief Wait for actor. */
	inline bool GetWaitForActor() const{ return pWaitForActor; }
	
	/** \brief Set to wait for actor. */
	void SetWaitForActor(bool wait);
	
	/** \brief Waiting considers only actor speaking not actor waiting. */
	inline bool GetWaitSpeakOnly() const{ return pWaitSpeakOnly; }
	
	/** \brief Set waiting considers only actor speaking not actor waiting. */
	void SetWaitSpeakOnly(bool speakOnly);
	
	/** \brief ID of actor to wait for or empty string to wait for all actors. */
	inline const decString &GetWaitForActorID() const{ return pWaitForActorID; }
	
	/** \brief Set ID of actor to wait for or empty string to wait for all actors. */
	void SetWaitForActorID(const char *id);
	
	/** \brief Retrieves the delay in seconds before processing this action. */
	inline float GetDelay() const{ return pDelay; }
	
	/** \brief Sets the delay in seconds before processing this action. */
	void SetDelay(float delay);
	
	/** \brief Create a copy of this action. */
	virtual Ref CreateCopy() const = 0;
	/*@}*/
};

#endif
