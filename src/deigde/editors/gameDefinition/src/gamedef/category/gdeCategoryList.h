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

#ifndef _GDECATEGORYLIST_H_
#define _GDECATEGORYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeCategory;



/**
 * \brief Category list.
 */
class gdeCategoryList{
private:
	decObjectOrderedSet pCategories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create category list. */
	gdeCategoryList();
	
	/** \brief Create copy of category list. */
	gdeCategoryList( const gdeCategoryList &list );
	
	/** \brief Clean up category list. */
	~gdeCategoryList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of categoryes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	gdeCategory *GetAt( int index ) const;
	
	/** \brief Named category or \em NULL if absent. */
	gdeCategory *GetNamed( const char *name ) const;
	
	/** \brief Category with path or \em NULL if absent. */
	gdeCategory *GetWithPath( const char *path ) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOf( gdeCategory *proeprty ) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Property is present. */
	bool Has( gdeCategory *proeprty ) const;
	
	/** \brief Named category is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add category. */
	void Add( gdeCategory *proeprty );
	
	/** \brief Remove category. */
	void Remove( gdeCategory *proeprty );
	
	/** \brief Remove all categoryes. */
	void RemoveAll();
	
	/** \brief Set from another category list. */
	gdeCategoryList &operator=( const gdeCategoryList &list );
	/*@}*/
};

#endif
