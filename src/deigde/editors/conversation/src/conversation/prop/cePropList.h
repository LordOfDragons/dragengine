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

#ifndef _CEPROPLIST_H_
#define _CEPROPLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceProp;



/**
 * \brief Conversation prop list.
 */
class cePropList{
private:
	decObjectOrderedSet pProps;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new prop list. */
	cePropList();
	/** \brief Cleans up the prop list. */
	~cePropList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of props. */
	int GetCount() const;
	/** \brief Retrieves the prop at the given position. */
	ceProp *GetAt(int index) const;
	/** \brief Retrieves the index of the given prop or -1 if not found. */
	int IndexOf(ceProp *prop) const;
	/** \brief Determines if a prop exists. */
	bool Has(ceProp *prop) const;
	/** \brief Adds a prop. */
	void Add(ceProp *prop);
	/** \brief Removes a prop. */
	void Remove(ceProp *prop);
	/** \brief Removes all props. */
	void RemoveAll();
	
	/** \brief Sets the list from another list. */
	cePropList &operator=(const cePropList &list);
	/*@}*/
};

#endif
