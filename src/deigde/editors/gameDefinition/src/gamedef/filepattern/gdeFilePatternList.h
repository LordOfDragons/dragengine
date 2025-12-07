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

#ifndef _GDEFILEPATTERNLIST_H_
#define _GDEFILEPATTERNLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeFilePattern;



/**
 * \brief File pattern list.
 */
class gdeFilePatternList{
private:
	decObjectOrderedSet pPatterns;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern list. */
	gdeFilePatternList();
	
	/** \brief Create copy of file pattern list. */
	gdeFilePatternList(const gdeFilePatternList &list);
	
	/** \brief Clean up file pattern list. */
	~gdeFilePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file patterns. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeFilePattern *GetAt(int index) const;
	
	/** \brief Index of named file pattern or -1 if absent. */
	int IndexOf(gdeFilePattern *pattern) const;
	
	/** \brief Object class is present. */
	bool Has(gdeFilePattern *pattern) const;
	
	/** \brief Add file pattern. */
	void Add(gdeFilePattern *pattern);
	
	/** \brief Remove file pattern. */
	void Remove(gdeFilePattern *pattern);
	
	/** \brief Remove all file patterns. */
	void RemoveAll();
	
	/** \brief Set from another file pattern list. */
	gdeFilePatternList &operator=(const gdeFilePatternList &list);
	/*@}*/
};

#endif
 
