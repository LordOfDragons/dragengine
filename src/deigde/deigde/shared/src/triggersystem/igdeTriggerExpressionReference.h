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

#ifndef _IGDETRIGGEREXPRESSIONREFERENCE_H_
#define _IGDETRIGGEREXPRESSIONREFERENCE_H_

#include <dragengine/deObjectReference.h>

class igdeTriggerExpression;


/**
 * \brief Trigger expression reference.
 * 
 * Safe way to hold an expression reference with correct reference handling.
 * Storing NULL is allowed. Initial value is NULL.
 * 
 * \note This class is designed to be used as stack or member expression. Do not use
 *       it as pointer and memory allocate it. For this reason the destructor
 *       is on purpose not virtual.
 */
class DE_DLL_EXPORT igdeTriggerExpressionReference : protected deObjectReference{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty expression reference holder. */
	igdeTriggerExpressionReference();
	
	/**
	 * \brief Create expression reference holder with expression.
	 * 
	 * Reference is added if expression is not NULL.
	 */
	igdeTriggerExpressionReference( igdeTriggerExpression *expression );
	
	/**
	 * \brief Create expression reference holder with expression from another holder.
	 * 
	 * Reference is added if expression in holder is not NULL.
	 */
	igdeTriggerExpressionReference( const igdeTriggerExpressionReference &reference );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set expression without adding reference.
	 * 
	 * Use this method if the expression to hold has been added a reference already. This is
	 * the case with created expressions as well as certain methods returning newly created
	 * expressions. In all these cases the expression has to be held without adding a reference.
	 * For all other situations use the constructor or assignment operator.
	 * 
	 * It is allowed for \em expression to be a NULL expression.
	 */
	void TakeOver( igdeTriggerExpression *expression );
	
	/** \brief Object is NULL. */
	bool operator!() const;
	
	/** \brief Test if expression is not NULL. */
	operator bool() const;
	
	/** \brief Pointer to expression. */
	operator igdeTriggerExpression*() const;
	
	/**
	 * \brief Reference to expression.
	 * 
	 * \throws deeNullPointer if expression is NULL.
	 */
	operator igdeTriggerExpression&() const;
	
	/**
	 * \brief Pointer to expression.
	 * 
	 * \throws deeNullPointer if expression is NULL.
	 */
	igdeTriggerExpression* operator->() const;
	
	/**
	 * \brief Store expression.
	 * 
	 * If an expression is already held its reference is release and the new expression
	 * stored. If the new expression is not NULL a reference is added.
	 */
	igdeTriggerExpressionReference &operator=( igdeTriggerExpression *expression );
	
	/**
	 * \brief Store expression.
	 * 
	 * If an expression is already held its reference is release and the new expression
	 * stored. If the new expression is not NULL a reference is added.
	 */
	igdeTriggerExpressionReference &operator=( const igdeTriggerExpressionReference &reference );
	
	/** \brief Test if expression is held by this holder. */
	bool operator==( igdeTriggerExpression *expression ) const;
	bool operator!=( igdeTriggerExpression *expression ) const;
	
	/** \brief Test if expression is held by this holder. */
	bool operator==( const igdeTriggerExpressionReference &reference ) const;
	bool operator!=( const igdeTriggerExpressionReference &reference ) const;
	/*@}*/
};

#endif
