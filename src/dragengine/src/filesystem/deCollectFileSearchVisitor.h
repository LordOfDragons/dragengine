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

#ifndef _DECOLLECTFILESEARCHVISITOR_H_
#define _DECOLLECTFILESEARCHVISITOR_H_

#include "dePathList.h"
#include "deFileSearchVisitor.h"


/**
 * \brief File search visitor collecting visited files if matching pattern.
 * 
 * Used with deVirtualFileSystem::SearchFiles() call. Stores visited files. Does recursion
 * into directories if desired. Empty pattern list matches all files. Special files are
 * not supported.
 */
class DE_DLL_EXPORT deCollectFileSearchVisitor : public deFileSearchVisitor{
private:
	dePathList pPatterns;
	bool pRecursive;
	dePathList pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file search visitor. */
	deCollectFileSearchVisitor();
	
	/** \brief Create file search visitor with pattern. */
	deCollectFileSearchVisitor( const char *pattern, bool recursive = false );
	
	/** \brief Clean up file search visitor. */
	virtual ~deCollectFileSearchVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of patterns to match. */
	inline dePathList &GetPatterns(){ return pPatterns; }
	inline const dePathList &GetPatterns() const{ return pPatterns; }
	
	/** \brief Add pattern. */
	void AddPattern( const char *pattern );
	
	/** \brief Enable recursion. */
	inline bool GetRecursive() const{ return pRecursive; }
	
	/** \brief Set if recursion is enabled. */
	void SetRecursive( bool recursive );
	
	/** \brief Found files. */
	inline dePathList &GetFiles(){ return pFiles; }
	inline const dePathList &GetFiles() const{ return pFiles; }
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
