/* 
 * Drag[en]gine IGDE
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
class igdeTriggerExpressionReference : protected deObjectReference{
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
