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
