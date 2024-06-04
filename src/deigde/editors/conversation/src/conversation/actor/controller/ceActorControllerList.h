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

#ifndef _CEACTORCONTROLLERLIST_H_
#define _CEACTORCONTROLLERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorController;



/**
 * Actor controller list.
 */
class ceActorControllerList{
private:
	decObjectOrderedSet pControllers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller list. */
	ceActorControllerList();
	
	/** Create copy of controller list. */
	ceActorControllerList( const ceActorControllerList &list );
	
	/** Clean up controller list. */
	~ceActorControllerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of controllers. */
	int GetCount() const;
	
	/** Controller at index. */
	ceActorController *GetAt( int index ) const;
	
	/** Named controller or nullptr. */
	ceActorController *GetNamed( const char *name ) const;
	
	/** Index of controller or -1 if absent. */
	int IndexOf( ceActorController *controller ) const;
	
	/** Controller is present. */
	bool Has( ceActorController *controller ) const;
	
	/** Named controller is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add controller. */
	void Add( ceActorController *controller );
	
	/** Remove controller. */
	void Remove( ceActorController *controller );
	
	/** Remove all controller. */
	void RemoveAll();
	
	/** Set from another list. */
	ceActorControllerList &operator=( const ceActorControllerList &list );
	/*@}*/
};

#endif
