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

#ifndef _GDEOBJECTCLASSLIST_H_
#define _GDEOBJECTCLASSLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeObjectClass;



/**
 * \brief Object class list.
 */
class gdeObjectClassList{
private:
	decObjectOrderedSet pObjectClasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class list. */
	gdeObjectClassList();
	
	/** \brief Create copy of object class list. */
	gdeObjectClassList( const gdeObjectClassList &list );
	
	/** \brief Clean up object class list. */
	~gdeObjectClassList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object classes. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeObjectClass *GetAt( int index ) const;
	
	/** \brief Named object class or \em NULL if absent. */
	gdeObjectClass *GetNamed( const char *name ) const;
	
	/** \brief Index of named object class or -1 if absent. */
	int IndexOf( gdeObjectClass *objectClass ) const;
	
	/** \brief Index of named object class or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeObjectClass *objectClass ) const;
	
	/** \brief Named object class is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add object class. */
	void Add( gdeObjectClass *objectClass );
	
	/** \brief Remove object class. */
	void Remove( gdeObjectClass *objectClass );
	
	/** \brief Remove all object classes. */
	void RemoveAll();
	
	/** \brief Set from another object class list. */
	gdeObjectClassList &operator=( const gdeObjectClassList &list );
	/*@}*/
};

#endif
