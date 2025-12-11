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

#include "decWeakFileReader.h"
#include "../exceptions.h"



// Class decWeakReader
////////////////////////

// Constructor, Destructor
////////////////////////////

decWeakFileReader::decWeakFileReader(decBaseFileReader *reader) :
pReader(reader)
{
	DEASSERT_NOTNULL(reader);
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

void decWeakFileReader::SetPosition(int position){
	pReader->SetPosition(position);
}

void decWeakFileReader::MovePosition(int offset){
	pReader->MovePosition(offset);
}

void decWeakFileReader::SetPositionEnd(int position){
	pReader->SetPositionEnd(position);
}

void decWeakFileReader::Read(void *buffer, int size){
	pReader->Read(buffer, size);
}

decBaseFileReader::Ref decWeakFileReader::Duplicate(){
	return decWeakFileReader::Ref::NewWith(pReader);
}
