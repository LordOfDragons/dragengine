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

#ifndef _SEPROPERTYLIST_H_
#define _SEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seProperty;



/**
 * @brief Property List.
 */
class sePropertyList{
private:
	decObjectOrderedSet pProperties;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new property list. */
	sePropertyList();
	/** Cleans up the property list. */
	~sePropertyList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of properties. */
	int GetCount() const;
	/** Retrieves the property at the given position. */
	seProperty *GetAt( int index ) const;
	/** Retrieves the property with the given name or NULL if not found. */
	seProperty *GetNamed( const char *name ) const;
	/** Retrieves the index of the given property or -1 if not found. */
	int IndexOf( seProperty *property ) const;
	/** Retrieves the index of the property with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a property exists. */
	bool Has( seProperty *property ) const;
	/** Determines if a property with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a property. */
	void Add( seProperty *property );
	/** Inserts a new property. */
	void InsertAt( seProperty *property, int index );
	/** Moves a property to a new position. */
	void MoveTo( seProperty *property, int index );
	/** Removes a property. */
	void Remove( seProperty *property );
	/** Removes all properties. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	sePropertyList &operator=( const sePropertyList &list );
	/*@}*/
};

#endif
