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

#ifndef _IGDEGDCCOMPONENTLIST_H_
#define _IGDEGDCCOMPONENTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCComponent;



/**
 * \brief Game Definition Class Component List.
 */
class DE_DLL_EXPORT igdeGDCComponentList{
private:
	decObjectOrderedSet pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCComponentList();
	
	/** \brief Create copy of a component list. */
	igdeGDCComponentList( const igdeGDCComponentList &list );
	
	/** \brief Clean up list. */
	~igdeGDCComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of components. */
	int GetCount() const;
	
	/** \brief Component by index. */
	igdeGDCComponent *GetAt( int index ) const;
	
	/** \brief Index of an component or -1 if not found. */
	int IndexOf( igdeGDCComponent *component ) const;
	
	/** \brief Determines if an component exists. */
	bool Has( igdeGDCComponent *component ) const;
	
	/** \brief Adds an component. */
	void Add( igdeGDCComponent *component );
	
	/** \brief Inserts a new component. */
	void InsertAt( igdeGDCComponent *component, int index );
	
	/** \brief Moves an component to a new position. */
	void MoveTo( igdeGDCComponent *component, int index );
	
	/** \brief Removes an component. */
	void Remove( igdeGDCComponent *component );
	
	/** \brief Removes all components. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCComponentList &list );
	
	/** \brief Set list from another list. */
	igdeGDCComponentList &operator=( const igdeGDCComponentList &list );
	/*@}*/
};

#endif
