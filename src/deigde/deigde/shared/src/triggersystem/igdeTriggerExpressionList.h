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

#ifndef _IGDETRIGGEREXPRESSIONLIST_H_
#define _IGDETRIGGEREXPRESSIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTriggerExpression;



/**
 * \brief Trigger Expression List.
 */
class igdeTriggerExpressionList{
private:
	decObjectOrderedSet pExpressions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeTriggerExpressionList();
	
	/** \brief Clean up list. */
	~igdeTriggerExpressionList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of expressions. */
	int GetCount() const;
	
	/** \brief Expression at the given position. */
	igdeTriggerExpression *GetAt( int position ) const;
	
	/** \brief Index of the given expression or -1 if not found. */
	int IndexOf( igdeTriggerExpression *expression ) const;
	
	/** \brief Determines if the given expression exists. */
	bool Has( igdeTriggerExpression *expression ) const;
	
	/** \brief Adds an expression throwing an exception if already existing. */
	void Add( igdeTriggerExpression *expression );
	
	/** \brief Adds an expression if not existing already. */
	void AddIfAbsent( igdeTriggerExpression *expression );
	
	/** \brief Removes an expression throwing an exception if not existing. */
	void Remove( igdeTriggerExpression *expression );
	
	/** \brief Removes an expression if existing. */
	void RemoveIfPresent( igdeTriggerExpression *expression );
	
	/** \brief Removes all expressions. */
	void RemoveAll();
	/*@}*/
};

#endif
