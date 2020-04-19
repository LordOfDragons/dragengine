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
