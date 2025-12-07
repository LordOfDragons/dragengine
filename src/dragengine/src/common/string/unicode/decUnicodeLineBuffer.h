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

#ifndef _DECUNICODELINEBUFFER_H_
#define _DECUNICODELINEBUFFER_H_

#include "../../../dragengine_export.h"

class decUnicodeString;


/**
 * \brief Manages a line buffer of unicode string.
 * Unicode buffer lists are useful for logging purpose or any other
 * situation where holding a number of text lines in memory with easy
 * line rotation. New unicode strings are added to the end of the list.
 * If the number of lines increases above a given threshold all lines
 * beyond are removed. Best example for this are consoles which hold
 * the most recent lines.
 */
class DE_DLL_EXPORT decUnicodeLineBuffer{
private:
	decUnicodeString **pLines;
	int pLineCount, pBufferSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new line buffer object. */
	decUnicodeLineBuffer(int initialSize);
	
	/** \brief Frees the line buffer object. */
	~decUnicodeLineBuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of lines in the buffer. */
	inline int GetLineCount() const{ return pLineCount; }
	
	/**
	 * \brief Size of the line buffer.
	 * 
	 * All lines beyond this limit are discarded automatically.
	 */
	inline int GetBufferSize() const{ return pBufferSize; }
	
	/**
	 * \brief Set the size of the line buffer.
	 * 
	 * All lines beyond this limit are discarded automatically.
	 */
	void SetBufferSize(int bufferSize);
	
	/** \brief String at the given line with 0 being the most recent line. */
	const decUnicodeString *GetLineAt(int line) const;
	
	/** \brief Adds a string to the end of the list and discards lines beyond the buffer size. */
	void AddLine(const decUnicodeString &line);
	
	/** \brief Clears the line buffer. */
	void Clear();
	
	/**
	 * \brief Convenience function for adding lines separated by newlines.
	 * 
	 * Breaks up the provided string into individual lines and adds them.
	 */
	void AddMultipleLines(const decUnicodeString &lines);
	
	/** \brief Fills a unicode string with all lines separated by newlines. */
	void FillLinesInto(decUnicodeString &string);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
