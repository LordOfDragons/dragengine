/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _GDEOCWORLDLIST_H_
#define _GDEOCWORLDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCWorld;


/**
 * \brief Object class world list.
 */
class gdeOCWorldList{
private:
	decObjectOrderedSet pWorlds;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class world list. */
	gdeOCWorldList() = default;
	
	/** \brief Create copy of object class world list. */
	gdeOCWorldList(const gdeOCWorldList &list);
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of worlds. */
	int GetCount() const;
	
	/** \brief World at index. */
	gdeOCWorld *GetAt(int index) const;
	
	/** \brief Index of world or -1 if absent. */
	int IndexOf(gdeOCWorld *world) const;
	
	/** \brief World is present. */
	bool Has(gdeOCWorld *world) const;
	
	/** \brief Add world. */
	void Add(gdeOCWorld *world);
	
	/** \brief Remove world. */
	void Remove(gdeOCWorld *world);
	
	/** \brief Remove all worlds. */
	void RemoveAll();
	
	/** \brief Set from another world list. */
	gdeOCWorldList &operator=(const gdeOCWorldList &list);
	/*@}*/
};

#endif
