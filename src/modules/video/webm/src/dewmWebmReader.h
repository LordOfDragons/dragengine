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

#ifndef _DEWMWEBMREADER_H_
#define _DEWMWEBMREADER_H_

#include <webm/reader.h>

class decBaseFileReader;


/**
 * Bridges decBaseFileReader and WebM Reader class.
 */
class dewmWebmReader : public webm::Reader{
private:
	decBaseFileReader &pReader;
	const std::uint64_t pLength;
	std::uint64_t pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create reader. */
	dewmWebmReader ( decBaseFileReader &reader );
	
	/** Clean up reader. */
	virtual ~dewmWebmReader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Read data. */
	virtual webm::Status Read( std::size_t num_to_read, std::uint8_t *buffer, std::uint64_t *num_actually_read );
	
	/** Skip data. */
	virtual webm::Status Skip( std::uint64_t num_to_skip, std::uint64_t *num_actually_skipped );
	
	/** Read position. */
	virtual std::uint64_t Position() const;
	
	/** Set position from start of file. */
	void SetPosition( std::uint64_t position );
	/*@}*/
};

#endif
