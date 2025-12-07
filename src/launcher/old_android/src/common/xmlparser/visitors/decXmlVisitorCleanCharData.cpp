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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlVisitorCleanCharData.h"
#include "../decXmlContainer.h"
#include "../decXmlCharacterData.h"
#include "../../exceptions.h"



// Class decXmlVisitorCleanCharData
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlVisitorCleanCharData::decXmlVisitorCleanCharData() :
pIsFirstElement(false),
pIsLastElement(false){
}

decXmlVisitorCleanCharData::~decXmlVisitorCleanCharData(){
}



// Visiting
/////////////

void decXmlVisitorCleanCharData::VisitContainer(decXmlContainer &container){
	const int count = container.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		pIsFirstElement = i == 0;
		pIsLastElement = i == count - 1;
		container.GetElementAt(i)->Visit(*this);
	}
}

void decXmlVisitorCleanCharData::VisitCharacterData(decXmlCharacterData &data){
	const decString &orgData = data.GetData();
	if(orgData.IsEmpty()){
		return;
	}
	
	const int orgLen = orgData.GetLength();
	
	// create new buffer to hold data. it will be at most orgLen but mostly shorter.
	char * const newData = new char[orgLen + 1];
	bool hasSpace = false;
	int curPos = 0;
	int orgPos;
	
	// copy over data and replace consecutive spaces with one single space. spaces before and
	// after text are ignored if the character data is the first and/or last element
	try{
		for(orgPos=0; orgPos<orgLen; orgPos++){
			if(IsSpace(orgData[orgPos])){
				hasSpace = true;
				
			}else{
				if(hasSpace){
					hasSpace = false;
					if(!pIsFirstElement || curPos > 0){
						newData[curPos++] = ' ';
					}
				}
				
				newData[curPos++] = orgData[orgPos];
			}
		}
		
		if(hasSpace && !pIsLastElement){
			newData[curPos++] = ' ';
		}
		
		newData[curPos] = '\0';
		
		// store the new data and free the buffer
		data.SetData(newData);
		delete [] newData;
		
	}catch(const deException &){
		if(newData){
			delete [] newData;
		}
		throw;
	}
}



// Private functions
//////////////////////

bool decXmlVisitorCleanCharData::IsSpace(int character){
	switch(character){
	case 0x20:
	case 0x9:
	case 0xD:
	case 0xA:
		return true;
		
	default:
		return false;
	}
}
