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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decNullFileWriter.h"
#include "../exceptions.h"



// Class decNullFileWriter
////////////////////////////

// Constructor, Destructor
////////////////////////////

decNullFileWriter::decNullFileWriter( const char *filename ) :
pFilename( filename ){
}

decNullFileWriter::~decNullFileWriter(){
}



// Seeking
////////////

int decNullFileWriter::GetPosition(){
	return 0;
}

void decNullFileWriter::SetPosition( int ){
}

void decNullFileWriter::MovePosition( int ){
}

void decNullFileWriter::SetPositionEnd( int ){
}



// Management
///////////////

const char *decNullFileWriter::GetFilename(){
	return pFilename;
}

void decNullFileWriter::Write( const void*, int ){
}

decBaseFileWriter::Ref decNullFileWriter::Duplicate(){
	return decBaseFileWriter::Ref::New( new decNullFileWriter( pFilename ) );
}
