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

#ifndef _DEPATTERNLIST_H_
#define _DEPATTERNLIST_H_


/**
 * \brief List of file patterns.
 *
 * Manages a list of patterns used for searching files in virtual file
 * system containers. The patterns in the list are of the form '.X' where
 * X is any kind of extension including additional periods. There is no
 * '*' in front of the pattern.
 */
class dePatternList{
private:
	char **pPatterns;
	int pPatternCount;
	int pPatternSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new pattern list. */
	dePatternList();
	
	/** \brief Clean up pattern list. */
	~dePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of patterns. */
	inline int GetPatternCount() const{ return pPatternCount; }
	
	/** \brief Pattern at the given position. */
	const char *GetPatternAt(int index) const;
	
	/** \brief Index of the pattern or -1 if not found. */
	int IndexOfPattern(const char *pattern) const;
	
	/** \brief Determines if a pattern exists. */
	bool HasPattern(const char *pattern) const;
	
	/** \brief Adds a pattern. */
	void AddPattern(const char *pattern);
	
	/** \brief Removes a pattern. */
	void RemovePattern(const char *pattern);
	
	/** \brief Removes all pattern. */
	void RemoveAllPatterns();
	
	/** \brief Sort patterns by length from the longest pattern to the shortest one. */
	void SortPatternByLength();
	
	/** \brief Given path matches a pattern. */
	bool PathMatches(const char *path) const;
	
	/** \brief Index of the pattern matching the given path or -1 if none matches. */
	int IndexOfPatternMatchingPath(const char *path) const;
	/*@}*/
};

#endif
