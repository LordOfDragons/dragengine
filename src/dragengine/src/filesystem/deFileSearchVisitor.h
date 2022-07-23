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

#ifndef _DEFILESEARCHVISITOR_H_
#define _DEFILESEARCHVISITOR_H_

#include "../dragengine_export.h"

class decPath;
class deVirtualFileSystem;


/**
 * \brief File search visitor.
 * 
 * Used with deVirtualFileSystem::SearchFiles() call.
 */
class DE_DLL_EXPORT deFileSearchVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file search visitor. */
	deFileSearchVisitor();
	
	/** \brief Clean up file search visitor. */
	virtual ~deFileSearchVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * \brief Visit regular file.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full file path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path );
	
	/**
	 * \brief Visit directory.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full directory path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path );
	
	/**
	 * \brief Visit special.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full file path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	virtual bool VisitSpecial( const deVirtualFileSystem &vfs, const decPath &path );
	/*@}*/
};

#endif
