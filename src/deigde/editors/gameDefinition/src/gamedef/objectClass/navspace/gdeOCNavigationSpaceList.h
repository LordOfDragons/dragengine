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

#ifndef _GDEOCNAVIGATIONSPACELIST_H_
#define _GDEOCNAVIGATIONSPACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCNavigationSpace;



/**
 * \brief Object class navigation space list.
 */
class gdeOCNavigationSpaceList{
private:
	decObjectOrderedSet pNavigationSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class navigation space list. */
	gdeOCNavigationSpaceList();
	
	/** \brief Create copy of object class navigation space list. */
	gdeOCNavigationSpaceList( const gdeOCNavigationSpaceList &list );
	
	/** \brief Clean up object class navigation space list. */
	~gdeOCNavigationSpaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class navigation spaces. */
	int GetCount() const;
	
	/** \brief Object class navigation space at position. */
	gdeOCNavigationSpace *GetAt( int index ) const;
	
	/** \brief Index of named object class navigation space or -1 if absent. */
	int IndexOf( gdeOCNavigationSpace *space ) const;
	
	/** \brief Object class navigation space is present. */
	bool Has( gdeOCNavigationSpace *space ) const;
	
	/** \brief Add object class navigation space. */
	void Add( gdeOCNavigationSpace *space );
	
	/** \brief Remove object class navigation space. */
	void Remove( gdeOCNavigationSpace *space );
	
	/** \brief Remove all object class navigation spaces. */
	void RemoveAll();
	
	/** \brief Set from another object class navigation space list. */
	gdeOCNavigationSpaceList &operator=( const gdeOCNavigationSpaceList &list );
	/*@}*/
};

#endif
