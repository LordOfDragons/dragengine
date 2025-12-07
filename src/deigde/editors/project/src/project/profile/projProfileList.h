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

#ifndef _PROJPROFILELIST_H_
#define _PROJPROFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class projProfile;



/**
 * \brief Profile list.
 */
class projProfileList{
private:
	decObjectOrderedSet pProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile list. */
	projProfileList();
	
	/** \brief Create copy of profile list. */
	projProfileList(const projProfileList &copy);
	
	/** \brief Clean up profile list. */
	~projProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of profilees. */
	int GetCount() const;
	
	/** \brief Profile at position. */
	projProfile *GetAt(int index) const;
	
	/** \brief Named profile or \em NULL if absent. */
	projProfile *GetNamed(const char *name) const;
	
	/** \brief Index of named profile or -1 if absent. */
	int IndexOf(projProfile *profile) const;
	
	/** \brief Index of named profile or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Profile is present. */
	bool Has(projProfile *profile) const;
	
	/** \brief Named profile is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add profile. */
	void Add(projProfile *profile);
	
	/** \brief Remove profile. */
	void Remove(projProfile *profile);
	
	/** \brief Remove all profilees. */
	void RemoveAll();
	
	/** \brief Set from another profile list. */
	projProfileList &operator=(const projProfileList &list);
	/*@}*/
};

#endif
