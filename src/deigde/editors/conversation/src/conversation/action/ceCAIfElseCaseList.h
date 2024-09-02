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

#ifndef _CECAIFELSECASELIST_H_
#define _CECAIFELSECASELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCAIfElseCase;



/**
 * \brief If-Else Case List.
 */
class ceCAIfElseCaseList{
private:
	decObjectOrderedSet pCases;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create case list. */
	ceCAIfElseCaseList();
	
	/** \brief Clean up case list. */
	~ceCAIfElseCaseList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of cases. */
	int GetCount() const;
	
	/** \brief Case at index. */
	ceCAIfElseCase *GetAt( int index ) const;
	
	/** \brief Index of case or -1 if not found. */
	int IndexOf( ceCAIfElseCase *ifcase ) const;
	
	/** \brief Case is present. */
	bool Has( ceCAIfElseCase *ifcase ) const;
	
	/** \brief Add case. */
	void Add( ceCAIfElseCase *ifcase );
	
	/** \brief Insert case. */
	void InsertAt( ceCAIfElseCase *ifcase, int index );
	
	/** \brief Move case to new position. */
	void MoveTo( ceCAIfElseCase *ifcase, int index );
	
	/** \brief Remove case. */
	void Remove( ceCAIfElseCase *ifcase );
	
	/** \brief Remove all cases. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	ceCAIfElseCaseList &operator=( const ceCAIfElseCaseList &list );
	/*@}*/
};

#endif
