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

#ifndef _CECCONDITIONACTORPARAMETER_H_
#define _CECCONDITIONACTORPARAMETER_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Actor parameter conversation condition.
 */
class ceCConditionActorParameter : public ceConversationCondition{
public:
	/** \brief Compare operators. */
	enum eOperators{
		/** \brief True if the parameter value is equal to the test value. */
		eopEqual,
		
		/** \brief True if the parameter value is not equal to the test value. */
		eopNotEqual,
		
		/** \brief True if the parameter value is less than the test value. */
		eopLess,
		
		/** \brief True if the parameter value is less than or equal to the test value. */
		eopLessEqual,
		
		/** \brief True if the parameter value is greater than the test value. */
		eopGreater,
		
		/** \brief True if the parameter value is greater or equal to the test value. */
		eopGreaterEqual
	};
	
private:
	decString pActor;
	decString pParameter;
	eOperators pOperator;
	int pTestValue;
	decString pTestVariable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation condition. */
	ceCConditionActorParameter();
	
	/** \brief Create conversation condition. */
	ceCConditionActorParameter( const ceCConditionActorParameter &condition );
	
	/** \brief Clean up conversation condition. */
	virtual ~ceCConditionActorParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Actor id. */
	inline const decString &GetActor() const{ return pActor; }
	
	/** \brief Set actor id. */
	void SetActor( const char *id );
	
	/** \brief Name of parameter. */
	inline const decString &GetParameter() const{ return pParameter; }
	
	/** \brief Set name of parameter. */
	void SetParameter( const char *name );
	
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
