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

#ifndef _CEGESTURELIST_H_
#define _CEGESTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceGesture;



/**
 * \brief Gesture List.
 */
class ceGestureList{
private:
	decObjectOrderedSet pGestures;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new gesture list. */
	ceGestureList();
	/** Cleans up the gesture list. */
	~ceGestureList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of gestures. */
	int GetCount() const;
	/** Retrieves the gesture at the given position. */
	ceGesture *GetAt(int index) const;
	/** Retrieves the named gesture or NULL if not found. */
	ceGesture *GetNamed(const char *name) const;
	/** Retrieves the index of a gesture or -1 if not found. */
	int IndexOf(ceGesture *gesture) const;
	/** Retrieves the index of the named gesture or -1 if not found. */
	int IndexOfNamed(const char *name) const;
	/** Determines if a gesture exists. */
	bool Has(ceGesture *gesture) const;
	/** Determines if a named gesture exists. */
	bool HasNamed(const char *name) const;
	/** Adds a gesture. */
	void Add(ceGesture *gesture);
	/** Removes a gesture. */
	void Remove(ceGesture *gesture);
	/** Removes all gestures. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceGestureList &operator=(const ceGestureList &list);
	
	/** \brief Append gesture if absent. */
	ceGestureList &operator+=(const ceGestureList &list);
	/*@}*/
};

#endif
