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

#ifndef _PEECONTROLLERLIST_H_
#define _PEECONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class peeController;



/**
 * \brief Emitter Controller List.
 */
class peeControllerList{
private:
	decObjectOrderedSet pControllers;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new controller list. */
	peeControllerList();
	/** \brief Clean up the controller list. */
	~peeControllerList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of controllers. */
	int GetCount() const;
	/** Retrieves the controller at the given position. */
	peeController *GetAt( int index ) const;
	/** Retrieves the controller with the given name or NULL if not found. */
	peeController *GetNamed( const char *name ) const;
	/** Retrieves the index of the given controller or -1 if not found. */
	int IndexOf( peeController *controller ) const;
	/** Retrieves the index of the controller with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a controller exists. */
	bool Has( peeController *controller ) const;
	/** Determines if a controller with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a controller. */
	void Add( peeController *controller );
	/** Inserts a new controller. */
	void InsertAt( peeController *controller, int index );
	/** Moves a controller to a new position. */
	void MoveTo( peeController *controller, int index );
	/** Removes a controller. */
	void Remove( peeController *controller );
	/** Removes all controllers. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	peeControllerList &operator=( const peeControllerList &list );
	/*@}*/
};

#endif
