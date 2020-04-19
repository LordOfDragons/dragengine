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

#ifndef _DEFILERESOURCELIST_H_
#define _DEFILERESOURCELIST_H_

#include "deResourceList.h"

class deVirtualFileSystem;


/**
 * \brief File resource list.
 * 
 * Extends the resource list with a file resource specific check for the existence
 * of a file resource with a given name.
 */
class deFileResourceList : public deResourceList{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file resource list. */
	deFileResourceList();
	
	/** \brief Clean up resource list. */
	virtual ~deFileResourceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resource filename. */
	deResource *GetWithFilename( deVirtualFileSystem *vfs, const char *filename ) const;
	/*@}*/
};

#endif
