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

#ifndef _SECONTROLLERLIST_H_
#define _SECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seController;



/**
 * \brief Controller list.
 */
class seControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller list. */
	seControllerList();
	
	/** \brief Create copy of controller list. */
	seControllerList(const seControllerList &copy);
	
	/** \brief Clean up controller list. */
	~seControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seController *GetAt(int index) const;
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seController *GetNamed(const char *name) const;
	
	/**
	 * \brief Index of controller or -1 if not present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	int IndexOf(seController *controller) const;
	
	/**
	 * \brief Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool Has(seController *controller) const;
	
	/**
	 * \brief Determine if a controller is present in the list.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	bool HasNamed(const char *name) const;
	
	/**
	 * \brief Add controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 */
	void Add(seController *controller);
	
	/**
	 * \brief Insert controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert(seController *controller, int index);
	
	/**
	 * \brief Move controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move(seController *controller, int index);
	
	/**
	 * \brief Remove controller.
	 * \throws deeInvalidParam \em controller is \em NULL.
	 * \throws deeInvalidParam \em controller is not present in the list.
	 */
	void Remove(seController *controller);
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller list to this controller list. */
	seControllerList &operator=(const seControllerList &list);
	
	/** \brief Add controllers from controller list if not present in this controller list. */
	seControllerList &operator+=(const seControllerList &list);
	/*@}*/
};

#endif
