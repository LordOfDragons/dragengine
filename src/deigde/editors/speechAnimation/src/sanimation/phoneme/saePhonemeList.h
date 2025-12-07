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

#ifndef _SAEPHONEMELIST_H_
#define _SAEPHONEMELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class saePhoneme;



/**
 * Speech Animation Phoneme List.
 */
class saePhonemeList{
private:
	decObjectOrderedSet pPhonemes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new phoneme list. */
	saePhonemeList();
	/** Cleans up the phoneme list. */
	~saePhonemeList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of phonemes. */
	int GetCount() const;
	/** Retrieves the phoneme at the given position. */
	saePhoneme *GetAt(int index) const;
	/** Retrieves the phoneme with the given ipa symbol or NULL if not found. */
	saePhoneme *GetIPA(int ipa) const;
	/** Retrieves the index of the given phoneme or -1 if not found. */
	int IndexOf(saePhoneme *phoneme) const;
	/** Retrieves the index of the phoneme with the given ipa symbol or -1 if not found. */
	int IndexOfIPA(int ipa) const;
	/** Determines if a phoneme exists. */
	bool Has(saePhoneme *phoneme) const;
	/** Determines if a phoneme with the given ipa symbol exists. */
	bool HasIPA(int ipa) const;
	/** Adds a phoneme. */
	void Add(saePhoneme *phoneme);
	/** Removes a phoneme. */
	void Remove(saePhoneme *phoneme);
	/** Removes all phonemes. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	saePhonemeList &operator=(const saePhonemeList &list);
	/*@}*/
};

#endif
