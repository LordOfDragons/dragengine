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

#ifndef _IGDEGDSKYLIST_H_
#define _IGDEGDSKYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDSky;



/**
 * \brief Game definition sky list.
 */
class DE_DLL_EXPORT igdeGDSkyList{
private:
	decObjectOrderedSet pSkies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDSkyList();
	
	/** \brief Clean up list. */
	~igdeGDSkyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skies. */
	int GetCount() const;
	
	/** \brief Sky by index. */
	igdeGDSky *GetAt(int index) const;
	
	/** \brief Sky with path or NULL if absent. */
	igdeGDSky *GetWithPath(const char *path) const;
	
	/** \brief Sky with name or NULL if absent. */
	igdeGDSky *GetWithName(const char *name) const;
	
	/** \brief Index of sky or -1 if absent. */
	int IndexOf(igdeGDSky *sky) const;
	
	/** \brief Index of sky with path or -1 if absent. */
	int IndexOfWithPath(const char *path) const;
	
	/** \brief Index of sky with name or -1 if absent. */
	int IndexOfWithName(const char *name) const;
	
	/** \brief Index of sky with name or -1 if absent. */
	int IndexOfWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Sky is present. */
	bool Has(igdeGDSky *sky) const;
	
	/** \brief Sky with path is present. */
	bool HasWithPath(const char *path) const;
	
	/** \brief Sky with name is present. */
	bool HasWithName(const char *name) const;
	
	/** \brief Sky with name or path is present. */
	bool HasWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Add sky. */
	void Add(igdeGDSky *sky);
	
	/** \brief Insert sky. */
	void InsertAt(igdeGDSky *sky, int index);
	
	/** \brief Move sky to new position. */
	void MoveTo(igdeGDSky *sky, int index);
	
	/** \brief Remove sky. */
	void Remove(igdeGDSky *sky);
	
	/** \brief Remove all skies. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	igdeGDSkyList &operator=(const igdeGDSkyList &list);
	/*@}*/
};

#endif
