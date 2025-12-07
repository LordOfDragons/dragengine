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

#ifndef _GDEOCSPEAKERLIST_H_
#define _GDEOCSPEAKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCSpeaker;



/**
 * \brief Object class speaker list.
 */
class gdeOCSpeakerList{
private:
	decObjectOrderedSet pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class speaker list. */
	gdeOCSpeakerList();
	
	/** \brief Create copy of object class speaker list. */
	gdeOCSpeakerList(const gdeOCSpeakerList &list);
	
	/** \brief Clean up object class speaker list. */
	~gdeOCSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class speakers. */
	int GetCount() const;
	
	/** \brief Object class speaker at position. */
	gdeOCSpeaker *GetAt(int index) const;
	
	/** \brief Index of named object class speaker or -1 if absent. */
	int IndexOf(gdeOCSpeaker *speaker) const;
	
	/** \brief Object class speaker is present. */
	bool Has(gdeOCSpeaker *speaker) const;
	
	/** \brief Add object class speaker. */
	void Add(gdeOCSpeaker *speaker);
	
	/** \brief Remove object class speaker. */
	void Remove(gdeOCSpeaker *speaker);
	
	/** \brief Remove all object class speakers. */
	void RemoveAll();
	
	/** \brief Set from another object class speaker list. */
	gdeOCSpeakerList &operator=(const gdeOCSpeakerList &list);
	/*@}*/
};

#endif
