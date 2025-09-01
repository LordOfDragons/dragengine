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

#ifndef _MEOBJECTLIST_H_
#define _MEOBJECTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meObject;



/**
 * @brief Object List.
 */
class meObjectList{
public:
	class Comparator{
	public:
		Comparator() = default;
		virtual int operator() (meObject &a, meObject &b) = 0;
	};
	
	class Visitor{
	public:
		Visitor() = default;
		virtual void operator() (meObject &object) = 0;
	};
	
	class Evaluator{
	public:
		Evaluator() = default;
		virtual bool operator() (meObject &object) = 0;
	};
	
private:
	decObjectOrderedSet pObjects;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectList();
	/** Cleans up the list. */
	~meObjectList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of objects. */
	int GetCount() const;
	/** Retrieves the object at the given position. */
	meObject *GetAt( int position ) const;
	/** Retrieves the index of the given object or -1 if not found. */
	int IndexOf( meObject *object ) const;
	/** Determines if the given object exists. */
	bool Has( meObject *object ) const;
	/** Adds an object throwing an exception if already existing. */
	void Add( meObject *object );
	/** Adds an object if not existing already. */
	void AddIfAbsent( meObject *object );
	/** Removes an object throwing an exception if not existing. */
	void Remove( meObject *object );
	/** Removes an object if existing. */
	void RemoveIfPresent( meObject *object );
	/** Removes all objects. */
	void RemoveAll();
	
	void Visit(Visitor &visitor, int from = 0, int to = -1, int step = 1) const;
	
	meObject *Find(Evaluator &evaluator, int from = 0, int to = -1, int step = 1) const;
	
	meObjectList Collect(Evaluator &evaluator, int from = 0, int to = -1, int step = 1) const;
	
	void RemoveIf(Evaluator &evaluator, int from = 0, int to = -1, int step = 1);
	
	void Sort(Comparator &comparator);
	
	meObjectList GetSorted(Comparator &comparator) const;
	
	/** Sets the list to the contain the same objects as another list. */
	meObjectList &operator=( const meObjectList &list );
	/*@}*/
};

#endif
