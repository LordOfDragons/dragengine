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

#ifndef _CECAPLAYERCHOICEOPTIONLIST_H_
#define _CECAPLAYERCHOICEOPTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCAPlayerChoiceOption;



/**
 * \brief Player Choice Option List.
 */
class ceCAPlayerChoiceOptionList{
private:
	decObjectOrderedSet pOptions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new option list. */
	ceCAPlayerChoiceOptionList();
	/** Cleans up the option list. */
	~ceCAPlayerChoiceOptionList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of options. */
	int GetCount() const;
	/** Retrieves the option at the given position. */
	ceCAPlayerChoiceOption *GetAt(int index) const;
	/** Retrieves the index of the given option or -1 if not found. */
	int IndexOf(ceCAPlayerChoiceOption *option) const;
	/** Determines if a option exists. */
	bool Has(ceCAPlayerChoiceOption *option) const;
	/** Adds a option. */
	void Add(ceCAPlayerChoiceOption *option);
	/** Inserts a new option. */
	void InsertAt(ceCAPlayerChoiceOption *option, int index);
	/** Moves a option to a new position. */
	void MoveTo(ceCAPlayerChoiceOption *option, int index);
	/** Removes a option. */
	void Remove(ceCAPlayerChoiceOption *option);
	/** Removes all options. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceCAPlayerChoiceOptionList &operator=(const ceCAPlayerChoiceOptionList &list);
	/*@}*/
};

#endif
