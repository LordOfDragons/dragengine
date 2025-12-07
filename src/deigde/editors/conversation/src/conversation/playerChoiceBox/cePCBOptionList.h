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

#ifndef _CEPCBOPTIONLIST_H_
#define _CEPCBOPTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePCBOption;



/**
 * \brief Player Choice Box Option List.
 */
class cePCBOptionList{
private:
	decObjectOrderedSet pOptions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new option list. */
	cePCBOptionList();
	/** Cleans up the option list. */
	~cePCBOptionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of options. */
	int GetCount() const;
	/** Retrieves the option at the given position. */
	cePCBOption *GetAt(int index) const;
	/** Retrieves the index of the given option or -1 if not found. */
	int IndexOf(cePCBOption *option) const;
	/** Determines if a option exists. */
	bool Has(cePCBOption *option) const;
	/** Adds a option. */
	void Add(cePCBOption *option);
	/** Inserts a new option. */
	void InsertAt(cePCBOption *option, int index);
	/** Moves a option to a new position. */
	void MoveTo(cePCBOption *option, int index);
	/** Removes a option. */
	void Remove(cePCBOption *option);
	/** Removes all options. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePCBOptionList &operator=(const cePCBOptionList &list);
	/*@}*/
};

#endif
