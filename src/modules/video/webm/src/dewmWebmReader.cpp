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

#include "dewmWebmReader.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>


// Class dewmWebmReader
/////////////////////////

// Constructor, destructor
////////////////////////////

dewmWebmReader::dewmWebmReader( decBaseFileReader &reader ) :
pReader( reader ),
pLength( reader.GetLength() ),
pPosition( ( std::uint64_t )0 )
{
	reader.SetPosition( 0 );
}

dewmWebmReader::~dewmWebmReader(){
}



// Management
///////////////

webm::Status dewmWebmReader::Read( std::size_t num_to_read, std::uint8_t *buffer, std::uint64_t *num_actually_read ){
	if( pPosition + num_to_read <= pLength ){
		pReader.Read( buffer, ( int )num_to_read );
		pPosition += num_to_read;
		*num_actually_read = num_to_read;
		return webm::Status( webm::Status::Code::kOkCompleted );
		
	}else{
		*num_actually_read = ( std::uint64_t )0;
		return webm::Status( webm::Status::Code::kEndOfFile );
	}
}

webm::Status dewmWebmReader::Skip( std::uint64_t num_to_skip, std::uint64_t *num_actually_skipped ){
	if( pPosition + num_to_skip <= pLength ){
		pReader.MovePosition( ( int )num_to_skip );
		pPosition += num_to_skip;
		*num_actually_skipped = num_to_skip;
		return webm::Status( webm::Status::Code::kOkCompleted );
		
	}else{
		*num_actually_skipped = ( std::uint64_t )0;
		return webm::Status( webm::Status::Code::kEndOfFile );
	}
}

std::uint64_t dewmWebmReader::Position() const{
	return pPosition;
}

void dewmWebmReader::SetPosition( std::uint64_t position ){
	DEASSERT_TRUE( position < ( ( std::uint64_t )1 << 32 ) )
	pReader.SetPosition( ( int )position );
	pPosition = position;
}
