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

#ifndef _CECCONDITIONVARIABLE_H_
#define _CECCONDITIONVARIABLE_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Variable conversation condition.
 */
class ceCConditionVariable : public ceConversationCondition{
public:
	typedef deTObjectReference<ceCConditionVariable> Ref;

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
	ceCConditionVariable(const ceCConditionVariable &condition);
	
	/** \brief Clean up conversation condition. */
protected:
	~ceCConditionVariable() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of variable to test. */
	inline const decString &GetVariable() const{ return pVariable; }
	
	/** \brief Set name of variable to test. */
	void SetVariable(const char *name);
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator(eOperators aOperator);
	
	/** \brief Test value. */
	inline int GetTestValue() const{ return pTestValue; }
	
	/** \brief Set test value. */
	void SetTestValue(int value);
	
	/** \brief Test variable or empty string to use test value. */
	inline const decString &GetTestVariable() const{ return pTestVariable; }
	
	/** \brief Set test variable or empty string to use test value. */
	void SetTestVariable(const char *variable);
	
	
	
	/** \brief Create copy of condition. */
    ceConversationCondition::Ref CreateCopy() const override;
	/*@}*/
};

#endif
