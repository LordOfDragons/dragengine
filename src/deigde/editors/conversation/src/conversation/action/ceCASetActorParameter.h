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

#ifndef _CECASETACTORPARAMETER_H_
#define _CECASETACTORPARAMETER_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Set Actor Parameter Conversation Action.
 */
class ceCASetActorParameter : public ceConversationAction{
public:
	/** \brief Operator. */
	enum eOperators{
		/** \brief Set parameter. */
		eopSet,
		
		/** \brief Increment parameter. */
		eopIncrement,
		
		/** \brief Decrement parameter. */
		eopDecrement,
		
		/** \brief Randomize parameter. */
		eopRandom,
		
		/** \brief Dummy, count of operators. */
		EOP_COUNT
	};
	
private:
	decString pActor;
	decString pName;
	eOperators pOperator;
	int pValue;
	decString pValueVariable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation action. */
	ceCASetActorParameter();
	
	/** \brief Create conversation action. */
	ceCASetActorParameter( const ceCASetActorParameter &action );
	
	/** \brief Clean up conversation action. */
	virtual ~ceCASetActorParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Actor id. */
	inline const decString &GetActor() const{ return pActor; }
	
	/** \brief Set actor id. */
	void SetActor( const char *id );
	
	/** \brief Name of parameter to update. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of parameter to update. */
	void SetName( const char *name );
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator( eOperators aOperator );
	
	/** \brief Value to use for operation. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value to use for operation. */
	void SetValue( int value );
	
	/** \brief Variable to use as value for operation or empty string to use value. */
	inline const decString &GetValueVariable() const{ return pValueVariable; }
	
	/** \brief Set variable to use as value for operation or empty string to use value. */
	void SetValueVariable( const char *variable );
	
	
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
