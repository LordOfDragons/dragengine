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

#ifndef _CECASETACTORPARAMETER_H_
#define _CECASETACTORPARAMETER_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Set Actor Parameter Conversation Action.
 */
class ceCASetActorParameter : public ceConversationAction{
public:
	typedef deTObjectReference<ceCASetActorParameter> Ref;
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
	ceCASetActorParameter(const ceCASetActorParameter &action);
	
	/** \brief Clean up conversation action. */
	~ceCASetActorParameter() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Actor id. */
	inline const decString &GetActor() const{ return pActor; }
	
	/** \brief Set actor id. */
	void SetActor(const char *id);
	
	/** \brief Name of parameter to update. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of parameter to update. */
	void SetName(const char *name);
	
	/** \brief Operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	
	/** \brief Set operator. */
	void SetOperator(eOperators aOperator);
	
	/** \brief Value to use for operation. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value to use for operation. */
	void SetValue(int value);
	
	/** \brief Variable to use as value for operation or empty string to use value. */
	inline const decString &GetValueVariable() const{ return pValueVariable; }
	
	/** \brief Set variable to use as value for operation or empty string to use value. */
	void SetValueVariable(const char *variable);
	
	
	
	/** Create a copy of this action. */
    ceConversationAction::Ref CreateCopy() const override;
	/*@}*/
};

#endif
