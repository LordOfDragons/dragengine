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

#ifndef _CECONVERSATIONACTORLIST_H_
#define _CECONVERSATIONACTORLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationActor;



/**
 * \brief Conversation Actor List.
 */
class ceConversationActorList{
private:
	decObjectOrderedSet pActors;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new actor list. */
	ceConversationActorList();
	/** \brief Cleans up the actor list. */
	~ceConversationActorList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of actors. */
	int GetCount() const;
	/** \brief Retrieves the actor at the given position. */
	ceConversationActor *GetAt(int index) const;
	/** \brief Retrieves the actor with the given id or NULL if not found. */
	ceConversationActor *GetWithID(const char *id) const;
	/** \brief Retrieves the actor with the given alias id or NULL if not found. */
	ceConversationActor *GetWithAliasID(const char *id) const;
	/** \brief Retrieves the actor with the given id or alias id or NULL if not found. */
	ceConversationActor *GetWithIDOrAliasID(const char *id) const;
	/** \brief Retrieves the index of the given actor or -1 if not found. */
	int IndexOf(ceConversationActor *actor) const;
	/** \brief Retrieves the index of the actor with the given id or -1 if not found. */
	int IndexWithID(const char *id) const;
	/** \brief Retrieves the index of the actor with the given alias id or -1 if not found. */
	int IndexWithAliasID(const char *id) const;
	/** \brief Retrieves the index of the actor with the given id or alias id or -1 if not found. */
	int IndexWithIDOrAliasID(const char *id) const;
	/** \brief Determines if a actor exists. */
	bool Has(ceConversationActor *actor) const;
	/** \brief Determines if an actor with the given id exists. */
	bool HasWithID(const char *id) const;
	/** \brief Determines if an actor with the given alias id exists. */
	bool HasWithAliasID(const char *id) const;
	/** \brief Determines if an actor with the given id or alias id exists. */
	bool HasWithIDOrAliasID(const char *id) const;
	/** \brief Adds a actor. */
	void Add(ceConversationActor *actor);
	/** \brief Inserts a new actor. */
	void InsertAt(ceConversationActor *actor, int index);
	/** \brief Moves a actor to a new position. */
	void MoveTo(ceConversationActor *actor, int index);
	/** \brief Removes a actor. */
	void Remove(ceConversationActor *actor);
	/** \brief Removes all actors. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	ceConversationActorList &operator=(const ceConversationActorList &list);
	/*@}*/
};

#endif
