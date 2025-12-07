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

#include "deIesImageInfo.h"

#include <dragengine/common/file/decBaseFileReader.h>


// Class deIesImageInfo
/////////////////////////

// Constructor, destructor
////////////////////////////

deIesImageInfo::deIesImageInfo() :
pWidth(0),
pHeight(0),
pDepth(1),
pCurLine(0){
}

deIesImageInfo::~deIesImageInfo(){
}


// Management
///////////////

int deIesImageInfo::GetWidth(){
	return pWidth;
}

int deIesImageInfo::GetHeight(){
	return pHeight;
}

int deIesImageInfo::GetDepth(){
	return pDepth;
}

int deIesImageInfo::GetComponentCount(){
	return 1;
}

int deIesImageInfo::GetBitCount(){
	return 16; //32;
}



// Protected Functions
////////////////////////

void deIesImageInfo::pReadLines(decBaseFileReader &reader){
	const int length = reader.GetLength();
	decString content;
	content.Set(' ', length);
	reader.Read((char*)content.GetString(), length);
	
	int position = 0;
	while(position < length){
		const int deli = content.FindString("\r\n", position);
		if(deli == -1){
			break;
		}
		
		pLines.Add(content.GetMiddle(position, deli));
		position = deli + 2;
	}
	
	if(position < length){
		pLines.Add(content.GetMiddle(position));
	}
}
