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

#ifndef _AECONTROLLERLIST_H_
#define _AECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class aeController;



/**
 * Controller List.
 */
class aeControllerList{
private:
	decObjectOrderedSet pControllers;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new controller list. */
	aeControllerList();
	
	/** Create copy of a controller list. */
	aeControllerList(const aeControllerList &copy);
	
	/** Clean up controller list. */
	~aeControllerList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Number of controllers. */
	int GetCount() const;
	
	/**
	 * Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeController *GetAt(int index) const;
	
	/** Named controller. */
	aeController *GetNamed(const char *name) const;
	
	/**
	 * Index of controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 */
	int IndexOf(aeController *controller) const;
	
	/**
	 * Index of named controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 */
	int IndexOfNamed(const char *name) const;
	
	/**
	 * Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 */
	bool Has(aeController *controller) const;
	
	/**
	 * Determine if named controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 */
	bool HasNamed(const char *name) const;
	
	/**
	 * Add controller.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 */
	void Add(aeController *controller);
	
	/**
	 * Insert controller.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert(aeController *controller, int index);
	
	/**
	 * Move controller.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move(aeController *controller, int index);
	
	/**
	 * Remove controller.
	 * \throws deeInvalidParam \em controller is \em nullptr.
	 * \throws deeInvalidParam \em controller is not present in the list.
	 */
	void Remove(aeController *controller);
	
	/** Remove all controllers. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy controller list to this controller list. */
	aeControllerList &operator=(const aeControllerList &list);
	
	/** Add controllers from controller list if not present in this controller list. */
	aeControllerList &operator+=(const aeControllerList &list);
	/*@}*/
};

#endif
