/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDENATIVENULLTABBOOK_H_
#define _IGDENATIVENULLTABBOOK_H_

#include "../../igdeTabBook.h"


/**
 * Null tabBook.
 */
class igdeNativeNullTabBook{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullTabBook();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullTabBook();
	
	/** \brief Create native widget. */
	static igdeNativeNullTabBook* CreateNativeWidget( igdeTabBook &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void AddHeader( const igdeTabBook::cHeader &header );
	virtual void RemoveHeader( int index );
	virtual void RemoveAllHeaders();
	virtual void ChangePanel( int index );
	virtual void *GetNativeContainer();
	/*@}*/
};

typedef igdeNativeNullTabBook igdeNativeTabBook;

#endif
