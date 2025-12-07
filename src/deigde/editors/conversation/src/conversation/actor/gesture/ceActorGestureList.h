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

#ifndef _CEACTORGESTURELIST_H_
#define _CEACTORGESTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorGesture;



/**
 * \brief Actor gesture list.
 */
class ceActorGestureList{
private:
	decObjectOrderedSet pGestures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gesture list. */
	ceActorGestureList();
	
	/** \brief Create gesture list. */
	ceActorGestureList(const ceActorGestureList &gesture);
	
	/** \brief Clean up gesture list. */
	~ceActorGestureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of gestures. */
	int GetCount() const;
	
	/** \brief Gesture at index. */
	ceActorGesture *GetAt(int index) const;
	
	/** \brief Named gesture or \em NULL if absent. */
	ceActorGesture *GetNamed(const char *name) const;
	
	/** \brief Index of gesture or -1 if absent. */
	int IndexOf(ceActorGesture *gesture) const;
	
	/** \brief Index of named gesture or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Gesture is present. */
	bool Has(ceActorGesture *gesture) const;
	
	/** \brief Named gesture is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add gesture. */
	void Add(ceActorGesture *gesture);
	
	/** \brief Remove gesture. */
	void Remove(ceActorGesture *gesture);
	
	/** \brief Remove all gestures. */
	void RemoveAll();
	
	/** \brief Set from another list. */
	ceActorGestureList &operator=(const ceActorGestureList &list);
	/*@}*/
};

#endif
