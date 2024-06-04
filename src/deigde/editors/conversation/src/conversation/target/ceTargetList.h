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

#ifndef _CETARGETLIST_H_
#define _CETARGETLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceTarget;



/**
 * \brief Target List.
 */
class ceTargetList{
private:
	decObjectOrderedSet pTargets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new target list. */
	ceTargetList();
	/** Cleans up the target list. */
	~ceTargetList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of targets. */
	int GetCount() const;
	/** Retrieves the target at the given position. */
	ceTarget *GetAt( int index ) const;
	/** Retrieves the named target or NULL if not found. */
	ceTarget *GetNamed( const char *name ) const;
	/** Retrieves the index of a target or -1 if not found. */
	int IndexOf( ceTarget *target ) const;
	/** Retrieves the index of the named target or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a target exists. */
	bool Has( ceTarget *target ) const;
	/** Determines if a named target exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a target. */
	void Add( ceTarget *target );
	/** Removes a target. */
	void Remove( ceTarget *target );
	/** Removes all targets. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceTargetList &operator=( const ceTargetList &list );
	
	/** \brief Add targets to list if absent. */
	ceTargetList &operator+=( const ceTargetList &list );
	/*@}*/
};

#endif
