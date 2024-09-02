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

#ifndef _PEETYPELIST_H_
#define _PEETYPELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class peeType;



/**
 * \brief Emitter Type List.
 */
class peeTypeList{
private:
	decObjectOrderedSet pTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new type list. */
	peeTypeList();
	/** \brief Clean up the type list. */
	~peeTypeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of types. */
	int GetCount() const;
	/** Retrieves the type at the given position. */
	peeType *GetAt( int index ) const;
	/** Retrieves the type with the given name or NULL if not found. */
	peeType *GetNamed( const char *name ) const;
	/** Retrieves the index of the given type or -1 if not found. */
	int IndexOf( peeType *type ) const;
	/** Retrieves the index of the type with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a type exists. */
	bool Has( peeType *type ) const;
	/** Determines if a type with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a type. */
	void Add( peeType *type );
	/** Inserts a new type. */
	void InsertAt( peeType *type, int index );
	/** Moves a type to a new position. */
	void MoveTo( peeType *type, int index );
	/** Removes a type. */
	void Remove( peeType *type );
	/** Removes all types. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	peeTypeList &operator=( const peeTypeList &list );
	/*@}*/
};

#endif
