/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECONTAINERFILESEARCH_H_
#define _DECONTAINERFILESEARCH_H_

#include "deVFSContainer.h"


/**
 * \brief Container file search.
 * 
 * Used with deVFSContainer::SearchFiles() call.
 */
class DE_DLL_EXPORT deContainerFileSearch{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container file search. */
	deContainerFileSearch();
	
	/** \brief Clean up container file search. */
	virtual ~deContainerFileSearch();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Add file. */
	virtual void Add( const char *name, deVFSContainer::eFileTypes type ) = 0;
	
	/** \brief Remove file. */
	virtual void Remove( const char *name ) = 0;
	/*@}*/
};

#endif
