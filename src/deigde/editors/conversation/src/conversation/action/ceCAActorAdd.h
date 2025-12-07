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

#ifndef _CECAACTORADD_H_
#define _CECAACTORADD_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Add actor conversation action.
 * \details Adds an actor to the conversation if not existing already.
 */
class ceCAActorAdd : public ceConversationAction{
private:
	decString pID;
	decString pAliasID;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCAActorAdd();
	/** \brief Creates a new conversation action. */
	ceCAActorAdd(const ceCAActorAdd &action);
	/** \brief Cleans up the conversation action. */
	virtual ~ceCAActorAdd();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the actor id. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the actor id. */
	void SetID(const char *id);
	/** \brief Retrieves the alias id or an empty string if not used. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** \brief Sets the alias id or an empty string if not used. */
	void SetAliasID(const char *id);
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
