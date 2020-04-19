/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECUTF8DECODER_H_
#define _DECUTF8DECODER_H_



/**
 * @brief UTF8 Decoder.
 * Utility class for decoding a utf8 encoded character stream. Works with
 * simply a pointer to the character data so no copying is required.
 */
class decUTF8Decoder{
private:
	const char *pString;
	int pLength;
	int pPosition;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new utf8 decoder which is initially empty. */
	decUTF8Decoder();
	/** Frees the unicode string. */
	~decUTF8Decoder();
	/*@}*/
	
	/** @name Accessors and Mutators */
	/*@{*/
	/** Retrieves the string in use. */
	inline const char *GetString() const{ return pString; }
	/** Retrieves the position in the string. */
	inline int GetPosition() const{ return pPosition; }
	/** Retrieves the length of the string. */
	inline int GetLength() const{ return pLength; }
	/** Determines if the end of the string has been reached. */
	bool HasReachedEnd() const;
	/** Sets the encoder to use the given string and sets the position. */
	void SetString( const char *string );
	/** Sets the position. */
	void SetPosition( int position );
	/**
	 * Decodes the next unicode character from the string. If the string end has been
	 * reached or the encoded character is invalid -1 is returned. Otherwise the
	 * unicode value is of the character is returned.
	 */
	int DecodeNextCharacter();
	/*@}*/
};

#endif
