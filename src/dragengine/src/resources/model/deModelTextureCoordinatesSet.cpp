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

#include "deModelTextureCoordinatesSet.h"
#include "../../common/exceptions.h"



// Class deModelTextureCoordinatesSet
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deModelTextureCoordinatesSet::deModelTextureCoordinatesSet(){
	pTextureCoordinates = NULL;
	pTextureCoordinatesCount = 0;
}

deModelTextureCoordinatesSet::~deModelTextureCoordinatesSet(){
	if(pTextureCoordinates){
		delete [] pTextureCoordinates;
	}
}



// Management
///////////////

void deModelTextureCoordinatesSet::SetTextureCoordinatesCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pTextureCoordinates){
		delete [] pTextureCoordinates;
		pTextureCoordinates = NULL;
		pTextureCoordinatesCount = 0;
	}
	
	if(count > 0){
		pTextureCoordinates = new decVector2[count];
		pTextureCoordinatesCount = count;
	}
}

const decVector2 &deModelTextureCoordinatesSet::GetTextureCoordinatesAt(int index) const{
	if(index < 0 || index >= pTextureCoordinatesCount){
		DETHROW(deeInvalidParam);
	}
	
	return pTextureCoordinates[index];
}

void deModelTextureCoordinatesSet::SetTextureCoordinatesAt(int index, const decVector2 &textureCoordinates){
	if(index < 0 || index >= pTextureCoordinatesCount){
		DETHROW(deeInvalidParam);
	}
	
	pTextureCoordinates[index] = textureCoordinates;
}
