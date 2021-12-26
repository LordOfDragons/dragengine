/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	decUnicodeLineBuffer( int initialSize );
	
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
	void SetBufferSize( int bufferSize );
	
	/** \brief String at the given line with 0 being the most recent line. */
	const decUnicodeString *GetLineAt( int line ) const;
	
	/** \brief Adds a string to the end of the list and discards lines beyond the buffer size. */
	void AddLine( const decUnicodeString &line );
	
	/** \brief Clears the line buffer. */
	void Clear();
	
	/**
	 * \brief Convenience function for adding lines separated by newlines.
	 * 
	 * Breaks up the provided string into individual lines and adds them.
	 */
	void AddMultipleLines( const decUnicodeString &lines );
	
	/** \brief Fills a unicode string with all lines separated by newlines. */
	void FillLinesInto( decUnicodeString &string );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
