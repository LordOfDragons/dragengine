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

#ifndef _SELINKLIST_H_
#define _SELINKLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seLink;



/**
 * \brief Link List.
 */
class seLinkList{
private:
	decObjectOrderedSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link list. */
	seLinkList();
	
	/** \brief Create copy of link list. */
	seLinkList( const seLinkList &copy );
	
	/** \brief Clean up link list. */
	~seLinkList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seLink *GetAt( int index ) const;
	
	/**
	 * \brief Index of link or -1 if not present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	int IndexOf( seLink *link ) const;
	
	/**
	 * \brief Determine if a link is present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	bool Has( seLink *link ) const;
	
	/**
	 * \brief Add link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	void Add( seLink *link );
	
	/**
	 * \brief Insert link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( seLink *link, int index );
	
	/**
	 * \brief Move link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( seLink *link, int index );
	
	/**
	 * \brief Remove link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em link is not present in the list.
	 */
	void Remove( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link list to this link list. */
	seLinkList &operator=( const seLinkList &list );
	
	/** \brief Add links from link list if not present in this link list. */
	seLinkList &operator+=( const seLinkList &list );
	/*@}*/
};

#endif
