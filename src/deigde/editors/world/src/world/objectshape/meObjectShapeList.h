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

#ifndef _MEOBJECTSHAPELIST_H_
#define _MEOBJECTSHAPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class meObjectShape;
class decShapeList;



/**
 * @brief Object Shape List.
 */
class meObjectShapeList{
private:
	decObjectOrderedSet pObjectShapes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectShapeList();
	/** Cleans up the list. */
	~meObjectShapeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of object shapes. */
	int GetCount() const;
	/** Retrieves the object shape at the given position. */
	meObjectShape *GetAt( int position ) const;
	/** Retrieves the index of the given object shape or -1 if not found. */
	int IndexOf( meObjectShape *objectShape ) const;
	/** Determines if the given object shape exists. */
	bool Has( meObjectShape *objectShape ) const;
	/** Adds an object shape throwing an exception if already existing. */
	void Add( meObjectShape *objectShape );
	/** Adds an object shape if not existing already. */
	void AddIfAbsent( meObjectShape *objectShape );
	/** Removes an object shape throwing an exception if not existing. */
	void Remove( meObjectShape *objectShape );
	/** Removes an object shape if existing. */
	void RemoveIfPresent( meObjectShape *objectShape );
	/** Removes all object shapes. */
	void RemoveAll();
	
	/** Creates a shape list from this list. */
	void CreateShapeList( decShapeList &list ) const;
	/** Create property string from shape list. */
	void CreatePropertyString( decString &string ) const;
	
	/** Sets the list to the contain the same object shapes as another list. */
	meObjectShapeList &operator=( const meObjectShapeList &list );
	/*@}*/
};

#endif
