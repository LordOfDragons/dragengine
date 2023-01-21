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

#include <stdlib.h>

#include "decWeakFileReader.h"
#include "../exceptions.h"



// Class decWeakReader
////////////////////////

// Constructor, Destructor
////////////////////////////

decWeakFileReader::decWeakFileReader( decBaseFileReader *reader ) :
pReader( reader )
{
	DEASSERT_NOTNULL( reader );
}

decWeakFileReader::~decWeakFileReader(){
	DropReader();
}



// Management
///////////////

bool decWeakFileReader::IsValid() const{
	return pReader;
}

void decWeakFileReader::DropReader(){
	pReader = nullptr;
}



const char *decWeakFileReader::GetFilename(){
	return pReader->GetFilename();
}

int decWeakFileReader::GetLength(){
	return pReader->GetLength();
}

TIME_SYSTEM decWeakFileReader::GetModificationTime(){
	return pReader->GetModificationTime();
}

int decWeakFileReader::GetPosition(){
	return pReader->GetPosition();
}

void decWeakFileReader::SetPosition( int position ){
	pReader->SetPosition( position );
}

void decWeakFileReader::MovePosition( int offset ){
	pReader->MovePosition( offset );
}

void decWeakFileReader::SetPositionEnd( int position ){
	pReader->SetPositionEnd( position );
}

void decWeakFileReader::Read( void *buffer, int size ){
	pReader->Read( buffer, size );
}

decBaseFileReader::Ref decWeakFileReader::Duplicate(){
	return decBaseFileReader::Ref::New( new decWeakFileReader( pReader ) );
}
