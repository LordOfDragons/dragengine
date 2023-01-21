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

#include "decWeakFileWriter.h"
#include "../exceptions.h"



// Class decWeakWriter
////////////////////////

// Constructor, Destructor
////////////////////////////

decWeakFileWriter::decWeakFileWriter( decBaseFileWriter *writer ) :
pWriter( writer )
{
	DEASSERT_NOTNULL( writer );
}

decWeakFileWriter::~decWeakFileWriter(){
	DropWriter();
}



// Management
///////////////

bool decWeakFileWriter::IsValid() const{
	return pWriter;
}

void decWeakFileWriter::DropWriter(){
	pWriter = nullptr;
}



const char *decWeakFileWriter::GetFilename(){
	return pWriter->GetFilename();
}

int decWeakFileWriter::GetPosition(){
	return pWriter->GetPosition();
}

void decWeakFileWriter::SetPosition( int position ){
	return pWriter->SetPosition( position );
}

void decWeakFileWriter::MovePosition( int offset ){
	pWriter->MovePosition( offset );
}

void decWeakFileWriter::SetPositionEnd( int position ){
	pWriter->SetPositionEnd( position );
}

void decWeakFileWriter::Write( const void *buffer, int size ){
	pWriter->Write( buffer, size );
}

decBaseFileWriter::Ref decWeakFileWriter::Duplicate(){
	return decBaseFileWriter::Ref::New( new decWeakFileWriter( pWriter ) );
}
