/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
