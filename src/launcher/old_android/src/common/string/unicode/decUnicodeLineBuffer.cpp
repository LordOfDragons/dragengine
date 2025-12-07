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



// class decUnicodeLineBuffer
///////////////////////////////

// constructor, destructor
////////////////////////////

decUnicodeLineBuffer::decUnicodeLineBuffer(int initialSize){
	pLines = NULL;
	pLineCount = 0;
	pBufferSize = 0;
	
	try{
		SetBufferSize(initialSize);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

decUnicodeLineBuffer::~decUnicodeLineBuffer(){
	if(pLines) delete [] pLines;
}



// management
///////////////

void decUnicodeLineBuffer::SetBufferSize(int bufferSize){
	if(bufferSize < 1) DETHROW(deeInvalidParam);
	decUnicodeString **newArray = NULL;
	int i, realBufferSize = bufferSize + 1;
	
	if(bufferSize == pBufferSize) return;
	
	try{
		newArray = new decUnicodeString*[realBufferSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		for(i=0; i<realBufferSize; i++) newArray[i] = NULL;
		
		for(i=pBufferSize; i<realBufferSize; i++){
			newArray[i] = new decUnicodeString;
			if(! newArray[i]) DETHROW(deeOutOfMemory);
		}
		
		if(realBufferSize < pLineCount){
			for(i=0; i<realBufferSize; i++){
				newArray[i] = pLines[i];
				pLines[i] = NULL;
			}
		}else{
			for(i=0; i<pLineCount; i++){
				newArray[i] = pLines[i];
				pLines[i] = NULL;
			}
		}
		
		if(pLines){
			for(i=0; i<pBufferSize; i++){
				if(pLines[i]) delete pLines[i];
			}
			delete [] pLines;
		}
		
	}catch(const deException &){
		if(newArray){
			for(i=0; i<bufferSize; i++){
				if(newArray[i]) delete newArray[i];
			}
			delete [] newArray;
		}
		throw;
	}
	
	pLines = newArray;
	pBufferSize = bufferSize;
	if(pLineCount > pBufferSize) pLineCount = pBufferSize;
}

const decUnicodeString *decUnicodeLineBuffer::GetLineAt(int index) const{
	if(index < 0 || index >= pLineCount) DETHROW(deeOutOfBoundary);
	return (const decUnicodeString *)pLines[index];
}

void decUnicodeLineBuffer::AddLine(const decUnicodeString &line){
	decUnicodeString *rotateLine;
	int i;
	
	if(pLineCount == pBufferSize){
		*pLines[ pLineCount ] = line;
		rotateLine = pLines[0];
		for(i=0; i<pBufferSize; i++) pLines[i] = pLines[i + 1];
		pLines[pBufferSize] = rotateLine;
		
	}else{
		*pLines[ pLineCount ] = line;
		pLineCount++;
	}
	
	if(pLineCount < pBufferSize) pLineCount++;
}

void decUnicodeLineBuffer::Clear(){
	pLineCount = 0;
}

void decUnicodeLineBuffer::AddMultipleLines(const decUnicodeString &lines){
	int i, start = 0, next, len = lines.GetLength();
	decUnicodeString *rotateLine;
	
	if(len > 0){
		while(start < len){
			next = lines.Find('\n', start);
			if(next == -1) next = len;
			
			if(pLineCount == pBufferSize){
				*pLines[ pLineCount ] = lines.GetMiddle( start, next );
				rotateLine = pLines[0];
				for(i=0; i<pBufferSize; i++) pLines[i] = pLines[i + 1];
				pLines[pBufferSize] = rotateLine;
				
			}else{
				*pLines[ pLineCount ] = lines.GetMiddle( start, next );
				pLineCount++;
			}
			
			start = next + 1;
		}
		
		if(lines.GetAt(len - 1) == '\n'){
			if(pLineCount == pBufferSize){
				pLines[pLineCount]->SetFromUTF8("");
				rotateLine = pLines[0];
				for(i=0; i<pBufferSize; i++) pLines[i] = pLines[i + 1];
				pLines[pBufferSize] = rotateLine;
				
			}else{
				pLines[pLineCount]->SetFromUTF8("");
				pLineCount++;
			}
		}
	}
}

void decUnicodeLineBuffer::FillLinesInto(decUnicodeString &string){
	int i;
	
	if(pLineCount == 0){
		string.SetFromUTF8("");
		
	}else{
		string = *pLines[0];
		for(i=1; i<pLineCount; i++){
			string.AppendCharacter('\n');
			string += *pLines[i];
		}
	}
}



// Private Functions
//////////////////////

void decUnicodeLineBuffer::pCleanUp(){
	int i;
	
	if(pLines){
		for(i=0; i<pBufferSize+1; i++){
			if(pLines[i]) delete pLines[i];
		}
		delete [] pLines;
	}
}
