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

#ifndef _GDEOCINHERITLIST_H_
#define _GDEOCINHERITLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCInherit;



/**
 * \brief Object class inherit list.
 */
class gdeOCInheritList{
private:
	decObjectOrderedSet pInheritObjectClasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inherit object class list. */
	gdeOCInheritList();
	
	/** \brief Create copy of inherit object class list. */
	gdeOCInheritList( const gdeOCInheritList &list );
	
	/** \brief Clean up inherit object class list. */
	~gdeOCInheritList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of inherit object classes. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCInherit *GetAt( int index ) const;
	
	/** \brief Index of named inherit object class or -1 if absent. */
	int IndexOf( gdeOCInherit *inherit ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeOCInherit *inherit ) const;
	
	/** \brief Add inherit object class. */
	void Add( gdeOCInherit *inherit );
	
	/** \brief Remove inherit object class. */
	void Remove( gdeOCInherit *inherit );
	
	/** \brief Remove all inherit object classes. */
	void RemoveAll();
	
	/** \brief Set from another inherit object class list. */
	gdeOCInheritList &operator=( const gdeOCInheritList &list );
	/*@}*/
};

#endif
