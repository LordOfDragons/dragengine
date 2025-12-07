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

#ifndef _SECONTROLLERTARGETLIST_H_
#define _SECONTROLLERTARGETLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class seControllerTarget;



/**
 * \brief Controller target List.
 */
class seControllerTargetList{
private:
	decPointerList pTargets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller target list. */
	seControllerTargetList();
	
	/** \brief Create copy of controller target list. */
	seControllerTargetList(const seControllerTargetList &copy);
	
	/** \brief Clean up controller target list. */
	~seControllerTargetList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controller targets. */
	int GetCount() const;
	
	/**
	 * \brief Controller target at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seControllerTarget *GetAt(int index) const;
	
	/**
	 * \brief Index of controller target or -1 if not present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	int IndexOf(seControllerTarget *target) const;
	
	/**
	 * \brief Determine if a controller target is present in the list.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	bool Has(seControllerTarget *target) const;
	
	/**
	 * \brief Add controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 */
	void Add(seControllerTarget *target);
	
	/**
	 * \brief Insert controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert(seControllerTarget *target, int index);
	
	/**
	 * \brief Move controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move(seControllerTarget *target, int index);
	
	/**
	 * \brief Remove controller target.
	 * \throws deeInvalidParam \em target is \em NULL.
	 * \throws deeInvalidParam \em target is not present in the list.
	 */
	void Remove(seControllerTarget *target);
	
	/** \brief Remove all controller targets. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller target list to this controller target list. */
	seControllerTargetList &operator=(const seControllerTargetList &list);
	/*@}*/
};

#endif
