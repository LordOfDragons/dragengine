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

#ifndef _IGDETRIGGEREXPRESSIONLIST_H_
#define _IGDETRIGGEREXPRESSIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTriggerExpression;



/**
 * \brief Trigger Expression List.
 */
class DE_DLL_EXPORT igdeTriggerExpressionList{
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
