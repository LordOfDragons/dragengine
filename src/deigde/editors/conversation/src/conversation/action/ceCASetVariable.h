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

#ifndef _CECASETVARIABLE_H_
#define _CECASETVARIABLE_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Set Variable Conversation Action.
 */
class ceCASetVariable : public ceConversationAction{
public:
	/** \brief Operator. */
	enum eOperators{
		/** \brief Set variable. */
		eopSet,
		
		/** \brief Increment variable. */
		eopIncrement,
		
		/** \brief Decrement variable. */
		eopDecrement,
		
		/** \brief Randomize variable. */
		eopRandom,
		
		/** \brief Dummy, count of operators. */
		EOP_COUNT
	};
	
private:
	decString pName;
	eOperators pOperator;
	int pValue;
	decString pValueVariable;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation action. */
	ceCASetVariable();
	
	/** \brief Create conversation action. */
	ceCASetVariable( const ceCASetVariable &action );
	
	/** \brief Clean up conversation action. */
	virtual ~ceCASetVariable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of variable to set. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of variable to set. */
	void SetName( const char *name );
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator( eOperators aOperator );
	
	/** \brief Value used for operation. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value used for operation. */
	void SetValue( int value );
	
	/** \brief Variable used for operation or empty string to use value. */
	inline const decString &GetValueVariable() const{ return pValueVariable; }
	
	/** \brief Set variable used for operation or empty string to use value. */
	void SetValueVariable( const char *value );
	
	
	
	/** \brief Create copy of action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
