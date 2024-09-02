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

#ifndef _IGDEFILEPATTERNLIST_H_
#define _IGDEFILEPATTERNLIST_H_

#include <dragengine/dragengine_export.h>

class igdeFilePattern;



/**
 * \brief File Pattern List.
 */
class DE_DLL_EXPORT igdeFilePatternList{
private:
	igdeFilePattern **pFilePatterns;
	int pFilePatternCount;
	int pFilePatternSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file pattern list. */
	igdeFilePatternList();
	
	/** \brief Create copy of a file pattern list. */
	igdeFilePatternList( const igdeFilePatternList &list );
	
	/** \brief Clean up file pattern list. */
	~igdeFilePatternList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of file patterns. */
	inline int GetFilePatternCount() const{ return pFilePatternCount; }
	
	/** \brief File pattern at the given position. */
	igdeFilePattern *GetFilePatternAt( int index ) const;
	
	/** \brief Index of the file pattern or -1 if not found. */
	int IndexOfFilePattern( igdeFilePattern *filePattern ) const;
	
	/** \brief Determines if a file pattern exists. */
	bool HasFilePattern( igdeFilePattern *filePattern ) const;
	
	/** \brief Adds a file pattern. */
	void AddFilePattern( igdeFilePattern *filePattern );
	
	/** \brief Removes a file pattern. */
	void RemoveFilePattern( igdeFilePattern *filePattern );
	
	/** \brief Removes all file patterns. */
	void RemoveAllFilePatterns();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeFilePatternList &operator=( const igdeFilePatternList &list );
	/*@}*/
};

#endif
