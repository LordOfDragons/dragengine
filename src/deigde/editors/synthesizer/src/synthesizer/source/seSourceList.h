/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCELIST_H_
#define _SESOURCELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seSource;



/**
 * \brief Source list.
 */
class seSourceList{
private:
	decObjectOrderedSet pSources;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source list. */
	seSourceList();
	
	/** \brief Create copy of source list. */
	seSourceList( const seSourceList &copy );
	
	/** \brief Clean up source list. */
	~seSourceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sources. */
	int GetCount() const;
	
	/**
	 * \brief Source at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seSource *GetAt( int index ) const;
	
	/**
	 * \brief Index of source or -1 if not present in the list.
	 * \throws deeInvalidParam \em source is \em NULL.
	 */
	int IndexOf( seSource *source ) const;
	
	/**
	 * \brief Determine if a source is present in the list.
	 * \throws deeInvalidParam \em source is \em NULL.
	 */
	bool Has( seSource *source ) const;
	
	/**
	 * \brief Add source.
	 * \throws deeInvalidParam \em source is \em NULL.
	 */
	void Add( seSource *source );
	
	/**
	 * \brief Insert source.
	 * \throws deeInvalidParam \em source is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( seSource *source, int index );
	
	/**
	 * \brief Move source.
	 * \throws deeInvalidParam \em source is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( seSource *source, int index );
	
	/**
	 * \brief Remove source.
	 * \throws deeInvalidParam \em source is \em NULL.
	 * \throws deeInvalidParam \em source is not present in the list.
	 */
	void Remove( seSource *source );
	
	/** \brief Remove all sources. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy source list to this source list. */
	seSourceList &operator=( const seSourceList &list );
	
	/** \brief Add sources from source list if not present in this source list. */
	seSourceList &operator+=( const seSourceList &list );
	/*@}*/
};

#endif
