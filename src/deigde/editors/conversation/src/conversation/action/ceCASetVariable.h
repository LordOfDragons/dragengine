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

#ifndef _CECASETVARIABLE_H_
#define _CECASETVARIABLE_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Set Variable Conversation Action.
 */
class ceCASetVariable : public ceConversationAction{
public:
	typedef deTObjectReference<ceCASetVariable> Ref;
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
	ceCASetVariable(const ceCASetVariable &action);
	
	/** \brief Clean up conversation action. */
protected:
	~ceCASetVariable() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of variable to set. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of variable to set. */
	void SetName(const char *name);
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator(eOperators aOperator);
	
	/** \brief Value used for operation. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value used for operation. */
	void SetValue(int value);
	
	/** \brief Variable used for operation or empty string to use value. */
	inline const decString &GetValueVariable() const{ return pValueVariable; }
	
	/** \brief Set variable used for operation or empty string to use value. */
	void SetValueVariable(const char *value);
	
	
	
	/** \brief Create copy of action. */
    ceConversationAction::Ref CreateCopy() const override;
	/*@}*/
};

#endif
