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

#ifndef _CECCONDITIONTRIGGER_H_
#define _CECCONDITIONTRIGGER_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Conversation condition testing trigger state.
 */
class ceCConditionTrigger : public ceConversationCondition{
public:
	/** \brief Test mode. */
	enum eTestModes{
		/** \brief True if the trigger is in fired state. */
		etmFired,
		
		/** \brief True if the trigger is in reset state. */
		etmNotFired,
		
		/** \brief True if the trigger has ever been fired. */
		etmEverFired,
		
		/** \brief True if the trigger has never been fired. */
		etmNeverFired,
	};
	
	
	
private:
	decString pTrigger;
	eTestModes pTestMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation condition. */
	ceCConditionTrigger();
	
	/** \brief Creates a new conversation condition. */
	ceCConditionTrigger( const ceCConditionTrigger &condition );
	
	/** \brief Cleans up the conversation condition. */
	virtual ~ceCConditionTrigger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the trigger to test. */
	inline const decString &GetTrigger() const{ return pTrigger; }
	
	/** \brief Set name of the trigger to test. */
	void SetTrigger( const char *name );
	
	/** \brief Test mode. */
	inline eTestModes GetTestMode() const{ return pTestMode; }
	
	/** \brief Set test mode. */
	void SetTestMode( eTestModes testMode );
	
	/** \brief Create copy of condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
