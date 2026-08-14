/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deAvifIO.h"

#include <cstring>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>


// IO Callbacks
/////////////////

avifResult deAvifIO::ReadCallback(struct avifIO *io, uint32_t /*readFlags*/,
uint64_t offset, size_t size, avifROData *out){
	try{
		auto &self = *reinterpret_cast<deAvifIO*>(io->data);
		
		if((size_t)self.pBuffer.GetCount() < size){
			self.pBuffer.SetCountDiscard((int)size);
		}
		
		self.pReader.SetPosition((int)offset);
		self.pReader.Read(self.pBuffer.GetArrayPointer(), (int)size);
		
		out->data = self.pBuffer.GetArrayPointer();
		out->size = size;
		
		return AVIF_RESULT_OK;
		
	}catch(const deException &){
		return AVIF_RESULT_IO_ERROR;
	}
}

void deAvifIO::DestroyCallback(struct avifIO *io){
	if(io && io->data){
		reinterpret_cast<deAvifIO*>(io->data)->pBuffer.SetCountDiscard(0);
	}
}


// Constructor, destructor
////////////////////////////

deAvifIO::deAvifIO(decBaseFileReader &reader) :
pReader(reader),
pIO{}
{
	pIO.data = this;
	pIO.read = ReadCallback;
	pIO.destroy = DestroyCallback;
}

deAvifIO::~deAvifIO() = default;
