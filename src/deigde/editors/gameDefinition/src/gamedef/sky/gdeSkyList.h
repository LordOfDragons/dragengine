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

#ifndef _GDESKYLIST_H_
#define _GDESKYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSky;



/**
 * \brief Sky list.
 */
class gdeSkyList{
private:
	decObjectOrderedSet pSkys;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky list. */
	gdeSkyList();
	
	/** \brief Create copy of sky list. */
	gdeSkyList(const gdeSkyList &list);
	
	/** \brief Clean up sky list. */
	~gdeSkyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skys. */
	int GetCount() const;
	
	/** \brief Sky at position. */
	gdeSky *GetAt(int index) const;
	
	/** \brief Sky with path or \em NULL if absent. */
	gdeSky *GetWithPath(const char *path) const;
	
	/** \brief Index of sky or -1 if absent. */
	int IndexOf(gdeSky *sky) const;
	
	/** \brief Index of  sky with path or -1 if absent. */
	int IndexOfWithPath(const char *path) const;
	
	/** \brief Sky is present. */
	bool Has(gdeSky *sky) const;
	
	/** \brief Sky with path is present. */
	bool HasWithPath(const char *path) const;
	
	/** \brief Add sky. */
	void Add(gdeSky *sky);
	
	/** \brief Remove sky. */
	void Remove(gdeSky *sky);
	
	/** \brief Remove all skys. */
	void RemoveAll();
	
	/** \brief Set from another sky list. */
	gdeSkyList &operator=(const gdeSkyList &list);
	/*@}*/
};

#endif
