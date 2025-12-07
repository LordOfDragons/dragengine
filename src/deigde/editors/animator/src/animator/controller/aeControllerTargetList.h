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

#ifndef _AECONTROLLERTARGETLIST_H_
#define _AECONTROLLERTARGETLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class aeControllerTarget;



/**
 * Controller target List.
 */
class aeControllerTargetList{
private:
	decPointerList pTargets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new controller target list. */
	aeControllerTargetList();
	
	/** Create copy of a controller target list. */
	aeControllerTargetList(const aeControllerTargetList &copy);
	
	/** Clean up controller target list. */
	~aeControllerTargetList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Number of controller targets. */
	int GetCount() const;
	
	/**
	 * Controller target at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeControllerTarget *GetAt(int index) const;
	
	/**
	 * Index of controller target or -1 if not present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	int IndexOf(aeControllerTarget *target) const;
	
	/**
	 * Determine if a controller target is present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	bool Has(aeControllerTarget *target) const;
	
	/**
	 * Add controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	void Add(aeControllerTarget *target);
	
	/**
	 * Insert controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert(aeControllerTarget *target, int index);
	
	/**
	 * Move controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move(aeControllerTarget *target, int index);
	
	/**
	 * Remove controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em target is not present in the list.
	 */
	void Remove(aeControllerTarget *target);
	
	/** Remove all controller targets. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy controller target list to this controller target list. */
	aeControllerTargetList &operator=(const aeControllerTargetList &list);
	/*@}*/
};

#endif
