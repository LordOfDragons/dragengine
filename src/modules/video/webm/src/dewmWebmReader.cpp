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
