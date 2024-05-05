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

#ifndef _CECONTROLLERVALUELIST_H_
#define _CECONTROLLERVALUELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceControllerValue;



/**
 * \brief Controller Value List.
 */
class ceControllerValueList{
private:
	decObjectOrderedSet pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new controller value list. */
	ceControllerValueList();
	/** Cleans up the controller value list. */
	~ceControllerValueList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetCount() const;
	/** Retrieves the entry at the given position. */
	ceControllerValue *GetAt( int index ) const;
	/** Retrieves the entry with the given controller or -1 if not found. */
	ceControllerValue *GetNamed( const char *controller ) const;
	/** Retrieves the index of the entry or -1 if not found. */
	int IndexOf( ceControllerValue *entry ) const;
	/** Retrieves the index of the entry with the given controller or -1 if not found. */
	int IndexOfNamed( const char *controller ) const;
	/** Determines if an entry exists. */
	bool Has( ceControllerValue *entry ) const;
	/** Determines if an entry with the given controller exists. */
	bool HasNamed( const char *controller ) const;
	/** Adds an entry. */
	void Add( ceControllerValue *entry );
	/** Removes an entry. */
	void Remove( ceControllerValue *entry );
	/** Removes all entries. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Set this list to the content of another list. */
	ceControllerValueList &operator=( const ceControllerValueList &list );
	/*@}*/
};

#endif
