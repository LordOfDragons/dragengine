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

#ifndef _GDESKYCONTROLLERLIST_H_
#define _GDESKYCONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSkyController;



/**
 * \brief Sky controller list.
 */
class gdeSkyControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller list. */
	gdeSkyControllerList();
	
	/** \brief Create copy of controller list. */
	gdeSkyControllerList(const gdeSkyControllerList &list);
	
	/** \brief Clean up controller list. */
	~gdeSkyControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/** \brief Controller at position. */
	gdeSkyController *GetAt(int index) const;
	
	/** \brief Named controller or \em NULL. */
	gdeSkyController *GetNamed(const char *name) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOf(gdeSkyController *controller) const;
	
	/** \brief Controller is present. */
	bool Has(gdeSkyController *controller) const;
	
	/** \brief Named controller is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add controller. */
	void Add(gdeSkyController *controller);
	
	/** \brief Remove controller. */
	void Remove(gdeSkyController *controller);
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	
	/** \brief Set from another controller list. */
	gdeSkyControllerList &operator=(const gdeSkyControllerList &list);
	/*@}*/
};

#endif
