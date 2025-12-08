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

#ifndef _IGDEGDCSPEAKERLIST_H_
#define _IGDEGDCSPEAKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCSpeaker;



/**
 * \brief Game definition class speaker list.
 */
class DE_DLL_EXPORT igdeGDCSpeakerList{
private:
	decObjectOrderedSet pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCSpeakerList();
	
	/** \brief Create copy of a game definition class speaker list. */
	igdeGDCSpeakerList(const igdeGDCSpeakerList &list);
	
	/** \brief Clean up list. */
	~igdeGDCSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of speakers. */
	int GetCount() const;
	
	/** \brief Speaker by index. */
	igdeGDCSpeaker *GetAt(int index) const;
	
	/** \brief Index of an speaker or -1 if not found. */
	int IndexOf(igdeGDCSpeaker *speaker) const;
	
	/** \brief Determines if an speaker exists. */
	bool Has(igdeGDCSpeaker *speaker) const;
	
	/** \brief Adds an speaker. */
	void Add(igdeGDCSpeaker *speaker);
	
	/** \brief Inserts a new speaker. */
	void InsertAt(igdeGDCSpeaker *speaker, int index);
	
	/** \brief Moves an speaker to a new position. */
	void MoveTo(igdeGDCSpeaker *speaker, int index);
	
	/** \brief Removes an speaker. */
	void Remove(igdeGDCSpeaker *speaker);
	
	/** \brief Removes all speakers. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCSpeakerList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCSpeakerList &operator=(const igdeGDCSpeakerList &list);
	/*@}*/
};

#endif
