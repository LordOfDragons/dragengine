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

#ifndef _DECOLLECTDIRECTORYSEARCHVISITOR_H_
#define _DECOLLECTDIRECTORYSEARCHVISITOR_H_

#include "dePathList.h"
#include "deFileSearchVisitor.h"


/**
 * \brief File search visitor collecting visited directories.
 * 
 * Used with deVirtualFileSystem::SearchFiles() call. Stores visited directories.
 * Does recursion into directories by default.
 */
class deCollectDirectorySearchVisitor : public deFileSearchVisitor{
private:
	bool pRecursion;
	dePathList pDirectories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file search visitor. */
	deCollectDirectorySearchVisitor( bool recursive = false );
	
	/** \brief Clean up file search visitor. */
	virtual ~deCollectDirectorySearchVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Enable recursion. */
	inline bool GetRecursion() const{ return pRecursion; }
	
	/** \brief Set if recursion is enabled. */
	void SetRecursion( bool recursion );
	
	/** \brief Found directories. */
	inline dePathList &GetDirectories(){ return pDirectories; }
	inline const dePathList &GetDirectories() const{ return pDirectories; }
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
