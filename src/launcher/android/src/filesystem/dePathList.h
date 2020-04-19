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

#ifndef _DEPATHLIST_H_
#define _DEPATHLIST_H_

class decPath;


/**
 * \brief List of path.
 */
class dePathList{
private:
	decPath **pPath;
	int pPathCount;
	int pPathSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create path list. */
	dePathList();
	
	/** \brief Create path list. */
	dePathList( const dePathList &list );
	
	/** \brief Clean up path list. */
	~dePathList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of path. */
	inline int GetCount() const{ return pPathCount; }
	
	/** \brief Path at index. */
	const decPath &GetAt( int index ) const;
	
	/** \brief Index of the path or -1 if absent. */
	int IndexOf( const decPath &path ) const;
	
	/** \brief Path is present. */
	bool Has( const decPath &path ) const;
	
	/** \brief Add path. */
	void Add( const decPath &path );
	
	/** \brief Add path if absent. */
	void AddIfAbsent( const decPath &path );
	
	/** \brief Remove path. */
	void Remove( const decPath &path );
	
	/** \brief Remove path if present. */
	void RemoveIfPresent( const decPath &path );
	
	/** \brief Remove all path. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list. */
	dePathList &operator=( const dePathList &list );
	/*@}*/
};

#endif
