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

#ifndef _SESKYCONTROLLERLIST_H_
#define _SESKYCONTROLLERLIST_H_

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
	
	/** \brief Clean up controller list. */
	~seControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of controllers. */
	int GetCount() const;
	
	/** \brief Controller at index. */
	seController *GetAt( int index ) const;
	
	/** \brief Index of controller or -1 if absent. */
	int IndexOf( seController *controller ) const;
	
	/** \brief Controller is present. */
	bool Has( seController *controller ) const;
	
	/** \brief Add controller. */
	void Add( seController *controller );
	
	/** \brief Insert controller. */
	void InsertAt( seController *controller, int index );
	
	/** \brief Move controller. */
	void MoveTo( seController *controller, int index );
	
	/** \brief Remove controller. */
	void Remove( seController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seControllerList &operator=( const seControllerList &list );
	/*@}*/
};

#endif
