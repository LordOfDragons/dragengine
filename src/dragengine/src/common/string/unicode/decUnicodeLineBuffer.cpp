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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "decUnicodeLineBuffer.h"
#include "decUnicodeString.h"
#include "../../exceptions.h"


decUnicodeLineBuffer::decUnicodeLineBuffer(int initialSize) :
pLineCount(0),
pPosition(0)
{
	SetBufferSize(initialSize);
}

decUnicodeLineBuffer::~decUnicodeLineBuffer(){
}


// management
///////////////

int decUnicodeLineBuffer::GetBufferSize() const{
	return pLines.GetCount();
}

void decUnicodeLineBuffer::SetBufferSize(int bufferSize){
	DEASSERT_TRUE(bufferSize > 0)
	
	int oldSize = pLines.GetCount();
	if(oldSize == bufferSize){
		// nothing to do, keep ring state
		if(pPosition >= bufferSize){
			pPosition = 0;
		}
		if(pLineCount > bufferSize){
			pLineCount = bufferSize;
		}
		return;
	}
	
	// build a new list of the requested size and copy the newest
	// valid lines (up to bufferSize) from the current ring into it,
	// preserving the chronological order.
	decUnicodeStringList newLines;
	while(newLines.GetCount() < bufferSize){
		newLines.Add({});
	}
	
	if(oldSize > 0 && pLineCount > 0){
		int copyCount = pLineCount;
		if(copyCount > bufferSize){
			copyCount = bufferSize;
		}
		
		// Skip the oldest lines if we have more lines than buffer size
		const int skipCount = pLineCount - copyCount;
		
		int i;
		for(i=0; i<copyCount; i++){
			newLines.SetAt(i, pLines.GetAt((pPosition + skipCount + i) % oldSize));
		}
		
		pLineCount = copyCount;
		pPosition = 0;
		
	}else{
		// no valid lines
		pLineCount = 0;
		pPosition = 0;
	}
	
	pLines = std::move(newLines);
}

const decUnicodeString &decUnicodeLineBuffer::GetLineAt(int index) const{
	DEASSERT_TRUE(index >= 0 && index < pLineCount)
	
	const int size = pLines.GetCount();
	DEASSERT_TRUE(size > 0)
	
	return pLines.GetAt((pPosition + index) % size);
}

void decUnicodeLineBuffer::AddLine(const decUnicodeString &line){
	const int size = pLines.GetCount();
	DEASSERT_TRUE(size > 0)
	
	if(pLineCount < size){
		// append at tail
		pLines.SetAt((pPosition + pLineCount) % size, line);
		pLineCount++;
		
	}else{
		// overwrite oldest at pPosition and advance start
		pLines.SetAt(pPosition, line);
		pPosition = (pPosition + 1) % size;
		// pLineCount remains == size
	}
}

void decUnicodeLineBuffer::Clear(){
	pLineCount = 0;
	pPosition = 0;
}

void decUnicodeLineBuffer::AddMultipleLines(const decUnicodeString &lines){
	if(lines.IsEmpty()){
		return;
	}
	
	const int len = lines.GetLength();
	int start = 0;
	
	while(start < len){
		int next = lines.Find('\n', start);
		if(next == -1){
			next = len;
		}
		
		AddLine(lines.GetMiddle(start, next));
		start = next + 1;
	}
	
	// if the input ended with a newline, we must add an empty trailing line.
	if(lines.GetAt(len - 1) == '\n'){
		AddLine({});
	}
}

void decUnicodeLineBuffer::FillLinesInto(decUnicodeString &string){
	if(pLineCount == 0){
		string.Empty();
		return;
	}
	
	string = GetLineAt(0);
	int i;
	for(i=1; i<pLineCount; i++){
		string.AppendCharacter('\n');
		string += GetLineAt(i);
	}
}
