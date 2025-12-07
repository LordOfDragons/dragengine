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

#ifndef _DECUTF8DECODER_H_
#define _DECUTF8DECODER_H_

#include "../../../dragengine_export.h"


/**
 * \brief UTF8 Decoder.
 * Utility class for decoding a utf8 encoded character stream. Works with
 * simply a pointer to the character data so no copying is required.
 */
class DE_DLL_EXPORT decUTF8Decoder{
private:
	const char *pString;
	int pLength;
	int pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new utf8 decoder which is initially empty. */
	decUTF8Decoder();
	
	/** \brief Frees the unicode string. */
	~decUTF8Decoder();
	/*@}*/
	
	
	
	/** \name Accessors and Mutators */
	/*@{*/
	/** \brief String in use. */
	inline const char *GetString() const{return pString;}
	
	/** \brief Position in the string. */
	inline int GetPosition() const{return pPosition;}
	
	/** \brief Length of the string. */
	inline int GetLength() const{return pLength;}
	
	/** \brief End of the string has been reached. */
	bool HasReachedEnd() const;
	
	/** \brief Set encoder to use the given string and sets the position. */
	void SetString(const char *string);
	
	/** \brief Set position. */
	void SetPosition(int position);
	
	/**
	 * \brief Decode next unicode character from the string.
	 * 
	 * If the string end has been reached or the encoded character is invalid -1 is returned.
	 * Otherwise the unicode value is of the character is returned.
	 */
	int DecodeNextCharacter();
	/*@}*/
};

#endif
