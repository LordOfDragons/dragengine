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

#ifndef _CESAWORDLIST_H_
#define _CESAWORDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceSAWord;



/**
 * \brief Speech Animation Word List.
 */
class ceSAWordList{
private:
	decObjectOrderedSet pWords;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new word list. */
	ceSAWordList();
	/** Cleans up the word list. */
	~ceSAWordList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of words. */
	int GetCount() const;
	/** Retrieves the word at the given position. */
	ceSAWord *GetAt(int index) const;
	/** Retrieves the word with the given name or NULL if not found. */
	ceSAWord *GetNamed(const char *name) const;
	/** Retrieves the index of the given word or -1 if not found. */
	int IndexOf(ceSAWord *word) const;
	/** Retrieves the index of the word with the given name or -1 if not found. */
	int IndexOfNamed(const char *name) const;
	/** Determines if a word exists. */
	bool Has(ceSAWord *word) const;
	/** Determines if a word with the given name exists. */
	bool HasNamed(const char *name) const;
	/** Adds a word. */
	void Add(ceSAWord *word);
	/** Removes a word. */
	void Remove(ceSAWord *word);
	/** Removes all words. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceSAWordList &operator=(const ceSAWordList &list);
	/*@}*/
};

#endif
