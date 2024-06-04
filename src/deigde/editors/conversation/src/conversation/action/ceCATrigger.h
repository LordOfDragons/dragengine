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

#ifndef _CECATRIGGER_H_
#define _CECATRIGGER_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Trigger Conversation Action.
 */
class ceCATrigger : public ceConversationAction{
public:
	/** \brief Action. */
	enum eActions{
		/** \brief Fire trigger. */
		eaFire,
		/** \brief Reset trigger. */
		eaReset,
		/** \brief Fire then immediately reset trigger. */
		eaPulse,
		/** \brief Full reset trigger. */
		eaFullReset,
		/** \brief Dummy, count of actions. */
		EA_COUNT
	};
	
private:
	decString pName;
	eActions pAction;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new trigger conversation action. */
	ceCATrigger();
	/** \brief Creates a new trigger conversation action. */
	ceCATrigger( const ceCATrigger &action );
	/** \brief Cleans up the trigger conversation action. */
	virtual ~ceCATrigger();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the name of the trigger. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the name of the trigger. */
	void SetName( const char *name );
	/** \brief Retrieves the action. */
	inline eActions GetAction() const{ return pAction; }
	/** \brief Sets the action. */
	void SetAction( eActions action );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
