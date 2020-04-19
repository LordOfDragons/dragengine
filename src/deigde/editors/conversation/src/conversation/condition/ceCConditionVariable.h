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

#ifndef _CECCONDITIONVARIABLE_H_
#define _CECCONDITIONVARIABLE_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Variable conversation condition.
 */
class ceCConditionVariable : public ceConversationCondition{
public:
	/** \brief Compare operators. */
	enum eOperators{
		/** \brief True if the variable value is equal to the test value. */
		eopEqual,
		
		/** \brief True if the variable value is not equal to the test value. */
		eopNotEqual,
		
		/** \brief True if the variable value is less than the test value. */
		eopLess,
		
		/** \brief True if the variable value is less than or equal to the test value. */
		eopLessEqual,
		
		/** \brief True if the variable value is greater than the test value. */
		eopGreater,
		
		/** \brief True if the variable value is greater or equal to the test value. */
		eopGreaterEqual
	};
	
	
	
private:
	decString pVariable;
	eOperators pOperator;
	int pTestValue;
	decString pTestVariable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation condition. */
	ceCConditionVariable();
	
	/** \brief Create conversation condition. */
	ceCConditionVariable( const ceCConditionVariable &condition );
	
	/** \brief Clean up conversation condition. */
	virtual ~ceCConditionVariable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of variable to test. */
	inline const decString &GetVariable() const{ return pVariable; }
	
	/** \brief Set name of variable to test. */
	void SetVariable( const char *name );
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator( eOperators aOperator );
	
	/** \brief Test value. */
	inline int GetTestValue() const{ return pTestValue; }
	
	/** \brief Set test value. */
	void SetTestValue( int value );
	
	/** \brief Test variable or empty string to use test value. */
	inline const decString &GetTestVariable() const{ return pTestVariable; }
	
	/** \brief Set test variable or empty string to use test value. */
	void SetTestVariable( const char *variable );
	
	
	
	/** \brief Create copy of condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
