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

#include "deoglModelLODTexCoordSet.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelLODTexCoordSet
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelLODTexCoordSet::deoglModelLODTexCoordSet(){
	pTexCoords = NULL;
	pTexCoordCount = 0;
	
	pTangents = NULL;
	pNegateTangents = NULL;
	pTangentCount = 0;
}

deoglModelLODTexCoordSet::~deoglModelLODTexCoordSet(){
	pCleanUp();
}



// Management
///////////////

void deoglModelLODTexCoordSet::SetTextureCoordinateCount(int count, bool copyContent){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	decVector2 *texCoords = NULL;
	
	if(count > 0){
		texCoords = new decVector2[count];
		
		if(copyContent && pTexCoords){
			int copyCount = count;
			
			if(copyCount > pTexCoordCount){
				copyCount = pTexCoordCount;
			}
			
			memcpy(texCoords, pTexCoords, sizeof(decVector2) * copyCount);
		}
	}
	
	if(pTexCoords){
		delete [] pTexCoords;
	}
	
	pTexCoords = texCoords;
	pTexCoordCount = count;
}

void deoglModelLODTexCoordSet::SetTangentCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	decVector *tangents = NULL;
	bool *negateTangent = NULL;
	
	if(count > 0){
		tangents = new decVector[count];
		negateTangent = new bool[count];
	}
	
	if(pTangents){
		delete [] pTangents;
	}
	if(pNegateTangents){
		delete [] pNegateTangents;
	}
	
	pTangents = tangents;
	pNegateTangents = negateTangent;
	
	pTangentCount = count;
}



// Private functions
//////////////////////

void deoglModelLODTexCoordSet::pCleanUp(){
	if(pNegateTangents){
		delete [] pNegateTangents;
	}
	if(pTangents){
		delete [] pTangents;
	}
	if(pTexCoords){
		delete [] pTexCoords;
	}
}
