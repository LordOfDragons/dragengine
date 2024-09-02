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
