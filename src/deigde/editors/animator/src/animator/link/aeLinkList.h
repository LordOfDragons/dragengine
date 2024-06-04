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

#ifndef _AELINKLIST_H_
#define _AELINKLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class aeLink;



/**
 * Link List.
 */
class aeLinkList{
private:
	decObjectOrderedSet pLinks;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new link list. */
	aeLinkList();
	
	/** Create copy of a link list. */
	aeLinkList( const aeLinkList &copy );
	
	/** Clean up link list. */
	~aeLinkList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Number of links. */
	int GetCount() const;
	
	/**
	 * Link at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeLink *GetAt( int index ) const;
	
	/** Named link. */
	aeLink *GetNamed( const char *name ) const;
	
	/**
	 * Index of link or -1 if not present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	int IndexOf( aeLink *link ) const;
	
	/**
	 * Determine if a link is present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	bool Has( aeLink *link ) const;
	
	/**
	 * Named link is present in the list.
	 */
	bool HasNamed( const char *name ) const;
	
	/**
	 * Add link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	void Add( aeLink *link );
	
	/**
	 * Insert link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( aeLink *link, int index );
	
	/**
	 * Move link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( aeLink *link, int index );
	
	/**
	 * Remove link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em link is not present in the list.
	 */
	void Remove( aeLink *link );
	
	/** Remove all links. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy link list to this link list. */
	aeLinkList &operator=( const aeLinkList &list );
	
	/** Add links from link list if not present in this link list. */
	aeLinkList &operator+=( const aeLinkList &list );
	/*@}*/
};

#endif
